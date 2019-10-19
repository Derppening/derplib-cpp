namespace derplib {

template<typename Func>
template<typename Rep, typename Period>
timer<Func>::timer(std::chrono::duration<Rep, Period> duration, const Func& callback) :
    timer(duration, std::move(callback), duration) {}

template<typename Func>
template<typename Rep, typename Period>
timer<Func>::timer(std::chrono::duration<Rep, Period> duration, const Func& callback, std::chrono::duration<Rep, Period> poll_rate) :
    _state(state::not_started),
    _keep_alive(true),
    _duration(std::chrono::nanoseconds(duration)),
    _poll_rate(std::chrono::nanoseconds(poll_rate)),
    _callback(callback) {}

template<typename Func>
timer<Func>::timer(const timer<Func>& other) :
    timer(other._duration, other._callback, other._poll_rate) {}

template<typename Func>
timer<Func>& timer<Func>::operator=(const timer& other) {
  if (this != &other) {
    _state = state::not_started;
    _keep_alive = true;
    _duration = other._duration;
    _poll_rate = other._poll_rate;
    _callback = other._callback;
  }

  return *this;
}

template<typename Func>
timer<Func>::~timer() {
  _keep_alive = false;

  if (_thread.joinable()) {
    _thread.join();
  }
}

template<typename Func>
void timer<Func>::start() {
  if (_state == state::active) {
    throw std::logic_error("Attempted to start a running timer");
  } else if (_state == state::expired) {
    throw std::logic_error("Attempted to restart an expired timer");
  }

  _state = state::active;
  _end_time = std::chrono::steady_clock::now() + _duration;
  _thread = std::thread(&timer::countdown_daemon, this);
}

template<typename Func>
void timer<Func>::stop() {
  if (_state == state::not_started) {
    return;
  }
  if (_state != state::active) {
    throw std::logic_error("Attempted to stop a halted timer");
  }

  _keep_alive = false;

  if (_thread.joinable()) {
    _thread.join();
  }

  _thread = std::thread();
}

template<typename Func>
bool timer<Func>::active() const {
  return _state == state::active;
}

template<typename Func>
bool timer<Func>::expired() const {
  return _state == state::expired;
}

template<typename Func>
void timer<Func>::countdown_daemon() {
  _state = state::active;

  while (std::chrono::steady_clock::now() < _end_time) {
    if (!_keep_alive) {
      _state = state::halted;

      return;
    }

    std::this_thread::sleep_for(_poll_rate);
  }

  _state = state::expired;
  if (_callback) {
    _callback();
  }
}

}  // namespace derplib

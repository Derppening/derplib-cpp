namespace derplib {
namespace container {

template<typename InT, typename ConsumerT>
cfq_parallel_consumer<InT, ConsumerT>::cfq_parallel_consumer(const std::size_t concurrency,
                                                             const consumer_type& consumer) :
    _consumer(consumer),
    _keep_alive{true},
    _buffers{concurrency},
    _threads{concurrency},
    _mutexes{concurrency},
    _cvs{concurrency} {
  for (std::size_t i = 0; i < _threads.size(); ++i) {
    _threads[i] = std::thread(&cfq_parallel_consumer::daemon, this, i);
  }
}

template<typename InT, typename ConsumerT>
cfq_parallel_consumer<InT, ConsumerT>::cfq_parallel_consumer(const std::size_t concurrency, consumer_type&& consumer) :
    _consumer(consumer),
    _keep_alive{true},
    _buffers{concurrency},
    _threads{concurrency},
    _mutexes{concurrency},
    _cvs{concurrency} {
  for (std::size_t i = 0; i < _threads.size(); ++i) {
    _threads[i] = std::thread(&cfq_parallel_consumer::daemon, this, i);
  }
}

template<typename InT, typename ConsumerT>
cfq_parallel_consumer<InT, ConsumerT>::~cfq_parallel_consumer() {
  _keep_alive = false;

  for (auto& cv : _cvs) {
    cv.notify_all();
  }

  for (auto& thread : _threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

template<typename InT, typename ConsumerT>
void cfq_parallel_consumer<InT, ConsumerT>::push(const stdext::decay_t<InT>& value) {
  std::size_t n = std::numeric_limits<std::size_t>::max();
  std::size_t it = std::numeric_limits<std::size_t>::max();

  for (std::size_t i = 0; i < _buffers.size(); ++i) {
    std::size_t size = _buffers[i].size();
    if (n > size) {
      it = i;
      n = size;
    }
  }

  std::lock_guard<std::mutex> lk(_mutexes[it]);
  _buffers[it].push_back(value);
}

template<typename InT, typename ConsumerT>
void cfq_parallel_consumer<InT, ConsumerT>::push(stdext::decay_t<InT>&& value) {
  std::size_t it = std::numeric_limits<std::size_t>::max();

  for (std::size_t i = 0, n = std::numeric_limits<std::size_t>::max(); i < _buffers.size(); ++i) {
    std::size_t size = _buffers[i].size();
    if (n > size) {
      it = i;
      n = size;
    }
  }

  std::unique_lock<std::mutex> lk(_mutexes[it]);
  _buffers[it].push_back(value);

  lk.unlock();
  _cvs[it].notify_one();
}

template<typename InT, typename ConsumerT>
template<typename... Args>
void cfq_parallel_consumer<InT, ConsumerT>::emplace(Args&&... args) {
  std::size_t n = std::numeric_limits<std::size_t>::max();
  std::size_t it = std::numeric_limits<std::size_t>::max();

  for (std::size_t i = 0; i < _buffers.size(); ++i) {
    std::size_t size = _buffers[i].size();
    if (n > size) {
      it = i;
      n = size;
    }
  }

  std::lock_guard<std::mutex> lk(_mutexes[it]);
  _buffers[it].emplace_back(std::forward<Args>(args)...);
}

template<typename InT, typename ConsumerT>
void cfq_parallel_consumer<InT, ConsumerT>::daemon(const std::size_t i) {
  while (_keep_alive || !_buffers[i].empty()) {
    std::unique_lock<std::mutex> lk(_mutexes[i]);
    _cvs[i].wait(lk, [&] { return !_keep_alive || !_buffers[i].empty(); });

    if (!_keep_alive && _buffers[i].empty()) {
      break;
    }

    _consumer(_buffers[i].front());
    _buffers[i].pop_front();

    lk.unlock();
    _cvs[i].notify_one();
  }
}

}  // namespace container
}  // namespace derplib

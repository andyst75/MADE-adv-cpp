namespace task {

#include <stdexcept>

/**
 * UniquePtr
 */
template <typename T>
UniquePtr<T>::~UniquePtr() {
  delete ptr;
}

template <typename T>
UniquePtr<T>::UniquePtr(T* ptr) : ptr(ptr) {}

template <typename T>
UniquePtr<T>::UniquePtr(UniquePtr<T>&& ptr) noexcept : ptr(ptr.release()) {}

template <typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr<T>&& ptr) noexcept {
  if (this != &ptr) {
    delete this->ptr;
    this->ptr = ptr.ptr;
    ptr.ptr = nullptr;
  }
  return *this;
}

template <typename T>
T& UniquePtr<T>::operator*() const {
  if (ptr == nullptr) {
    throw std::runtime_error{"Can't reference on nullptr"};
  }
  return *ptr;
}

template <typename T>
T* UniquePtr<T>::operator->() const {
  return ptr;
}

template <typename T>
T* UniquePtr<T>::get() {
  return ptr;
}

template <typename T>
T* UniquePtr<T>::release() {
  T* ptr = nullptr;
  std::swap(ptr, this->ptr);
  return ptr;
}

template <typename T>
void UniquePtr<T>::reset(T* ptr) {
  if (ptr != nullptr) {
    delete this->ptr;
  }
  this->ptr = ptr;
}

template <typename T>
void UniquePtr<T>::swap(UniquePtr& other) noexcept {
  std::swap(ptr, other);
}

/**
 * SmartCounter
 */
template <typename T>
SmartCounter<T>::SmartCounter() = default;

template <typename T>
SmartCounter<T>::SmartCounter(T* ptr, long shared_cnt, long weak_cnt)
    : ptr(ptr), shared_cnt(shared_cnt), weak_cnt(weak_cnt) {}

template <typename T>
SmartCounter<T>::~SmartCounter() {
  if (shared_cnt == 0 && weak_cnt == 0 && ptr != nullptr) {
    delete ptr;
  }
}

/**
 * SharedPtr
 */
template <typename T>
SharedPtr<T>::~SharedPtr() {
  if (counter != nullptr) {
    --counter->shared_cnt;
    if (counter->shared_cnt == 0) {
      if (counter->ptr != nullptr) {
        delete counter->ptr;
      }
      counter->ptr = nullptr;
    }
  }
}

template <typename T>
SharedPtr<T>::SharedPtr() : counter(new SmartCounter<T>()) {}

template <typename T>
SharedPtr<T>::SharedPtr(T* ptr) : counter(new SmartCounter<T>(ptr)) {}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other) noexcept
    : counter(other.counter) {
  if (counter != nullptr) {
    ++counter->shared_cnt;
  } else {
    counter = new SmartCounter<T>();
  }
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& other) noexcept : counter(other.counter) {
  other.counter = nullptr;
}

template <typename T>
SharedPtr<T>::SharedPtr(const WeakPtr<T>& other) : counter(other.counter) {
  if (counter != nullptr) {
    ++counter->shared_cnt;
  } else {
    counter = new SmartCounter<T>();
  }
}

template <typename T>
T* SharedPtr<T>::get() const noexcept {
  return counter == nullptr ? nullptr : counter->ptr;
}

template <typename T>
long SharedPtr<T>::use_count() const noexcept {
  return counter == nullptr ? 0 : counter->shared_cnt;
}

template <typename T>
void SharedPtr<T>::reset(T* ptr) {
  SharedPtr<T>(ptr).swap(*this);
}

template <typename T>
void SharedPtr<T>::swap(SharedPtr& other) noexcept {
  std::swap(counter, other.counter);
}

template <typename T>
T& SharedPtr<T>::operator*() const noexcept {
  return *get();
}

template <typename T>
T* SharedPtr<T>::operator->() const noexcept {
  return get();
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& other) noexcept {
  SharedPtr<T>(other).swap(*this);
  return *this;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& other) noexcept {
  SharedPtr<T>(std::move(other)).swap(*this);
  return *this;
}

/**
 * WeakPtr
 */
template <typename T>
WeakPtr<T>::WeakPtr() = default;

template <typename T>
WeakPtr<T>::WeakPtr(const WeakPtr& other) noexcept
    : counter(new SmartCounter<T>(other.counter->ptr, 0, 1)) {}

template <typename T>
WeakPtr<T>::WeakPtr(WeakPtr&& other) noexcept : counter(other.counter) {
  other.counter = nullptr;
}

template <typename T>
WeakPtr<T>::WeakPtr(const SharedPtr<T>& other) noexcept
    : counter(other.counter) {
  if (counter != nullptr) {
    ++counter->weak_cnt;
  } else {
    counter = new SmartCounter<T>();
  }
}

template <typename T>
WeakPtr<T>::~WeakPtr() {
  if (counter != nullptr) {
    --counter->weak_cnt;

    if (counter->ptr == nullptr && counter->shared_cnt == 0 &&
        counter->weak_cnt == 0) {
      delete counter;
    }
  }
}

template <typename T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr& other) noexcept {
  if (counter != nullptr) {
    --counter->weak_cnt;
    other.swap(*this);
  }
  return *this;
}

template <typename T>
WeakPtr<T>& WeakPtr<T>::operator=(const SharedPtr<T>& other) noexcept {
  if (counter != nullptr) {
    --counter->weak_cnt;
    WeakPtr<T>(other).swap(*this);
  }
  return *this;
}

template <typename T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr&& other) noexcept {
  WeakPtr<T>(std::move(other)).swap(*this);
  return *this;
}

template <typename T>
long WeakPtr<T>::use_count() const noexcept {
  return counter != nullptr ? counter->shared_cnt : 0;
}

template <typename T>
SharedPtr<T> WeakPtr<T>::lock() const noexcept {
  return expired() ? SharedPtr<T>() : SharedPtr<T>(*this);
}

template <typename T>
bool WeakPtr<T>::expired() const noexcept {
  return use_count() == 0;
}

template <typename T>
void WeakPtr<T>::reset() noexcept {
  WeakPtr().swap(*this);
}

template <typename T>
void WeakPtr<T>::swap(WeakPtr& other) noexcept {
  std::swap(counter, other.counter);
}

}  // namespace task

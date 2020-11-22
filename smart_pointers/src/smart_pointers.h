#pragma once

namespace task {

template <class T>
class UniquePtr {
 private:
  T* ptr;

 public:
  ~UniquePtr();

  explicit UniquePtr(T*);
  UniquePtr(UniquePtr<T>&&) noexcept;

  T* get();
  T* release();
  void reset(T* ptr = nullptr);
  void swap(UniquePtr&) noexcept;

  T& operator*() const;
  T* operator->() const;
  UniquePtr<T>& operator=(UniquePtr<T>&&) noexcept;
};

template <typename T>
struct SmartCounter {
  T* ptr = nullptr;
  long shared_cnt = 0;
  long weak_cnt = 0;
  SmartCounter();
  SmartCounter(T* ptr, long shared_cnt = 1, long weak_cnt = 0);
  ~SmartCounter();
};

template <typename T>
class WeakPtr;

template <class T>
class SharedPtr {
 private:
  friend WeakPtr<T>;
  SmartCounter<T>* counter = nullptr;

 public:
  ~SharedPtr();

  SharedPtr();
  explicit SharedPtr(T* ptr);
  SharedPtr(const SharedPtr&) noexcept;
  SharedPtr(SharedPtr&&) noexcept;
  explicit SharedPtr(const WeakPtr<T>&);

  T* get() const noexcept;
  void reset(T* ptr = nullptr);
  void swap(SharedPtr&) noexcept;
  long use_count() const noexcept;

  T& operator*() const noexcept;
  T* operator->() const noexcept;
  SharedPtr& operator=(const SharedPtr&) noexcept;
  SharedPtr& operator=(SharedPtr&&) noexcept;
};

template <class T>
class WeakPtr {
 private:
  friend SharedPtr<T>;
  SmartCounter<T>* counter = nullptr;

 public:
  ~WeakPtr();

  WeakPtr();
  WeakPtr(const WeakPtr&) noexcept;
  WeakPtr(WeakPtr&&) noexcept;
  WeakPtr(const SharedPtr<T>&) noexcept;

  bool expired() const noexcept;
  void reset() noexcept;
  void swap(WeakPtr&) noexcept;
  long use_count() const noexcept;
  SharedPtr<T> lock() const noexcept;

  WeakPtr& operator=(const WeakPtr&) noexcept;
  WeakPtr& operator=(WeakPtr&&) noexcept;
  WeakPtr& operator=(const SharedPtr<T>&) noexcept;
};

}  // namespace task

#include "smart_pointers.tpp"

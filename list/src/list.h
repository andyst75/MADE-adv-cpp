#pragma once
#include <algorithm>
#include <iterator>

namespace task {

template <class T, class Alloc = std::allocator<T>>
class list {
 private:
  struct BaseNode {
    BaseNode *prev_;
    BaseNode *next_;
    BaseNode() : prev_(nullptr), next_(nullptr) {}
    void append(BaseNode *other) {
      if (other != nullptr) {
        next_ = other;
        other->prev_ = this;
      }
    }
  };

  struct Node : BaseNode {
    T value;

    Node() = default;

    template <class... Args>
    Node(Args &&... args) : value(std::forward<Args>(args)...) {}
  };

 public:
  class const_iterator;

  class iterator {
   public:
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::bidirectional_iterator_tag;

    iterator() = delete;
    iterator(const iterator &);

    iterator &operator=(const iterator &);
    iterator &operator++();
    iterator operator++(int);
    reference operator*() const;
    pointer operator->() const;
    iterator &operator--();
    iterator operator--(int);

    bool operator==(const iterator &) const;
    bool operator!=(const iterator &) const;
    operator const_iterator() const;

   private:
    friend class list;
    iterator(BaseNode *node);
    list::BaseNode *node_;
  };

  class const_iterator {
   public:
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = const T *;
    using reference = const T &;
    using iterator_category = std::bidirectional_iterator_tag;

    const_iterator() = delete;
    const_iterator(const const_iterator &);

    const_iterator &operator=(const const_iterator &);
    const_iterator &operator++();
    const_iterator operator++(int);
    reference operator*() const;
    pointer operator->() const;
    const_iterator &operator--();
    const_iterator operator--(int);

    bool operator==(const const_iterator &) const;
    bool operator!=(const const_iterator &) const;

   private:
    friend class list;
    const_iterator(BaseNode *);
    list::BaseNode *node_;
  };

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  list();
  explicit list(const Alloc &alloc);
  list(size_t count, const T &value, const Alloc &alloc = Alloc());
  explicit list(size_t, const Alloc &alloc = Alloc());

  ~list();

  list(const list &other);
  list(list &&other);
  list &operator=(const list &other);
  list &operator=(list &&other);

  Alloc get_allocator() const;

  T &front();
  const T &front() const;

  T &back();
  const T &back() const;

  iterator begin();
  iterator end();

  const_iterator cbegin() const;
  const_iterator cend() const;

  reverse_iterator rbegin();
  reverse_iterator rend();

  const_reverse_iterator crbegin() const;
  const_reverse_iterator crend() const;

  bool empty() const;
  size_t size() const;
  size_t max_size() const;
  void clear();

  iterator insert(const_iterator, const T &);
  iterator insert(const_iterator, T &&);
  iterator insert(const_iterator, size_t, const T &);

  iterator erase(const_iterator);
  iterator erase(const_iterator, const_iterator);

  void push_back(const T &);
  void push_back(T &&);
  void pop_back();

  void push_front(const T &);
  void push_front(T &&);
  void pop_front();

  template <class... Args>
  iterator emplace(const_iterator, Args &&...);

  template <class... Args>
  void emplace_back(Args &&...);

  template <class... Args>
  void emplace_front(Args &&...);

  void resize(size_t);
  void swap(list &);

  void merge(list &);
  void splice(const_iterator, list &);
  void remove(const T &);
  void reverse();
  void unique();
  void sort();

 private:
  void __init_link(size_t new_size = 0);

  template <class... Args>
  std::pair<Node *, Node *> __create(size_t, Args &&...);

  void __insert(const_iterator, BaseNode *, BaseNode *, size_t count = 0);

  using allocator_node =
      typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;

  using allocator_traits = std::allocator_traits<allocator_node>;

  size_t size_;
  BaseNode front_;
  BaseNode back_;
  allocator_node alloc_;
};

}  // namespace task

#include "list.cpp"

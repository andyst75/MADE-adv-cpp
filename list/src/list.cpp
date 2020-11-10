namespace task {

/**
 *
 * Iterators
 *
 */
template <typename T, typename Alloc>
list<T, Alloc>::iterator::iterator(const iterator &other)
    : node_(other.node_) {}

template <typename T, typename Alloc>
list<T, Alloc>::iterator::iterator(BaseNode *node) : node_(node) {}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator &list<T, Alloc>::iterator::operator=(
    const iterator &other) {
  if (*this != other) {
    node_ = other.node_;
  }
  return *this;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator &list<T, Alloc>::iterator::operator++() {
  node_ = node_->next_;
  return *this;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::iterator::operator++(int) {
  iterator i(*this);
  node_ = node_->next_;
  return i;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator::reference list<T, Alloc>::iterator::operator
    *() const {
  return static_cast<Node *>(node_)->value;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator::pointer
    list<T, Alloc>::iterator::operator->() const {
  return &static_cast<Node *>(node_)->value;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator &list<T, Alloc>::iterator::operator--() {
  node_ = node_->prev_;
  return *this;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::iterator::operator--(int) {
  iterator i(*this);
  node_ = node_->prev_;
  return i;
}

template <typename T, typename Alloc>
bool list<T, Alloc>::iterator::operator==(const iterator &other) const {
  return node_ == other.node_;
}

template <typename T, typename Alloc>
bool list<T, Alloc>::iterator::operator!=(const iterator &other) const {
  return node_ != other.node_;
}

template <typename T, typename Alloc>
list<T, Alloc>::iterator::operator const_iterator() const {
  return {node_};
}

/*
 * const_iterator
 */
template <typename T, typename Alloc>
list<T, Alloc>::const_iterator::const_iterator(const const_iterator &other)
    : node_(other.node_) {}

template <typename T, typename Alloc>
list<T, Alloc>::const_iterator::const_iterator(BaseNode *node) : node_(node) {}

template <typename T, typename Alloc>
typename list<T, Alloc>::const_iterator &
list<T, Alloc>::const_iterator::operator=(const const_iterator &other) {
  if (*this != other) {
    node_ = other.node_;
  }
  return *this;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::const_iterator &
list<T, Alloc>::const_iterator::operator++() {
  node_ = node_->next_;
  return *this;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::const_iterator
list<T, Alloc>::const_iterator::operator++(int) {
  const_iterator i(*this);
  node_ = node_->next_;
  return i;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::const_iterator &
list<T, Alloc>::const_iterator::operator--() {
  node_ = node_->prev_;
  return *this;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::const_iterator
list<T, Alloc>::const_iterator::operator--(int) {
  iterator i(*this);
  node_ = node_->prev_;
  return i;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::const_iterator::reference
    list<T, Alloc>::const_iterator::operator*() const {
  return static_cast<Node *>(node_)->value;
}

template <typename T, typename Alloc>
typename list<T, Alloc>::const_iterator::pointer
    list<T, Alloc>::const_iterator::operator->() const {
  return &static_cast<Node *>(node_)->value;
}

template <typename T, typename Alloc>
bool list<T, Alloc>::const_iterator::operator==(
    const const_iterator &other) const {
  return node_ == other.node_;
}

template <typename T, typename Alloc>
bool list<T, Alloc>::const_iterator::operator!=(
    const const_iterator &other) const {
  return node_ != other.node_;
}

/**
 *
 * Get Front / Back iterator
 *
 */
template <class T, class Alloc>
T &list<T, Alloc>::front() {
  return *begin();
}

template <class T, class Alloc>
const T &list<T, Alloc>::front() const {
  return *cbegin();
}

template <class T, class Alloc>
T &list<T, Alloc>::back() {
  return *(--end());
}

template <class T, class Alloc>
const T &list<T, Alloc>::back() const {
  return *(--cend());
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::begin() {
  return {front_.next_};
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::end() {
  return {&back_};
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator list<T, Alloc>::cbegin() const {
  return {front_.next_};
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator list<T, Alloc>::cend() const {
  return {const_cast<BaseNode *>(&back_)};
}

template <class T, class Alloc>
typename list<T, Alloc>::reverse_iterator list<T, Alloc>::rbegin() {
  return reverse_iterator(end());
}

template <class T, class Alloc>
typename list<T, Alloc>::reverse_iterator list<T, Alloc>::rend() {
  return reverse_iterator(begin());
}

template <class T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator list<T, Alloc>::crbegin()
    const {
  return const_reverse_iterator(cend());
}

template <class T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator list<T, Alloc>::crend() const {
  return const_reverse_iterator(cbegin());
}

/**
 *
 * Constructor / destructor
 *
 *
 */

template <class T, class Alloc>
list<T, Alloc>::~list() {
  clear();
}

template <class T, class Alloc>
list<T, Alloc>::list() : size_(0), front_(), back_(), alloc_() {
  __init_link();
}

template <class T, class Alloc>
list<T, Alloc>::list(const Alloc &alloc)
    : size_(0), front_(), back_(), alloc_(alloc) {
  __init_link();
}

template <class T, class Alloc>
list<T, Alloc>::list(size_t count, const T &value, const Alloc &alloc)
    : list(alloc) {
  auto [head, tail] = __create(count, value);
  __insert(cend(), head, tail, count);
}

template <class T, class Alloc>
list<T, Alloc>::list(size_t count, const Alloc &alloc) : list(alloc) {
  auto [head, tail] = __create(count);
  __insert(cend(), head, tail, count);
}

template <class T, class Alloc>
list<T, Alloc>::list(const list &other)
    : size_(0), front_(), back_(), alloc_(other.alloc_) {
  __init_link();
  auto it = other.cbegin();
  while (it != other.cend()) {
    emplace_back(*it++);
  }
}

template <class T, class Alloc>
list<T, Alloc>::list(list &&other)
    : size_(0), front_(), back_(), alloc_(std::move(other.alloc_)) {
  __init_link(other.size_);
  if (other.size_) {
    front_.append(other.front_.next_);
    other.back_.prev_->append(&back_);
    other.__init_link();
  }
}

/**
 *
 * Operators
 *
 */
template <class T, class Alloc>
list<T, Alloc> &list<T, Alloc>::operator=(const list &other) {
  if (this == &other) {
    return *this;
  }

  auto list(other);
  list.swap(*this);

  return *this;
}

template <class T, class Alloc>
list<T, Alloc> &list<T, Alloc>::operator=(list &&other) {
  if (this == &other) {
    return *this;
  }

  clear();
  other.swap(*this);

  return *this;
}

/**
 *
 * Insert/Delete/Emplace
 *
 */

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos,
                                                         const T &value) {
  auto res = __create(1, value);
  __insert(pos, res.first, res.second, 1);
  return {res.first};
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos,
                                                         T &&value) {
  auto res = __create(1, std::move(value));
  __insert(pos, res.first, res.second, 1);
  return {res.first};
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos,
                                                         size_t count,
                                                         const T &value) {
  auto res = __create(count, value);
  __insert(pos, res.first, res.second, count);
  return {res.first};
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(const_iterator pos) {
  auto node = pos.node_;
  auto next = node->next_;
  node->prev_->append(next);
  --size_;
  delete node;
  return {next};
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(const_iterator head,
                                                        const_iterator tail) {
  while (head != tail) {
    erase(head++);
  }
  return {tail.node_};
}

template <class T, class Alloc>
void list<T, Alloc>::push_back(const T &value) {
  insert(cend(), value);
}

template <class T, class Alloc>
void list<T, Alloc>::push_back(T &&value) {
  insert(cend(), std::move(value));
}

template <class T, class Alloc>
void list<T, Alloc>::pop_back() {
  erase(--cend());
}

template <class T, class Alloc>
void list<T, Alloc>::push_front(const T &value) {
  insert(cbegin(), value);
}

template <class T, class Alloc>
void list<T, Alloc>::push_front(T &&value) {
  insert(cbegin(), std::move(value));
}

template <class T, class Alloc>
void list<T, Alloc>::pop_front() {
  erase(cbegin());
}

template <class T, class Alloc>
template <class... Args>
typename list<T, Alloc>::iterator list<T, Alloc>::emplace(const_iterator pos,
                                                          Args &&... args) {
  auto res = __create(1, std::forward<Args>(args)...);
  __insert(pos, res.first, res.second, 1);
  return {res.first};
}

template <class T, class Alloc>
template <class... Args>
void list<T, Alloc>::emplace_back(Args &&... args) {
  emplace(cend(), std::forward<Args>(args)...);
}

template <class T, class Alloc>
template <class... Args>
void list<T, Alloc>::emplace_front(Args &&... args) {
  emplace(cbegin(), std::forward<Args>(args)...);
}

/**
 *
 * Other public methods
 *
 */
template <class T, class Alloc>
Alloc list<T, Alloc>::get_allocator() const {
  return Alloc();
}

template <class T, class Alloc>
bool list<T, Alloc>::empty() const {
  return size_ == 0;
}

template <class T, class Alloc>
size_t list<T, Alloc>::size() const {
  return size_;
}

template <class T, class Alloc>
size_t list<T, Alloc>::max_size() const {
  return allocator_traits::max_size(alloc_);
}

template <class T, class Alloc>
void list<T, Alloc>::clear() {
  while (cbegin() != cend()) {
    erase(cbegin());
  }
}

template <class T, class Alloc>
void list<T, Alloc>::resize(size_t count) {
  while (count > size_) {
    emplace_back(T());
  }
  while (count < size_) {
    pop_back();
  }
}

template <class T, class Alloc>
void list<T, Alloc>::remove(const T &value) {
  auto node = cbegin();
  while (node != cend()) {
    if (*node == value) {
      node = erase(node);
    } else {
      node++;
    }
  }
}

template <class T, class Alloc>
void list<T, Alloc>::swap(list &other) {
  std::swap(size_, other.size_);
  std::swap(alloc_, other.alloc_);
  std::swap(front_, other.front_);
  std::swap(back_, other.back_);

  if (!empty()) {
    front_.next_->prev_ = &front_;
    back_.prev_->next_ = &back_;
  } else {
    __init_link();
  }
  if (!other.empty()) {
    other.front_.next_->prev_ = &other.front_;
    other.back_.prev_->next_ = &other.back_;
  } else {
    other.__init_link();
  }
}

template <class T, class Alloc>
void list<T, Alloc>::splice(const_iterator pos, list &other) {
  if (other.empty()) {
    return;
  }

  __insert(pos, other.front_.next_, other.back_.prev_, other.size_);
  other.__init_link();
}

template <class T, class Alloc>
void list<T, Alloc>::reverse() {
  if (empty()) {
    return;
  }

  auto node = front_.next_;
  while (node != &back_) {
    std::swap(node->next_, node->prev_);
    node = node->prev_;
  }

  auto old_back = back_.prev_;
  auto old_front = front_.next_;
  front_.append(old_back);
  old_front->append(&back_);
}

template <class T, class Alloc>
void list<T, Alloc>::merge(list &other) {
  if (this == &other) {
    return;
  }

  size_t new_size = size_ + other.size_;

  BaseNode *prev = nullptr;
  BaseNode *front = nullptr;

  auto head1 = cbegin();
  auto head2 = other.cbegin();
  auto tail1 = cend();
  auto tail2 = other.cend();

  while (head1 != tail1 or head2 != tail2) {
    BaseNode *node = nullptr;
    if (head1 != tail1 and head2 != tail2) {
      node = reinterpret_cast<Node *>(head2.node_)->value <
                     reinterpret_cast<Node *>(head1.node_)->value
                 ? head2++.node_
                 : head1++.node_;
    } else if (head1 != tail1) {
      node = head1++.node_;
    } else if (head2 != tail2) {
      node = head2++.node_;
    }

    if (prev == nullptr) {
      front = node;
    } else {
      prev->append(node);
    }

    prev = node;
  }

  __init_link();
  __insert(cend(), front, prev, new_size);
  other.__init_link();
}

template <class T, class Alloc>
void list<T, Alloc>::unique() {
  if (empty()) {
    return;
  }

  auto node = cbegin();
  auto prev_node = ++node;
  while (node != cend()) {
    if (*node == *prev_node) {
      node = erase(node);
    } else {
      prev_node = node++;
    }
  }
}

template <class T, class Alloc>
void list<T, Alloc>::sort() {
  Node *current = reinterpret_cast<Node *>(front_.next_);
  while (current != &back_) {
    Node *pos = reinterpret_cast<Node *>(current->next_);
    Node *min = current;
    while (pos != &back_) {
      if (pos->value < min->value) {
        min = pos;
      }
      pos = reinterpret_cast<Node *>(pos->next_);
    }
    std::swap(min->value, current->value);
    current = reinterpret_cast<Node *>(current->next_);
  }
}

/**
 *
 * Nodes supply function
 *
 */

template <class T, class Alloc>
void list<T, Alloc>::__init_link(size_t new_size) {
  size_ = new_size;
  if (!new_size) {
    back_.prev_ = &front_;
    front_.next_ = &back_;
  }
}

template <class T, class Alloc>
template <class... Args>
std::pair<typename list<T, Alloc>::Node *, typename list<T, Alloc>::Node *>
list<T, Alloc>::__create(size_t count, Args &&... args) {
  Node *tail = nullptr;
  Node *head = nullptr;
  for (size_t i = 0; i < count; ++i) {
    Node *node = alloc_.allocate(1);
    alloc_.construct(node, std::forward<Args>(args)...);
    if (head == nullptr) {
      head = node;
    } else {
      tail->append(node);
    }
    tail = node;
  }
  return std::make_pair(head, tail);
}

template <class T, class Alloc>
void list<T, Alloc>::__insert(const_iterator pos, BaseNode *front,
                              BaseNode *back, size_t count) {
  pos.node_->prev_->append(front);
  back->append(pos.node_);
  if (count == 0 && front != nullptr) {
    BaseNode *node = front;
    while (node != back) {
      ++count;
      node = node->next_;
    }
  }
  size_ += count;
}

}  // namespace task

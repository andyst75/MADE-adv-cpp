#include <iostream>
#include <limits>
#include <set>
#include <string>
#include <vector>

template <typename T>
class MemManager {
  struct ChunkNode {
    ChunkNode* next = nullptr;
    ChunkNode* prev = nullptr;
    size_t size = 0;
    bool free = false;
  };

  struct MemNode {
    MemNode* next = nullptr;
    MemNode* prev = nullptr;
    size_t size = 0;
    uint8_t* pointer = nullptr;
    ChunkNode* nodes = nullptr;
  };

 private:
  MemNode* root = nullptr;
  int count = 1;

  T* create_node(const size_t bytes) {
    uint8_t* pointer = new uint8_t[bytes + sizeof(MemNode) + sizeof(ChunkNode)];
    MemNode* node = new (pointer) MemNode;

    node->pointer = pointer + sizeof(MemNode) + sizeof(ChunkNode);
    if (this->root != nullptr) {
      node->next = this->root;
      this->root->prev = node;
    }
    this->root = node;
    ChunkNode* chunk = new (pointer + sizeof(MemNode)) ChunkNode;
    chunk->size = bytes;

    node->nodes = chunk;
    node->size = bytes;

    return reinterpret_cast<T*>(node->pointer);
  }

  T* get_chunk(ChunkNode* chunk, const size_t bytes) {
    uint8_t* pointer = reinterpret_cast<uint8_t*>(chunk) + sizeof(ChunkNode);

    if (chunk->size > bytes + sizeof(ChunkNode)) {
      ChunkNode* new_chunk = new (pointer + bytes) ChunkNode;
      new_chunk->free = true;
      new_chunk->size = chunk->size - bytes - sizeof(ChunkNode);
      new_chunk->prev = chunk;
      if (chunk->next != nullptr) {
        new_chunk->next = chunk->next;
        new_chunk->next->prev = new_chunk;
      }
      chunk->next = new_chunk;
    }
    chunk->free = false;
    chunk->size = bytes;
    return reinterpret_cast<T*>(pointer);
  }

  T* find_allocate(const size_t bytes) {
    MemNode* node = root;
    while (node != nullptr) {
      if (node->size >= bytes) {
        ChunkNode* chunk = node->nodes;
        while (chunk != nullptr) {
          if (chunk->free && chunk->size >= bytes) {
            return get_chunk(chunk, bytes);
          }
          chunk = chunk->next;
        }
      }
      node = node->next;
    }

    return create_node(bytes);
  }

  void free_node(MemNode* node) {
    size_t v = 0;
    ChunkNode* chunk = node->nodes;
    while (chunk != nullptr) {
      if (chunk->free) {
        v += chunk->size + sizeof(ChunkNode);
      }
      chunk = chunk->next;
    }
    if (v >= node->size) {
      if (node->next != nullptr) {
        node->next->prev = node->prev;
      }

      if (node->prev != nullptr) {
        node->prev->next = node->next;
      }

      if (root == node) {
        root = node->next;
      }
      delete[] node;
    }
  }

  void find_deallocate(T* pointer, const size_t bytes) {
    MemNode* node = root;
    uint8_t* p = reinterpret_cast<uint8_t*>(pointer);
    while (node != nullptr) {
      if (p >= node->pointer && p < node->pointer + node->size) {
        ChunkNode* chunk = node->nodes;
        while (chunk != nullptr) {
          uint8_t* cp = reinterpret_cast<uint8_t*>(chunk) + sizeof(ChunkNode);
          if (!chunk->free && cp == p) {
            if (chunk->size == bytes) {
              chunk->free = true;
              if (chunk->prev != nullptr && chunk->prev->free) {
                chunk->prev->size += chunk->size + sizeof(ChunkNode);
                chunk->prev->next = chunk->next;
                if (chunk->next != nullptr) {
                  chunk->next->prev = chunk->prev;
                }
              }
              free_node(node);
            } else if (chunk->size >= bytes) {
              get_chunk(chunk, chunk->size - bytes);
            }

            return;
          }
          chunk = chunk->next;
        }
      }
      node = node->next;
    }
  }

 public:
  T* allocate(const size_t cnt) { return find_allocate(sizeof(T) * cnt); }

  void deallocate(T* pointer, const size_t cnt) {
    find_deallocate(pointer, sizeof(T) * cnt);
  }

  void count_increment() { ++count; }

  void count_decrement() {
    if (count > 0) {
      --count;
    } else {
    }
  }

  int get_count() { return count; }

  std::pair<size_t, size_t> get_allocated() {
    size_t capacity = 0;
    size_t allocated = 0;
    MemNode* node = root;

    while (node != nullptr) {
      capacity += node->size;
      ChunkNode* chunk = node->nodes;
      while (chunk != nullptr) {
        if (!chunk->free) {
          allocated += chunk->size;
        }
        chunk = chunk->next;
      }
      node = node->next;
    }

    return {capacity, allocated};
  }

  ~MemManager<T>() {
    if (count == 0) {
      while (root != nullptr) {
        auto* next = root->next;
        delete[] root;
        root = next;
      }
    } else {
      --count;
    }
  }
};

template <typename T>
class StdAllocator {
 public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

 private:
  MemManager<T>* manager = nullptr;

 public:
  StdAllocator<T>() {}

  StdAllocator<T>(const StdAllocator<T>& other) {
    if (other.manager != nullptr) {
      manager = other.manager;
      manager->count_increment();
    }
  }
  ~StdAllocator<T>() {
    if (manager != nullptr) {
      manager->count_decrement();
      if (!manager->get_count()) {
        delete manager;
      }
    }
  }

  template <typename U>
  struct rebind {
    typedef StdAllocator<U> other;
  };

  T* allocate(const size_type cnt) {
    if (manager == nullptr) {
      manager = new MemManager<T>();
    }
    return manager->allocate(cnt);
  }

  void deallocate(pointer p, const size_type cnt) {
    manager->deallocate(p, cnt);
  }

  std::pair<size_t, size_t> get_allocated() {
    if (manager == nullptr) {
      return {0, 0};
    }
    return manager->get_allocated();
  }

  size_t max_size() const {
    return std::numeric_limits<size_type>::max() / sizeof(T);
  }

  void destroy(T* p) { p->~T(); }

  template <class U, class... Args>
  void* construct(U* p, Args&&... args) {
    return new (p) U(std::forward<Args>(args)...);
  }

  StdAllocator& operator=(const StdAllocator& other) {
    if (manager != other.manager) {
      if (manager->get_count()) {
        manager->count_decrement();
      } else {
        delete manager;
      }
      manager = other.manager;
      manager->count_increment();
    }
    return *this;
  }

  bool operator==(StdAllocator const& other) const {
    return manager != other.manager;
  }
  bool operator!=(StdAllocator const& other) const { return !(this == other); }
};

int main() {
  std::cout << "Start test" << std::endl;

  StdAllocator<int> alloc1;
  auto stat = alloc1.get_allocated();
  std::cout << "Empty allocator: " << stat.first << " " << stat.second
            << std::endl;

  int* a = alloc1.allocate(128);
  stat = alloc1.get_allocated();
  std::cout << "Allocate int[128]: " << stat.first << " " << stat.second
            << std::endl;

  alloc1.deallocate(a, 64);
  stat = alloc1.get_allocated();
  std::cout << "After dellocate 64 int's: " << stat.first << " " << stat.second
            << std::endl;

  StdAllocator<int> alloc2 = alloc1;
  stat = alloc2.get_allocated();
  std::cout << "Copyed allocator: " << stat.first << " " << stat.second
            << std::endl;

  alloc2.deallocate(a, 32);
  stat = alloc1.get_allocated();
  std::cout << "After copyed dellocate 32 int's: " << stat.first << " "
            << stat.second << std::endl;

  std::vector<int, decltype(alloc1)> vect;
  stat = vect.get_allocator().get_allocated();
  std::cout << "Empty vector: " << stat.first << " " << stat.second
            << std::endl;
  for (int i = 0; i < 100000; ++i) {
    vect.push_back(i);
  }

  stat = vect.get_allocator().get_allocated();
  std::cout << "After push_backs 100000 int's: " << stat.first << " "
            << stat.second << std::endl;

  std::vector<int, decltype(alloc1)> set;
  stat = set.get_allocator().get_allocated();
  std::cout << "Empty set: " << stat.first << " " << stat.second
            << std::endl;

  set.push_back(1);
  stat = set.get_allocator().get_allocated();
  std::cout << "After 1 int insert: " << stat.first << " " << stat.second
            << std::endl;

  StdAllocator<std::string> allocs;
  std::vector<std::string, decltype(allocs)> sets;
  sets.push_back("string 1");
  stat = sets.get_allocator().get_allocated();
  std::cout << "After 1 string insert: " << stat.first << " " << stat.second
            << std::endl;

  sets.push_back("string 2");
  stat = sets.get_allocator().get_allocated();
  std::cout << "After 2 string insert: " << stat.first << " " << stat.second
            << std::endl;

  std::cout << "Finish test" << std::endl;

  return 0;
}

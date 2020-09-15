#pragma once
#include <iterator>


namespace task {


template<class T, class Alloc = std::allocator<T>>
class list {

public:
    class iterator {
    public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;

        iterator();
        iterator(const iterator&);
        iterator& operator=(const iterator&);

        iterator& operator++();
        iterator operator++(int);
        reference operator*() const;
        pointer operator->() const;
        iterator& operator--();
        iterator operator--(int);

        bool operator==(iterator other) const;
        bool operator!=(iterator other) const;

        // Your code goes here?..

    private:
        // Your code hoes here...
    };

    using reverse_iterator = std::reverse_iterator<iterator>;

    list();
    explicit list(const Alloc& alloc);
    list(size_t count, const T& value, const Alloc& alloc = Alloc());
    explicit list(size_t count, const Alloc& alloc = Alloc());

    ~list();
    list& operator=(const list& other);

    Alloc get_allocator() const;


    T& front();
    const T& front() const;

    T& back();
    const T& back() const;


    iterator begin();
    iterator end();

    reverse_iterator rbegin();
    reverse_iterator rend();


    bool empty() const;
    size_t size() const;
    size_t max_size() const;
    void clear();

    iterator insert(iterator pos, const T& value);
    iterator insert(iterator pos, size_t count, const T& value);

    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);

    void push_back(const T& value);
    void pop_back();
    void push_front(const T& value);
    void pop_front();
    void resize(size_t count);
    void swap(list& other);


    void merge(list& other);
    void splice(iterator pos, list& other);
    void remove(const T& value);
    void reverse();
    void unique();
    void sort();

    // Your code goes here?..

private:

    // Your code goes here...

};

}  // namespace task

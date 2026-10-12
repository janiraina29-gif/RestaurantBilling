#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include <stdexcept>

// A growable array built on new[] / delete[].
// It OWNS the memory it allocates and releases it in its destructor.
// (Templates must live entirely in a header file, so there is no .cpp.)
//
// Because it owns raw memory it needs the "Rule of Three":
//   destructor + copy constructor + copy assignment.
// Without the last two, copying an array would copy only the pointer and
// both copies would later delete[] the same memory (a crash).
template <typename T>
class DynamicArray {
private:
    T* items;       // the heap block
    int count;      // how many slots are in use
    int capacity;   // how many slots exist

    void grow()
    {
        int newCapacity = capacity * 2;
        T* bigger = new T[newCapacity];
        for (int i = 0; i < count; i++) {
            bigger[i] = items[i];
        }
        delete[] items;
        items = bigger;
        capacity = newCapacity;
    }

public:
    explicit DynamicArray(int initialCapacity = 4)
        : items(nullptr), count(0), capacity(initialCapacity > 0 ? initialCapacity : 4)
    {
        items = new T[capacity];
    }

    DynamicArray(const DynamicArray& other)
        : items(nullptr), count(other.count), capacity(other.capacity)
    {
        items = new T[capacity];
        for (int i = 0; i < count; i++) {
            items[i] = other.items[i];
        }
    }

    DynamicArray& operator=(const DynamicArray& other)
    {
        if (this != &other) {
            T* copy = new T[other.capacity];       // build the copy first
            for (int i = 0; i < other.count; i++) {
                copy[i] = other.items[i];
            }
            delete[] items;                        // then release the old block
            items = copy;
            count = other.count;
            capacity = other.capacity;
        }
        return *this;
    }

    ~DynamicArray()
    {
        delete[] items;
    }

    void add(const T& value)
    {
        if (count == capacity) {
            grow();
        }
        items[count] = value;
        count++;
    }

    void removeAt(int index)
    {
        if (index < 0 || index >= count) {
            throw std::out_of_range("DynamicArray::removeAt index out of range");
        }
        for (int i = index; i < count - 1; i++) {
            items[i] = items[i + 1];
        }
        count--;
    }

    // Forgets all elements (does not delete what pointers point to).
    void clear() { count = 0; }

    int size() const { return count; }

    T& operator[](int index)
    {
        if (index < 0 || index >= count) {
            throw std::out_of_range("DynamicArray index out of range");
        }
        return items[index];
    }

    const T& operator[](int index) const
    {
        if (index < 0 || index >= count) {
            throw std::out_of_range("DynamicArray index out of range");
        }
        return items[index];
    }
};

#endif

#pragma once
#include <iostream>
#include <stdexcept> 
#include <utility> 

template<typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}


template<typename T>
class DynamicArray {
private:
    T* data;
    unsigned int capacity;
    unsigned int length;

    void resize(unsigned int newCapacity) {
        T* newData = new T[newCapacity];
        for (unsigned int i = 0; i < length; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    void quickSort(int left, int right) {
        if (left >= right) return;

        T pivot = data[(left + right) / 2];
        int i = left, j = right;

        while (i <= j) {
            while (data[i] < pivot) i++;
            while (data[j] > pivot) j--;
            if (i <= j) {
                std::swap(data[i], data[j]);
                i++;
                j--;
            }
        }

        if (left < j) quickSort(left, j);
        if (i < right) quickSort(i, right);
    }

    template<typename Compare>
    void quickSortCustom(int left, int right, Compare comp) {
        if (left >= right) return;

        T pivot = data[(left + right) / 2];
        int i = left, j = right;

        while (i <= j) {
            while (comp(data[i], pivot)) i++;
            while (comp(pivot, data[j])) j--;
            if (i <= j) {
                std::swap(data[i], data[j]);
                i++;
                j--;
            }
        }

        if (left < j) quickSortCustom(left, j, comp);
        if (i < right) quickSortCustom(i, right, comp);
    }

public:
    DynamicArray() : data(nullptr), capacity(0), length(0) {}

    DynamicArray(const DynamicArray& other)
        : data(nullptr), capacity(0), length(0) {
        *this = other;
    }

    ~DynamicArray() {
        delete[] data;
    }

    void append(const T& value) {
        if (length == capacity) {
            resize(capacity == 0 ? 2 : capacity * 2);
        }
        data[length++] = value;
    }

    T& operator[](unsigned int index) {
        if (index >= length) {
            throw std::out_of_range("DynamicArray: Index out of range");
        }
        return data[index];
    }


    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) return *this;

        delete[] data;

        capacity = other.capacity;
        length = other.length;
        data = new T[capacity];

        for (unsigned int i = 0; i < length; ++i) {
            data[i] = other.data[i];
        }

        return *this;
    }

    unsigned int size() const {
        return length;
    }

    bool isEmpty() const {
        return length == 0;
    }

    void clear() {
        delete[] data;
        data = nullptr;
        capacity = 0;
        length = 0;
    }


    void sort() {
        if (length > 1) {
            quickSort(0, length - 1);
        }
    }

    template<typename Compare>
    void sort(Compare comp) {
        if (length > 1) {
            quickSortCustom(0, length - 1, comp);
        }
    }

    void removeAt(unsigned int index) {
        if (index >= length) {
            throw std::out_of_range("DynamicArray: Index out of range");
        }
        for (unsigned int i = index; i < length - 1; ++i) {
            data[i] = data[i + 1];
        }
        --length;
    }

    bool removeValue(const T& value) {
        for (unsigned int i = 0; i < length; ++i) {
            if (data[i] == value) {
                removeAt(i);
                return true;
            }
        }
        return false;
    }
};

// CybSec.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>

using namespace std;

template <typename T>
class IEnumerator {
public:
    virtual T Current() = 0;
    virtual bool MoveNext() = 0;
    virtual void Reset() = 0;
};

template <typename T>
class Enumerator : public IEnumerator<T> {
private:
    T* arr_ptr = nullptr;
    uint32_t len = 0;
    int32_t counter = -1;

public:
    Enumerator(T* ptr, uint32_t lenght) {
        arr_ptr = ptr;
        len = lenght - 1;
    }

    T Current() {
        return arr_ptr[counter];
    }

    bool MoveNext() {
        if (counter >= (int32_t)len) {
            return false;
        } else {
            counter++;
            return true;
        }
    }

    void Reset() {
        counter = 0;
    }
};

template <typename T>
class IEnumerable {
public:
    virtual IEnumerator<T>* GetEnumerator();
};

template <typename T>
struct Span {
private:
    uint32_t struct_len = 0;
    T* rw_struct_pointer = nullptr;         // read-write
    T const* ro_struct_pointer = nullptr;   // read-only
    Enumerator<T>* enumer;

public:
    Span(T* data, uint32_t lenght, bool isReadOnly = false) : struct_len(lenght) {
        rw_struct_pointer = new T[lenght];
        rw_struct_pointer = data;
        enumer = new Enumerator<T>(rw_struct_pointer, lenght);
    }

    /* for RO memory - declare Span as const and use const data pointer. Ex: const Span<int>(const data, len) */
    Span(const T* data, uint32_t length) {
        ro_struct_pointer = data;
        enumer = new Enumerator<T>(ro_struct_pointer, length);
    }

    T* Data() {
        return rw_struct_pointer;
    }

    const T* Data() const {
        return ro_struct_pointer;
    }

    int32_t Length() {
        return struct_len;
    }

    bool IsEmpty() {
        (rw_struct_pointer == nullptr or ro_struct_pointer == nullptr) ? true : false;
    }

    T& operator[](uint32_t index) {
        return rw_struct_pointer[index];
    }

    const T& operator[](uint32_t index) const{
        return ro_struct_pointer[index];
    }

    IEnumerator<T>* GetEnumerator() {
        return enumer;
    }
};



int main()
{
    const int SZ = 10;
    int* data = new int[SZ];
    for (int i = 0; i < SZ; i++)
        data[i] = i + 1;

    Span<int> s(data, SZ);

    auto enumerator = s.GetEnumerator();

    while (enumerator->MoveNext()) {
        printf("N=%d\n", enumerator->Current());
    }

    delete enumerator;
    delete[] data;
}

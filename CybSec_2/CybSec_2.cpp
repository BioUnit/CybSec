/**/

#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

struct HandleInfo {
    uint64_t size;
    void* ptr;
};

class MemoryAllocator{
    uint8_t counter=0;
    vector<HandleInfo> vec_arr;
    
public:
    void show() {
        HandleInfo a = vec_arr.back();
        cout << "vec size[0]= " << a.size << endl;
    }
    
    void add(HandleInfo handle) {
        vec_arr.push_back(handle);
        counter++;
    }
};

MemoryAllocator malloc_monitor;

void* operator new(uint64_t size) {
    printf("%d",size);

    void* ptr = malloc(size);

    if (ptr != nullptr) {
        
        struct HandleInfo a = {
            a.size = size,
            a.ptr = ptr
        };
        //cout << size << endl;
        //malloc_monitor.add(a);
        
        return ptr;

    } else {
        return nullptr;
    }
}
/*
void* operator new[](uint64_t size) {

}
*/
void operator delete(void* p) {
    free(p);
}

int main()
{
    const int BUFFER_SIZE = 50;

    int* a = new int[BUFFER_SIZE];
    a[0] = 1;
    a[49] = 2;
    free(a);
    //malloc_monitor.show();
    /*
    int* buffer = new int[BUFFER_SIZE];
    for (int i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = i * 3;
    //TODO: print number of handles, and total memory allocation size (bytes)
    struct Complex {
        double real;
        double imaginary;
    };
    auto magnitudes = new struct Complex[BUFFER_SIZE];
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        magnitudes[i].real = i * 10;
        magnitudes[i].imaginary = i * 20;
    }
    //TODO: print number of handles, and total memory allocation size (bytes)
    delete[] buffer;
    */
    //TODO: print number of handles, and total memory allocation size (bytes)

    // At the end of program, the allocator should delete the lef-over handles (i.e:
    //magnitudes), the total memory allocation size should be ZERO(bytes)
    return 0;
}

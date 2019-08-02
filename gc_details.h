// This class defines an element that is stored
// in the garbage collection information list.
//
template<class T>
class PtrDetails {
public:
    unsigned refcount; // current reference count
    T *memPtr;         // pointer to allocated memory
    /* isArray is true if memPtr points
to an allocated array. It is false
otherwise. */
    bool isArray; // true if pointing to array
    /* If memPtr is pointing to an allocated
array, then arraySize contains its size */
    unsigned arraySize; // size of array
    // Here, mPtr points to the allocated memory.
    // If this is an array, then size specifies
    // the size of the array.

    PtrDetails(T *ptr, unsigned int arraySize = 0) : refcount(0), memPtr(ptr), arraySize(arraySize) {
        isArray = arraySize > 0;
    }

    PtrDetails(void) {
        PtrDetails(NULL);
    }

};

// Overloading operator== allows two class objects to be compared.
// This is needed by the STL list class.
template<class T>
bool operator==(const PtrDetails<T> &obj_1,
                const PtrDetails<T> &obj_2) {
    return obj_1.memPtr == obj_2.memPtr &&
           obj_1.refcount == obj_2.refcount &&
           obj_1.arraySize == obj_2.arraySize;
}
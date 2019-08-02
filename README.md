# SmartPointer
This is a derived work from Udacity starter code at project [CppND-Garbage-Collector](https://github.com/udacity/CppND-Garbage-Collector).

# Prerequisites
The project has been tested with compiler:
- g++ (GCC) 7.2. (Ubuntu 1.04.6 LTS)
- Apple LLVM version 10.0.1 (clang-1001.0.46.3)

# Compile and Run 
```bash
g++ main.cpp -std=c++17 -Wall
./a.out
```

## Project TODO List:
- Complete `Pointer` constructor - done
- Complete `Pointer` `operator==` - done
- Complete `Pointer` destructor -done
- Complete `PtrDetails` class -done

The main.cpp contains several test:

- 1-3: constructors
- 4: self-assignment
- 5-6: assignment with/without initialized left hand side
- 7-8: constructor for pointer arrays
- 9: Compiler guided error detection on assignment if array sizes are different
- 10: Test Pointer to custom type 'SimpleStruct'

## Remarks
- I tried to follow the DRY principle and factored out equal code parts for different constructors.
See the private method void Pointer::updateRefContainer(T *t, int arraySize);
- For better testability I also added a public function to Pointer to read out the refCount. This method adds no new functionality, but makes it possible to make asertions in test code. 


#include <string>
#include <sstream>
#include <iostream>
#include <assert.h>
#include "gc_pointer.h"
#include "LeakTester.h"

// Define a custom type for testing
struct SimpleStruct {
    int a;
    int b;
};

std::ostream &operator<<(std::ostream &os, const SimpleStruct &s) {
    return (os << "{ a: " << s.a << "; b: " << s.b << " }" << std::endl);
}

void selfAssignment(Pointer<int> &x, Pointer<int> &y) {
    x = y;           // Could be self-assignment if &x == &y
}

int main() {

    // 1. Test constructor
    std::cout << ">>>TEST constructor: Pointer<int> p = new int(19)" << std::endl;
    Pointer<int> p = new int(19);
    // refContainer shall now contain one entry
    assert(p.refContainerSize() == 1);
    assert(p.getRefCount() == 1);
    // calling collect shall not free memory, because pointer ius still in use
    assert(p.collect() == false);
    Pointer<int>::showlist();

    // construct a second Pointer instance
    std::cout << ">>>TEST constructor: Pointer<int> p = new int(42)" << std::endl;
    Pointer<int> q = new int(42);
    // refContainer shall now contain two entries
    assert(q.refContainerSize() == 2);
    assert(q.getRefCount() == 1);
    // calling collect shall not free memory, because all pointers are still in use
    assert(q.collect() == false);
    Pointer<int>::showlist();

    // 2. Test default constructor
    std::cout << ">>>TEST constructor: Pointer<int> r; " << std::endl;
    Pointer<int> r;
    // refContainer shall now contain two entries
    std::cout << ">>>TEST assign r = q" << std::endl;
    r = q;
    assert(r.refContainerSize() == 2);
    assert(r.getRefCount() == 2);
    // calling collect shall not free memory, because all pointers are still in use
    assert(r.collect() == false);
    Pointer<int>::showlist();

    // 3. Test copy constructor
    std::cout << ">>>TEST copy constructor: t = Pointer<int>(q); " << std::endl;
    Pointer<int> t = Pointer<int>(q);
    assert(t.refContainerSize() == 2); // still only two adresses to point to
    assert(t.getRefCount() == 3);

    // 4. Test SELF - ASSIGNMENT
    std::cout << ">>>TEST self asignment: p = p" << std::endl;
    selfAssignment(p, p);
    //  p = p; this line would lead to compiler warning
    assert(t.refContainerSize() == 2); // still only two adresses to point to
    Pointer<int>::showlist();

    // 5. Test assignment with PREDEFINED LHS
    std::cout << ">>>TEST assignment with predefined lhs: q = p" << std::endl;
    q = p;
    assert(q.refContainerSize() == 2); // still only two adresses to point to

    // 6. Test assignment with UNINITIALIZED LHS;
    std::cout << ">>>TEST assignment with uninitialized lhs: r = p" << std::endl;
    r = p;
    assert(q.refContainerSize() == 2); // still only two adresses to point to

    // 7. Test array default constructor
    std::cout << ">>>TEST default array constructor: Pointer<int, 3> v;" << std::endl;
    Pointer<int, 3> v;
    Pointer<int, 3>::showlist();

    v = Pointer<int, 3>(new int[3]);
    v[0] = 42;
    v[1] = 43;
    v[2] = 44;
    // refContainer shall now contain one entry
    assert(v.refContainerSize() == 1);
    assert(v.getRefCount() == 1);
    // calling collect shall not free memory, because pointer is still in use
    assert(v.collect() == false);
    Pointer<int, 3>::showlist();

    // 8. Test array (non-default-) constructor
    std::cout << ">>>TEST constructor: Pointer<int, 3> u = new int[3];" << std::endl;
    Pointer<int, 3> u = new int[3];
    u[0] = 11;
    u[1] = 12;
    u[2] = 13;
    // refContainer shall now contain one entry
    assert(u.refContainerSize() == 2);
    assert(u.getRefCount() == 1);
    // calling collect shall not free memory, because pointer is still in use
    assert(u.collect() == false);
    Pointer<int, 3>::showlist();

    // 9. Assignment of different array sized pointers shall create compiler error
    // Pointer<int, 3> w = Pointer<int, 2>(new int[3]);

    // 10. Test custom type
    Pointer<SimpleStruct> sP = new SimpleStruct{1, 2};
    Pointer<SimpleStruct> sP2 = sP;
    // refContainer shall now contain one entry
    assert(sP.refContainerSize() == 1);
    assert(sP.getRefCount() == 2);
    // calling collect shall not free memory, because pointer ius still in use
    assert(sP.collect() == false);
    Pointer<SimpleStruct>::showlist();

    return 0;
}
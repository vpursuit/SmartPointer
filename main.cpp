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


namespace custom {
    template<typename T>
    class list {
    private:
        struct node {
            T value;
            Pointer<node> prev;
            Pointer<node> next;

            node(T val, node *_prev, node *_next) : value(val), prev(_prev), next(_next) {}
        };

        node *head;
        node *tail;
    public:

        // We don't have any destructors or freeing of memory
        list() : head(NULL), tail(NULL) {}

        void push_back(T);

        bool empty() const { return (!head || !tail); }

        void print();
    };

    template<typename T>
    void list<T>::push_back(T val) {
        list::tail = new node(val, list::tail, NULL);
        if (list::tail->prev) {
            list::tail->prev->next = list::tail;
        }
        if (list::empty()) {
            list::head = list::tail;
            list::tail->prev = list::tail;
        }
    }

    template<typename T>
    void list<T>::print() {
        node *ptr = list::head;
        while (ptr != NULL) {
            std::cout << ptr->value << " ";
            ptr = ptr->next;
        }
        std::cout << std::endl;
    }
}


void unitTest() {
    // 1. Test constructor
    std::cout << ">>>TEST constructor: Pointer<int> p = new int(19)" << std::endl;
    Pointer<int> p = new int(19);
    // refContainer shall now contain one entry
    assert(Pointer<int>::refContainerSize() == 1);
    assert(p.getRefCount() == 1);
    Pointer<int>::showlist();

    // construct a second Pointer instance
    std::cout << ">>>TEST constructor: Pointer<int> q = new int(42)" << std::endl;
    Pointer<int> q = new int(42);
    // refContainer shall now contain two entries
    assert(Pointer<int>::refContainerSize() == 2);
    assert(q.getRefCount() == 1);
    Pointer<int>::showlist();

    // 2. Test default constructor
    std::cout << ">>>TEST constructor: Pointer<int> r; " << std::endl;
    Pointer<int> r = q;
    Pointer<int>::showlist();
    // refContainer shall now contain two entries
    std::cout << ">>>TEST assign r = q" << std::endl;
    r = q;
    Pointer<int>::showlist();
    assert(Pointer<int>::refContainerSize() == 2);
    assert(r.getRefCount() == 2);

    // 3. Test copy constructor
    std::cout << ">>>TEST copy constructor: t = Pointer<int>(q); " << std::endl;
    Pointer<int> t = Pointer<int>(q);
    assert(Pointer<int>::refContainerSize() == 2); // still only two adresses to point to
    assert(t.getRefCount() == 3);

    // 4. Test SELF - ASSIGNMENT
    std::cout << ">>>TEST self asignment: p = p" << std::endl;
    selfAssignment(p, p);
    //  p = p; this line would lead to compiler warning
    assert(Pointer<int>::refContainerSize() == 2); // still only two adresses to point to
    Pointer<int>::showlist();

    // 5. Test assignment with PREDEFINED LHS
    std::cout << ">>>TEST assignment with predefined lhs: q = p" << std::endl;
    q = p;
    assert(Pointer<int>::refContainerSize() == 2); // still only two adresses to point to

    // 6. Test assignment with UNINITIALIZED LHS;
    std::cout << ">>>TEST assignment with uninitialized lhs: r = p" << std::endl;
    r = p;
    assert(Pointer<int>::refContainerSize() == 2); // still only two adresses to point to

    // 7. Test array default constructor
    std::cout << ">>>TEST default array constructor: Pointer<int, 3> v;" << std::endl;
    Pointer<int, 3> v;
    Pointer<int, 3>::showlist();

    v = Pointer<int, 3>(new int[3]);
    v[0] = 42;
    v[1] = 43;
    v[2] = 44;
    Pointer<int, 3>::showlist();
    assert(v.getRefCount() == 1);

    // 8. Test array (non-default-) constructor
    std::cout << ">>>TEST constructor: Pointer<int, 3> u = new int[3];" << std::endl;
    Pointer<int, 3> u = new int[3];
    u[0] = 11;
    u[1] = 12;
    u[2] = 13;
    Pointer<int, 3>::showlist();
    assert(u.getRefCount() == 1);

    // 9. Assignment of different array sized pointers shall create compiler error
    // Pointer<int, 3> w = Pointer<int, 2>(new int[3]);

    // 10. Test custom type
    Pointer<SimpleStruct> sP = new SimpleStruct{1, 2};
    Pointer<SimpleStruct> sP2 = sP;
    assert(sP.getRefCount() == 2);
    Pointer<SimpleStruct>::showlist();
}

void doubleLinkedListTest() {
    custom::list<int> list;

    std::cout << "Add 5 numbers: " << std::endl;
    for (int i = 0; i < 5; i++) {
        int temp;
        std::cout << "Add " << i + 1 << ". element:" << std::endl;
        std::cin >> temp;
        list.push_back(temp);
    }

    list.print();
}

int main() {
    doubleLinkedListTest();
    // test();
    return 0;
}
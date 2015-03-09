#include "model.hpp"

#include <iostream>

class A {
public:
    virtual ~A() {
        std::cout << "A's DESTRUCTOR IS CALLED" << std::endl;
    }

    virtual void bah() {
        std::cout << "A" << std::endl;
    }
};

class B : public A {
public:
    virtual void bah() {
        std::cout << "B" << std::endl;
    }
};

class C : public A {
public:
    virtual void bah() {
        std::cout << "C" << std::endl;
    }
};

int main() {
    std::cout << "HI!" << std::endl;
    A* a = new B();

    B* b = dynamic_cast<B*>(a);
    C* c = dynamic_cast<C*>(a);

    std::cout << "B: " << b << std::endl;
    b->bah();

    std::cout << "C: " << c << std::endl;

    delete a;
}
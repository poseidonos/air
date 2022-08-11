
#ifndef DELEGATOR_H
#define DELEGATOR_H

#include "src/api/Air.h"

struct Foo
{
    int i {10};
    double d {3.14};
};

class Delegator
{
public:
    Delegator(int call_count): call_count(call_count)
    {
        std::cout << "Delegator constructor call\n";
        foo = new Foo {};
    }
    Delegator(const Delegator& rhs): call_count(rhs.call_count)
    {
        std::cout << "Delegator copy constructor call\n";
        foo = new Foo {};
    }
    ~Delegator()
    {
        std::cout << "Delegator destructor call\n";
        if (nullptr != foo)
        {
            std::cout << "foo delete\n";
            delete foo;
            foo = nullptr;
        }
    }
    void Test(void);
    void CallByLambda(void);

private:
    int call_count {0};
    Foo* foo {nullptr};
};

#endif // #define DELEGATOR_H

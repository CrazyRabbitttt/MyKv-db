#include <iostream>
using namespace std;


class A {

public:
    A() {
        printf("construct of class A\n");
    }

    virtual void func()  = 0;           //virtual function;


    virtual void myname() {
        cout << "class A\n";
    }
private:
    int value_a;

};


class B : public A
{
public: 

    B();
    ~B();

    void func() override {
        cout << "LaLaLALa\n";
    }    

    void Name() {
        cout << "class B\n";
    }
private:
    virtual void myname() {
        cout << "class B\n";
    }

};



class C
{
public:
    C(A* a) 
    :user_(a)
    {
        cout << "Construct of class C\n";
    }

    void ChildName() {
        user_->myname();
    }

private:

    A * user_;

};

int main()
{
    B* bb;

    C c(bb);

    c.ChildName();

    exit(0);
}
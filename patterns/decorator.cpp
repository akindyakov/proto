/*
 * g++(clang++) -std=c++11
 *
 */

#include <exception>
#include <iostream>
#include <vector>
#include <string>


class TBase {
public:
    TBase(TBase* next)
        : Next(next)
    {
    }

    virtual void Do(int i) {
        if (Next != nullptr) {
            Next->Do(i);
        }
    }

private:
    TBase* Next;
};

class TFirst: public TBase {
public:
    TFirst(TBase* next)
        : TBase(next)
    {
    }

    void Do(int i) override {
        TBase::Do(i);
        std::cout << "First do: " << i << std::endl;
    }
};

class TSecond: public TBase {
public:
    TSecond(TBase* next)
        : TBase(next)
    {
    }

    void Do(int i) override {
        TBase::Do(i);
        std::cout << "Second do: " << i << std::endl;
    }
};

class TAggregator: public TBase {
public:
    TAggregator()
        : TBase(nullptr)
    {
    }

    void Add(TBase* cons) {
        Consumers.push_back(cons);
    }

    void Do(int i) override {
        for (auto cons : Consumers) {
            cons->Do(i);
        }
    }

private:
    std::vector<TBase*> Consumers;
};


int main(int argn, char** args) {
    {
        TFirst f(nullptr);
        TSecond s(&f);
        s.Do(12);
    }
    {
        TAggregator a;
        TFirst f(nullptr);
        TSecond s(nullptr);
        a.Add(&f);
        a.Add(&s);
        a.Do(12);
    }
    return 0;
}


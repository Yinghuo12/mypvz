#ifndef __TEST_OBJECT_H__
#define __TEST_OBJECT_H__

#include "CoreMinimal.h"
#include "Timer.h"



class TestObject : public Object
{
public:
    TestObject(){
        time_handle.Bind(0.5, this, &TestObject::Func, true);     //与计时器绑定
        time_handle1.Bind(1.0, this, &TestObject::Func1, true);     //与计时器绑定
    };

    void Func(){
        std::cout << "Hello World" << std::endl;
    }

    void Func1(){
        std::cout << "Bye World" << std::endl;
    }



private:
    Timer time_handle;
    Timer time_handle1;
};


#endif // __TEST_OBJECT_H__
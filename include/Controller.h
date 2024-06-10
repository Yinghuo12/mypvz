#ifndef __CONTROLLER_H__
#define ___CONTROLLER_H__

#include "CoreMinimal.h"
#include "InputComponent.h"



class Controller : public Object{

public:
    Controller();

    //创建后就需要立马执行的函数逻辑
    virtual void BeginPlay() override;

    //与用户IO交互
    void PeekInfo();

protected:
    //绑定按键
    virtual void SetupInputComponent(InputComponent *inputcomponent);

protected:
    class Camera *camera;                  //相机


private:
    InputComponent *inputcomponent;        //输入组件
}; 

#endif //__CONTROLLER_H__
#ifndef __BATTLE_CONTROLLER_H__
#define __BATTLE_CONTROLLER_H__

#include "Controller.h"
#include "Timer.h"

class BattleController : public Controller
{
public:
    BattleController();                 //绑定回调函数到计时器

    virtual void SetupInputComponent(InputComponent* inputComponent) override;
    
    /*下面这句不重写是因为要使用父类的BeginPlay()来执行SetupInputComponentInput(Component* inputComponent)
        而inputcomponent是在父类Controller中私有的,子类无法访问，
        所以不能使用子类的BeginPlay()来执行SetupInputComponentInput(Component* inputComponent) */
    
    // virtual void BeginPlay() override;  
    
    void Update();          //平移
    void Shake();           //抖动

    /* 按键触发事件 */
    void MoveRight();
    void MoveLeft();
    void ZoomIn();
    void ZoomOut();

private:
    Timer OverallTimeHandle;          //全局计时器
    Timer ShakeTimeHandle;            //抖动计时器
};

#endif  //__BATTLE_CONTROLLER_H__
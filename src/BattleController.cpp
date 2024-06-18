#include "../include/BattleController.h"
#include "../include/Camera.h"
#include "../easyx/Graphics.h"


BattleController::BattleController(){
    ShakeTimeHandle.Bind(1.f, this, &BattleController::Shake, false);
}


void BattleController::SetupInputComponent(InputComponent* inputComponent){
    //调用父类
    Controller::SetupInputComponent(inputComponent);

    inputComponent->SetMapping("MoveRight", VK_D);
    inputComponent->BindAction("MoveRight", Holding, this, &BattleController::MoveRight);

    inputComponent->SetMapping("MoveLeft", VK_A);
    inputComponent->BindAction("MoveLeft", Holding, this, &BattleController::MoveLeft);

    inputComponent->SetMapping("ZoomIn", VK_Q);
    inputComponent->BindAction("ZoomIn", Holding, this, &BattleController::ZoomIn);

    inputComponent->SetMapping("ZoomOut", VK_E);
    inputComponent->BindAction("ZoomOut", Holding, this, &BattleController::ZoomOut);

}


// void BattleController::BeginPlay(){
//     ;
// }



//平移
void BattleController::Update(){
    // Controller::Update();
    // if(OverallTimeHandle.GetDelay()>2.f 
    //     && OverallTimeHandle.GetDelay()<=5.f 
    //     && GetWorldPosition().x < 600) {

    //         AddPosition(Vec2D(5.f, 0));
    // }

    // if(OverallTimeHandle.GetDelay()>7.f
    //     && GetWorldPosition().x >= 225.f) {

    //         AddPosition(Vec2D(-5.f, 0)); 
    // }
}


//抖动
void BattleController::Shake(){
    // camera->ShakeCamera(10.f, 1);
}



/* 按键触发事件 */
void BattleController::MoveRight(){
    AddPosition(Vec2D(0.5f, 0));
}

void BattleController::MoveLeft(){
    AddPosition(Vec2D(-0.5f, 0));
}

void BattleController::ZoomIn(){
    camera->SetSpringArmLength(camera->springArmLength - 0.05f);
}

void BattleController::ZoomOut(){
    camera->SetSpringArmLength(camera->springArmLength + 0.05f);
}
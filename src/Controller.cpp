#include "../include/Controller.h"
#include "../include/Camera.h"
#include "../include/Math.h"

Controller::Controller(){
    camera = ConstructComponent<Camera>();
    camera->AttachTo(root);
    camera->SetMainCamera();

    inputcomponent = ConstructComponent<InputComponent>();
    
}


//创建后就需要立马执行的函数逻辑
void Controller::BeginPlay(){
    //Q:为什么这句不能写在构造函数里直接调用？
    //A:因为BeginPlay函数是虚函数，在构造函数里调用的话，会调用父类的BeginPlay函数，而父类没有实现这个函数，所以会报错
    //所以需要将BeginPlay函数的调用放在一个创建后就要立马执行的BeginPlay函数里
    SetupInputComponent(inputcomponent);
}


//与用户IO交互
void Controller::PeekInfo() {
    inputcomponent->Update(); 
}


//获取鼠标位置
Vec2D Controller::GetCursorPosition() const {
    return inputcomponent->GetMousePosition();
}


//获取鼠标下的物体
Object* Controller::GetObjectUnderCursor(){
    Vec2D pos = inputcomponent->GetMousePosition();
    int x = (int)(pos.x) / 100; x = Math::Clamp(x, 0, 7);
    int y = (int)(pos.y) / 100; y = Math::Clamp(y, 0, 5);

    if (!mainWorld.ColliderZones[x][y].empty()){
        //因为ColliderZones容器按照图层顺序排序，我们倒着找位于图层最上方的物体
        for(auto it = mainWorld.ColliderZones[x][y].rbegin(); it != mainWorld.ColliderZones[x][y].rend(); ++it){
            if ((*it)->IsMouseOver())     //如果被鼠标选中
                return (*it)->owner;        
        }
    }
    return nullptr;
}



//这个是虚函数，不能放在构造函数中调用，因此放在BeginPlay函数中，创建对象后立即调用
void Controller::SetupInputComponent(InputComponent *inputcomponent){
    ;
}
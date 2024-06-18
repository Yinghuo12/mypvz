#ifndef __INPUT_COMPONENT_H__
#define __INPUT_COMPONENT_H__

#include "CoreMinimal.h"
#include <map>
#include <functional>
#include <utility>



//键位码
enum keyboard
{
    VK_LButton = 1, // 左键
    VK_RButton = 2, // 右键
    VK_A = 0x41,
    VK_D = 0x44,
    VK_Q = 0x51,
    VK_E = 0x45

};

//事件触发类型
enum InputType{
    Pressed,    //按下触发类型
    Released,   //松开触发类型
    Holding     //长按触发类型
};




//按键绑定类
class KeyBindInfo{
public:
    KeyBindInfo(){}
    KeyBindInfo(std::function<void()> action, InputType type, bool isPressed = false) : action(action), type(type), isPressed(isPressed){}


    //拷贝
    KeyBindInfo(const KeyBindInfo& other){
        this->action = other.action;
        this->type = other.type;
        this->isPressed = other.isPressed;
    }

   
    //赋值
    KeyBindInfo& operator=(const KeyBindInfo& other){
        this->action = other.action;
        this->type = other.type;
        this->isPressed = other.isPressed;
        return *this;
    }

    

public:
    std::function<void()> action;  //触发事件本身
    InputType type;                //触发类型
    bool isPressed = false;        //检测是否被压下
};



class InputComponent : public Component
{
public:
    //按键映射虚拟键值
    void SetMapping(std::string mappingName, int value);

    //绑定按键回调事件
    template<typename T>
    void BindAction(std::string mappingName, InputType type, T*obj, void(T::*func)()){
        if(keymappings.find(mappingName) != keymappings.end()){
            KeyBindInfo Temp = KeyBindInfo(std::bind(func, obj), type, false);
            callbacks.insert({ mappingName, Temp });
        }
    }

    //获取鼠标位置
    Vec2D GetMousePosition();

    virtual void Update() override; 

private:
    std::map<std::string, int> keymappings;          //按键映射虚拟键值
    std::map<std::string, KeyBindInfo> callbacks;    //按键回调事件映射
    Vec2D mousePos;                                  //鼠标位置
};



#endif // __INPUT_COMPONENT_H__
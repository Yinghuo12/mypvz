#include "../include/InputComponent.h"
#include <windows.h>


//按键映射虚拟键值
void InputComponent::SetMapping(std::string mappingName, int value){
    keymappings.insert({mappingName, value});
}



//获取鼠标位置
Vec2D InputComponent::GetMousePosition(){
    return mousePos;
}



void InputComponent::Update(){
    /* 1.处理鼠标 */
    POINT point;        
    GetCursorPos(&point);   //获取鼠标当前位置
    mousePos = Vec2D(float(point.x), float(point.y));

    /* 2.处理键盘 */
    //遍历检测按键状态
    for (auto& keymapping : keymappings) {
        KeyBindInfo& info = callbacks[keymapping.first];
        if(GetAsyncKeyState(keymapping.second) & 0x8000){      //检测到按键压下
            if(info.type == Pressed && !info.isPressed) {       //如果之前没被压且属于按下触发类型
                info.action();
            }
            if(info.type == Holding) {             //如果属于长按类型，不管之前有没有被压都触发
                info.action();
            }
            info.isPressed = true;
        }
        else if(info.isPressed){                  //未压下
            if(info.type == Pressed){     // 如果属于按下触发类型 
                ;
            }              
            else if(info.type == Released) {      // 如果属于松开触发类型
                info.action();
            }    
            info.isPressed = false;
        }
    }
}
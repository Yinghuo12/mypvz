#ifndef __RIGID_BODY_H__
#define __RIGID_BODY_H__

#include "CoreMinimal.h"

/* 刚体组件 */
//只是为了操控所属的游戏对象Object的坐标内容，而本身的坐标内容无意义，因此无需继承SceneComponent，仅继承Component
class RigidBody : public Component{
public:
    virtual void Update() override;

    
    void SetMoveable(bool moveable);       //设置是否可移动
    void SetGravityEnabled(bool enabled);  //设置是否开启重力
    void SetRotatable(bool rotatable);     //设置是否可旋转

    
    void SetGravity(float g);          //设置重力加速度
    void SetVelocity(Vec2D v);         //设置速度
    void SetAngularVelocity(float angularVelocity);  //设置角速度
    void AddForce(Vec2D force);        //添加作用力

private: 
    bool bMoveable = true;          //是否可移动
    bool bGravityEnabled = true;    //是否启用重力
    bool bRotatable = true;         //是否可旋转

    Vec2D velocity = Vec2D(0,0);     //速度
    float gravity = 9.81f;           //重力加速度
    float angular_velocity = 0.0f;   //角速度


    
};

#endif   //__RIGID_BODY_H__
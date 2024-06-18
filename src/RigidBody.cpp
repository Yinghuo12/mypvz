#include "../include/RigidBody.h"
#include "../include/Macro.h"

void RigidBody::Update(){
    //是否可移动
    if(bMoveable){
        owner->AddPosition(velocity);
        //是否启用重力
        if(bGravityEnabled)   velocity.y += gravity * DELTA_TIME;
    }
    //是否可旋转
    if(bRotatable){
        owner->AddRotation(angular_velocity);
    }
}


//设置是否可移动
void RigidBody::SetMoveable(bool moveable){
    bMoveable = moveable;
}

//设置是否开启重力
void RigidBody::SetGravityEnabled(bool enabled){
    bGravityEnabled = enabled;
}

//设置是否可旋转
void RigidBody::SetRotatable(bool rotatable){
    bRotatable = rotatable;
}



//设置重力加速度
void RigidBody::SetGravity(float g){
    gravity = g;
}
    

//设置速度
void RigidBody::SetVelocity(Vec2D v){
    velocity = v;
}

//设置角速度
void RigidBody::SetAngularVelocity(float angularVelocity){
    angular_velocity = angularVelocity;
}

//添加作用力
void RigidBody::AddForce(Vec2D force){
    velocity += force * DELTA_TIME;
}      



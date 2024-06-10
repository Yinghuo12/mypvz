#include "../include/Camera.h"
#include "../include/Math.h"



//设置平滑度
void Camera::SetSmoothness(short smoothness){
    this->smoothness = Math::Clamp(smoothness, 0, 100);
}


//设置距离阈值
void Camera::SetDistanceTreshold(float distanceTreshold){
    this->distanceTreshold =  Math::Clamp(distanceTreshold, 0.f, 500.f);
}


//把世界中Camera的掌控权归给自己
void Camera::SetMainCamera() {
    mainWorld.mainCamera = this;
}


//获取相机位置
Vec2D Camera::GetCameraPosition(){
    return virtual_transform.position;
}



//插值函数(Vec2D)
Vec2D Camera::Lerp(Vec2D start, Vec2D end, float t){
    t = Math::Clamp(t, 0.001f, 0.1f);   //为了实现平滑
    return start + (end - start) * t;
}
//插值函数(float)
float Camera::Lerp(float start, float end, float t){
    return start + (end - start) * t;
}


//函数 x^(smoothness/100)
float Camera::SmoothStep(float x){
    x = x < 1 ? 1 : x;        //若 距离比（实际距离/阈值距离）< 1, 则在规定范围内,x=1  否则，x > 1
    return pow(x, float(smoothness) / 100);
}

//设置弹簧长度
void Camera::SetSpringArmLength(float armLength){
    springArmLength += armLength;
}


//抖动相机
void Camera::ShakeCamera(float intensity, int decay){
    intensity = Math::Clamp(intensity, 0, 100);
    decay = Math::Clamp(decay, 1, 100);
    shakeIntensity = (float)intensity;
    shakeDecay = decay;
    if(!shakeFlag){
        shakeCenter = GetLocalPosition();
        shakeFlag = true;
    }
}



//低频率低精度
void Camera::Update(){
    ;
}



//高频率高精度
void Camera::Calculate(){

    static bool first = true;
    if(first){
        virtual_transform.position = GetWorldPosition();
        virtual_transform.rotation = GetWorldRotation();
        virtual_springArmLength = springArmLength;
        first = false;
    }  //初始化虚拟参数,只执行一次


    /*** 1.相机移动 ***/
    if(smoothness)   //平滑则平滑移动到实际位置
        virtual_transform.position = Lerp(virtual_transform.position,  GetWorldPosition(), 
            0.1f /smoothness * SmoothStep(Vec2D::Distance(virtual_transform.position, GetWorldPosition())/distanceTreshold));
  
    else    //不平滑则直接变为实际位置
        virtual_transform.position = GetWorldPosition();


    /*** 2.相机缩放 ***/
    if(smoothnessForSpringArm)
        virtual_springArmLength = Lerp(virtual_springArmLength, springArmLength, 0.1f/smoothnessForSpringArm);
    
    else    //不平滑则直接变为实际位置
        virtual_springArmLength = springArmLength;
    


    /*** 3.相机抖动 ***/
    if(shakeFlag){
        //抖动相机
        if(GetLocalPosition() == shakeCenter){
            shakeSpeed.x = float(Math::RandomInt(-1, 1));
            shakeSpeed.y = float(Math::RandomInt(-1, 1));
            shakeIntensity -= float(shakeDecay) / 100.f;

            //抖动强度衰减为0以下
            if(shakeIntensity <= 0){
                shakeFlag = false;
                return;
            }
        }

        //抖动中心在抖动半径之内
        if(Vec2D::Distance(GetLocalPosition(), shakeCenter) < shakeIntensity){
            //虚拟相机和真实相机都要移动
            AddPosition(shakeSpeed);
            virtual_transform.position += shakeSpeed;
        }
            
        else{
            shakeSpeed = -shakeSpeed;
            //虚拟相机和真实相机都要移动
            AddPosition(shakeSpeed);
            virtual_transform.position += shakeSpeed;
        }
    }

}



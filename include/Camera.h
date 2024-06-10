#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "CoreMinimal.h"

class Camera: public SceneComponent{
    friend class SpriteRenderer;   //for SpriteRenderer::Render()

public:
 
    void SetSmoothness(short smoothness);                //设置平滑度
    void SetDistanceTreshold(float distanceTreshold);    //设置距离阈值

    //把世界中Camera的掌控权归给自己
    void SetMainCamera();

    //获取相机位置
    Vec2D GetCameraPosition();

    //高精度计算
    void Calculate();

    //抖动相机
    void ShakeCamera(float intensity, int decay = 20);

    //设置弹簧长度
    void SetSpringArmLength(float length);


    virtual void Update() override;

private:
    //插值函数 t=0相机不动 t=1相机移动到目标位置
    Vec2D Lerp(Vec2D start, Vec2D end, float t);
    float Lerp(float start, float end, float t);

    //函数
    float SmoothStep(float x);

private:
    Transform virtual_transform;                 //虚拟相机位置

    /* 相机移动 */
    float distanceTreshold = 50.f;               //距离阈值0-500（让相机与对象保持在这个距离左右）
    short smoothness = 50;                       //平滑度0-100

    /* 相机抖动*/
    float shakeIntensity = 0.f;                   //相机抖动强度0-100
    bool shakeFlag = false;                      //抖动标记
    Vec2D shakeCenter = Vec2D();                 //抖动中心
    Vec2D shakeSpeed = Vec2D(0.05f, 0.05f);      //抖动速度
    int shakeDecay = 5;                          //抖动衰减系数1-100

    /* 相机缩放 */
    float springArmLength = 50.f;           //弹簧长度1—10000
    float virtual_springArmLength = 100.f;  //虚拟弹簧长度
    float smoothnessForSpringArm = 20.f;    //弹簧平滑度

};

#endif  // __CAMERA_H__
#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "CoreMinimal.h"
#include "VisualInterface.h"
#include <chrono>
#include <deque>

//粒子模式
enum class ParticlePattern
{
    Center, // 中心放射状粒子
    Line    // 线性放射状粒子
};



//单个粒子信息
class ParticleInfo{
public:
    //构造函数
    ParticleInfo(){
        lastTime = std::chrono::steady_clock::now();
    }
public:
    Vec2D offset;            //相对于粒子发生器的相对偏移
    int index;               //当前帧索引
    Vec2D velocity;          //速度
    std::chrono::time_point<std::chrono::steady_clock> lastTime;   //粒子创建的时间
    float transparency = 255.f;                                    // 透明度
};



//粒子组件
class Particle : public SceneComponent, public ImageInterface{
public:
    void Load(std::string name);      //从资源库中加载粒子资源
    virtual void Update() override;   //更新粒子
    virtual void Render() override;   //渲染粒子

    //设置参数
    void SetCapacity(int capacity);
    void SetGravity(float gravity);
    void SetLifeCycle(float lifeCycle);
    void SetIsLoop(bool isLoop);
    void SetInterval(float interval);
    void SetFadingTime(float fadingTime);
    void SetSpeed(float speed);

    //设置参数
    void SetPattern(ParticlePattern pattern);
    void SetCenter(Vec2D radius, Vec2D scoop = Vec2D(0,360));
    void SetLine(float length, float angle);

private:
    void Produce();     //生产粒子  

private: 
    IMAGE** images = nullptr;     //粒子帧数组
    int number = 0;               //粒子帧数

    std::deque<ParticleInfo> particles;   //粒子容器
    int capacity = 0;                     //粒子容量

    std::chrono::time_point<std::chrono::steady_clock> lastTime;   //用于记录生产的时间点

    float speed = 1.f;               //粒子速度（标量）
    float gravity = 9.81f;           //重力加速度

    bool isLoop = true;             //是否持续生产
    float interval = 0.1f;           //生产间隔

    float lifeCycle = 1.f;           //粒子生命周期
    float fadingTime = 0;            //粒子逐渐消失的时间
    

    ParticlePattern pattern = ParticlePattern::Center;       //粒子模式(默认放射状)
    Vec2D unitVec = Vec2D(0, 1);                             //粒子方向单位向量
    /* 中心放射状属性成员 */
    Vec2D radius = Vec2D(0, 0);    //(内径，外径)
    Vec2D scoop = Vec2D(0,360);    //放射范围

    /* 线性放射状属性成员 */
    float length = 1.f;     //放射长度
    float angle = 0.f;      //放射角度
};

#endif // __PARTICLE_H__
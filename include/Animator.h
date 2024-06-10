#ifndef __ANIMATOR_H__
#define __ANIMATOR_H__

#include "CoreMinimal.h"
#include "../easyx/easyx.h"
#include "Timer.h"
#include <unordered_map>



//动画类
class Animation{
    friend class Animator;

public:
    Animation();                                             //绑定动画帧更新函数到计时器回调函数
    void Load(std::string name, POINT delta  = {0,0});       //加载动画资源
    void SetInterval(double interval);                       //设置计时器更新频率
    void SetIndex(int i);                                    //设置动画下标

private:
    void UpdateIndex();              //动画帧更新

private:
    int num = 0;                    //动画帧数
    int index = 0;                  //当前帧
    IMAGE **images = nullptr;       //动画帧数组
    Timer clock;                    //计时器
    POINT offset = {0, 0};          //偏移量
};





//动画播放器
class Animator final:public Component{

public:
    virtual void Update() override;
    void Insert(std::string name, Animation &ani);    //传入动画
    void SetNode(std::string nodeName);               //设定当前播放的动画
    Animation* GetNode() const;                       //获取当前播放的动画
    void SetCalled(bool called);                      //是否播放

private:
    std::unordered_map<std::string, Animation &> animations;    //动画容器
    Animation *aniNode = nullptr;                               //当前播放的动画指针
    class SpriteRenderer *rendererAttached = nullptr;           //附着的渲染器
};

#endif   // __ANIMATOR_H__
#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include <unordered_map>
#include <string>
#include "../easyx/easyx.h"

//动画资源类
struct AnimationResource{
    IMAGE **images;    //指向存放元素类型为IMAGE*的数组，
    int num;           //动画资源的图片数量
};


class ResourceManager
{
public:

    //初始化
    void Initialize();

    //获取图片
    IMAGE* FetchImage(std::string name);

    //获取动画
    AnimationResource FetchAnimation(std::string name);


    ~ResourceManager();

private:

    void Load(std::string name, std::string path, int w, int h);                             //加载单张图片
    void Load(std::string name, std::string path, int w, int h, int num);                    //加载多张动画图片
    void Load(std::string name, std::string path, int w, int h, int num, int row, int col);  //加载SpriteSheet动画图片


private:
    std::unordered_map<std::string, IMAGE*> imagePool;                 //图片资源池
    std::unordered_map<std::string, AnimationResource> animationPool;  //动画资源池
};

#endif  // __RESOURCE_MANAGER_H__
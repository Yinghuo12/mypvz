#ifndef __VISUAL_INTERFACE_H__
#define __VISUAL_INTERFACE_H__

#include <windows.h>
#include <string>
#include <set>

class IMAGE;
typedef unsigned char BYTE;

class LayerInterface
    {
    public:

        LayerInterface();
        virtual ~LayerInterface();
        void SetLayer(int layer);
        int GetLayer() const;

        virtual void Render() = 0;

    private:
        int layer = 0;
};




//渲染参数
typedef class SpriteInfo{
public:
    POINT offset = {0, 0};
} spriteInfo;


//滤镜参数
typedef class FilterInfo{
public:
    FilterInfo(){};
    FilterInfo(COLORREF col, int level){
        this->color = col;
        this->level = level;
    }
    FilterInfo(COLORREF col, int level, int layer){
        this->color = col;
        this->level = level;
        this->layer = layer;
    }

public:
    COLORREF color = 0x00000000; //滤镜颜色
    int level = 50;              //颜色过渡层级1-100
    int layer = 0;               //滤镜图层(0和1)  //一个是寒冰蓝色层，一个是被子弹打中的闪光层
} filterInfo;



//滤镜图层比较器
struct FilterCmp{
    bool operator()(const FilterInfo &a, const FilterInfo &b) const{
        return a.layer < b.layer;
    }
};


class ImageInterface:public LayerInterface {

public:
    //获取透明度
    BYTE GetTransparency() const;

    //设置透明度
    void SetTransparency(BYTE transparency);

    //加载非动画资源
    void LoadSprite(std::string name);

    //设置滤镜
    void SetFilter(bool enabled, COLORREF col = 0x00000000, int level = 60);

    //添加滤镜
    void AddFilter(FilterInfo filterInfo);

    //移除滤镜
    void RemoveFilter();

    virtual ~ImageInterface();

protected:
    //旋转图像
    void RotateImage(double rad);

    //滤镜图像
    void FilterImage();

protected:
    IMAGE* sprite = nullptr;
    IMAGE* copy = nullptr;
    IMAGE* filter = nullptr;     //滤镜图像
    BYTE transparency = 255;      //透明度
    float angle = 0;             //角度,用于旋转
    SpriteInfo spriteInfo;       //渲染参数结构体
    std::set<FilterInfo, FilterCmp> filterLayers;   //多个图层，实现多层滤镜效果
    // bool bFilterEnabled = false; //是否开启滤镜
    

    
};

#endif // __VISUAL_INTERFACE_H__
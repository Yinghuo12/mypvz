#ifndef __VISUAL_INTERFACE_H__
#define __VISUAL_INTERFACE_H__

#include <windows.h>
#include <string>


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



class ImageInterface:public LayerInterface {

public:
    //获取透明度
    BYTE GetTransprancy() const;

    //设置透明度
    void SetTransprancy(BYTE transprancy);

    //加载非动画资源
    void LoadSprite(std::string name);

    //设置滤镜
    void SetFilter(bool enabled, COLORREF col = 0, int level = 5);

    virtual ~ImageInterface();

protected:
    //旋转图像
    void RotateImage(double rad);

    //添加滤镜
    void FilterImage();

protected:
    IMAGE *sprite = nullptr;
    IMAGE *copy = nullptr;
    BYTE transprancy = 255;      //透明度
    float angle = 0;             //角度,用于旋转
    bool bFilterEnabled = false; //是否开启滤镜

    //滤镜参数
    struct FilterInfo{
        COLORREF color = 0;         //滤镜颜色
        int level = 5;              //颜色过渡层级1-10
        IMAGE *filter = nullptr;
    } filterInfo;

    //渲染参数
    struct SpriteInfo{
        POINT offset = {0, 0};
    } spriteInfo;
};

#endif // __VISUAL_INTERFACE_H__
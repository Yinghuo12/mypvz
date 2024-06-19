#include "../include/VisualInterface.h"
#include "../include/CoreMinimal.h"
#include "../include/ResourceManager.h"

#include "../easyx/easyx.h"
#include "../include/Math.h"

LayerInterface::LayerInterface(){
    mainWorld.game_renderers.insert(this);
}

LayerInterface::~LayerInterface(){
    mainWorld.game_renderers.erase(this);
}

void LayerInterface::SetLayer(int layer){
    mainWorld.game_renderers.erase(this);
    this->layer = layer;
    mainWorld.game_renderers.insert(this);   //默认进行了排序（set底层以layer的大小进行排序）
}


int LayerInterface::GetLayer() const {
    return layer;
}





//获取透明度
BYTE ImageInterface::GetTransparency()const{
    return transparency;
}

//设置透明度
void ImageInterface::SetTransparency(BYTE transparency){
    this->transparency = transparency;
}

//加载非动画资源
void ImageInterface::LoadSprite(std::string name){
    sprite = mainWorld.resourcePool->FetchImage(name);
}



//设置滤镜
void ImageInterface::SetFilter(bool enabled, COLORREF col, int level){
    //若enabled = false，则把第零层滤镜移除
    //若enabled = true, 则第零层滤镜移除后，给第零层添加回一个新的滤镜
    filterLayers.erase(FilterInfo{col, level});
    if(enabled){
        level = Math::Clamp(level, 0, 100);
        filterLayers.insert(FilterInfo{col, level});
    }
}



//添加滤镜
void ImageInterface::AddFilter(FilterInfo filterInfo){
    filterLayers.insert(filterInfo);
}

//移除滤镜
void ImageInterface::RemoveFilter(){
    //把第一层的滤镜移除
    filterLayers.erase(FilterInfo{0x00000000, 60, 1});  //前两个参数随意传，最后一个是第1层
}





//旋转图像
void ImageInterface::RotateImage(double rad){
    float fSin = (float)sin(rad), fCos = (float)cos(rad);
    int w = sprite->getheight(), h = sprite->getheight();
    POINT points[4] = {{0, 0}, {w, 0}, {0, h}, {w, h}};   //旋转前图像的四个坐标

    //旋转后图像的包围盒
    int min_x = 0, min_y = 0;
    int max_x = 0, max_y = 0;
    for (int i = 0; i < 4; i++){
        points[i] = {
            (int)(points[i].x * fCos - points[i].y * fSin),
            (int)(points[i].x * fSin + points[i].y * fCos)
        };
        min_x = (int)(std::min((LONG)min_x, points[i].x));
        min_y = (int)(std::min((LONG)min_y, points[i].y));
        max_x = (int)(std::max((LONG)max_x, points[i].x));
        max_y = (int)(std::max((LONG)max_y, points[i].y));
    }

    //旋转后图像包围盒的宽度和高度以及像素个数
    int nw = max_x - min_x;
    int nh = max_y - min_y;
    int nSize = nw * nh;

    if(!copy)
        copy = new IMAGE(nw, nh);
    copy->Resize(nw, nh);
    
    DWORD *dst_buffer = GetImageBuffer(copy);
    DWORD *src_buffer = GetImageBuffer(sprite);

    //遍历
    for (int i = min_x, ni = 0; ni < nw; ++i,++ni){                //i用于映射源图像坐标，ni用于定位旋转后的图像坐标
        for (int j = min_y, nj = 0; nj < nh; ++j, ++nj){
            dst_buffer[nj * nw + ni] = 0;
            //需要从原像素坐标系转换到旋转后图片的像素坐标系(以左上角为原点)
            int nx = (int)(i * fCos + j * fSin);     //从旋转后的图像向源图像坐标映射
            int ny = (int)( j * fCos - i * fSin);

            if(nx>=0 && nx<=w && ny>=0 && ny<h){  //若目标映射在源图像范围内，则拷贝色值
                dst_buffer[nj * nw + ni] = src_buffer[ny * w + nx];
            }
        }
    }
}




//滤镜图像
void ImageInterface::FilterImage(){
    IMAGE *img = copy ? copy : sprite;
    if(!filter){
        filter = new IMAGE(img->getwidth(), img->getheight());
    }
    filter->Resize(img->getwidth(), img->getheight());

    const DWORD* src_buffer = GetImageBuffer(img);
    DWORD *dst_buffer = GetImageBuffer(filter);
    int num = img->getheight() * img->getwidth();

    for(int i = 0; i < num; ++i){
        if(src_buffer[i] >> 24){   //有透明度即存在像素
            //获取BGR
            DWORD dst_bufferB = src_buffer[i] & 0xFF;
            DWORD dst_bufferG = (src_buffer[i] & 0xFF00) >> 8;
            DWORD dst_bufferR = (src_buffer[i] & 0xFF0000) >> 16;

            for(auto &filterInfo: filterLayers){
                int level = filterInfo.level;
                if(src_buffer[i] >> 24 < 250){
                    //使得颜色滤镜从低透明度到高透明度的平滑过渡
                    level = (src_buffer[i] >> 24) * level >> 8;  
                }
                //将颜色进行平均化(level越小滤镜效果越差)
                dst_bufferB = (dst_bufferB * (128-level) + GetBValue(filterInfo.color) * level) >> 7;
                dst_bufferG = (dst_bufferG * (128-level) + GetBValue(filterInfo.color) * level) >> 7;
                dst_bufferR = (dst_bufferR * (128-level) + GetBValue(filterInfo.color) * level) >> 7;
            }
            

            //重新组合成一个新的颜色值BGR
            dst_buffer[i] = (src_buffer[i] & 0xFF000000) | (dst_bufferR << 16) | (dst_bufferG << 8) | dst_bufferB;
        }
        else  dst_buffer[i] = 0;
    }
}





ImageInterface::~ImageInterface(){
    if(copy){
        delete copy;
        copy = nullptr;
    }
    if(filter){
        delete filter;
        filter = nullptr;
    }  
}
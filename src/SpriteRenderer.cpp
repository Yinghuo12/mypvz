#include "../include/SpriteRenderer.h"
#include "../easyx/graphics.h"
#include "../easyx/easyx.h"
#include "../include/Camera.h"


void SpriteRenderer::Render(){

    if(sprite){

        //世界坐标-相机绝对坐标 = 相对于相机左上角位置（即真正需要渲染在屏幕上的位置）
        Vec2D pos = (GetWorldPosition() - mainWorld.mainCamera->virtual_transform.position)*20.f / mainWorld.mainCamera->virtual_springArmLength + Vec2D(WIN_WIDTH / 2, WIN_HEIGHT / 2) ;  //获取坐标(以图像中心为坐标) 把该向量平移到相机左上角，使w,h都是正值
        HDC dstDC = GetImageHDC();          //目标窗口句柄

        IMAGE *img = copy ? copy : sprite;    //copy指针为空，则操作sprite,否则操作copy
        
        HDC srcDC = GetImageHDC(img);    //源图片句柄

        //源图像高度和宽度
        int w = sprite->getwidth();
        int h = sprite->getheight();

        //目标图像的高度和宽度
        int dw = int(w*GetWorldScale().x*20.f/mainWorld.mainCamera->virtual_springArmLength);  //弹簧长度与游戏对象的规模成反比，默认长度为20
        int dh = int(h*GetWorldScale().y*20.f/mainWorld.mainCamera->virtual_springArmLength);

    
        BLENDFUNCTION bf = {AC_SRC_OVER, 0, transprancy, AC_SRC_ALPHA};
        if (bFilterEnabled && filterInfo.filter){
            HDC srcDC_filter = GetImageHDC(filterInfo.filter);
            AlphaBlend(dstDC, (int)pos.x - dw/2, (int)pos.y -dh/2, dw, dh, srcDC_filter, 0, 0, w, h, bf);
        }
        else{
            AlphaBlend(dstDC, (int)pos.x - dw/2, (int)pos.y -dh/2, dw, dh, srcDC, 0, 0, w, h, bf);
        }
    }
}



void SpriteRenderer::Update(){
    if (GetWorldRotation() != angle){
        angle = GetWorldRotation();
        if(sprite){
            RotateImage(PI * angle / 180.f);
        }
    }
    if(sprite && bFilterEnabled){
        FilterImage();
    }
}




StaticMesh::StaticMesh(){
    renderer = ConstructComponent<SpriteRenderer>();
    renderer->AttachTo(root);
}


void StaticMesh::BeginPlay(){
    ;
}
#include "../include/ResourceManager.h"

void ResourceManager::Initialize(){
    /* 资源图像加载 */
    Load("bg0", "../resource/images/bg0.jpg", 1400, 600);
    Load("bg1", "../resource/images/bg1.jpg", 1400, 600);
    Load("sunshine", "../resource/animations/bullets/sun.png", 483, 159, 12, 2, 6);

}


//获取图片
IMAGE* ResourceManager::FetchImage(std::string name){
    return (imagePool.find(name))->second;
}

//获取动画
AnimationResource ResourceManager::FetchAnimation(std::string name){
    return (animationPool.find(name))->second;
}

//加载单张图片
void ResourceManager::Load(std::string name, std::string path, int w, int h){
    IMAGE *img = new IMAGE;
    loadimage(img, LPCTSTR(path.c_str()), w, h);
    imagePool.insert({name, img});
}


//加载多张动画图片
void ResourceManager::Load(std::string name, std::string path, int w, int h, int num){
    IMAGE **images = new IMAGE*[num];
    for (int i = 0; i < num; i++){
        images[i] = new IMAGE;
        loadimage(images[i], LPCTSTR((path + "/" + std::to_string(i) + ".png").c_str()), w, h);
    }
    animationPool.insert({name, {images,num}});
}


//加载SpriteSheet动画图片 
void ResourceManager::Load(std::string name, std::string path, int w, int h, int num, int row, int col){
    IMAGE **images = new IMAGE*[num];      //子图片容器
    IMAGE img;                             //priteSheet图片容器

    loadimage(&img, LPCTSTR(path.c_str()), w, h);    //先把一整张SpriteSheet图片加载进来
    DWORD* src = GetImageBuffer(&img);   //指向SpriteSheet图片像素容器的首地址
    
    int wid = w / col;   //每帧的宽度
    int hei = h / row;   //每帧的高度

    //第i行j列个子图片
    for (int i = 0; i < row; ++i){
        for (int j = 0; j < col; ++j){
            int x = j * wid;     //子图片的左上角像素x坐标
            int y = i * hei;     //子图片的左上角像素y坐标
            int index = i * col + j;   //子图片在数组中的索引
            if (index >= num) {   //如果索引超过总帧数，则不加载
                break;
            }
            images[index] = new IMAGE(wid, hei);   //为子图片分配内存
            DWORD* dst = GetImageBuffer(images[index]);   //指向子图片像素容器的首地址

            //遍历子图片的第sub_row行sub_col列个像素
            for (int sub_col = 0; sub_col < wid; ++sub_col) {
                for (int sub_row = 0; sub_row < hei; ++sub_row){
                    dst[sub_row*wid + sub_col] = src[(y+sub_row)*w + (x+sub_col)];   //将子图片的像素复制到子图片的像素容器中
                }
            }
        }
    }
    animationPool.insert({name, {images, num}});  //把构成SpriteSheet的所有子图片插入容器
}


//释放资源
ResourceManager::~ResourceManager(){
    for (auto &image : imagePool){
        if(image.second){
            delete image.second;
            image.second = nullptr;
        }
    }
    for (auto &animation : animationPool){
        for (int j = 0; j < animation.second.num; j++){
            if(animation.second.images[j]){
                delete animation.second.images[j];
                animation.second.images[j] = nullptr;
            }
        }
        delete[] animation.second.images;
    }
}


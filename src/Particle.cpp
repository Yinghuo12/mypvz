#include "../include/Particle.h"
#include "../include/ResourceManager.h"
#include "../include/Macro.h"
#include "../include/Math.h"
#include "../include/Camera.h"

//从资源库中加载粒子资源
void Particle::Load(std::string name){
    AnimationResource aniRes = mainWorld.resourcePool->FetchAnimation(name);
    number = aniRes.num;
    images = aniRes.images;
}

//生产粒子 
void Particle::Produce(){
    ParticleInfo temp;
    temp.transparency = transparency;    //后者来自ImageInterface
    temp.index = Math::RandomInt(0, number - 1);
    //中心放射
    if(pattern == ParticlePattern::Center){
        double radian = Math::RandomReal(scoop.x, scoop.y) * PI / 180;
        unitVec = Vec2D(cos(radian), sin(radian));
        if(radius != Vec2D(0,0)){     //圆环内外径不为0
            //所创建的粒子坐标为圆环中心坐标+该方向单位向量*内径+内外径差值*(0-1的一个数)*该方向单位向量
            temp.offset = unitVec * (std::min(radius.x, radius.y) + std::abs(radius.y - radius.x)*Math::RandPerc());   //粒子位置
            temp.velocity = unitVec * speed;                                                                           //粒子速度
        }
    }
    //线性放射
    else{
        double radian = angle * PI / 180;
        unitVec = Vec2D(cos(radian), sin(radian));
        Vec2D lineVec = Vec2D::RotateVec(90, unitVec);    //法线
        if(length){     //线段长度不为0
            //所创建的粒子坐标为线段起点+该方向单位向量*线段长度*(0-1的一个数)
            temp.offset = lineVec * (-length / 2 + length*Math::RandPerc());   //粒子位置
            temp.velocity = lineVec * speed;                                   //粒子速度
        }
    }
    particles.push_back(temp);

}  


//更新粒子
void Particle::Update(){
    /* 1.生产管理 */
    if(particles.size()< capacity){
        //需要瞬时产生粒子
        if(interval == 0){
            for (int i = 0; i < number; i++){
                Produce();
            }
        }
        //需要间隔产生粒子
            //超过间隔则生产
        else if(std::chrono::duration<float>(std::chrono::steady_clock::now() - lastTime).count() >= interval){
            Produce();
            lastTime = std::chrono::steady_clock::now();
        }

        //如果不是持续生产 且 粒子数量达到上限 则不再生产
        if(!isLoop && particles.size() >= capacity){
            capacity = -1;     //之后则永远也不会进入Update进行创建粒子操作
        }
    }

    /* 2.销毁管理 */
    if(particles.size()){
        //如果是瞬时生产的粒子则所有粒子一起销毁/过渡
        if(interval == 0){
            //瞬时销毁逻辑
            if(std::chrono::duration<float>(std::chrono::steady_clock::now() - particles.begin()->lastTime).count() >= lifeCycle){   //取第一个例子的时间即所有粒子的时间
                particles.clear();
            }
            //瞬时过渡逻辑
            else if(fadingTime && std::chrono::duration<float>(std::chrono::steady_clock::now() - particles.begin()->lastTime).count() >= lifeCycle - fadingTime){
                for(auto &each:particles){
                    each.transparency -= transparency * DELTA_TIME / fadingTime;   //后者的transparency是ImageInterface中的
                }
            }
        }
        //如果是间隔生产粒子则每个粒子单独销毁/过渡
        else{
            //间隔销毁逻辑(因为interval大于每帧时间，因此每帧最多产生一个粒子，所以在每帧内最多销毁一个粒子，只需要判断deque头部的粒子是否到达生命周期即可)
            if(std::chrono::duration<float>(std::chrono::steady_clock::now() - particles.begin()->lastTime).count() >= lifeCycle){
                particles.pop_front();
            }
        }
    }

    /* 3.粒子参数管理 */
    for(auto &each : particles){
        if(gravity){
            each.velocity.y += gravity * DELTA_TIME;
        }
        each.offset += each.velocity * DELTA_TIME;

        //间隔过渡逻辑
        if(fadingTime && std::chrono::duration<float>(std::chrono::steady_clock::now() - each.lastTime).count() >= lifeCycle - fadingTime){
            each.transparency -= transparency * DELTA_TIME / fadingTime;    //后者的transparency是ImageInterface中的
        }
    }


}


//渲染粒子
void Particle::Render(){
    if(images){

        //世界坐标-相机绝对坐标 = 相对于相机左上角位置（即真正需要渲染在屏幕上的位置）
        Vec2D center = (GetWorldPosition() - mainWorld.mainCamera->virtual_transform.position);
        HDC dstDC = GetImageHDC();          //目标窗口句柄

        //源图像高度和宽度
        int w = images[0]->getwidth();
        int h = images[0]->getheight();

        //目标图像的高度和宽度
        float dw = int(w*GetWorldScale().x*20.f/mainWorld.mainCamera->virtual_springArmLength);  //弹簧长度与游戏对象的规模成反比，默认长度为20
        float dh = int(h*GetWorldScale().y*20.f/mainWorld.mainCamera->virtual_springArmLength);

    
        //遍历所有粒子
        for(auto &each : particles){
            BLENDFUNCTION bf = {AC_SRC_OVER, 0, (BYTE)each.transparency, AC_SRC_ALPHA};
            //计算粒子在屏幕上的位置
            Vec2D pos = center + each.offset;
            pos = pos * 20.f/mainWorld.mainCamera->virtual_springArmLength + Vec2D(WIN_WIDTH / 2, WIN_HEIGHT /2) - Vec2D(dw/2,dh/2);

            HDC srcDC = GetImageHDC(images[each.index]);
            //绘制粒子
            AlphaBlend(dstDC, (int)pos.x, (int)pos.y, dw, dh, srcDC, 0, 0, w, h, bf);
        }    
    }
}




//设置参数
void Particle::SetCapacity(int capacity){
    this->capacity = capacity;
}
void Particle::SetGravity(float gravity){
    this->gravity = gravity;
}
void Particle::SetLifeCycle(float lifeCycle){
    this->lifeCycle = lifeCycle;
}
void Particle::SetIsLoop(bool isLoop){
    this->isLoop = isLoop;
}
void Particle::SetInterval(float interval){
    this->interval = interval;
}
void Particle::SetFadingTime(float fadingTime){
    this->fadingTime = fadingTime;
}
void Particle::SetSpeed(float speed){
    this->speed = speed;
}

//设置参数
void Particle::SetPattern(ParticlePattern pattern){
    this->pattern = pattern;
}
void Particle::SetCenter(Vec2D radius, Vec2D scoop){
    this->radius = radius;
    this->scoop = scoop;
}
void Particle::SetLine(float length, float angle){
    this->length = length;
    this->angle = angle;
}

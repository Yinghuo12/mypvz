#include "../include/Sprite.h"
#include "../include/Math.h"
#include "../include/Macro.h"

Sprite::Sprite(){
    renderer = ConstructComponent<SpriteRenderer>();
    renderer->AttachTo(root);
}


void Sprite::BeginPlay(){
    ;
}


void Sprite::Update(){
    Object::Update();
    //处于前半段
    if(isBlinking){
        if(!blinkFlag){
            if(transitionLevel > 0){
                transitionLevel -= transitionSpeed;
            }
            if(transitionLevel <= 0){
                isBlinking = false;
            }
        }
        //移除掉第一层滤镜，再添加新的第一层滤镜
        renderer->RemoveFilter();
        if(isBlinking){
            renderer->AddFilter({blinkFilter.color, int(transitionLevel), 1});
        }
    }
}



void Sprite::LoadSprite(std::string name){

}

void Sprite::Blink(float duration, COLORREF color, int level){
    //正在blinking
    if(!isBlinking){
        duration = Math::Clamp(duration, 0.1f, 1.f);  //闪光时间不能过短/过长
        isBlinking = true;
        blinkFlag = true;
        blinkFilter = FilterInfo(color, level, 1);   //创建闪光滤镜(位于第一层)
        transitionLevel = float(level);
        transitionSpeed = 2 * level * DELTA_TIME / duration;  //后半段时间执行了多少次
        recoverTimerHandle.Bind(duration / 2, this, &Sprite::Recover);
    }
}


void Sprite::Recover(){
    blinkFlag = false;
}

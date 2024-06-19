#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "CoreMinimal.h"
#include "SpriteRenderer.h"
#include "Timer.h"

typedef DWORD COLORREF;
class SpriteRenderer;

class Sprite : public Object{

public:
    Sprite();
    virtual void BeginPlay() override;
    virtual void Update() override;
    void LoadSprite(std::string name);
    void Blink(float duration = 0.2f, COLORREF color = 0x00000000, int level = 60);

private:
    void Recover();
    
private: 
    SpriteRenderer *renderer; // 渲染器

    bool isBlinking = false;
    bool blinkFlag = false;
    float transitionLevel = 0.f;
    float transitionSpeed = 0.f;
    FilterInfo blinkFilter;        //闪烁滤镜
    Timer recoverTimerHandle;      //计时器
    
};

#endif // __SPRITE_H__
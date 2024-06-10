#ifndef __SUN_H__
#define __SUN_H__

#include "SpriteRenderer.h"
#include "Animator.h"

class Sun:public StaticMesh{
public:
    Sun(){
        
        SpriteRenderer * renderer = GetComponentByClass<SpriteRenderer>();
        renderer->SetLayer(10);
        // renderer->SetFilter(true, RGB(235,51,36), 2);
        // renderer->SetFilter(true);

        animator = ConstructComponent<Animator>();

        animation.Load("sunshine");
        animation.SetInterval(0.07f);

        animator->Insert("idle", animation);
        animator->SetNode("idle");
        // animator->SetCalled(false);   //不播放动画
    }

private:
    Animation animation;
    Animator *animator;
};

#endif  // __SUN_H__

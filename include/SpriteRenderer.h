#ifndef __SPRITE_RENDERER_H__
#define __SPRITE_RENDERER_H__

#include "CoreMinimal.h"
#include "VisualInterface.h"


class SpriteRenderer final: public SceneComponent, public ImageInterface
{
    friend class Animator;  //for Animator::Update()

public:
    virtual void Render() override;
    virtual void Update() override;
    
private:
  
};




#endif  //__SPRITE_RENDERER_H__
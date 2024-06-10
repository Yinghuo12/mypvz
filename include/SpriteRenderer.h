#ifndef __SPRITE_RENDERER_H__
#define __SPRITE_RENDERER_H__

#include "CoreMinimal.h"
#include "VisualInterface.h"

const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;


class SpriteRenderer final: public SceneComponent, public ImageInterface
{
    friend class Animator;  //for Animator::Update()

public:
    virtual void Render() override;
    virtual void Update() override;
    
private:
  
};



//静态网格
class StaticMesh : public Object{

public:
    StaticMesh();
    virtual void BeginPlay() override;

private:
    SpriteRenderer* renderer;      //渲染器
    
};


#endif  //__SPRITE_RENDERER_H__
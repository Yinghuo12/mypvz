#ifndef __SUN_H__
#define __SUN_H__

#include "SpriteRenderer.h"
#include "Animator.h"
#include "Collider.h"
#include "RigidBody.h"

class Sun:public StaticMesh{
public:
    Sun(){
        
        /**** renderer ****/
        SpriteRenderer * renderer = GetComponentByClass<SpriteRenderer>();
        renderer->SetLayer(10);
        // renderer->SetFilter(true, RGB(235,51,36), 6);
        // renderer->SetFilter(true);


        /**** animator && animation ****/
        animator = ConstructComponent<Animator>();

        animation.Load("sunshine");
        animation.SetInterval(0.07f);

        animator->Insert("idle", animation);
        animator->SetNode("idle");
        // animator->SetCalled(false);   //不播放动画


        /**** collider ****/
        collider = ConstructComponent<CircleCollider>();
        collider->AttachTo(root);
        collider->SetRadius(35);

        collider_ = ConstructComponent<BoxCollider>();
        collider_->AttachTo(root);
        collider_->SetSize(Vec2D(70,70));

        /**** rigidBody ****/
        rigidBody = ConstructComponent<RigidBody>();
        rigidBody->SetGravity(1);
        rigidBody->AddForce(Vec2D(0,-200));
    }

    virtual void Update()override{
        StaticMesh::Update();
    }


private:
    Animation animation;
    Animator *animator;
    CircleCollider *collider;      //圆形
    BoxCollider *collider_;        //矩形
    RigidBody * rigidBody;       //刚体
};

#endif  // __SUN_H__

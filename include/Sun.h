#ifndef __SUN_H__
#define __SUN_H__

#include "Sprite.h"
#include "Animator.h"
#include "Collider.h"
#include "RigidBody.h"
#include "Timer.h"
#include "Particle.h"

class Sun:public Sprite{
public:
    Sun(){
        
        //渲染
        SpriteRenderer * renderer = GetComponentByClass<SpriteRenderer>();
        renderer->SetLayer(10);
        // renderer->SetFilter(true, 0x00FF0000);   //蓝色滤镜Error?
        // renderer->SetFilter(true, 0x00FFFFFF);   //白色滤镜Yes?
        // renderer->SetFilter(true);


        //动画
        animator = ConstructComponent<Animator>();

        animation.Load("sunshine");      //加载动画
        animation.SetInterval(0.07f);

        animator->Insert("idle", animation);
        animator->SetNode("idle");
        // animator->SetCalled(false);   //不播放动画

        // 碰撞
         collider = ConstructComponent<CircleCollider>();
         collider->AttachTo(root);
         collider->SetRadius(35);
         collider->SetCollisionMode(CollisionMode::Trigger);
         collider->SetType("Sunshine");

        // collider_ = ConstructComponent<BoxCollider>();
        // collider_->AttachTo(root);
        // collider_->SetSize(Vec2D(70,70));

        //碰撞与结束碰撞委托函数（还可以用lambda函数）
        collider->OnCollision.AddDynamic(this, &Sun::BeginCollision);
        collider->OffCollision.AddDynamic(this, &Sun::EndCollision);
        
        //刚体
        rigidBody = ConstructComponent<RigidBody>();
        rigidBody->SetGravityEnabled(false);
        // rigidBody->SetGravity(1);
        // rigidBody->AddForce(Vec2D(0,-200));


        //闪烁
        BlinkTimer.Bind(2, this, &Sun::MyBlink, true);   //白色滤镜Yes

        //粒子
        particle = ConstructComponent<Particle>();
        particle->AttachTo(root);
        particle->Load("mineparticle");
        particle->SetCapacity(10);
        particle->SetSpeed(100.f);
        particle->SetLayer(10);
        particle->SetGravity(98.f);
        particle->SetLifeCycle(2.f);
        particle->SetInterval(0);
        particle->SetCenter(Vec2D(1,2));

    }

    virtual void Update()override{
        Sprite::Update();
    }

    void MyBlink(){
        Sprite::Blink(0.2, 0x00FFFFFF, 45);        //白色
    }

    void BeginCollision(Collider *OverlapCollider, Object *OverlapActor){
        std::cout << "Hello" << std::endl;
        // Cast<AZombie*>(OverlapActor)->LoseHp(5);
    }
    void EndCollision(Collider *OverlapCollider, Object *OverlapActor){
        std::cout << "Bye" << std::endl;
    }

private:
    Animation animation;
    Animator *animator;
    CircleCollider *collider;      //圆形
    BoxCollider *collider_;        //矩形
    RigidBody * rigidBody;       //刚体
    Timer BlinkTimer;            //闪烁计时器
    Particle *particle;          //粒子效果
};

#endif  // __SUN_H__

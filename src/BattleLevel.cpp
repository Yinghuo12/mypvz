#include "../include/BattleLevel.h"
#include "../include/GameStatics.h"
#include "../include/SpriteRenderer.h"
#include "../include/Sun.h"


BattleLevel::BattleLevel()
{
    background0 = GameStatics::CreateObject<Sprite>();
    background0->GetComponentByClass<SpriteRenderer>()->LoadSprite("bg0");
    // background0->SetLocalPosition(Vec2D(300, 300));

    sunshine = GameStatics::CreateObject<Sun>();
    sunshine->SetLocalPosition(Vec2D(100, 100));
    // for (int i = 0; i < 100; i++){
    //     GameStatics::CreateObject<Sun>(Vec2D(Math::RandomReal(-350,350), Math::RandomReal(-21800,200)));
    // }

    // GameStatics::CreateObject<Sun>(Vec2D(Math::RandomReal(-350, 350), Math::RandomReal(-21800, 200)));
}                                                                 

void BattleLevel::Update(){
    // Error
    // sunshine->AddRotation(0.1f);
    sunshine->Update();
}


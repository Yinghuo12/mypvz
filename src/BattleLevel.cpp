#include "../include/BattleLevel.h"
#include "../include/GameStatics.h"
#include "../include/SpriteRenderer.h"
#include "../include/Sun.h"
#include "../include/Math.h"

BattleLevel::BattleLevel()
{
    background0 = GameStatics::CreateObject<Sprite>();
    background0->GetComponentByClass<SpriteRenderer>()->LoadSprite("bg0");
    // background0->SetLocalPosition(Vec2D(300, 300));

    sunshine = GameStatics::CreateObject<Sun>();
    sunshine->SetLocalPosition(Vec2D(100, 100));
    // for (int i = 0; i < 100; i++){
    //     GameStatics::CreateObject<Sun>(Vec2D(Math::RandomReal(-200,700), Math::RandomReal(-10000,0)));
    // }

    // GameStatics::CreateObject<Sun>(Vec2D(200,300));
}                                                                 

void BattleLevel::Update(){
    // Error
    // sunshine->AddRotation(0.1f);
    sunshine->Update();
}


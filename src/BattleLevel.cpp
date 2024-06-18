#include "../include/BattleLevel.h"
#include "../include/GameStatics.h"
#include "../include/SpriteRenderer.h"
#include "../include/Sun.h"


BattleLevel::BattleLevel()
{
    background0 = GameStatics::CreateObject<StaticMesh>();
    background0->GetComponentByClass<SpriteRenderer>()->LoadSprite("bg0");

    sunshine = GameStatics::CreateObject<Sun>();
    sunshine->SetLocalPosition(Vec2D(100, 100));
}                                                                 

void BattleLevel::Update(){
    // Error
    // sunshine->AddRotation(0.1f);
    sunshine->Update();
}


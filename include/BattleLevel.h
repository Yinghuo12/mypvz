#ifndef __BATTLE_LEVEL_H__
#define __BATTLE_LEVEL_H__

#include "CoreMinimal.h"

class Sun;
class StaticMesh;

class BattleLevel : public Level
{
public:
    BattleLevel();     //加载场景资源

    virtual void Update() override;

private:
    StaticMesh *background0;
    Sun *sunshine;
};

#endif // __BATTLE_LEVEL_H__
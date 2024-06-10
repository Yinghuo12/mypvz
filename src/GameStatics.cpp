#include "../include/GameStatics.h"

GameInstance* GameStatics::GetInstance(){
    return mainWorld.gameInstance;
}

Controller* GameStatics::GetController(){
    return mainWorld.mainController;
}
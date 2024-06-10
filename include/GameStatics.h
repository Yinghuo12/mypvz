#ifndef __GAME_STATICS_H__
#define __GAME_STATICS_H__

#include "CoreMinimal.h"

class GameStatics final{      //不希望该类被继承
public:
    static GameInstance *GetInstance();      //获取当前游戏实例
    static Controller *GetController();      //获取当前游戏控制器

    //创建对象实例
    template <typename T>
    static T *CreateObject(Vec2D pos = Vec2D(0, 0)){
        T* instance = new T;
        if(instance && static_cast<Object* >(instance)){    //如果T是Object或Object的子类
            mainWorld.game_objects.insert(instance);
            instance->BeginPlay();             //需要立马执行的逻辑
            instance->SetLocalPosition(pos);
            return instance;
        }
        return nullptr;    //转型失败则创建对象失败
    }

    //找到一个World中 某个类的所有对象
    template <typename T>
    static std::vector<T*>FindObjectsOfClass(){
        std::vector<T*> result;
        result.reserve(mainWorld.game_objects.size());
        for(auto& object : mainWorld.game_objects){
            if(T* obj = static_cast<T*>(object)){
                result.push_back(obj);
            }
        }
        return result;
    }

};




#endif // __GAME_STATICS_H__
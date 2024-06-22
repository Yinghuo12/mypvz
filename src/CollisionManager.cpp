#include "../include/CollisionManager.h"

#include <unordered_map>
#include <unordered_set>
#include <string>


CollisionManager::CollisionManager(){
    collisionMap.insert({"Default", std::unordered_set<std::string>{"Default"}});
}

void CollisionManager::Initialize(){
    /* 哈希碰撞表建立 */
    AddMapping("Sunshine", "Platform");
}

bool CollisionManager::FindMapping(const std::string &key1, const std::string &key2){
    auto it = collisionMap.find(key1);
    //如果键值存在
    if(it != collisionMap.end()){
        return it->second.find(key2) != it->second.end();  //返回映射
    }
    return false;
}

void CollisionManager::AddMapping(const std::string &key1, const std::string &key2){
    if(collisionMap.find(key1) == collisionMap.end()){
        collisionMap.insert({key1, std::unordered_set<std::string>()});
    }
    if(collisionMap.find(key2) == collisionMap.end()){
        collisionMap.insert({key2, std::unordered_set<std::string>()});
    }
    collisionMap[key1].insert(key2);
    collisionMap[key2].insert(key1);
}
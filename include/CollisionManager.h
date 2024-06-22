#ifndef __COLLISION_MANAGER_H__
#define __COLLISION_MANAGER_H__

#include <unordered_map>
#include <unordered_set>
#include <string>


class CollisionManager{
public:
    CollisionManager();
    void Initialize();
    bool FindMapping(const std::string &key1, const std::string &key2);
    void AddMapping(const std::string &key1, const std::string &key2);

private:
    std::unordered_map<std::string, std::unordered_set<std::string>> collisionMap;    //碰撞映射
};

#endif // __COLLISION_MANAGER_H__
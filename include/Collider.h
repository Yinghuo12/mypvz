#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "CoreMinimal.h"
#include <functional>

#define AddDynamic Add
#define RemoveDynamic Remove

//碰撞体形状
enum ColliderShape {
    Circle,
     Box
};
//碰撞模式
enum CollisionMode{
    None,
    Trigger,
    Collision
};


//碰撞委托
class CollisionDelehate{
public:
    //添加回调函数
    template <typename T>
    void Add(T* obj, void(T::*func)(Collider*, Object*)){
        callbacks.push_back(std::bind(func, obj, std::placeholders::_1, std::placeholders::_2));
    }

    //提供给lambda函数以及类静态函数版本
    void Add(std::function<void(Collider*, Object*)> func){
        callbacks.push_back(func);
    }

    // 移除回调函数
    template <typename T>
    void Remove(T* obj, void(T::*func)(Collider*, Object*)){
        //先把仿函数转换为函数包装器才能调用target函数
        std::function<void(Collider *, Object *)> targetFunc = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
        for(auto it = callbacks.begin(); it != callbacks.end(); ++it){
            if(it->target<void(Collider*, Object*)>() == targetFunc.target<void(Collider*, Object*)>()){
                callbacks.erase(it);
                break;
            }
        }
    }

    //调用函数包装器(发布委托)
    void BroadCast(Collider *OverlapCollider, Object *OverlapActor);
    

private:
    std::vector<std::function<void(Collider *, Object *)>> callbacks;
};

//碰撞器
class Collider : public SceneComponent
{
    friend class Controller;      //for IsMouseOver()

public:
   
    Collider();
    virtual ~Collider();

    const std::vector<Object *> &GetCollisions(std::string type);     //获取指定类型碰撞体容器
    int GetLayer() const;                 //获取碰撞层
    void SetLayer(int layer);             //设置碰撞层                            
    const std::string &GetType();         //获取碰撞体类型
    void SetType(std::string type);       //设置碰撞体类型
    ColliderShape GetShape() const;       //获取碰撞体形状
    void SetCollisionMode(CollisionMode mode);  //设置碰撞模式
    CollisionMode GetCollisionMode() const;     //获取碰撞模式

    void Clear();                         //清空
    void Insert(Collider *another);       //插入
    void Erase();                         //删除
    virtual void DrawDebugLine() = 0;     //绘制碰撞体轮廓

    CollisionDelehate OnCollision;        //碰撞委托
    CollisionDelehate OffCollision;       //碰撞委托结束
    

protected:
    ColliderShape shape = Circle;                                 //碰撞体形状
    CollisionMode mode = Trigger;                                    //碰撞模式
    virtual bool CollisionDetection(Collider *another) = 0;       //碰撞检测
    virtual bool IsMouseOver() = 0;                               //检测鼠标是否在物体上

private:
    std::string type = "Default";   //碰撞体类型：  植物 僵尸 子弹 捡拾物品
    int layer = 0;                  //碰撞层

    std::unordered_set<Collider *> collisions;        //碰撞体容器（发生碰撞的碰撞体存入这个容器）
    std::vector<Object *> aims;                       //目标容器
    std::vector<Collider *> collisions_to_erase;      //需要删除的碰撞体
};







//圆形碰撞器
class CircleCollider final: public Collider
{
public:
    CircleCollider();
    virtual void Update() override;
    virtual void DrawDebugLine() override;    //绘制碰撞体轮廓
    float GetRadius() const;
    void SetRadius(float r);

private:
    virtual bool CollisionDetection(Collider *another) override;  //碰撞检测
    virtual bool IsMouseOver() override;                          // 检测鼠标是否在物体上

private:
    float radius = 0.0f;
    float radius_ini = 0.0f;
};





//矩形碰撞器
class BoxCollider final: public Collider
{
public:
    BoxCollider();
    virtual void Update() override;
    virtual void DrawDebugLine() override;    //绘制碰撞体轮廓
    Vec2D GetSize() const;
    void SetSize(Vec2D size);

private:
    virtual bool CollisionDetection(Collider *another) override;  //碰撞检测
    virtual bool IsMouseOver() override;                          // 检测鼠标是否在物体上


private:
    Vec2D size = Vec2D(0.0f, 0.0f);
    Vec2D size_ini = Vec2D(0.0f, 0.0f);
};




#endif // !__COLLIDER_H__
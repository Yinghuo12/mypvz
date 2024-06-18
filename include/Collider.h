#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "CoreMinimal.h"

//碰撞体形状
enum ColliderShape {
    Circle,
     Box
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
    void Clear();                         //清空
    void Insert(Collider *another);       //插入
    virtual void DrawDebugLine() = 0;     //绘制碰撞体轮廓



protected:
    ColliderShape shape = Circle;                                 //碰撞体形状
    virtual bool CollisionDetection(Collider *another) = 0;       //碰撞检测
    virtual bool IsMouseOver() = 0;                               //检测鼠标是否在物体上

private:
    std::string type = "Default";   //碰撞体类型：  植物 僵尸 子弹 捡拾物品
    int layer = 0;                  //碰撞层

    std::unordered_set<Collider *> collisions;        //碰撞体容器（发生碰撞的碰撞体存入这个容器）
    std::vector<Object *> aims;                       //目标容器
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
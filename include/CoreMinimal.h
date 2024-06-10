#ifndef __CORE_MINIMAL_H__
#define __CORE_MINIMAL_H__

#include <cmath>
#include <iostream>
#include <set>
#include <vector>
#include <stack>
#include <unordered_set>


/***********************************************************
 *                   核心代码文件
 * 
 *   基础外部头文件，基础结构体，基础类，基础函数，基础全局变量
 * 
 **********************************************************/


constexpr double PI = 3.14159265;
constexpr double DELTA_TIME = 0.01;    //每一帧是0.01秒

class World;
extern World mainWorld;    //全局World实例



//二维向量
class Vec2D{
public:

    Vec2D();
    Vec2D(float a, float b);

    friend std::ostream& operator<<(std::ostream &cout, const Vec2D &vec);

    Vec2D operator+(const Vec2D& rhs);
    Vec2D operator-(const Vec2D& rhs);
    Vec2D operator*(float rhs);
    Vec2D operator/(float rhs);

    Vec2D operator-();

    Vec2D& operator+=(const Vec2D& rhs);
    Vec2D& operator-=(const Vec2D& rhs);

    bool operator==(const Vec2D& rhs);
    bool operator!=(const Vec2D& rhs);
    
    static float Distance(const Vec2D& a, const Vec2D& b);
    static Vec2D RotateVec(float angle, const Vec2D& vec);

public:
    float x;
    float y;
};


//对象基类
class Base
{
public:
    virtual void Update() = 0;    //纯虚函数，用于实现每帧更新
};


//变换类
class Transform
{
public:
    Transform();
    Transform(Vec2D position, float rotation, Vec2D scale);
    Transform Identity() const;

public:
    Vec2D position;                   //位置
    float rotation;                   //旋转角度
    Vec2D scale;                      //缩放
};

class Object;
//组件基类
class Component : public Base
{

public:
    //SetOwner设置为public仅向组件管理器创建组件时提供调用接口，而不希望我们直接调用它
    void SetOwner(Object *owner);               // 设置对象(宿主)

    virtual void Update() override;             //重写更新函数
    virtual void Destruct();                    // 虚函数，用于实现销毁

protected:
    Object *owner = nullptr;                    //所属的宿主对象，用于处理控制逻辑

};


//场景组件类
class SceneComponent : public Component
{
public:
    //不希望用户使用，如果手动调用，则会导致内存重复释放(delete) 这里我们应该设置为private
    void process_Destruct();                    //辅助销毁函数，递归销毁

public:
    virtual void Update() override;
    virtual void Destruct() override;

    void AttachTo(SceneComponent *parent);      //绑定到父组件和宿主对象
    void DetachFrom(SceneComponent *parent);    //解除与父组件、宿主对象的绑定

    //获取相对变换
    Vec2D GetLocalPosition() const;
    float GetLocalRotation() const;
    Vec2D GetLocalScale() const;

    //获取绝对变换
    Vec2D GetWorldPosition() const;
    float GetWorldRotation() const;
    Vec2D GetWorldScale() const;

    //设置相对变换
    void SetLocalPosition(const Vec2D &pos);
    void SetLocalRotation(float rot);
    void SetLocalScale(const Vec2D &scale);

    //叠加变换
    void AddPosition(const Vec2D &pos);
    void AddRotation(float rot);

private:
    std::unordered_set<SceneComponent*> children;         //子组件  set红黑树方便增删O(logn)
    SceneComponent* parent = nullptr;                     //父组件，处理场景属性关系并且便于增删
    Transform transform;                                  //此对象支持变换
};





//对象基类————组件管理器(管理组件创建、更新与销毁)
class Object : public Base{
    friend void SceneComponent::process_Destruct();
    friend void Component::Destruct();

protected:
    //固定的场景根组件，赋予场景信息 不能改变指向
    SceneComponent * const root = new SceneComponent;

public:
    Object();
    virtual ~Object();

    //组件注册
    void RegisterComponent(Component *component);

    //组件注销
    void UnregisterComponent(Component *component);

    //组件构造器，采用模板使之可以支持任意类型组件的构造 功能：创建组件，绑定到对象并且注册到对象容器
    template <typename T>
    T *ConstructComponent(){
        T* instance = new T;
        //我们希望创建出的T一定从属于component基类
        if(instance && static_cast<Component*>(instance)){ //如果T能向上转型为Component
            instance->SetOwner(this);       //把管理者置为自己
            RegisterComponent(instance);    //加入组件容器
            return instance;
        }
        return nullptr;    //转型失败则创建组件失败
    }

    //获取组件
    template <typename T>
    T *GetComponentByClass(){
        for(auto& component : components){
            if(T* com = dynamic_cast<T*>(component)){    //把component转化为我们传入的模板T(子类指针)
                return com;     //返回子类指针
            }
        }
        return nullptr;
    }


    //组件更新
    virtual void Update() override;

    //开始使用(存放对象创建后需要立马调用的函数逻辑)
    virtual void BeginPlay() = 0;

    //绑定到父对象
    void AttachTo(Object *parent);

    //解除与父对象的绑定    
    void DetachFrom(Object *parent);

    //从mainWorld里删除对象
    void Destroy();  

    //获取相对变换
    Vec2D GetLocalPosition() const;
    float GetLocalRotation() const;
    Vec2D GetLocalScale() const;

    //获取绝对变换
    Vec2D GetWorldPosition() const;
    float GetWorldRotation() const;
    Vec2D GetWorldScale() const;

    //设置相对变换
    void SetLocalPosition(const Vec2D &pos);
    void SetLocalRotation(float rot);
    void SetLocalScale(const Vec2D &scale);

    //叠加变换
    void AddPosition(const Vec2D &pos);
    void AddRotation(float rot);

private:
    std::unordered_set<Object *> children;        //与子对象的绑定
    Object *parent = nullptr;                     //与父对象的绑定


    std::unordered_set<Component*>             components;    //组件容器，用于管理组件
    std::unordered_set<Component*>::iterator components_it;   //组件指针，用于组件遍历和增删

};






//游戏实体类(全局)
class GameInstance{

};


//场景类（放对象用）
class Level : public Base{
public:
    virtual void Update() override;
};



//图层比较器
struct LayerCmp{
    bool operator()(const class LayerInterface *a, const class LayerInterface *b) const;
};


//世界类
class World final{   //final表示它不能被继承

    friend class Engine;
    friend class GameStatics;        //管理World类，玩家可调用
    friend class Timer;
    friend class LayerInterface;
    friend class ImageInterface;   //for LoadSprite()
    friend class Camera;           //for SetMainCamera()
    friend class SpriteRenderer;     //for Render()
    friend class Animation;         //for Load()

    friend void Object::Destroy();

private:
    /* 核心逻辑遍历 */
    void Update();
    void Update_();
    void Render();
    void Input();

    /* DEBUG模式 */
    void Debug();

private:
    /* 场景对象、UI、计时器容器 */
    std::unordered_set<Object *> game_objects;
    std::unordered_set<class UserInterface *> game_UIs;
    std::unordered_set<class Timer *> game_timers;

    /* 垃圾回收容器，防止destruct之后，指针的变化影响Object的更新 */
    std::unordered_set<Object *> game_objects_to_delete;   
    std::unordered_set<class UserInterface *> game_UIs_to_delete;


    /* 渲染、碰撞计算容器 */
    std::set<class LayerInterface*, LayerCmp> game_renderers;  //图层渲染容器
    std::unordered_set<class BoxCollider *> game_colliders;    //碰撞计算容器
    // std::set<class Collider*, ColliderSort> ColliderZones[8][6];

    /* 游戏单例对象 */
    Level *current_level = nullptr;               // 当前场景
    GameInstance * gameInstance = nullptr;        // 游戏实体
    class Controller *mainController = nullptr;   //管理IO(玩家交互)
    class Camera* mainCamera = nullptr;           // 摄像机
    class Timer *FPSClock;                        //全局计时器(显示帧率)

    /* 资源池 */
    class ResourceManager *resourcePool = nullptr;
};



//用户接口类
class UserInterface{

};




//类型转换函数  可以是子类转向父类 也可以是父类转向子类
template <typename T>
inline T* Cast(Base * base){
    if(base) 
        return dynamic_cast<T*>(base);
    return nullptr;
}


#endif // __CORE_MINIMAL_H__
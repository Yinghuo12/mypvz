#include "../include/CoreMinimal.h"
#include "../include/Timer.h"
#include "../include/VisualInterface.h"
#include "../include/Controller.h"
#include "../include/Camera.h"
#include "../easyx/graphics.h"
#include "../include/Collider.h"
#include "../include/Math.h"
#include "../include/Macro.h"


//全局World实例
World mainWorld{};

/*二维向量*/
Vec2D::Vec2D(): x(0.0f), y(0.0f){}
Vec2D::Vec2D(float a, float b): x(a), y(b){};
Vec2D Vec2D::operator+(const Vec2D& rhs){
    return Vec2D(x + rhs.x, y + rhs.y);
}
Vec2D Vec2D::operator-(const Vec2D& rhs){
    return Vec2D(x - rhs.x, y - rhs.y);
}
Vec2D Vec2D::operator*(float rhs){
    return Vec2D(x * rhs, y * rhs);
}
Vec2D Vec2D::operator/(float rhs){
    return Vec2D(x / rhs, y / rhs);
}
Vec2D Vec2D::operator*(const Vec2D& rhs){
    return Vec2D(x * rhs.x, y * rhs.y);
}
Vec2D Vec2D::operator/(const Vec2D& rhs){
    return Vec2D(x / rhs.x, y / rhs.y);
}
Vec2D Vec2D::operator-(){
    return Vec2D(-x, -y);
}
Vec2D& Vec2D::operator+=(const Vec2D& rhs){
    x += rhs.x;
    y += rhs.y;
    return *this;
}
Vec2D& Vec2D::operator-=(const Vec2D& rhs){
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}
bool Vec2D::operator==(const Vec2D& rhs){
    return (x == rhs.x) && (y == rhs.y);
}
bool Vec2D::operator!=(const Vec2D& rhs){
    return (x != rhs.x) || (y != rhs.y);
}
float Vec2D::Distance(const Vec2D& a, const Vec2D& b){
    return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
Vec2D Vec2D::RotateVec(float angle, const Vec2D& vec){
    double rad = PI * angle / 180;    //弧度
    float fsin = float(sin(rad));
    float fcos = float(cos(rad));

    return Vec2D(vec.x*fcos - vec.y*fsin, vec.x*fsin + vec.y*fcos);
}
std::ostream& operator<<(std::ostream & cout, const Vec2D &vec){
    cout << "(" << vec.x << "," << vec.y << ")";
    return cout;
}


std::string Vec2D::ToString() const{
    return "(" + std::to_string((int)x) + "," + std::to_string((int)y) + ")";
}


/*二维向量*  end*/


/*变换类*/
Transform::Transform() : position(0, 0), rotation(0), scale(1, 1){};
Transform::Transform(Vec2D position, float rotation, Vec2D scale)
        : position(position), rotation(rotation), scale(scale){};
Transform Transform::Identity() const{
    return Transform(Vec2D(0, 0), 0, Vec2D(1, 1));
}



/*组件类*/
//设置对象(宿主)
void Component::SetOwner(Object * owner){
    this->owner = owner;
}

//虚函数，用于实现销毁
void Component::Destruct(){
    this->owner->UnregisterComponent(this);
    delete this;
}

//组件更新
void Component::Update(){
    ;
}




/*场景组件*/

//绑定
void SceneComponent::AttachTo(SceneComponent *parent){
    if(parent){
        //1.绑定到父组件
        parent->children.insert(this);       //将该组件添加到新的父组件的子组件列表中
        this->parent = parent;              //将父组件设置为新的父组件

        //2.绑定到宿主对象
        SetOwner(parent->owner);
    }
}

//解绑
void SceneComponent::DetachFrom(SceneComponent *parent){
    if(parent){
        //1.解绑父组件
        parent->children.erase(this);      //从父组件的子组件容器中移除自己
        this->parent = nullptr;           //自己的父组件置为空

        //2.解绑宿主对象
        // SetOwner(nullptr);  //若解绑后不立马绑定到另一个组件身上，则失去了宿主，若执行destruct时候会发生危险。
    }
}


//场景组件更新
void SceneComponent::Update(){
    ;
}


//辅助销毁函数 用于递归销毁包括它自身的所有子组件
void SceneComponent::process_Destruct(){
    if(!children.empty()){
        for(auto child : children){
            child->process_Destruct();
        }
    }
    //最基本的销毁单步操作,处理自己本身
    Component::Destruct();
}

//真正的销毁操作
void SceneComponent::Destruct(){
    if(this->parent) this->parent->children.erase(this);   //从父组件的子组件容器中移除自己(断前)
    process_Destruct();                                    //递归销毁所有子组件(绝后)
}


//获取相对变换
Vec2D SceneComponent::GetLocalPosition() const { return this->transform.position;}
float SceneComponent::GetLocalRotation() const { return this->transform.rotation; }
Vec2D SceneComponent::GetLocalScale() const { return this->transform.scale; }


//获取绝对变换
Vec2D SceneComponent::GetWorldPosition() const{
    if(parent)
        return parent->GetWorldPosition() + GetLocalPosition();  //递归
    else{
        if(owner)
            return owner->GetWorldPosition();
        else   //如果玩家误创出一些独立的组件，他是没有宿主的（悬浮组件）
            return GetLocalPosition();
    }
}
float SceneComponent::GetWorldRotation() const{
    if(parent)
        return parent->GetWorldRotation() + GetLocalRotation();
    else{
        if(owner)
            return owner->GetWorldRotation();
        else   //如果玩家误创出一些独立的组件，他是没有owner的
            return GetLocalRotation();
    }
}
Vec2D SceneComponent::GetWorldScale() const{
    if(parent)
        return parent->GetWorldScale() + GetLocalScale();
    else{
        if(owner)
            return owner->GetWorldScale();
        else  //如果玩家误创出一些独立的组件，他是没有owner的
            return GetLocalScale();

    }
}

//设置相对变换
void SceneComponent::SetLocalPosition(const Vec2D &pos) { this->transform.position = pos; }
void SceneComponent::SetLocalRotation(float rot) { this->transform.rotation = rot; }
void SceneComponent::SetLocalScale(const Vec2D &scale) { this->transform.scale = scale; }

//叠加变换
void SceneComponent::AddPosition(const Vec2D &pos) { this->transform.position += pos; }
void SceneComponent::AddRotation(float rot) { this->transform.rotation += rot; }



/*场景组件*  end*/








/*对象类*/

//构造函数
Object::Object() {
    root->SetOwner(this);  //表明场景根组件root的拥有者是Object
}

//析构函数
Object::~Object() {
    for(auto& com:components)
        delete com;
    delete root;
}


//组件注册
void Object::RegisterComponent(Component* component){
    components.insert(component);
}

//组件注销
void Object::UnregisterComponent(Component* component){
    auto it = components.find(component);
    components_it = components.erase(it);  //因为set树形结构，删除元素会发生树形变化，所以要重置迭代器位置
}

//组件更新
void Object::Update(){
    components_it = components.begin();
    while(components_it != components.end()){
        (*components_it)->Update();
        if(components_it == components.end()) //因为遍历过程中，迭代器可能会发生变化
            break;
        components_it++;
    }
}

//绑定到父对象
void Object::AttachTo(Object *parent){
    if(parent){
        parent->children.insert(this);       //将该组件添加到新的父组件的子组件列表中
        this->parent = parent;               //将父组件设置为新的父组件
    }
    /* Object与宿主对象之间无需绑定，因为他们之间不需要相互控制
        只需要通过空间属性、场景属性关系来绑定
        而组件是为对象服务的，因此组件需要绑定到宿主来接受控制 */
}

//解除与父对象的绑定
void Object::DetachFrom(Object *parent){
    if(parent){
        parent->children.erase(this);      //从父组件的子组件容器中移除自己
        this->parent = nullptr;           //自己的父组件置为空
    }
}

//从mainWorld里删除对象
void Object::Destroy(){
    if(parent)
        parent->children.erase(this);  //从父组件的子组件容器中移除自己

    std::stack<Object *> objects_to_delete;   //需要删除的对象容器
    objects_to_delete.push(this);             //把自己压入栈中
    while(!objects_to_delete.empty()){
        Object *current_object = objects_to_delete.top();   //保存栈顶指针
        objects_to_delete.pop();

        if(!current_object->children.empty()){    //若有孩子对象
            for(auto & child:current_object->children){
                objects_to_delete.push(child);    //孩子对象入栈
            }
        }
        mainWorld.game_objects_to_delete.insert(current_object);  //压入世界类的待删除对象容器中
    }
}

//获取相对变换
Vec2D Object::GetLocalPosition() const { return root->GetLocalPosition(); }
float Object::GetLocalRotation() const { return root->GetLocalRotation(); }
Vec2D Object::GetLocalScale() const { return root->GetLocalScale(); }


//获取绝对变换
Vec2D Object::GetWorldPosition() const{
    if(parent)   //Object没有owner(宿主)
        return parent->GetWorldPosition() + GetLocalPosition();
    else    //如果没有parent的话，那么他就是祖先对象，即未绑定在任何对象身上，此时可以返回局部位置
        return GetLocalPosition();
}
float Object::GetWorldRotation() const{
    if(parent)
        return parent->GetWorldRotation() + GetLocalRotation();
    else
        return GetLocalRotation();
}
Vec2D Object::GetWorldScale() const{
    if(parent)
        return parent->GetWorldScale() + GetLocalScale();
    else
        return GetLocalScale();
}

//设置相对变换
void Object::SetLocalPosition(const Vec2D &pos) { this->root->SetLocalPosition(pos); }
void Object::SetLocalRotation(float rot) { this->root->SetLocalRotation(rot); }
void Object::SetLocalScale(const Vec2D &scale) { this->root->SetLocalScale(scale); }

//叠加变换
void Object::AddPosition(const Vec2D &pos) { this->root->AddPosition(pos); }
void Object::AddRotation(float rot) { this->root->AddRotation(rot); }

//物体位置Debug
void Object::DrawDebugPosition(){
    Vec2D pos = (GetWorldPosition() - mainWorld.mainCamera->virtual_transform.position)
        *20.f / mainWorld.mainCamera->virtual_springArmLength + Vec2D(WIN_WIDTH / 2, WIN_HEIGHT / 2);
    outtextxy((int)pos.x, (int)pos.y, GetWorldPosition().ToString().c_str());
}

/*组件管理器组件  end*/





//场景类
// void Level::Update(){
    
// }




//世界类
/* 核心逻辑遍历 */
void World::Update(){
    /*对Collider的操作*/
    ProcessColliderZones();

    /*对Object的操作*/
    for (auto &obj : game_objects)
        obj->Update();
    for(auto &obj:gameobjects_to_add)
        game_objects.insert(obj);
    gameobjects_to_add.clear();
    
    for(auto &obj : game_objects_to_delete){
        game_objects.erase(obj);
        delete obj;
    }
    game_objects_to_delete.clear();

    /*对UI的操作*/
    for(auto& ui: game_UIs)
        ;
    for(auto &ui : game_UIs_to_delete){
        game_UIs.erase(ui);
        delete ui;
    }
    game_objects_to_delete.clear();

    /*对计时器操作*/
    for(auto& timer: game_timers)
        timer->Execute();

    /* 对场景的操作 */
    current_level->Update();

}



void World::Update_(){
    /* 对摄像机的操作 */
    mainCamera->Calculate();
}

void World::Render(){
    cleardevice();
    for(auto& renderer : game_renderers)
        renderer->Render();
    Debug();
    FlushBatchDraw();
}

void World::Input(){
    mainController->PeekInfo();
}

void World::ProcessColliderZones(){
    //清除上一帧碰撞信息
    for(auto& arr_i : ColliderZones){
        for(auto& arr_j : arr_i){
            arr_j.clear();
        }
    }
    //对于上一帧发生碰撞的每一个碰撞体
    for (auto &it : game_colliders){
        //跳过判断以优化性能
        if(it->GetCollisionMode() == CollisionMode::None)
            continue;

        Vec2D half;          //用于计算外接正方形
        if(it->GetShape() == ColliderShape::Circle){
            float a = Cast<CircleCollider>(it)->GetRadius();
            half = Vec2D(a, a);
        }
        else
            half = Cast<BoxCollider>(it)->GetSize() / 2;

        Vec2D pos = it->GetWorldPosition();
        pos -= half;         //外接正方形的左上角
        int x = (int)pos.x / 100;   x = Math::Clamp(x, 0, 7);      //横向只有8个格子
        int y = (int)pos.y / 100;   y = Math::Clamp(y, 0, 5);      //纵向只有6个格子
        pos += half * 2;     //外接正方形的右下角
        int x_1 = (int)pos.x / 100;   x_1 = Math::Clamp(x_1, 0, 7);
        int y_1 = (int)pos.y / 100;   y_1 = Math::Clamp(y_1, 0, 5);

        //把碰撞体插入到网格中
        for (int i = x; i <= x_1; ++i)
            for (int j = y; j <= y_1; ++j)
                ColliderZones[i][j].insert(it);
    }

    //碰撞信息更新
    for(auto& arr_i : ColliderZones){
        for(auto& arr_j : arr_i){
            if(!arr_j.empty()){
                for (auto me = arr_j.begin(); me != arr_j.end(); ++me){
                    for (auto he = arr_j.begin(); he != arr_j.end(); ++he){
                        if(he != me){
                            (*me)->Insert(*he);
                        }
                    }
                }
            }
        }
    }

    // //碰撞删除信息更新
    // for(auto & it: game_colliders){
    //     it->Erase();
    // }
}


void World::Debug(){
    //碰撞体轮廓
    for (auto& obj : game_colliders)
        obj->DrawDebugLine();
    
    //物体位置显示
    for (auto& obj : game_objects)
        obj->DrawDebugPosition();

    //FPS显示
    static int FPS = 0;
    static int number = 0;
    number++;   //Debug在Render中执行，每渲染一帧就自增一次
    outtextxy(0, 0, (std::string("FPS: ") + std::to_string(FPS)).c_str());
    if(FPSClock->GetDelay() >= 1.0){   //每秒的帧率
        FPS = number;
        FPSClock->Reset();
        number = 0;
    }

}


//渲染图层比较器
bool LayerCmp::operator()(const class LayerInterface* a, const class LayerInterface *b) const{
    if (a->GetLayer() == b->GetLayer())
        return a < b;    //按图层指针地址大小排序
    else
        return a->GetLayer() < b->GetLayer();    //按照layer的大小排序
}


//碰撞图层比较器
bool ColliderCmp::operator()(const class Collider *a, const class Collider *b) const{
    if (a->GetLayer() == b->GetLayer())
        return a < b;    //按图层指针地址大小排序
    else
        return a->GetLayer() < b->GetLayer();    //按照layer的大小排序
}


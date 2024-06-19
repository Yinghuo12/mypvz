#include "../include/Collider.h"
#include "../include/Camera.h"
#include "../include/CoreMinimal.h"
#include "../easyx/easyx.h"
#include "../include/Macro.h"
#include "../include/Controller.h"



Collider::Collider(){
    mainWorld.game_colliders.insert(this);
}

Collider::~Collider(){
    mainWorld.game_colliders.erase(this);
}


//获取指定类型碰撞体容器
const std::vector<Object *>& Collider::GetCollisions(std::string type){
    aims.clear();
    if(!collisions.empty()){
        for(auto it = collisions.begin(); it != collisions.end(); ++it){
            if((*it)->GetType() == type){
                aims.push_back((*it)->owner);
            }
        }
    }
    return aims;
}


//获取碰撞层
int Collider::GetLayer() const{
    return layer;
}

//设置碰撞层 
void Collider::SetLayer(int layer){
    this->layer = layer;
}


//获取碰撞体类型
const std::string& Collider::GetType(){
    return type;
}

//设置碰撞体类型
void Collider::SetType(std::string type){
    this->type = type;
}


//获取碰撞体形状
ColliderShape Collider::GetShape() const{
    return shape;
}

//清空
void Collider::Clear(){
    collisions.clear();
}


//插入
void Collider::Insert(Collider *another){
    //互相插入对方碰撞体容器
    if(collisions.find(another) == collisions.end() && CollisionDetection(another)){
        collisions.insert(another);
        another->collisions.insert(this);
    }       
}





/* 圆形碰撞体 */
CircleCollider::CircleCollider(){
    shape = Circle;
}

void CircleCollider::Update(){
    Collider::Update();
    radius = radius_ini * sqrtf(GetWorldScale().x * GetWorldScale().y);
}

//绘制碰撞体轮廓
void CircleCollider::DrawDebugLine(){
    Vec2D pos = (GetWorldPosition() - mainWorld.mainCamera->virtual_transform.position)*20.f / mainWorld.mainCamera->virtual_springArmLength
         + Vec2D(WIN_WIDTH / 2, WIN_HEIGHT / 2);
    circle((int)pos.x, (int)pos.y, int(radius * 20.f / mainWorld.mainCamera->virtual_springArmLength));       //easyx绘图函数
}

float CircleCollider::GetRadius() const{
    return radius;
}

void CircleCollider::SetRadius(float r){
    radius = r;
    radius_ini = r / sqrtf(GetWorldScale().x * GetWorldScale().y);
}


//碰撞检测
bool CircleCollider::CollisionDetection(Collider *another) {
    //1.与圆形碰撞
    if(another->GetShape() == Circle){
        CircleCollider *circle = Cast<CircleCollider>(another);
        return (Vec2D::Distance(GetWorldPosition(), circle->GetWorldPosition()) <= (radius + circle->GetRadius()));
    }

    //2.与矩形碰撞
    else {
        BoxCollider *box = Cast<BoxCollider>(another);
        Vec2D pos = GetWorldPosition(), pos_another = box->GetWorldPosition();
        Vec2D size = box->GetSize();
        float top = pos_another.y + size.y / 2, bottom = pos_another.y - size.y / 2, left = pos_another.x - size.x / 2, right = pos_another.x + size.x / 2;
        
        //圆心在矩形内
        if(pos.x <=right && pos.x >= left && pos.y <= top && pos.y >= bottom)
            return true;
        //圆心在矩形外
        else{
            if(pos.x < left){
                if(pos.y > top)            return Vec2D::Distance(pos, {left, top}) <= radius;         //在左上角 
                else if(pos.y < bottom)    return Vec2D::Distance(pos, {left, bottom}) <= radius;      //在左下角
                else                       return left - pos.x <= radius;                              //在左边      
            }
            else if(pos.x > right){
                if(pos.y > top)            return Vec2D::Distance(pos, {right, top}) <= radius;        //在右上角 
                else if(pos.y < bottom)    return Vec2D::Distance(pos, {right, bottom}) <= radius;     //在右下角
                else                       return pos.x - right <= radius;                             //在右边      
            }
            else{
                if(pos.y > top)            return pos.y - top <= radius;                               //在上边      
                else                       return bottom - pos.y <= radius;                            //在下边        
            }           
        }
    }  

    return false;
}

bool CircleCollider::IsMouseOver(){
    return Vec2D::Distance(GetWorldPosition(), mainWorld.mainController->GetCursorPosition()) <= radius;
}





/* 矩形碰撞体 */
BoxCollider::BoxCollider(){
    shape = Box;
}



void BoxCollider::Update(){
    Collider::Update();
    size = size_ini * GetWorldScale();
}

//绘制碰撞体轮廓
void BoxCollider::DrawDebugLine(){
    Vec2D pos = (GetWorldPosition() - mainWorld.mainCamera->virtual_transform.position)*20.f / mainWorld.mainCamera->virtual_springArmLength 
        + Vec2D(WIN_WIDTH / 2, WIN_HEIGHT / 2);
    Vec2D current_size = this->size * 20.f / mainWorld.mainCamera->virtual_springArmLength;
    float left = pos.x - current_size.x / 2, right = pos.x + current_size.x / 2,  
          top = pos.y + current_size.y / 2, bottom = pos.y - current_size.y / 2;    
    rectangle((int)left, (int)top, (int)right, (int)bottom);         //easyx绘图函数
}

Vec2D BoxCollider::GetSize() const{
    return size;
}

void BoxCollider::SetSize(Vec2D size){
    this->size = size;
    size_ini = size / GetWorldScale();   //实时变换
}



//碰撞检测
bool BoxCollider::CollisionDetection(Collider *another){
    //1.与圆形碰撞
    if(another->GetShape() == Circle){
        CircleCollider *circle = Cast<CircleCollider>(another);
        Vec2D pos = GetWorldPosition(), pos_another = circle->GetWorldPosition();
        float radius = circle->GetRadius();
        float top = pos.y + size.y / 2, bottom = pos.y - size.y / 2, left = pos.x - size.x / 2, right = pos.x + size.x / 2;
        
        //圆心在矩形内
        if(pos_another.x <=right && pos_another.x >= left && pos_another.y <= top && pos_another.y >= bottom)
            return true;
        //圆心在矩形外
        else{
            if(pos_another.x < left){
                if(pos_another.y > top)            return Vec2D::Distance(pos_another, {left, top}) <= radius;         //在左上角 
                else if(pos_another.y < bottom)    return Vec2D::Distance(pos_another, {left, bottom}) <= radius;      //在左下角
                else                               return left - pos_another.x <= radius;                              //在左边      
            }
            else if(pos_another.x > right){
                if(pos_another.y > top)            return Vec2D::Distance(pos_another, {right, top}) <= radius;        //在右上角 
                else if(pos_another.y < bottom)    return Vec2D::Distance(pos_another, {right, bottom}) <= radius;     //在右下角
                else                               return pos_another.x - right <= radius;                             //在右边      
            }
            else{
                if(pos_another.y > top)            return pos_another.y - top <= radius;                               //在上边      
                else                               return bottom - pos_another.y <= radius;                            //在下边        
            }           
        }
    } 

    //2.与矩形碰撞
    else{
        BoxCollider* box = Cast<BoxCollider>(another);
        Vec2D pos = GetWorldPosition() - size / 2 , pos_another = box->GetWorldPosition() - box->GetSize() / 2;    //统一到左上角
        return (pos.x<pos_another.x + box->GetSize().x 
            && pos.x + size.x > pos_another.x 
            && pos.y<pos_another.y + box->GetSize().y 
            && pos.y + size.y > pos_another.y);
    }

    return false; 
}


bool BoxCollider::IsMouseOver(){
    Vec2D pos = GetWorldPosition(),  pos_mouse = mainWorld.mainController->GetCursorPosition();
    return pos_mouse.x <= pos.x + size.x/2 && pos_mouse.x >= pos.x - size.x/2 && pos_mouse.y <= pos.y + size.x/2 && pos_mouse.y >= pos.y - size.x/2;
}


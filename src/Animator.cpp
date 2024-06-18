#include "../include/Animator.h"
#include "../include/ResourceManager.h"
#include "../include/SpriteRenderer.h"

Animation::Animation(){
    clock.Bind(0, this, &Animation::UpdateIndex, true);
}


//动画帧更新
void Animation::UpdateIndex(){
    index = (index + 1) % num;
}


//加载动画资源
void Animation::Load(std::string name,  POINT delta){
    AnimationResource aniRes = mainWorld.resourcePool->FetchAnimation(name);
    this->num = aniRes.num;
    this->images = aniRes.images;
    offset = delta;
}


//设置计时器更新频率
void Animation::SetInterval(double interval){
    clock.SetDelay(interval);
}

//设置动画下标
void Animation::SetIndex(int i){
    this->index = i;
}






void Animator::Update(){
    //找到附着渲染器
    if(!rendererAttached){
        rendererAttached = owner->GetComponentByClass<SpriteRenderer>();
    }
    else if(aniNode){  //如果指针为空，则不播放动画
        //渲染
        rendererAttached->sprite = aniNode->images[aniNode->index];
        rendererAttached->spriteInfo.offset = aniNode->offset;
    }
}



 //传入动画
void Animator::Insert(std::string name, Animation &ani){
    if(ani.images)   //健壮性，判断指针是否为空
        animations.insert({name, ani});
}


//设定当前播放的动画
void Animator::SetNode(std::string nodeName){
    if(aniNode)                                         //如果之前正在播放动画
        aniNode->clock.Stop();                          //暂停计时器
    aniNode = &(animations.find(nodeName)->second);     //设置新动画
    aniNode->clock.Continue();                          //启动计时器
}


//获取当前播放的动画
Animation* Animator::GetNode() const{
    return aniNode;
}


//是否播放
void Animator::SetCalled(bool called){
    called ? aniNode->clock.Continue() : aniNode->clock.Stop();
}
#ifndef __TIMER_H__
#define __TIMER_H__

#include "CoreMinimal.h"
#include <functional>
#include <chrono>       //里面的计时器精度高，而且跨平台

class Timer final{

private:
    //获取当前延迟
    std::chrono::duration<double> GetDurationDelay() { 
        return std::chrono::steady_clock::now() - last_time;
    }

public:
    Timer() { last_time = std::chrono::steady_clock::now(); }
    ~Timer() { mainWorld.game_timers.erase(this); }

    
    //绑定回调函数等初始化操作
    template<typename T>
    void Bind(double delay, T*obj, void(T::*func)(), bool repeat = false){
        callback = std::bind(func, obj);             //绑定回调函数，参数1：类成员函数指针  参数2：类地址
        this->delay = std::chrono::duration<double, std::ratio<1, 1> >(delay);
        is_persistent = repeat;
        last_time = std::chrono::steady_clock::now();               //开始计时

        mainWorld.game_timers.insert(this);                        //把定时器插入到mainWorld的定时器列表中
    }



    //执行计时操作
    void Execute(){
        if(delay.count() > 0.0  && GetDelay() >= delay.count() && is_running){    //如果我们不设置delay（即默认值为0.0）,则表示每帧都在执行，不需要计时
            callback();
            if(is_persistent){
                last_time = std::chrono::steady_clock::now();          //更新上一次执行的时间，便于下次计时
            }
            else{
                delay = std::chrono::duration<double>(0.0);           //设置delay为0.0，表示执行完这次后不再需要计时
            }
        }
    }

    //设置时间段
    void SetDelay(double delay){
        this->delay = std::chrono::duration<double, std::ratio<1,1> >(delay);
    }

    //返回计时器执行的时间
    double GetDelay(){
        return GetDurationDelay().count();     //把duration时间转换为double类型
    }

    //重置计时器
    void Reset(){
        last_time = std::chrono::steady_clock::now();
    }

    //停止计时器
    void Stop(){
        is_running = false;
    }

    //开始计时器
    void Continue(){
        is_running = true;
    }

private: 
    std::function<void()> callback;
    std::chrono::duration<double> delay = std::chrono::duration<double>(0.0);    // 时间段,表示多长时间后执行一次回调操作
    std::chrono::time_point<std::chrono::steady_clock> last_time;                // 时间点,表示上一次执行回调操作的时间
    bool is_running = true;         //是否正在运行
    bool is_persistent = false;     //是否持续计时
};

#endif // __TIMER_H__
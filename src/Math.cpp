#include "../include/Math.h"
//#include <random>

//静态函数 获取一个随机整数
int Math::RandomInt(int lower, int upper){
    if(lower > upper){
        int temp = lower;
        lower = upper;
        upper = temp;
    }

    static std::random_device RandomSeed;   //种子发生器
    unsigned int seed = RandomSeed();        //生成一个随机的种子值
    std::mt19937 engine(seed);       //随机数引擎
    std::uniform_int_distribution<int> dis(lower, upper);  //整数均匀分布

    return dis(engine);              //返回随机数

}



//获取一个随机实数
double Math::RandomReal(double lower, double upper){
    if(lower > upper){
        int temp = lower;
        lower = upper;
        upper = temp;
    }

    static std::random_device RandomSeed;   //种子发生器
    std::mt19937 engine(RandomSeed());       //随机数引擎
    std::uniform_real_distribution<double> dis(lower, upper);  //整数均匀分布

    return dis(engine);              //返回随机数

}

double Math::RandPerc(){
    static std::random_device RandomSeed;   //种子发生器
    std::mt19937 engine(RandomSeed());       //随机数引擎
    return std::generate_canonical<double, 10>(engine);                  //返回0-1之间的浮点数
}


//将某数限制在某个范围内
double Math::Clamp(double value, double lower, double upper){
    return std::min(std::max(value, std::min(lower, upper)), std::max(lower, upper));
}


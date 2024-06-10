#ifndef __MATH_H__
#define __MATH_H__
#include <random>

//数学类，包含随机数生成函数
class Math{
public:

    static int RandomInt(int lower, int upper);           //获取一个随机整数
    static double RandomReal(double lower, double upper); //获取一个随机实数
    static double RandPerc();                             //获取百分比(0-1之间的随机实数)
    static double Clamp(double value, double lower, double upper);  //将某数限制在某个范围内
};



#endif //__MATH_H__   
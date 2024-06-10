#include <Windows.h>

#include "./include/CoreMinimal.h"
#include "./include/Math.h"
#include "./easyx/graphics.h"
#include "./include/ResourceManager.h"
// #include "./include/Controller.h"


#include "./include/Camera.h"
#include "./include/SpriteRenderer.h"
#include "./include/Timer.h"
#include "./include/VisualInterface.h"


/* 测试 */
// #include "./include/TestObject.h"
#include "./include/BattleLevel.h"
#include "./include/BattleController.h"
#include "./include/GameStatics.h"


extern const int WIN_WIDTH;
extern const int WIN_HEIGHT;



class Engine{
public:
    static void Init(){
        
        initgraph(WIN_WIDTH, WIN_HEIGHT, EW_SHOWCONSOLE);           //创建画布

        //系统debug绘制参数
        setlinestyle(PS_SOLID | PS_JOIN_BEVEL);
        setfillcolor(LIGHTBLUE);
        setlinecolor(LIGHTBLUE);

        //系统文字绘制参数
        setbkmode(TRANSPARENT);
        settextstyle(25, 10, "Arial");
        settextcolor(WHITE);

        
        mainWorld.resourcePool = new ResourceManager;  //初始化资源池
        mainWorld.resourcePool->Initialize();          //初始化资源池

        BeginBatchDraw();

        mainWorld.current_level = new BattleLevel;     //初始化场景
        mainWorld.gameInstance = new GameInstance;     //初始化游戏实例
        mainWorld.mainController = GameStatics::CreateObject<BattleController>(Vec2D(0.f, 0.f));
        mainWorld.FPSClock = new Timer;     //初始化游戏实例

        /* 测试 */
        //mainWorld.mainController = new Controller;     //初始化主控制器
        // TestObject* test_object = new TestObject;  //初始化测试对象
        // mainWorld.game_objects.insert(test_object);  //将测试对象添加到游戏实例中
    }


    //进程1(频率低，精度低，例如世界的渲染)
    static void Start(){
        // mainWorld.Input();
        mainWorld.Update();
        mainWorld.Render();
    }

    //进程2(频率高，精度高，例如保持相机平滑移动)
    static void Start_(){
        mainWorld.Input();
        mainWorld.Update_();
    }


    static void Exit(){
        ;
    }
};


//低精度渲染回调
void CALLBACK timecallback(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR){
    Engine::Start();
}

//高精度渲染回调
void CALLBACK timecallback_(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR){
    Engine::Start_();
}


int main(){

    // //测试随机数
    // for (int i = 0; i < 100; i++){
    //     std::cout << Math::RandPerc() << " ";
    // }

    // HWND hConsole = GetConsoleWindow();   //获取控制台窗口句柄      
    // //设置控制台窗口不被最小化
    // SetWindowPos(hConsole, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
        

    Engine::Init();

    // 计时器事件操作
    timeSetEvent(UINT(DELTA_TIME * 1000), 1, timecallback, 0, TIME_CALLBACK_FUNCTION|TIME_PERIODIC);  
    timeSetEvent(1, 1, timecallback_, 0, TIME_CALLBACK_FUNCTION|TIME_PERIODIC); //1毫秒执行一次


    system("pause"); 
    //Sleep(INFINITE);   //主线程停止，额外开辟的定时器触发事件线程一直执行
    
    
    return 0;
} 
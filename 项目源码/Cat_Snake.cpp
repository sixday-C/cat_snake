#include<iostream>
#include<stdio.h>
#include<Windows.h>
#include<easyx.h>
#include<graphics.h>
#include<stdlib.h>
#include<conio.h>
#include<ctime>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include<limits>
#include<sstream>
#include<stdbool.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"MSIMG32.LIB") // 加载图像处理库
using namespace std;
/*
  1.主菜单界面
      玩家可以用鼠标操作选择  开始游戏（调用模式选择界面） 继续游戏 设置
      排行榜 更多界面 退出游戏
  2.模式选择界面
     2.0玩家可以选择闯关模式，无尽模式，返回主菜单
     2.1闯关模式后调用关卡选择界面 选择关卡后进入游戏界面
  3.设置界面
      玩家可以选择增加/减少游戏音量  返回主菜单
  4.排行榜界面
      玩家在该界面查看得分排名  返回主菜单
   5.更多界面
      1.玩法说明界面 展示游戏玩法 返回主菜单
      2.开发者说明界面 展示开发者名单 返回主菜单
      3.皮肤系统 展示皮肤 返回主菜单
      4.成就系统 展示成就 返回主菜单
      5.返回主菜单
   6.退出游戏

*/
/*-------------------------------------数据设计-------------------------------*/
constexpr auto Width = 1200;//窗口宽度
constexpr auto Height = 750;//窗口高度
constexpr auto MAX_LEN = 25 * 25;//蛇身最大长度
/*
蛇的基本属性：
   1.蛇是否存活
   2.蛇身长度
   3.蛇头方向
   4.每一节蛇的坐标
   5.蛇的速度
   6.获得的分数
   7.每一节身体的图片
通过改变蛇的属性进行游戏程序，例如吃到食物改变蛇身长度，wasd对应蛇头方向改变蛇头坐标...
*/
struct Snake
{
    int flag;//蛇是否存活
    int len;//蛇身长度
    int dir;//蛇头方向 UP DOWN RIGHT LEFT
    int pos[2][MAX_LEN];//每一节蛇的坐标，行数为0时表示横坐标x，为1时表示纵坐标y
    double speed;//蛇的速度
    int score;//获得的分数
    IMAGE snakes[MAX_LEN];
}snake;
/*玩家得分*/
vector<int> playerscore;

/*上下左右，用枚举表示方便根据蛇的方向实现蛇的移动*/
enum DIR
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};
/*
食物（1.蔬菜，2.肉，3.兔子）：
  1.是否生成/存在/被吃掉
  2.横坐标
  3.纵坐标
  4.属性加成
  5.图片
*/
struct Veg//蔬菜
{
    int flag; //是否生成 / 存在 / 被吃掉
    int x;//横坐标
    int y;//纵坐标
    int add;//属性加成
    IMAGE vegs[5];//图像
}veg;
struct Meat//肉
{
    int flag;
    int x;
    int y;
    int add;
    IMAGE meats[5];

}meat;
struct Rabbit//兔子
{
    int flag;
    int x;
    int y;
    int add;
    IMAGE rabbits[5];
}rabbit;
int mode_choice = 0;
/*
地图：
  用于绘制地图和设计关卡
  int map[level][25][25]
  level表示当前关卡数,大小25*25，其中0表示空地，1表示墙体

*/
int level = 0;//定义当前关卡数
int map[6][25][25] = {
{
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
},
{
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1},
{0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
{0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
{0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
{1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
{1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
},
{
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
},
{
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,0,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0,0,1,0,0},
{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,0,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0,0,1,0,0},
{0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
},
{
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0},
{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,0,0,1,1,1,1,0,0,0,1,0,0,0,1,1,1,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0},
{0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0},
{0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0},
{0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0},
{0,0,1,0,0,1,1,1,1,0,0,0,1,0,0,0,1,1,1,1,0,0,1,0,0},
{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
{0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
},
{
{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
}
};
/*-------------------------------------数据设计-------------------------------*/
/*-------------------------------------service---------------------------------*/
/*核心
 负责人：李子同学
   功能：初始化游戏地图，蛇的状态（出生地，初始方向（右），初始长度（3），初始速度（1），分数0），食物分布(调用函数)以及属性（flag1，add为吃到后增加的长度）
   参数：void
   返回值：void
*/
void Init();
/*核心
 负责人：Anti同学
   功能：实现蛇的移动，根据当前的方向和速度以及蛇的长度改变蛇的坐标，进而实现蛇的移动(包括*穿墙*的实现)
   参数：void
   返回值：void
*/
void SnakeMove();
/*核心
 负责人：雷同学
   功能：获取键盘消息，实现*蛇方向的改变*包括wasd以及WASD和上下左右
   参数：void
   返回值：void
*/
void keyControl();
/*核心
 负责人：影同学
   功能：随机生成食物包括蔬菜的坐标 肉的坐标 兔子的坐标，标定flag值
        不能与地图冲突，不能互相冲突
   参数：void
   返回值：void
*/
void foodCreat();
/*核心
 负责人：雷同学
   功能：实现食物与蛇的交互功能，当吃到蔬菜蛇长-3速度加快（sleep函数控制）（不扣分），吃到肉蛇长+1（分数+5），吃到兔子蛇长+2（分数+10）
          需要单帧动画演示
   参数：void
   返回值：void
*/
void eatFood();
/*
 负责人：影同学
   功能：判断蛇是否死亡，如果碰到墙/自己/长度==0死亡（调用游戏结算界面）
   参数：void
   返回值：void
*/
void dieJudge();
/*
 负责人：钟晴
  功能：判断plalyer1和player2的大小并返回其中更大的值
  参数：int player1,int player2
  返回值：bool
*/
bool cmp(int player1, int player2);
/*--------------------------------------service---------------------------------*/

/*----------------------------------------view--------------------------------*/
/*核心
 负责人：六天组长
   功能：绘制主菜单界面，玩家可以在这里选择（鼠标操作）（绘制和设置选择按钮）开始游戏，继续游戏，查看排行榜，设置，更多，退出游戏（调用函数）
   参数：void
   返回值：void
*/
void menuView();
/*核心
 负责人：六天组长
   功能：绘制模式选择界面，玩家可以在这里选择（鼠标操作+绘制和设置选择按钮）闯关模式，无尽模式和返回主菜单（调用函数）
   参数：void
   返回值：void
*/
void modeView();//
/*核心
 负责人：shy同学
   功能：绘制游戏地图，需要使用easyx判断游戏地图的值（0/1），进而调用图片进行绘制
   参考数据：map[level][25][25],Width,Height，地图数据：
 //得分框：长28*25  高度50（暂定）    总长L：28*25+x（x表示游戏地图以外的界面）  高度H：28*25+50（50暂定）
// L/H=1920/1080  L：？（尽量可以整除，方便将游戏画面置中）
//游戏地图  每一格： 28*28   游戏地图大小：28*28*25
   参数：void
   返回值：void

*/
void mapDraw();
/*核心
 负责人：evy同学
   功能：绘制蛇身，需要使用easyx根据蛇的坐标调用图片绘制蛇身/蛇头
   参数：void
   返回值：void
*/
void snakeDraw();
/*核心
 负责人：影同学
   功能：绘制食物，包括蔬菜，肉，兔子，需要使用easyx根据不同食物的坐标绘制食物
   参数：void
   返回值：void

*/
void foodDraw();
/*核心
 负责人：荣同学
   功能：绘制无尽模式地图和闯关模式地图界面（调用函数），绘制蛇（调用函数），食物（调用函数）得分情况
   参数：void
   返回值：void
*/
void GameDraw();
/*核心
 负责人：六天组长
   功能：绘制游戏暂停界面，获取键盘消息esc，实现游戏暂停玩家可以选择1继续游戏，2设置（调用函数）3返回主菜单（键盘/鼠标操作，需要绘制按钮）
   参数：void
   返回值：void
*/
void pauseView();
/*核心
 负责人：钟晴同学
   功能：绘制游戏结算画面，记录得分（文件流）玩家可以查看得分情况，返回主菜单
   参数：void
   返回值：void
*/
void endView();
/*核心
 负责人：Anti同学
   功能：绘制‘更多’界面，玩家在这里可以选择1查看玩法说明，2查看开发者名单，3皮肤系统，4成就系统，5返回主菜单（需要实现鼠标选择操作，按钮绘制和使用）
   参数：void
   返回值：void
*/
void moreView();
/*非核心
 负责人：shy同学
   功能：绘制玩法说明界面，玩家可以查看玩法说明，返回主界面（鼠标返回，需要绘制按钮）
   参数：void
   返回值：void
*/
void ruleView();
/*核心
 负责人：evy同学
   功能：绘制开发者名单界面，玩家可以查看开发者，返回主界面（鼠标返回，需要绘制按钮）
   参数：void
   返回值：void
*/
void developersView();
/*
 负责人：邵同学
   功能：绘制皮肤系统界面，展示皮肤（easyx调用图片），返回主菜单（鼠标选择返回，需要绘制按钮）
   参数：void
   返回值：void
*/
void skinView();
/*
 负责人：邵同学
   功能：绘制成就系统界面，陈列成就，返回主菜单（鼠标选择返回，需要绘制按钮）
   参数：void
   返回值：void
*/
void achievementView();
/*
 负责人：钟晴同学
   功能：绘制排行榜，玩家可以查看排行榜数据，返回主菜单（鼠标选择返回，需要绘制按钮）
   参数：void
   返回值：void

*/
void listView();
/*
 负责人：荣同学
   功能：绘制设置界面，玩家可以选择+/-音量，返回主菜单（鼠标选择返回，需要绘制按钮）
   参数：void
   返回值：void
*/
void musicsetView1();
void musicsetView2();
/*
 负责人：雷同学
   功能：播放游戏音乐（主界面，游戏界面)
   参数：int i(曲目顺序)
   返回值：void
*/
void musicPlay(int i);
/*
负责人：六天组长
  函数功能：绘制按钮
  参数：int x, int y, int w, int h, const char* text
  返回值：void*/
void button(int x, int y, int w, int h, const char* text);
/*
负责人：雷同学
  函数功能：控制帧率
  参数：double f
  返回值：int
*/
int FPS(double f);
/*------------------------------------------view--------------------------------*/
int main()
{

    initgraph(Width, Height);
    Init();//初始化
    setbkcolor(RGB(113, 218, 239));
    cleardevice();//清屏
    menuView();//调用菜单界面



    system("pause");
}

void putimage(int x, int y, IMAGE *img) { // 重写原有的putimage函数
    int w = img->getwidth();
    int h = img->getheight();
    AlphaBlend(GetImageHDC(NULL), x, y, w, h,
               GetImageHDC(img), 0, 0, w, h, {AC_SRC_OVER,0,255,AC_SRC_ALPHA});
}

//-------------------------------view函数实现--------------------------------------
void button(int x, int y, int w, int h, const char* text) {// 横坐标，纵坐标，按钮宽度，按钮高度，文本
    // 绘制图形
    setlinestyle(PS_SOLID, 2);
    //填充颜色
    setlinecolor(BLACK);
    setfillcolor(RGB(115, 251, 253));
    fillroundrect(x, y, x + w, y + h, 10, 10); // 左，上，右，下，上下角度，左右角度；圆角矩形
    // 绘制文字
    settextcolor(BLACK);
    settextstyle(30, 0, "楷体加粗");
    setbkmode(TRANSPARENT);
    // 文字居中
    int width = (w - textwidth(text)) / 2;
    // 根据矩形进行调整
    int height = (h - textheight(text)) / 2;
    outtextxy(width + x, height + y, text);

}
void menuView() {
    //播放音乐
    musicPlay(rand() % 6);
    cleardevice();
    //加载了主菜单背景和标题
    IMAGE menuImg;
    loadimage(&menuImg, "./image/menuView.jpg", 1200, 750);
    putimage(0, 0, &menuImg);
    //定义 开始游戏,继续游戏，查看排行榜，设置，更多，退出游戏 按钮
    button(200, 350, 200, 75, "开始游戏");// 横坐标，纵坐标，按钮宽度，按钮高度，文本
    button(500, 350, 200, 75, "继续游戏");
    button(800, 350, 200, 75, "查看排行榜");
    button(200, 500, 200, 75, "设置");
    button(500, 500, 200, 75, "更多");
    button(800, 500, 200, 75, "退出游戏");
    //定义ExMessage变量进行鼠标操作
    ExMessage msg;
    while (true) {
        if (peekmessage(&msg, EX_MOUSE)) {
            switch (msg.message) {
            case WM_LBUTTONDOWN:
                if ((msg.x >= 200) && (msg.x <= 400) && (msg.y >= 350) && (msg.y <= 425)) {
                    //开始游戏，进入模式选择
                    modeView();
                }
                if ((msg.x >= 500) && (msg.x <= 700) && (msg.y >= 350) && (msg.y <= 425)) {
                    //继续游戏,敬请期待
                    setbkcolor(RGB(113, 218, 239));
                    cleardevice();
                    outtextxy(450, 300, "暂时未实现该功能，敬请期待");
                    //返回主菜单
                    button(500, 500, 200, 75, "返回主菜单");
                    while (true) {
                        if (peekmessage(&msg, EX_MOUSE)) {
                            switch (msg.message) {
                                //左键按下
                            case WM_LBUTTONDOWN:
                                if ((msg.x >= 500) && (msg.x <= 700) && (msg.y >= 500) && (msg.y <= 575)) {
                                    //返回主菜单
                                    menuView();
                                }
                                break;
                            default:
                                break;
                            }
                        }
                    }
                }
                if ((msg.x >= 800) && (msg.x <= 1000) && (msg.y >= 350) && (msg.y <= 425)) {
                    // 查看排行榜
                    listView();
                }
                if ((msg.x >= 200) && (msg.x <= 400) && (msg.y >= 500) && (msg.y <= 575)) {
                    // 设置
                    musicsetView1();
                }
                if ((msg.x >= 500) && (msg.x <= 700) && (msg.y >= 500) && (msg.y <= 575)) {
                    // 更多
                    moreView();
                }
                if ((msg.x >= 800) && (msg.x <= 1000) && (msg.y >= 500) && (msg.y <= 575)) {
                    // 退出游戏
                    exit(0);
                }
                break;
            default:
                break;
            }
        }
    }
    getchar();
    closegraph();
}
void modeView() {
    //清屏
    setbkcolor(RGB(113, 218, 239));
    cleardevice();
    //定义图片变量
    IMAGE modeImg;
    //加载并输出图片
    loadimage(&modeImg, "./image/modeView.jpg", 1200, 750);
    putimage(0, 0, &modeImg);
    // 设置 闯关模式，无尽模式和返回主菜单 按钮
    button(500, 100, 200, 75, "闯关模式");// 横坐标，纵坐标，按钮宽度，按钮高度，文本
    button(500, 300, 200, 75, "无尽模式");
    button(500, 500, 200, 75, "返回主菜单");
    ExMessage msg;
    while (true) {
        if (peekmessage(&msg, EX_MOUSE)) {
            switch (msg.message) {
            case WM_LBUTTONDOWN:
                if ((msg.x >= 500) && (msg.x <= 700) && (msg.y >= 100) && (msg.y <= 175)) {
                    mode_choice = 1;
                    GameDraw();    //闯关模式
                    break;
                }
                if ((msg.x >= 500) && (msg.x <= 700) && (msg.y >= 300) && (msg.y <= 375)) {
                    //无尽模式
                    mode_choice = 2;
                    GameDraw();
                }
                if ((msg.x >= 500) && (msg.x <= 700) && (msg.y >= 500) && (msg.y <= 575)) {
                    //返回主菜单
                    menuView();
                }
                break;
            default:
                break;
            }
        }
    }
}
void mapDraw()
{
    cleardevice();
    settextcolor(TRANSPARENT);
    settextstyle(20, 0, "微软雅黑");
    char arr[] = "当前得分为：";
    int width = (Width - textwidth(arr)) / 2; // 根据矩形进行调整
    int height = (50 - textheight(arr)) / 2;
    //打印 当前得分
    outtextxy(width, height, arr);
    char str[50] = "";
    sprintf_s(str, "%d", snake.score);
    outtextxy(width + textwidth(arr), height, str);
    //遍历地图，打印地图
    IMAGE img;


    for (int i = 0; i <= 24; i++)
    {

        for (int j = 0; j <= 24; j++) {

            if (map[level][i][j] == 0)
            {
                //输出空白
                loadimage(&img, "./image/blank.jpg", 28, 28);
                //横坐标初始为250,纵坐标初始为50
                putimage(250 + j * 28, 50 + i * 28, &img);
            }
            else if (map[level][i][j] == 1)
            {
                //输出墙
                loadimage(&img, "./image/wall.jpg", 28, 28);
                putimage(250 + j * 28, 50 + i * 28, &img);
            }
        }
    }


}
void snakeDraw()
{
    IMAGE img;
    for (int i = 0; i < snake.len; i++)
    {
        if (i == 0)//首先确定头部图片
        {
            switch (snake.dir)//根据方向
            {


            case UP:
                loadimage(&img, "./image/head_up.png", 28, 28);
                putimage(snake.pos[0][i] - 14, snake.pos[1][i] - 14, &img);
                break;
            case DOWN:
                loadimage(&img, "./image/head_down.png", 28, 28);
                putimage(snake.pos[0][i] - 14, snake.pos[1][i] - 14, &img);
                break;
            case RIGHT:
                loadimage(&img, "./image/head_right.png", 28, 28);
                putimage(snake.pos[0][i] - 14, snake.pos[1][i] - 14, &img);
                break;
            case LEFT:
                loadimage(&img, "./image/head_left.png", 28, 28);
                putimage(snake.pos[0][i] - 14, snake.pos[1][i] - 14, &img);
                break;
            }
        }
        else if (i != 0 && i != snake.len - 1)//如果是身体
        {
            if (snake.pos[0][i - 1] == snake.pos[0][i + 1])//如果是垂直移动
            {
                loadimage(&img, "./image/body_ud.png", 28, 28);
                putimage(snake.pos[0][i] - 14, snake.pos[1][i] - 14, &img);

            }
            else if (snake.pos[1][i - 1] == snake.pos[1][i + 1])//如果是水平移动
            {
                loadimage(&img, "./image/body_lr.png", 28, 28);
                putimage(snake.pos[0][i] - 14, snake.pos[1][i] - 14, &img);
            }
            else if (snake.pos[0][i + 1] != snake.pos[0][i - 1] && snake.pos[1][i + 1] != snake.pos[1][i - 1])
            {
                loadimage(&img, "./image/body_t.jpg", 28, 28);
                putimage(snake.pos[0][i] - 14, snake.pos[1][i] - 14, &img);
            }

        }

        else if (i == snake.len - 1)//如果是尾巴
        {
            if (snake.pos[0][i - 1] == snake.pos[0][i] && snake.pos[1][i - 1] > snake.pos[1][i])//尾巴向下
            {
                loadimage(&img, "./image/tail_down.png", 28, 28);
                putimage(snake.pos[0][i] - 14, snake.pos[1][i] - 14, &img);
            }
            else  if (snake.pos[0][i - 1] == snake.pos[0][i] && snake.pos[1][i - 1] < snake.pos[1][i])//尾巴向上
            {
                loadimage(&img, "./image/tail_up.png", 28, 28);
                putimage(snake.pos[0][i] - 14, snake.pos[1][i] - 14, &img);
            }
            else  if (snake.pos[0][i - 1] > snake.pos[0][i] && snake.pos[1][i - 1] == snake.pos[1][i])//尾巴向右
            {
                loadimage(&img, "./image/tail_right.png", 28, 28);
                putimage(snake.pos[0][i] - 14, snake.pos[1][i] - 14, &img);
            }
            else  if (snake.pos[0][i - 1] < snake.pos[0][i] && snake.pos[1][i - 1] == snake.pos[1][i])//尾巴向左
            {
                loadimage(&img, "./image/tail_left.png", 28, 28);
                putimage(snake.pos[0][i] - 14, snake.pos[1][i] - 14, &img);
            }
        }
    }
}
void foodDraw()
{
    foodCreat();
    // 加载食物图像并指定实际大小
    IMAGE vegImage, meatImage, rabbitImage;
    loadimage(&vegImage, _T("./image/veg.png"), 28, 28);
    loadimage(&meatImage, _T("./image/meat.png"), 28, 28);
    loadimage(&rabbitImage, _T("./image/rabbit.jpg"), 28, 28);

    // 绘制蔬菜
    if (veg.flag) {
        putimage(veg.x, veg.y, &vegImage);
    }

    // 绘制肉
    if (meat.flag) {
        putimage(meat.x, meat.y, &meatImage); // 垂直居中对齐
    }

    // 绘制兔子
    if (rabbit.flag) {
        putimage(rabbit.x, rabbit.y, &rabbitImage); // 水平居中对齐
    }
}
void GameDraw() {
    mciSendString("close music", 0, 0, 0);
    //移动延迟
    DWORD t1, t2;
    t1 = t2 = GetTickCount();
    while (1)
    {
        mciSendString("open ./music/BGM1.mp3 alias BGM ", 0, 0, 0);
        mciSendString("play BGM repeat", 0, 0, 0);

        while (1) {
            pauseView();//暂停界面
            if (FPS(snake.speed))
            {  //双缓冲绘图防止闪屏
                BeginBatchDraw();
                mapDraw();
                snakeDraw();
                foodDraw();
                EndBatchDraw();



                if (t2 - t1 > 2)//控制t2t1间隔优化手感
                {



                    SnakeMove();
                    keyControl();//控制
                    t1 = t2;
                    dieJudge();

                }
                t2 = GetTickCount();

                eatFood();//交互判断

                if (snake.score >= (level + 1) * 10 && mode_choice == 1) //如果到达通过条件并且是闯关模式
                {
                    snake.pos[0][0] = 264;
                    snake.pos[1][0] = 64;
                    snake.dir = RIGHT;
                    snake.len = 3;
                    level++;
                    foodDraw();
                    Sleep(500);
                }

                if (level == 6)//如果完全通关
                    endView();
            }
            else Sleep(0.0001);
        }

    }
}
void pauseView() {
    //定义bool变量用来判断是否进入暂停界面
    bool gamePaused = false;

    // 检查ESC键是否被按下
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    {
        if (!gamePaused)
        {

            //清屏
            setbkcolor(RGB(113, 218, 239));
            cleardevice();
            IMAGE pauseImg;
            loadimage(&pauseImg, "./image/pause.jpg", 1200, 750);
            putimage(0, 0, &pauseImg);
            // 设置 1继续游戏，2设置（调用函数）3返回主菜单 按钮
            button(500, 100, 200, 75, "1.继续游戏");// 横坐标，纵坐标，按钮宽度，按钮高度，文本
            button(500, 300, 200, 75, "2.设置");
            button(500, 500, 200, 75, "3.返回主菜单");
            ExMessage msg;
            while (true)
            {
                if (peekmessage(&msg, EX_MOUSE))
                {
                    switch (msg.message)
                    {
                    case WM_LBUTTONDOWN:
                        if ((msg.x >= 500) && (msg.x <= 700) && (msg.y >= 100) && (msg.y <= 175)) {
                            //继续游戏
                            GameDraw();
                        }
                        if ((msg.x >= 500) && (msg.x <= 700) && (msg.y >= 300) && (msg.y <= 375)) {
                            //设置
                            musicsetView2();
                        }
                        if ((msg.x >= 500) && (msg.x <= 700) && (msg.y >= 500) && (msg.y <= 575)) {
                            mciSendString("close BGM", 0, 0, 0);
                            Init();
                            //返回主菜单
                            menuView();
                        }
                        break;
                    default:
                        break;
                    }
                }
            }

            while (true)
            {
                ExMessage pauseM;//创建消息结构体
                getmessage(EX_KEY);
                if (pauseM.message == WM_KEYDOWN)
                {//判断有没有键盘按下
                    switch (pauseM.vkcode)
                    {
                    case  '1':
                        GameDraw();
                        break;
                    case '2':
                        musicsetView2();
                        break;
                    case '3':
                        menuView();
                        break;
                    default:
                        break;
                    }
                }
            }
            gamePaused = true;
        }
        else //没进入
        {
            gamePaused = false;
        }
    }

}
void endView() {
    mciSendString("close BGM", 0, 0, 0);
    mciSendString("open ./music/happy.mp3 alias WIN", 0, 0, 0);
    mciSendString("play WIN ", 0, 0, 0);
    // 设置背景颜色为 RGB(113, 218, 239)  
    setbkcolor(RGB(113, 218, 239));

    // 清空设备（屏幕）  
    cleardevice();

    // 设置文本背景为透明  
    setbkmode(TRANSPARENT);

    // 设置填充矩形的颜色为棕色  
    setfillcolor(RGB(115, 251, 253));

    // 绘制一个圆角矩形，坐标和角的半径分别为(1000, 650, 1200, 750)和10  
    fillroundrect(1000, 650, 1200, 750, 10, 10);

    // 设置文本样式，字体大小为30，字体为“微软雅黑”  
    settextstyle(30, 0, "楷体加粗");

    // 定义要显示的字符串  
    char arr[50] = "返回主菜单";

    // 计算文本的中心位置  
    int tx = 1000 + (200 - textwidth(arr)) / 2;
    int ty = 650 + (100 - textheight(arr)) / 2;

    // 输出文本到计算出的中心位置  
    outtextxy(tx, ty, arr);

    // 定义图片变量  
    IMAGE img_mm;

    // 设置更大的文本样式，字体大小为100  
    settextstyle(100, 0, _T("微软雅黑"));
    settextcolor(BLACK);  // 设置文本颜色为黑色  
    setbkmode(TRANSPARENT); // 设置背景为透明  

    // 定义并格式化当前得分字符串  
    char arr1[50] = "";
    sprintf_s(arr1, "当前得分：%d", snake.score);

    // 计算当前得分文本的中心位置  
    int w = (1200 - textwidth(arr1)) / 2;
    int h = (120 - textheight(arr1)) / 2;

    // 输出当前得分文本  
    outtextxy(w, h, arr1);

    // 设置文本样式和颜色，用于“游戏结算！”的显示  
    settextstyle(80, 0, _T("微软雅黑"));
    settextcolor(RED);
    char str[50] = "游戏结束！";

    // 计算“游戏结算！”文本的中心位置  
    int ww = (1200 - textwidth(str)) / 2;
    int hh = (370 - textheight(str)) / 2;

    // 输出“游戏结算！”文本  
    outtextxy(ww, hh, str);

    // 加载图像文件  
    loadimage(&img_mm, "./image/true.jpg");

    // 在指定位置绘制加载的图像  
    putimage(ww - 50, hh + 100, &img_mm);

    // 获取窗口句柄  
    HWND hnd = GetHWnd();

    // 设置窗口标题为空  
    SetWindowText(hnd, "");

    // 弹出消息框询问用户是否上传得分  
    int isyes = MessageBox(hnd, "你是否要上传本次得分", "提示", MB_YESNO);

    // 如果用户选择“是”  
    if (isyes == IDYES) {
        // 以追加模式打开得分文件  
        ofstream outFile("scores.txt", ios::app);

        if (outFile) // 检查文件是否成功打开  
        {
            // 打开同一文件以读取当前得分排名  
            ifstream inFile("scores.txt");
            int rank = 0;  // 初始化排名计数  
            char buffer[256];

            // 读取文件以计算当前行数即排名  
            while (inFile.getline(buffer, sizeof(buffer))) {
                rank++;
            }

            // 将当前得分写入文件  
            outFile << rank + 1 << ". " << snake.score << "分" << endl;
            // 将得分添加到玩家得分列表  
            playerscore.push_back(snake.score);
            cout << "得分已记录!" << endl; // 提示得分已记录  
        }
        else {
            cout << "无法打开文件以记录得分!" << endl; // 如果无法打开文件  
        }

        outFile.close(); // 关闭文件  
    }

    // 进入一个循环，判断用户是否要返回主菜单  
    while (true) {
        ExMessage msg;
        // 检查鼠标消息  
        if (peekmessage(&msg, WH_MOUSE)) {
            switch (msg.message)
            {
            case WM_LBUTTONDOWN: // 鼠标左键按下事件  
                // 检查点击位置是否在“返回主菜单”按钮范围内  
                if (msg.x >= 1000 && msg.x <= 1200 && msg.y >= 650 && msg.y <= 750)
                {
                    mciSendString("close WIN", 0, 0, 0);
                    Init(); // 重新初始化游戏  
                    menuView(); // 返回主菜单  

                }
                break;
            default:
                break;
            }
        }
    }

    // 等待用户输入（未得到使用的内容）  
    getchar();

    // 关闭图形界面  
    closegraph();
}
void moreView() {
    //设置背景颜色

    setbkcolor(RGB(113, 218, 239));
    cleardevice();

    //背景图
    IMAGE img;
    loadimage(&img, "./image/bk1.jpg", Width, Height);
    putimage(0, 0, &img);

    //按钮绘制
    button(200, 350, 200, 75, "玩法说明");//400
    button(500, 350, 200, 75, "开发者名单");//500 700
    button(800, 350, 200, 75, "返回主菜单");
    button(350, 550, 200, 75, "皮肤展示");
    button(650, 550, 200, 75, "成就展示");
    //鼠标操作
    ExMessage msg;
    while (true) {
        if (peekmessage(&msg, EX_MOUSE)) {
            switch (msg.message) {
            case WM_LBUTTONDOWN:
                if (msg.x >= 200 && msg.x <= 400 && msg.y >= 350 && msg.y <= 425)
                {
                    ruleView();
                }
                if (msg.x >= 500 && msg.x <= 700 && msg.y >= 350 && msg.y <= 425)
                {
                    developersView();
                }
                if (msg.x >= 350 && msg.x <= 550 && msg.y >= 550 && msg.y <= 625)
                {
                    skinView();
                }
                if (msg.x >= 650 && msg.x <= 850 && msg.y >= 550 && msg.y <= 625)
                {
                    achievementView();
                }
                if (msg.x >= 800 && msg.x <= 1000 && msg.y >= 350 && msg.y <= 425)
                {
                    menuView();
                }
                break;
            default:
                break;
            }
        }

    }

}
void ruleView()
{
    //玩法说明
    setbkcolor(RGB(113, 218, 239));//颜色取自猫猫虫原型图主界面
    cleardevice();
    //绘制标题
    settextcolor(BLACK);
    settextstyle(40, 0, "微软雅黑");
    char arr[] = "玩法说明";
    int wid = Width / 2 - textwidth(arr) / 2;
    int hei = textheight(arr) / 2;
    outtextxy(wid, hei, arr);
    setbkmode(TRANSPARENT);
    //输出游戏规则
    settextstyle(30, 0, "微软雅黑");
    char arr1[] = "1.猫猫虫出于觅食的本能，每隔一段时间会自己移动一格";
    char arr2[] = "2.玩家点击w,a,s,d 或 方向键 改变猫猫虫移动方向";
    char arr3[] = "3.猫猫虫讨厌吃蔬菜，吃到蔬菜，猫猫虫会呕吐并长度-3，分数不变，速度增加";
    char arr4[] = "4.猫猫虫愿意吃肉，吃到肉，猫猫虫长度+1，分数+5";
    char arr5[] = "5.猫猫虫喜欢吃兔兔，吃到兔兔，猫猫虫长度+2，分数+10，速度增加";
    char arr6[] = "6.猫猫虫很脆弱，碰到墙或者自身都会死亡，GAMEOVER";
    outtextxy(Width / 2 - textwidth(arr3) / 2, Height / 4, arr1);
    outtextxy(Width / 2 - textwidth(arr3) / 2, Height / 4 + 2 * textheight(arr1), arr2);
    outtextxy(Width / 2 - textwidth(arr3) / 2, Height / 4 + 4 * textheight(arr1), arr3);
    outtextxy(Width / 2 - textwidth(arr3) / 2, Height / 4 + 6 * textheight(arr1), arr4);
    outtextxy(Width / 2 - textwidth(arr3) / 2, Height / 4 + 8 * textheight(arr1), arr5);
    outtextxy(Width / 2 - textwidth(arr3) / 2, Height / 4 + 10 * textheight(arr1), arr6);
    //返回主界面（绘制按钮）
    setbkmode(TRANSPARENT);
    setfillcolor(RGB(115, 251, 253));//颜色可再议
    int left = Width / 2 - 75;
    int right = Width / 2 + 75;
    int higher = Height * 3 / 4 + 25;
    int top = Height * 3 / 4 + 25;
    int bottom = Height * 3 / 4 + 75;
    fillroundrect(left, top, right, bottom, 10, 10);
    settextstyle(20, 0, "楷体加粗");
    const char* text = "返回更多";
    int tx = (left + right) / 2 - textwidth(text) / 2;
    int ty = (top + bottom) / 2 - textheight(text) / 2;
    outtextxy(tx, ty, text);
    ExMessage msg;
    while (true) { //不断地处理事务，需要循环
        if (peekmessage(&msg, EX_MOUSE))
        {
            switch (msg.message)
            {
            case WM_LBUTTONDOWN:
                if (msg.x >= left && msg.x <= right && msg.y >= top && msg.y <= bottom)
                    moreView();
                break;
            default:
                break;
            }

        }
    }

    closegraph();
}
void developersView()
{
    //创建背景
    setbkcolor(RGB(113, 218, 239));
    cleardevice();
    //打印标题
    settextcolor(RGB(0, 0, 0));
    settextstyle(50, 0, "黑体加粗");
    setbkmode(TRANSPARENT);
    char arr[] = "开发者名单";
    int x = (1200 - textwidth(arr)) / 2;
    outtextxy(x, 80, arr);
    //逐一打印制作者
    char arr0[] = "六天";
    char arr1[] = "Antithesis";
    char arr2[] = "evy";
    char arr3[] = "荣";
    char arr4[] = "钟晴";
    char arr5[] = "雷";
    char arr6[] = "影";
    char arr7[] = "李子";
    char arr8[] = "shy";
    char arr9[] = "邵";

    settextcolor(RGB(0, 0, 0));
    settextstyle(40, 0, "楷体加粗");
    setbkmode(TRANSPARENT);
    int x1 = 0;
    int y1 = 160;
    x1 = (1200 - textwidth(arr0)) / 2;
    outtextxy(x1, y1, arr0);
    y1 += textheight(arr0);

    x1 = (1200 - textwidth(arr1)) / 2;
    outtextxy(x1, y1, arr1);
    y1 += textheight(arr0);

    x1 = (1200 - textwidth(arr2)) / 2;
    outtextxy(x1, y1, arr2);
    y1 += textheight(arr0);

    x1 = (1200 - textwidth(arr3)) / 2;
    outtextxy(x1, y1, arr3);
    y1 += textheight(arr0);

    x1 = (1200 - textwidth(arr4)) / 2;
    outtextxy(x1, y1, arr4);
    y1 += textheight(arr0);

    x1 = (1200 - textwidth(arr5)) / 2;
    outtextxy(x1, y1, arr5);
    y1 += textheight(arr0);

    x1 = (1200 - textwidth(arr6)) / 2;
    outtextxy(x1, y1, arr6);
    y1 += textheight(arr0);

    x1 = (1200 - textwidth(arr7)) / 2;
    outtextxy(x1, y1, arr7);
    y1 += textheight(arr0);

    x1 = (1200 - textwidth(arr8)) / 2;
    outtextxy(x1, y1, arr8);
    y1 += textheight(arr0);

    x1 = (1200 - textwidth(arr9)) / 2;
    outtextxy(x1, y1, arr9);
    y1 += textheight(arr0);
    //打印返回按钮
    button(500, 600, 200, 75, "返回更多");
    //接受鼠标信息
    ExMessage msg;
    while (true)
    {
        if (peekmessage(&msg, EX_MOUSE))
        {
            switch (msg.message) {
            case WM_LBUTTONDOWN:
                if (msg.x >= 500 && msg.x <= 700 && msg.y <= 675 && msg.y >= 600)
                {
                    moreView();
                }
            }


        }
    }

    getchar();
    closegraph();

}
void skinView() {
    //完成代码
}
void achievementView() {
    cleardevice();
    //背景
    IMAGE img;
    loadimage(&img, "./image/bk1.jpg", Width, Height);
    putimage(0, 0, &img);
    //选项
    button(150, 100, 200, 75, "成就1");
    button(150, 200, 200, 75, "成就2");
    button(150, 300, 200, 75, "成就3");
    button(150, 400, 200, 75, "成就4");
    button(150, 600, 200, 75, "返回更多");

    ifstream achin;                      //创建文件输入流对象
    achin.open("achievements.txt");      //打开输入流文件
    string ach_data;
    int arr[4] = { 0 };
    for (int i = 0, j = 0; i < 8 && getline(achin, ach_data); i += 2, ++j) {
        istringstream ss(ach_data);
        ss >> arr[j];
    }                                    //读取
    achin.close();                       //关闭输入流文件
    arr[0] = 1;
    settextstyle(30, 0, "微软雅黑");
    setbkmode(TRANSPARENT);
    ExMessage msg;
    while (true) {                       //点击
        if (peekmessage(&msg, EX_MOUSE)) {
            switch (msg.message) {
            case WM_LBUTTONDOWN:
                if (msg.x >= 150 && msg.x <= 350 && msg.y >= 100 && msg.y <= 175) {
                    button(550, 250, 400, 250, "");
                    if (arr[0] == 1) {
                        outtextxy(550 + (400 - textwidth(_T("初出茅庐"))) / 2, 325, _T("初出茅庐"));
                        outtextxy(550 + (400 - textwidth(_T("已解锁"))) / 2, 355, _T("已解锁"));
                        outtextxy(550 + (400 - textwidth(_T("首次游玩本游戏！"))) / 2, 385, _T("首次游玩本游戏！"));
                    }
                    else {
                        button(550, 250, 400, 250, "未解锁的成就");
                    }
                }
                if (msg.x >= 150 && msg.x <= 350 && msg.y >= 200 && msg.y <= 275) {
                    button(550, 250, 400, 250, "未解锁的成就");
                }
                if (msg.x >= 150 && msg.x <= 350 && msg.y >= 300 && msg.y <= 375) {
                    button(550, 250, 400, 250, "未解锁的成就");
                }
                if (msg.x >= 150 && msg.x <= 350 && msg.y >= 400 && msg.y <= 475) {
                    button(550, 250, 400, 250, "未解锁的成就");
                }
                if (msg.x >= 150 && msg.x <= 350 && msg.y >= 600 && msg.y <= 675) {
                    moreView();
                }
                break;
            default:
                break;
            }
        }

    }
}
void listView() {
    Init();
    setbkcolor(RGB(113, 218, 239)); // 设置背景颜色  
    cleardevice(); // 清空绘图设备，准备绘制  

    // 设置文本背景模式为透明  
    setbkmode(TRANSPARENT);
    setfillcolor(RGB(115, 251, 253)); // 设置填充颜色为棕色  
    // 绘制一个圆角矩形  
    fillroundrect(1000, 650, 1200, 750, 10, 10); // 矩形的坐标和圆角半径  

    // 设置文本样式，字体大小为30，字体为“黑体”  
    settextstyle(30, 0, "黑体");
    char arr[50] = "返回主菜单"; // 按钮文本  
    // 计算文本的位置，使其居中  
    int tx = 1000 + (200 - textwidth(arr)) / 2;
    int ty = 650 + (100 - textheight(arr)) / 2;

    // 在计算的位置输出文本  
    outtextxy(tx, ty, arr);

    sort(playerscore.begin(), playerscore.end(), cmp);
    // 输出"排行榜"标题  
    settextstyle(50, 0, "黑体"); // 设置标题字体样式  
    char s[50] = "排行榜"; // 标题文本  
    // 计算标题的位置，使其居中  
    int ww = (1200 - textwidth(s)) / 2;
    int hh = (120 - textheight(s)) / 2;
    outtextxy(ww, hh, s); // 输出标题  
    settextstyle(20, 0, _T("楷体")); // 设置分数列表字体样式  
    settextcolor(BLACK); // 设置文本颜色为黑色  
    setbkmode(TRANSPARENT); // 设置文本背景模式为透明 
    int h = 120; // 初始化输出文本的 y 坐标  
    outtextxy(0, h, "名次  分数\n");
    h += 30;
    char a[100];
    for (int i = 0; i < playerscore.size(); i++) {
        sprintf_s(a, "%d     %d\n", i + 1, playerscore[i]); // 格式化字符串，输出排名和分数   
        outtextxy(0, h, a); // 输出分数在指定位置  
        h += 30; // 增加 y 坐标，使下一个分数下移  
    }
    ofstream file("leaderboard.txt");
    if (file.is_open()) {
        file << "名次\t分数\n";
        for (int i = 0; i < playerscore.size(); i++) {
            file << i + 1 << '\t' << playerscore[i] << "\n";
        }
        file.close();
    }
    else {
        cout << "打开文件leaderboard.txt失败！";
    }
    // 处理消息循环，用于检测鼠标点击以返回菜单  
    while (true) {
        ExMessage msg; // 定义消息结构体  
        if (peekmessage(&msg, WH_MOUSE)) { // 检查是否有鼠标消息  
            switch (msg.message) {
            case WM_LBUTTONDOWN: // 检测左键点击  
                // 检查点击是否在“返回主菜单”按钮区域内  
                if (msg.x >= 1000 && msg.x <= 1200 && msg.y >= 650 && msg.y <= 750) {
                    menuView(); // 调用菜单视图函数  
                }
                break;
            default:
                break;
            }
        }
    }

    getchar(); // 等待用户输入  
    closegraph(); // 关闭图形窗口  
}
void musicsetView1() {

    cleardevice();
    IMAGE img;//定义一个图片名.
    loadimage(&img, "./image/musicset.jpg", 1200, 750);//从图片文件获取图像
    putimage(0, 0, &img);//绘制图像到屏幕，图片左上角坐标为(0,0)
    button(660, 360, 90, 40, "+");
    button(660, 420, 90, 40, "-");
    button(500, 610, 185, 70, "返回主菜单");
    button(480, 370, 100, 80, "音量");
    int i = 200;
    char a[100];
    mciSendString("status music volume", a, 0, 0);//获取当前的音量
    i = atoi(a);//转换成int 类型
    ExMessage msg;
    while (true) {
        if (peekmessage(&msg, EM_MOUSE))
        {
            switch (msg.message)
            {
            case WM_LBUTTONDOWN:
                if (msg.x > 660 && msg.x < 750 && msg.y>360 && msg.y < 400)
                {
                    i += 100;
                }
                else if (msg.x > 660 && msg.x < 750 && msg.y>420 && msg.y < 460)
                {
                    i -= 100;
                }
                else if (msg.x > 500 && msg.x < 685 && msg.y>610 && msg.y < 680)
                {
                    menuView();
                }
                break;
            default:
                break;
            }
            sprintf_s(a, "setaudio music volume to %d", i);//把此时的音量以及操作字符串赋给a
            mciSendString(a, 0, 0, 0);

        }
    }

    getchar();
}
void musicsetView2() {

    cleardevice();
    IMAGE img;//定义一个图片名.
    loadimage(&img, "./image/musicset.jpg", 1200, 750);//从图片文件获取图像
    putimage(0, 0, &img);//绘制图像到屏幕，图片左上角坐标为(0,0)
    button(660, 360, 90, 40, "+");
    button(660, 420, 90, 40, "-");
    button(500, 520, 185, 70, "返回游戏");
    button(500, 610, 185, 70, "返回主菜单");
    button(480, 370, 100, 80, "音量");
    int i = 200;
    char a[100];
    mciSendString("status BGM volume", a, 0, 0);//获取当前的音量
    i = atoi(a);//转换成int 类型
    ExMessage msg;
    while (true) {
        if (peekmessage(&msg, EM_MOUSE))
        {
            switch (msg.message)
            {
            case WM_LBUTTONDOWN:
                if (msg.x > 660 && msg.x < 750 && msg.y>360 && msg.y < 400)
                {
                    i += 100;
                }
                else if (msg.x > 660 && msg.x < 750 && msg.y>420 && msg.y < 460)
                {
                    i -= 100;
                }
                else if (msg.x > 500 && msg.x < 685 && msg.y>520 && msg.y < 590)
                {
                    GameDraw();
                }
                else if (msg.x > 500 && msg.x < 685 && msg.y>610 && msg.y < 680)
                {
                    mciSendString("close BGM", 0, 0, 0);
                    menuView();
                }
                break;
            default:
                break;
            }
            sprintf_s(a, "setaudio BGM volume to %d", i);//把此时的音量以及操作字符串赋给a
            mciSendString(a, 0, 0, 0);

        }
    }

    getchar();
}
void musicPlay(int i) {

    switch (i)//根据i值播放对应曲目
    {
    case 0:
        mciSendString("open ./music/0.mp3 alias music", 0, 0, 0);
        mciSendString("play music repeat ", 0, 0, 0);


        break;
    case 1:
        mciSendString("open ./music/1.mp3 alias music", 0, 0, 0);
        mciSendString("play music repeat", 0, 0, 0);

        break;

    case 2:
        mciSendString("open ./music/2.mp3 alias music", 0, 0, 0);
        mciSendString("play music repeat", 0, 0, 0);


        break;
    case 3:
        mciSendString("open ./music/3.mp3 alias music", 0, 0, 0);
        mciSendString("play music repeat", 0, 0, 0);
        ;

        break;
    case 4:
        mciSendString("open ./music/4.mp3 alias music", 0, 0, 0);
        mciSendString("play music repeat ", 0, 0, 0);
        break;
    case 5:
        mciSendString("open ./music/5.mp3 alias music", 0, 0, 0);
        mciSendString("play music repeat ", 0, 0, 0);
        break;
    case 6:
        mciSendString("open ./music/6.mp3 alias music", 0, 0, 0);
        mciSendString("play music repeat ", 0, 0, 0);
        break;

    }




}
//-------------------------------service函数实现--------------------------------------
void Init()
{
    srand(GetTickCount());
    snake.flag = 1;//蛇是否存活
    snake.len = 3;//蛇身长度
    snake.dir = DIR::RIGHT;//蛇头方向 UP DOWN RIGHT LEFT
    snake.pos[0][0] = 320;
    snake.pos[1][0] = 64;
    snake.pos[0][1] = 292;
    snake.pos[1][1] = 64;
    snake.pos[0][2] = 264;
    snake.pos[1][2] = 64;
    //每一节蛇的坐标，行数为0时表示横坐标x，为1时表示纵坐标y
    snake.speed = 300;//蛇的速度
    snake.score = 0;//获得的分数
    veg.add = -3;//蔬菜属性
    meat.add = 1;//肉的属性
    rabbit.add = 2;//兔子属性
    veg.flag = 0;//蔬菜flag为0，方便后面随机生成
    meat.flag = 0;//肉同理
    rabbit.flag = 0;//兔子同理
    level = 0;//定义当前关卡数
    mode_choice = 0;

}
void SnakeMove() {
    //遍历所有蛇的节点
    for (int i = snake.len - 1; i > 0; i--)
    {//每一节跟着前一节移动
        snake.pos[0][i] = snake.pos[0][i - 1];
        snake.pos[1][i] = snake.pos[1][i - 1];
    }
    switch (snake.dir)//根据蛇头方向实现对应的移动
    {
    case UP:

        snake.pos[1][0] = snake.pos[1][0] - 28;
        if (snake.pos[1][0] <= 50)//穿过上面
        {
            snake.pos[1][0] = 750 - 14;
        }
        break;
    case DOWN:
        snake.pos[1][0] = snake.pos[1][0] + 28;
        if (snake.pos[1][0] >= 750)//穿过下面
        {
            snake.pos[1][0] = 50 + 14;
        }
        break;
    case LEFT:
        snake.pos[0][0] = snake.pos[0][0] - 28;
        if (snake.pos[0][0] <= 250)//穿过左边
        {
            snake.pos[0][0] = 950 - 14;
        }
        break;
    case RIGHT:
        snake.pos[0][0] = snake.pos[0][0] + 28;
        if (snake.pos[0][0] >= 950)//穿过右边
        {
            snake.pos[0][0] = 250 + 14;
        }
        break;
    default:
        break;
    }

}
void keyControl() {
    //获取对应按键，实现蛇头方向改变
    if ((GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP)) && snake.dir != DOWN)
    {
        snake.dir = UP;
    }
    if ((GetAsyncKeyState('S') || GetAsyncKeyState(VK_DOWN)) && snake.dir != UP)
    {
        snake.dir = DOWN;
    }
    if ((GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT)) && snake.dir != RIGHT)
    {
        snake.dir = LEFT;
    }
    if ((GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT)) && snake.dir != LEFT)
    {
        snake.dir = RIGHT;
    }

}
void foodCreat()
{
    if (veg.flag == 0 || map[level][(veg.y - 50) / 28][(veg.x - 250) / 28] == 1||(veg.x==meat.x&&veg.y==meat.y)||(veg.x==rabbit.x&&veg.y==rabbit.y)) {
    re1:
        veg.x = 250 + 28 * (rand() % 25);
        veg.y = 50 + 28 * (rand() % 25);
        if (map[level][(veg.y - 50) / 28][(veg.x - 250) / 28] == 0)
            veg.flag = 1; // 标记蔬菜存在
        else
            goto re1; // 如果冲突则重新生成
    }

    // 生成肉的坐标
    if (meat.flag == 0 || map[level][(meat.y - 50) / 28][(meat.x - 250) / 28] == 1 || (veg.x == meat.x && veg.y == meat.y) || (meat.x == rabbit.x && meat.y == rabbit.y)) 
    {
    re2:
        meat.x = 250 + 28 * (rand() % 25);
        meat.y = 50 + 28 * (rand() % 25);
        if (map[level][(meat.y - 50) / 28][(meat.x - 250) / 28] == 0)
            meat.flag = 1; // 标记肉存在
        else
            goto re2; // 如果冲突则重新生成
    }

    // 生成兔子的坐标
    if (rabbit.flag == 0 || map[level][(rabbit.y - 50) / 28][(rabbit.x - 250) / 28] == 1 || (rabbit.x == meat.x && rabbit.y == meat.y) || (veg.x == rabbit.x && veg.y == rabbit.y)) {
    re3:
        rabbit.x = 250 + 28 * (rand() % 25);
        rabbit.y = 50 + 28 * (rand() % 25);
        if (map[level][(rabbit.y - 50) / 28][(rabbit.x - 250) / 28] == 0)
            rabbit.flag = 1; // 标记兔子存在
        else
            goto re3; // 如果冲突则重新生成
    }
}
void eatFood() {
    if (snake.pos[0][0] == veg.x + 14 && snake.pos[1][0] == veg.y + 14)//当判定蛇头的坐标与蔬菜的坐标一样时
    {
        snake.len += veg.add;//获得蔬菜的加成
        snake.speed = snake.speed * 0.8;//速度增加
        veg.flag = 0;//标记为吃掉
        //播放音效

    }
    else if (snake.pos[0][0] == meat.x + 14 && snake.pos[1][0] == meat.y + 14)//当判定蛇头的坐标与肉的坐标一样时
    {
        snake.len += meat.add;
        snake.score += 5;//分数增加
        meat.flag = 0;//标记为吃掉
        //播放音效



    }
    else if (snake.pos[0][0] == rabbit.x + 14 && snake.pos[1][0] == rabbit.y + 14)//当判定蛇头的坐标与兔子的坐标一样时
    {
        snake.len += rabbit.add;
        snake.score += 10;//加分
        snake.speed = snake.speed * 0.9;//速度增加
        rabbit.flag = 0;//标记为吃掉
        //播放音效

    }
    if (veg.flag == 0 || meat.flag == 0 || rabbit.flag == 0)//如果有一个被吃掉
    {
        foodCreat();//重新生成该食物
    }
}
void dieJudge() {
    int k;
    int m;
    // 判断蛇是否碰到墙
    k = (snake.pos[1][0] - 64) / 28;
    m = (snake.pos[0][0] - 264) / 28;
    if (map[level][k][m] == 1)
    {
        snake.flag = 0;// 蛇死亡
        // 如果蛇死亡，调用游戏结算界面
        if (snake.flag == 0)
        {
            endView();
        }
    }


    // 判断蛇的长度是否为0
    if (snake.len <= 0) {
        snake.flag = 0; // 蛇死亡
        if (snake.flag == 0) {
            endView();
        }
    }
    // 判断蛇是否碰到自己
    for (int i = 1; i < snake.len; i++)
    {

        if (snake.pos[0][0] == snake.pos[0][i] && snake.pos[1][0] == snake.pos[1][i]) {
            snake.flag = 0; // 蛇死亡
            break;
        }

    }

    // 如果蛇死亡，调用游戏结算界面
    if (snake.flag == 0) {
        endView();
    }
}
bool cmp(int player1, int player2)
{
    return player1 > player2;
}
int FPS(double f)
{
    static long long oldtime = 0;
    long long nowtime = clock();
    if (nowtime - oldtime >= f)
    {
        oldtime = nowtime;
        return 1;
    }
    return 0;
}

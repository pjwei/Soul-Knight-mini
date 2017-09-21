#ifndef SOUL_KNIGHT
#define SOUL_KNIGHT


#include <graphics.h>      // 引用图形库头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#pragma comment(lib,"Winmm.lib")


#define KEY_DOWN(vk_c) (GetAsyncKeyState(vk_c) & 0x8000 ? 1 : 0)  //按键判断
#define window_width   1280 
#define window_height  720                  //窗口大小
#define speed          14                   //玩家移动速度
#define EBspeed        15                   //怪物子弹移动速度
#define RIGHT          0					//角色朝向（右）
#define LEFT           1                    //角色朝向（左）
#define VK_Z           0x5A					//z按键
#define VK_X           0x58					//x按键
#define KEY_ESC        27					//ESC ASCII码
#define KEY_RIGHT      77					//右方向键 ASCII码
#define KEY_LEFT       75                   //左方向键 ASCII码
#define PI             3.1415926536			//圆周率
#define FLOOR          1					//地板等可行走位置
#define GATEWAY        2					//传送门
#define CRYSTAL        3					//水晶治疗范围
#define IMMORTAL_OBJ   0					//墙壁等不可行走/破坏物品
#define SPECIES        3					//怪物种类
#define WEAPONS        2					//武器种类
#define STAGE		   4					//关卡总数


typedef struct weapon {
	char *name;			//武器名称
	int damage;			//武器攻击力
	int cost;			//武器消耗
	int swift;			//武器射速
	int Bspeed;			//子弹移动速度
	double accuracy;	//武器精准度
	double spin;		//武器旋转角
	IMAGE left;			//左侧图像
	IMAGE right;		//右侧图像
	IMAGE v_left;		//左侧遮蔽图
	IMAGE v_right;		//右侧遮蔽图
} Weapon;				//武器信息结构体

typedef struct player {
	const int HP_MAX = 7;	//HP上限
	const int AM_MAX = 6;	//AM上限
	const int MP_MAX = 200;	//MP上限
	int left;				//人物判定点左侧坐标（窗口）
	int right;				//人物判定点右侧坐标（窗口）
	int top;				//人物判定点顶部坐标（窗口）
	int bottom;				//人物判定点底部坐标（窗口）
	POINT center;			//人物判定点中心横纵坐标（窗口）
	int HP;					//当前HP
	int AM;					//当前AM
	int MP;					//当前MP
	int aspect;				//人物朝向
	int weapon_number;		//当前携带武器编号
} Player;					//人物信息结构体

typedef struct enemy_status {
	int x;					//贴图基准横坐标
	int y;					//贴图基准纵坐标
	int pic_x;				//行走图列
	int pic_y;				//行走图行
	double spin;			//远程怪物武器旋转角
	int think;				//思考行为时间比例调控
	int move;				//移动行为时间比例调控
	int think_time;			//思考持续时间
	int move_time;			//移动持续时间	
	int direction_time[5];	//单方向持续时间
	int back_time;			//回退持续时间
	int attack_time;		//攻击冷却时间
	int not_cnt;			//未发现状态的时间计数
	int is_cnt;				//发现状态的时间计数
	int direction_cnt;		//单方向时间计数
	int back_cnt;			//回退时间计数
	int attack_cnt;			//攻击冷却时间计数
	int aspect;				//怪物朝向
	int HP;					//当前血量
	int is_damaged;			//被击判定
	int damaged_cnt;		//被击硬直时间
} enemy_status;				//单个怪物信息结构体

typedef struct enemy_species {
	int amount;				//怪物总数
	int old_amount;			//房间内怪物总数
	enemy_status num[50];	//同种的怪物组
	int pic_left;			//图片中怪物图像距图片边框左侧距离
	int pic_right;			//图片中怪物图像距图片边框右侧距离
	int pic_top;			//图片中怪物图像距图片边框顶部距离
	int pic_bottom;			//图片中怪物图像距图片边框底部距离
	int HP_MAX;				//HP上限
	int damage;				//攻击力
	int Espeed;				//移动速度
	IMAGE model;
	IMAGE model_vice;
	IMAGE init_gun_left;
	IMAGE init_gun_right;
	IMAGE init_gun_vice_left;
	IMAGE init_gun_vice_right;
	IMAGE bullet;
	IMAGE bullet_vice;
} enemy_species;			//怪物种族结构体

typedef struct bullet
{
	POINT position;			//子弹中心点坐标
	double spin;			//子弹发射角
	int Bspeed;				//子弹移动速度
	int hit;				//子弹击中敌人情况
	int path;				//子弹穿过路径模拟
	int burst_cnt;			//子弹炸裂效果计时
	int spc;				//怪物子弹的种族标记
	struct bullet *next;
}BULLET;					//存储子弹信息的链表


VOID CALLBACK timer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void start_menu();						//开始界面相关
void pause_menu();						//暂停界面相关
void pause_back();						//暂停界面回退
void startup_music();					//音乐初始化
void startup_menu_show();				//菜单初始化
void startup_map_show();				//地图初始化
void startup_status_show();				//人物状态初始化
void startup_weapon_show();				//武器初始化
void startup_character_show();			//人物初始化
void map_show();						//地图显示相关
void character_show();					//人物显示相关
void transform();						//过关传送动画
int auto_aiming();						//自动瞄准相关
void bullet_show();						//子弹显示相关
void status_show();						//状态栏显示
void armor_recover();					//护甲回复
void operate_control();					//人物控制键盘操作相关
void operate_select();					//菜单选择键盘操作相关
void startup_enemy();					//怪物信息初始化
void obj_mark(int left, int top, int right, int bottom, int obj);  //将指定区域内的地图对象标记为指定物品种类
int enemy_generate(int left, int top, int right, int bottom, int spc, int range);  //在指定区域内的适当位置随机生成指定范围数量内的指定种类怪物
void enemy_show(int spc, int num);		//怪物显示相关
void enemy_shoot(int spc, int num);		//怪物射击判断
void enemy_bullet();					//怪物子弹显示相关
int isdetected(int spc, int num);		//判断怪物是否发现玩家
void melee_AI(int spc, int num);		//近战怪物AI
void shoot_AI(int spc, int num);		//远程怪物AI
void BOSS_AI(int spc, int num);			//首领怪物AI
void room1_mid();						//关卡1中间房间
void room1_left();						//关卡1左侧房间
void room1_down();						//关卡1下方房间
void room2_mid();						//关卡2中间房间
void room2_right();						//关卡2右侧房间
void room2_down();						//关卡2下方房间
void room3_mid();						//关卡3中间房间
void room3_right();						//关卡3右侧房间
void room3_down();						//关卡3下方房间
void room3_downright();					//关卡3右下房间
void BOSS_room();						//BOSS房间


#endif // !SOUL_KNIGHT


#include "AI.h"

void melee_AI(int spc, int num) {
	//以怪物为中心建立坐标系，向右为x轴正方向、向下为y轴正方向（即默认图形坐标系）

	int dir;							//方向循环变量
	int dx;								//怪物到玩家判定中心横坐标增量
	int dy;								//怪物到玩家判定中心纵坐标增量
	double distance;					//怪物与玩家之间的距离
	double cosarc;						//怪物到玩家夹角余弦值
	double sinarc;						//怪物到玩家夹角正弦值
	double arc;							//怪物到玩家夹角，范围 [0, 2PI)
	double marc;						//夹角偏差值
	int action;							//行为判断参量
	int	move;							//移动判断参量
	int attack;							//攻击判断参量
	int	mx;								//横向移动距离
	int my;								//纵向移动距离
	static int backx;					//横向回退距离记录
	static int backy;					//纵向回退距离记录
	int Eleft, Eright, Etop, Ebottom;   //怪物当前在窗口中判定点四方向坐标
	POINT Ecenter;                      //怪物当前在窗口中中心点坐标

	Eleft = enemy[spc].num[num].x - map.x + enemy[spc].pic_left;
	Eright = enemy[spc].num[num].x - map.x + enemy[spc].pic_right;
	Etop = enemy[spc].num[num].y - map.y + enemy[spc].pic_top;
	Ebottom = enemy[spc].num[num].y - map.y + enemy[spc].pic_bottom;    
	Ecenter.x = (Eleft + Eright) / 2;
	Ecenter.y = (Etop + Ebottom) / 2;

	dx = character_position.x - (enemy[spc].num[num].x - map.x);
	dy = character_position.y - (enemy[spc].num[num].y - map.y);
	distance = sqrt(dx * dx + dy * dy);										  

	if (distance > 0 && enemy[spc].num[num].HP > 0) {
		cosarc = dx / distance;												  
		sinarc = dy / distance;												  	
		if (dy >= 0) arc = acos(cosarc);
		else arc = 2 * PI - acos(cosarc);	

		if (dy >= 0) enemy[spc].num[num].spin = 2 * PI - acos(cosarc) - PI / 2;
		else enemy[spc].num[num].spin = acos(cosarc) - PI / 2;
		if (enemy[spc].num[num].spin < 0) enemy[spc].num[num].spin += 2 * PI;

		if (enemy[spc].num[num].pic_x == 3)
			enemy[spc].num[num].pic_x = 0;									  //行为图轮换

		//未发现玩家																  
		if (!isdetected(spc, num)) {
			if (enemy[spc].num[num].think_time != 0 && enemy[spc].num[num].not_cnt <= enemy[spc].num[num].think_time) action = 0;     //记忆思考行为
			else if (enemy[spc].num[num].move_time != 0 && enemy[spc].num[num].not_cnt <= enemy[spc].num[num].move_time) action = 1;  //记忆移动行为
			else action = rand() % 2;										  //产生随机行为

			//约1/2概率进入思考行为
			if (action == 0) {												  
				enemy[spc].num[num].move = 0;								  //初始化移动行为行走图连续切换
				if (enemy[spc].num[num].not_cnt == 0)
					enemy[spc].num[num].think_time = rand() % 21 + 30;		  //随机思考时间30~50 * 30ms = 0.9~1.5s

				enemy[spc].num[num].pic_y = enemy[spc].num[num].aspect;       //切换到站立图状态行
				if (enemy[spc].num[num].think == 6) {
					enemy[spc].num[num].pic_x++;							  //按时间调控比例切换站立图					
				}

				if (enemy[spc].num[num].not_cnt == enemy[spc].num[num].think_time) {
					enemy[spc].num[num].not_cnt = 0;
					enemy[spc].num[num].think_time = 0;						  //思考行为周期结束
				}
				else enemy[spc].num[num].not_cnt++;							  //思考行为周期计数
			}

			//约1/2概率进入移动行为
			else {															  
				enemy[spc].num[num].think = 0;								  //初始化思考行为行走图连续切换
				if (enemy[spc].num[num].not_cnt == 0)
					enemy[spc].num[num].move_time = rand() % 111 + 30;		  //随机移动时间30~140 * 30ms = 0.9~4.2s
				move = rand() % 100 + 1;
				for (dir = 0; dir < 5; dir++) {								  //根据记忆路径进行移动
					if (enemy[spc].num[num].direction_time[dir] != 0 && enemy[spc].num[num].direction_cnt <= enemy[spc].num[num].direction_time[dir]) {
						switch (dir) {
						case 0:move = 1; break;
						case 1:move = 36; break;
						case 2:move = 61; break;
						case 3:move = 81; break;
						case 4:move = 100; break;
						}
						break;
					}
				}
				if (enemy[spc].num[num].back_time != 0 && enemy[spc].num[num].back_cnt <= enemy[spc].num[num].back_time) move = 0;  //记忆为回退时的移动

				//按概率递减的方式从最优路径到最差路径进行决策															  
				if (move >= 1 && move <= 35) {								  //35%概率向最优方向前进
					mx = (int)(enemy[spc].Espeed * cosarc);
					my = (int)(enemy[spc].Espeed * sinarc);
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[0] = rand() % 21 + 10; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[0]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[0] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}															  
				else if (move >= 36 && move <= 60) {						  //25%概率向 (0,PI/4] 偏差范围大幅前进
					if (rand() % 2) marc = (rand() % 5 + 1) * PI / 20;
					else marc = -(rand() % 5 + 1) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[1] = rand() % 21 + 10; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[1]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[1] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}															  
				else if (move >= 61 && move <= 80) {						  //20%概率向 (PI/4,PI/2] 偏差范围小幅前进
					if (rand() % 2) marc = (rand() % 5 + 6) * PI / 20;
					else marc = -(rand() % 5 + 6) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[2] = rand() % 21 + 10; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[2]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[2] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}															  
				else if (move >= 81 && move <= 95) {						  //15%概率向 (PI/2,3PI/4] 偏差范围小幅后退
					if (rand() % 2) marc = (rand() % 5 + 11) * PI / 20;
					else marc = -(rand() % 5 + 11) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[3] = rand() % 11 + 20; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[3]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[3] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}															  
				else {														  //5%概率向 (3PI/4,PI] 偏差范围大幅后退
					if (rand() % 2) marc = (rand() % 5 + 16) * PI / 20;
					else marc = -(rand() % 5 + 16) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[4] = rand() % 11 + 20; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[4]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[4] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}															  
				//障碍撞击判定
				if (object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
					|| object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
					|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
					|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
					|| move == 0)
				{
					if (enemy[spc].num[num].back_cnt == 0) {
						backx = -mx;
						backy = -my;										  //记录回退方向
						enemy[spc].num[num].direction_cnt = 0;				  //清除单次移动周期计数
						for (dir = 0; dir < 5; dir++)
							enemy[spc].num[num].direction_time[dir] = 0;	  //清除前进方向周期记忆
						enemy[spc].num[num].back_time = rand() % 11 + 20;     //随机回退时间20~30 * 30ms = 0.6~0.9s
					}
					mx = backx;
					my = backy;

					if (object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ)
					{
						mx = 0;
						my = 0;
						enemy[spc].num[num].back_cnt = 0;
						enemy[spc].num[num].back_time = 0;					  //回退过程中发生碰撞，中止回退
					}

					if (enemy[spc].num[num].back_cnt == enemy[spc].num[num].back_time) {
						enemy[spc].num[num].back_cnt = 0;
						enemy[spc].num[num].back_time = 0;					  //回退行为周期结束
					}
					else enemy[spc].num[num].back_cnt++;					  //回退行为周期计数
				}
																			
				enemy[spc].num[num].x += mx;
				enemy[spc].num[num].y += my;								  //移动

				enemy[spc].num[num].pic_y = enemy[spc].num[num].aspect + 2;   //切换到行走图状态行
				if (enemy[spc].num[num].move == 6) {
					enemy[spc].num[num].pic_x++;							  //按时间调控比例切换行走图
				}

				if (enemy[spc].num[num].not_cnt == enemy[spc].num[num].move_time) {
					enemy[spc].num[num].not_cnt = 0;
					enemy[spc].num[num].move_time = 0;						  //移动行为周期结束
				}
				else enemy[spc].num[num].not_cnt++;							  //移动行为周期计数
			}

			if (enemy[spc].num[num].spin < PI) enemy[spc].num[num].aspect = LEFT;
			if (enemy[spc].num[num].spin > PI) enemy[spc].num[num].aspect = RIGHT;		//判断行走朝向
		}

		//发现玩家																  
		else {																  
			if (enemy[spc].num[num].think_time != 0 && enemy[spc].num[num].is_cnt <= enemy[spc].num[num].think_time) action = 0;     //记忆思考行为
			else if (enemy[spc].num[num].move_time != 0 && enemy[spc].num[num].is_cnt <= enemy[spc].num[num].move_time) action = 1;  //记忆移动行为
			else action = rand() % 2;										  //产生随机行为

			if (enemy[spc].num[num].attack_time != 0) {						  //检查攻击冷却
				if (enemy[spc].num[num].attack_cnt == enemy[spc].num[num].attack_time) {
					enemy[spc].num[num].attack_cnt = 0;
					enemy[spc].num[num].attack_time = 0;					  //攻击冷却周期结束
				}
				else enemy[spc].num[num].attack_cnt++;						  //攻击冷却周期计数
			}

			//约1/2概率进入思考行为
			if (action == 0) {												  
				enemy[spc].num[num].move = 0;								  //初始化移动行为行走图连续切换
				if (enemy[spc].num[num].is_cnt == 0)
					enemy[spc].num[num].think_time = rand() % 21 + 20;		  //随机思考时间20~40 * 30ms = 0.6~1.2s

				enemy[spc].num[num].pic_y = enemy[spc].num[num].aspect;       //切换到站立图状态行
				if (enemy[spc].num[num].think == 6) {
					enemy[spc].num[num].pic_x++;							  //按时间调控比例切换站立图					
				}

				if (enemy[spc].num[num].is_cnt == enemy[spc].num[num].think_time) {
					enemy[spc].num[num].is_cnt = 0;
					enemy[spc].num[num].think_time = 0;						  //思考行为周期结束
				}
				else enemy[spc].num[num].is_cnt++;							  //思考行为周期计数
			}

			//约1/2概率进入移动行为																  
			else {															  
				enemy[spc].num[num].think = 0;								  //初始化思考行为行走图连续切换
				if (enemy[spc].num[num].is_cnt == 0)
					enemy[spc].num[num].move_time = rand() % 111 + 60;		  //随机移动时间60~140 * 30ms = 1.8~4.2s
				move = rand() % 100 + 1;
				for (dir = 0; dir < 3; dir++) {								  //根据记忆路径进行移动
					if (enemy[spc].num[num].direction_time[dir] != 0 && enemy[spc].num[num].direction_cnt <= enemy[spc].num[num].direction_time[dir]) {
						switch (dir) {
						case 0:move = 1; break;
						case 1:move = 51; break;
						case 2:move = 100; break;
						}
						break;
					}
				}
				if (enemy[spc].num[num].back_time != 0 && enemy[spc].num[num].back_cnt <= enemy[spc].num[num].back_time) move = 0;  //记忆为回退时的移动

				//按概率递减的方式从最优前进路径到最差前进路径进行决策															  
				if (move >= 1 && move <= 50) {								  //50%概率向最优方向前进
					mx = (int)(enemy[spc].Espeed * cosarc);
					my = (int)(enemy[spc].Espeed * sinarc);
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[0] = rand() % 21 + 10; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[0]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[0] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				else if (move >= 51 && move <= 80) {						  //30%概率向 (0,PI/4] 偏差范围大幅前进
					if (rand() % 2) marc = (rand() % 5 + 1) * PI / 20;
					else marc = -(rand() % 5 + 1) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[1] = rand() % 21 + 10; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[1]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[1] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				else {													      //20%概率向 (PI/4,PI/2] 偏差范围小幅前进
					if (rand() % 2) marc = (rand() % 5 + 6) * PI / 20;
					else marc = -(rand() % 5 + 6) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[2] = rand() % 21 + 10; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[2]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[2] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}

				//障碍撞击判定
				if (object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
					|| object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
					|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
					|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
					|| move == 0)
				{
					if (enemy[spc].num[num].back_cnt == 0) {
						backx = -mx;
						backy = -my;										  //记录回退方向
						enemy[spc].num[num].direction_cnt = 0;				  //清除单次移动周期计数
						for (dir = 0; dir < 5; dir++)
							enemy[spc].num[num].direction_time[dir] = 0;	  //清除前进方向周期记忆
						enemy[spc].num[num].back_time = rand() % 11 + 20;     //随机回退时间20~30 * 30ms = 0.6~0.9s
					}
					mx = backx;
					my = backy;

					if (object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ)
					{
						mx = 0; 
						my = 0;
						enemy[spc].num[num].back_cnt = 0;
						enemy[spc].num[num].back_time = 0;					  //回退过程中发生碰撞，中止回退
					}

					if (enemy[spc].num[num].back_cnt == enemy[spc].num[num].back_time) {
						enemy[spc].num[num].back_cnt = 0;
						enemy[spc].num[num].back_time = 0;					  //回退行为周期结束
					}
					else enemy[spc].num[num].back_cnt++;					  //回退行为周期计数
				}
				//怪物与玩家判定点接触
				if (((int)(fabs(Ecenter.x + mx - knight.center.x)) <= (60 - enemy[spc].pic_left + knight.center.x - knight.left))
					&& ((int)(fabs(Ecenter.y + my - knight.center.y)) <= (60 - enemy[spc].pic_top + knight.center.y - knight.top)))																		  	
				{
					attack = rand() % 2;

					//1/2概率进行攻击
					if (attack == 0 && enemy[spc].num[num].attack_cnt == 0 && knight.HP >0)   
					{ 
						enemy[spc].num[num].attack_time = rand() % 31 + 40;   //随机攻击冷却时间40~70 * 30ms = 1.2~2.1s														  
						if (knight.AM > 0) {
							if (enemy[spc].damage > knight.AM)knight.AM = 0;
							else knight.AM -= enemy[spc].damage;			  //护甲损失判定
						}
						else if (knight.HP > 0) {
							if (enemy[spc].damage > knight.HP)knight.HP = 0;
							else knight.HP -= enemy[spc].damage;			  //HP损失判定
						}
						player_damaged = 1;									  //玩家被击判定
																			  //玩家被击音效
						mciSendString(_T("close resource\\player_hurt.wav"), NULL, 0, NULL);
						mciSendString(_T("play resource\\player_hurt.wav"), NULL, 0, NULL);
																			
						armor_cnt = 0;										  //护甲回复计时归零
						//角色受到冲击回退
						if (mx > 0 && object[map.x + 2 * speed + knight.right][map.y + knight.top] == FLOOR
							&& object[map.x + 2 * speed + knight.right][map.y + knight.bottom] == FLOOR)
							map.x += 2 * speed;
						else if (mx < 0 && object[map.x - 2 * speed + knight.left][map.y + knight.top] == FLOOR
							&& object[map.x - 2 * speed + knight.left][map.y + knight.bottom] == FLOOR)
							map.x -= 2 * speed;
						if (my > 0 && object[map.x + knight.left][map.y + 2 * speed + knight.bottom] == FLOOR
							&& object[map.x + knight.right][map.y + 2 * speed + knight.bottom] == FLOOR)
							map.y += 2 * speed;
						else if (my < 0 && object[map.x + knight.left][map.y - 2 * speed + knight.top] == FLOOR
							&& object[map.x + knight.right][map.y - 2 * speed + knight.top] == FLOOR)
							map.y -= 2 * speed;
																			  //回退判断退路是否发生碰撞
						mx = 0;
						my = 0;												  //不移动
					}

					//2/3概率回退
					else {													  
						if (enemy[spc].num[num].back_cnt == 0) {
							backx = -mx;
							backy = -my;									  //记录回退方向
							enemy[spc].num[num].direction_cnt = 0;			  //清除单次移动周期计数
							for (dir = 0; dir < 5; dir++)
								enemy[spc].num[num].direction_time[dir] = 0;  //清除前进方向周期记忆
							enemy[spc].num[num].back_time = rand() % 21 + 20; //随机回退时间20~40 * 30ms = 0.6~1.2s
							enemy[spc].num[num].is_cnt = 0;
							enemy[spc].num[num].think_time = 0;
							enemy[spc].num[num].move_time = 0;				  //清除动作记忆
						}
						mx = backx;
						my = backy;

						if (object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
							|| object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
							|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
							|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ)
						{
							mx = 0;
							my = 0;
							enemy[spc].num[num].back_cnt = 0;
							enemy[spc].num[num].back_time = 0;				  //回退过程中发生碰撞，中止回退
						}

						if (enemy[spc].num[num].back_cnt == enemy[spc].num[num].back_time) {
							enemy[spc].num[num].back_cnt = 0;
							enemy[spc].num[num].back_time = 0;				  //回退行为周期结束
						}
						else enemy[spc].num[num].back_cnt++;				  //回退行为周期计数
					}
				}					

				enemy[spc].num[num].x += mx;
				enemy[spc].num[num].y += my;								  //移动										

				enemy[spc].num[num].pic_y = enemy[spc].num[num].aspect + 2;
				if (enemy[spc].num[num].move == 6) {
					enemy[spc].num[num].pic_x++;							  //按时间调控比例切换行走图
				}
				if (enemy[spc].num[num].is_cnt == enemy[spc].num[num].move_time) {
					enemy[spc].num[num].is_cnt = 0;
					enemy[spc].num[num].move_time = 0;						  //移动行为周期结束
				}
				else enemy[spc].num[num].is_cnt++;							  //移动行为周期计数
			}

			if (enemy[spc].num[num].spin < PI) enemy[spc].num[num].aspect = LEFT;
			if (enemy[spc].num[num].spin > PI) enemy[spc].num[num].aspect = RIGHT;		//判断行走朝向
		}
	}

	else if (enemy[spc].num[num].HP <= 0) {
		enemy[spc].num[num].pic_y = 4;
		enemy[spc].num[num].pic_x = enemy[spc].num[num].aspect;
	}

	if (enemy[spc].num[num].think == 6) enemy[spc].num[num].think = 0;
	else enemy[spc].num[num].think++;										  //思考时站立图改变与地图变换时间比例1:6
	if (enemy[spc].num[num].move == 6) enemy[spc].num[num].move = 0;
	else enemy[spc].num[num].move++;										  //移动时行走图改变与地图变换时间比例1:6
}

void shoot_AI(int spc, int num) 
{
	//以怪物为中心建立坐标系，向右为x轴正方向、向下为y轴正方向（即默认图形坐标系）

	int dir;							//方向循环变量
	int dx;								//怪物到玩家判定中心横坐标增量
	int dy;								//怪物到玩家判定中心纵坐标增量
	double distance;					//怪物与玩家之间的距离
	double cosarc;						//怪物到玩家夹角余弦值
	double sinarc;						//怪物到玩家夹角正弦值
	double arc;							//怪物到玩家夹角，范围 [0, 2PI)
	double marc;						//夹角偏差值
	int action;							//行为判断参量
	int	move;							//移动判断参量
	int attack;							//攻击判断参量
	int	mx;								//横向移动距离
	int my;								//纵向移动距离
	static int backx;					//横向回退距离记录
	static int backy;					//纵向回退距离记录
	int Eleft, Eright, Etop, Ebottom;   //怪物当前在窗口中判定点四方向坐标
	POINT Ecenter;                      //怪物当前在窗口中中心点坐标

	Eleft = enemy[spc].num[num].x - map.x + enemy[spc].pic_left;
	Eright = enemy[spc].num[num].x - map.x + enemy[spc].pic_right;
	Etop = enemy[spc].num[num].y - map.y + enemy[spc].pic_top;
	Ebottom = enemy[spc].num[num].y - map.y + enemy[spc].pic_bottom;
	Ecenter.x = (Eleft + Eright) / 2;
	Ecenter.y = (Etop + Ebottom) / 2;

	dx = character_position.x - (enemy[spc].num[num].x - map.x);
	dy = character_position.y - (enemy[spc].num[num].y - map.y);
	distance = sqrt(dx * dx + dy * dy);

	if (distance > 0 && enemy[spc].num[num].HP > 0) {
		cosarc = dx / distance;
		sinarc = dy / distance;
		if (dy >= 0) arc = acos(cosarc);
		else arc = 2 * PI - acos(cosarc);

		if (dy >= 0) enemy[spc].num[num].spin = 2 * PI - acos(cosarc) - PI / 2;
		else enemy[spc].num[num].spin = acos(cosarc) - PI / 2;
		if (enemy[spc].num[num].spin < 0) enemy[spc].num[num].spin += 2 * PI;

		if (enemy[spc].num[num].pic_x == 3)
			enemy[spc].num[num].pic_x = 0;									  //行为图轮换

		//未发现玩家																  
		if (!isdetected(spc, num)) {
			enemy[spc].num[num].is_cnt = 0;
			if (enemy[spc].num[num].think_time != 0 && enemy[spc].num[num].not_cnt <= enemy[spc].num[num].think_time) action = 0;     //记忆思考行为
			else if (enemy[spc].num[num].move_time != 0 && enemy[spc].num[num].not_cnt <= enemy[spc].num[num].move_time) action = 1;  //记忆移动行为
			else action = rand() % 2;										  //产生随机行为

			//约1/2概率进入思考行为
			if (action == 0) {
				enemy[spc].num[num].move = 0;								  //初始化移动行为行走图连续切换
				if (enemy[spc].num[num].not_cnt == 0)
					enemy[spc].num[num].think_time = rand() % 31 + 30;		  //随机思考时间30~60 * 30ms = 0.9~1.8s

				enemy[spc].num[num].pic_y = enemy[spc].num[num].aspect;       //切换到站立图状态行
				if (enemy[spc].num[num].think == 6) {
					enemy[spc].num[num].pic_x++;							  //按时间调控比例切换站立图					
				}

				if (enemy[spc].num[num].not_cnt == enemy[spc].num[num].think_time) {
					enemy[spc].num[num].not_cnt = 0;
					enemy[spc].num[num].think_time = 0;						  //思考行为周期结束
				}
				else enemy[spc].num[num].not_cnt++;							  //思考行为周期计数
			}

			//约1/2概率进入移动行为
			else {
				enemy[spc].num[num].think = 0;								  //初始化思考行为行走图连续切换
				if (enemy[spc].num[num].not_cnt == 0)
					enemy[spc].num[num].move_time = rand() % 81 + 30;		  //随机移动时间30~100 * 30ms = 0.9~3.0s
				move = rand() % 100 + 1;
				for (dir = 0; dir < 5; dir++) {								  //根据记忆路径进行移动
					if (enemy[spc].num[num].direction_time[dir] != 0 && enemy[spc].num[num].direction_cnt <= enemy[spc].num[num].direction_time[dir]) {
						switch (dir) {
						case 0:move = 1; break;
						case 1:move = 21; break;
						case 2:move = 41; break;
						case 3:move = 61; break;
						case 4:move = 100; break;
						}
						break;
					}
				}
				if (enemy[spc].num[num].back_time != 0 && enemy[spc].num[num].back_cnt <= enemy[spc].num[num].back_time) move = 0;  //记忆为回退时的移动

																																	//按概率递减的方式从最优路径到最差路径进行决策															  
				if (move >= 1 && move <= 20) {								  //20%概率向最优方向前进
					mx = (int)(enemy[spc].Espeed * cosarc);
					my = (int)(enemy[spc].Espeed * sinarc);
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[0] = rand() % 21 + 10; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[0]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[0] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				else if (move >= 21 && move <= 40) {						  //20%概率向 (0,PI/4] 偏差范围大幅前进
					if (rand() % 2) marc = (rand() % 5 + 1) * PI / 20;
					else marc = -(rand() % 5 + 1) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[1] = rand() % 21 + 10; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[1]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[1] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				else if (move >= 41 && move <= 60) {						  //20%概率向 (PI/4,PI/2] 偏差范围小幅前进
					if (rand() % 2) marc = (rand() % 5 + 6) * PI / 20;
					else marc = -(rand() % 5 + 6) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[2] = rand() % 21 + 10; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[2]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[2] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				else if (move >= 61 && move <= 80) {						  //20%概率向 (PI/2,3PI/4] 偏差范围小幅后退
					if (rand() % 2) marc = (rand() % 5 + 11) * PI / 20;
					else marc = -(rand() % 5 + 11) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[3] = rand() % 11 + 20; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[3]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[3] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				else {														  //20%概率向 (3PI/4,PI] 偏差范围大幅后退
					if (rand() % 2) marc = (rand() % 5 + 16) * PI / 20;
					else marc = -(rand() % 5 + 16) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[4] = rand() % 11 + 20; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[4]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[4] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				//障碍撞击判定
				if (object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
					|| object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
					|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
					|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
					|| move == 0)
				{
					if (enemy[spc].num[num].back_cnt == 0) {
						backx = -mx;
						backy = -my;										  //记录回退方向
						enemy[spc].num[num].direction_cnt = 0;				  //清除单次移动周期计数
						for (dir = 0; dir < 5; dir++)
							enemy[spc].num[num].direction_time[dir] = 0;	  //清除前进方向周期记忆
						enemy[spc].num[num].back_time = rand() % 11 + 20;     //随机回退时间20~30 * 30ms = 0.6~0.9s
					}
					mx = backx;
					my = backy;

					if (object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ)
					{
						mx = 0;
						my = 0;
						enemy[spc].num[num].back_cnt = 0;
						enemy[spc].num[num].back_time = 0;					  //回退过程中发生碰撞，中止回退
					}

					if (enemy[spc].num[num].back_cnt == enemy[spc].num[num].back_time) {
						enemy[spc].num[num].back_cnt = 0;
						enemy[spc].num[num].back_time = 0;					  //回退行为周期结束
					}
					else enemy[spc].num[num].back_cnt++;					  //回退行为周期计数
				}

				enemy[spc].num[num].x += mx;
				enemy[spc].num[num].y += my;								  //移动

				enemy[spc].num[num].pic_y = enemy[spc].num[num].aspect + 2;   //切换到行走图状态行
				if (enemy[spc].num[num].move == 6) {
					enemy[spc].num[num].pic_x++;							  //按时间调控比例切换行走图
				}

				if (enemy[spc].num[num].not_cnt == enemy[spc].num[num].move_time) {
					enemy[spc].num[num].not_cnt = 0;
					enemy[spc].num[num].move_time = 0;						  //移动行为周期结束
				}
				else enemy[spc].num[num].not_cnt++;							  //移动行为周期计数
			}

			if (enemy[spc].num[num].spin < PI) enemy[spc].num[num].aspect = LEFT;
			if (enemy[spc].num[num].spin > PI) enemy[spc].num[num].aspect = RIGHT;		//判断行走朝向
		}

		//发现玩家																  
		else {
			enemy[spc].num[num].not_cnt = 0;
			if (enemy[spc].num[num].think_time != 0 && enemy[spc].num[num].is_cnt <= enemy[spc].num[num].think_time) action = 0;     //记忆思考行为
			else if (enemy[spc].num[num].move_time != 0 && enemy[spc].num[num].is_cnt <= enemy[spc].num[num].move_time) action = 1;  //记忆移动行为
			else action = rand() % 2;										  //产生随机行为

			if (enemy[spc].num[num].attack_time != 0) {						  //检查攻击冷却
				if (enemy[spc].num[num].attack_cnt == enemy[spc].num[num].attack_time) {
					enemy[spc].num[num].attack_cnt = 0;
					enemy[spc].num[num].attack_time = 0;					  //攻击冷却周期结束
				}
				else enemy[spc].num[num].attack_cnt++;						  //攻击冷却周期计数
			}

			//约1/2概率进入思考行为
			if (action == 0) {
				enemy[spc].num[num].move = 0;								  //初始化移动行为行走图连续切换
				if (enemy[spc].num[num].is_cnt == 0)
					enemy[spc].num[num].think_time = rand() % 21 + 20;		  //随机思考时间20~40 * 30ms = 0.6~1.2s

				enemy[spc].num[num].pic_y = enemy[spc].num[num].aspect;       //切换到站立图状态行
				if (enemy[spc].num[num].think == 6) {
					enemy[spc].num[num].pic_x++;							  //按时间调控比例切换站立图					
				}

				if (enemy[spc].num[num].is_cnt == enemy[spc].num[num].think_time) {
					enemy[spc].num[num].is_cnt = 0;
					enemy[spc].num[num].think_time = 0;						  //思考行为周期结束
				}
				else enemy[spc].num[num].is_cnt++;							  //思考行为周期计数
			}

			//约1/2概率进入移动行为																  
			else {
				enemy[spc].num[num].think = 0;								  //初始化思考行为行走图连续切换
				if (enemy[spc].num[num].is_cnt == 0)
					enemy[spc].num[num].move_time = rand() % 111 + 60;		  //随机移动时间60~140 * 30ms = 1.8~4.2s
				if (distance > 250) move = rand() % 100 + 1;				  //距离充足时，随机移动行为
				else move = rand() % 50 + 51;								  //保持距离，采取后退行为
				for (dir = 0; dir < 5; dir++) {								  //根据记忆路径进行移动
					if (enemy[spc].num[num].direction_time[dir] != 0 && enemy[spc].num[num].direction_cnt <= enemy[spc].num[num].direction_time[dir]) {
						switch (dir) {
						case 0:move = 1; break;
						case 1:move = 21; break;
						case 2:move = 36; break;
						case 3:move = 51; break;
						case 4:move = 100; break;
						}
						break;
					}
				}
				if (enemy[spc].num[num].back_time != 0 && enemy[spc].num[num].back_cnt <= enemy[spc].num[num].back_time) move = 0;  //记忆为回退时的移动

				//按概率递减的方式从最优前进路径到最差前进路径进行决策															  
				if (move >= 1 && move <= 20) {								  //20%概率向最优方向前进
					mx = (int)(enemy[spc].Espeed * cosarc);
					my = (int)(enemy[spc].Espeed * sinarc);

					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[0] = rand() % 21 + 10; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[0]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[0] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				else if (move >= 21 && move <= 35) {						  //15%概率向 (0,PI/4] 偏差范围大幅前进
					if (rand() % 2) marc = (rand() % 5 + 1) * PI / 20;
					else marc = -(rand() % 5 + 1) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));

					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[1] = rand() % 21 + 10; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[1]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[1] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				else if (move >= 36 && move <= 50) {						 //15%概率向 (PI/4,PI/2] 偏差范围小幅前进
					if (rand() % 2) marc = (rand() % 5 + 6) * PI / 20;
					else marc = -(rand() % 5 + 6) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));

					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[2] = rand() % 21 + 10; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[2]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[2] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				else if (move >= 51 && move <= 70) {						  //20%概率向 (PI/2,3PI/4] 偏差范围小幅后退
					if (rand() % 2) marc = (rand() % 5 + 11) * PI / 20;
					else marc = -(rand() % 5 + 11) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[3] = rand() % 11 + 20; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[3]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[3] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				else {														  //30%概率向 (3PI/4,PI] 偏差范围大幅后退
					if (rand() % 2) marc = (rand() % 5 + 16) * PI / 20;
					else marc = -(rand() % 5 + 16) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[4] = rand() % 11 + 20; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[4]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[4] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}

				//障碍撞击判定
				if (object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
					|| object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
					|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
					|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
					|| move == 0)
				{
					if (enemy[spc].num[num].back_cnt == 0) {
						backx = -mx;
						backy = -my;										  //记录回退方向
						enemy[spc].num[num].direction_cnt = 0;				  //清除单次移动周期计数
						for (dir = 0; dir < 5; dir++)
							enemy[spc].num[num].direction_time[dir] = 0;	  //清除前进方向周期记忆
						enemy[spc].num[num].back_time = rand() % 11 + 20;     //随机回退时间20~30 * 30ms = 0.6~0.9s
					}
					mx = backx;
					my = backy;

					if (object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ)
					{
						mx = 0;
						my = 0;
						enemy[spc].num[num].back_cnt = 0;
						enemy[spc].num[num].back_time = 0;					  //回退过程中发生碰撞，中止回退
					}

					if (enemy[spc].num[num].back_cnt == enemy[spc].num[num].back_time) {
						enemy[spc].num[num].back_cnt = 0;
						enemy[spc].num[num].back_time = 0;					  //回退行为周期结束
					}
					else enemy[spc].num[num].back_cnt++;					  //回退行为周期计数
				}

				//怪物与玩家判定点接触
				if (((int)(fabs(Ecenter.x + mx - knight.center.x)) <= (60 - enemy[spc].pic_left + knight.center.x - knight.left))
					&& ((int)(fabs(Ecenter.y + my - knight.center.y)) <= (60 - enemy[spc].pic_top + knight.center.y - knight.top)))
				{
					if (enemy[spc].num[num].back_cnt == 0) {
						backx = -mx;
						backy = -my;									  //记录回退方向
						enemy[spc].num[num].direction_cnt = 0;			  //清除单次移动周期计数
						for (dir = 0; dir < 5; dir++)
							enemy[spc].num[num].direction_time[dir] = 0;  //清除前进方向周期记忆
						enemy[spc].num[num].back_time = rand() % 21 + 20; //随机回退时间20~40 * 30ms = 0.6~1.2s
						enemy[spc].num[num].is_cnt = 0;
						enemy[spc].num[num].think_time = 0;
						enemy[spc].num[num].move_time = 0;				  //清除动作记忆
					}
					mx = backx;
					my = backy;

					if (object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ)
					{
						mx = 0;
						my = 0;
						enemy[spc].num[num].back_cnt = 0;
						enemy[spc].num[num].back_time = 0;				  //回退过程中发生碰撞，中止回退
					}

					if (enemy[spc].num[num].back_cnt == enemy[spc].num[num].back_time) {
						enemy[spc].num[num].back_cnt = 0;
						enemy[spc].num[num].back_time = 0;				  //回退行为周期结束
					}
					else enemy[spc].num[num].back_cnt++;				  //回退行为周期计数				
				}

				attack = rand() % 4;

				//1/4概率进行攻击
				if (attack == 0 && enemy[spc].num[num].attack_cnt == 0 && knight.HP >0)
				{
					enemy[spc].num[num].attack_time = rand() % 31 + 40;   //随机攻击冷却时间40~70 * 30ms = 1.2~2.1s				

					enemy_shoot(spc, num);
				}

				enemy[spc].num[num].x += mx;
				enemy[spc].num[num].y += my;								  //移动

				enemy[spc].num[num].pic_y = enemy[spc].num[num].aspect + 2;
				if (enemy[spc].num[num].move == 6) {
					enemy[spc].num[num].pic_x++;							  //按时间调控比例切换行走图
				}
				if (enemy[spc].num[num].is_cnt == enemy[spc].num[num].move_time) {
					enemy[spc].num[num].is_cnt = 0;
					enemy[spc].num[num].move_time = 0;						  //移动行为周期结束
				}
				else enemy[spc].num[num].is_cnt++;							  //移动行为周期计数
			}

			if (enemy[spc].num[num].spin < PI) enemy[spc].num[num].aspect = LEFT;
			if (enemy[spc].num[num].spin > PI) enemy[spc].num[num].aspect = RIGHT;		//判断行走朝向
		}
	}

	else if (enemy[spc].num[num].HP <= 0) {
		enemy[spc].num[num].pic_y = 4;
		enemy[spc].num[num].pic_x = enemy[spc].num[num].aspect;
	}

	if (enemy[spc].num[num].think == 6) enemy[spc].num[num].think = 0;
	else enemy[spc].num[num].think++;										  //思考时站立图改变与地图变换时间比例1:6
	if (enemy[spc].num[num].move == 6) enemy[spc].num[num].move = 0;
	else enemy[spc].num[num].move++;										  //移动时行走图改变与地图变换时间比例1:6
}

void BOSS_AI(int spc, int num)
{
	//以怪物为中心建立坐标系，向右为x轴正方向、向下为y轴正方向（即默认图形坐标系）

	int dir;							//方向循环变量
	int dx;								//怪物到玩家判定中心横坐标增量
	int dy;								//怪物到玩家判定中心纵坐标增量
	double distance;					//怪物与玩家之间的距离
	double cosarc;						//怪物到玩家夹角余弦值
	double sinarc;						//怪物到玩家夹角正弦值
	double arc;							//怪物到玩家夹角，范围 [0, 2PI)
	double marc;						//夹角偏差值
	double garc;						//攻击夹角
	int action;							//行为判断参量
	int	move;							//移动判断参量
	int attack;							//攻击判断参量
	int	mx;								//横向移动距离
	int my;								//纵向移动距离
	static int backx;					//横向回退距离记录
	static int backy;					//纵向回退距离记录
	int Eleft, Eright, Etop, Ebottom;   //怪物当前在窗口中判定点四方向坐标
	POINT Ecenter;                      //怪物当前在窗口中中心点坐标
	static int angry;

	Eleft = enemy[spc].num[num].x - map.x + enemy[spc].pic_left;
	Eright = enemy[spc].num[num].x - map.x + enemy[spc].pic_right;
	Etop = enemy[spc].num[num].y - map.y + enemy[spc].pic_top;
	Ebottom = enemy[spc].num[num].y - map.y + enemy[spc].pic_bottom;
	Ecenter.x = (Eleft + Eright) / 2;
	Ecenter.y = (Etop + Ebottom) / 2;

	dx = character_position.x - (enemy[spc].num[num].x - map.x);
	dy = character_position.y - (enemy[spc].num[num].y - map.y);
	distance = sqrt(dx * dx + dy * dy);

	if (enemy[spc].num[num].HP >= enemy[spc].HP_MAX / 2) {
		enemy[spc].num[num].pic_y = 0;
		angry = 0;
	}		
	else if (enemy[spc].num[num].HP > 0) {
		enemy[spc].num[num].pic_y = 1;
		if (angry == 0) {
			mciSendString(_T("close resource\\boss_angry.wav"), NULL, 0, NULL);
			mciSendString(_T("play resource\\boss_angry.wav"), NULL, 0, NULL);
			angry = 1;
		}
	}	
	else {
		enemy[spc].num[num].pic_y = 2;
		enemy[spc].num[num].pic_x = enemy[spc].num[num].aspect;
	}

	if (distance > 0 && enemy[spc].num[num].HP > 0) {
		cosarc = dx / distance;
		sinarc = dy / distance;
		if (dy >= 0) arc = acos(cosarc);
		else arc = 2 * PI - acos(cosarc);

		if (dy >= 0) enemy[spc].num[num].spin = 2 * PI - acos(cosarc) - PI / 2;
		else enemy[spc].num[num].spin = acos(cosarc) - PI / 2;
		if (enemy[spc].num[num].spin < 0) enemy[spc].num[num].spin += 2 * PI;
																			  //未发现玩家																  
		if (!isdetected(spc, num)) {
			enemy[spc].num[num].is_cnt = 0;
			if (enemy[spc].num[num].think_time != 0 && enemy[spc].num[num].not_cnt <= enemy[spc].num[num].think_time) action = 0;     //记忆思考行为
			else if (enemy[spc].num[num].move_time != 0 && enemy[spc].num[num].not_cnt <= enemy[spc].num[num].move_time) action = 1;  //记忆移动行为
			else action = rand() % 3;										  //产生随机行为

			//约1/3概率进入思考行为
			if (action == 0) {
				enemy[spc].num[num].move = 0;								  //初始化移动行为行走图连续切换
				if (enemy[spc].num[num].not_cnt == 0)
					enemy[spc].num[num].think_time = rand() % 11 + 30;		  //随机思考时间30~40 * 30ms = 0.9~1.2s

				enemy[spc].num[num].pic_x = enemy[spc].num[num].aspect;		  				

				if (enemy[spc].num[num].not_cnt == enemy[spc].num[num].think_time) {
					enemy[spc].num[num].not_cnt = 0;
					enemy[spc].num[num].think_time = 0;						  //思考行为周期结束
				}
				else enemy[spc].num[num].not_cnt++;							  //思考行为周期计数
			}

			//约1/2概率进入移动行为
			else {
				enemy[spc].num[num].think = 0;								  //初始化思考行为行走图连续切换
				if (enemy[spc].num[num].not_cnt == 0)
					enemy[spc].num[num].move_time = rand() % 81 + 30;		  //随机移动时间30~100 * 30ms = 0.9~3.0s
				move = rand() % 100 + 1;
				for (dir = 0; dir < 5; dir++) {								  //根据记忆路径进行移动
					if (enemy[spc].num[num].direction_time[dir] != 0 && enemy[spc].num[num].direction_cnt <= enemy[spc].num[num].direction_time[dir]) {
						switch (dir) {
						case 0:move = 1; break;
						case 1:move = 21; break;
						case 2:move = 41; break;
						case 3:move = 61; break;
						case 4:move = 100; break;
						}
						break;
					}
				}
				if (enemy[spc].num[num].back_time != 0 && enemy[spc].num[num].back_cnt <= enemy[spc].num[num].back_time) move = 0;  //记忆为回退时的移动

																																	//按概率递减的方式从最优路径到最差路径进行决策															  
				if (move >= 1 && move <= 20) {								  //20%概率向最优方向前进
					mx = (int)(enemy[spc].Espeed * cosarc);
					my = (int)(enemy[spc].Espeed * sinarc);
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[0] = rand() % 21 + 10; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[0]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[0] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				else if (move >= 21 && move <= 40) {						  //20%概率向 (0,PI/4] 偏差范围大幅前进
					if (rand() % 2) marc = (rand() % 5 + 1) * PI / 20;
					else marc = -(rand() % 5 + 1) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[1] = rand() % 21 + 10; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[1]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[1] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				else if (move >= 41 && move <= 60) {						  //20%概率向 (PI/4,PI/2] 偏差范围小幅前进
					if (rand() % 2) marc = (rand() % 5 + 6) * PI / 20;
					else marc = -(rand() % 5 + 6) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[2] = rand() % 21 + 10; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[2]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[2] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				else if (move >= 61 && move <= 80) {						  //20%概率向 (PI/2,3PI/4] 偏差范围小幅后退
					if (rand() % 2) marc = (rand() % 5 + 11) * PI / 20;
					else marc = -(rand() % 5 + 11) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[3] = rand() % 11 + 20; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[3]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[3] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				else {														  //20%概率向 (3PI/4,PI] 偏差范围大幅后退
					if (rand() % 2) marc = (rand() % 5 + 16) * PI / 20;
					else marc = -(rand() % 5 + 16) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[4] = rand() % 11 + 20; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[4]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[4] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				//障碍撞击判定
				if (object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
					|| object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
					|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
					|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
					|| move == 0)
				{
					if (enemy[spc].num[num].back_cnt == 0) {
						backx = -mx;
						backy = -my;										  //记录回退方向
						enemy[spc].num[num].direction_cnt = 0;				  //清除单次移动周期计数
						for (dir = 0; dir < 5; dir++)
							enemy[spc].num[num].direction_time[dir] = 0;	  //清除前进方向周期记忆
						enemy[spc].num[num].back_time = rand() % 11 + 20;     //随机回退时间20~30 * 30ms = 0.6~0.9s
					}
					mx = backx;
					my = backy;

					if (object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ)
					{
						mx = 0;
						my = 0;
						enemy[spc].num[num].back_cnt = 0;
						enemy[spc].num[num].back_time = 0;					  //回退过程中发生碰撞，中止回退
					}

					if (enemy[spc].num[num].back_cnt == enemy[spc].num[num].back_time) {
						enemy[spc].num[num].back_cnt = 0;
						enemy[spc].num[num].back_time = 0;					  //回退行为周期结束
					}
					else enemy[spc].num[num].back_cnt++;					  //回退行为周期计数
				}

				enemy[spc].num[num].x += mx;
				enemy[spc].num[num].y += my;								  //移动

				enemy[spc].num[num].pic_x = enemy[spc].num[num].aspect + 2;   

				if (enemy[spc].num[num].not_cnt == enemy[spc].num[num].move_time) {
					enemy[spc].num[num].not_cnt = 0;
					enemy[spc].num[num].move_time = 0;						  //移动行为周期结束
				}
				else enemy[spc].num[num].not_cnt++;							  //移动行为周期计数
			}

			if (enemy[spc].num[num].spin < PI) enemy[spc].num[num].aspect = LEFT;
			if (enemy[spc].num[num].spin > PI) enemy[spc].num[num].aspect = RIGHT;		//判断行走朝向
		}

		//发现玩家																  
		else {
			enemy[spc].num[num].not_cnt = 0;
			if (enemy[spc].num[num].think_time != 0 && enemy[spc].num[num].is_cnt <= enemy[spc].num[num].think_time) action = 0;     //记忆思考行为
			else if (enemy[spc].num[num].move_time != 0 && enemy[spc].num[num].is_cnt <= enemy[spc].num[num].move_time) action = 1;  //记忆移动行为
			else action = rand() % 2;										  //产生随机行为

			if (enemy[spc].num[num].attack_time != 0 && enemy[spc].num[num].attack_cnt <= enemy[spc].num[num].attack_time) attack = 0;     //记忆攻击行为
			else attack = rand() % 4;										  //1/4概率进行攻击

			if (enemy[spc].num[num].attack_time != 0) {						  //检查攻击冷却
				if (enemy[spc].num[num].attack_cnt == enemy[spc].num[num].attack_time) {
					enemy[spc].num[num].attack_cnt = 0;
					enemy[spc].num[num].attack_time = 0;					  //攻击冷却周期结束
				}
				else enemy[spc].num[num].attack_cnt++;						  //攻击冷却周期计数
			}

			//约1/2概率进入思考行为
			if (action == 0) {
				enemy[spc].num[num].move = 0;								  //初始化移动行为行走图连续切换
				if (enemy[spc].num[num].is_cnt == 0)
					enemy[spc].num[num].think_time = rand() % 21 + 20;		  //随机思考时间20~40 * 30ms = 0.6~1.2s

				enemy[spc].num[num].pic_x = enemy[spc].num[num].aspect;

				if (enemy[spc].num[num].is_cnt == enemy[spc].num[num].think_time) {
					enemy[spc].num[num].is_cnt = 0;
					enemy[spc].num[num].think_time = 0;						  //思考行为周期结束
				}
				else enemy[spc].num[num].is_cnt++;							  //思考行为周期计数
			}

			//约1/2概率进入移动行为																  
			else {
				enemy[spc].num[num].think = 0;								  //初始化思考行为行走图连续切换
				if (enemy[spc].num[num].is_cnt == 0)
					enemy[spc].num[num].move_time = rand() % 111 + 60;		  //随机移动时间60~140 * 30ms = 1.8~4.2s
				if (distance > 300) move = rand() % 100 + 1;				  //距离充足时，随机移动行为
				else move = rand() % 50 + 51;								  //保持距离，采取后退行为
				for (dir = 0; dir < 5; dir++) {								  //根据记忆路径进行移动
					if (enemy[spc].num[num].direction_time[dir] != 0 && enemy[spc].num[num].direction_cnt <= enemy[spc].num[num].direction_time[dir]) {
						switch (dir) {
						case 0:move = 1; break;
						case 1:move = 16; break;
						case 2:move = 36; break;
						case 3:move = 61; break;
						case 4:move = 100; break;
						}
						break;
					}
				}
				if (enemy[spc].num[num].back_time != 0 && enemy[spc].num[num].back_cnt <= enemy[spc].num[num].back_time) move = 0;  //记忆为回退时的移动

																																	//按概率递减的方式从最优前进路径到最差前进路径进行决策															  
				if (move >= 1 && move <= 15) {								  //15%概率向最优方向前进
					mx = (int)(enemy[spc].Espeed * cosarc);
					my = (int)(enemy[spc].Espeed * sinarc);

					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[0] = rand() % 21 + 10; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[0]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[0] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				else if (move >= 16 && move <= 35) {						  //20%概率向 (0,PI/4] 偏差范围大幅前进
					if (rand() % 2) marc = (rand() % 5 + 1) * PI / 20;
					else marc = -(rand() % 5 + 1) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));

					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[1] = rand() % 21 + 10; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[1]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[1] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				else if (move >= 36 && move <= 60) {						 //25%概率向 (PI/4,PI/2] 偏差范围小幅前进
					if (rand() % 2) marc = (rand() % 5 + 6) * PI / 20;
					else marc = -(rand() % 5 + 6) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));

					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[2] = rand() % 21 + 10; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[2]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[2] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				else if (move >= 61 && move <= 80) {						  //20%概率向 (PI/2,3PI/4] 偏差范围小幅后退
					if (rand() % 2) marc = (rand() % 5 + 11) * PI / 20;
					else marc = -(rand() % 5 + 11) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[3] = rand() % 11 + 20; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[3]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[3] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}
				else {														  //20%概率向 (3PI/4,PI] 偏差范围大幅后退
					if (rand() % 2) marc = (rand() % 5 + 16) * PI / 20;
					else marc = -(rand() % 5 + 16) * PI / 20;

					mx = (int)(enemy[spc].Espeed * cos(arc + marc));
					my = (int)(enemy[spc].Espeed * sin(arc + marc));
					if (enemy[spc].num[num].direction_cnt == 0)
						enemy[spc].num[num].direction_time[4] = rand() % 11 + 20; //随机单次移动时间20~30 * 30ms = 0.6~0.9s
					if (enemy[spc].num[num].direction_cnt == enemy[spc].num[num].direction_time[4]) {
						enemy[spc].num[num].direction_cnt = 0;
						enemy[spc].num[num].direction_time[4] = 0;			  //单次移动结束
					}
					else enemy[spc].num[num].direction_cnt++;				  //单次移动周期计数
				}

				//障碍撞击判定
				if (object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
					|| object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
					|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
					|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
					|| move == 0)
				{
					if (enemy[spc].num[num].back_cnt == 0) {
						backx = -mx;
						backy = -my;										  //记录回退方向
						enemy[spc].num[num].direction_cnt = 0;				  //清除单次移动周期计数
						for (dir = 0; dir < 5; dir++)
							enemy[spc].num[num].direction_time[dir] = 0;	  //清除前进方向周期记忆
						enemy[spc].num[num].back_time = rand() % 11 + 20;     //随机回退时间20~30 * 30ms = 0.6~0.9s
					}
					mx = backx;
					my = backy;

					if (object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ)
					{
						mx = 0;
						my = 0;
						enemy[spc].num[num].back_cnt = 0;
						enemy[spc].num[num].back_time = 0;					  //回退过程中发生碰撞，中止回退
					}

					if (enemy[spc].num[num].back_cnt == enemy[spc].num[num].back_time) {
						enemy[spc].num[num].back_cnt = 0;
						enemy[spc].num[num].back_time = 0;					  //回退行为周期结束
					}
					else enemy[spc].num[num].back_cnt++;					  //回退行为周期计数
				}

				//怪物与玩家判定点接触
				if (((int)(fabs(Ecenter.x + mx - knight.center.x)) <= (60 - enemy[spc].pic_left + knight.center.x - knight.left))
					&& ((int)(fabs(Ecenter.y + my - knight.center.y)) <= (60 - enemy[spc].pic_top + knight.center.y - knight.top)))
				{
					if (enemy[spc].num[num].back_cnt == 0) {
						backx = -mx;
						backy = -my;									  //记录回退方向
						enemy[spc].num[num].direction_cnt = 0;			  //清除单次移动周期计数
						for (dir = 0; dir < 5; dir++)
							enemy[spc].num[num].direction_time[dir] = 0;  //清除前进方向周期记忆
						enemy[spc].num[num].back_time = rand() % 21 + 20; //随机回退时间20~40 * 30ms = 0.6~1.2s
						enemy[spc].num[num].is_cnt = 0;
						enemy[spc].num[num].think_time = 0;
						enemy[spc].num[num].move_time = 0;				  //清除动作记忆
					}
					mx = backx;
					my = backy;

					if (object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == IMMORTAL_OBJ
						|| object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == IMMORTAL_OBJ)
					{
						mx = 0;
						my = 0;
						enemy[spc].num[num].back_cnt = 0;
						enemy[spc].num[num].back_time = 0;				  //回退过程中发生碰撞，中止回退
					}

					if (enemy[spc].num[num].back_cnt == enemy[spc].num[num].back_time) {
						enemy[spc].num[num].back_cnt = 0;
						enemy[spc].num[num].back_time = 0;				  //回退行为周期结束
					}
					else enemy[spc].num[num].back_cnt++;				  //回退行为周期计数				
				}

				if (attack == 0 && knight.HP >0)
				{
					if (enemy[spc].num[num].attack_cnt == 0)
						enemy[spc].num[num].attack_time = rand() % 51 + 50;		  //随机攻击时间50~100 * 30ms = 1.5~3s

					if (enemy[spc].num[num].attack_cnt % 40 == 0)				 //射速1.2s/发
					{
						if (enemy[spc].num[num].HP >= enemy[spc].HP_MAX / 2)
						{
							marc = (rand() % 7 - 3) * PI / 18;
							enemy[spc].num[num].spin += marc;
							enemy_shoot(spc, num);
							enemy[spc].num[num].spin += PI / 12;
							enemy_shoot(spc, num);
							enemy[spc].num[num].spin += PI / 12;
							enemy_shoot(spc, num);
							enemy[spc].num[num].spin -= PI / 4;
							enemy_shoot(spc, num);
							enemy[spc].num[num].spin -= PI / 12;
							enemy_shoot(spc, num);
							enemy[spc].num[num].spin += PI / 6;
							enemy[spc].num[num].spin -= marc;
						}
						else {
							marc = (rand() % 7 - 3) * PI / 18;
							for (garc = marc; garc < 2 * PI + marc; garc += PI / 12) {
								enemy[spc].num[num].spin += garc;
								enemy_shoot(spc, num);
							}				
							enemy[spc].num[num].spin -= (23 * PI / 12 + marc);
						}
					}

					if (enemy[spc].num[num].attack_cnt == enemy[spc].num[num].attack_time) {
						enemy[spc].num[num].attack_cnt = 0;
						enemy[spc].num[num].attack_time = 0;					  //攻击行为周期结束
					}
					else enemy[spc].num[num].attack_cnt++;						  //攻击行为周期计数									
				}

				enemy[spc].num[num].x += mx;
				enemy[spc].num[num].y += my;								  //移动

				enemy[spc].num[num].pic_x = enemy[spc].num[num].aspect + 2;
				
				if (enemy[spc].num[num].is_cnt == enemy[spc].num[num].move_time) {
					enemy[spc].num[num].is_cnt = 0;
					enemy[spc].num[num].move_time = 0;						  //移动行为周期结束
				}
				else enemy[spc].num[num].is_cnt++;							  //移动行为周期计数
			}

			if (enemy[spc].num[num].spin < PI) enemy[spc].num[num].aspect = LEFT;
			if (enemy[spc].num[num].spin > PI) enemy[spc].num[num].aspect = RIGHT;		//判断行走朝向
		}
	}

	if (enemy[spc].num[num].think == 6) enemy[spc].num[num].think = 0;
	else enemy[spc].num[num].think++;										  //思考时站立图改变与地图变换时间比例1:6
	if (enemy[spc].num[num].move == 6) enemy[spc].num[num].move = 0;
	else enemy[spc].num[num].move++;										  //移动时行走图改变与地图变换时间比例1:6
}
#include "character.h"

POINT character, character_position;

int armor_cnt;
int player_damaged;

int shoot, recover, runout;
BULLET *head = NULL, *tail = NULL;

void startup_weapon_show()
{
	//初始手枪
	weapon[0].name = "gun";
	weapon[0].damage = 3;
	weapon[0].cost = 0;
	weapon[0].swift = 10; 
	weapon[0].accuracy = PI / 24;
	weapon[0].spin = 0;
	weapon[0].Bspeed = 40;
	loadimage(&weapon[0].left, _T("resource\\gun_left.jpg"), 72, 72);
	loadimage(&weapon[0].right, _T("resource\\gun_right.jpg"), 72, 72);
	loadimage(&weapon[0].v_left, _T("resource\\gun_vice_left.jpg"), 72, 72);
	loadimage(&weapon[0].v_right, _T("resource\\gun_vice_right.jpg"), 72, 72);         //载入武器图片

	//AK-47
	weapon[1].name = "AK-47";
	weapon[1].damage = 3;
	weapon[1].cost = 1;
	weapon[1].swift = 5;
	weapon[1].accuracy = PI / 18;
	weapon[1].spin = 0;
	weapon[1].Bspeed = 40;
	loadimage(&weapon[1].left, _T("resource\\AK-47_left.jpg"), 104, 104);
	loadimage(&weapon[1].right, _T("resource\\AK-47_right.jpg"), 104, 104);
	loadimage(&weapon[1].v_left, _T("resource\\AK-47_vice_left.jpg"), 104, 104);
	loadimage(&weapon[1].v_right, _T("resource\\AK-47_vice_right.jpg"), 104, 104);     //载入武器图片

}

void startup_character_show()
{
	character_position.x = 576;
	character_position.y = 312;  //设置人物在屏幕中的位置
	character.x = 0;
	character.y = 0;             //初始化人物贴图序号

	knight.aspect = RIGHT;       //初始化人物朝向
	knight.left = character_position.x + 39;
	knight.right = character_position.x + 81;
	knight.top = character_position.y + 36;         
	knight.bottom = character_position.y + 92;			//初始化人物判定坐标范围
	knight.center.x = character_position.x + 60;
	knight.center.y = character_position.y + 64;
	knight.HP = knight.HP_MAX;
	knight.AM = knight.AM_MAX;
	knight.MP = knight.MP_MAX;   //初始化状态信息
	knight.weapon_number = 0;    //初始化武器编号

	loadimage(&Character_vice, _T("resource\\character_vice.jpg"), 480, 600);
	loadimage(&Character, _T("resource\\character.jpg"), 480, 600);						 //载入人物图片
	loadimage(&init_bullet, _T("resource\\bullet.jpg"), 42, 42);
	loadimage(&init_bullet_vice, _T("resource\\bullet_vice.jpg"), 42, 42);				 //载入子弹图片
	loadimage(&bullet_burst, _T("resource\\bullet_burst.jpg"), 216, 72);
	loadimage(&bullet_burst_vice, _T("resource\\bullet_burst_vice.jpg"), 216, 72);		 //载入子弹爆炸效果图片
	BeginBatchDraw();
}

void character_show()
{
	IMAGE gun_left, gun_right, gun_vice_left, gun_vice_right;
	static int damaged_cnt;

	if (knight.HP > 0) 
	{
		//每个小人的像素是（120，120）
		putimage(character_position.x, character_position.y, 120, 120, &Character_vice, character.x * 120, character.y * 120, NOTSRCERASE); 
		if (player_damaged == 1) {
			putimage(character_position.x, character_position.y, 120, 120, &Character, character.x * 120, character.y * 120, DSTINVERT);
			if (damaged_cnt == 3) {
				damaged_cnt = 0;
				player_damaged = 0;
			}
			else damaged_cnt++;
		}		
		else
			putimage(character_position.x, character_position.y, 120, 120, &Character, character.x * 120, character.y * 120, SRCINVERT);

		if (knight.aspect == RIGHT)
		{
			rotateimage(&gun_vice_right, &weapon[knight.weapon_number].v_right, weapon[knight.weapon_number].spin, BLACK, true, true);
			rotateimage(&gun_right, &weapon[knight.weapon_number].right, weapon[knight.weapon_number].spin, WHITE, true, true);

			putimage(635 - gun_vice_right.getwidth() / 2, 391 - gun_vice_right.getheight() / 2, &gun_vice_right, NOTSRCERASE);
			putimage(635 - gun_right.getwidth() / 2, 391 - gun_right.getheight() / 2, &gun_right, SRCINVERT);
		}
		if (knight.aspect == LEFT)
		{
			rotateimage(&gun_vice_left, &weapon[knight.weapon_number].v_left, weapon[knight.weapon_number].spin, BLACK, true, true);
			rotateimage(&gun_left, &weapon[knight.weapon_number].left, weapon[knight.weapon_number].spin, WHITE, true, true);

			putimage(636 - gun_vice_left.getwidth() / 2, 391 - gun_vice_left.getheight() / 2, &gun_vice_left, NOTSRCERASE);
			putimage(636 - gun_left.getwidth() / 2, 391 - gun_left.getheight() / 2, &gun_left, SRCINVERT);
		}
	}
	else {
			putimage(character_position.x, character_position.y, 120, 120, &Character_vice, knight.aspect * 120, 4 * 120, NOTSRCERASE);
			putimage(character_position.x, character_position.y, 120, 120, &Character, knight.aspect * 120, 4 * 120, SRCINVERT);
	}
}

void bullet_show()
{
	BULLET *Blt, *sBlt;
	double deviation;
	int spc, num;
	int mx, my;

	if (shoot == 1)
	{
		shoot = 0;
		if (knight.MP >= weapon[knight.weapon_number].cost) 
		{
			knight.MP -= weapon[knight.weapon_number].cost;
			deviation = (rand() % 11 - 5) * weapon[knight.weapon_number].accuracy / 10;

			Blt = (BULLET*)malloc(sizeof(BULLET));
			Blt->spin = weapon[knight.weapon_number].spin + deviation;
			Blt->Bspeed = weapon[knight.weapon_number].Bspeed;
			Blt->hit = 0;
			Blt->burst_cnt = 0;
			if (knight.aspect == RIGHT) {
				Blt->position.x = map.x + 635 - (int)((36 * sin(Blt->spin)) + 4 * cos(Blt->spin));
				Blt->position.y = map.y + 391 - (int)((36 * cos(Blt->spin)) - 4 * sin(Blt->spin));
			}
			if (knight.aspect == LEFT) {
				Blt->position.x = map.x + 636 - (int)((36 * sin(Blt->spin)) - 4 * cos(Blt->spin));
				Blt->position.y = map.y + 391 - (int)((36 * cos(Blt->spin)) + 4 * sin(Blt->spin));
			}
			Blt->next = NULL;
			if (head == NULL) head = Blt;
			else tail->next = Blt;
			tail = Blt;
		}	
	}

	for (sBlt = NULL, Blt = head; Blt;)
	{
		if (Blt->hit == 0) 
		{
			for (spc = 0; spc < SPECIES; spc++) {
				for (num = enemy[spc].old_amount; num < enemy[spc].amount; num++) {
					if (enemy[spc].num[num].HP > 0) {
						if (enemy[spc].num[num].x + enemy[spc].pic_left <= Blt->position.x
							&& enemy[spc].num[num].x + enemy[spc].pic_right >= Blt->position.x
							&& enemy[spc].num[num].y + enemy[spc].pic_top <= Blt->position.y
							&& enemy[spc].num[num].y + enemy[spc].pic_bottom >= Blt->position.y)
						{
							Blt->hit = 1;
							break;
						}
						else {
							for (Blt->path = Blt->Bspeed / 4; Blt->path < Blt->Bspeed; Blt->path += Blt->Bspeed / 4) {
								if (enemy[spc].num[num].x + enemy[spc].pic_left <= Blt->position.x + (int)(Blt->path * sin(Blt->spin))
									&& enemy[spc].num[num].x + enemy[spc].pic_right >= Blt->position.x + (int)(Blt->path * sin(Blt->spin))
									&& enemy[spc].num[num].y + enemy[spc].pic_top <= Blt->position.y + (int)(Blt->path * cos(Blt->spin))
									&& enemy[spc].num[num].y + enemy[spc].pic_bottom >= Blt->position.y + (int)(Blt->path * cos(Blt->spin)))
								{
									Blt->hit = 2;
									break;
								}
							}
							if (Blt->hit == 2) break;
						}
					}
				}
				if (Blt->hit != 0) break;
			}
		}

		if (object[Blt->position.x][Blt->position.y] != IMMORTAL_OBJ && Blt->hit == 0)
		{
			rotateimage(&bullet_vice, &init_bullet_vice, Blt->spin, BLACK, true, true);
			rotateimage(&bullet, &init_bullet, Blt->spin, WHITE, true, true);

			putimage(Blt->position.x - bullet.getwidth() / 2 - map.x, Blt->position.y - bullet.getheight() / 2 - map.y, &bullet_vice, NOTSRCERASE);
			putimage(Blt->position.x - bullet.getwidth() / 2 - map.x, Blt->position.y - bullet.getheight() / 2 - map.y, &bullet, SRCINVERT);

			if (pause == 0) {
				Blt->position.x -= (int)(Blt->Bspeed * sin(Blt->spin));
				Blt->position.y -= (int)(Blt->Bspeed * cos(Blt->spin));
			}
			
			sBlt = Blt;
			Blt = Blt->next;
		}
		else {
			if (Blt->hit != 2) {
				putimage(Blt->position.x - 36 - map.x, Blt->position.y - 36 - map.y, 72, 72, &bullet_burst_vice, Blt->burst_cnt / 3 * 72, 0, NOTSRCERASE);
				putimage(Blt->position.x - 36 - map.x, Blt->position.y - 36 - map.y, 72, 72, &bullet_burst, Blt->burst_cnt / 3 * 72, 0, SRCINVERT);
			}
			else {
				putimage(Blt->position.x + (int)(Blt->path * sin(Blt->spin)) - 36 - map.x, Blt->position.y + (int)(Blt->path * cos(Blt->spin)) - 36 - map.y,
					72, 72, &bullet_burst_vice, Blt->burst_cnt / 3 * 72, 0, NOTSRCERASE);
				putimage(Blt->position.x + (int)(Blt->path * sin(Blt->spin)) - 36 - map.x, Blt->position.y + (int)(Blt->path * cos(Blt->spin)) - 36 - map.y,
					72, 72, &bullet_burst, Blt->burst_cnt / 3 * 72, 0, SRCINVERT);
			}
			
			if (Blt->burst_cnt == 0 && Blt->hit != 0) {
				enemy[spc].num[num].is_damaged = 1;
				mx = - (int)(30 * sin(Blt->spin));
				my = - (int)(30 * cos(Blt->spin));

				if (object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == FLOOR
					&& object[enemy[spc].num[num].x + enemy[spc].pic_left + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == FLOOR
					&& object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_top + my] == FLOOR
					&& object[enemy[spc].num[num].x + enemy[spc].pic_right + mx][enemy[spc].num[num].y + enemy[spc].pic_bottom + my] == FLOOR) 
				{
					if (spc != 2) {
						enemy[spc].num[num].x += mx;
						enemy[spc].num[num].y += my;
					}				
				}

				enemy[spc].num[num].HP -= weapon[knight.weapon_number].damage;
				if (enemy[spc].num[num].HP <= 0) {
					mciSendString(_T("close resource\\enemy_dead.wav"), NULL, 0, NULL);
					mciSendString(_T("play resource\\enemy_dead.wav"), NULL, 0, NULL);			//怪物击杀音效
				}			
			}

			if (Blt->burst_cnt == 6) {
				if (Blt == head) {
					head = Blt->next;
					free(Blt);
					Blt = head;
				}
				else {
					sBlt->next = Blt->next;
					if (Blt == tail) tail = sBlt;
					free(Blt);
					Blt = sBlt->next;
				}
			}
			else {
				if (pause == 0) Blt->burst_cnt++;
				sBlt = Blt;
				Blt = Blt->next;
			}
		}
	}
}

int auto_aiming()
{
	int dx;								//玩家到怪物判定中心横坐标增量
	int dy;								//玩家到怪物判定中心纵坐标增量
	int path;							//模拟路径
	double distance;					//玩家与怪物之间的距离		
	double shortest = 1000;				//存储最短路径，初始值1000
	double cos;							//玩家与怪物之间夹角余弦值（图形坐标系）
	double sin;							//玩家与怪物之间夹角正弦值（图形坐标系）
	double arc;							//玩家与怪物之间夹角（数学坐标系）
	int spc;							//怪物种族循环变量
	int	num;							//怪物个体循环变量
	int result;							//返回瞄准结果

	for (spc = 0; spc < SPECIES; spc++) {
		for (num = enemy[spc].old_amount; num < enemy[spc].amount; num++) {
			if (enemy[spc].num[num].HP > 0) {
				dx = enemy[spc].num[num].x - map.x - character_position.x;
				dy = enemy[spc].num[num].y - map.y - character_position.y;

				distance = sqrt(dx * dx + dy * dy);
				cos = dx / distance;
				sin = dy / distance;

				//以12为步长，沿怪物到玩家路径判断，出现障碍物则被阻碍
				for (path = 12; path < (int)distance; path += 12) {
					if (object[knight.center.x + map.x + (int)(path * cos)][knight.center.y + map.y + (int)(path * sin)] == IMMORTAL_OBJ)
						break;
				}
				if (path >= (int)distance) {
					if (distance < shortest) {
						shortest = distance;
						if (dy >= 0) arc = 2 * PI - acos(cos) - PI / 2;
						else arc = acos(cos) - PI / 2;
						if (arc < 0) arc += 2 * PI;
					}
				}
			}
		}
	}

	if (shortest != 1000) {
		if (arc < PI) knight.aspect = LEFT;
		if (arc > PI) knight.aspect = RIGHT;
		weapon[knight.weapon_number].spin = arc;
		result = 1;
	}
	else result = 0;

	return result;
}

void armor_recover()
{
	if (knight.AM < knight.AM_MAX) {
		armor_cnt++;	//护甲不满时，恢复开始计时

						//间隔133*30ms=4s时开始回复，此后每隔33*30ms=1s回复一点护甲，直至护甲回满
		if (armor_cnt == 133 || (armor_cnt > 133 && (armor_cnt - 133) % 33 == 0))
			knight.AM++;
	}
	else armor_cnt = 0;  //满护甲时，计时器归零%

	if (runout == 0 && object[knight.center.x + map.x][knight.center.y + map.y] == CRYSTAL) {
		if (recover % 33 == 0) {
			if (knight.MP <= knight.MP_MAX - 8)
				knight.MP += 8;
			else knight.MP = knight.MP_MAX;
			if (recover == 165) {
				if (knight.HP <= knight.HP_MAX - 2)
					knight.HP += 2;
				else knight.HP = knight.HP_MAX;
				recover = 0;
				runout = 1;
			}
		}
		recover++;
	}

}
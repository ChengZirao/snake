#include<stdio.h>
#include<conio.h>//_getch()函数
#include<graphics.h>//easyx图形化窗口
#include<stdlib.h>//随机数
#include<string>
using namespace std;
#define SNAKE_NUM 500//蛇的最大节数
enum DIR { UP, DOWN, LEFT, RIGHT };//方向枚举
struct Snake {
	int size;//蛇的节数
	int dir;//蛇的方向
	int speed;//蛇的速度
	POINT coor[SNAKE_NUM];//坐标，包含xy轴，控制台的y轴和数学y轴是反向的
}snake;
struct Food {
	int x;
	int y;
	int r;		//食物半径（食物大小）
	bool flag;  //食物是否被吃
	DWORD color;//食物颜色
	int score;  //玩家得分
	int TopScore = 0;
}food;
//游戏开始时的数据初始化
void GameInit()
{
	//初始化图形窗口，宽640，高480，包含在graphics.h
	initgraph(640, 480);
	//设置随机数种子，以时间为种子，GetTickCount函数获取从系统开机到现在的毫秒数
	srand(GetTickCount());
	//初始化蛇
	snake.size = 13;//蛇有3节
	snake.speed = 10;
	snake.dir = RIGHT;
	snake.coor[0].x = 10;
	snake.coor[0].y = 10;
	for (int i = 0; i < snake.size; i++)
	{
		//蛇头是coor[0],赋予其它节坐标
		snake.coor[i].x = snake.size + 10 - 10 * i;//10*i因为半径为5，每个圆的圆心距离为10，蛇的每一节圆相切
		snake.coor[i].y = 10;//y轴都一样，蛇身在一条直线上
	}
	//初始化食物，rand()为随机函数，'%'可保证不大于被除的数，简单的数学逻辑:)
	food.x = rand() % 630 + 5;
	food.y = rand() % 470 + 5;
	food.color = WHITE;
	food.r = 5;
	food.flag = true;
	food.score = 0;
}
void GameDraw()
{
	//双缓冲绘图
	BeginBatchDraw();
	//设置背景颜色，bk==background
	setbkcolor(RGB(232, 58, 169));
	//清空绘图设备，清除上一次循环的图像
	cleardevice();
	//打印蛇,每一节蛇身为半径为5的实心圆
	setfillcolor(WHITE);
	settextcolor(BLACK);
	settextstyle(30, 0, "黑体");
	outtextxy(10, 10, "Designed By KiromChan");
	settextstyle(25, 0, "黑体");
	outtextxy(230, 240, "Scores:");
	//定义字符串变量，准备接收int转换变量
	char num[20];
	//将int类型转换为字符串，用字符串num接收
	sprintf_s(num, "%d", food.score);
	//输出数字(以字符串形式)
	outtextxy(320, 240, num);
	char topscore[20];
	sprintf_s(topscore, "%d", food.TopScore);
	outtextxy(150, 270, "Gamer's TopScores:");
	outtextxy(390, 270, topscore);
	for (int i = 0; i < snake.size; i++)
	{
		solidcircle(snake.coor[i].x, snake.coor[i].y, 5);
	}
	//绘制食物
	//如果food存在
	setfillcolor(food.color);
	if (food.flag)
	{
		solidcircle(food.x, food.y, food.r);
	}
	EndBatchDraw();
}
//移动蛇
void snakeMove()
{
	//蛇身的每一节跟着前一节的轨迹走
	for (int i = snake.size - 1; i > 0; i--)
	{
		snake.coor[i] = snake.coor[i - 1];
	}
	//判断蛇头移动的方向
	switch (snake.dir)
	{
	case UP:
		snake.coor[0].y -= snake.speed;//控制台的y轴和数学y轴反向，DOWN同理
		//若超出上边界
		if (snake.coor[0].y <= 0)
		{
			//使蛇头从下边界出来，480是图形化框的高度
			snake.coor[0].y = 480;
		}
		break;
	case DOWN:
		snake.coor[0].y += snake.speed;
		if (snake.coor[0].y >= 480)
		{
			snake.coor[0].y = 0;
		}
		break;
	case LEFT:
		snake.coor[0].x -= snake.speed;
		if (snake.coor[0].x <= 0)
		{
			snake.coor[0].x = 640;
		}
		break;
	case RIGHT:
		snake.coor[0].x += snake.speed;
		if (snake.coor[0].x >= 640)
		{
			snake.coor[0].x = 0;
		}
		break;
	}

}
//通过按键改变蛇的方向
void keycontrol()
{
	//检测用户有没有按下键盘，若有则返回true
	//如果不写该判断则蛇无法移动（或只有用户一直按下按键才会移动）
	if (_kbhit())
	{
		switch (_getch())//_getch()包含在conio.h
		{
		case 'w':
			if (snake.dir != DOWN)
				snake.dir = UP;
			break;
		case 'a':
			if (snake.dir != RIGHT)
				snake.dir = LEFT;
			break;
		case 's':
			if (snake.dir != UP)
				snake.dir = DOWN;
			break;
		case 'd':
			if (snake.dir != LEFT)
				snake.dir = RIGHT;
			break;
			//游戏暂停
		case' ':
			outtextxy(230, 240, "GAME PAUSE");
			while (1)
			{
				if (_getch() == ' ')
					return;
			}
			break;
		case (char)27:   //Esc键
			closegraph();//关闭界面，退出游戏
			break;
		}
	}
	//咬到自己，游戏结束
	for (int i = 1; i < snake.size; i++)
	{
		if (snake.coor[0].x >= snake.coor[i].x - 5 && snake.coor[0].x <= snake.coor[i].x + 5 &&
			snake.coor[0].y >= snake.coor[i].y - 5 && snake.coor[0].y <= snake.coor[i].y + 5)
		{
			outtextxy(230, 240, "GAME OVER!");
			outtextxy(130, 270, "Press Space Key to Continue");
			if (food.score > food.TopScore)
				food.TopScore = food.score;
			while (1)
			{
				//outtextxy(230, 240, L"GAME OVER!");
				if (_getch() == ' ')
				{
					//初始化，游戏重新开始
					GameInit();
					return;
				}
			}
		}
	}
}
void EatFood()
{
	//如果食物存在且蛇头与食物的圆心距离小于他们的半径之和
	if (food.flag && snake.coor[0].x >= food.x - food.r && snake.coor[0].x <= food.x + food.r &&
		snake.coor[0].y >= food.y - food.r && snake.coor[0].y <= food.y + food.r)
	{
		//flag为假，食物消失
		food.flag = false;
		//吃了食物后，蛇身长度加1
		snake.size++;
		food.score++;
	}
	//如果食物消失，则重新生成（重新初始化）
	if (!food.flag)
	{
		food.x = rand() % 630 + 5;
		food.y = rand() % 470 + 5;
		food.color = WHITE;
		food.r = 5;
		food.flag = true;
	}
}
int main()
{
	GameInit();
	while (1)
	{
		GameDraw();
		snakeMove();
		keycontrol();
		EatFood();
		Sleep(60);//延时60毫秒
	}
	return 0;
}
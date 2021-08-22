#include<stdio.h>
#include<conio.h>//_getch()����
#include<graphics.h>//easyxͼ�λ�����
#include<stdlib.h>//�����
#include<string>
using namespace std;
#define SNAKE_NUM 500//�ߵ�������
enum DIR { UP, DOWN, LEFT, RIGHT };//����ö��
struct Snake {
	int size;//�ߵĽ���
	int dir;//�ߵķ���
	int speed;//�ߵ��ٶ�
	POINT coor[SNAKE_NUM];//���꣬����xy�ᣬ����̨��y�����ѧy���Ƿ����
}snake;
struct Food {
	int x;
	int y;
	int r;		//ʳ��뾶��ʳ���С��
	bool flag;  //ʳ���Ƿ񱻳�
	DWORD color;//ʳ����ɫ
	int score;  //��ҵ÷�
	int TopScore = 0;
}food;
//��Ϸ��ʼʱ�����ݳ�ʼ��
void GameInit()
{
	//��ʼ��ͼ�δ��ڣ���640����480��������graphics.h
	initgraph(640, 480);
	//������������ӣ���ʱ��Ϊ���ӣ�GetTickCount������ȡ��ϵͳ���������ڵĺ�����
	srand(GetTickCount());
	//��ʼ����
	snake.size = 13;//����3��
	snake.speed = 10;
	snake.dir = RIGHT;
	snake.coor[0].x = 10;
	snake.coor[0].y = 10;
	for (int i = 0; i < snake.size; i++)
	{
		//��ͷ��coor[0],��������������
		snake.coor[i].x = snake.size + 10 - 10 * i;//10*i��Ϊ�뾶Ϊ5��ÿ��Բ��Բ�ľ���Ϊ10���ߵ�ÿһ��Բ����
		snake.coor[i].y = 10;//y�ᶼһ����������һ��ֱ����
	}
	//��ʼ��ʳ�rand()Ϊ���������'%'�ɱ�֤�����ڱ����������򵥵���ѧ�߼�:)
	food.x = rand() % 630 + 5;
	food.y = rand() % 470 + 5;
	food.color = WHITE;
	food.r = 5;
	food.flag = true;
	food.score = 0;
}
void GameDraw()
{
	//˫�����ͼ
	BeginBatchDraw();
	//���ñ�����ɫ��bk==background
	setbkcolor(RGB(232, 58, 169));
	//��ջ�ͼ�豸�������һ��ѭ����ͼ��
	cleardevice();
	//��ӡ��,ÿһ������Ϊ�뾶Ϊ5��ʵ��Բ
	setfillcolor(WHITE);
	settextcolor(BLACK);
	settextstyle(30, 0, "����");
	outtextxy(10, 10, "Designed By KiromChan");
	settextstyle(25, 0, "����");
	outtextxy(230, 240, "Scores:");
	//�����ַ���������׼������intת������
	char num[20];
	//��int����ת��Ϊ�ַ��������ַ���num����
	sprintf_s(num, "%d", food.score);
	//�������(���ַ�����ʽ)
	outtextxy(320, 240, num);
	char topscore[20];
	sprintf_s(topscore, "%d", food.TopScore);
	outtextxy(150, 270, "Gamer's TopScores:");
	outtextxy(390, 270, topscore);
	for (int i = 0; i < snake.size; i++)
	{
		solidcircle(snake.coor[i].x, snake.coor[i].y, 5);
	}
	//����ʳ��
	//���food����
	setfillcolor(food.color);
	if (food.flag)
	{
		solidcircle(food.x, food.y, food.r);
	}
	EndBatchDraw();
}
//�ƶ���
void snakeMove()
{
	//�����ÿһ�ڸ���ǰһ�ڵĹ켣��
	for (int i = snake.size - 1; i > 0; i--)
	{
		snake.coor[i] = snake.coor[i - 1];
	}
	//�ж���ͷ�ƶ��ķ���
	switch (snake.dir)
	{
	case UP:
		snake.coor[0].y -= snake.speed;//����̨��y�����ѧy�ᷴ��DOWNͬ��
		//�������ϱ߽�
		if (snake.coor[0].y <= 0)
		{
			//ʹ��ͷ���±߽������480��ͼ�λ���ĸ߶�
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
//ͨ�������ı��ߵķ���
void keycontrol()
{
	//����û���û�а��¼��̣������򷵻�true
	//�����д���ж������޷��ƶ�����ֻ���û�һֱ���°����Ż��ƶ���
	if (_kbhit())
	{
		switch (_getch())//_getch()������conio.h
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
			//��Ϸ��ͣ
		case' ':
			outtextxy(230, 240, "GAME PAUSE");
			while (1)
			{
				if (_getch() == ' ')
					return;
			}
			break;
		case (char)27:   //Esc��
			closegraph();//�رս��棬�˳���Ϸ
			break;
		}
	}
	//ҧ���Լ�����Ϸ����
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
					//��ʼ������Ϸ���¿�ʼ
					GameInit();
					return;
				}
			}
		}
	}
}
void EatFood()
{
	//���ʳ���������ͷ��ʳ���Բ�ľ���С�����ǵİ뾶֮��
	if (food.flag && snake.coor[0].x >= food.x - food.r && snake.coor[0].x <= food.x + food.r &&
		snake.coor[0].y >= food.y - food.r && snake.coor[0].y <= food.y + food.r)
	{
		//flagΪ�٣�ʳ����ʧ
		food.flag = false;
		//����ʳ��������ȼ�1
		snake.size++;
		food.score++;
	}
	//���ʳ����ʧ�����������ɣ����³�ʼ����
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
		Sleep(60);//��ʱ60����
	}
	return 0;
}
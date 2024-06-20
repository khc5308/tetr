/****************************************************************************
* @mainpage Tetris : ��Ʈ���� ���� ����
* @version 1.0
* @date 1985��
* @author �˷����� �������� (���þ�)
* @brief ���� ��Ʈ���� ������ �����Ͽ� ����� ��Ʈ���� ������ �����.
* @detail ��ó : ȥ�� �����ϴ� C/C++ (http://www.soen.kr/)
*            ��.��Ʈ���� http://www.soen.kr/lecture/ccpp/cpp2/14-3-1.htm
*            ��.������ ��� ���� http://www.soen.kr/lecture/ccpp/cpp2/14-3-2.htm
*            ��.������ http://www.soen.kr/lecture/ccpp/cpp2/14-3-3.htm
*            ��.Turboc.h http://soen.kr/lecture/ccpp/cpp1/6-4-4.htm
*****************************************************************************/

#include "turbo.h"

/* ��� */
#define LEFT 75		//��Ű
#define RIGHT 77	//��Ű
#define UP 72		//��Ű
#define DOWN 80		//��Ű   
#define ESC 27
#define A 83
#define Z 82
#define BX 5		//�������� �»�� ��ǥ X (�ܺ� ���� �»�� ��ǥ X)
#define BY 1		//�������� �»�� ��ǥ Y (�ܺ� ���� �»�� ��ǥ Y)
#define BW 10		//�������� �� (�ܺκ� ����)
#define BH 20		//�������� ���� (�ܺκ� ����)
#define max(a,b) (((a) > (b)) ? (a) : (b))

/* �Լ� */
void drawScreen();			//������ ���ȭ���� �������� �׸���.
void drawBoard();			//�������� ����ȭ���� �׸���. (�ܺκ��� ���ڿ� ����)
void printBrick(bool Show);	//������ �׸���.
bool moveDown();			//������ �¿�, �Ʒ��� �����δ�.
bool processKey();			//Ű�� ������ �� ������ �����Ѵ�.
int getAround(int x, int y, int b, int r);	//������ �ֺ��� �����Ͽ� ������� �ִ��� üũ�Ѵ�.
void testFull();			//�������� ������ �� á���� üũ�Ѵ�.
void bagbag();				//�ߺ����� 7���� �̱�
void nextPrint();
int bag[7] = { 0,0,0,0,0,0,0 };
int bagNum = 0;
int bagMino = 0;
int first_next = 0;
int next[5];
int nextMino = 0;
int line = 0;
int line_select;

/* ����ü */
typedef struct point
{
	int x, y;
} point;


/* ���� ���� */
//shape[brick][rot][tile], shape[������ȣ][ȸ����ȣ][Ÿ�Ϲ�ȣ]
//������(0,0)���κ����� �����ǥ�� �����ϴ� ���
point shape[][4][4] =
{
	{
		{ {0,0},{1,0},{2,0},{-1,0} }, { {0,0},{0,1},{0,-1},{0,-2} },
		{ {0,0},{1,0},{2,0},{-1,0} }, { {0,0},{0,1},{0,-1},{0,-2} }
	},	//0�� ��
	{
		{ {0,0},{1,0},{0,1},{1,1} }, { {0,0},{1,0},{0,1},{1,1} },
		{ {0,0},{1,0},{0,1},{1,1} }, { {0,0},{1,0},{0,1},{1,1} }
	},	//1�� ��
	{
		{ {0,0},{-1,0},{0,-1},{1,-1} }, { {0,0},{0,1},{-1,0},{-1,-1} },
		{ {0,0},{-1,0},{0,-1},{1,-1} }, { {0,0},{0,1},{-1,0},{-1,-1} }
	},	//2�� ����
	{
		{ {0,0},{-1,-1},{0,-1},{1,0} }, { {0,0},{-1,0},{-1,1},{0,-1} },
		{ {0,0},{-1,-1},{0,-1},{1,0} }, { {0,0},{-1,0},{-1,1},{0,-1} }
	},	//3�� ����
	{
		{ {0,0},{-1,0},{1,0},{-1,-1} }, { {0,0},{0,-1},{0,1},{-1,1} },
		{ {0,0},{-1,0},{1,0},{1,1} }, { {0,0},{0,-1},{0,1},{1,-1} }
	},	//4�� ����
	{
		{ {0,0},{1,0},{-1,0},{1,-1} }, { {0,0},{0,1},{0,-1},{-1,-1} },
		{ {0,0},{1,0},{-1,0},{-1,1} }, { {0,0},{0,-1},{0,1},{1,1} }
	},	//5�� ����
	{
		{ {0,0},{-1,0},{1,0},{0,1} }, { {0,0},{0,-1},{0,1},{1,0} },
		{ {0,0},{-1,0},{1,0},{0,-1} }, { {0,0},{-1,0},{0,-1},{0,1} }
	}	//6�� ��
};


enum { EMPTY, BRICK, WALL }; //0-EMPTY-����, 1-BRICK-�ٸ� ����, 2-WALL-��

const char* arTile[] = { " ","��","��" }; //����, ����, �� ����� �����ϴ� ���ڿ� �迭, Ÿ�� 1���� 2���ڷ� ��� ǥ��

int board[BW + 2][BH + 2]; //��ü �������� ����

int nx, ny; //�̵����� ������ �迭���� ���� ��ǥ (ȭ����� ��ǥ BX+nx*2, BY+ny)
int brick; //�̵����� ������ ��ȣ
int rot; //�̵����� ������ ȸ�� ��� ��ȣ


/* main�Լ� */
int main() {

	int nFrame, nStay; //������ �������� �ӵ�
	int x, y;

	/*���� �ʱ�ȭ*/
	setCursorType(NOCURSOR); //Ŀ�� ����
	srand((unsigned)time(NULL)); //���������� �ʱ�ȭ
	clearScreen(); //ȭ�� ����
	gotoXY(0, 0);
	printf(" ���� �� ���� �Է��ϼ��� : ");
	scanf("%d", &line_select);
	delay(1000);
	gotoXY(0, 0);
	printf("                                             ");
	//���� �迭�� �ٱ����� ������ �ʱ�ȭ
	for (x = 0; x < BW + 2; x++)
	{
		for (y = 0; y < BH + 2; y++)
		{
			board[x][y] = (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) ? WALL : EMPTY;
		}
	}

	drawScreen(); //ȭ�� ��ü �׸���

	nFrame = 20; //������ �������� �ӵ�����
	gotoXY(50, 20);
	printf("%d / %d", line, line_select);

	while (1) {
		if (line >= line_select) {
			clearScreen();
			gotoXY(30, 12);
			puts("F Id N S H");
			setCursorType(NORMALCURSOR);
			delay(1000);
			return 0;
		}

		nx = BW / 2; //������ �ʱ� X��ǥ
		ny = 2; //������ �ʱ� Y��ǥ
		rot = 0; //������ �ʱ� ȸ����� ��ȣ

		if (first_next == 0) {
			for (int i = 0; i < 5; i++)
				bagbag();
			first_next++;
		}

		bagbag();
		nextPrint();

		/*���� �� ����*/
		if (getAround(nx, ny, brick, rot) != EMPTY) //���� �ֺ��� ������� üũ
			break;
		nStay = nFrame;

		while (1)
		{
			/*��������*/
			if (--nStay == 0)
			{
				nStay = nFrame;
				if (moveDown()) //�ٴڿ� ������ true
					break;
			}
			/*Ű�Է� ó��*/
			if (processKey()) //�ٸ� ������ ������ true
				break;
			/*�ð� ����*/
			delay(1000 / 20);  //0.05�ʾ� ���� (1000=1��)
		}
	}
	
	/*���� �� ó��*/
	clearScreen();
	gotoXY(30, 12);
	puts("G A M E  O V E R");
	setCursorType(NORMALCURSOR);

}

/**
* @brief drawScreen ������ ����� �������� �׸���.
* @detail board[x][y]�� EMPTY(0), BRICK(1), WALL(2)�� ������ �Ϳ� �°�
*         arTile[]�� ���� ". "(0), "��"(1), "��"(2)�� ȭ�鿡 �׸���.
*/
void drawScreen()
{
	int x, y;

	for (x = 0; x < BW + 2; x++)
	{
		for (y = 0; y < BH + 2; y++)
		{
			gotoXY(BX + x * 2, BY + y);
			puts(arTile[board[x][y]]);
		}
	}

	gotoXY(50, 3);
	puts("Tetris Ver 1.0");

	gotoXY(50, 5);
	puts("�¿�:�̵�, ��:ȸ��, �Ʒ�:����");

	gotoXY(50, 6);
	puts("����:���� ����");

	gotoXY(50, 30);


}


/**
* @brief drawBoard �������� ���θ� �׸���.
* @detail �ܺκ��� ���ڿ��� ������ �׿� �ִ� ������ �׸���.
*         board[x][y]�� EMPTY(0), BRICK(1), WALL(2)�� ������ �Ϳ� �°�
*         arTile[]�� ���� ". "(0), "��"(1), "��"(2)�� ȭ�鿡 �׸���.
*/
void drawBoard()
{
	int x, y;

	for (x = 1; x < BW + 1; x++)
	{
		for (y = 1; y < BH + 1; y++)
		{
			gotoXY(BX + x * 2, BY + y);
			puts(arTile[board[x][y]]);
		}
	}
}


/**
* @brief printBrick ������ �׸���.
* @param bool Show true�̸� ������ �׸���, false�̸� ������ �����Ѵ�.
*/
void printBrick(bool Show)
{

	for (int i = 0; i < 4; i++)
	{
		gotoXY(BX + (shape[brick][rot][i].x + nx) * 2, BY + shape[brick][rot][i].y + ny);
		puts(arTile[Show ? BRICK : EMPTY]);
	}

}




/**
* @brief moveDown ������ �� ĭ �� �Ʒ��� ������.
* @return bool �ٸ� ������ ������ true�� ��ȯ�Ѵ�.
*/
bool moveDown()
{
	if (getAround(nx, ny + 1, brick, rot) != EMPTY)
	{
		testFull();
		return true;
	}
	printBrick(false);
	ny++;
	printBrick(true);
	return false;
}



/**
* @brief processKey Ű�Է��� �޾� �׿� �´� ó���� �Ѵ�.
* @return �̵� ���� ������ �ٸ� ������ ������ true�� ��ȯ�Ѵ�.
*/
bool processKey()
{
	int ch, trot;

	if (_kbhit()) //������ Ű�� ������ true�� ��ȯ�Ѵ�.
	{
		ch = _getch(); //<conio.h> getch ���� �Է��� ������ ����ߴٰ� ���� ������ �Է¹��ۿ��� 1����Ʈ�� �����´�.

		//ȭ��ǥŰ(��Ű��Ű��Ű��Ű)�� 2byte�� ǥ���Ǵ� Ȯ��Ű�ڵ��̴�.
		//Ȯ��Ű�ڵ尡 �ԷµǾ��� ��쿡�� 0 �Ǵ� 0xE0�� ��ȯ�� ��, �ش� Ű ��ĵ�ڵ带 ��ȯ�Ѵ�.
		if (ch == 0xE0 || ch == 0)
		{
			ch = _getch();

			switch (ch)
			{
			case LEFT: //��Ű �Է� : ���� �̵�
				if (getAround(nx - 1, ny, brick, rot) == EMPTY)
				{
					printBrick(false);
					nx--;
					printBrick(true);
				}
				break;

			case RIGHT: //��Ű �Է� : ������ �̵�
				if (getAround(nx + 1, ny, brick, rot) == EMPTY)
				{
					printBrick(false);
					nx++;
					printBrick(true);
				}
				break;

			case UP: //��Ű �Է� : ȸ��
				trot = (rot == 3 ? 0 : rot + 1);
				if (getAround(nx, ny, brick, trot) == EMPTY)
				{
					printBrick(false);
					rot = trot;
					printBrick(true);
				}
				break;

			case DOWN: //��Ű �Է� : �Ʒ��� ����
				if (moveDown())
				{
					return true;
				}
				break;
			case A: // 'a' Ű �Է� : 180�� ȸ��
				trot = (rot == 3 ? 1 : (rot == 2 ? 0 : rot + 2));
				if (getAround(nx, ny, brick, trot) == EMPTY)
				{
					printBrick(false);
					rot = trot;
					printBrick(true);
				}
				break;

			case Z: // 'z' Ű �Է� : 270�� ȸ��
				trot = (rot == 0 ? 3 : rot - 1);
				if (getAround(nx, ny, brick, trot) == EMPTY)
				{
					printBrick(false);
					rot = trot;
					printBrick(true);
				}
				break;
			}
			
		}
		else {

			switch (ch)
			{
			case ' ': //spaceŰ �Է� : �ٴڱ��� ����
				while (moveDown() == false)
				{
					;
				}
				return true;

			}

		}

	}

	return false;

}


/**
* @brief getAround ������ �ֺ��� ��������, �ٸ� ������ �ִ��� �Ǵ� �������� �Ǵ��Ѵ�.
* @param int x ���� X��ǥ
* @param int y ���� Y��ǥ
* @param int b ���� ��ȣ
* @param int r ������ ȸ�� ��ȣ
* @return int �����̸� EMPTY, �ٸ� ������ ������ BRICK, ���̸� WALL�� ��ȯ�Ѵ�.
*/
int getAround(int x, int y, int b, int r)
{
	int i, k = EMPTY;

	for (i = 0; i < 4; i++)
	{
		k = max(k, board[x + shape[b][r][i].x][y + shape[b][r][i].y]);
	}

	return k;
}


/**
* @brief testFull �������� ������ �� á���� üũ�Ѵ�.
* @detail �̵����� ������ board�迭�� ����Ѵ�.
*         ���� ��� �����̸� ������ �����.
*/
void testFull()
{
	int i, x, y, ty;

	//�̵����� ������ board �迭�� ����Ѵ�.
	for (i = 0; i < 4; i++)
	{
		board[nx + shape[brick][rot][i].x][ny + shape[brick][rot][i].y] = BRICK;
	}

	for (y = 1; y < BH + 1; y++)
	{
		//������ ��� �������� �Ǵ�
		for (x = 1; x < BW + 1; x++)
		{
			if (board[x][y] != BRICK)
				break;
		}
		//������ ��� ������ ���, ������ �� ���ٺ��� ��ĭ�� �Ʒ��� �����Ѵ�.
		if (x == BW + 1)
		{
			line++;
			gotoXY(50, 20);
			printf("%d / %d", line, line_select);
			for (ty = y; ty > 1; ty--)
			{
				for (x = 1; x < BW + 1; x++)
				{
					board[x][ty] = board[x][ty - 1];
				}
			}

			drawBoard(); //�� �� ���� �� ������ ����ȭ���� �׸���.
			delay(200); //0.2�� �ð� ����
		}
	}

}

void bagbag() {
	while (1) {
		int tmp = rand() % (sizeof(shape) / sizeof(shape[0])); // 0~6�� ���� �����ϱ�
		if (bag[tmp] == 0) {
			if (nextMino < 5) {
				next[nextMino] = tmp;
				nextMino++;
				
				bag[tmp]++;
				bagMino++;
				break;
			}
			else {
				bag[tmp]++;
				bagMino++;
				brick = next[0];
				printBrick(true); //���� �׸��� �Լ� ȣ��

				next[0] = next[1];
				next[1] = next[2];
				next[2] = next[3];
				next[3] = next[4];
				next[4] = tmp;
			}

			if (bagMino == 7) { // 7�� ������ ��� �������� ��
				for (int i = 0; i < 7; i++) 
					bag[i] = 0; // bag �迭 �ʱ�ȭ
				bagMino = 0;
			}	
			break;
		}
	}
		
}
//void nextPrint() {
//	for (int i = 0; i < 5; i++) {
//		printf(shape[brick[i]][0][].x);
//		printf(shape[brick[i]][0][].y);
//	}
//}

void nextPrint()
{
	for (int i = 0; i < 5; i++) {
		switch (next[i]){
			case 0 : gotoXY(50 + i * 2, 1); printf("I"); break;
			case 1 : gotoXY(50 + i * 2, 1); printf("O"); break;
			case 2 : gotoXY(50 + i * 2, 1); printf("S"); break;
			case 3 : gotoXY(50 + i * 2, 1); printf("Z"); break;
			case 4 : gotoXY(50 + i * 2, 1); printf("J"); break;
			case 5 : gotoXY(50 + i * 2, 1); printf("L"); break;
			case 6 : gotoXY(50 + i * 2, 1); printf("T"); break;
		}
	}
}
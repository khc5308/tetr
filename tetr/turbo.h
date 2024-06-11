//bool processKey()
//{
//    int ch, trot;
//
//    if (_kbhit()) //눌러진 키가 있으면 true를 반환한다.
//    {
//        ch = _getch(); //<conio.h> getch 값을 입력할 때까지 대기했다가 값이 들어오면 입력버퍼에서 1바이트를 가져온다.
//
//        //화살표키(←키→키↑키↓키)는 2byte로 표현되는 확장키코드이다.
//        //확장키코드가 입력되었을 경우에는 0 또는 0xE0을 반환한 후, 해당 키 스캔코드를 반환한다.
//        if (ch == 0xE0 || ch == 0)
//        {
//            ch = _getch();
//
//            switch (ch)
//            {
//            case LEFT: //←키 입력 : 왼쪽 이동
//                if (getAround(nx - 1, ny, brick, rot) == EMPTY)
//                {
//                    printBrick(false);
//                break;
//
//            case UP: //↑키 입력 : 회전
//                trot = (rot == 3 ? 0 : rot + 1);
//                if (getAround(nx, ny, brick, trot) == EMPTY)
//                {
//                    printBrick(false);
//                    rot = trot;
//                    printBrick(true);
//                }
//                break;
//
//            case DOWN: //↓키 입력 : 아래로 내림
//                if (moveDown())
//                {
//                    return true;
//                }
//                break;
//            }
//
//        }
//        else {
//
//            switch (ch)
//            {
//            case ' ': //space키 입력 : 바닥까지 내림
//                while (moveDown() == false)
//                {
//                    ;
//                }
//                return true;
//
//            case ROT180: // z키 입력 : 180도 회전
//                trot = (rot + 2) % 4;
//                if (getAround(nx, ny, brick, trot) == EMPTY)
//                {
//                    printBrick(false);
//                    rot = trot;
//                    printBrick(true);
//                }
//                break;
//
//            case ROT270: // x키 입력 : 270도 회전
//                trot = (rot == 0 ? 3 : rot - 1);
//                if (getAround(nx, ny, brick, trot) == EMPTY)
//                {
//                    printBrick(false);
//                    rot = trot;
//                    printBrick(true);
//                }
//                break;
//            }
//
//        }
//
//    }
//
//    return false;
//
//}
/****************************************************************************
* @version 1.0
* @date - 2018/4/16
* @author sohi
* @brief 비주얼 C++ 환경에서 터보 C 스타일의 함수를 정의한다.
* @detail 출처 : 혼자 연구하는 C/C++ (http://www.soen.kr/)
*            라.Turboc.h http://soen.kr/lecture/ccpp/cpp1/6-4-4.htm
*****************************************************************************/

#ifndef TURBOC_HEADER //TURBOC_HEADER 매크로가 정의되어 있지 않으면 아래 문장 컴파일
#define TURBOC_HEADER
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>x`

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } cursor_type;

void clearScreen();			// 화면을 모두 지운다. //tc->clrscr();
void gotoXY(int x, int y);	// 커서를 x,y좌표로 이동시킨다.
int whereX();				// 커서의 x 좌표를 조사한다.
int whereY();				// 커서의 y 좌표를 조사한다.
void setCursorType(cursor_type c);	// 커서를 숨기거나 다시 표시한다.

#define delay(n) Sleep(n)							// n/1000초만큼 시간 지연
#define randomize() srand((unsigned)time(NULL))		// 난수 발생기 clearScreen
//<stdlib.h> srand함수 : 난수 발생기를 초기화 한다.
#define random(n) (rand() % (n))					//0~n까지의 난수 발생
					 //<stdlib.h> rand함수 : 난수표에서 난수를 선택한다.

// 이 매크로가 정의되어 있으면 함수의 원형만 선언하고 정의는 하지 않는다.
#ifndef TURBOC_PROTOTYPE_ONLY
	/**
	* @brief clearScreen 화면을 지우고 커서를 (1,1)로 옮긴다.
	*/
void clearScreen()
{
	system("cls");
}

/**
* @brief gotoXY 콘솔화면에서 커서를 (x,y)로 이동시킨다.
* @param int 콘솔화면에서의 가로 위치를 지정
* @param int 콘솔화면에서의 세로 위치를 지정
*/
void gotoXY(int x, int y)
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

/**
* @brief whereX 커서의 X 좌표를 조사한다.
* @return int 현재 X 좌표
*/
int whereX()
{
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
	return BufInfo.dwCursorPosition.X;
}

/**
* @brief whereY 커서의 Y 좌표를 조사한다.
* @return int 현재 Y 좌표
*/
int whereY()
{
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
	return BufInfo.dwCursorPosition.Y;
}

/**
* @brief setCursorType 커서의 모양을 결정한다.
* @param cursor_type NOCURSOR(0), SOLIDCURSOR(1), NORMALCURSOR(2)
*/
void setCursorType(cursor_type c)
{
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

#endif // TURBOC_PROTOTYPE_ONLY
#endif // TURBOC_HEADER

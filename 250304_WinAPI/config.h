﻿// config.h

#pragma once
// 메모리 할당 시 파일, 라인 정보 확인 가능
//#define _CRTDBG_MAP_ALLOC
//#include <cstdlib>
//#include <crtdbg.h>
//
//#ifdef _DEBUG
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#endif

#pragma comment(lib, "Winmm.lib")

#include <Windows.h>
#include <string>
#include <iostream>
#include <bitset>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

#include "KeyManager.h"
#include "ImageManager.h"
#include "TimerManager.h"
#include "SceneManager.h"

/*
	컴파일러에서 해당 코드를 뒤에 정의된 코드로 변경한다. 
*/
#define WINSIZE_X	1080
#define WINSIZE_Y	500
#define TILEMAPTOOL_X	1420
#define TILEMAPTOOL_Y	700

#define DEG_TO_RAD(degree) ((3.14 / 180.0) * degree)
#define RAD_TO_DEG(radian) ((180.0 / 3.14) * radian)

typedef struct tagFPOINT
{
	float x;
	float y;
} FPOINT;

/*
	extern 키워드 : 변수나 함수가 다른 파일에 정의되어 있다 라는
	사실을 알리는 키워드.
*/
extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;

// 샘플 타일 정보
// 640 * 288
#define SAMPLE_TILE_X	20
#define SAMPLE_TILE_Y	9
#define TILE_SIZE	32

// 메인 그리기 공간 정보
#define TILE_X	20
#define TILE_Y	20

#define SAFE_DELETE(ptr) \
	if (ptr)			 \
	{					 \
		ptr->Release();	 \
		delete ptr;		 \
		ptr = nullptr;	 \
	}					 \

typedef struct tagTile
{
	RECT rc;
	int frameX;
	int frameY;
} TILE_INFO;

enum Dir
{
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
};


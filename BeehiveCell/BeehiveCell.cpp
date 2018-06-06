// BeehiveCell.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "BeehiveCell.h"
#include "GetCell.h"

#define MAX_LOADSTRING 100

int g_CellSize;

HBITMAP g_BitmapBkGnd; //背景位图

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
HBITMAP CreateBkBitmap(HWND hWnd);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BEEHIVECELL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BEEHIVECELL));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BEEHIVECELL));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_BEEHIVECELL);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   	//设置蜂窝CELL的边长
	g_CellSize = 50; //50像素
	SetCellSize(g_CellSize);

	//创建背景位图
	g_BitmapBkGnd = CreateBkBitmap(hWnd);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	//被选中的Cell的中心点坐标
	static POINT selCell;
	static HBRUSH hCellBrush; //绘制被选中的Cell的Brush！

	switch (message)
	{
	case WM_CREATE:
		{
			//初始化
			selCell.x = 0;
			selCell.y = 0;
			//创建蓝色刷子
			hCellBrush = CreateSolidBrush(RGB(0,0,255));
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		{
		POINT points[6];
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		if(selCell.x >= 0 && selCell.y >=0)
		{
			HGDIOBJ hOldBrush = SelectObject(hdc, hCellBrush);

			//根据Cell的中心点坐标，得到Cell的六边形端点坐标
			GetCellPoints(selCell.x, selCell.y, g_CellSize, points);
			
			Polygon(hdc, points, 6);
			SelectObject(hdc, hOldBrush);
		}		
		EndPaint(hWnd, &ps);
		}
		break;

		//贴我们已经准备好的背景位图
	case WM_ERASEBKGND:
		{
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);
		hdc = (HDC)wParam;
		HDC hMemDC = CreateCompatibleDC(hdc);

		HGDIOBJ hOldBitmap = SelectObject(hMemDC, g_BitmapBkGnd);
		BitBlt(hdc, 0, 0, rcClient.right, rcClient.bottom, hMemDC, 0, 0, SRCCOPY);
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
		}
		break;

		//鼠标移动时候，即时获取被捕获的CELL
	case WM_MOUSEMOVE:
		{
			RECT rc;//无效矩形
			POINT ptDown = {LOWORD(lParam), HIWORD(lParam)};
			int cellx, celly;

			GetCell(ptDown.x, ptDown.y, &cellx, &celly);

			//被选中的cell是否改变了？
			if(selCell.x != cellx || selCell.y != celly)
			{

				rc.left = min(selCell.x, cellx) - g_CellSize;
				rc.top = min(selCell.y, celly) - g_CellSize;
				rc.right = max(selCell.x, cellx) + g_CellSize;
				rc.bottom = max(selCell.y, celly) + g_CellSize;
				//更新被选中的CELL
				selCell.x = cellx;
				selCell.y = celly;
				//更新视图
				InvalidateRect(hWnd, &rc, TRUE);
			}
		}
		break;
	case WM_DESTROY:
		DeleteObject(hCellBrush);
		DeleteObject(g_BitmapBkGnd);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//创建一个背景位图，用于刷新的时候绘制背景
HBITMAP CreateBkBitmap(HWND hWnd)
{
	int cx=0,cy=0,i, j;
	POINT points[6];
	RECT rc;
	//查询屏幕尺寸
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	
	//创建兼容的内存DC
	HDC hDC = GetDC(hWnd);
	HDC hMemDC = CreateCompatibleDC(hDC);
	ReleaseDC(hWnd, hDC);

	HBITMAP hBitmap = CreateCompatibleBitmap(hMemDC, width, height);

	HGDIOBJ hOldBitmap = SelectObject(hMemDC, hBitmap);

	rc.left = rc.top = 0;
	rc.right = width;
	rc.bottom = height;

	FillRect(hMemDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
	
	//绘制蜂窝网格
	int jmax = (int)(height / g_unity) +1;
	int imax = (int)(width / g_unitx) +1; 
	for(j=0; j < jmax; j++)
	{
		for(i=0; i< imax; i++)
		{
			//如果是奇数，向右移动半个格子
			if(j%2 == 0)
				cx = (int)(g_unitx*i);
			else
				cx = (int)(g_unitx*(i+0.5));

			cy = (int)(g_unity * j);
			
			//根据中心点，求取六边形端点
			GetCellPoints(cx, cy, g_CellSize, points);
			Polygon(hMemDC, points, 6);
		}
	}

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
	return hBitmap;
}

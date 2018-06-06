// BeehiveCell.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "BeehiveCell.h"
#include "GetCell.h"

#define MAX_LOADSTRING 100

int g_CellSize;

HBITMAP g_BitmapBkGnd; //����λͼ

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
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

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BEEHIVECELL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BEEHIVECELL));

	// ����Ϣѭ��:
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   	//���÷���CELL�ı߳�
	g_CellSize = 50; //50����
	SetCellSize(g_CellSize);

	//��������λͼ
	g_BitmapBkGnd = CreateBkBitmap(hWnd);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	//��ѡ�е�Cell�����ĵ�����
	static POINT selCell;
	static HBRUSH hCellBrush; //���Ʊ�ѡ�е�Cell��Brush��

	switch (message)
	{
	case WM_CREATE:
		{
			//��ʼ��
			selCell.x = 0;
			selCell.y = 0;
			//������ɫˢ��
			hCellBrush = CreateSolidBrush(RGB(0,0,255));
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
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
		// TODO: �ڴ���������ͼ����...
		if(selCell.x >= 0 && selCell.y >=0)
		{
			HGDIOBJ hOldBrush = SelectObject(hdc, hCellBrush);

			//����Cell�����ĵ����꣬�õ�Cell�������ζ˵�����
			GetCellPoints(selCell.x, selCell.y, g_CellSize, points);
			
			Polygon(hdc, points, 6);
			SelectObject(hdc, hOldBrush);
		}		
		EndPaint(hWnd, &ps);
		}
		break;

		//�������Ѿ�׼���õı���λͼ
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

		//����ƶ�ʱ�򣬼�ʱ��ȡ�������CELL
	case WM_MOUSEMOVE:
		{
			RECT rc;//��Ч����
			POINT ptDown = {LOWORD(lParam), HIWORD(lParam)};
			int cellx, celly;

			GetCell(ptDown.x, ptDown.y, &cellx, &celly);

			//��ѡ�е�cell�Ƿ�ı��ˣ�
			if(selCell.x != cellx || selCell.y != celly)
			{

				rc.left = min(selCell.x, cellx) - g_CellSize;
				rc.top = min(selCell.y, celly) - g_CellSize;
				rc.right = max(selCell.x, cellx) + g_CellSize;
				rc.bottom = max(selCell.y, celly) + g_CellSize;
				//���±�ѡ�е�CELL
				selCell.x = cellx;
				selCell.y = celly;
				//������ͼ
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

// �����ڡ������Ϣ�������
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

//����һ������λͼ������ˢ�µ�ʱ����Ʊ���
HBITMAP CreateBkBitmap(HWND hWnd)
{
	int cx=0,cy=0,i, j;
	POINT points[6];
	RECT rc;
	//��ѯ��Ļ�ߴ�
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	
	//�������ݵ��ڴ�DC
	HDC hDC = GetDC(hWnd);
	HDC hMemDC = CreateCompatibleDC(hDC);
	ReleaseDC(hWnd, hDC);

	HBITMAP hBitmap = CreateCompatibleBitmap(hMemDC, width, height);

	HGDIOBJ hOldBitmap = SelectObject(hMemDC, hBitmap);

	rc.left = rc.top = 0;
	rc.right = width;
	rc.bottom = height;

	FillRect(hMemDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
	
	//���Ʒ�������
	int jmax = (int)(height / g_unity) +1;
	int imax = (int)(width / g_unitx) +1; 
	for(j=0; j < jmax; j++)
	{
		for(i=0; i< imax; i++)
		{
			//����������������ƶ��������
			if(j%2 == 0)
				cx = (int)(g_unitx*i);
			else
				cx = (int)(g_unitx*(i+0.5));

			cy = (int)(g_unity * j);
			
			//�������ĵ㣬��ȡ�����ζ˵�
			GetCellPoints(cx, cy, g_CellSize, points);
			Polygon(hMemDC, points, 6);
		}
	}

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
	return hBitmap;
}

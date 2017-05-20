// MyPaint.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "MyPaint.h"
#include "atlstr.h"

//打开保存文件对话框  
#include<Commdlg.h>  

using namespace std;

#define MAX_LOADSTRING 100


// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

static int iWidth = 2;//线条宽度

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DlgProcIwidthInfo(HWND, UINT, WPARAM, LPARAM);



int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MYPAINT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	RegisterColorClass(hInstance);//注册自己写的颜色窗口
	RegisterStyleClass(hInstance);//注冊自己写的类型窗口

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPAINT));

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
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPAINT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MYPAINT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassEx(&wcex);
}

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
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

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
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
	//绘图
	static DRAWSTRU ds;
	static PDRAWFUN pMouseDown, pMouseUp, pMouseMove;
	HMENU hMenu = GetMenu(hWnd);

	//调色板
	static HPEN hPen, hOldPen;
	static HBRUSH hBrush;
	static COLORREF color;
	static HWND hWndColor;
	static BOOL bColorBoxIsDock;

	static HWND hWndStyle;
	static BOOL bStyleBoxIsDock;
	static int flag;

	int tempIwidth = 0;
	HPEN tempHpen;
	RECT rt;
	//save
	PBITMAPINFO pbmi;
	HBITMAP hBitmap = NULL;
	TCHAR filename[260] = TEXT("");
	USES_CONVERSION;
	switch (message)
	{
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
			//绘制线条
		/*case IDM_DRAW_PEN:
			pMouseDown = PenMouseDown;
			pMouseUp = PenMouseUp;
			pMouseMove = PenMouseMove;
			CheckMenuItem(hMenu, IDM_DRAW_PEN, MF_BYCOMMAND | MF_CHECKED);
			CheckMenuItem(hMenu, IDM_DRAW_LINE, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_DRAW_RECT, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_DRAW_ELLIPSE, MF_BYCOMMAND | MF_UNCHECKED);
			break;
		case IDM_DRAW_LINE:
			pMouseDown = LineMouseDown;
			pMouseUp = LineMouseUp;
			pMouseMove = LineMouseMove;
			CheckMenuItem(hMenu, IDM_DRAW_PEN, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_DRAW_LINE, MF_BYCOMMAND | MF_CHECKED);
			CheckMenuItem(hMenu, IDM_DRAW_RECT, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_DRAW_ELLIPSE, MF_BYCOMMAND | MF_UNCHECKED);
			break;
		case IDM_DRAW_RECT:
			pMouseDown = RectMouseDown;
			pMouseUp = RectMouseUp;
			pMouseMove = RectMouseMove;
			CheckMenuItem(hMenu, IDM_DRAW_PEN, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_DRAW_LINE, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_DRAW_RECT, MF_BYCOMMAND | MF_CHECKED);
			CheckMenuItem(hMenu, IDM_DRAW_ELLIPSE, MF_BYCOMMAND | MF_UNCHECKED);
			break;
		case IDM_DRAW_ELLIPSE:
			pMouseDown = EllipseMouseDown;
			pMouseUp = EllipseMouseUp;
			pMouseMove = EllipseMouseMove;
			CheckMenuItem(hMenu, IDM_DRAW_PEN, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_DRAW_LINE, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_DRAW_RECT, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_DRAW_ELLIPSE, MF_BYCOMMAND | MF_CHECKED);
			break;*/
		case IDM_IWIDTH:
			DialogBox(hInst, (LPCTSTR)IDD_DIALOG_LINEWIDTH, hWnd, (DLGPROC)DlgProcIwidthInfo);
			DeleteObject(hOldPen);//此处删除了 pen 所以DrawStyle.cpp中不需要再次删除
			hPen = CreatePen(PS_SOLID, iWidth, color);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case IDM_COLORBOX_ISDOCK:
			bColorBoxIsDock = TRUE;
			CheckMenuItem(hMenu, IDM_COLORBOX_ISDOCK, MF_BYCOMMAND | MF_CHECKED);
			CheckMenuItem(hMenu, IDM_COLORBOX_NOTDOCK, MF_BYCOMMAND | MF_UNCHECKED);
			DestroyWindow(hWndColor);
			hWndColor = CreateColorBox(hInst, hWnd, bColorBoxIsDock);
			break;
		case IDM_COLORBOX_NOTDOCK:
			bColorBoxIsDock = FALSE;
			CheckMenuItem(hMenu, IDM_COLORBOX_ISDOCK, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_COLORBOX_NOTDOCK, MF_BYCOMMAND | MF_CHECKED);
			DestroyWindow(hWndColor);
			hWndColor = CreateColorBox(hInst, hWnd, bColorBoxIsDock);
			break;
		case IDM_TOOLBOX_ISDOCK:
			bStyleBoxIsDock = TRUE;
			CheckMenuItem(hMenu, IDM_TOOLBOX_ISDOCK, MF_BYCOMMAND | MF_CHECKED);
			CheckMenuItem(hMenu, IDM_TOOLBOX_NOTDOCK, MF_BYCOMMAND | MF_UNCHECKED);
			DestroyWindow(hWndStyle);
			hWndStyle = CreateStyleBox(hInst, hWnd, bStyleBoxIsDock);
			break;
		case IDM_TOOLBOX_NOTDOCK:
			bStyleBoxIsDock = FALSE;
			CheckMenuItem(hMenu, IDM_TOOLBOX_ISDOCK, MF_BYCOMMAND | MF_UNCHECKED);
			CheckMenuItem(hMenu, IDM_TOOLBOX_NOTDOCK, MF_BYCOMMAND | MF_CHECKED);
			DestroyWindow(hWndStyle);
			hWndStyle = CreateStyleBox(hInst, hWnd, bStyleBoxIsDock);
			break;
		case IDM_SAVEAS_BMP:
			pbmi = CreateBitmapInfoStruct(hWnd, ds.hBitmap);
			hdc = GetDC(hWnd);
			_tcscpy(filename, SaveFile());
			CreateBMPFile(hWnd,filename, pbmi, ds.hBitmap, hdc);
			ReleaseDC(hWnd, hdc);
			break;
		case IDM_OPEN_BMP:
			hdc = GetDC(hWnd);
			_tcscpy(filename, OpenFile());
			ds.hBitmap = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			SelectObject(ds.hMemDc, ds.hBitmap);
			BitBlt(hdc, 0, 0, ds.iWidth, ds.iHeight, ds.hMemDc, 0, 0, SRCCOPY);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		/*SelectObject(ds.hMemDc, ds.hBitmap);
		BitBlt(hdc, 0, 0, ds.iWidth, ds.iHeight, ds.hMemDc, 0, 0, SRCCOPY);*/
		for (vector<int>::size_type i = 0; i < vec.size(); i++){
			switch (vec[i].drawType)
			{
			case line:
				hdc = GetDC(hWnd);
				SetROP2(hdc, R2_COPYPEN);
				DeleteObject(hOldPen);
				tempIwidth = vec[i].iwidth;
				tempHpen = CreatePen(PS_SOLID, tempIwidth, vec[i].color);
				
				hOldPen = tempHpen;
				SelectObject(hdc, tempHpen);
				MoveToEx(hdc, vec[i].pt1.x,vec[i].pt1.y, NULL);
				LineTo(hdc, vec[i].pt2.x, vec[i].pt2.y);
				ReleaseDC(hWnd,hdc);
				break;
			case rect:
				hdc = GetDC(hWnd);
				SetROP2(hdc, R2_COPYPEN);
				DeleteObject(hOldPen);
				tempIwidth = vec[i].iwidth;
				hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
				SelectObject(hdc, hBrush);
				tempHpen = CreatePen(PS_SOLID, tempIwidth, vec[i].color);
				hOldPen = tempHpen;
				SelectObject(hdc, tempHpen);
				Rectangle(hdc, vec[i].pt1.x, vec[i].pt1.y, vec[i].pt2.x, vec[i].pt2.y);
				DeleteObject(hBrush);
				ReleaseDC(hWnd, hdc);
				break;
			case ellipse:
				hdc = GetDC(hWnd);
				SetROP2(hdc, R2_COPYPEN);
				DeleteObject(hOldPen);
				tempIwidth = vec[i].iwidth;
				hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
				SelectObject(hdc, hBrush);
				tempHpen = CreatePen(PS_SOLID, tempIwidth, vec[i].color);
				hOldPen = tempHpen;
				SelectObject(hdc, tempHpen);
				Ellipse(hdc, vec[i].pt1.x, vec[i].pt1.y, vec[i].pt2.x, vec[i].pt2.y);
				DeleteObject(hBrush);
				ReleaseDC(hWnd, hdc);
				break;
			case fillEllipse:
				hdc = GetDC(hWnd);
				SetROP2(hdc, R2_COPYPEN);
				DeleteObject(hOldPen);
				tempIwidth = vec[i].iwidth;

				tempHpen = CreatePen(PS_SOLID, tempIwidth, vec[i].color);
				hOldPen = tempHpen;
				hBrush = CreateSolidBrush(vec[i].color);
				SelectObject(hdc, hBrush);
				SelectObject(hdc, tempHpen);
				Ellipse(hdc, vec[i].pt1.x, vec[i].pt1.y, vec[i].pt2.x, vec[i].pt2.y);
				DeleteObject(hBrush);
				ReleaseDC(hWnd, hdc);
				break;
			case fillRect:
				hdc = GetDC(hWnd);
				SetROP2(hdc, R2_COPYPEN);
				DeleteObject(hOldPen);
				tempIwidth = vec[i].iwidth;

				tempHpen = CreatePen(PS_SOLID, tempIwidth, vec[i].color);
				hOldPen = tempHpen;
				hBrush = CreateSolidBrush(vec[i].color);
				SelectObject(hdc, tempHpen);
				SelectObject(hdc, hBrush);
				Rectangle(hdc, vec[i].pt1.x, vec[i].pt1.y, vec[i].pt2.x, vec[i].pt2.y);
				DeleteObject(hBrush);
				ReleaseDC(hWnd, hdc);
				break;
			case earser:
				hdc = GetDC(hWnd);
				DeleteObject(hOldPen);
				tempIwidth = vec[i].iwidth;
				tempHpen = CreatePen(PS_SOLID, tempIwidth, RGB(255,255,255));

				hOldPen = tempHpen;
				SelectObject(hdc, tempHpen);
				MoveToEx(hdc, vec[i].pt1.x, vec[i].pt1.y, NULL);
				LineTo(hdc, vec[i].pt2.x, vec[i].pt2.y);
				ReleaseDC(hWnd, hdc);
				break;
			default:
				break;
			}
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		ReleaseDC(hWnd, ds.hMemDc);
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		pMouseDown = PenMouseDown;
		pMouseUp = PenMouseUp;
		pMouseMove = PenMouseMove;
		CheckMenuItem(hMenu, IDM_DRAW_PEN, MF_BYCOMMAND | MF_CHECKED);
		CheckMenuItem(hMenu, IDM_DRAW_LINE, MF_BYCOMMAND | MF_UNCHECKED);
		CheckMenuItem(hMenu, IDM_DRAW_RECT, MF_BYCOMMAND | MF_UNCHECKED);
		CheckMenuItem(hMenu, IDM_DRAW_ELLIPSE, MF_BYCOMMAND | MF_UNCHECKED);

		CheckMenuItem(hMenu, IDM_COLORBOX_ISDOCK, MF_BYCOMMAND | MF_UNCHECKED);
		CheckMenuItem(hMenu, IDM_COLORBOX_NOTDOCK, MF_BYCOMMAND | MF_CHECKED);
		CheckMenuItem(hMenu, IDM_TOOLBOX_ISDOCK, MF_BYCOMMAND | MF_UNCHECKED);
		CheckMenuItem(hMenu, IDM_TOOLBOX_NOTDOCK, MF_BYCOMMAND | MF_CHECKED);
		//调色板
		bColorBoxIsDock = FALSE;
		hWndColor = CreateColorBox(hInst, hWnd, bColorBoxIsDock);
		color = RGB(0, 0, 0);
		hPen = CreatePen(PS_SOLID, iWidth, color);
		hOldPen = hPen;

		//类型栏
		bStyleBoxIsDock = FALSE;
		hWndStyle = CreateStyleBox(hInst, hWnd, bStyleBoxIsDock);

		//save
		GetClientRect(hWnd, &rt);
		ds.iWidth = rt.right-rt.left;
		ds.iHeight = rt.bottom - rt.top;
		hdc = GetDC(hWnd);
		ds.hMemDc = CreateCompatibleDC(hdc);
		ds.hBitmap = CreateCompatibleBitmap(hdc, ds.iWidth, ds.iHeight);
		SelectObject(ds.hMemDc, ds.hBitmap);
		SelectObject(ds.hMemDc, GetStockObject(WHITE_BRUSH));
		SelectObject(ds.hMemDc, GetStockObject(WHITE_PEN));
		Rectangle(ds.hMemDc, 0, 0, ds.iWidth + 1, ds.iHeight + 1);
		SelectObject(ds.hMemDc, GetStockObject(WHITE_BRUSH));
		SelectObject(ds.hMemDc, GetStockObject(BLACK_PEN));
		break;
	case WM_SIZE:
		GetClientRect(hWnd, &rt);
		ds.iWidth = rt.right - rt.left;
		ds.iHeight = rt.bottom - rt.top;
		break;
	case WM_CHANGE_COLOR:
		color = (COLORREF)lParam;
		DeleteObject(hOldPen);//此处删除了 pen 所以DrawStyle.cpp中不需要再次删除
		hPen = CreatePen(PS_SOLID, iWidth, color);
		hOldPen = hPen;
		break;
	case WM_CHANGE_STYLE:
		flag = (int)lParam;
		switch (flag)
		{
		case IDB_U_PEN:
			pMouseDown = PenMouseDown;
			pMouseUp = PenMouseUp;
			pMouseMove = PenMouseMove;
			break;
		case IDB_U_LINE:
			pMouseDown = LineMouseDown;
			pMouseUp = LineMouseUp;
			pMouseMove = LineMouseMove;
			break;
		case IDB_UB_CIRCLE:
			pMouseDown = EllipseMouseDown;
			pMouseUp = EllipseMouseUp;
			pMouseMove = EllipseMouseMove;
			break;
		case IDB_UB_RECT:
			pMouseDown = RectMouseDown;
			pMouseUp = RectMouseUp;
			pMouseMove = RectMouseMove;
			break;
		case IDB_UF_CIRCLE:
			pMouseDown = FillEllipseMouseDown;
			pMouseUp = FillEllipseMouseUp;
			pMouseMove = FillEllipseMouseMove;
			break;
		case IDB_UF_RECT:
			pMouseDown = FillRectMouseDown;
			pMouseUp = FillRectMouseUp;
			pMouseMove = FillRectMouseMove;
			break;
		case IDB_U_EARSER:
			pMouseDown = EarserMouseDown;
			pMouseUp = EarserMouseUp;
			pMouseMove = EarserMouseMove;
			break;
		case IDB_U_CUT:
			pMouseDown = CutMouseDown;
			pMouseUp = CutMouseUp;
			pMouseMove = CutMouseMove;
			break;;
		default:
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		pMouseDown(&ds, hWnd, wParam, lParam, hPen, color,iWidth);
		break;
	case WM_LBUTTONUP:
		pMouseUp(&ds, hWnd, wParam, lParam, hPen, color, iWidth);
		break;
	case WM_MOUSEMOVE:
		pMouseMove(&ds, hWnd, wParam, lParam, hPen, color, iWidth);
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

// 线条宽度框
LRESULT CALLBACK DlgProcIwidthInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR szIWidth[50] = TEXT("");
	char* chRtn = "";
	char a[50] = "";
	int iLen = 0;
	USES_CONVERSION;
	switch (message)
	{
	case WM_INITDIALOG:
		_itoa(iWidth, a, 10);
		_tcscpy(szIWidth, A2T(a));
		SetDlgItemText(hDlg, IDC_LINEWIDTH, szIWidth);

		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK:
			GetDlgItemText(hDlg, IDC_LINEWIDTH, szIWidth, 20);
			iLen = 2 * wcslen(szIWidth);//CString,TCHAR汉字算一个字符，因此不用普通计算长度 
			chRtn = new char[iLen + 1];
			wcstombs(chRtn, szIWidth, iLen + 1);//转换成功返回为非负值 
			iWidth = atoi(chRtn);
				EndDialog(hDlg, 1);
			break;
		case IDC_CANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return FALSE;
}


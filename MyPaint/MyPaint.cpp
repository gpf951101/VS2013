// MyPaint.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "MyPaint.h"
#include "atlstr.h"

//打开保存文件对话框  
#include<Commdlg.h>  

using namespace std;

#define MAX_LOADSTRING 100
#define MSG_CUT WM_USER + 10

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

static int iWidth = 2;//线条宽度
bool isBegin = false;
bool isFinish = false;


// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DlgProcIwidthInfo(HWND, UINT, WPARAM, LPARAM);
BOOL WINAPI DlgProc2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HBITMAP CopyScreenToBitmap(LPRECT lpRect,HWND hWnd);

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
	if (!InitInstance(hInstance, nCmdShow))
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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPAINT));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_MYPAINT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
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
	HBITMAP oldHbmp;
	//save
	PBITMAPINFO pbmi;
	HBITMAP hBitmap = NULL;
	TCHAR filename[260] = TEXT("");
	HWND hwnd2;
	USES_CONVERSION;
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
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
			if (_tcscmp(filename, L"") != 0){
				CreateBMPFile(hWnd, filename, pbmi, ds.hBitmap, hdc);
			}
			ReleaseDC(hWnd, hdc);
			break;
		case IDM_OPEN_BMP:
			hdc = GetDC(hWnd);
			_tcscpy(filename, OpenFile());
			if (_tcscmp(filename, L"") != 0){
				ds.hBitmap = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				SelectObject(ds.hMemDc, ds.hBitmap);
				BitBlt(hdc, 0, 0, ds.iWidth, ds.iHeight, ds.hMemDc, 0, 0, SRCCOPY);
			}
			break;
		case IDM_CLEAR:
			vec.clear();
			SelectObject(ds.hMemDc, ds.hBitmap);
			SelectObject(ds.hMemDc, GetStockObject(WHITE_BRUSH));
			SelectObject(ds.hMemDc, GetStockObject(WHITE_PEN));
			GetClientRect(hWnd, &rt);
			Rectangle(ds.hMemDc, rt.left, rt.top, rt.right, rt.bottom);
			SelectObject(ds.hMemDc, GetStockObject(WHITE_BRUSH));
			SelectObject(ds.hMemDc, GetStockObject(BLACK_PEN));
			InvalidateRect(hWnd, NULL, true);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_HOTKEY:
		if (wParam == MSG_CUT)
		{
			HWND hwnd2 = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG2), NULL, (DLGPROC)&DlgProc2);
			ShowWindow(hwnd2, SW_MAXIMIZE);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SelectObject(ds.hMemDc, ds.hBitmap);
		BitBlt(hdc, 0, 0, ds.iWidth, ds.iHeight, ds.hMemDc, 0, 0, SRCCOPY);
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
				MoveToEx(hdc, vec[i].pt1.x, vec[i].pt1.y, NULL);
				LineTo(hdc, vec[i].pt2.x, vec[i].pt2.y);
				ReleaseDC(hWnd, hdc);
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
				tempHpen = CreatePen(PS_SOLID, tempIwidth, RGB(255, 255, 255));

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
		RegisterHotKey(hWnd, MSG_CUT, MOD_CONTROL | MOD_ALT, 88); //Ctrl + Alt + X
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
		ds.iWidth = rt.right - rt.left;
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
			hwnd2 = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG2), NULL, (DLGPROC)&DlgProc2);
			ShowWindow(hwnd2, SW_MAXIMIZE);
			/*pMouseDown = CutMouseDown;
			pMouseUp = CutMouseUp;
			pMouseMove = CutMouseMove;*/
			break;;
		default:
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		pMouseDown(&ds, hWnd, wParam, lParam, hPen, color, iWidth);
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
bool isLeftDown = false;
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
BOOL WINAPI DlgProc2(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC				hdc;
	HPEN			hPen;
	static POINT	pointBegin, pointEnd;

	switch (uMsg)
	{
	case WM_RBUTTONDOWN:
		if (isBegin == false)
			EndDialog(hWnd, 0);
		else
		{
			InvalidateRect(hWnd, NULL, TRUE);
			isBegin = false;
			isFinish = false;
		}
		break;

	case WM_LBUTTONDOWN:			//左键按下
		if (isFinish == false)		//避免双击截图时重新获取坐标
		{
			pointBegin.x = LOWORD(lParam);	//矩形起点x
			pointBegin.y = HIWORD(lParam);	//矩形起点y
			pointEnd.x = LOWORD(lParam);
			pointEnd.y = HIWORD(lParam);
			isLeftDown = true;
		}
		break;

	case WM_LBUTTONUP:				//左键弹起
		if (isBegin == true)			//有拖动才为true，说明矩形非空
		{
			isFinish = true;
		}
		break;

	case WM_LBUTTONDBLCLK:
	{
		if (isFinish == true)	//如果已选取完成
		{
		RECT rect;
		rect.left = (pointBegin.x < pointEnd.x) ? pointBegin.x : pointEnd.x;
		rect.top = (pointBegin.y < pointEnd.y) ? pointBegin.y : pointEnd.y;
		rect.right = (pointEnd.x > pointBegin.x) ? pointEnd.x : pointBegin.x;
		rect.bottom = (pointEnd.y > pointBegin.y) ? pointEnd.y : pointBegin.y;

		HBITMAP	 hBitmap;
		hBitmap = CopyScreenToBitmap(&rect,hWnd);

		if (OpenClipboard(hWnd))
		{
		//清空剪贴板
		EmptyClipboard();

		//把屏幕内容粘贴到剪贴板上
		SetClipboardData(CF_BITMAP, hBitmap);

		//关闭剪贴板
			 CloseClipboard();
		}
		 isBegin = false;
		 isFinish = false;
		}
	EndDialog(hWnd, 0);
	}
		break;

	case WM_MOUSEMOVE:					//进入鼠标移动消息
		if (wParam & MK_LBUTTON)			//检测左键是否按下
		{
			if (isFinish == false && isLeftDown == true)		//禁止二次截图
			{
				HWND other= GetDesktopWindow();
				hdc = GetDC(other);
				SetROP2(hdc, R2_NOTXORPEN);   //需这句才能消除内部残影

				hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));	//画笔
				SelectObject(hdc, hPen);
				SelectObject(hdc, GetStockObject(NULL_BRUSH));	//画刷

				Rectangle(hdc, pointBegin.x, pointBegin.y, pointEnd.x, pointEnd.y);	//需这句才能消除外部残影,前提是有SetROP2
				pointEnd.x = LOWORD(lParam);
				pointEnd.y = HIWORD(lParam);
				Rectangle(hdc, pointBegin.x, pointBegin.y, pointEnd.x, pointEnd.y);

				DeleteObject(hPen);                 //删除画笔
				ReleaseDC(other, hdc);

				isBegin = true;
			}
		}
		break;

	case WM_INITDIALOG:
		Transparent(hWnd, 10);
		return TRUE;

	case WM_DESTROY:
		PostQuitMessage(WM_QUIT);
		return TRUE;

	default:
		return FALSE;
	}
	return TRUE;
}
HBITMAP CopyScreenToBitmap(LPRECT lpRect, HWND hWnd)
{
	HDC       hScrDC, hMemDC;               //屏幕和内存设备描述表
	HBITMAP   hBitmap, hOldBitmap;			//位图句柄
	int       nX, nY, nX2, nY2;				//选定区域坐标
	int       nWidth, nHeight;				//位图宽度和高度
	int       xScrn, yScrn;                 //屏幕分辨率

	//确保选定区域不为空矩形
	if (IsRectEmpty(lpRect))
		return NULL;

	//为屏幕创建设备描述表
	hScrDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);

	//为屏幕设备描述表创建兼容的内存设备描述表
	hMemDC = CreateCompatibleDC(hScrDC);

	//获得选定区域坐标
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;

	//获得屏幕分辨率
	xScrn = GetDeviceCaps(hScrDC, HORZRES);
	yScrn = GetDeviceCaps(hScrDC, VERTRES);

	//确保选定区域是可见的
	if (nX < 0)
		nX = 0;

	if (nY < 0)
		nY = 0;

	if (nX2 > xScrn)
		nX2 = xScrn;

	if (nY2 > yScrn)
		nY2 = yScrn;

	nWidth = nX2 - nX;
	nHeight = nY2 - nY;

	//创建一个与屏幕设备描述表兼容的位图
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);

	//把新位图选到内存设备描述表中
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	//把屏幕设备描述表拷贝到内存设备描述表中
	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY);

	//得到屏幕位图的句柄
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	TCHAR filename[260] = TEXT("");
	PBITMAPINFO pbmi = CreateBitmapInfoStruct(hWnd, hBitmap);
	HDC hdc = GetDC(hWnd);
	_tcscpy(filename, SaveFile());
	if (_tcscmp(filename, L"") != 0){
		CreateBMPFile(hWnd, filename, pbmi, hBitmap, hdc);
	}
	ReleaseDC(hWnd, hdc);
	//清除
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);

	//   返回位图句柄
	return   hBitmap;
}


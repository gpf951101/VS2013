// MyPaint.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MyPaint.h"

using namespace std;

#define MAX_LOADSTRING 100


// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);



int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MYPAINT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	RegisterColorClass(hInstance);//ע���Լ�д����ɫ����
	RegisterStyleClass(hInstance);//ע���Լ�д�����ʹ���

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPAINT));

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
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
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
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
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

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
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
	//��ͼ
	static DRAWSTRU ds;
	static PDRAWFUN pMouseDown, pMouseUp, pMouseMove;
	HMENU hMenu = GetMenu(hWnd);

	//��ɫ��
	static HPEN hPen, hOldPen;
	static HBRUSH hBrush;
	static COLORREF color;
	static INT iWidth;
	static HWND hWndColor;
	static BOOL bColorBoxIsDock;

	static HWND hWndStyle;
	static BOOL bStyleBoxIsDock;
	static int flag;

	switch (message)
	{
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
			//��������
		case IDM_DRAW_PEN:
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
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (vector<int>::size_type i = 0; i < vec.size(); i++){
			switch (vec[i].drawType)
			{
			case line:
				hdc = GetDC(hWnd);
				SetROP2(hdc, R2_COPYPEN);
				DeleteObject(hOldPen);
				hPen = CreatePen(PS_SOLID, iWidth, vec[i].color);
				
				hOldPen = hPen;
				SelectObject(hdc, hPen);
				MoveToEx(hdc, vec[i].pt1.x,vec[i].pt1.y, NULL);
				LineTo(hdc, vec[i].pt2.x, vec[i].pt2.y);
				ReleaseDC(hWnd,hdc);
				break;
			case rect:
				hdc = GetDC(hWnd);
				SetROP2(hdc, R2_COPYPEN);
				DeleteObject(hOldPen);
				hPen = CreatePen(PS_SOLID, iWidth, vec[i].color);
				hOldPen = hPen;
				SelectObject(hdc, hPen);
				Rectangle(hdc, vec[i].pt1.x, vec[i].pt1.y, vec[i].pt2.x, vec[i].pt2.y);
				ReleaseDC(hWnd, hdc);
				break;
			case ellipse:
				hdc = GetDC(hWnd);
				SetROP2(hdc, R2_COPYPEN);
				DeleteObject(hOldPen);
				hPen = CreatePen(PS_SOLID, iWidth, vec[i].color);
				hOldPen = hPen;
				SelectObject(hdc, hPen);
				Ellipse(hdc, vec[i].pt1.x, vec[i].pt1.y, vec[i].pt2.x, vec[i].pt2.y);
				ReleaseDC(hWnd, hdc);
				break;
			case fillEllipse:
				hdc = GetDC(hWnd);
				SetROP2(hdc, R2_COPYPEN);
				DeleteObject(hOldPen);
				hPen = CreatePen(PS_SOLID, iWidth, vec[i].color);
				hOldPen = hPen;
				hBrush = CreateSolidBrush(vec[i].color);
				SelectObject(hdc, hBrush);
				SelectObject(hdc, hPen);
				Ellipse(hdc, vec[i].pt1.x, vec[i].pt1.y, vec[i].pt2.x, vec[i].pt2.y);
				DeleteObject(hBrush);
				ReleaseDC(hWnd, hdc);
				break;
			case fillRect:
				hdc = GetDC(hWnd);
				SetROP2(hdc, R2_COPYPEN);
				DeleteObject(hOldPen);
				hPen = CreatePen(PS_SOLID, iWidth, vec[i].color);
				hOldPen = hPen;
				hBrush = CreateSolidBrush(vec[i].color);
				SelectObject(hdc, hPen);
				SelectObject(hdc, hBrush);
				Rectangle(hdc, vec[i].pt1.x, vec[i].pt1.y, vec[i].pt2.x, vec[i].pt2.y);
				DeleteObject(hBrush);
				ReleaseDC(hWnd, hdc);
				break;
			default:
				break;
			}
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
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

		//��ɫ��
		bColorBoxIsDock = FALSE;
		hWndColor = CreateColorBox(hInst, hWnd, bColorBoxIsDock);
		color = RGB(0, 0, 0);
		iWidth = 2;
		hPen = CreatePen(PS_SOLID, iWidth, color);
		hOldPen = hPen;

		//������
		bStyleBoxIsDock = FALSE;
		hWndStyle = CreateStyleBox(hInst, hWnd, bStyleBoxIsDock);
		break;
	case WM_CHANGE_COLOR:
		color = (COLORREF)lParam;
		DeleteObject(hOldPen);//�˴�ɾ���� pen ����DrawStyle.cpp�в���Ҫ�ٴ�ɾ��
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
		default:
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		pMouseDown(&ds, hWnd, wParam, lParam, hPen, color);
		break;
	case WM_LBUTTONUP:
		pMouseUp(&ds, hWnd, wParam, lParam, hPen, color);
		break;
	case WM_MOUSEMOVE:
		pMouseMove(&ds, hWnd, wParam, lParam, hPen, color);
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


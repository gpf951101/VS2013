#include"stdafx.h"
#include"DrawStyleBox.h"

static TCHAR szStyleBtnClass[] = TEXT("StyleBtn");
static TCHAR szStyleBoxClass[] = TEXT("StyleBox");
int styleId[12] = {
	IDB_U_PEN, IDB_D_PEN,
	IDB_U_LINE, IDB_D_LINE,
	IDB_UB_RECT, IDB_DB_RECT,
	IDB_UB_CIRCLE, IDB_DB_CIRCLE,
	IDB_UF_RECT, IDB_DF_RECT,
	IDB_UF_CIRCLE, IDB_DF_CIRCLE
};
static int flag;
ATOM RegisterStyleClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.hIconSm = NULL;
	wcex.lpszClassName = szStyleBtnClass;
	wcex.lpszMenuName = NULL;
	wcex.lpfnWndProc = WndStyleBtnProc;
	wcex.cbWndExtra = 4;
	if (!RegisterClassEx(&wcex))
		return 0;
	wcex.lpszClassName = szStyleBoxClass;
	wcex.lpszMenuName = NULL;
	wcex.lpfnWndProc = WndStyleBoxProc;
	wcex.cbWndExtra = 0;
	if (!RegisterClassEx(&wcex))
		return 0;
	return 1;
}
static BOOL isDown[6] = { TRUE, FALSE, FALSE, FALSE, FALSE, FALSE };
LRESULT CALLBACK WndStyleBtnProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;
	static HBITMAP hBitmap1, hBitmap2;
	static HDC hDcMem;
	static HWND hWndSendTo;
	switch (message)
	{
	case WM_CREATE:
		flag = (int)((LPCREATESTRUCT)lParam)->lpCreateParams;
		SetWindowLong(hWnd, 0, flag);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		flag = (int)GetWindowLong(hWnd, 0);
		int temp;
		for (int i = 0; i < 12; i++){
			if (flag == styleId[i]){
				temp = i / 2;
				break;
			}
		}
		HINSTANCE hInst;
		hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
		hDcMem = CreateCompatibleDC(hdc);
		if (isDown[temp] == TRUE){
			hBitmap1 = LoadBitmap(hInst, MAKEINTRESOURCE(styleId[temp*2+1]));
		}
		else{
			hBitmap1 = LoadBitmap(hInst, MAKEINTRESOURCE(flag));
		}
		SelectObject(hDcMem, hBitmap1);
		BitBlt(hdc, 0, 0, 28, 28, hDcMem, 0, 0, SRCCOPY);
		ReleaseDC(hWnd, hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		flag = (int)GetWindowLong(hWnd, 0);
		SendMessage(GetParent(hWnd),
			WM_CHANGE_STYLE, (WPARAM)0, (LPARAM)flag);
		int temp2;
		for (int i = 0; i < 12; i++){
			if (flag == styleId[i]){
				temp2 = i / 2;
				isDown[temp2] = TRUE;
				break;
			}
		}
		for (int i = 0; i < 6; i++){
			if (i != temp2)
				isDown[i] = FALSE;
		}
		InvalidateRect(hWndSendTo, NULL, true);
		break;
	case WM_DESTROY:
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
LRESULT CALLBACK WndStyleBoxProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	int i;
	HWND hWndStyle;
	static HWND hWndSendTo;
	switch (message)
	{
	case WM_CREATE:
		hWndSendTo = (HWND)(((LPCREATESTRUCT)(lParam))
			->lpCreateParams);
		HINSTANCE hInsthWnd;
		hInsthWnd = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
		for (i = 0; i < 12; i = i+2)
		{
			hWndStyle = CreateWindow(szStyleBtnClass, NULL,
				WS_CHILD, 0, 14 * i, 28, 28, hWnd, NULL, hInsthWnd, (LPVOID)styleId[i]);
			ShowWindow(hWndStyle, SW_NORMAL);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_CHANGE_STYLE:
		SendMessage(hWndSendTo, message, wParam, lParam);
		break;
	case WM_DESTROY:
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
HWND CreateStyleBox(HINSTANCE hInst, HWND hWnd, BOOL isDock)
{
	RECT rt;
	POINT pt1, pt2;
	HWND hWndColorBox;
	if (isDock)
	{
		SetRect(&rt, 0, 0, 28, 168);
		AdjustWindowRect(&rt, WS_CHILD | WS_VISIBLE | WS_BORDER, false);
		hWndColorBox =
			CreateWindow(szStyleBoxClass,
			NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			rt.left, rt.top, rt.right - rt.left,
			rt.bottom - rt.top, hWnd, NULL, hInst,
			(LPVOID)hWnd);
	}
	else
	{

		pt1.x = 100, pt1.y = 100;
		pt2.x = 128, pt2.y = 268;
		ClientToScreen(hWnd, &pt1);
		ClientToScreen(hWnd, &pt2);

		SetRect(&rt, pt1.x, pt1.y, pt2.x, pt2.y);

		AdjustWindowRect(&rt, WS_POPUP | WS_VISIBLE | WS_BORDER | WS_CAPTION, false);
		hWndColorBox =
			CreateWindow(szStyleBoxClass,
			NULL, WS_POPUP | WS_VISIBLE | WS_BORDER | WS_CAPTION,
			rt.left, rt.top, rt.right - rt.left,
			rt.bottom - rt.top, hWnd, NULL, hInst,
			(LPVOID)hWnd);
	}
	return hWndColorBox;
}

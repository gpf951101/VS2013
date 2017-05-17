#include"stdafx.h"
#include"ColorBox.h"
static TCHAR szColorBtnClass[] = TEXT("ColorBtn");
static TCHAR szColorBoxClass[] = TEXT("ColorBox");
ATOM RegisterColorClass(HINSTANCE hInstance)
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
	wcex.lpszClassName = szColorBtnClass;
	wcex.lpszMenuName = NULL;
	wcex.lpfnWndProc = WndColorBtnProc;
	wcex.cbWndExtra = 4;
	if (!RegisterClassEx(&wcex))
		return 0;
	wcex.lpszClassName = szColorBoxClass;
	wcex.lpszMenuName = NULL;
	wcex.lpfnWndProc = WndColorBoxProc;
	wcex.cbWndExtra = 0;
	if (!RegisterClassEx(&wcex))
		return 0;
	return 1;
}
LRESULT CALLBACK WndColorBtnProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	COLORREF color;
	HBRUSH hBr, hOldBr;
	RECT rt;
	switch (message)
	{
	case WM_CREATE:
		color = (COLORREF)((LPCREATESTRUCT)lParam)->lpCreateParams;
		SetWindowLong(hWnd, 0, color);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rt);
		color = (COLORREF)GetWindowLong(hWnd, 0);
		hBr = CreateSolidBrush(color);
		hOldBr = (HBRUSH)SelectObject(hdc, hBr);
		Rectangle(hdc, 0, 0, rt.right, rt.bottom);
		// TODO: 在此添加任意绘图代码...
		SelectObject(hdc, hOldBr);
		DeleteObject(hBr);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		color = (COLORREF)GetWindowLong(hWnd, 0);
		SendMessage(GetParent(hWnd),
			WM_CHANGE_COLOR, (WPARAM)0, (LPARAM)color);
		break;
	case WM_DESTROY:
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
LRESULT CALLBACK WndColorBoxProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	int i;
	HWND hWndColor;
	static COLORREF aColor[6] = {
		RGB(0, 0, 0), RGB(255, 0, 0),
		RGB(0, 255, 0), RGB(0, 0, 255),
		RGB(255, 255, 0), RGB(255, 0, 255),
	};
	static HWND hWndSendTo;
	switch (message)
	{
	case WM_CREATE:
		hWndSendTo = (HWND)(((LPCREATESTRUCT)(lParam))
			->lpCreateParams);
		HINSTANCE hInsthWnd;
		hInsthWnd = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
		for (i = 0; i < 5; i++)
		{
			hWndColor = CreateWindow(szColorBtnClass, NULL,
				WS_CHILD, i * 40, 0, 40, 40, hWnd, NULL, hInsthWnd, (LPVOID)aColor[i]);
			ShowWindow(hWndColor, SW_NORMAL);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_CHANGE_COLOR:
		SendMessage(hWndSendTo, message, wParam, lParam);
		break;
	case WM_DESTROY:
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
HWND CreateColorBox(HINSTANCE hInst, HWND hWnd, BOOL isDock)
{
	RECT rt;
	POINT pt1, pt2;
	HWND hWndColorBox;
	if (isDock)
	{
		SetRect(&rt, 0, 0, 200, 40);
		AdjustWindowRect(&rt, WS_CHILD | WS_VISIBLE | WS_BORDER, false);
		hWndColorBox =
			CreateWindow(szColorBoxClass,
			NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			rt.left, rt.top, rt.right - rt.left,
			rt.bottom - rt.top, hWnd, NULL, hInst,
			(LPVOID)hWnd);
	}
	else
	{

		pt1.x = 400, pt1.y = 0;
		pt2.x = 600, pt2.y = 40;
		ClientToScreen(hWnd, &pt1);
		ClientToScreen(hWnd, &pt2);

		SetRect(&rt, pt1.x, pt1.y, pt2.x, pt2.y);

		AdjustWindowRect(&rt, WS_POPUP | WS_VISIBLE | WS_BORDER | WS_CAPTION, false);
		hWndColorBox =
			CreateWindow(szColorBoxClass,
			NULL, WS_POPUP | WS_VISIBLE | WS_BORDER | WS_CAPTION,
			rt.left, rt.top, rt.right - rt.left,
			rt.bottom - rt.top, hWnd, NULL, hInst,
			(LPVOID)hWnd);
	}
	return hWndColorBox;
}

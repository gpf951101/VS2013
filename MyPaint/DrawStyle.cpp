#include "stdafx.h"
#include "DrawStyle.h"
#include <windowsx.h>
#include <winuser.h>

//重绘
vector<node> vec;
node tempNode;

cutBmp cutbmp;
//绘图
VOID LineMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	pDs->ptStart.x = GET_X_LPARAM(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);

	tempNode.pt1 = pDs->ptStart;
	tempNode.color = color;
	tempNode.drawType = line;
	tempNode.iwidth = iwidth;

	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
	pDs->bMouseDown = TRUE;
	SetCapture(hWnd);
}
VOID LineMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	HDC hdc;
	pDs->bMouseDown = FALSE;
	ReleaseCapture();
	hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOT);
	HPEN newHpen = CreatePen(PS_SOLID, iwidth, color);
	SelectObject(hdc, newHpen);
	MoveToEx(hdc, pDs->ptStart.x, pDs->ptStart.y, NULL);
	LineTo(hdc, pDs->ptEnd.x, pDs->ptEnd.y);
	pDs->ptEnd.x = GET_X_LPARAM(lParam);
	pDs->ptEnd.y = GET_Y_LPARAM(lParam);
	SetROP2(hdc, R2_COPYPEN);
	MoveToEx(hdc, pDs->ptStart.x, pDs->ptStart.y, NULL);
	LineTo(hdc, pDs->ptEnd.x, pDs->ptEnd.y);
	DeleteObject(newHpen);
	ReleaseDC(hWnd, hdc);

	SelectObject(pDs->hMemDc, GetStockObject(NULL_BRUSH));
	SelectObject(pDs->hMemDc, newHpen);
	MoveToEx(pDs->hMemDc, pDs->ptStart.x, pDs->ptStart.y, NULL);
	LineTo(pDs->hMemDc, pDs->ptEnd.x, pDs->ptEnd.y);
	
	tempNode.pt2 = pDs->ptEnd;
	vec.push_back(tempNode);
}
VOID LineMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	HDC hdc;
	if (pDs->bMouseDown)
	{
		hdc = GetDC(hWnd);
		SetROP2(hdc, R2_NOTXORPEN);
		HPEN newHpen = CreatePen(PS_SOLID, iwidth, color);
		SelectObject(hdc, newHpen);
		MoveToEx(hdc, pDs->ptStart.x, pDs->ptStart.y, NULL);
		LineTo(hdc, pDs->ptEnd.x, pDs->ptEnd.y);
		pDs->ptEnd.x = GET_X_LPARAM(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);
		MoveToEx(hdc, pDs->ptStart.x, pDs->ptStart.y, NULL);
		LineTo(hdc, pDs->ptEnd.x, pDs->ptEnd.y);
		DeleteObject(newHpen);
		ReleaseDC(hWnd, hdc);
	}
}

VOID RectMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	pDs->ptStart.x = GET_X_LPARAM(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);

	tempNode.pt1 = pDs->ptStart;
	tempNode.color = color;
	tempNode.drawType = rect;
	tempNode.iwidth = iwidth;

	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
	pDs->bMouseDown = TRUE;
	SetCapture(hWnd);
}
VOID RectMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	HDC hdc;
	if (pDs->bMouseDown)
	{
		hdc = GetDC(hWnd);
		SetROP2(hdc, R2_NOTXORPEN);
		HPEN newHpen = CreatePen(PS_SOLID, iwidth, color);
		SelectObject(hdc, newHpen);
		Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		pDs->ptEnd.x = GET_X_LPARAM(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);
		Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		DeleteObject(newHpen);
		ReleaseDC(hWnd, hdc);
	}
}
VOID RectMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	HDC hdc;
	pDs->bMouseDown = FALSE;
	ReleaseCapture();
	hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOTXORPEN);
	HPEN newHpen = CreatePen(PS_SOLID, iwidth, color);
	SelectObject(hdc, newHpen);
	Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	pDs->ptEnd.x = GET_X_LPARAM(lParam);
	pDs->ptEnd.y = GET_Y_LPARAM(lParam);
	SetROP2(hdc, R2_COPYPEN);
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	SelectObject(hdc, hBrush);
	Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	DeleteObject(newHpen);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hdc);

	SelectObject(pDs->hMemDc, GetStockObject(NULL_BRUSH));
	SelectObject(pDs->hMemDc, newHpen);
	Rectangle(pDs->hMemDc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);

	tempNode.pt2 = pDs->ptEnd;
	vec.push_back(tempNode);
}

VOID EllipseMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	pDs->ptStart.x = GET_X_LPARAM(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);

	tempNode.pt1 = pDs->ptStart;
	tempNode.color = color;
	tempNode.drawType = ellipse;
	tempNode.iwidth = iwidth;

	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
	pDs->bMouseDown = TRUE;
	SetCapture(hWnd);
}
VOID EllipseMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	HDC hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOTXORPEN);
	HPEN newHpen = CreatePen(PS_SOLID, iwidth, color);
	SelectObject(hdc, newHpen);
	Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	pDs->bMouseDown = FALSE;
	ReleaseCapture();
	pDs->ptEnd.x = GET_X_LPARAM(lParam);
	pDs->ptEnd.y = GET_Y_LPARAM(lParam);
	SetROP2(hdc, R2_COPYPEN);
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	SelectObject(hdc, hBrush);
	Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	DeleteObject(newHpen);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hdc);

	SelectObject(pDs->hMemDc, GetStockObject(NULL_BRUSH));
	SelectObject(pDs->hMemDc, newHpen);
	Ellipse(pDs->hMemDc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);

	tempNode.pt2 = pDs->ptEnd;
	vec.push_back(tempNode);
}
VOID EllipseMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	if (pDs->bMouseDown)
	{
		HDC hdc = GetDC(hWnd);
		HPEN newHpen = CreatePen(PS_SOLID, iwidth, color);
		SelectObject(hdc, newHpen);
		SetROP2(hdc, R2_NOTXORPEN);
		Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		pDs->ptEnd.x = GET_X_LPARAM(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);
		Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		DeleteObject(newHpen);
		ReleaseDC(hWnd, hdc);
	}
}

VOID PenMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	pDs->ptStart.x = GET_X_LPARAM(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);

	tempNode.pt1 = pDs->ptStart;
	tempNode.color = color;
	tempNode.drawType = line;
	tempNode.iwidth = iwidth;

	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
	pDs->bMouseDown = TRUE;
	SetCapture(hWnd);
}
VOID PenMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	pDs->bMouseDown = FALSE;
	ReleaseCapture();
	tempNode.pt2 = pDs->ptEnd;
	vec.push_back(tempNode);
}
VOID PenMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	if (pDs->bMouseDown){
		HDC hdc;	
		hdc = GetDC(hWnd);
		HPEN newHpen = CreatePen(PS_SOLID, iwidth, color);
		SelectObject(hdc, newHpen);
		pDs->ptEnd.x = GET_X_LPARAM(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);

		tempNode.pt2 = pDs->ptEnd;
		vec.push_back(tempNode);

		MoveToEx(hdc, pDs->ptStart.x, pDs->ptStart.y, NULL);
		LineTo(hdc, pDs->ptEnd.x, pDs->ptEnd.y);

		SelectObject(pDs->hMemDc, GetStockObject(NULL_BRUSH));
		SelectObject(pDs->hMemDc, newHpen);
		MoveToEx(pDs->hMemDc, pDs->ptStart.x, pDs->ptStart.y, NULL);
		LineTo(pDs->hMemDc, pDs->ptEnd.x, pDs->ptEnd.y);

		pDs->ptStart.x = pDs->ptEnd.x;
		pDs->ptStart.y = pDs->ptEnd.y;

		tempNode.pt1 = pDs->ptStart;
		tempNode.color = color;
		tempNode.drawType = line;
		DeleteObject(newHpen);
		ReleaseDC(hWnd, hdc);
	}
}

VOID FillRectMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	HDC hdc = GetDC(hWnd);
	HBRUSH hBrush = CreateSolidBrush(color);
	SelectObject(hdc, hBrush);
	pDs->ptStart.x = GET_X_LPARAM(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);

	tempNode.pt1 = pDs->ptStart;
	tempNode.color = color;
	tempNode.drawType = fillRect;
	tempNode.iwidth = iwidth;

	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
	pDs->bMouseDown = TRUE;
	SetCapture(hWnd);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hdc);
}
VOID FillRectMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	HDC hdc;
	pDs->bMouseDown = FALSE;
	ReleaseCapture();
	hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOT);
	HBRUSH hBrush = CreateSolidBrush(color);
	HPEN newHpen = CreatePen(PS_SOLID, iwidth, color);
	SelectObject(hdc, newHpen);
	SelectObject(hdc, hBrush);
	Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	pDs->ptEnd.x = GET_X_LPARAM(lParam);
	pDs->ptEnd.y = GET_Y_LPARAM(lParam);
	SetROP2(hdc, R2_COPYPEN);
	Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);

	SelectObject(pDs->hMemDc, hBrush);
	SelectObject(pDs->hMemDc, newHpen);
	Rectangle(pDs->hMemDc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);

	DeleteObject(newHpen);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hdc);

	tempNode.pt2 = pDs->ptEnd;
	vec.push_back(tempNode);
}
VOID FillRectMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	HDC hdc;
	if (pDs->bMouseDown)
	{
		hdc = GetDC(hWnd);
		SetROP2(hdc, R2_NOTXORPEN);
		HBRUSH hBrush = CreateSolidBrush(color);
		HPEN newHpen = CreatePen(PS_SOLID, iwidth, color);
		SelectObject(hdc, newHpen);
		SelectObject(hdc, hBrush);
		Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		pDs->ptEnd.x = GET_X_LPARAM(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);
		Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		DeleteObject(hBrush);
		DeleteObject(newHpen);
		ReleaseDC(hWnd, hdc);
	}
}

VOID FillEllipseMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	HDC hdc = GetDC(hWnd);
	HBRUSH hBrush = CreateSolidBrush(color);
	SelectObject(hdc, hBrush);
	pDs->ptStart.x = GET_X_LPARAM(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);

	tempNode.pt1 = pDs->ptStart;
	tempNode.color = color;
	tempNode.drawType = fillEllipse;
	tempNode.iwidth = iwidth;

	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
	pDs->bMouseDown = TRUE;
	SetCapture(hWnd);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hdc);
}
VOID FillEllipseMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	HDC hdc;
	pDs->bMouseDown = FALSE;
	ReleaseCapture();
	hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOT);
	HBRUSH hBrush = CreateSolidBrush(color);
	HPEN newHpen = CreatePen(PS_SOLID, iwidth, color);
	SelectObject(hdc, newHpen);
	SelectObject(hdc, hBrush);
	Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	pDs->ptEnd.x = GET_X_LPARAM(lParam);
	pDs->ptEnd.y = GET_Y_LPARAM(lParam);
	SetROP2(hdc, R2_COPYPEN);
	Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);

	SelectObject(pDs->hMemDc, hBrush);
	SelectObject(pDs->hMemDc, newHpen);
	Ellipse(pDs->hMemDc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);

	DeleteObject(newHpen);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hdc);

	tempNode.pt2 = pDs->ptEnd;
	vec.push_back(tempNode);
}
VOID FillEllipseMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	HDC hdc;
	if (pDs->bMouseDown)
	{
		hdc = GetDC(hWnd);
		SetROP2(hdc, R2_NOTXORPEN);
		HBRUSH hBrush = CreateSolidBrush(color);
		HPEN newHpen = CreatePen(PS_SOLID, iwidth, color);
		SelectObject(hdc, newHpen);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		pDs->ptEnd.x = GET_X_LPARAM(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);
		Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		DeleteObject(hBrush);
		DeleteObject(newHpen);
		ReleaseDC(hWnd, hdc);
	}
}

VOID EarserMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	pDs->ptStart.x = GET_X_LPARAM(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);

	tempNode.pt1 = pDs->ptStart;
	tempNode.color = color;
	tempNode.drawType = earser;
	tempNode.iwidth = iwidth;

	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
	pDs->bMouseDown = TRUE;
	SetCapture(hWnd);
}
VOID EarserMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	pDs->bMouseDown = FALSE;
	ReleaseCapture();
	tempNode.pt2 = pDs->ptEnd;
	vec.push_back(tempNode);
}
VOID EarserMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	if (pDs->bMouseDown){
		HDC hdc;
		hdc = GetDC(hWnd);
		HPEN newHpen = CreatePen(PS_SOLID, iwidth, RGB(255,255,255));
		SelectObject(hdc, newHpen);
		pDs->ptEnd.x = GET_X_LPARAM(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);

		tempNode.pt2 = pDs->ptEnd;
		vec.push_back(tempNode);
		SetROP2(hdc, R2_WHITE);
		MoveToEx(hdc, pDs->ptStart.x, pDs->ptStart.y, NULL);
		LineTo(hdc, pDs->ptEnd.x, pDs->ptEnd.y);

		SelectObject(pDs->hMemDc, GetStockObject(NULL_BRUSH));
		SelectObject(pDs->hMemDc, newHpen);
		SetROP2(hdc, R2_WHITE);
		MoveToEx(pDs->hMemDc, pDs->ptStart.x, pDs->ptStart.y, NULL);
		LineTo(pDs->hMemDc, pDs->ptEnd.x, pDs->ptEnd.y);

		pDs->ptStart.x = pDs->ptEnd.x;
		pDs->ptStart.y = pDs->ptEnd.y;

		tempNode.pt1 = pDs->ptStart;
		tempNode.color = color;
		tempNode.drawType = earser;
		DeleteObject(newHpen);
		ReleaseDC(hWnd, hdc);
	}
}

VOID CutMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	pDs->ptStart.x = GET_X_LPARAM(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);

	/*tempNode.pt1 = pDs->ptStart;
	tempNode.color = color;
	tempNode.drawType = rect;
	tempNode.iwidth = iwidth;*/

	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
	pDs->bMouseDown = TRUE;
	SetCapture(hWnd);
}
VOID CutMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	HDC hdc;
	if (pDs->bMouseDown)
	{
		hdc = GetDC(GetDesktopWindow());
		SetROP2(hdc, R2_NOTXORPEN);
		HPEN newHpen = CreatePen(PS_SOLID, 1, color);
		SelectObject(hdc, newHpen);
		Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		pDs->ptEnd.x = GET_X_LPARAM(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);
		Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		DeleteObject(newHpen);
		ReleaseDC(hWnd, hdc);
	}
}
VOID CutMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth)
{
	HDC hdc;
	pDs->bMouseDown = FALSE;
	ReleaseCapture();
	//hdc = GetDC(GetDesktopWindow());
	//hdc = GetWindowDC(NULL);
	hdc = GetDC(NULL);	
	HDC hDcMem = CreateCompatibleDC(hdc);
	SetROP2(hdc, R2_NOTXORPEN);
	HPEN newHpen = CreatePen(PS_SOLID, 1, color);
	SelectObject(hdc, newHpen);
	Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	pDs->ptEnd.x = GET_X_LPARAM(lParam);
	pDs->ptEnd.y = GET_Y_LPARAM(lParam);
	SetROP2(hDcMem, R2_COPYPEN);
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	
	cutbmp.pt1 = pDs->ptStart;
	cutbmp.pt2 = pDs->ptEnd;

	HINSTANCE hInst;
	hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, cutbmp.pt2.x - cutbmp.pt1.x, cutbmp.pt2.y - cutbmp.pt1.y);
	TCHAR filename[260] = TEXT("");
	SelectObject(hDcMem, hBitmap);
	BitBlt(hDcMem, 0, 0, cutbmp.pt2.x-cutbmp.pt1.x, cutbmp.pt2.y-cutbmp.pt1.y, hdc, pDs->ptStart.x, pDs->ptStart.y, SRCCOPY);
	PBITMAPINFO pbmi = CreateBitmapInfoStruct(hWnd, hBitmap);
	_tcscpy(filename, SaveFile());
	if (_tcscmp(filename, L"") != 0){
		CreateBMPFile(hWnd, filename, pbmi, hBitmap, hDcMem);
	}



	DeleteObject(newHpen);
	DeleteObject(hBrush);
	ReleaseDC(NULL, hdc);

	/*tempNode.pt2 = pDs->ptEnd;
	vec.push_back(tempNode);*/
}

//-------------
//设置窗体透明
//-------------
void Transparent(HWND hWnd, int cd)
{
	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | 0x00080000);
	HINSTANCE hUser32 = LoadLibrary(TEXT("user32.DLL"));
	if (hUser32)
	{
		typedef BOOL(WINAPI * MYFUNC)(HWND, COLORREF, BYTE, DWORD);
		MYFUNC fun = NULL;

		//取得SetLayeredWindowAttributes函数指针
		fun = (MYFUNC)GetProcAddress(hUser32, "SetLayeredWindowAttributes");
		if (fun)
		{
			COLORREF maskColor = RGB(244, 244, 244);	//要透明的颜色
			fun(hWnd, maskColor, cd, 0x00000002);   //最后一个参数取1则完全透明，取2则再过第三个参数调节透明度
		}
		FreeLibrary(hUser32);
	}
}
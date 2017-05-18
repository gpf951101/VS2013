#include "stdafx.h"
#include "DrawStyle.h"
#include <windowsx.h>

//÷ÿªÊ
vector<node> vec;
node tempNode;

//ªÊÕº
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
		SetROP2(hdc, R2_NOT);
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
		HPEN newHpen = CreatePen(PS_SOLID, iwidth, color);
		SelectObject(hdc, newHpen);
		pDs->ptEnd.x = GET_X_LPARAM(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);

		tempNode.pt2 = pDs->ptEnd;
		vec.push_back(tempNode);
		SetROP2(hdc, R2_WHITE);
		MoveToEx(hdc, pDs->ptStart.x, pDs->ptStart.y, NULL);
		LineTo(hdc, pDs->ptEnd.x, pDs->ptEnd.y);

		pDs->ptStart.x = pDs->ptEnd.x;
		pDs->ptStart.y = pDs->ptEnd.y;

		tempNode.pt1 = pDs->ptStart;
		tempNode.color = color;
		tempNode.drawType = earser;
		DeleteObject(newHpen);
		ReleaseDC(hWnd, hdc);
	}
}
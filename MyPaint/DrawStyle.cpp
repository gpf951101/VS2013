#include "stdafx.h"
#include "DrawStyle.h"
#include <windowsx.h>

//÷ÿªÊ
vector<node> vec;
node tempNode;

//ªÊÕº
VOID LineMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color)
{
	pDs->ptStart.x = GET_X_LPARAM(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);

	tempNode.pt1 = pDs->ptStart;
	tempNode.color = color;
	tempNode.drawType = line;

	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
	pDs->bMouseDown = TRUE;
	SetCapture(hWnd);
}
VOID LineMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color)
{
	HDC hdc;
	pDs->bMouseDown = FALSE;
	ReleaseCapture();
	hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOT);
	SelectObject(hdc, hPen);
	MoveToEx(hdc, pDs->ptStart.x, pDs->ptStart.y, NULL);
	LineTo(hdc, pDs->ptEnd.x, pDs->ptEnd.y);
	pDs->ptEnd.x = GET_X_LPARAM(lParam);
	pDs->ptEnd.y = GET_Y_LPARAM(lParam);
	SetROP2(hdc, R2_COPYPEN);
	MoveToEx(hdc, pDs->ptStart.x, pDs->ptStart.y, NULL);
	LineTo(hdc, pDs->ptEnd.x, pDs->ptEnd.y);
	ReleaseDC(hWnd, hdc);
	
	tempNode.pt2 = pDs->ptEnd;
	vec.push_back(tempNode);
}
VOID LineMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color)
{
	HDC hdc;
	if (pDs->bMouseDown)
	{
		hdc = GetDC(hWnd);
		SetROP2(hdc, R2_NOTXORPEN);
		SelectObject(hdc, hPen);
		MoveToEx(hdc, pDs->ptStart.x, pDs->ptStart.y, NULL);
		LineTo(hdc, pDs->ptEnd.x, pDs->ptEnd.y);
		pDs->ptEnd.x = GET_X_LPARAM(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);
		MoveToEx(hdc, pDs->ptStart.x, pDs->ptStart.y, NULL);
		LineTo(hdc, pDs->ptEnd.x, pDs->ptEnd.y);
		ReleaseDC(hWnd, hdc);
	}
}

VOID RectMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color)
{
	pDs->ptStart.x = GET_X_LPARAM(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);

	tempNode.pt1 = pDs->ptStart;
	tempNode.color = color;
	tempNode.drawType = rect;

	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
	pDs->bMouseDown = TRUE;
	SetCapture(hWnd);
}
VOID RectMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color)
{
	HDC hdc;
	if (pDs->bMouseDown)
	{
		hdc = GetDC(hWnd);
		SetROP2(hdc, R2_NOTXORPEN);
		SelectObject(hdc, hPen);
		Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		pDs->ptEnd.x = GET_X_LPARAM(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);
		Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		ReleaseDC(hWnd, hdc);
	}
}
VOID RectMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color)
{
	HDC hdc;
	pDs->bMouseDown = FALSE;
	ReleaseCapture();
	hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOT);
	SelectObject(hdc, hPen);
	Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	pDs->ptEnd.x = GET_X_LPARAM(lParam);
	pDs->ptEnd.y = GET_Y_LPARAM(lParam);
	SetROP2(hdc, R2_COPYPEN);
	Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	ReleaseDC(hWnd, hdc);

	tempNode.pt2 = pDs->ptEnd;
	vec.push_back(tempNode);
}

VOID EllipseMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color)
{
	pDs->ptStart.x = GET_X_LPARAM(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);

	tempNode.pt1 = pDs->ptStart;
	tempNode.color = color;
	tempNode.drawType = ellipse;

	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
	pDs->bMouseDown = TRUE;
	SetCapture(hWnd);
}
VOID EllipseMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color)
{
	HDC hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOTXORPEN);
	SelectObject(hdc, hPen);
	Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	pDs->bMouseDown = FALSE;
	ReleaseCapture();
	pDs->ptEnd.x = GET_X_LPARAM(lParam);
	pDs->ptEnd.y = GET_Y_LPARAM(lParam);
	SetROP2(hdc, R2_COPYPEN);
	Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	ReleaseDC(hWnd, hdc);

	tempNode.pt2 = pDs->ptEnd;
	vec.push_back(tempNode);
}
VOID EllipseMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color)
{
	if (pDs->bMouseDown)
	{
		HDC hdc = GetDC(hWnd);
		SelectObject(hdc, hPen);
		SetROP2(hdc, R2_NOTXORPEN);
		Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		pDs->ptEnd.x = GET_X_LPARAM(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);
		Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		ReleaseDC(hWnd, hdc);
	}
}

VOID PenMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color)
{
	pDs->ptStart.x = GET_X_LPARAM(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);

	tempNode.pt1 = pDs->ptStart;
	tempNode.color = color;
	tempNode.drawType = line;

	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
	pDs->bMouseDown = TRUE;
	SetCapture(hWnd);
}
VOID PenMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color)
{
	pDs->bMouseDown = FALSE;
	ReleaseCapture();
	tempNode.pt2 = pDs->ptEnd;
	vec.push_back(tempNode);
}
VOID PenMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color)
{
	if (pDs->bMouseDown){
		HDC hdc;	
		hdc = GetDC(hWnd);
		SelectObject(hdc, hPen);
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

		ReleaseDC(hWnd, hdc);
	}
}

VOID FillRectMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color)
{
	HDC hdc = GetDC(hWnd);
	HBRUSH hBrush = CreateSolidBrush(color);
	SelectObject(hdc, hBrush);
	pDs->ptStart.x = GET_X_LPARAM(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);

	tempNode.pt1 = pDs->ptStart;
	tempNode.color = color;
	tempNode.drawType = fillRect;

	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
	pDs->bMouseDown = TRUE;
	SetCapture(hWnd);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hdc);
}
VOID FillRectMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color)
{
	HDC hdc;
	pDs->bMouseDown = FALSE;
	ReleaseCapture();
	hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOT);
	HBRUSH hBrush = CreateSolidBrush(color);
	SelectObject(hdc, hPen);
	SelectObject(hdc, hBrush);
	Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	pDs->ptEnd.x = GET_X_LPARAM(lParam);
	pDs->ptEnd.y = GET_Y_LPARAM(lParam);
	SetROP2(hdc, R2_COPYPEN);
	Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hdc);

	tempNode.pt2 = pDs->ptEnd;
	vec.push_back(tempNode);
}
VOID FillRectMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color)
{
	HDC hdc;
	if (pDs->bMouseDown)
	{
		hdc = GetDC(hWnd);
		SetROP2(hdc, R2_NOTXORPEN);
		HBRUSH hBrush = CreateSolidBrush(color);
		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);
		Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		pDs->ptEnd.x = GET_X_LPARAM(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);
		Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		DeleteObject(hBrush);
		ReleaseDC(hWnd, hdc);
	}
}

VOID FillEllipseMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color)
{
	HDC hdc = GetDC(hWnd);
	HBRUSH hBrush = CreateSolidBrush(color);
	SelectObject(hdc, hBrush);
	pDs->ptStart.x = GET_X_LPARAM(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);

	tempNode.pt1 = pDs->ptStart;
	tempNode.color = color;
	tempNode.drawType = fillEllipse;

	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
	pDs->bMouseDown = TRUE;
	SetCapture(hWnd);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hdc);
}
VOID FillEllipseMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color)
{
	HDC hdc;
	pDs->bMouseDown = FALSE;
	ReleaseCapture();
	hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOT);
	HBRUSH hBrush = CreateSolidBrush(color);
	SelectObject(hdc, hPen);
	SelectObject(hdc, hBrush);
	Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	pDs->ptEnd.x = GET_X_LPARAM(lParam);
	pDs->ptEnd.y = GET_Y_LPARAM(lParam);
	SetROP2(hdc, R2_COPYPEN);
	Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hdc);

	tempNode.pt2 = pDs->ptEnd;
	vec.push_back(tempNode);
}
VOID FillEllipseMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color)
{
	HDC hdc;
	if (pDs->bMouseDown)
	{
		hdc = GetDC(hWnd);
		SetROP2(hdc, R2_NOTXORPEN);
		HBRUSH hBrush = CreateSolidBrush(color);
		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		pDs->ptEnd.x = GET_X_LPARAM(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);
		Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		DeleteObject(hBrush);
		ReleaseDC(hWnd, hdc);
	}
}

#include "stdafx.h"
#include "DrawStyle.h"
#include <windowsx.h>

//»æÍ¼
VOID LineMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen)
{
	HDC hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOTXORPEN);
	SelectObject(hdc, hPen);
	pDs->ptStart.x = GET_X_LPARAM(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);
	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
	pDs->bMouseDown = TRUE;
	SetCapture(hWnd);
	ReleaseDC(hWnd, hdc);
}
VOID LineMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen)
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
}
VOID LineMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen)
{
	HDC hdc;
	if (pDs->bMouseDown)
	{
		hdc = GetDC(hWnd);
		SetROP2(hdc, R2_NOT);
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

VOID RectMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen)
{
	HDC hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOTXORPEN);
	SelectObject(hdc, hPen);
	pDs->ptStart.x = GET_X_LPARAM(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);
	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
	pDs->bMouseDown = TRUE;
	SetCapture(hWnd);
	ReleaseDC(hWnd, hdc);
}
VOID RectMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen)
{
	HDC hdc;
	if (pDs->bMouseDown)
	{
		hdc = GetDC(hWnd);
		SetROP2(hdc, R2_NOT);
		SelectObject(hdc, hPen);
		Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		pDs->ptEnd.x = GET_X_LPARAM(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);
		Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		ReleaseDC(hWnd, hdc);
	}
}
VOID RectMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen)
{
	HDC hdc;
	pDs->bMouseDown = FALSE;
	ReleaseCapture();
	hdc = GetDC(hWnd);
	SelectObject(hdc, hPen);
	pDs->ptEnd.x = GET_X_LPARAM(lParam);
	pDs->ptEnd.y = GET_Y_LPARAM(lParam);
	SetROP2(hdc, R2_COPYPEN);
	Rectangle(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	ReleaseDC(hWnd, hdc);
}

VOID EllipseMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen)
{
	HDC hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOTXORPEN);
	SelectObject(hdc, hPen);
	pDs->ptStart.x = GET_X_LPARAM(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);
	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
	pDs->bMouseDown = TRUE;
	SetCapture(hWnd);
	ReleaseDC(hWnd,hdc);
}
VOID EllipseMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen)
{
	HDC hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOTXORPEN);
	SelectObject(hdc, hPen);
	pDs->bMouseDown = FALSE;
	ReleaseCapture();
	pDs->ptEnd.x = GET_X_LPARAM(lParam);
	pDs->ptEnd.y = GET_Y_LPARAM(lParam);
	SetROP2(hdc, R2_COPYPEN);
	Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
	ReleaseDC(hWnd, hdc);
}
VOID EllipseMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen)
{
	if (pDs->bMouseDown)
	{
		HDC hdc = GetDC(hWnd);
		SelectObject(hdc, hPen);
		SetROP2(hdc, R2_NOT);
		Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		pDs->ptEnd.x = GET_X_LPARAM(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);
		Ellipse(hdc, pDs->ptStart.x, pDs->ptStart.y, pDs->ptEnd.x, pDs->ptEnd.y);
		ReleaseDC(hWnd, hdc);
	}
}

VOID PenMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen)
{
	HDC hdc = GetDC(hWnd);
	SetROP2(hdc, R2_NOTXORPEN);
	SelectObject(hdc, hPen);
	pDs->ptStart.x = GET_X_LPARAM(lParam);
	pDs->ptStart.y = GET_Y_LPARAM(lParam);
	pDs->ptEnd.x = pDs->ptStart.x;
	pDs->ptEnd.y = pDs->ptStart.y;
	pDs->bMouseDown = TRUE;
	SetCapture(hWnd);
	ReleaseDC(hWnd, hdc);
}
VOID PenMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen)
{
	pDs->bMouseDown = FALSE;
	ReleaseCapture();
}
VOID PenMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen)
{
	if (pDs->bMouseDown){
		HDC hdc;	
		hdc = GetDC(hWnd);
		SetROP2(hdc, R2_NOTXORPEN);
		SelectObject(hdc, hPen);
		pDs->ptEnd.x = GET_X_LPARAM(lParam);
		pDs->ptEnd.y = GET_Y_LPARAM(lParam);
		SetROP2(hdc, R2_COPYPEN);
		MoveToEx(hdc, pDs->ptStart.x, pDs->ptStart.y, NULL);
		LineTo(hdc, pDs->ptEnd.x, pDs->ptEnd.y);
		pDs->ptStart.x = pDs->ptEnd.x;
		pDs->ptStart.y = pDs->ptEnd.y;
		ReleaseDC(hWnd, hdc);
	}
}

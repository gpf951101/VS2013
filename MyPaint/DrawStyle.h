#include "resource.h"
#include <vector>
#include <algorithm>

using namespace std;
/*绘图*/
typedef struct tagDrawStru
{
	POINT ptStart, ptEnd;
	BOOL bMouseDown;
} DRAWSTRU, *PDRAWSTRU;
typedef VOID(*PDRAWFUN)(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen,COLORREF color);

enum DrawType
{
	line, ellipse, rect
};
//动态数组
struct node{
	POINT pt1;
	POINT pt2;
	DrawType drawType;
	COLORREF color;
};

//重绘
extern vector<node> vec;
extern node tempNode;

//绘图
VOID LineMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen,COLORREF color);
VOID LineMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color);
VOID LineMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color);
VOID RectMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color);
VOID RectMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color);
VOID RectMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color);
VOID EllipseMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color);
VOID EllipseMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color);
VOID EllipseMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color);
VOID PenMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color);
VOID PenMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color);
VOID PenMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color);
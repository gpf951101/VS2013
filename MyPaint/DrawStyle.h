#include "resource.h"
#include <vector>
#include <algorithm>

using namespace std;
/*��ͼ*/
typedef struct tagDrawStru
{
	POINT ptStart, ptEnd;
	BOOL bMouseDown;
} DRAWSTRU, *PDRAWSTRU;
typedef VOID(*PDRAWFUN)(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen,COLORREF color);

enum DrawType
{
	line, ellipse, rect,fillEllipse,fillRect
};
//��̬����
struct node{
	POINT pt1;
	POINT pt2;
	DrawType drawType;
	COLORREF color;
};

//�ػ�
extern vector<node> vec;
extern node tempNode;

//��ͼ
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
VOID FillRectMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color);
VOID FillRectMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color);
VOID FillRectMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color);
VOID FillEllipseMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color);
VOID FillEllipseMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color);
VOID FillEllipseMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color);
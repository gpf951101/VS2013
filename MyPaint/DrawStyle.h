#include "resource.h"
#include <vector>
#include <algorithm>

#include "FileSaveOpen.h"
#include "SaveBmp.h"
using namespace std;
/*��ͼ*/
typedef struct tagDrawStru
{
	POINT ptStart, ptEnd;
	BOOL bMouseDown;
	HBITMAP hBitmap;
	HDC hMemDc;
	int iWidth;
	int iHeight;
} DRAWSTRU, *PDRAWSTRU;
typedef VOID(*PDRAWFUN)(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);

enum DrawType
{
	line, ellipse, rect,fillEllipse,fillRect,earser
};
//��̬����
struct node{
	POINT pt1;
	POINT pt2;
	DrawType drawType;
	COLORREF color;
	int iwidth;
};

struct cutBmp
{
	POINT pt1;
	POINT pt2;
};

//�ػ�
extern vector<node> vec;
extern node tempNode;

extern cutBmp cutbmp;

//��ͼ
VOID LineMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID LineMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID LineMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID RectMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID RectMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID RectMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID EllipseMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID EllipseMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID EllipseMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID PenMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID PenMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID PenMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID FillRectMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID FillRectMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID FillRectMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID FillEllipseMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID FillEllipseMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID FillEllipseMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID EarserMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID EarserMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID EarserMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID CutMouseDown(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID CutMouseUp(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);
VOID CutMouseMove(PDRAWSTRU pDs, HWND hWnd, WPARAM wParam, LPARAM lParam, HPEN hPen, COLORREF color, int iwidth);

void Transparent(HWND hWnd, int cd);
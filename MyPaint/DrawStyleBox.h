#include "Resource.h"

#define WM_CHANGE_STYLE WM_USER+2
extern TCHAR szStyleBtnClass[];
extern TCHAR szStyleBoxClass[];
LRESULT CALLBACK WndStyleBtnProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndStyleBoxProc(HWND, UINT, WPARAM, LPARAM);
HWND CreateStyleBox(HINSTANCE hInst,
	HWND hWnd, BOOL isDock);
ATOM RegisterStyleClass(HINSTANCE hInstance);
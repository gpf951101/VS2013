#define WM_CHANGE_COLOR WM_USER+1
extern TCHAR szColorBtnClass[];
extern TCHAR szColorBoxClass[];
LRESULT CALLBACK WndColorBtnProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndColorBoxProc(HWND, UINT, WPARAM, LPARAM);
HWND CreateColorBox(HINSTANCE hInst,
	HWND hWnd, BOOL isDock);
ATOM RegisterColorClass(HINSTANCE hInstance);
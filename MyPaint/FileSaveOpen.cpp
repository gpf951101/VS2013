#include "stdafx.h"
#include "FileSaveOpen.h"
#include "atlstr.h"

TCHAR* getSystemTime(){
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y%m%d%H%M%S ", localtime(&t));
	USES_CONVERSION;
	TCHAR temp[50];
	_tcscpy(temp, A2T(tmp));
	return temp;
}
TCHAR* SaveFile()
{
	OPENFILENAME ofn;  
    ZeroMemory(&ofn, sizeof(ofn));  
	TCHAR * t = getSystemTime();
	TCHAR filename[MAX_PATH] = {0};
	_tcscpy(filename, t);
    ofn.lpstrFile         = filename;  
    ofn.nMaxFile          = MAX_PATH;  
    ofn.lpstrFilter       = _T("Text Files(*.bmp)|*.bmp|All Files(*.*)|*.*||");  
    ofn.lpstrDefExt       = _T("bmp");  
    ofn.lpstrTitle        = _T("Save To");  
    ofn.Flags             = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;  
    ofn.FlagsEx           = OFN_EX_NOPLACESBAR;  
    ofn.lStructSize       = sizeof(OPENFILENAME);  // No OFN_ENABLEHOOK  
    ofn.hwndOwner         = NULL;  
    if (GetSaveFileName(&ofn))  
    {  
		return filename;
	}
	else{
		return TEXT("");
	}
}

TCHAR * OpenFile()
{
	OPENFILENAME ofn;// common dialog box structure
	TCHAR szFile[MAX_PATH];// buffer for file name
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("Text files (*.bmp)\0*.bmp\0\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrTitle = _T("´ò¿ª");// set the dialog box title
	if (GetOpenFileName(&ofn))
	{
		return szFile;
	}
else
	return TEXT("");
}

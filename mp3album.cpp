
// main.cpp
//============================================================================//
// 更新：02/12/15(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "include.h"
#include "resource.h"
#include "ZipDlg.h"
#include "Profile.h"


/******************************************************************************/
// WinMain
//============================================================================//
// 更新：02/12/15(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int iCmdShow)
{
	ZipDlg zd ;
	Profile::Load() ;
	DialogBoxParam( hInst, MAKEINTRESOURCE( IDD_MAIN), NULL, ZipDlgProc, (LPARAM)&zd) ;
	Profile::Save() ;

	return 0 ;
}


/******************************************************************************/
// 出力用関数
//============================================================================//
// 更新：02/04/09(火)
// 概要：メッセージマップを記述する為の基本マクロ。
// 補足：なし。
//============================================================================//

void MyOutputDebugString( LPCSTR pszFormat, ...)
{
    va_list    argp;
    char pszBuf[ 256];
    va_start(argp, pszFormat);
    vsprintf_s( pszBuf, pszFormat, argp);
    va_end(argp);
    OutputDebugString( pszBuf);
}

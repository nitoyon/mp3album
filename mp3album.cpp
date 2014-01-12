
// main.cpp
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "include.h"
#include "resource.h"
#include "ZipDlg.h"
#include "Profile.h"


/******************************************************************************/
// WinMain
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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
// �o�͗p�֐�
//============================================================================//
// �X�V�F02/04/09(��)
// �T�v�F���b�Z�[�W�}�b�v���L�q����ׂ̊�{�}�N���B
// �⑫�F�Ȃ��B
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

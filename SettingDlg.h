
// SettingDlg.h
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __SETTING_DLG_H__
#define  __SETTING_DLG_H__

#include "include.h"
#include "MsgMap.h"


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class SettingDlg
{
private:
	HWND	m_hWnd ;
	HWND	hwndFolder ;
	HWND	hwndFile ;
	HWND	hwndCombo ;

public:
// �R���X�g���N�^����уf�X�g���N�^
	SettingDlg() ;
	~SettingDlg() ;

// ���b�Z�[�W�n���h��
private:
	DECLARE_DLG_MESSAGE_MAP( SettingDlgProc)

	BOOL OnInitDialog	( HWND, WPARAM, LPARAM) ;
	BOOL OnOk		( HWND, WPARAM, LPARAM) ;
	BOOL OnCancel		( HWND, WPARAM, LPARAM) ;

	BOOL OnFolderRdo	( HWND, WPARAM, LPARAM) ;
	BOOL OnFileRdo		( HWND, WPARAM, LPARAM) ;
	BOOL OnLogChk		( HWND, WPARAM, LPARAM) ;
	BOOL OnBrowseBtn	( HWND, WPARAM, LPARAM) ;
} ;

#endif
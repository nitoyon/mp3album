
// SettingDlg.h
//============================================================================//
// 更新：02/12/15(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __SETTING_DLG_H__
#define  __SETTING_DLG_H__

#include "include.h"
#include "MsgMap.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class SettingDlg
{
private:
	HWND	m_hWnd ;
	HWND	hwndFolder ;
	HWND	hwndFile ;
	HWND	hwndCombo ;

public:
// コンストラクタおよびデストラクタ
	SettingDlg() ;
	~SettingDlg() ;

// メッセージハンドラ
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


// ZipDlg.h
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __MAIN_DLG_H__
#define  __MAIN_DLG_H__

#include "include.h"
#include "MsgMap.h"


/******************************************************************************/
//		定義
/******************************************************************************/

class File ;


/******************************************************************************/
//		グローバル関数
/******************************************************************************/

int CALLBACK ListSortProc( LPARAM lp1, LPARAM lp2, LPARAM lp3) ;


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class ZipDlg
{
private:
	HWND		m_hWnd ;
	HWND		hwndList ;
	HWND		hwndOk ;
	HWND		hwndDel ;
	WNDPROC		wpcList ;
	HINSTANCE	hInstance ;

	BOOL	blnFullPath ;
	BOOL	blnOnlyMp3 ;

	vector<File*>	vecFileList ;

public:
// コンストラクタおよびデストラクタ
	ZipDlg() ;
	~ZipDlg() ;

// リスト操作
	BOOL ListAdd( File*, int intNum) ;
	BOOL ListDel() ;
	BOOL ListMove( BOOL) ;
	void ListReload( BOOL blnReload = FALSE) ;

// メッセージハンドラ
private:
	DECLARE_DLG_MESSAGE_MAP( ZipDlgProc)
	DECLARE_SUBCLASS_MESSAGE_MAP( ListProc)

	BOOL OnInitDialog	( HWND, WPARAM, LPARAM) ;
	LRESULT OnDropFiles	( HWND, WPARAM, LPARAM) ;
	BOOL OnSize		( HWND, WPARAM, LPARAM) ;

	BOOL OnOk		( HWND, WPARAM, LPARAM) ;
	BOOL OnCancel		( HWND, WPARAM, LPARAM) ;
	BOOL OnAboutBtn		( HWND, WPARAM, LPARAM) ;

	BOOL OnFullPathChanged	( HWND, WPARAM, LPARAM) ;
	BOOL OnOnlyMp3Changed	( HWND, WPARAM, LPARAM) ;

	BOOL OnBrowseBtn	( HWND, WPARAM, LPARAM) ;
	BOOL OnUpBtn		( HWND, WPARAM, LPARAM) ;
	BOOL OnDownBtn		( HWND, WPARAM, LPARAM) ;
	BOOL OnAddBtn		( HWND, WPARAM, LPARAM) ;
	BOOL OnDelBtn		( HWND, WPARAM, LPARAM) ;
	BOOL OnListNotify	( HWND, WPARAM, LPARAM) ;
} ;

#endif
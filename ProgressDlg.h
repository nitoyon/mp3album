
// ProgressDlg.h
//============================================================================//
// 更新：02/12/12(木)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __PROGRESS_DLG_H__
#define  __PROGRESS_DLG_H__

#include "include.h"
#include "MsgMap.h"


/******************************************************************************/
//		定義
/******************************************************************************/

class File ;


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class ProgressDlg
{
private:
	HWND		m_hWnd ;
	HWND		hwndArchiveProgress ;
	HWND		hwndFileProgress ;

	vector<File*>*	pvecFile ;
	string		strArchivePath ;
	HANDLE		hThread ;

public:
// コンストラクタおよびデストラクタ
	ProgressDlg( vector<File*>*, const string&) ;
	~ProgressDlg() ;

// 取得
	vector<File*>* GetFileList() const{ return pvecFile ;}
	string GetArchivePath() const{ return strArchivePath ;}

// 設定
	void SetFileName( const string& s) ;
	void SetProgressPos( int, ULONG) ;
	void SetProgressRange( int, ULONG) ;

// メッセージハンドラ
private:
	DECLARE_DLG_MESSAGE_MAP( ProgressDlgProc)

	BOOL OnInitDialog	( HWND, WPARAM, LPARAM) ;
	BOOL OnCancel		( HWND, WPARAM, LPARAM) ;
} ;

#endif
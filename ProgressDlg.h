
// ProgressDlg.h
//============================================================================//
// 更新：03/02/09(日)
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

class Mp3File ;


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class ProgressDlg
{
public:
// 定義
	enum State
	{
		ZIPPING,	// zip化中
		SUCCESS,	// zip化正常終了
		FAIL,		// zip化異常終了
		CANCELED,	// キャンセルされた
		NOTHREAD	// スレッド作成エラー
	} ;

private:
	HWND		m_hWnd ;
	HWND		hwndArchiveProgress ;
	HWND		hwndFileProgress ;

	vector<Mp3File*>*	pvecFile ;
	string		strArchivePath ;
	State		state ;

	string		strLog ;
	string		strErrLog ;

public:
// コンストラクタおよびデストラクタ
	ProgressDlg( vector<Mp3File*>*, const string&) ;
	~ProgressDlg() ;

// 取得
	vector<Mp3File*>* GetFileList() const{ return pvecFile ;}
	string GetArchivePath() const{ return strArchivePath ;}
	State GetState() const{ return state ;}
	string GetLog() const{ return strLog ;}
	string GetErrLog() const{ return strErrLog ;}

// 設定
	void SetFileName( const string& s) ;
	void SetProgressPos( int, ULONG) ;
	void SetProgressRange( int, ULONG) ;
	void SetState( State s) ;
	void SetLog( const string& s){ strLog = s ;}
	void SetErrLog( const string& s){ strErrLog = s ;}

// メッセージハンドラ
private:
	DECLARE_DLG_MESSAGE_MAP( ProgressDlgProc)

	BOOL OnInitDialog	( HWND, WPARAM, LPARAM) ;
	BOOL OnCancel		( HWND, WPARAM, LPARAM) ;

public:
	BOOL OnZipFinish() ;
} ;

#endif


// ZipDlg.h
//============================================================================//
// �X�V�F03/02/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __MAIN_DLG_H__
#define  __MAIN_DLG_H__

#include "include.h"
#include "MsgMap.h"


/******************************************************************************/
//		��`
/******************************************************************************/

class Mp3File ;


/******************************************************************************/
//		�O���[�o���֐�
/******************************************************************************/

int CALLBACK ListSortProc( LPARAM lp1, LPARAM lp2, LPARAM lp3) ;
int CALLBACK BrowseProc( HWND, UINT, WPARAM, LPARAM) ;


/******************************************************************************/
//		�N���X��`
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

	vector<Mp3File*>	vecFileList ;

public:
// �R���X�g���N�^����уf�X�g���N�^
	ZipDlg() ;
	~ZipDlg() ;

// ���X�g����
	BOOL ListAdd( Mp3File*, int intNum) ;
	BOOL ZipDlg::ListAddFileDlg() ;
	BOOL ListDel() ;
	BOOL ListMove( BOOL) ;
	void ListReload( BOOL blnReload = FALSE) ;
	void ListSetFolder() ;

// ���b�Z�[�W�n���h��
private:
	DECLARE_DLG_MESSAGE_MAP( ZipDlgProc)
	DECLARE_DLG_MESSAGE_MAP( FileNameDlgProc)
	DECLARE_SUBCLASS_MESSAGE_MAP( ListProc)

	BOOL OnInitDialog	( HWND, WPARAM, LPARAM) ;
	LRESULT OnDropFiles	( HWND, WPARAM, LPARAM) ;
	BOOL OnSize		( HWND, WPARAM, LPARAM) ;

	BOOL OnOk		( HWND, WPARAM, LPARAM) ;
	BOOL OnCancel		( HWND, WPARAM, LPARAM) ;
	BOOL OnAboutBtn		( HWND, WPARAM, LPARAM) ;
	BOOL OnSettingBtn	( HWND, WPARAM, LPARAM) ;

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
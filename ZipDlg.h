
// ZipDlg.h
//============================================================================//
// �X�V�F02/12/09(��)
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

#define PUTSH(a,f)	{putc((char)((a) & 0xff),(f)); putc((char)((a) >> 8),(f));}
#define PUTLG(a,f)	{PUTSH((a) & 0xffff,(f)) PUTSH((a) >> 16,(f))}
#define SBSZ		16384

class Mp3File ;
class File ;


/******************************************************************************/
//		�O���[�o���֐�
/******************************************************************************/

int CALLBACK ListSortProc( LPARAM lp1, LPARAM lp2, LPARAM lp3) ;


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class ZipDlg
{
private:
	HWND	m_hWnd ;
	HWND	hwndList ;
	HWND	hwndOk ;
	HWND	hwndDel ;
	WNDPROC	wpcList ;

	BOOL	blnFullPath ;
	BOOL	blnOnlyMp3 ;

	vector<File*>	vecFileList ;

public:
// �R���X�g���N�^����уf�X�g���N�^
	ZipDlg() ;
	~ZipDlg() ;

// ���X�g����
	BOOL ListAdd( File*, int intNum) ;
	BOOL ListDel() ;
	BOOL ListMove( BOOL) ;
	void ListReload( BOOL blnReload = FALSE) ;

// ���b�Z�[�W�n���h��
private:
	DECLARE_DLG_MESSAGE_MAP( ZipDlgProc)
	DECLARE_SUBCLASS_MESSAGE_MAP( ListProc)

	LRESULT OnDropFiles	( HWND, WPARAM, LPARAM) ;
	BOOL OnInitDialog	( HWND, WPARAM, LPARAM) ;

	BOOL OnOk		( HWND, WPARAM, LPARAM) ;
	BOOL OnCancel		( HWND, WPARAM, LPARAM) ;
	BOOL OnAboutBtn		( HWND, WPARAM, LPARAM) ;

	BOOL OnFullPathChanged	( HWND, WPARAM, LPARAM) ;
	BOOL OnOnlyMp3Changed	( HWND, WPARAM, LPARAM) ;

	BOOL OnUpBtn		( HWND, WPARAM, LPARAM) ;
	BOOL OnDownBtn		( HWND, WPARAM, LPARAM) ;
	BOOL OnAddBtn		( HWND, WPARAM, LPARAM) ;
	BOOL OnDelBtn		( HWND, WPARAM, LPARAM) ;
	BOOL OnListNotify	( HWND, WPARAM, LPARAM) ;

// ���k
	BOOL fire() ;
	BOOL OutputLocalFileHeader( Mp3File*, FILE*) ;
	BOOL OutputCentralDirectory( Mp3File*, FILE*) ;
	BOOL OutputEndCentralDirectory( int, ULONG, ULONG, FILE*) ;
} ;

#endif
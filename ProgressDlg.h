
// ProgressDlg.h
//============================================================================//
// �X�V�F02/12/12(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __PROGRESS_DLG_H__
#define  __PROGRESS_DLG_H__

#include "include.h"
#include "MsgMap.h"


/******************************************************************************/
//		��`
/******************************************************************************/

class File ;


/******************************************************************************/
//		�N���X��`
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
// �R���X�g���N�^����уf�X�g���N�^
	ProgressDlg( vector<File*>*, const string&) ;
	~ProgressDlg() ;

// �擾
	vector<File*>* GetFileList() const{ return pvecFile ;}
	string GetArchivePath() const{ return strArchivePath ;}

// �ݒ�
	void SetFileName( const string& s) ;
	void SetProgressPos( int, ULONG) ;
	void SetProgressRange( int, ULONG) ;

// ���b�Z�[�W�n���h��
private:
	DECLARE_DLG_MESSAGE_MAP( ProgressDlgProc)

	BOOL OnInitDialog	( HWND, WPARAM, LPARAM) ;
	BOOL OnCancel		( HWND, WPARAM, LPARAM) ;
} ;

#endif
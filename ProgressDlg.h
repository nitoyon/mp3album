
// ProgressDlg.h
//============================================================================//
// �X�V�F02/12/15(��)
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
public:
// ��`
	enum State
	{
		ZIPPING,	// zip����
		SUCCESS,	// zip������I��
		FAIL,		// zip���ُ�I��
		CANCELED,	// �L�����Z�����ꂽ
		NOTHREAD	// �X���b�h�쐬�G���[
	} ;

private:
	HWND		m_hWnd ;
	HWND		hwndArchiveProgress ;
	HWND		hwndFileProgress ;

	vector<File*>*	pvecFile ;
	string		strArchivePath ;
	State		state ;

	string		strLog ;
	string		strErrLog ;

public:
// �R���X�g���N�^����уf�X�g���N�^
	ProgressDlg( vector<File*>*, const string&) ;
	~ProgressDlg() ;

// �擾
	vector<File*>* GetFileList() const{ return pvecFile ;}
	string GetArchivePath() const{ return strArchivePath ;}
	State GetState() const{ return state ;}
	string GetLog() const{ return strLog ;}
	string GetErrLog() const{ return strErrLog ;}

// �ݒ�
	void SetFileName( const string& s) ;
	void SetProgressPos( int, ULONG) ;
	void SetProgressRange( int, ULONG) ;
	void SetState( State s) ;
	void SetLog( const string& s){ strLog = s ;}
	void SetErrLog( const string& s){ strErrLog = s ;}

// ���b�Z�[�W�n���h��
private:
	DECLARE_DLG_MESSAGE_MAP( ProgressDlgProc)

	BOOL OnInitDialog	( HWND, WPARAM, LPARAM) ;
	BOOL OnCancel		( HWND, WPARAM, LPARAM) ;

public:
	BOOL OnZipFinish() ;
} ;

#endif
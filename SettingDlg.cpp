
// SettingDlg.cpp
//============================================================================//
// �X�V�F03/04/03(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "SettingDlg.h"
#include "ZipDlg.h"	// for BrowseProc
#include "Profile.h"
#include "resource.h"
#include "Mp3File.h"
#include "util.h"


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

SettingDlg::SettingDlg() 
{
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

SettingDlg::~SettingDlg() 
{
}



/******************************************************************************/
//		���b�Z�[�W�n���h��
/******************************************************************************/
// ���b�Z�[�W�}�b�v��`
//============================================================================//
// �X�V�F03/04/03(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BEGIN_DLG_MESSAGE_MAP( SettingDlgProc, SettingDlg)
	ON_MESSAGE( WM_INITDIALOG	, OnInitDialog)
	BEGIN_COMMAND_MAP()
		ON_COMMAND( IDOK		, OnOk)
		ON_COMMAND( IDCANCEL		, OnCancel)

		ON_COMMAND( IDC_FOLDERSET	, OnFolderRdo)
		ON_COMMAND( IDC_FOLDER1		, OnFolderRdo)
		ON_COMMAND( IDC_FOLDER2		, OnFolderRdo)
		ON_COMMAND( IDC_FOLDERONZIP	, OnFolderRdo)
		ON_COMMAND( IDC_FILESET		, OnFileRdo)
		ON_COMMAND( IDC_FILEPARENT	, OnFileRdo)
		ON_COMMAND( IDC_DISPLAYLOG	, OnLogChk)
		ON_COMMAND( IDC_BROWSE		, OnBrowseBtn)
	END_COMMAND_MAP()
END_DLG_MESSAGE_MAP()


/******************************************************************************/
// ������
//============================================================================//
// �X�V�F03/04/03(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL SettingDlg::OnInitDialog( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	hwndFolder = GetDlgItem( hDlg, IDC_FOLDERNAME) ;
	hwndFile = GetDlgItem( hDlg, IDC_FILENAME) ;
	hwndCombo = GetDlgItem( hDlg, IDC_COMBO) ;

	// �`�F�b�N
	CheckRadioButton( hDlg, IDC_FOLDERSET, IDC_FOLDER2, IDC_FOLDERSET + Profile::intFolder) ;
	CheckRadioButton( hDlg, IDC_FILESET, IDC_FILEPARENT, IDC_FILESET + Profile::intFile) ;
	CheckDlgButton( hDlg, IDC_DISPLAYLOG, Profile::blnDisplayLog ? BST_CHECKED : 0) ;
	CheckDlgButton( hDlg, IDC_FAILLOGONLY, Profile::blnFailLogOnly ? BST_CHECKED : 0) ;
	CheckDlgButton( hDlg, IDC_CONFIRMOVERWRITE, Profile::blnConfirmOverwrite ? BST_CHECKED : 0) ;
	CheckDlgButton( hDlg, IDC_CLEARONSUCCESS, Profile::blnClearOnSuccess ? BST_CHECKED : 0) ;
	SetDlgItemText( hDlg, IDC_FOLDERNAME, Profile::strFolder.c_str()) ;
	SetDlgItemText( hDlg, IDC_FILENAME, Profile::strFile.c_str()) ;
	SendMessage( hwndCombo, CB_INSERTSTRING, -1, (LPARAM)(LPCTSTR)".zip") ;
	SendMessage( hwndCombo, CB_INSERTSTRING, -1, (LPARAM)(LPCTSTR)".zip.mp3") ;
	SetWindowText( hwndCombo, Profile::strExtension.c_str()) ;

	// ������
	OnFolderRdo( hDlg, 0, 0) ;
	OnFileRdo( hDlg, 0, 0) ;
	OnLogChk( hDlg, 0, 0) ;

	return TRUE ;
}


/******************************************************************************/
// OK
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL SettingDlg::OnOk( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// �_�C�A���O����ǂ݂Ƃ�
	char pszPath[ MAX_PATH + 1] ;
	Profile::intFolder = IsDlgButtonChecked( hDlg, IDC_FOLDERSET) ? Profile::ZipFolder::SET : 
			     IsDlgButtonChecked( hDlg, IDC_FOLDER1) ? Profile::ZipFolder::ONE : 
			     IsDlgButtonChecked( hDlg, IDC_FOLDER2) ? Profile::ZipFolder::TWO : 
			     Profile::ZipFolder::ONZIP ;
	Profile::intFile = IsDlgButtonChecked( hDlg, IDC_FILESET) ? Profile::ZipFile::FILESET : 
			   Profile::ZipFile::PARENT ;
	GetDlgItemText( hDlg, IDC_FOLDERNAME, pszPath, MAX_PATH + 1) ;
	Profile::strFolder = pszPath ;
	GetDlgItemText( hDlg, IDC_FILENAME, pszPath, MAX_PATH + 1) ;
	Profile::strFile = pszPath ;
	Profile::blnDisplayLog = IsDlgButtonChecked( hDlg, IDC_DISPLAYLOG) ;
	Profile::blnFailLogOnly = IsDlgButtonChecked( hDlg, IDC_FAILLOGONLY) ;
	Profile::blnConfirmOverwrite = IsDlgButtonChecked( hDlg, IDC_CONFIRMOVERWRITE) ;
	Profile::blnClearOnSuccess = IsDlgButtonChecked( hDlg, IDC_CLEARONSUCCESS) ;

	GetDlgItemText( hDlg, IDC_COMBO, pszPath, MAX_PATH + 1) ;
	Profile::strExtension = pszPath ;

	Profile::Save() ;

	EndDialog( hDlg, TRUE) ;
	return TRUE ;
}


/******************************************************************************/
// �L�����Z��
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL SettingDlg::OnCancel( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	EndDialog( hDlg, FALSE) ;
	return TRUE ;
}


/******************************************************************************/
// �t�H���_���W�I
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL SettingDlg::OnFolderRdo( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	BOOL b = IsDlgButtonChecked( hDlg, IDC_FOLDERSET) ;
	EnableWindow( hwndFolder, b) ;
	EnableWindow( GetDlgItem( hDlg, IDC_BROWSE), b) ;

	return TRUE ;
}


/******************************************************************************/
// �t�@�C�����W�I
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL SettingDlg::OnFileRdo( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	BOOL b = IsDlgButtonChecked( hDlg, IDC_FILESET) ;
	EnableWindow( hwndFile, b) ;

	return TRUE ;
}


/******************************************************************************/
// �`�F�b�N
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL SettingDlg::OnLogChk( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	if( IsDlgButtonChecked( hDlg, IDC_DISPLAYLOG))
	{
		EnableWindow( GetDlgItem( hDlg, IDC_FAILLOGONLY), TRUE) ;
	}
	else
	{
		EnableWindow( GetDlgItem( hDlg, IDC_FAILLOGONLY), FALSE) ;
	}
	return TRUE ;
}


/******************************************************************************/
// �u���E�Y
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL SettingDlg::OnBrowseBtn( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	char pszZipPath[ MAX_PATH + 1] ;
	GetDlgItemText( m_hWnd, IDC_FOLDERNAME, pszZipPath, MAX_PATH) ;
	char pszPath[ MAX_PATH + 1];
	strcpy( pszPath, GetDirName( pszZipPath).c_str()) ;

	BROWSEINFO bi ;
	bi.hwndOwner		= hDlg ;
	bi.pidlRoot		= NULL ;
	bi.pszDisplayName	= NULL ;
	bi.lpszTitle		= "�t�H���_��I�����ĉ�����" ;
	bi.ulFlags		= 0x8000 ;//BIF_SHAREABLE ;
	bi.lpfn			= (BFFCALLBACK)BrowseProc ;
	bi.lParam		= (LPARAM)pszPath ;
	bi.iImage		= NULL ;

	LPITEMIDLIST idlist = SHBrowseForFolder( &bi);
	if( idlist)
	{
		char pszPath[ MAX_PATH] ;
		SHGetPathFromIDList( idlist, pszPath) ;
		CoTaskMemFree( idlist) ;

		Mp3File file( pszPath) ;
		if( file.GetFileName() != "")
		{
			string s = string( pszPath) + "\\" ;
			SetDlgItemText( hDlg, IDC_FOLDERNAME, s.c_str()) ;
		}
		else
		{
			SetDlgItemText( hDlg, IDC_FOLDERNAME, pszPath) ;
		}
	}

	return TRUE ;
}

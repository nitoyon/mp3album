

// ZipDlg.cpp
//============================================================================//
// �X�V�F03/04/03(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "ZipDlg.h"
#include "Profile.h"
#include "ProgressDlg.h"
#include "SettingDlg.h"
#include "resource.h"
#include "about.h"
#include "Zip.h"
#include "util.h"


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �X�V�F02/12/07(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ZipDlg::ZipDlg() 
{
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �X�V�F02/12/07(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ZipDlg::~ZipDlg() 
{
}


/******************************************************************************/
//		���b�Z�[�W�n���h��
/******************************************************************************/
// ���b�Z�[�W�}�b�v��`
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BEGIN_DLG_MESSAGE_MAP( ZipDlgProc, ZipDlg)
	ON_MESSAGE( WM_INITDIALOG	, OnInitDialog)
//	ON_MESSAGE( WM_SIZE		, OnSize)
	BEGIN_COMMAND_MAP()
		ON_COMMAND( IDOK		, OnOk)
		ON_COMMAND( IDCANCEL		, OnCancel)
		ON_COMMAND( IDC_ABOUT		, OnAboutBtn)
		ON_COMMAND( IDC_SETTING		, OnSettingBtn)

		ON_COMMAND( IDC_ONLYMP3		, OnOnlyMp3Changed)

		ON_COMMAND( IDC_BROWSE		, OnBrowseBtn)
		ON_COMMAND( IDC_ADD		, OnAddBtn)
		ON_COMMAND( IDC_DEL		, OnDelBtn)
		ON_COMMAND( IDC_UP		, OnUpBtn)
		ON_COMMAND( IDC_DOWN		, OnDownBtn)
	END_COMMAND_MAP()
	BEGIN_NOTIFY_MAP()
		ON_NOTIFY( IDC_LIST		, OnListNotify)
	END_NOTIFY_MAP()
END_DLG_MESSAGE_MAP()

BEGIN_SUBCLASS_MESSAGE_MAP( ListProc, ZipDlg)
	ON_MESSAGE( WM_DROPFILES		, OnDropFiles)
END_SUBCLASS_MESSAGE_MAP( pWndObj->wpcList)


/******************************************************************************/
// �_�C�A���O������
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::OnInitDialog( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	InitCommonControls();

	// �ϐ�������
	hInstance = (HINSTANCE)GetWindowLong( hDlg, GWL_HINSTANCE) ;

	// �A�C�R���ݒ�
	HICON hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ICON));
	SendMessage( hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	SendMessage( hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	DestroyIcon( hIcon) ;

	// �E�C���h�E�n���h���擾
	hwndList	= GetDlgItem( hDlg, IDC_LIST) ;
	hwndOk		= GetDlgItem( hDlg, IDOK) ;
	hwndDel		= GetDlgItem( hDlg, IDC_DEL) ;

	// ���X�g�ݒ�
	SetWindowLong( hwndList, GWL_STYLE, GetWindowLong( hwndList, GWL_STYLE) | LVS_SHAREIMAGELISTS) ;
	DragAcceptFiles( hwndList, TRUE) ;
	ListView_DeleteAllItems( hwndList) ;
	LVCOLUMN lvclm ;
	lvclm.mask	= LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ;
	lvclm.fmt	= LVCFMT_LEFT ;
	lvclm.pszText	= "�t�@�C����" ;
	lvclm.cx	= 150 ;
	lvclm.iSubItem	= 0 ;
	ListView_InsertColumn( hwndList, 0, &lvclm); 
	lvclm.mask	= LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ;
	lvclm.fmt	= LVCFMT_LEFT ;
	lvclm.pszText	= "�p�X" ;
	lvclm.cx	= 250 ;
	lvclm.iSubItem	= 0 ;
	ListView_InsertColumn( hwndList, 1, &lvclm); 

	SHFILEINFO shfi ;
	HIMAGELIST hImageList = (HIMAGELIST)SHGetFileInfo( "", 0, &shfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON) ;
	ListView_SetImageList( hwndList, hImageList, LVSIL_SMALL) ;
	hImageList = (HIMAGELIST)SHGetFileInfo( "", 0, &shfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_LARGEICON) ;
	ListView_SetImageList( hwndList, hImageList, LVSIL_NORMAL) ;

	// ������
	EnableWindow( hwndDel, FALSE) ;
	EnableWindow( hwndOk, FALSE) ;
	if( Profile::blnOnlyMp3)CheckDlgButton( hDlg, IDC_ONLYMP3, BST_CHECKED) ;
	RECT rc ;
	GetWindowRect( hDlg, &rc) ;
	MoveWindow( hDlg, Profile::intX, Profile::intY, rc.right, rc.bottom, FALSE) ;

	wpcList = (WNDPROC)GetWindowLong( hwndList, GWL_WNDPROC) ;
	if( wpcList)
	{
		SetWindowLong( hwndList, GWL_WNDPROC , (LONG)ListProc) ;
		SetWindowLong( hwndList, GWL_USERDATA, (LONG)this) ;
	}

	return TRUE ;
}


/******************************************************************************/
// ���X�g�v���V�[�W��
//============================================================================//
// �X�V�F03/02/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

LRESULT ZipDlg::OnDropFiles( HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	HDROP hDrop = (HDROP)wParam ;
	unsigned int intIndex = DragQueryFile( hDrop, 0xFFFFFFFF, NULL, 0) ;
	for( unsigned int i = 0; i < intIndex; i++)
	{
		char pszBuf[ MAX_PATH + 1] ;
		DragQueryFile( hDrop, i, pszBuf, MAX_PATH) ;
		string s = pszBuf ;

		// �t�H���_���H
		if( GetFileAttributes( s.c_str()) & FILE_ATTRIBUTE_DIRECTORY)
		{
			// �����J�n
			string strFind = s + "\\*.*" ;
			WIN32_FIND_DATA wfd ;
			HANDLE hFind = FindFirstFile( strFind.c_str(), &wfd) ;
			if( hFind == INVALID_HANDLE_VALUE)
			{
				return 0 ;
			}
			do
			{
				// �A�C�e�����̃t�B���^�����O
				string strFile = wfd.cFileName ;
				if( strFile == "." || strFile == "..")
				{
					continue ;
				}

				Mp3File* pMp3 = new Mp3File( s + "\\" + strFile) ;
				pMp3->SetSaveName( GetFileName( s) + "\\" + strFile) ;
				ListAdd( pMp3, vecFileList.size()) ;
			}
			while( FindNextFile( hFind, &wfd)) ;

			FindClose( hFind) ;
		}
		else if( !Profile::blnOnlyMp3 || s.rfind( ".mp3") == s.size() - 4 || 
		    s.rfind( ".MP3") == s.size() - 4)
		{
			Mp3File* pMp3 = new Mp3File( pszBuf) ;
			ListAdd( pMp3, vecFileList.size()) ;
		}
	}
	DragFinish( hDrop) ;
	return TRUE ;
}


/******************************************************************************/
// �T�C�Y�ύX
//============================================================================//
// �X�V�F02/12/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::OnSize( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	return TRUE ;
}


/******************************************************************************/
// OK
//============================================================================//
// �X�V�F03/04/03(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::OnOk( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// �e��l�ݒ�
	char pszZipPath[ MAX_PATH + 1] ;
	GetDlgItemText( m_hWnd, IDC_ZIPPATH, pszZipPath, MAX_PATH) ;
	string	strPath = File( pszZipPath).GetBasePath() ;
	strPath = File( pszZipPath).GetFileName() ;

	// �t�@�C�����𑫂� (�f�B���N�g�� || ���͂Ȃ��̏ꍇ)
	if( strPath == "")
	{
		if( Profile::intFile == Profile::ZipFile::FILESET)
		{
			strPath += Profile::strFile ;
		}
		else
		{
			// �e�f�B���N�g����
			string s = vecFileList[ 0]->GetBasePath() ;
			s = s.substr( 0, s.size() - 1) ;
			s = File( s).GetFileName() ;
			if( s.find( ':') == string::npos)
			{
				strPath += s ;
			}
			else
			{
				strPath += s.substr( 0, s.find( ':')) ;
			}
		}

		// �g���q�𑫂�
		strPath += Profile::strExtension ;
	}
	// '.' ���Ȃ��ꍇ
	else if( File( strPath).GetFileName().find( '.') == string::npos)
	{
		strPath += Profile::strExtension ;
	}

	if( File( strPath).GetBasePath() == "")	// �f�B���N�g���̎w�肪�Ȃ��ꍇ
	{
		switch( Profile::intFolder)
		{
			case Profile::ZipFolder::SET:
				strPath = Profile::strFolder + strPath ;
				break ;

			case Profile::ZipFolder::ONE:
			{
				string strDir = vecFileList[ 0]->GetFilePath() ;
				strDir = strDir.substr( 0, strDir.rfind( '\\') + 1) ;
				strPath = strDir + strPath ;
				break ;
			}

			case Profile::ZipFolder::TWO:
			{
				string strDir = vecFileList[ 0]->GetFilePath() ;
				strDir = strDir.substr( 0, strDir.rfind( '\\')) ;
				if(strDir.rfind( '\\') != string::npos)
				{
					strDir = strDir.substr( 0, strDir.rfind( '\\') + 1) ;
				}
				strPath = strDir + strPath ;
				break ;
			}

			case Profile::ZipFolder::ONZIP:
			{
				char pszZipPath[ MAX_PATH + 1] ;
				GetDlgItemText( m_hWnd, IDC_ZIPPATH, pszZipPath, MAX_PATH) ;
				const char* pszPath = File( pszZipPath).GetBasePath().c_str() ;

				BROWSEINFO bi ;
				bi.hwndOwner		= hDlg ;
				bi.pidlRoot		= NULL ;
				bi.pszDisplayName	= NULL ;
				bi.lpszTitle		= "zip�t�@�C����ۑ�����t�H���_��I�����ĉ�����" ;
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

					File file( pszPath) ;
					string s = pszPath ;
					if( s == "")
					{
						return TRUE ;
					}
					if( file.GetFileName() != "")
					{
						s += "\\" ;
					}
					strPath = s + strPath ;
				}
				break ;
			}
		}
	}

	// ���݊m�F
	if( GetFileAttributes( strPath.c_str()) != 0xFFFFFFFF)
	{
		if( Profile::blnConfirmOverwrite)
		{
			if( MessageBox( hwndList, "zip�t�@�C�������łɑ��݂��Ă��܂��B\n�㏑�����Ă���낵���ł����H", APP_NAME, MB_YESNO | MB_ICONQUESTION) 
			    == IDNO)
			{
				return TRUE ;
			}
		}
		DeleteFile( strPath.c_str()) ;
	}

	// ���s
	ProgressDlg pd( &vecFileList, strPath) ;
	BOOL blnResult = DialogBoxParam( hInstance, MAKEINTRESOURCE( IDD_PROGRESS), hDlg, ProgressDlgProc, (LPARAM)&pd) ;

	if( Profile::blnDisplayLog && !blnResult)
	{
		MessageBox( hDlg, pd.GetErrLog().c_str(), APP_NAME, MB_OK) ;
		DeleteFile( strPath.c_str()) ;
	}
	else if( Profile::blnDisplayLog && !Profile::blnFailLogOnly)
	{
		MessageBox( hDlg, ( strPath + " �̍쐬�ɐ������܂���\n---\n" + pd.GetLog()).c_str(), APP_NAME, MB_OK) ;
	}

	// �������A�S�폜
	if(blnResult && Profile::blnClearOnSuccess)
	{
		for(int i = 0; i < vecFileList.size(); i++)
		{
			delete vecFileList[ i];
		}
		vecFileList.clear();
		ListView_DeleteAllItems( hwndList) ;
	}

	return TRUE ;
}


/******************************************************************************/
// �L�����Z��
//============================================================================//
// �X�V�F02/12/08(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::OnCancel( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	RECT rc ;
	GetWindowRect( hDlg, &rc) ;
	Profile::intX = rc.left ;
	Profile::intY = rc.top ;

	EndDialog( hDlg, FALSE) ;
	return TRUE ;
}


/******************************************************************************/
// �o�[�W�������
//============================================================================//
// �X�V�F02/12/08(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::OnAboutBtn( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	DialogBox( hInstance, MAKEINTRESOURCE(IDD_ABOUT), hDlg, AboutDlgProc) ;
	return TRUE ;
}


/******************************************************************************/
// �ݒ�{�^��
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::OnSettingBtn( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	SettingDlg sd ;
	DialogBoxParam( hInstance, MAKEINTRESOURCE( IDD_SETTING), hDlg, SettingDlgProc, (LPARAM)&sd) ;
	Profile::Save() ;
	return TRUE ;
}


/******************************************************************************/
// MP3�����`�F�b�N
//============================================================================//
// �X�V�F02/12/08(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::OnOnlyMp3Changed( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	Profile::blnOnlyMp3 = IsDlgButtonChecked( hDlg, IDC_ONLYMP3) ;

	return TRUE ;
}


/******************************************************************************/
// �u���E�Y�{�^��
//============================================================================//
// �X�V�F03/04/03(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::OnBrowseBtn( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	char pszZipPath[ MAX_PATH + 1] ;

	// ���ɓ��͂���Ă���p�X���擾
	GetDlgItemText( m_hWnd, IDC_ZIPPATH, pszZipPath, MAX_PATH) ;
	char pszPath[ MAX_PATH + 1];
	strcpy( pszPath, GetDirName( pszZipPath).c_str()) ;

	// ��Ȗڂ̃p�X���擾
	if( strcmp( pszPath, "") == 0 && vecFileList.size() != 0)
	{
		strcpy( pszPath, GetDirName( vecFileList[ 0]->GetFilePath()).c_str());
	}

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

		File file( pszPath) ;
		if( file.GetFileName() != "")
		{
			string s = string( pszPath) + "\\" ;
			SetDlgItemText( hDlg, IDC_ZIPPATH, s.c_str()) ;
		}
		else
		{
			SetDlgItemText( hDlg, IDC_ZIPPATH, pszPath) ;
		}
	}

	return TRUE ;
}


/******************************************************************************/
// �t�H���_�̑I���R�[���o�b�N
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

int CALLBACK BrowseProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lpData)
{
	switch(uMsg)
	{
		case BFFM_INITIALIZED:
			SendMessage(hWnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
			break;
	}
	return 0;
}

/******************************************************************************/
// ���X�g�̍��ڈړ�
//============================================================================//
// �X�V�F02/12/07(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::OnUpBtn( HWND, WPARAM, LPARAM)
{
	return ListMove( TRUE) ;
}

BOOL ZipDlg::OnDownBtn( HWND, WPARAM, LPARAM)
{
	return ListMove( FALSE) ;
}


/******************************************************************************/
// �ǉ�
//============================================================================//
// �X�V�F02/12/08(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::OnAddBtn( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	return ListAddFileDlg() ;
}


/******************************************************************************/
// �폜
//============================================================================//
// �X�V�F02/12/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::OnDelBtn( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	ListDel() ;
	return TRUE ;
}


/******************************************************************************/
// ���X�g�̒ʒm���b�Z�[�W
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::OnListNotify( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	NMHDR*	nmhdr = (NMHDR*)lParam ;

	// �L�[�_�E��
	if( nmhdr->code == LVN_KEYDOWN)
	{
		// �f���[�g
		if( ( (LPNMLVKEYDOWN)lParam)->wVKey == VK_DELETE)
		{
			ListDel( ) ;
			EnableWindow( hwndOk, ( ListView_GetItemCount( hwndList) != 0 ? TRUE : FALSE)) ;
		}
		// Ctrl + A
		else if( GetKeyState( VK_CONTROL) < 0 && ( (LPNMLVKEYDOWN)lParam)->wVKey == 'A')
		{
			ListView_SetItemState( hwndList, -1, LVIS_SELECTED, LVIS_SELECTED) ;
		}
		// Alt + ����
		else if( GetKeyState( VK_MENU) < 0)
		{
			if( ( (LPNMLVKEYDOWN)lParam)->wVKey == VK_UP)
			{
				ListMove( TRUE) ;
			}
			else if( ( (LPNMLVKEYDOWN)lParam)->wVKey == VK_DOWN)
			{
				ListMove( FALSE) ;
			}
		}
		return TRUE ;
	}
	// �I��ύX��
	else if( nmhdr->code == LVN_ITEMCHANGED)
	{
		EnableWindow( hwndOk, ( ListView_GetItemCount( hwndList) != 0 ? TRUE : FALSE)) ;
		EnableWindow( hwndDel, ( ListView_GetSelectedCount( hwndList) != 0 ? TRUE : FALSE)) ;
		return TRUE ;
	}
	// �w�b�_�����N���b�N
	else if( nmhdr->code == LVN_COLUMNCLICK)
	{
		ListView_SortItems( hwndList, ListSortProc, ( (NMLISTVIEW*)lParam)->iSubItem) ;
		ListReload() ;
		return TRUE ;
	}
	// �E�N���b�N
	else if( nmhdr->code == NM_RCLICK)
	{
		POINT pt ;
		GetCursorPos( &pt) ;
		HMENU hMenu = LoadMenu( hInstance, MAKEINTRESOURCE(IDR_POPMENU)) ;
		HMENU hMenuPopup = GetSubMenu( hMenu, 0) ;
		if( ListView_GetSelectedCount( hwndList) == 0)
		{
			EnableMenuItem( hMenuPopup, IDM_DEL, MF_BYCOMMAND | MF_GRAYED) ;
			EnableMenuItem( hMenuPopup, IDM_SETFOLDER, MF_BYCOMMAND | MF_GRAYED) ;
			EnableMenuItem( hMenuPopup, IDM_UP, MF_BYCOMMAND | MF_GRAYED) ;
			EnableMenuItem( hMenuPopup, IDM_DOWN, MF_BYCOMMAND | MF_GRAYED) ;
		}
		if( ListView_GetItemCount( hwndList) == 0)
		{
			EnableMenuItem( hMenuPopup, IDM_SORTFILE, MF_BYCOMMAND | MF_GRAYED) ;
			EnableMenuItem( hMenuPopup, IDM_SORTPATH, MF_BYCOMMAND | MF_GRAYED) ;
			EnableMenuItem( hMenuPopup, IDM_SORTZIP, MF_BYCOMMAND | MF_GRAYED) ;
		}

		DWORD dwID = TrackPopupMenu( hMenuPopup, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, 
			pt.x, pt.y, 0, hDlg, NULL) ;
		switch( dwID)
		{
			case IDM_ADD:
				ListAddFileDlg() ;
				break ;
			case IDM_DEL:
				ListDel() ;
				break ;
			case IDM_UP:
			case IDM_DOWN:
				ListMove( dwID == IDM_UP) ;
				break ;
			case IDM_SORTPATH:
				ListView_SortItems( hwndList, ListSortProc, 1) ;
				ListReload() ;
				break ;
			case IDM_SORTFILE:
				ListView_SortItems( hwndList, ListSortProc, 2) ;
				ListReload() ;
				break ;
			case IDM_SORTZIP:
				ListView_SortItems( hwndList, ListSortProc, 0) ;
				ListReload() ;
				break ;
			case IDM_SETFOLDER:
				ListSetFolder() ;
				break ;
		}

		DestroyMenu( hMenu) ;
	}

	return FALSE ;
}


/******************************************************************************/
// ���X�g�ɒǉ�
//============================================================================//
// �X�V�F03/02/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::ListAdd( Mp3File* pMp3, int intNum)
{
	intNum  = ( intNum < 0 ? ListView_GetItemCount( hwndList) : intNum) ;
	vector< Mp3File*>::iterator p = vecFileList.begin() + intNum ;

	// �A�C�R���擾
	SHFILEINFO shfi ;
	SHGetFileInfo( pMp3->GetFilePath().c_str(), 0, &shfi, sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	// �A�C�e���ݒ�
	LVITEM item = { LVIF_TEXT | LVIF_STATE | LVIF_PARAM | LVIF_IMAGE} ;
	intNum  = ( intNum < 0 ? ListView_GetItemCount( hwndList) : intNum) ;
	item.iItem		= intNum ;
	item.state		= LVIS_SELECTED ;
	item.lParam		= (LPARAM)pMp3 ;
	item.pszText		= (LPTSTR)pMp3->GetSaveName().c_str() ;
	item.iSubItem		= 0 ;
	item.iImage		= shfi.iIcon ;
	ListView_InsertItem( hwndList, &item) ;
	ListView_SetItemText( hwndList, intNum, 1, (LPTSTR)pMp3->GetFilePath().c_str()) ;

	// �����[�h
	ListReload() ;

	return TRUE ;
}


/******************************************************************************/
// �_�C�A���O����ǉ�
//============================================================================//
// �X�V�F03/02/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::ListAddFileDlg()
{
	OPENFILENAME	ofn ;
	char		pszFileBuf[ MAX_PATH * 256] = "" ;

	ZeroMemory( (LPVOID)&ofn, sizeof(OPENFILENAME) );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd ;
	ofn.lpstrFilter = "MP3(*.mp3)\0*.mp3\0���ׂẴt�@�C��\0*.*\0\0";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER | OFN_ALLOWMULTISELECT ;
	ofn.lpstrFile = pszFileBuf;
	ofn.nMaxFile = MAX_PATH * 256 ;
	if( !GetOpenFileName( &ofn))
	{
		return 0 ;
	}

	// �t�@�C�������
	char  szBuffer[512];
	char  szFolder[512];
	char  szPathName[512];
	LPTSTR lpEnd = strchr( pszFileBuf, '\0' );
	LPTSTR lpszNextString = lpEnd + 1;
	if( lpEnd)
	{
		if( *(lpszNextString) == '\0' )
		{
			//  �t�@�C�����P�����I������Ă��Ȃ�
			string str = pszFileBuf ;
			Mp3File* pMp3 = new Mp3File( str) ;
			ListAdd( pMp3, 0) ;
		}
		else
		{
			//  �t�@�C���������I������Ă���
			int  nLength;

			//  �t�H���_���̎擾
			lstrcpy( szFolder, pszFileBuf);
			nLength = lstrlen( szFolder );
			//  �u\�v�L���ŏI����Ă��Ȃ���Εt��
			if( szFolder[nLength] != '\\' )
			{
				lstrcat( szFolder, "\\" );
			}

			szBuffer[0] = '\0';
			while( *(lpszNextString) != '\0' )
			{
				//  �I�����ꂽ�t�@�C���̃p�X�����쐬
				lstrcpy( szPathName, szFolder );
				lstrcat( szPathName, lpszNextString );
				//  �m�F�p�̃f�[�^�쐬
				lstrcat( szBuffer, szPathName );
				//  ���̃t�@�C������T��
				lpEnd = strchr( lpszNextString, '\0' );
				lpszNextString = lpEnd + 1;

				Mp3File* pMp3 = new Mp3File( szBuffer) ;
				ListAdd( pMp3, 0) ;
				lstrcpy( szBuffer, "") ;
			}
		}
	}

	return TRUE ;
}


/******************************************************************************/
// ���X�g�폜
//============================================================================//
// �X�V�F03/04/03(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::ListDel()
{
	int intCount = ListView_GetItemCount( hwndList) ;
	BOOL blnFocus = FALSE ;
	for( int i = 0; i < intCount; i++)
	{
		// �I������Ă����ꍇ
		if( ListView_GetItemState( hwndList, intCount - i - 1, LVIS_SELECTED))
		{
			// ���̗v�f����폜
			ListView_DeleteItem( hwndList, intCount - i - 1) ;
			vector<Mp3File*>::iterator p = vecFileList.begin() + intCount - i - 1 ;
			delete vecFileList[ intCount - i - 1] ;
			vecFileList.erase( p) ;

			continue ;
		}
	}

	// �t�H�[�J�X�̂���A�C�e����I��
	for( i = 0; i < ListView_GetItemCount( hwndList); i++)
	{
		if( ListView_GetItemState( hwndList, i, LVIS_FOCUSED))
		{
			ListView_SetItemState( hwndList, i, LVIS_SELECTED, LVIS_SELECTED) ;
		}
	}

	EnableWindow( hwndOk, ( ListView_GetItemCount( hwndList) != 0 ? TRUE : FALSE)) ;
	return TRUE ;
}


/******************************************************************************/
// ���X�g�ړ�
//============================================================================//
// �X�V�F02/12/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::ListMove( BOOL blnUp)
{
	int	intSize		= ListView_GetItemCount( hwndList) ;
	bool	blnMoved	= false ;	// �����]�n�����邩�ǂ���
	int	intIndex ;
	
	vector<BOOL> vecSelected ;
	BOOL blnTop = TRUE ;

	for( int i = 0; i < intSize; i++)
	{
		intIndex = blnUp ? i : intSize - i - 1 ;
		
		if( ListView_GetItemState( hwndList, intIndex, LVIS_SELECTED))
		{
			if( !blnMoved)
			{
				continue ;
			}

			// ���X�g�r���[
			ListView_DeleteItem( hwndList, intIndex) ;

			if( blnUp) // index����
			{
				ListAdd( vecFileList[ intIndex], intIndex - 1) ;
			}
			else
			{
				ListAdd( vecFileList[ intIndex], intIndex + 1) ;
			}

			if( blnTop)
			{
				ListView_SetItemState( hwndList, intIndex + ( blnUp ? -1 : 1), LVIS_FOCUSED, LVIS_FOCUSED) ;
				ListView_EnsureVisible( hwndList, intIndex + ( blnUp ? -1 : 1), FALSE) ;
				blnTop = FALSE ;
			}

			// �h�L�������g
			ListReload() ;
		}
		else
		{
			blnMoved = true ;
		}
	}

	return TRUE ;
}


/******************************************************************************/
// ���X�g�����[�h
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�c���[�r���[��vecFileList �̑Ή�����蒼���B
// �⑫�F�Ȃ��B
//============================================================================//

void ZipDlg::ListReload( BOOL blnReloadLabel)
{
	vecFileList.clear() ;

	int intMax = ListView_GetItemCount( hwndList) ;
	for( int i = 0; i < intMax; i++)
	{
		LVITEM item = { LVIF_PARAM} ;
		item.iItem = i ;
		ListView_GetItem( hwndList, &item) ;

		Mp3File* pMp3 = (Mp3File*)item.lParam ;
		vecFileList.push_back( pMp3) ;

		if( blnReloadLabel)
		{
			ListView_SetItemText( hwndList, i, 0, (LPTSTR)pMp3->GetSaveName().c_str()) ;
			ListView_SetItemText( hwndList, i, 1, (LPTSTR)pMp3->GetFilePath().c_str()) ;
			ListView_SetColumnWidth( hwndList, 0, LVSCW_AUTOSIZE) ;
		}
	}
}


/******************************************************************************/
// ���X�g�\�[�g
//============================================================================//
// �X�V�F02/12/08(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

int CALLBACK ListSortProc( LPARAM lp1, LPARAM lp2, LPARAM lp3)
{
	int i ;
	if( lp3 == 0)
	{
		string s1 = ( (Mp3File*)lp1)->GetSaveName() ;
		string s2 = ( (Mp3File*)lp2)->GetSaveName() ;
		i = _mbscmp( (const unsigned char*)s1.c_str(), (const unsigned char*)s2.c_str()) ;
	}
	else if( lp3 == 1)
	{
		string s1 = ( (Mp3File*)lp1)->GetFilePath() ;
		string s2 = ( (Mp3File*)lp2)->GetFilePath() ;
		i = _mbscmp( (const unsigned char*)s1.c_str(), (const unsigned char*)s2.c_str()) ;
	}
	else if( lp3 == 2)
	{
		string s1 = ( (Mp3File*)lp1)->GetFileName() ;
		string s2 = ( (Mp3File*)lp2)->GetFileName() ;
		i = _mbscmp( (const unsigned char*)s1.c_str(), (const unsigned char*)s2.c_str()) ;
	}
	return i ;
}


/******************************************************************************/
// �t�H���_����ݒ�
//============================================================================//
// �X�V�F03/02/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void ZipDlg::ListSetFolder()
{
	// �����l�擾
	string str ;
	int intCount = ListView_GetItemCount( hwndList) ;
	int i ;
	for( i = 0; i < intCount; i++)
	{
		if( ListView_GetItemState( hwndList, i, LVIS_SELECTED))
		{
			str = GetDirName( vecFileList[ i]->GetSaveName()) ;
			break ;
		}
	}

	// �ҏW
	DialogBoxParam( hInstance, MAKEINTRESOURCE( IDD_FOLDERNAME), m_hWnd, FileNameDlgProc, (LPARAM)&str) ;
	if( str.size() > 0 && str[ str.size() - 1] != '\\')
	{
		str += "\\" ;
	}

	intCount = ListView_GetItemCount( hwndList) ;
	BOOL blnFocus = FALSE ;
	for( i = 0; i < intCount; i++)
	{
		// �I������Ă����ꍇ
		if( ListView_GetItemState( hwndList, i, LVIS_SELECTED))
		{
			// �폜
			string s = GetFileName( vecFileList[ i]->GetSaveName()) ;
			vecFileList[ i]->SetSaveName( str + s) ;
		}
	}

	ListReload( TRUE) ;
}


/******************************************************************************/
// �t�H���_���w��_�C�A���O
//============================================================================//
// �X�V�F03/02/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL CALLBACK FileNameDlgProc( HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch( uiMsg)
	{
		case WM_INITDIALOG:
		{
			SetWindowLong( hDlg, GWL_USERDATA, lParam) ;
			string* pstr = (string*)lParam ;
			SetWindowText( GetDlgItem( hDlg, IDC_EDIT), pstr->c_str()) ;
			return TRUE ;
			break ;
		}

		case WM_COMMAND:
			if( LOWORD(wParam) == IDOK)
			{
				string* pstr = (string*)GetWindowLong( hDlg, GWL_USERDATA) ;
				char pszBuf[ 1024] ;
				GetWindowText( GetDlgItem( hDlg, IDC_EDIT), pszBuf, 1024) ;
				*pstr = pszBuf ;
				EndDialog( hDlg, TRUE) ;
				return TRUE ;
			}
			else if( LOWORD(wParam) == IDCANCEL)
			{
				EndDialog( hDlg, FALSE) ;
				return TRUE ;
			}
	}

	return FALSE ;
}

// ZipDlg.cpp
//============================================================================//
// �X�V�F02/12/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "ZipDlg.h"
#include "Mp3File.h"
#include "resource.h"
extern "C" {
#include "crc32.h"
}

HWND ghwndList ;


#define CRC32(c, b) (crc_table[((int)(c) ^ (b)) & 0xff] ^ ((c) >> 8))
#define DO1(buf)  crc = CRC32(crc, *buf++)
#define DO2(buf)  DO1(buf); DO1(buf)
#define DO4(buf)  DO2(buf); DO2(buf)
#define DO8(buf)  DO4(buf); DO4(buf)

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
// ���X�g�ɒǉ�
//============================================================================//
// �X�V�F02/12/08(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::ListAdd( File* pMp3, int intNum)
{
	intNum  = ( intNum < 0 ? ListView_GetItemCount( hwndList) : intNum) ;

	// �A�C�e���ݒ�
	LVITEM item = { LVIF_TEXT | LVIF_STATE | LVIF_PARAM } ;
	intNum  = ( intNum < 0 ? ListView_GetItemCount( hwndList) : intNum) ;
	item.iItem = intNum ;
	item.state = LVIS_SELECTED ;
	item.lParam = (LPARAM)pMp3 ;

	// �ݒ�
	string str = blnFullPath ? pMp3->GetFilePath() : pMp3->GetFileName() ;
	item.pszText	= (LPTSTR)str.c_str() ;
	item.iSubItem	= 0 ;
	ListView_InsertItem( hwndList, &item) ;

	// �����[�h
	ListReload() ;

	return TRUE ;
}


/******************************************************************************/
// ���X�g�폜
//============================================================================//
// �X�V�F02/12/09(��)
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
			// �폜
			ListView_DeleteItem( hwndList, intCount - i - 1) ;
			vector<File*>::iterator p = vecFileList.begin() + intCount - i - 1 ;
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
// �X�V�F02/12/09(��)
// �T�v�F�Ȃ��B
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

		File* pMp3 = (File*)item.lParam ;
		vecFileList.push_back( pMp3) ;

		if( blnReloadLabel)
		{
			string s = ( blnFullPath ? pMp3->GetFilePath() : pMp3->GetFileName()) ;
			ListView_SetItemText( hwndList, i, 0, (LPTSTR)s.c_str()) ;
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
	if( lp3)
	{
		string s1 = ( (File*)lp1)->GetFilePath() ;
		string s2 = ( (File*)lp2)->GetFilePath() ;
		i = _mbscmp( (const unsigned char*)s1.c_str(), (const unsigned char*)s2.c_str()) ;
	}
	else
	{
		string s1 = ( (File*)lp1)->GetFileName() ;
		string s2 = ( (File*)lp2)->GetFileName() ;
		i = _mbscmp( (const unsigned char*)s1.c_str(), (const unsigned char*)s2.c_str()) ;
	}
	return i ;
}


/******************************************************************************/
//		���b�Z�[�W�n���h��
/******************************************************************************/
// ���b�Z�[�W�}�b�v��`
//============================================================================//
// �X�V�F02/12/08(��)
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

		ON_COMMAND( IDC_FULLPATH	, OnFullPathChanged)
		ON_COMMAND( IDC_ONLYMP3		, OnOnlyMp3Changed)

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
// �X�V�F02/12/07(�y)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::OnInitDialog( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// �ϐ�������
	blnFullPath = FALSE ;
	blnOnlyMp3 = TRUE ;

	// �A�C�R���ݒ�
	HICON hIcon = LoadIcon( GetModuleHandle( NULL), MAKEINTRESOURCE( IDI_ICON));
	SendMessage( hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	SendMessage( hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	DestroyIcon( hIcon) ;

	// �E�C���h�E�n���h���擾
	hwndList	= GetDlgItem( hDlg, IDC_LIST) ;
	hwndOk		= GetDlgItem( hDlg, IDOK) ;
	hwndDel		= GetDlgItem( hDlg, IDC_DEL) ;
	ghwndList	= hwndList ;

	DragAcceptFiles( hwndList, TRUE) ;
	ListView_DeleteAllItems( hwndList) ;
	LVCOLUMN lvclm ;
	lvclm.mask	= LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ;
	lvclm.fmt	= LVCFMT_LEFT ;
	lvclm.pszText	= "�Ȗ�" ;
	lvclm.cx	= 250 ;
	lvclm.iSubItem	= 0 ;
	ListView_InsertColumn( hwndList, 0, &lvclm); 

	// ������
	EnableWindow( hwndDel, FALSE) ;
	EnableWindow( hwndOk, FALSE) ;
	CheckDlgButton( hDlg, IDC_ONLYMP3, BST_CHECKED) ;

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
// �X�V�F02/12/08(��)
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
		if( !blnOnlyMp3 || s.rfind( ".mp3") == s.size() - 4 || 
		    s.rfind( ".MP3") == s.size() - 4)
		{
			File* pMp3 = new File( pszBuf) ;
			ListAdd( pMp3, 0) ;
		}
	}
	DragFinish( hDrop) ;
	return TRUE ;
}


/******************************************************************************/
// OK
//============================================================================//
// �X�V�F02/12/08(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::OnOk( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	fire() ;
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
	return TRUE ;
}


/******************************************************************************/
// �t���p�X
//============================================================================//
// �X�V�F02/12/08(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::OnFullPathChanged( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	blnFullPath = IsDlgButtonChecked( hDlg, IDC_FULLPATH) ;
	ListReload( TRUE) ;

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
	blnOnlyMp3 = IsDlgButtonChecked( hDlg, IDC_ONLYMP3) ;

	return TRUE ;
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
	OPENFILENAME	ofn ;
	char		pszFileBuf[ 10000] = "" ;

	ZeroMemory( (LPVOID)&ofn, sizeof(OPENFILENAME) );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hDlg ;
	ofn.lpstrFilter = "MP3(*.mp3)\0*.mp3\0���ׂẴt�@�C��\0*.*\0\0";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER | OFN_ALLOWMULTISELECT ;
	ofn.lpstrFile = pszFileBuf;
	ofn.nMaxFile = 100000;
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
	if( lpEnd )
	{
		if( *(lpszNextString) == '\0' )
		{
			//  �t�@�C�����P�����I������Ă��Ȃ�
			string str = pszFileBuf ;
			File* pMp3 = new File( str) ;
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

				File* pMp3 = new File( szBuffer) ;
				ListAdd( pMp3, 0) ;
				lstrcpy( szBuffer, "") ;
			}
		}
	}

	return TRUE ;
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
	int intCount = ListView_GetItemCount( hwndList) ;
	BOOL blnFocus = FALSE ;
	for( int i = 0; i < intCount; i++)
	{
		// �I������Ă����ꍇ
		if( ListView_GetItemState( hwndList, intCount - i - 1, LVIS_SELECTED))
		{
			// �폜
			ListView_DeleteItem( hwndList, intCount - i - 1) ;
			vector<File*>::iterator p = vecFileList.begin() + intCount - i - 1 ;
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

	// OK �{�^���̗L����
	EnableWindow( hwndOk, ( ListView_GetItemCount( hwndList) != 0 ? TRUE : FALSE)) ;
	return TRUE ;
}


/******************************************************************************/
// ���X�g�̒ʒm���b�Z�[�W
//============================================================================//
// �X�V�F02/12/08(��)
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
		ListView_SortItems( hwndList, ListSortProc, blnFullPath) ;
		ListReload() ;
		return TRUE ;
	}

	return FALSE ;
}


/******************************************************************************/
//		���k
/******************************************************************************/
// ���k���s
//============================================================================//
// �X�V�F02/12/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::fire()
{
	if( vecFileList.size() == 0)
	{
		return 0 ;
	}

	// �e��l�ݒ�
	char pszZipPath[ MAX_PATH + 1] ;
	GetDlgItemText( m_hWnd, IDC_ZIPPATH, pszZipPath, MAX_PATH) ;
	string	strPath = pszZipPath ;
	string	strFileName = File( strPath).GetFileName() ;

	// zip �t�@�C���̃p�X����
	if( strPath == "" || strPath.rfind( '\\') == strPath.size())
	{
		strPath += string( "album.zip.mp3") ;
	}
	if( strPath.find( '\\') == string::npos)
	{
		string strDir = vecFileList[ 0]->GetFilePath() ;
		strDir = strDir.substr( 0, strDir.rfind( '\\') + 1) ;
		strPath = strDir + strPath ;
	}
	if( GetFileAttributes( strPath.c_str()) != 0xFFFFFFFF)
	{
		if( MessageBox( hwndList, "zip�t�@�C�������łɑ��݂��Ă��܂��B\n�㏑�����Ă���낵���ł����H", "", MB_YESNO | MB_ICONQUESTION) 
		    == IDNO)
		{
			return 0 ;
		}
		else
		{
			DeleteFile( strPath.c_str()) ;
		}
	}

	// �����A��
	FILE* fzip ;
	fzip = fopen( strPath.c_str(), "wb") ;
	long lPrevTail = 0 ;
	vector<Mp3File*> vecMp3FileList ;
	int i ;

	for( i = 0; i < vecFileList.size(); i++)
	{
		Mp3File* pMp3File = new Mp3File( vecFileList[ i]) ;
		pMp3File->lLocalHeader = lPrevTail ;
		vecMp3FileList.push_back( pMp3File) ;

		// Local file �w�b�_�o��
		OutputLocalFileHeader( pMp3File, fzip) ;

		// MP3 �t�@�C���o��
		BYTE bBuf[ SBSZ] ;
		FILE* fMp3 = fopen( pMp3File->GetFilePath().c_str(), "rb") ;
		int intRead ;
		unsigned long ulCrc = 0 ;
		while( TRUE)
		{
			intRead = fread( bBuf, sizeof( BYTE), SBSZ, fMp3) ;
			if( ferror( fMp3))
			{
				break ;
			}
			ulCrc = crc32( ulCrc, (uch*)bBuf, intRead);
			fwrite( bBuf, sizeof( BYTE), intRead, fzip) ;
			if( feof( fMp3))
			{
				break ;
			}
		}

		// CRC�����㏑��
		long lTail = ftell( fzip) ;
		pMp3File->ulCrc = ulCrc ;
		if( fseek( fzip, lPrevTail + 14, SEEK_SET) == 0)
		{
			PUTLG( ulCrc, fzip) ;
			fseek( fzip, lTail, SEEK_SET) ;
		}
		lPrevTail = lTail ;

		fclose( fMp3) ;
	}

	// Central directory �̏o��
	int	intFileNum = 0 ;
	ULONG	ulTotalSize = 0 ;
	ULONG	ulOffset = ftell( fzip) ;

	for( i = 0; i < vecFileList.size(); i++)
	{
		OutputCentralDirectory( vecMp3FileList[ i], fzip) ;

		intFileNum++ ;
		ulTotalSize += vecMp3FileList[ i]->ulSize ;

		delete vecMp3FileList[ i] ;
	}

	// End of central directory record �̏o��
	ulOffset = ftell( fzip) - ulOffset ;
	OutputEndCentralDirectory( intFileNum, ulTotalSize, ulOffset, fzip) ;

	fclose( fzip) ;

	return 1 ;
}


/******************************************************************************/
// �w�b�_�o��
//============================================================================//
// �X�V�F02/12/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::OutputLocalFileHeader( Mp3File* pMp3File, FILE* fzip)
{
	char pszFileName[ _MAX_FNAME * 3] ;	// �t�@�C�����G���R�[�h�ōő�R�{�ɂȂ�
	strcpy( pszFileName, pMp3File->GetFileName().c_str()) ;

	// �w�b�_�̏o�� (zipfile.c (putlocal) : 1040�s��) 
	// http://www.goice.co.jp/member/mo/formats/zip.html
	PUTLG( 0x04034b50L, fzip) ;			// 
	PUTSH( 10, fzip) ;				// ver  = 10
	PUTSH( 0, fzip) ;				// lflg	= 0
	PUTSH( 0, fzip);				// how	= 0 (STORE)
	PUTLG( pMp3File->lModifiedTime, fzip) ;		// tim			[����]
	PUTLG( 0, fzip) ;				// crc	= 0 [CRC���B���Ƃŏ㏑��]
	PUTLG( pMp3File->ulSize, fzip);			// siz
	PUTLG( pMp3File->ulSize, fzip);			// len
	PUTSH( pMp3File->intFileNameSize, fzip) ;	// nam			[length of iname]
	PUTSH( 100, fzip) ;				// ext
	fputs( pszFileName, fzip) ;
	char pszBuf[ 101] ;
	for( int i = 0; i < 100; i++)
	{
		pszBuf[ i] = ' ' ;
	}
	pszBuf[ 100] = '\0' ;
	fputs( pszBuf, fzip) ;

	return TRUE ;
}


/******************************************************************************/
// Central Directory �o��
//============================================================================//
// �X�V�F02/12/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::OutputCentralDirectory( Mp3File* pMp3File, FILE* fzip)
{
	char pszFileName[ _MAX_FNAME * 3] ;	// �t�@�C�����G���R�[�h�ōő�R�{�ɂȂ�
	strcpy( pszFileName, pMp3File->GetFileName().c_str()) ;

	PUTLG( 0x02014b50L, fzip) ;			// 
	PUTSH( 0x0B17, fzip) ;				// Version made by = 0x0B17
	PUTSH( 10, fzip) ;				// Version needed to extract = 10
	PUTSH( 0, fzip) ;				// lflg	= 0
	PUTSH( 0, fzip);				// how	= 0 (STORE)
	PUTLG( pMp3File->lModifiedTime, fzip) ;		// tim			[����]
	PUTLG( pMp3File->ulCrc, fzip) ;			// crc	= 0 [CRC���B���Ƃŏ㏑��]
	PUTLG( pMp3File->ulSize, fzip);			// siz
	PUTLG( pMp3File->ulSize, fzip);			// len
	PUTSH( pMp3File->intFileNameSize, fzip) ;	// nam			[length of iname]
	PUTSH( 17, fzip) ;				// Extra Fild Length
	PUTSH( 0, fzip) ;				// File Comment Length
	PUTSH( 0, fzip) ;				// Disk Number		[???�@zip32.dll �ł͕K�� 0]
	PUTSH( 0, fzip) ;				// Internal file attributes [0 = BINARY]
	PUTLG( 0, fzip) ;				// External file attributes
	PUTLG( pMp3File->lLocalHeader, fzip) ;		// Relative offset of local header
	fputs( pszFileName, fzip) ;
	char pszBuf[ 18] ;
	for( int i = 0; i < 17; i++)
	{
		pszBuf[ i] = ' ' ;
	}
	pszBuf[ 17] = '\0' ;
	fputs( pszBuf, fzip) ;

	return TRUE ;

}


/******************************************************************************/
// End of central directory record �̏o��
//============================================================================//
// �X�V�F02/12/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL ZipDlg::OutputEndCentralDirectory( int intFileNum, ULONG ulTotalSize, ULONG ulOffset, FILE* fzip)
{
	PUTLG( 0x06054b50L, fzip) ;	// 
	PUTSH( 0, fzip) ;		// number of this disk
	PUTSH( 0, fzip) ;		// Number of the disk with the start of the central directory
	PUTSH( intFileNum, fzip) ;	// Total number of entries in the central dir on this disk
	PUTSH( intFileNum, fzip) ;	// Total number of entries in the central dir
	PUTLG( ulOffset, fzip) ;	// Size of the central directory
	PUTLG( ulTotalSize, fzip) ;	// Offset of start of central directory with respect to the starting disk number
	PUTSH( 0, fzip);		// zipfile comment length

	return TRUE ;
}
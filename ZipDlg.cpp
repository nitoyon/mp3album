
// ZipDlg.cpp
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "ZipDlg.h"
#include "resource.h"
#include "about.h"
#include "Zip.h"


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 更新：02/12/07(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

ZipDlg::ZipDlg() 
{
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 更新：02/12/07(土)
// 概要：なし。
// 補足：なし。
//============================================================================//

ZipDlg::~ZipDlg() 
{
}


/******************************************************************************/
// リストに追加
//============================================================================//
// 更新：02/12/08(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipDlg::ListAdd( File* pMp3, int intNum)
{
	intNum  = ( intNum < 0 ? ListView_GetItemCount( hwndList) : intNum) ;

	// アイテム設定
	LVITEM item = { LVIF_TEXT | LVIF_STATE | LVIF_PARAM } ;
	intNum  = ( intNum < 0 ? ListView_GetItemCount( hwndList) : intNum) ;
	item.iItem = intNum ;
	item.state = LVIS_SELECTED ;
	item.lParam = (LPARAM)pMp3 ;

	// 設定
	string str = blnFullPath ? pMp3->GetFilePath() : pMp3->GetFileName() ;
	item.pszText	= (LPTSTR)str.c_str() ;
	item.iSubItem	= 0 ;
	ListView_InsertItem( hwndList, &item) ;

	// リロード
	ListReload() ;

	return TRUE ;
}


/******************************************************************************/
// リスト削除
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipDlg::ListDel()
{
	int intCount = ListView_GetItemCount( hwndList) ;
	BOOL blnFocus = FALSE ;
	for( int i = 0; i < intCount; i++)
	{
		// 選択されていた場合
		if( ListView_GetItemState( hwndList, intCount - i - 1, LVIS_SELECTED))
		{
			// 削除
			ListView_DeleteItem( hwndList, intCount - i - 1) ;
			vector<File*>::iterator p = vecFileList.begin() + intCount - i - 1 ;
			vecFileList.erase( p) ;

			continue ;
		}
	}

	// フォーカスのあるアイテムを選択
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
// リスト移動
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipDlg::ListMove( BOOL blnUp)
{
	int	intSize		= ListView_GetItemCount( hwndList) ;
	bool	blnMoved	= false ;	// 動く余地があるかどうか
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

			// リストビュー
			ListView_DeleteItem( hwndList, intIndex) ;

			if( blnUp) // index減少
			{
				ListAdd( vecFileList[ intIndex], intIndex - 1) ;
			}
			else
			{
				ListAdd( vecFileList[ intIndex], intIndex + 1) ;
			}

			// ドキュメント
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
// リストリロード
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
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
			ListView_SetColumnWidth( hwndList, 0, LVSCW_AUTOSIZE) ;
		}
	}
}


/******************************************************************************/
// リストソート
//============================================================================//
// 更新：02/12/08(日)
// 概要：なし。
// 補足：なし。
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
//		メッセージハンドラ
/******************************************************************************/
// メッセージマップ定義
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
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
// ダイアログ初期化
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipDlg::OnInitDialog( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// 変数初期化
	blnFullPath = FALSE ;
	blnOnlyMp3 = TRUE ;
	hInstance = (HINSTANCE)GetWindowLong( hDlg, GWL_HINSTANCE) ;

	// アイコン設定
	HICON hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ICON));
	SendMessage( hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	SendMessage( hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	DestroyIcon( hIcon) ;

	// ウインドウハンドル取得
	hwndList	= GetDlgItem( hDlg, IDC_LIST) ;
	hwndOk		= GetDlgItem( hDlg, IDOK) ;
	hwndDel		= GetDlgItem( hDlg, IDC_DEL) ;

	DragAcceptFiles( hwndList, TRUE) ;
	ListView_DeleteAllItems( hwndList) ;
	LVCOLUMN lvclm ;
	lvclm.mask	= LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ;
	lvclm.fmt	= LVCFMT_LEFT ;
	lvclm.pszText	= "曲名" ;
	lvclm.cx	= 250 ;
	lvclm.iSubItem	= 0 ;
	ListView_InsertColumn( hwndList, 0, &lvclm); 

	// 初期化
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
// リストプロシージャ
//============================================================================//
// 更新：02/12/08(日)
// 概要：なし。
// 補足：なし。
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
// サイズ変更
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipDlg::OnSize( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	return TRUE ;
}


/******************************************************************************/
// OK
//============================================================================//
// 更新：02/12/08(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipDlg::OnOk( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// 各種値設定
	char pszZipPath[ MAX_PATH + 1] ;
	GetDlgItemText( m_hWnd, IDC_ZIPPATH, pszZipPath, MAX_PATH) ;
	string	strPath = pszZipPath ;
	string	strFileName = File( strPath).GetFileName() ;

	// zip ファイルのパス処理
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
		if( MessageBox( hwndList, "zipファイルがすでに存在しています。\n上書きしてもよろしいですか？", "", MB_YESNO | MB_ICONQUESTION) 
		    == IDNO)
		{
			return TRUE ;
		}
		else
		{
			DeleteFile( strPath.c_str()) ;
		}
	}

	// 実行
	fire( strPath, hwndList) ;
	MessageBox( hDlg, ( strPath + " に作成が完了しました。").c_str(), "完了", MB_OK) ;
	ListReload( TRUE) ;

	return TRUE ;
}


/******************************************************************************/
// キャンセル
//============================================================================//
// 更新：02/12/08(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipDlg::OnCancel( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	EndDialog( hDlg, FALSE) ;
	return TRUE ;
}


/******************************************************************************/
// バージョン情報
//============================================================================//
// 更新：02/12/08(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipDlg::OnAboutBtn( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	DialogBox( hInstance, MAKEINTRESOURCE(IDD_ABOUT), hDlg, AboutDlgProc) ;
	return TRUE ;
}


/******************************************************************************/
// フルパス
//============================================================================//
// 更新：02/12/08(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipDlg::OnFullPathChanged( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	blnFullPath = IsDlgButtonChecked( hDlg, IDC_FULLPATH) ;
	ListReload( TRUE) ;

	return TRUE ;
}


/******************************************************************************/
// MP3だけチェック
//============================================================================//
// 更新：02/12/08(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipDlg::OnOnlyMp3Changed( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	blnOnlyMp3 = IsDlgButtonChecked( hDlg, IDC_ONLYMP3) ;

	return TRUE ;
}


/******************************************************************************/
// ブラウズボタン
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipDlg::OnBrowseBtn( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	BROWSEINFO bi ;
	bi.hwndOwner		= hDlg ;
	bi.pidlRoot		= NULL ;
	bi.pszDisplayName	= NULL ;
	bi.lpszTitle		= "フォルダを選択して下さい" ;
	bi.ulFlags		= BIF_SHAREABLE ;
	bi.lpfn			= NULL ;
	bi.lParam		= 0 ;
	bi.iImage		= NULL ;

	LPITEMIDLIST idlist = SHBrowseForFolder( &bi);
	if( idlist)
	{
		char pszPath[ MAX_PATH] ;
		SHGetPathFromIDList( idlist, pszPath) ;
		CoTaskMemFree( idlist) ;

		string s = string( pszPath) + "\\" ;
		SetDlgItemText( hDlg, IDC_ZIPPATH, s.c_str()) ;
	}

	return TRUE ;
}


/******************************************************************************/
// リストの項目移動
//============================================================================//
// 更新：02/12/07(土)
// 概要：なし。
// 補足：なし。
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
// 追加
//============================================================================//
// 更新：02/12/08(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipDlg::OnAddBtn( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	OPENFILENAME	ofn ;
	char		pszFileBuf[ 10000] = "" ;

	ZeroMemory( (LPVOID)&ofn, sizeof(OPENFILENAME) );
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hDlg ;
	ofn.lpstrFilter = "MP3(*.mp3)\0*.mp3\0すべてのファイル\0*.*\0\0";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER | OFN_ALLOWMULTISELECT ;
	ofn.lpstrFile = pszFileBuf;
	ofn.nMaxFile = 100000;
	if( !GetOpenFileName( &ofn))
	{
		return 0 ;
	}

	// ファイル名解析
	char  szBuffer[512];
	char  szFolder[512];
	char  szPathName[512];
	LPTSTR lpEnd = strchr( pszFileBuf, '\0' );
	LPTSTR lpszNextString = lpEnd + 1;
	if( lpEnd )
	{
		if( *(lpszNextString) == '\0' )
		{
			//  ファイルが１つしか選択されていない
			string str = pszFileBuf ;
			File* pMp3 = new File( str) ;
			ListAdd( pMp3, 0) ;
		}
		else
		{
			//  ファイルが複数選択されている
			int  nLength;

			//  フォルダ名の取得
			lstrcpy( szFolder, pszFileBuf);
			nLength = lstrlen( szFolder );
			//  「\」記号で終わっていなければ付加
			if( szFolder[nLength] != '\\' )
			{
				lstrcat( szFolder, "\\" );
			}

			szBuffer[0] = '\0';
			while( *(lpszNextString) != '\0' )
			{
				//  選択されたファイルのパス名を作成
				lstrcpy( szPathName, szFolder );
				lstrcat( szPathName, lpszNextString );
				//  確認用のデータ作成
				lstrcat( szBuffer, szPathName );
				//  次のファイル名を探す
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
// 削除
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipDlg::OnDelBtn( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	int intCount = ListView_GetItemCount( hwndList) ;
	BOOL blnFocus = FALSE ;
	for( int i = 0; i < intCount; i++)
	{
		// 選択されていた場合
		if( ListView_GetItemState( hwndList, intCount - i - 1, LVIS_SELECTED))
		{
			// 削除
			ListView_DeleteItem( hwndList, intCount - i - 1) ;
			vector<File*>::iterator p = vecFileList.begin() + intCount - i - 1 ;
			vecFileList.erase( p) ;

			continue ;
		}
	}

	// フォーカスのあるアイテムを選択
	for( i = 0; i < ListView_GetItemCount( hwndList); i++)
	{
		if( ListView_GetItemState( hwndList, i, LVIS_FOCUSED))
		{
			ListView_SetItemState( hwndList, i, LVIS_SELECTED, LVIS_SELECTED) ;
		}
	}

	// OK ボタンの有効化
	EnableWindow( hwndOk, ( ListView_GetItemCount( hwndList) != 0 ? TRUE : FALSE)) ;
	return TRUE ;
}


/******************************************************************************/
// リストの通知メッセージ
//============================================================================//
// 更新：02/12/08(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL ZipDlg::OnListNotify( HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	NMHDR*	nmhdr = (NMHDR*)lParam ;

	// キーダウン
	if( nmhdr->code == LVN_KEYDOWN)
	{
		// デリート
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
		// Alt + ↑↓
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
	// 選択変更時
	else if( nmhdr->code == LVN_ITEMCHANGED)
	{
		EnableWindow( hwndOk, ( ListView_GetItemCount( hwndList) != 0 ? TRUE : FALSE)) ;
		EnableWindow( hwndDel, ( ListView_GetSelectedCount( hwndList) != 0 ? TRUE : FALSE)) ;
		return TRUE ;
	}
	// ヘッダ部分クリック
	else if( nmhdr->code == LVN_COLUMNCLICK)
	{
		ListView_SortItems( hwndList, ListSortProc, blnFullPath) ;
		ListReload() ;
		return TRUE ;
	}

	return FALSE ;
}

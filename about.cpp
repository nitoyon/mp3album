
// main.cpp
//============================================================================//
// 更新：02/06/26(水)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "include.h"
#include "resource.h"


/******************************************************************************/
//		定義
/******************************************************************************/

#define VERSION_URL	"http://www.nitoyon.com/"
#define VERSION_MAIL	"soft@nitoyon.com"


/******************************************************************************/
//		グローバル変数
/******************************************************************************/

BOOL	blnUrlHover ;
BOOL	blnMailHover ;
HWND	hwndUrl ;
HWND	hwndMail ;
HFONT	hFontLink ;
WNDPROC	wpcStatic ;



/******************************************************************************/
// スタティックプロシージャ
//============================================================================//
// 更新：02/06/26(水)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL CALLBACK LinkStaticProc( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch( uiMsg)
	{
		case WM_PAINT :
		{
			HDC		hdc ;
			PAINTSTRUCT	ps;

			hdc = BeginPaint(hWnd, &ps) ;
			SelectObject( hdc, hFontLink) ;
			SetBkColor( hdc, GetSysColor( COLOR_BTNFACE));
			SetBkMode(hdc, TRANSPARENT);

			if( hWnd == hwndUrl)
			{
				SetTextColor( hdc, ( blnUrlHover ? RGB( 255, 128, 0) : RGB(0,0,255))) ;
				TextOut( hdc, 0, 0, VERSION_URL, ( sizeof( VERSION_URL) - 1) / sizeof( '0'));
			}
			else if( hWnd == hwndMail)
			{
				SetTextColor( hdc, ( blnMailHover ? RGB( 255, 128, 0) : RGB(0,0,255))) ;
				TextOut( hdc, 0, 0, VERSION_MAIL, ( sizeof( VERSION_MAIL) - 1) / sizeof( '0'));
			}

			EndPaint(hWnd, &ps);
			return TRUE ;
		}

		case WM_SETCURSOR :
		{
			if( hWnd == hwndUrl)
			{
				blnUrlHover  = true ;
				blnMailHover = false ;
			}
			else if( hWnd == hwndMail)
			{
				blnUrlHover  = false ;
				blnMailHover = true ;
			}

			// 手カーソル読みとり
			HCURSOR hCurHand	= LoadCursor( GetModuleHandle( NULL), MAKEINTRESOURCE( IDC_CURSOR_HAND)) ;
			SetCursor( hCurHand) ;
			DestroyCursor( hCurHand) ;
			InvalidateRect( hwndUrl , NULL, NULL) ;
			InvalidateRect( hwndMail, NULL, NULL) ;
			return 0 ;
		}
	}

	return CallWindowProc( wpcStatic, hWnd, uiMsg, wParam, lParam) ;
}


/******************************************************************************/
// AboutDlgProc
//============================================================================//
// 更新：02/06/26(水)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL CALLBACK AboutDlgProc( HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	static LOGFONT	logfont ;
	static HCURSOR	hCurArrow ;

	switch( uiMsg)
	{
		case WM_INITDIALOG:
		{
			// ウインドウハンドル
			hwndUrl = GetDlgItem( hDlg, IDC_URL) ;
			hwndMail = GetDlgItem( hDlg, IDC_MAIL) ;

			// フォントの設定（アンダーライン）
			hFontLink = (HFONT)SendMessage( hwndUrl, WM_GETFONT, 0, 0) ;
			GetObject( hFontLink, sizeof( logfont), &logfont);
			logfont.lfUnderline = 1;
			hFontLink = CreateFontIndirect( &logfont);
			SendMessage( hwndUrl , WM_SETFONT, (WPARAM)hFontLink, 0) ;
			SendMessage( hwndMail, WM_SETFONT, (WPARAM)hFontLink, 0) ;

			// サブクラス化
			wpcStatic = (WNDPROC)GetWindowLong( hwndUrl, GWL_WNDPROC) ;
			SetWindowLong( hwndUrl , GWL_WNDPROC, 	(LONG)LinkStaticProc) ;
			SetWindowLong( hwndMail, GWL_WNDPROC, 	(LONG)LinkStaticProc) ;

			// 手カーソル読みとり
			hCurArrow	= LoadCursor( NULL	, IDC_ARROW) ;

			break ;
		}

		case WM_COMMAND:
			switch( LOWORD( wParam))
			{
				case IDOK :
				case IDCANCEL :
					EndDialog( hDlg, TRUE) ;
					return 0 ;

				case IDC_MAIL :
				{
					if( HIWORD( wParam) != STN_CLICKED)
					{
						return false ;
					}

					string str = VERSION_MAIL ;
					str = string( "mailto:") + str ;
					ShellExecute( hDlg, NULL, str.c_str(), NULL, NULL, SW_SHOWNORMAL) ;
					break ;
				}

				case IDC_URL :
					if( HIWORD( wParam) != STN_CLICKED)
					{
						return false ;
					}
					ShellExecute( hDlg, NULL, VERSION_URL, NULL, NULL, SW_SHOWNORMAL) ;
					break ;
			}

		case WM_SETCURSOR :
		{
			blnUrlHover  = false ;
			blnMailHover = false ;

			InvalidateRect( hwndUrl , NULL, NULL) ;
			InvalidateRect( hwndMail, NULL, NULL) ;
		
			// 手カーソル読みとり
			SetCursor( hCurArrow) ;
			return TRUE ;
		}

		default :
			return FALSE ;
	}

	return TRUE ;
}

// main.cpp
//============================================================================//
// �X�V�F02/06/26(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "include.h"
#include "resource.h"


/******************************************************************************/
//		��`
/******************************************************************************/

#define VERSION_URL	"http://www.nitoyon.com/"
#define VERSION_MAIL	"soft@nitoyon.com"


/******************************************************************************/
//		�O���[�o���ϐ�
/******************************************************************************/

BOOL	blnUrlHover ;
BOOL	blnMailHover ;
HWND	hwndUrl ;
HWND	hwndMail ;
HFONT	hFontLink ;
WNDPROC	wpcStatic ;



/******************************************************************************/
// �X�^�e�B�b�N�v���V�[�W��
//============================================================================//
// �X�V�F02/06/26(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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

			// ��J�[�\���ǂ݂Ƃ�
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
// �X�V�F02/06/26(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL CALLBACK AboutDlgProc( HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	static LOGFONT	logfont ;
	static HCURSOR	hCurArrow ;

	switch( uiMsg)
	{
		case WM_INITDIALOG:
		{
			// �E�C���h�E�n���h��
			hwndUrl = GetDlgItem( hDlg, IDC_URL) ;
			hwndMail = GetDlgItem( hDlg, IDC_MAIL) ;

			// �t�H���g�̐ݒ�i�A���_�[���C���j
			hFontLink = (HFONT)SendMessage( hwndUrl, WM_GETFONT, 0, 0) ;
			GetObject( hFontLink, sizeof( logfont), &logfont);
			logfont.lfUnderline = 1;
			hFontLink = CreateFontIndirect( &logfont);
			SendMessage( hwndUrl , WM_SETFONT, (WPARAM)hFontLink, 0) ;
			SendMessage( hwndMail, WM_SETFONT, (WPARAM)hFontLink, 0) ;

			// �T�u�N���X��
			wpcStatic = (WNDPROC)GetWindowLong( hwndUrl, GWL_WNDPROC) ;
			SetWindowLong( hwndUrl , GWL_WNDPROC, 	(LONG)LinkStaticProc) ;
			SetWindowLong( hwndMail, GWL_WNDPROC, 	(LONG)LinkStaticProc) ;

			// ��J�[�\���ǂ݂Ƃ�
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
		
			// ��J�[�\���ǂ݂Ƃ�
			SetCursor( hCurArrow) ;
			return TRUE ;
		}

		default :
			return FALSE ;
	}

	return TRUE ;
}

// MsgMap.h
// メッセージマップ
//============================================================================//
// 更新：02/10/09(水)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __MSGMAP_H__
#define  __MSGMAP_H__


/******************************************************************************/
// メッセージマップ用基本マクロ
//============================================================================//
// 更新：1996/10/20 (Sun)
// 概要：メッセージマップを記述する為の基本マクロ。
// 補足：なし。
//============================================================================//

#ifndef  __macro_message_map_primitive__
#define  __macro_message_map_primitive__

#define  DECLARE_MESSAGE_MAP_PRIMITIVE( MapName, ResultType)			\
	friend  ResultType  CALLBACK  MapName( HWND, UINT, WPARAM, LPARAM) ;

#define  BEGIN_MESSAGE_MAP_PRIMITIVE( MapName, ClassName, ResultType, InitMessage, Converter, DefHandler)\
									\
	ResultType  CALLBACK  MapName( HWND    hWnd,			\
				       UINT    uiMessage,		\
				       WPARAM  wprParam,		\
				       LPARAM  lprParam){		\
									\
		ClassName	*pWndObj ;				\
									\
		if( uiMessage == InitMessage){				\
			pWndObj = (ClassName *)Converter ;		\
			SetWindowLong( hWnd, GWL_USERDATA, (LONG)pWndObj) ;\
			pWndObj->m_hWnd = hWnd ;			\
			}						\
									\
		pWndObj = (ClassName *)GetWindowLong( hWnd, GWL_USERDATA) ;\
									\
		if( !pWndObj){						\
			return DefHandler ;				\
			}						\
									\
									\
		switch( uiMessage){

#define  END_MESSAGE_MAP_PRIMITIVE( DefHandler)					\
			default:					\
				break ;					\
			}						\
									\
		if( uiMessage == WM_NCDESTROY){				\
			pWndObj->m_hWnd = NULL ;			\
			}						\
									\
		return DefHandler ;					\
		}
#endif



/******************************************************************************/
// メッセージハンドルマップ用基本マクロ
//============================================================================//
// 更新：1996/10/20 (Sun)
// 概要：メッセージハンドラエントリを記述するための基本マクロ。
// 補足：なし。
//============================================================================//

#ifndef  __macro_message_handler_primitive__
#define  __macro_message_handler_primitive__

#define  ON_MESSAGE( MessageName, HandlerName)					\
			case MessageName:				\
				return pWndObj->HandlerName( hWnd, wprParam, lprParam) ;

// デフォルトプロシージャに処理を渡す
#define  ON_MSG_BRK( MessageName, HandlerName)					\
			case MessageName:				\
				pWndObj->HandlerName( hWnd, wprParam, lprParam) ;\
				break ;

#define  BEGIN_HANDLE_MAP( Message, SubCode)				\
			case Message:				\
				switch( SubCode){

#define  END_HANDLE_MAP()							\
					default:			\
						break ;			\
					}				\
				break ;

#endif


/******************************************************************************/
// メッセージハンドルマップ用マクロ
//============================================================================//
// 更新：1996/10/20 (Sun)
// 概要：メッセージハンドラエントリを記述するためのマクロ。
// 補足：なし。
//============================================================================//

#ifndef  __macro_message_handler__
#define  __macro_message_handler__

#define  BEGIN_COMMAND_MAP()		BEGIN_HANDLE_MAP( WM_COMMAND, LOWORD( wprParam))
#define  ON_COMMAND( Command, Handler)	ON_MESSAGE( Command, Handler)
#define  END_COMMAND_MAP()		END_HANDLE_MAP()

#define  BEGIN_NOTIFY_MAP()		BEGIN_HANDLE_MAP( WM_NOTIFY, wprParam)
#define  ON_NOTIFY( Item, Handler)	ON_MESSAGE( Item, Handler)
#define  END_NOTIFY_MAP()		END_HANDLE_MAP()

#define  BEGIN_DRAWITEM_MAP()		BEGIN_HANDLE_MAP( WM_DRAWITEM, wprParam)
#define  ON_DRAWITEM( Item, Handler)	ON_MESSAGE( Item, Handler)
#define  END_DRAWITEM_MAP()		END_HANDLE_MAP()

#endif


/******************************************************************************/
// ウィンドウメッセージマップ用マクロ
//============================================================================//
// 更新：2001/07/23 (月)
// 概要：ウィンドウクラスのメッセージマップ記述用マクロ。
// 補足：なし。
//============================================================================//

#ifndef  __macro_wnd_message_map__
#define  __macro_wnd_message_map__

#define  DECLARE_MESSAGE_MAP( MapName)						\
		DECLARE_MESSAGE_MAP_PRIMITIVE( MapName, LRESULT)

#define  BEGIN_MESSAGE_MAP( MapName, ClassName)					\
		BEGIN_MESSAGE_MAP_PRIMITIVE( MapName, ClassName, LRESULT,	\
					     WM_NCCREATE,			\
					     ((CREATESTRUCT *)lprParam)->lpCreateParams,\
					     DefWindowProc( hWnd, uiMessage, wprParam, lprParam))

#define  END_MESSAGE_MAP()							\
		END_MESSAGE_MAP_PRIMITIVE( DefWindowProc( hWnd, uiMessage, wprParam, lprParam))

#endif


/******************************************************************************/
// サブクラス用メッセージマップ
//============================================================================//
// 更新：02/10/09(水)
// 概要：なし。
// 補足：なし。
//============================================================================//

#define  DECLARE_SUBCLASS_MESSAGE_MAP( MapName)						\
		DECLARE_MESSAGE_MAP_PRIMITIVE( MapName, LRESULT)

#define  BEGIN_SUBCLASS_MESSAGE_MAP( MapName, ClassName)					\
		BEGIN_MESSAGE_MAP_PRIMITIVE( MapName, ClassName, LRESULT,	\
					     WM_NCCREATE,			\
					     ((CREATESTRUCT *)lprParam)->lpCreateParams,\
					     DefWindowProc( hWnd, uiMessage, wprParam, lprParam))
#define  END_SUBCLASS_MESSAGE_MAP( MapName)							\
		END_MESSAGE_MAP_PRIMITIVE( CallWindowProc( MapName, hWnd, uiMessage, wprParam, lprParam))


/******************************************************************************/
// MDI ウィンドウメッセージマップ用マクロ
//============================================================================//
// 更新：2001/07/23 (月)
// 概要：ウィンドウクラスのメッセージマップ記述用マクロ。
// 補足：なし。
//============================================================================//

#ifndef  __macro_frame_message_map__
#define  __macro_frame_message_map__

#define  BEGIN_FRAME_MESSAGE_MAP( MapName, ClassName)					\
		BEGIN_MESSAGE_MAP_PRIMITIVE( MapName, ClassName, LRESULT,	\
					     WM_NCCREATE,			\
					     ((CREATESTRUCT *)lprParam)->lpCreateParams,\
					     DefFrameProc( hWnd, NULL, uiMessage, wprParam, lprParam))

#define  END_FRAME_MESSAGE_MAP()							\
		END_MESSAGE_MAP_PRIMITIVE( DefFrameProc( hWnd, pWndObj->m_hClient, uiMessage, wprParam, lprParam))

#define  BEGIN_CHILD_MESSAGE_MAP( MapName, ClassName)					\
		BEGIN_MESSAGE_MAP_PRIMITIVE( MapName, ClassName, LRESULT,	\
					     WM_NCCREATE,			\
					     ((MDICREATESTRUCT*)((CREATESTRUCT *)lprParam)->lpCreateParams)->lParam,\
					     DefMDIChildProc( hWnd, uiMessage, wprParam, lprParam))

#define  END_CHILD_MESSAGE_MAP()							\
		END_MESSAGE_MAP_PRIMITIVE( DefMDIChildProc( hWnd, uiMessage, wprParam, lprParam))

#endif



/******************************************************************************/
// ダイアログメッセージマップ用マクロ
//============================================================================//
// 更新：1996/10/03 (Thu)
// 概要：ダイアログクラスのメッセージマップ記述用マクロ。
// 補足：なし。
//============================================================================//

#ifndef  __macro_dlg_message_map__
#define  __macro_dlg_message_map__

#define  DECLARE_DLG_MESSAGE_MAP( MapName)					\
		DECLARE_MESSAGE_MAP_PRIMITIVE( MapName, BOOL)

#define  BEGIN_DLG_MESSAGE_MAP( MapName, ClassName)				\
		BEGIN_MESSAGE_MAP_PRIMITIVE( MapName, ClassName, BOOL,		\
					     WM_INITDIALOG, lprParam,		\
					     FALSE)

#define  END_DLG_MESSAGE_MAP()	END_MESSAGE_MAP_PRIMITIVE( FALSE)

#endif


#endif

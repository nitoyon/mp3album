
// debug.h
// デバッグ関数
//============================================================================//
// 更新：02/11/03(日)
// 概要：URLを表現。
// 補足：なし。
//============================================================================//

#ifndef  __DEBUG_h__
#define  __DEBUG_h__


/******************************************************************************/
//				インクルード
/******************************************************************************/

#include "include.h"


/******************************************************************************/
//				定義
/******************************************************************************/

void MyOutputDebugString( LPCSTR pszFormat, ...) ;


/******************************************************************************/
// TRACEマクロ
//============================================================================//
// 更新：1996/10/20 (Sun)
// 概要：デバッグのメッセージ出力
// 補足：なし。
//============================================================================//

#if defined(_DEBUG) || defined(DEBUG)
// Debugのとき
#define TRACE(x)   OutputDebugString(x)
#define TRACE0(x)   OutputDebugString(x)
#define TRACE1(x, a)            MyOutputDebugString(x, a)
#define TRACE2(x, a, b)         MyOutputDebugString(x, a, b)
#define TRACE3(x, a, b, c)      MyOutputDebugString(x, a, b, c)
#define TRACE4(x, a, b, c, d)   MyOutputDebugString(x, a, b, c, d)
#else
// Releaseのとき
#define TRACE(x)
#define TRACE0(x)
#define TRACE1(x, a)
#define TRACE2(x, a, b)
#define TRACE3(x, a, b, c)
#define TRACE4(x, a, b, c, d)
#endif


/******************************************************************************/
// ASSERTマクロ
//============================================================================//
// 更新：02/04/09(火)
// 概要：変数チェック
// 補足：なし。
//============================================================================//

#if defined(_DEBUG) || defined(DEBUG)
// Debugのとき
#define ASSERT(x) \
    if (!(x)) { \
        MyOutputDebugString("Assertion failed! in %s (%d)\n", \
            __FILE__, __LINE__); \
        DebugBreak(); \
    }
#define VERIFY(x)   ASSERT(x)
#else
// Releaseのとき
#define ASSERT(x)
#define VERIFY(x)   x
#endif


#endif


// Mp3File.cpp
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "Mp3File.h"


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// コンストラクタ
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

Mp3File::Mp3File( File* f)
: ulSize( 0), lModifiedTime( 0), ulCrc( 0), uiErr( 0)
{
	File::SetFilePath( ConvertFilePath( f->GetFilePath())) ;
	intFileNameSize = strlen( f->GetFileName().c_str()) ;

	struct _stat stat ;
	if( _stat( GetFilePath().c_str(), &stat) == 0)
	{
		ulSize = stat.st_size ;
		lModifiedTime = UnixTime2DosTime(&stat.st_mtime) ;
	}
}


/******************************************************************************/
// デストラクタ
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

Mp3File::~Mp3File() 
{
}


/******************************************************************************/
// zip に格納できるファイルパスに変更
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

string Mp3File::ConvertFilePath( const string& strPath)
{
	char pszTmp[ MAX_PATH] ;
	strcpy( pszTmp, strPath.c_str()) ;

	string str ;
	char* pszPtr = pszTmp ;
	while( *pszPtr != '\0')
	{
		// ２バイト文字でない
		if( IsDBCSLeadByte( *pszPtr) == 0)
		{
			if( *pszPtr == '-')
			{
				str += "[-]" ;
			}
			else if( *pszPtr == '[')
			{
				str += "[[]" ;
			}
			else
			{
				str += *pszPtr ;
			}
		}
		// ２バイト文字
		else
		{
			str += pszPtr[ 0] ;
			str += pszPtr[ 1] ;
		}

		// 一文字分進める
		pszPtr = CharNext( pszPtr) ;
	}

	return string( pszTmp) ;
}


/******************************************************************************/
// time_t -> ULONG への時間変換
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

ULONG Mp3File::UnixTime2DosTime( time_t* t)
{
	time_t	t_even ;
	tm*	s ;         /* result of localtime() */

	t_even = (time_t)(((unsigned long)(*t) + 1) & (~1));
	                              /* Round up to even seconds. */
	s = localtime(&t_even);       /* Use local time since MSDOS does. */
	if (s == (struct tm *)NULL) {
	    /* time conversion error; use current time as emergency value
	       (assuming that localtime() does at least accept this value!) */
	    t_even = (time_t)(((unsigned long)time(NULL) + 1) & (~1));
	    s = localtime(&t_even);
	}

	return GetDosTime(s->tm_year + 1900, s->tm_mon + 1, s->tm_mday,
	               s->tm_hour, s->tm_min, s->tm_sec);
}


/******************************************************************************/
// 年月日 -> dostime への変換
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

ULONG Mp3File::GetDosTime( int y, int n, int d, int h, int m, int s)
{
#ifndef DOSTIME_MINIMUM
#define DOSTIME_MINIMUM         ((ULONG)0x00210000L)
#endif

	return y < 1980 ? DOSTIME_MINIMUM /* dostime(1980, 1, 1, 0, 0, 0) */ :
		(((ULONG)y - 1980) << 25) | ((ULONG)n << 21) | ((ULONG)d << 16) |
		((ULONG)h << 11) | ((ULONG)m << 5) | ((ULONG)s >> 1);
}

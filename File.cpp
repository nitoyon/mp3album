
// File.cpp
//============================================================================//
// 更新：02/12/08(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "File.h"


/******************************************************************************/
//		コンストラクタおよびデストラクタ
/******************************************************************************/
// 
//============================================================================//
// 更新：02/12/08(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

File::File( const string s) 
: strPath( s)
{
}


/******************************************************************************/
// 
//============================================================================//
// 更新：02/12/08(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

File::~File() 
{
}



/******************************************************************************/
//		ファイルパス関連
/******************************************************************************/
// 
//============================================================================//
// 更新：02/12/08(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

string File::GetFileName() const 
{
	int	intLastYen = 0 ;
	char	pszFile[ MAX_PATH] ;
	char*	pszPointer = pszFile ;
	strcpy( pszFile, strPath.c_str()) ;

	for( int i = 0; i < strPath.size(); i++)
	{
		pszPointer = pszFile + i ;

		if( IsDBCSLeadByte( *pszPointer))
		{
			// ２バイト文字なら２進む
			i++ ;
			continue ;
		}

		if( *pszPointer == '\\')
		{
			intLastYen = i ;
		}
	}

	// \ があって、最終文字ではない場合
	if( intLastYen == 0)
	{
		return strPath ;
	}
	else if( intLastYen == strPath.size() - 1)
	{
		return "" ;
	}
	else
	{
		return strPath.substr( intLastYen + 1) ;
	}
}


/******************************************************************************/
// 
//============================================================================//
// 更新：02/12/08(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

string File::GetFilePath() const 
{
	return strPath ;
}


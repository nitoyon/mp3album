
// util.cpp
//============================================================================//
// 更新：03/02/02(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "util.h"


/******************************************************************************/
// フルパスからディレクトリ取得
//============================================================================//
// 更新：03/02/02(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

string GetDirName( const string& strPath)
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

		if( *pszPointer == '\\' || *pszPointer == '/')
		{
			intLastYen = i ;
		}
	}

	if( intLastYen > 0)
	{
		return strPath.substr( 0, intLastYen + 1) ;	// Yen も含めて返す
	}
	else
	{
		return "" ;
	}
}


/******************************************************************************/
// フルパスからファイル名取得
//============================================================================//
// 更新：03/02/02(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

string GetFileName( const string& strPath)
{
	string strDirName = GetDirName( strPath) ;
	return strPath.substr( strDirName.size()) ;
}

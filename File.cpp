
// File.cpp
//============================================================================//
// 更新：03/02/09(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "File.h"
#include "util.h"


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
// ファイル名
//============================================================================//
// 更新：02/12/08(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

string File::GetFileName() const 
{
	if( strPath == "")
	{
		return "" ;
	}

	return ::GetFileName( strPath) ;
}


/******************************************************************************/
// ベースパス
//============================================================================//
// 更新：03/02/09(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

string File::GetBasePath() const
{
	if( strPath == "")
	{
		return "" ;
	}

	return strPath.substr( 0, strPath.size() - ::GetFileName( strPath).size()) ;
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


// File.h
//============================================================================//
// 更新：02/12/15(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef __File_h__
#define __File_h__

#include "include.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class File
{
protected:
	string	strPath ;

public:
// コンストラクタおよびデストラクタ
	File( const string s = "") ;
	~File() ;

// ファイルパス関連
	string GetFileName() const ;
	string GetBasePath() const ;
	string GetFilePath() const ;
	virtual void SetFilePath( const string& s)
	{
		strPath = s ;
	}
} ;

#endif

// Profile.h
// アプリケーションの設定
//============================================================================//
// 更新：02/12/15(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __PROFILE_H__
#define  __PROFILE_H__

#include "include.h"


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class Profile
{
private:
	static string strPath ;

public:
	enum ZipFolder
	{
		SET, ONE, ONZIP
	} ;

	enum ZipFile
	{
		FILESET, PARENT
	} ;

// リスト
	static BOOL blnFullPath ;
	static BOOL blnOnlyMp3 ;

// 場所
	static int intX ;
	static int intY ;

// 設定
	static ZipFolder	intFolder ;
	static ZipFile		intFile ;
	static string		strFolder ;
	static string		strFile ;
	static string		strExtension ;
	static BOOL		blnDisplayLog ;
	static BOOL		blnFailLogOnly ;
	static BOOL		blnConfirmOverwrite ;

// 読み書き
	static void Save() ;
	static void Load() ;

private:
	static void WriteProfile( const string& s, LPTSTR pszSection, LPTSTR pszName, UINT ui) ;
} ;


#endif
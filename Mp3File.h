
// Mp3File.h
//============================================================================//
// 更新：03/02/09(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef __Mp3File_h__
#define __Mp3File_h__

#include "include.h"
#include "File.h"


/******************************************************************************/
//		定義
/******************************************************************************/

#define  ERR_ATTR_UNREADABLE	1
#define  ERR_FILE_UNREADABLE	2


/******************************************************************************/
//		クラス定義
/******************************************************************************/

class Mp3File : public File
{
private:
	string	strSaveName ;
	string	strSaveNameInZip ;

public:
	ULONG	ulSize ;
	LONG	lModifiedTime ;
	int	intSaveNameSize ;
	ULONG	ulCrc ;
	LONG	lLocalHeader ;
	UINT	uiErr ;

public:
// コンストラクタおよびデストラクタ
	Mp3File( const string&) ;
	virtual ~Mp3File() ;

// ファイル設定
	void SetSaveName( const string&) ;
	string GetSaveName() const{ return strSaveName ;}
	string GetSaveNameInZip() const{ return strSaveNameInZip ;}
	void GetFileData() ;

private:
	string ConvertFilePath( const string& strPath) ;
	ULONG  UnixTime2DosTime( time_t* t) ;
	ULONG  GetDosTime( int y, int n, int d, int h, int m, int s) ;
} ;

#endif
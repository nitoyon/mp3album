
// Mp3File.h
//============================================================================//
// 更新：02/12/09(月)
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
public:
	ULONG	ulSize ;
	LONG	lModifiedTime ;
	int	intFileNameSize ;
	ULONG	ulCrc ;
	LONG	lLocalHeader ;
	UINT	uiErr ;

public:
// コンストラクタおよびデストラクタ
	Mp3File( File*) ;
	~Mp3File() ;

// ファイル設定
	void SetFilePath( const string&){}

private:
	string ConvertFilePath( const string& strPath) ;
	ULONG  UnixTime2DosTime( time_t* t) ;
	ULONG  GetDosTime( int y, int n, int d, int h, int m, int s) ;
} ;

#endif
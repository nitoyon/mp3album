
// Zip.cpp
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "Zip.h"


/******************************************************************************/
//		圧縮
/******************************************************************************/
// 圧縮実行
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

DWORD WINAPI fire( const string& strPath, HWND hwnd)
{
	int intCount = ListView_GetItemCount( hwnd) ;
	if( intCount == 0)
	{
		return 0 ;
	}

	// 順次、解凍
	FILE* fzip ;
	fzip = fopen( strPath.c_str(), "wb") ;
	ULONG lPrevTail = 0 ;
	ULONG  ulTotalSize = 0 ;
	vector<Mp3File*> vecMp3FileList ;
	int i ;

	for( i = 0; i < intCount; i++)
	{
		LVITEM lvitem = { LVIF_PARAM} ;
		lvitem.iItem = i ;
		ListView_GetItem( hwnd, &lvitem) ;

		Mp3File* pMp3File = new Mp3File( (File*)lvitem.lParam) ;
		if( pMp3File->ulSize == 0 && pMp3File->lModifiedTime == 0)
		{
			pMp3File->uiErr = ERR_ATTR_UNREADABLE ;
			ZipLog( hwnd, i, "[×ファイル情報]") ;
		}
		pMp3File->lLocalHeader = lPrevTail ;
		vecMp3FileList.push_back( pMp3File) ;

		// Local file ヘッダ出力
		OutputLocalFileHeader( pMp3File, fzip) ;
		long lTail = ftell( fzip) ;

		// MP3 ファイル出力
		BYTE bBuf[ SBSZ] ;
		FILE* fMp3 = fopen( pMp3File->GetFilePath().c_str(), "rb") ;
		ULONG ulRead ;
		ULONG ulTotal = 0 ;
		unsigned long ulCrc = 0 ;
		while( TRUE)
		{
			// 読みとり
			ulRead = fread( bBuf, sizeof( BYTE), SBSZ, fMp3) ;
			if( ferror( fMp3))
			{
				pMp3File->uiErr = ERR_FILE_UNREADABLE ;
				ZipLog( hwnd, i, "[×ファイル読取] ") ;
				break ;
			}

			// ログ
			ulTotal += ulRead ;
			char pszBuf[ 100] ;
			double f = (double)ulTotal / (double)pMp3File->ulSize ;
			int intPercent = f * 100.0 ;
			sprintf( pszBuf, "[%3d％ 完了]", intPercent) ;
			ZipLog( hwnd, i, pszBuf) ;

			// 書き込み
			ulCrc = crc32( ulCrc, (uch*)bBuf, ulRead);
			fwrite( bBuf, sizeof( BYTE), ulRead, fzip) ;
			if( feof( fMp3))
			{
				break ;
			}
		}

		// CRC情報を上書き
		lTail = ftell( fzip) ;
		ulTotalSize += lTail ;
		pMp3File->ulCrc = ulCrc ;
		if( fseek( fzip, lPrevTail + 14, SEEK_SET) == 0)
		{
			PUTLG( ulCrc, fzip) ;
			fseek( fzip, lTail, SEEK_SET) ;
		}
		ZipLog( hwnd, i, "[  成    功  ]") ;

		lPrevTail = lTail ;
		fclose( fMp3) ;
	}

	// Central directory の出力
	int	intFileNum = 0 ;
	ULONG	ulOffset = ftell( fzip) ;
	ULONG	ulDirSize = ftell( fzip) ;

	for( i = 0; i < intCount; i++)
	{
		OutputCentralDirectory( vecMp3FileList[ i], fzip) ;
		intFileNum++ ;

		delete vecMp3FileList[ i] ;
	}

	// End of central directory record の出力
	ulDirSize = ftell( fzip) - ulDirSize ;
	OutputEndCentralDirectory( intFileNum, ulDirSize, ulOffset, fzip) ;

	fclose( fzip) ;

	return 1 ;
}


/******************************************************************************/
// ヘッダ出力
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL OutputLocalFileHeader( Mp3File* pMp3File, FILE* fzip)
{
	char pszFileName[ _MAX_FNAME * 3] ;	// ファイル名エンコードで最大３倍になる
	strcpy( pszFileName, pMp3File->GetFileName().c_str()) ;

	// ヘッダの出力 (zipfile.c (putlocal) : 1040行目) 
	// http://www.goice.co.jp/member/mo/formats/zip.html
	PUTLG( 0x04034b50L, fzip) ;			// 
	PUTSH( 10, fzip) ;				// ver  = 10
	PUTSH( 0, fzip) ;				// lflg	= 0
	PUTSH( 0, fzip);				// how	= 0 (STORE)
	PUTLG( pMp3File->lModifiedTime, fzip) ;		// tim			[時間]
	PUTLG( 0, fzip) ;				// crc	= 0 [CRC情報。あとで上書き]
	PUTLG( pMp3File->ulSize, fzip);			// siz
	PUTLG( pMp3File->ulSize, fzip);			// len
	PUTSH( pMp3File->intFileNameSize, fzip) ;	// nam			[length of iname]
	PUTSH( 0, fzip) ;				// ext
	fputs( pszFileName, fzip) ;

	return TRUE ;
}


/******************************************************************************/
// Central Directory 出力
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL OutputCentralDirectory( Mp3File* pMp3File, FILE* fzip)
{
	char pszFileName[ _MAX_FNAME * 3] ;	// ファイル名エンコードで最大３倍になる
	strcpy( pszFileName, pMp3File->GetFileName().c_str()) ;

	PUTLG( 0x02014b50L, fzip) ;			// 
	PUTSH( 0x0B17, fzip) ;				// Version made by = 0x0B17
	PUTSH( 10, fzip) ;				// Version needed to extract = 10
	PUTSH( 0, fzip) ;				// lflg	= 0
	PUTSH( 0, fzip);				// how	= 0 (STORE)
	PUTLG( pMp3File->lModifiedTime, fzip) ;		// tim			[時間]
	PUTLG( pMp3File->ulCrc, fzip) ;			// crc	= 0 [CRC情報。あとで上書き]
	PUTLG( pMp3File->ulSize, fzip);			// siz
	PUTLG( pMp3File->ulSize, fzip);			// len
	PUTSH( pMp3File->intFileNameSize, fzip) ;	// nam			[length of iname]
	PUTSH( 0, fzip) ;				// Extra Fild Length
	PUTSH( 0, fzip) ;				// File Comment Length
	PUTSH( 0, fzip) ;				// Disk Number		[???　zip32.dll では必ず 0]
	PUTSH( 0, fzip) ;				// Internal file attributes [0 = BINARY]
	PUTLG( 0, fzip) ;				// External file attributes
	PUTLG( pMp3File->lLocalHeader, fzip) ;		// Relative offset of local header
	fputs( pszFileName, fzip) ;

	return TRUE ;
}


/******************************************************************************/
// End of central directory record の出力
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL OutputEndCentralDirectory( int intFileNum, ULONG ulDirSize, ULONG ulOffset, FILE* fzip)
{
	PUTLG( 0x06054b50L, fzip) ;	// 
	PUTSH( 0, fzip) ;		// number of this disk
	PUTSH( 0, fzip) ;		// Number of the disk with the start of the central directory
	PUTSH( intFileNum, fzip) ;	// Total number of entries in the central dir on this disk
	PUTSH( intFileNum, fzip) ;	// Total number of entries in the central dir
	PUTLG( ulDirSize, fzip) ;	// Size of the central directory
	PUTLG( ulOffset, fzip) ;	// Offset of start of central directory with respect to the starting disk number
	PUTSH( 0, fzip);		// zipfile comment length

	return TRUE ;
}


/******************************************************************************/
// ログ出力
//============================================================================//
// 更新：02/12/09(月)
// 概要：なし。
// 補足：なし。
//============================================================================//

void ZipLog( HWND hwnd, int i, const string& s)
{
	LVITEM lvitem = { LVIF_PARAM} ;
	lvitem.iItem = i ;
	ListView_GetItem( hwnd, &lvitem) ;
	File* pFile = (File*)lvitem.lParam ;

	string str = s + " " + pFile->GetFilePath() ;
	lvitem.mask	= LVIF_TEXT ;
	lvitem.iItem	= i ;
	lvitem.pszText	= (LPTSTR)str.c_str() ;
	ListView_SetItem( hwnd, &lvitem) ;

	MSG msg ;
	while( PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

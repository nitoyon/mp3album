

// Zip.cpp
//============================================================================//
// 更新：02/12/15(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#include "Zip.h"
#include "ProgressDlg.h"


/******************************************************************************/
//		圧縮
/******************************************************************************/
// 圧縮実行
//============================================================================//
// 更新：02/12/15(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

void _cdecl fire( void* p)
{
	ProgressDlg* pProgressDlg = (ProgressDlg*)p ;
	vector<File*>* pvecFileList = pProgressDlg->GetFileList() ;
	string strPath = pProgressDlg->GetArchivePath() ;

	// 情報取得
	vector<Mp3File*> vecMp3FileList ;
	int i ;
	if( !GetFileAttr( &vecMp3FileList, pProgressDlg))
	{
		// エラー処理
		string s ;
		for( i = 0; i < vecMp3FileList.size(); i++)
		{
			if( vecMp3FileList[ i]->uiErr == ERR_ATTR_UNREADABLE)
			{
				s += vecMp3FileList[ i]->GetFilePath() + "\n" ;
			}
		}

		pProgressDlg->SetErrLog( string( "次のファイルの情報を取得できませんでした。\n---\n") + s) ;
		pProgressDlg->SetState( ProgressDlg::State::FAIL) ;
		pProgressDlg->OnZipFinish() ;
		return ;
	}
	int intCount = vecMp3FileList.size() ;

	// プログレスセット
	ULONG  ulArchiveTotal = 0 ;
	for( i = 0; i < intCount; i++)
	{
		ulArchiveTotal += vecMp3FileList[ i]->ulSize ;
	}
	pProgressDlg->SetProgressRange( 0, ulArchiveTotal) ;

	// 順次、解凍
	FILE* fzip ;
	fzip = fopen( strPath.c_str(), "wb") ;
	if( !fzip)
	{
		pProgressDlg->SetState( ProgressDlg::State::FAIL) ;
		pProgressDlg->SetErrLog( 
			  string( "次のファイルを書き込みモードで開けませんでした。\n---\n")
			+ strPath
			) ;
		pProgressDlg->OnZipFinish() ;
		return ;
	}

	ULONG lPrevTail = 0 ;
	ulArchiveTotal = 0 ;

	for( i = 0; i < intCount; i++)
	{
		Mp3File* pMp3File = vecMp3FileList[ i] ;
		pMp3File->lLocalHeader = lPrevTail ;
		pProgressDlg->SetFileName( pMp3File->GetFilePath()) ;
		pProgressDlg->SetProgressRange( 1, pMp3File->ulSize) ;

		// Local file ヘッダ出力
		OutputLocalFileHeader( pMp3File, fzip) ;
		long lTail = ftell( fzip) ;

		// MP3 ファイル出力
		BYTE bBuf[ SBSZ] ;
		FILE* fMp3 = fopen( pMp3File->GetFilePath().c_str(), "rb") ;
		if( !fMp3)
		{
			pProgressDlg->SetState( ProgressDlg::State::FAIL) ;
			pProgressDlg->SetErrLog( 
				  string( "次のファイルを読みとりモードで開けませんでした\n---\n") + pMp3File->GetFilePath()
				  ) ;
			pProgressDlg->OnZipFinish() ;
			return ;
		}

		ULONG ulRead ;
		ULONG ulFileTotal = 0 ;
		ULONG ulCrc = 0 ;
		while( TRUE)
		{
			// 読みとり
			ulRead = fread( bBuf, sizeof( BYTE), SBSZ, fMp3) ;
			if( ferror( fMp3))
			{
				pProgressDlg->SetState( ProgressDlg::State::FAIL) ;
				pProgressDlg->SetErrLog( string( "[NG:読みとり中にエラーが発生しました] " + pMp3File->GetFilePath())) ;
				pProgressDlg->OnZipFinish() ;
				return ;
			}

			// ログ
			ulFileTotal += ulRead ;
			ulArchiveTotal += ulRead ;
			pProgressDlg->SetProgressPos( 0, ulArchiveTotal) ;
			pProgressDlg->SetProgressPos( 1, ulFileTotal) ;

			// 書き込み
			ulCrc = crc32( ulCrc, (unsigned char*)bBuf, ulRead);
			fwrite( bBuf, sizeof( BYTE), ulRead, fzip) ;
			if( feof( fMp3))
			{
				break ;
			}

			if( pProgressDlg->GetState() == ProgressDlg::State::CANCELED)
			{
				fclose( fzip) ;
				fclose( fMp3) ;
				pProgressDlg->SetErrLog( string( "ユーザーによってキャンセルされました。")) ;
				pProgressDlg->OnZipFinish() ;
				return ;
			}
		}

		// CRC情報を上書き
		lTail = ftell( fzip) ;
		pMp3File->ulCrc = ulCrc ;
		if( fseek( fzip, lPrevTail + 14, SEEK_SET) == 0)
		{
			PUTLG( ulCrc, fzip) ;
			fseek( fzip, lTail, SEEK_SET) ;
		}

		lPrevTail = lTail ;
		pProgressDlg->SetLog( pProgressDlg->GetLog() + string( "[OK] " + pMp3File->GetFilePath() + "\n")) ;
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

		if( pProgressDlg->GetState() == ProgressDlg::State::CANCELED)
		{
			fclose( fzip) ;
			pProgressDlg->OnZipFinish() ;
			return ;
		}
	}

	// End of central directory record の出力
	ulDirSize = ftell( fzip) - ulDirSize ;
	OutputEndCentralDirectory( intFileNum, ulDirSize, ulOffset, fzip) ;

	fclose( fzip) ;
	pProgressDlg->SetState( ProgressDlg::State::SUCCESS) ;
	pProgressDlg->OnZipFinish() ;

	return ;
}


/******************************************************************************/
// ファイル情報取得
//============================================================================//
// 更新：02/12/15(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

BOOL GetFileAttr( vector<Mp3File*>* pvecMp3FileList, ProgressDlg* pProgressDlg)
{
	vector<File*>* pvecFileList = pProgressDlg->GetFileList() ;
	int intCount = pvecFileList->size() ;

	// １つずつ取得
	BOOL blnSuccess = TRUE ;
	for( int i = 0; i < intCount; i++)
	{
		Mp3File* pMp3File = new Mp3File( (File*)pvecFileList->at( i)) ;
		if( pMp3File->ulSize == 0 && pMp3File->lModifiedTime == 0)
		{
			pMp3File->uiErr = ERR_ATTR_UNREADABLE ;
			blnSuccess = FALSE ;
		}

		pvecMp3FileList->push_back( pMp3File) ;
	}

	return blnSuccess ;
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



// Zip.cpp
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "Zip.h"
#include "ProgressDlg.h"


/******************************************************************************/
//		���k
/******************************************************************************/
// ���k���s
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void _cdecl fire( void* p)
{
	ProgressDlg* pProgressDlg = (ProgressDlg*)p ;
	vector<File*>* pvecFileList = pProgressDlg->GetFileList() ;
	string strPath = pProgressDlg->GetArchivePath() ;

	// ���擾
	vector<Mp3File*> vecMp3FileList ;
	int i ;
	if( !GetFileAttr( &vecMp3FileList, pProgressDlg))
	{
		// �G���[����
		string s ;
		for( i = 0; i < vecMp3FileList.size(); i++)
		{
			if( vecMp3FileList[ i]->uiErr == ERR_ATTR_UNREADABLE)
			{
				s += vecMp3FileList[ i]->GetFilePath() + "\n" ;
			}
		}

		pProgressDlg->SetErrLog( string( "���̃t�@�C���̏����擾�ł��܂���ł����B\n---\n") + s) ;
		pProgressDlg->SetState( ProgressDlg::State::FAIL) ;
		pProgressDlg->OnZipFinish() ;
		return ;
	}
	int intCount = vecMp3FileList.size() ;

	// �v���O���X�Z�b�g
	ULONG  ulArchiveTotal = 0 ;
	for( i = 0; i < intCount; i++)
	{
		ulArchiveTotal += vecMp3FileList[ i]->ulSize ;
	}
	pProgressDlg->SetProgressRange( 0, ulArchiveTotal) ;

	// �����A��
	FILE* fzip ;
	fzip = fopen( strPath.c_str(), "wb") ;
	if( !fzip)
	{
		pProgressDlg->SetState( ProgressDlg::State::FAIL) ;
		pProgressDlg->SetErrLog( 
			  string( "���̃t�@�C�����������݃��[�h�ŊJ���܂���ł����B\n---\n")
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

		// Local file �w�b�_�o��
		OutputLocalFileHeader( pMp3File, fzip) ;
		long lTail = ftell( fzip) ;

		// MP3 �t�@�C���o��
		BYTE bBuf[ SBSZ] ;
		FILE* fMp3 = fopen( pMp3File->GetFilePath().c_str(), "rb") ;
		if( !fMp3)
		{
			pProgressDlg->SetState( ProgressDlg::State::FAIL) ;
			pProgressDlg->SetErrLog( 
				  string( "���̃t�@�C����ǂ݂Ƃ胂�[�h�ŊJ���܂���ł���\n---\n") + pMp3File->GetFilePath()
				  ) ;
			pProgressDlg->OnZipFinish() ;
			return ;
		}

		ULONG ulRead ;
		ULONG ulFileTotal = 0 ;
		ULONG ulCrc = 0 ;
		while( TRUE)
		{
			// �ǂ݂Ƃ�
			ulRead = fread( bBuf, sizeof( BYTE), SBSZ, fMp3) ;
			if( ferror( fMp3))
			{
				pProgressDlg->SetState( ProgressDlg::State::FAIL) ;
				pProgressDlg->SetErrLog( string( "[NG:�ǂ݂Ƃ蒆�ɃG���[���������܂���] " + pMp3File->GetFilePath())) ;
				pProgressDlg->OnZipFinish() ;
				return ;
			}

			// ���O
			ulFileTotal += ulRead ;
			ulArchiveTotal += ulRead ;
			pProgressDlg->SetProgressPos( 0, ulArchiveTotal) ;
			pProgressDlg->SetProgressPos( 1, ulFileTotal) ;

			// ��������
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
				pProgressDlg->SetErrLog( string( "���[�U�[�ɂ���ăL�����Z������܂����B")) ;
				pProgressDlg->OnZipFinish() ;
				return ;
			}
		}

		// CRC�����㏑��
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

	// Central directory �̏o��
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

	// End of central directory record �̏o��
	ulDirSize = ftell( fzip) - ulDirSize ;
	OutputEndCentralDirectory( intFileNum, ulDirSize, ulOffset, fzip) ;

	fclose( fzip) ;
	pProgressDlg->SetState( ProgressDlg::State::SUCCESS) ;
	pProgressDlg->OnZipFinish() ;

	return ;
}


/******************************************************************************/
// �t�@�C�����擾
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL GetFileAttr( vector<Mp3File*>* pvecMp3FileList, ProgressDlg* pProgressDlg)
{
	vector<File*>* pvecFileList = pProgressDlg->GetFileList() ;
	int intCount = pvecFileList->size() ;

	// �P���擾
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
// �w�b�_�o��
//============================================================================//
// �X�V�F02/12/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL OutputLocalFileHeader( Mp3File* pMp3File, FILE* fzip)
{
	char pszFileName[ _MAX_FNAME * 3] ;	// �t�@�C�����G���R�[�h�ōő�R�{�ɂȂ�
	strcpy( pszFileName, pMp3File->GetFileName().c_str()) ;

	// �w�b�_�̏o�� (zipfile.c (putlocal) : 1040�s��) 
	// http://www.goice.co.jp/member/mo/formats/zip.html
	PUTLG( 0x04034b50L, fzip) ;			// 
	PUTSH( 10, fzip) ;				// ver  = 10
	PUTSH( 0, fzip) ;				// lflg	= 0
	PUTSH( 0, fzip);				// how	= 0 (STORE)
	PUTLG( pMp3File->lModifiedTime, fzip) ;		// tim			[����]
	PUTLG( 0, fzip) ;				// crc	= 0 [CRC���B���Ƃŏ㏑��]
	PUTLG( pMp3File->ulSize, fzip);			// siz
	PUTLG( pMp3File->ulSize, fzip);			// len
	PUTSH( pMp3File->intFileNameSize, fzip) ;	// nam			[length of iname]
	PUTSH( 0, fzip) ;				// ext
	fputs( pszFileName, fzip) ;

	return TRUE ;
}


/******************************************************************************/
// Central Directory �o��
//============================================================================//
// �X�V�F02/12/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

BOOL OutputCentralDirectory( Mp3File* pMp3File, FILE* fzip)
{
	char pszFileName[ _MAX_FNAME * 3] ;	// �t�@�C�����G���R�[�h�ōő�R�{�ɂȂ�
	strcpy( pszFileName, pMp3File->GetFileName().c_str()) ;

	PUTLG( 0x02014b50L, fzip) ;			// 
	PUTSH( 0x0B17, fzip) ;				// Version made by = 0x0B17
	PUTSH( 10, fzip) ;				// Version needed to extract = 10
	PUTSH( 0, fzip) ;				// lflg	= 0
	PUTSH( 0, fzip);				// how	= 0 (STORE)
	PUTLG( pMp3File->lModifiedTime, fzip) ;		// tim			[����]
	PUTLG( pMp3File->ulCrc, fzip) ;			// crc	= 0 [CRC���B���Ƃŏ㏑��]
	PUTLG( pMp3File->ulSize, fzip);			// siz
	PUTLG( pMp3File->ulSize, fzip);			// len
	PUTSH( pMp3File->intFileNameSize, fzip) ;	// nam			[length of iname]
	PUTSH( 0, fzip) ;				// Extra Fild Length
	PUTSH( 0, fzip) ;				// File Comment Length
	PUTSH( 0, fzip) ;				// Disk Number		[???�@zip32.dll �ł͕K�� 0]
	PUTSH( 0, fzip) ;				// Internal file attributes [0 = BINARY]
	PUTLG( 0, fzip) ;				// External file attributes
	PUTLG( pMp3File->lLocalHeader, fzip) ;		// Relative offset of local header
	fputs( pszFileName, fzip) ;

	return TRUE ;
}


/******************************************************************************/
// End of central directory record �̏o��
//============================================================================//
// �X�V�F02/12/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
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

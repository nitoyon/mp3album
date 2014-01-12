
// Mp3File.cpp
//============================================================================//
// �X�V�F03/02/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "Mp3File.h"


/******************************************************************************/
//		�R���X�g���N�^����уf�X�g���N�^
/******************************************************************************/
// �R���X�g���N�^
//============================================================================//
// �X�V�F03/02/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

Mp3File::Mp3File( const string& s)
: File( s), ulSize( 0), lModifiedTime( 0), ulCrc( 0), uiErr( 0)
{
	File::SetFilePath( GetFilePath()) ;
	SetSaveName( GetFileName()) ;
}


/******************************************************************************/
// �f�X�g���N�^
//============================================================================//
// �X�V�F02/12/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

Mp3File::~Mp3File() 
{
}


/******************************************************************************/
// �Z�[�u����Ƃ��̖��O��ݒ�
//============================================================================//
// �X�V�F03/02/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Mp3File::SetSaveName( const string& s)
{
	strSaveName = s ;
	strSaveNameInZip = ConvertFilePath( s) ;	// ZIP�ŕۑ�����Ƃ��̃t�@�C����
	intSaveNameSize = strSaveNameInZip.size() ;
}


/******************************************************************************/
// �t�@�C�����擾
//============================================================================//
// �X�V�F03/02/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Mp3File::GetFileData()
{
	struct _stat stat ;
	if( _stat( GetFilePath().c_str(), &stat) == 0)
	{
		ulSize = stat.st_size ;
		lModifiedTime = UnixTime2DosTime(&stat.st_mtime) ;
	}
}


/******************************************************************************/
// zip �Ɋi�[�ł���t�@�C���p�X�ɕύX
//============================================================================//
// �X�V�F02/12/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

string Mp3File::ConvertFilePath( const string& strPath)
{
	char pszTmp[ MAX_PATH] ;
	strcpy_s( pszTmp, strPath.c_str()) ;

	string str ;
	char* pszPtr = pszTmp ;
	while( *pszPtr != '\0')
	{
		// �Q�o�C�g�����łȂ�
		if( IsDBCSLeadByte( *pszPtr) == 0)
		{
			if( *pszPtr == '-')
			{
				str += "[-]" ;
			}
			else if( *pszPtr == '[')
			{
				str += "[[]" ;
			}
			else
			{
				str += *pszPtr ;
			}
		}
		// �Q�o�C�g����
		else
		{
			str += pszPtr[ 0] ;
			str += pszPtr[ 1] ;
		}

		// �ꕶ�����i�߂�
		pszPtr = CharNext( pszPtr) ;
	}

	return string( pszTmp) ;
}


/******************************************************************************/
// time_t -> ULONG �ւ̎��ԕϊ�
//============================================================================//
// �X�V�F02/12/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ULONG Mp3File::UnixTime2DosTime( time_t* t)
{
	time_t	t_even ;
	tm	s ;         /* result of localtime() */

	t_even = (time_t)(((unsigned long)(*t) + 1) & (~1));
	                              /* Round up to even seconds. */
	errno_t err = localtime_s(&s, &t_even);       /* Use local time since MSDOS does. */
	if (err != 0) {
	    /* time conversion error; use current time as emergency value
	       (assuming that localtime() does at least accept this value!) */
	    t_even = (time_t)(((unsigned long)time(NULL) + 1) & (~1));
	    err = localtime_s(&s, &t_even);
		if (err != 0) {
			return 0;
		}
	}

	return GetDosTime(s.tm_year + 1900, s.tm_mon + 1, s.tm_mday,
	               s.tm_hour, s.tm_min, s.tm_sec);
}


/******************************************************************************/
// �N���� -> dostime �ւ̕ϊ�
//============================================================================//
// �X�V�F02/12/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

ULONG Mp3File::GetDosTime( int y, int n, int d, int h, int m, int s)
{
#ifndef DOSTIME_MINIMUM
#define DOSTIME_MINIMUM         ((ULONG)0x00210000L)
#endif

	return y < 1980 ? DOSTIME_MINIMUM /* dostime(1980, 1, 1, 0, 0, 0) */ :
		(((ULONG)y - 1980) << 25) | ((ULONG)n << 21) | ((ULONG)d << 16) |
		((ULONG)h << 11) | ((ULONG)m << 5) | ((ULONG)s >> 1);
}


// Mp3File.h
//============================================================================//
// �X�V�F02/12/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef __Mp3File_h__
#define __Mp3File_h__

#include "include.h"
#include "File.h"


/******************************************************************************/
//		��`
/******************************************************************************/

#define  ERR_ATTR_UNREADABLE	1
#define  ERR_FILE_UNREADABLE	2


/******************************************************************************/
//		�N���X��`
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
// �R���X�g���N�^����уf�X�g���N�^
	Mp3File( File*) ;
	~Mp3File() ;

// �t�@�C���ݒ�
	void SetFilePath( const string&){}

private:
	string ConvertFilePath( const string& strPath) ;
	ULONG  UnixTime2DosTime( time_t* t) ;
	ULONG  GetDosTime( int y, int n, int d, int h, int m, int s) ;
} ;

#endif
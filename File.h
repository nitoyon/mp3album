
// File.h
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef __File_h__
#define __File_h__

#include "include.h"


/******************************************************************************/
//		�N���X��`
/******************************************************************************/

class File
{
protected:
	string	strPath ;

public:
// �R���X�g���N�^����уf�X�g���N�^
	File( const string s = "") ;
	~File() ;

// �t�@�C���p�X�֘A
	string GetFileName() const ;
	string GetBasePath() const ;
	string GetFilePath() const ;
	virtual void SetFilePath( const string& s)
	{
		strPath = s ;
	}
} ;

#endif
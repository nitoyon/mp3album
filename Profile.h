
// Profile.h
// �A�v���P�[�V�����̐ݒ�
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __PROFILE_H__
#define  __PROFILE_H__

#include "include.h"


/******************************************************************************/
//		�N���X��`
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

// ���X�g
	static BOOL blnFullPath ;
	static BOOL blnOnlyMp3 ;

// �ꏊ
	static int intX ;
	static int intY ;

// �ݒ�
	static ZipFolder	intFolder ;
	static ZipFile		intFile ;
	static string		strFolder ;
	static string		strFile ;
	static string		strExtension ;
	static BOOL		blnDisplayLog ;
	static BOOL		blnFailLogOnly ;
	static BOOL		blnConfirmOverwrite ;

// �ǂݏ���
	static void Save() ;
	static void Load() ;

private:
	static void WriteProfile( const string& s, LPTSTR pszSection, LPTSTR pszName, UINT ui) ;
} ;


#endif

// util.cpp
//============================================================================//
// �X�V�F03/02/02(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "util.h"


/******************************************************************************/
// �t���p�X����f�B���N�g���擾
//============================================================================//
// �X�V�F03/02/02(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

string GetDirName( const string& strPath)
{
	int	intLastYen = 0 ;
	char	pszFile[ MAX_PATH] ;
	char*	pszPointer = pszFile ;
	strcpy( pszFile, strPath.c_str()) ;

	for( int i = 0; i < strPath.size(); i++)
	{
		pszPointer = pszFile + i ;

		if( IsDBCSLeadByte( *pszPointer))
		{
			// �Q�o�C�g�����Ȃ�Q�i��
			i++ ;
			continue ;
		}

		if( *pszPointer == '\\' || *pszPointer == '/')
		{
			intLastYen = i ;
		}
	}

	if( intLastYen > 0)
	{
		return strPath.substr( 0, intLastYen + 1) ;	// Yen ���܂߂ĕԂ�
	}
	else
	{
		return "" ;
	}
}


/******************************************************************************/
// �t���p�X����t�@�C�����擾
//============================================================================//
// �X�V�F03/02/02(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

string GetFileName( const string& strPath)
{
	string strDirName = GetDirName( strPath) ;
	return strPath.substr( strDirName.size()) ;
}

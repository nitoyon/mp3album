
// Profile.cpp
// �A�v���P�[�V�����̐ݒ�
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#include "Profile.h"


/******************************************************************************/
//		��`
/******************************************************************************/

string	Profile::strPath = "" ;

// ���X�g
BOOL	Profile::blnFullPath = FALSE ;
BOOL	Profile::blnOnlyMp3 = FALSE ;

// �ꏊ
int	Profile::intX = 0 ;
int	Profile::intY = 0 ;

// �ݒ�
Profile::ZipFolder	Profile::intFolder = Profile::ZipFolder::ONE ;
Profile::ZipFile	Profile::intFile = Profile::ZipFile::FILESET;
string			Profile::strFolder = "" ;
string			Profile::strFile = "" ;
string			Profile::strExtension = "" ;
BOOL			Profile::blnDisplayLog = FALSE ;
BOOL			Profile::blnFailLogOnly = FALSE ;
BOOL			Profile::blnConfirmOverwrite = FALSE ;



/******************************************************************************/
//		�ۑ��E�ǂ݂Ƃ�
/******************************************************************************/
// �ۑ�
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Profile::Save()
{
	if( strPath == "")
	{
		char pszPath[ MAX_PATH] ;
		GetModuleFileName( NULL, pszPath, MAX_PATH) ;
		strPath = pszPath ;
		strPath.replace( strPath.rfind( "."), 4, ".ini", 5) ;
	}
	const char* pszFile = strPath.c_str() ;

	// ���X�g
	WritePrivateProfileString( "list", "FullPath", blnFullPath ? "yes" : "no", pszFile) ;
	WritePrivateProfileString( "list", "OnlyMp3", blnOnlyMp3 ? "yes" : "no", pszFile) ;

	// �ꏊ
	WriteProfile( pszFile, "pos", "x", intX) ;
	WriteProfile( pszFile, "pos", "y", intY) ;

	// �ݒ�
	WriteProfile( pszFile, "DefaultPath", "folder", intFolder) ;
	WriteProfile( pszFile, "DefaultPath", "file", intFile) ;
	WritePrivateProfileString( "DefaultPath", "Extension", ".zip.mp3", pszFile) ;
	WritePrivateProfileString( "DefaultPath", "FilePath", strFile.c_str(), pszFile) ;
	WritePrivateProfileString( "DefaultPath", "FolderPath", strFolder.c_str(), pszFile) ;
	WritePrivateProfileString( "Dialog", "DisplayLog", blnDisplayLog ? "yes" : "no", pszFile) ;
	WritePrivateProfileString( "Dialog", "FailLogOnly", blnFailLogOnly ? "yes" : "no", pszFile) ;
	WritePrivateProfileString( "Dialog", "ConfirmOverwrite", blnConfirmOverwrite ? "yes" : "no", pszFile) ;
}


/******************************************************************************/
// �ǂ݂Ƃ�
//============================================================================//
// �X�V�F02/12/15(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Profile::Load() 
{
	if( strPath == "")
	{
		char pszPath[ MAX_PATH] ;
		GetModuleFileName( NULL, pszPath, MAX_PATH) ;
		strPath = pszPath ;
		strPath.replace( strPath.rfind( "."), 4, ".ini", 5) ;
	}
	const char* pszFile = strPath.c_str() ;
	char	pszBuf[ MAX_PATH] ;

	// ���X�g
	GetPrivateProfileString( "list", "FullPath", "no", pszBuf, MAX_PATH, pszFile) ;
	blnFullPath = ( strcmp( pszBuf, "yes") == 0) ;
	GetPrivateProfileString( "list", "OnlyMp3", "no", pszBuf, MAX_PATH, pszFile) ;
	blnOnlyMp3 = ( strcmp( pszBuf, "yes") == 0) ;

	// �ꏊ
	intX = GetPrivateProfileInt( "pos", "x", 50, pszFile) ;
	intY = GetPrivateProfileInt( "pos", "y", 30, pszFile) ;

	// �ݒ�
	intFolder = (Profile::ZipFolder)GetPrivateProfileInt( "DefaultPath", "folder", (int)ZipFolder::ONE, pszFile) ;
	intFile = (Profile::ZipFile)GetPrivateProfileInt( "DefaultPath", "file", (int)ZipFile::FILESET, pszFile) ;
	GetPrivateProfileString( "DefaultPath", "FolderPath", "", pszBuf, MAX_PATH, pszFile) ;
	strFolder = pszBuf ;
	GetPrivateProfileString( "DefaultPath", "FilePath", "album", pszBuf, MAX_PATH, pszFile) ;
	strFile = pszBuf ;
	GetPrivateProfileString( "DefaultPath", "Extension", ".zip.mp3", pszBuf, MAX_PATH, pszFile) ;
	strExtension = pszBuf ;
	GetPrivateProfileString( "Dialog", "DisplayLog", "yes", pszBuf, MAX_PATH, pszFile) ;
	blnDisplayLog = ( strcmp( pszBuf, "yes") == 0 ? TRUE : FALSE) ;
	GetPrivateProfileString( "Dialog", "FailLogOnly", "yes", pszBuf, MAX_PATH, pszFile) ;
	blnFailLogOnly = ( strcmp( pszBuf, "yes") == 0 ? TRUE : FALSE) ;
	GetPrivateProfileString( "Dialog", "ConfirmOverwrite", "yes", pszBuf, MAX_PATH, pszFile) ;
	blnConfirmOverwrite = ( strcmp( pszBuf, "yes") == 0 ? TRUE : FALSE) ;
}


/******************************************************************************/
//		���[�e�B���e�B�[�֐�
/******************************************************************************/
// ��������������
//============================================================================//
// �X�V�F02/10/27(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

void Profile::WriteProfile( const string& s, LPTSTR pszSection, LPTSTR pszName, UINT ui)
{
	char	pszBuf[ 256] ;
	wsprintf( pszBuf, "%u", ui) ;
	WritePrivateProfileString( pszSection, pszName, pszBuf, s.c_str()) ;
}
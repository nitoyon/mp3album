

// Zip.h
//============================================================================//
// �X�V�F02/12/09(��)
// �T�v�F�Ȃ��B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __ZIP_H__
#define  __ZIP_H__

#include "Mp3File.h"
#include "crc32.h"


/******************************************************************************/
//		��`
/******************************************************************************/

#define PUTSH(a,f)	{putc((char)((a) & 0xff),(f)); putc((char)((a) >> 8),(f));}
#define PUTLG(a,f)	{PUTSH((a) & 0xffff,(f)) PUTSH((a) >> 16,(f))}
//#define SBSZ		16384
class ProcessDlg ;


/******************************************************************************/
//		�v���g�^�C�v�錾
/******************************************************************************/

// ���k
DWORD WINAPI fire( void*) ;
BOOL OutputLocalFileHeader( Mp3File* pMp3File, FILE* fzip) ;
BOOL OutputCentralDirectory( Mp3File* pMp3File, FILE* fzip) ;
BOOL OutputEndCentralDirectory( int intFileNum, ULONG ulDirSize, ULONG ulOffset, FILE* fzip) ;

// ���O
void ZipLog( HWND, int, const string&) ;

#endif


// Zip.h
//============================================================================//
// 更新：02/12/15(日)
// 概要：なし。
// 補足：なし。
//============================================================================//

#ifndef  __ZIP_H__
#define  __ZIP_H__

#include "Mp3File.h"
#include "crc32.h"


/******************************************************************************/
//		定義
/******************************************************************************/

#define PUTSH(a,f)	{putc((char)((a) & 0xff),(f)); putc((char)((a) >> 8),(f));}
#define PUTLG(a,f)	{PUTSH((a) & 0xffff,(f)) PUTSH((a) >> 16,(f))}
#define SBSZ		16384
class ProgressDlg ;


/******************************************************************************/
//		プロトタイプ宣言
/******************************************************************************/

// 圧縮
void _cdecl fire( void*) ;
BOOL GetFileAttr( vector<Mp3File*>* pvecMp3FileList, ProgressDlg* pProgressDlg) ;
BOOL OutputLocalFileHeader( Mp3File* pMp3File, FILE* fzip) ;
BOOL OutputCentralDirectory( Mp3File* pMp3File, FILE* fzip) ;
BOOL OutputEndCentralDirectory( int intFileNum, ULONG ulDirSize, ULONG ulOffset, FILE* fzip) ;

#endif

// debug.h
// �f�o�b�O�֐�
//============================================================================//
// �X�V�F02/11/03(��)
// �T�v�FURL��\���B
// �⑫�F�Ȃ��B
//============================================================================//

#ifndef  __DEBUG_h__
#define  __DEBUG_h__


/******************************************************************************/
//				�C���N���[�h
/******************************************************************************/

#include "include.h"


/******************************************************************************/
//				��`
/******************************************************************************/

void MyOutputDebugString( LPCSTR pszFormat, ...) ;


/******************************************************************************/
// TRACE�}�N��
//============================================================================//
// �X�V�F1996/10/20 (Sun)
// �T�v�F�f�o�b�O�̃��b�Z�[�W�o��
// �⑫�F�Ȃ��B
//============================================================================//

#if defined(_DEBUG) || defined(DEBUG)
// Debug�̂Ƃ�
#define TRACE(x)   OutputDebugString(x)
#define TRACE0(x)   OutputDebugString(x)
#define TRACE1(x, a)            MyOutputDebugString(x, a)
#define TRACE2(x, a, b)         MyOutputDebugString(x, a, b)
#define TRACE3(x, a, b, c)      MyOutputDebugString(x, a, b, c)
#define TRACE4(x, a, b, c, d)   MyOutputDebugString(x, a, b, c, d)
#else
// Release�̂Ƃ�
#define TRACE(x)
#define TRACE0(x)
#define TRACE1(x, a)
#define TRACE2(x, a, b)
#define TRACE3(x, a, b, c)
#define TRACE4(x, a, b, c, d)
#endif


/******************************************************************************/
// ASSERT�}�N��
//============================================================================//
// �X�V�F02/04/09(��)
// �T�v�F�ϐ��`�F�b�N
// �⑫�F�Ȃ��B
//============================================================================//

#if defined(_DEBUG) || defined(DEBUG)
// Debug�̂Ƃ�
#define ASSERT(x) \
    if (!(x)) { \
        MyOutputDebugString("Assertion failed! in %s (%d)\n", \
            __FILE__, __LINE__); \
        DebugBreak(); \
    }
#define VERIFY(x)   ASSERT(x)
#else
// Release�̂Ƃ�
#define ASSERT(x)
#define VERIFY(x)   x
#endif


#endif

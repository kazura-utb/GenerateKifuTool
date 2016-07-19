/***************************************************************************
* Name  : search.h
* Brief : �T���̏����S�ʂ��s��
* Date  : 2016/02/01
****************************************************************************/

#include "stdafx.h"

#pragma once

#include "hash.h"

#define KEY_HASH_MACRO(b, w) (UINT32)((b ^ ((w) >> 1ULL)) % (g_casheSize - 1))

#define ILLIGAL_ARGUMENT 0x80000001
#define MOVE_PASS 0x0

#define ON_MIDDLE 0
#define ON_WINLOSS 1
#define ON_EXACT 2

#define EMPTIES_MID_ORDER_TO_END_ORDER 12
#define EMPTIES_DEEP_TO_SHALLOW_SEARCH 8
#define DEPTH_DEEP_TO_SHALLOW_SEARCH 5

typedef struct
{
	UINT32 color;				// CPU�̐F
	UINT32 casheSize;			// �u���\�̃T�C�Y
	UINT32 searchDepth;			// ���Փǂ݂̐[��
	UINT32 winLossDepth;		// ���s�T�����J�n����[��
	UINT32 exactDepth;			// �΍��T�����J�n����[��
	BOOL   bookFlag;			// ��΂��g�p���邩�ǂ���
	UINT32 bookVariability;	    // ��΂̕ω��x
	BOOL   mpcFlag;				// MPC���g�p���邩�ǂ���
	BOOL   tableFlag;			// �u���\���g�p���邩�ǂ���

}CPUCONFIG;


typedef void(__stdcall *SetMessageToGUI)(char *);
extern SetMessageToGUI g_set_message_funcptr;

/* MPC */
typedef struct
{
	int depth;
	int offset;
	int deviation;
}MPCINFO;

extern BOOL g_tableFlag;
extern char g_cordinates_table[64][4];
extern INT32 g_limitDepth;
extern INT32 g_empty;
extern UINT64 g_casheSize;
extern MPCINFO mpcInfo[22];
extern UINT64 g_countNode;
extern HashTable *g_hash;

extern INT32 g_pvline[20];
extern INT32 g_pvline_len;

UINT64 RandomMove(UINT64 bk, UINT64 wh);
UINT64 SemiBestMove(UINT64 bk, UINT64 wh, UINT32 empty, UINT32 color);

/***************************************************************************
* Name  : GetMoveFromAI
* Brief : CPU�̒����T���ɂ���Č��肷��
* Args  : bk        : ���̃r�b�g��
*         wh        : ���̃r�b�g��
*         empty     : �󔒃}�X�̐�
*         cpuConfig : CPU�̐ݒ�
* Return: ����\�ʒu�̃r�b�g��
****************************************************************************/
UINT64 GetMoveFromAI(UINT64 bk, UINT64 wh, UINT32 emptyNum, CPUCONFIG *cpuConfig);

/***************************************************************************
* Name  : OrderingAlphaBeta
* Brief : ����\��̕��ёւ���󂢒T���ɂ���Č��肷��
* Args  : bk        : ���̃r�b�g��
*         wh        : ���̃r�b�g��
*         depth     : �T�����Ă���m�[�h�̐[��
*         empty     : �󔒃}�X�̐�
*         color     : �T�����Ă���m�[�h�̐F
*         alpha     : 
*         beta      :
*         passed    : �p�X�������̃t���O
* Return: �]���l
****************************************************************************/
INT32 OrderingAlphaBeta(UINT64 bk, UINT64 wh, INT32 depth, INT32 empty, UINT32 color,
	INT32 alpha, INT32 beta, UINT32 passed);

/***************************************************************************
* Name  : SetAbortFlag
* Brief : CPU�̏����𒆒f����
****************************************************************************/
void SetAbortFlag();


INT32 PVS_SearchDeep(UINT64 bk, UINT64 wh, INT32 depth, INT32 empty, UINT32 color,
	HashTable *hash, INT32 alpha, INT32 beta, UINT32 passed);
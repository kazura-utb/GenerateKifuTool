/***************************************************************************
* Name  : move.cpp
* Brief : ����֘A�̌v�Z���s��
* Date  : 2016/02/01
****************************************************************************/
#include "stdafx.h"
#include <math.h>
#include "mt.h"
#include "bit64.h"
#include "board.h"
#include "move.h"
#include "rev.h"
#include "eval.h"
#include "cpu.h"
#include "ordering.h"

typedef void(*INIT_MMX)(void);

BIT_MOB g_bit_mob;
st_bit g_stbit_bk;
st_bit g_stbit_wh;


/***************************************************************************
* Name  : AlocMobilityFunc
* Brief : ����\���񋓂���֐����R�[�����邽�߂�DLL������������
* Return: TRUE:DLL���[�h���� FALSE:DLL���[�h���s
****************************************************************************/
BOOL AlocMobilityFunc()
{
	HMODULE hDLL;

	if ((hDLL = LoadLibrary("mobility.dll")) == NULL)
	{
		return FALSE;
	}

	INIT_MMX init_mmx;
	init_mmx = (INIT_MMX)GetProcAddress(hDLL, "init_mmx");
	g_bit_mob = (BIT_MOB)GetProcAddress(hDLL, "bitboard_mobility");

	(init_mmx)();

	return TRUE;
}

/***************************************************************************
* Name  : CreateMoves
* Args  : bk - ���̔z�u�r�b�g��
* wh - ���̔z�u�r�b�g��
* count - ����\���̊i�[�ϐ�
* Brief : ����\���ƒ���\�ʒu�̃r�b�g����v�Z����
* Return: ����\�ʒu�̃r�b�g��
****************************************************************************/
UINT64 CreateMoves(UINT64 bk_p, UINT64 wh_p, UINT32 *p_count_p)
{
	UINT64 moves;

	g_stbit_bk.high = (bk_p >> 32);
	g_stbit_bk.low = (bk_p & 0xffffffff);

	g_stbit_wh.high = (wh_p >> 32);
	g_stbit_wh.low = (wh_p & 0xffffffff);

	*p_count_p = g_bit_mob(g_stbit_bk, g_stbit_wh, &moves);

	return moves;

}




void StoreMovelist(MoveList *start, UINT64 bk, UINT64 wh, UINT64 moves)
{
	MoveList *list = start + 1, *previous = start;

	while (moves)
	{
		list->move.pos = CountBit((~moves) & (moves - 1));
		list->move.rev = GetRev[list->move.pos](bk, wh);
		previous = previous->next = list;
		moves ^= 1ULL << list->move.pos;
		list++;
	}
	previous->next = NULL;

}

/**
* @brief Get some potential moves.
*
* @param P bitboard with player's discs.
* @param dir flipping direction.
* @return some potential moves in a 64-bit unsigned integer.
*/
UINT64 GetSomePotentialMoves(const unsigned long long P, const int dir)
{
	return (P << dir | P >> dir);
}

/**
* @brief Get potential moves.
*
* Get the list of empty squares in contact of a player square.
*
* @param P bitboard with player's discs.
* @param O bitboard with opponent's discs.
* @return all potential moves in a 64-bit unsigned integer.
*/
UINT64 GetPotentialMoves(UINT64 P, UINT64 O, UINT64 blank)
{
	return (GetSomePotentialMoves(O & 0x7E7E7E7E7E7E7E7Eull, 1) // horizontal
		| GetSomePotentialMoves(O & 0x00FFFFFFFFFFFF00ull, 8)   // vertical
		| GetSomePotentialMoves(O & 0x007E7E7E7E7E7E00ull, 7)   // diagonals
		| GetSomePotentialMoves(O & 0x007E7E7E7E7E7E00ull, 9))
		& blank; // mask with empties
}


BOOL boardMoves(UINT64 *bk, UINT64 *wh, UINT64 move, INT32 pos)
{

	if ((*bk & move) || (*wh & move))
	{
		return FALSE;
	}

	UINT64 rev = GetRev[pos](*bk, *wh);

	if (rev == 0)
	{
		return FALSE;
	}

	*bk ^= (rev | move);
	*wh ^= rev;

	return TRUE;

}

/***************************************************************************
* Name  : RandomMove
* Brief : �]���l�ɂ��d�ݕt�������_������
****************************************************************************/
UINT64 RandomMove(UINT64 bk, UINT64 wh, UINT32 empty, UINT32 color)
{
	MoveList *iter, movelist[36];
	UINT32 move_cnt;
	UINT64 moves = CreateMoves(bk, wh, &move_cnt);
	if (moves == 0) return 0xFFFF;

	// �]���l�Ń\�[�g
	StoreMovelist(movelist, bk, wh, moves);
	SortMoveListBest(movelist, bk, wh, g_hash, empty,
		NEGAMIN, NEGAMAX, color);

	// �ň���Ƀ��X�g�J��
	for (iter = movelist->next; iter->next != NULL; iter = iter->next);

	// �ň���̐�Βl�X�R�A��2�{�̒l��S�Ă̕]���l�ɉ��Z���Ă��̑��a�����߂�
	double rate[36];
	INT64 nom_score_sum = 0;
	INT64 abs_val = (INT64)(llabs(iter->move.score) * 1.2);

	// �ň���̕]����0�ɋ߂��ꍇ�A�ň��肪�قƂ�Ǒł���Ȃ��Ȃ邽�ߑ΍�
	if (abs_val < 4 * EVAL_ONE_STONE)
	{
		abs_val = 4 * EVAL_ONE_STONE;
	}

	for (iter = movelist->next; iter != NULL; iter = iter->next)
	{
		iter->move.score += abs_val;
		nom_score_sum += iter->move.score;
	}

	// ����m�����v�Z
	iter = movelist->next;
	for (int i = 0; iter != NULL; iter = iter->next)
	{
		rate[i++] = iter->move.score / (double)nom_score_sum;
	}

	double rnd = genrand_real1();
	INT32 pos = -1;
	iter = movelist->next;
	for (int j = 0; j < (INT32)move_cnt; j++, iter = iter->next)
	{
		if (rate[j] >= rnd)
		{
			pos = iter->move.pos;
			break;
		}
		rnd -= rate[j];
	}

	if (pos == -1)
	{
		// �ۂߌ덷�ɂ��G���[�������̃P�[�X�͊m�����ɏ��Ȃ̂ŁA�K���ɍőP���I��
		pos = movelist->next->move.pos;
	}

	return 1ULL << pos;
}


/***************************************************************************
* Name  : RandomMove
* Brief :
****************************************************************************/
UINT64 SemiBestMove(UINT64 bk, UINT64 wh, UINT32 empty, UINT32 color)
{
	MoveList movelist[36];
	UINT32 move_cnt;
	UINT64 moves;

	g_empty = empty;
	moves = CreateMoves(bk, wh, &move_cnt);
	StoreMovelist(movelist, bk, wh, moves);

	if (move_cnt == 1)
	{
		return 1ULL << (movelist->next->move.pos);
	}

	SortMoveListBest(movelist, bk, wh, g_hash, empty,
		NEGAMIN, NEGAMAX, color);

	// ���P��ƍőP��Ƃ̌덷��-2�ȉ��̎��̂ݎ��P���ł�
	if (movelist->next->move.score - movelist->next->next->move.score < 20000)
	{
		return 1ULL << (movelist->next->next->move.pos);
	}

	return 1ULL << (movelist->next->move.pos);

}
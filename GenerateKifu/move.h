/***************************************************************************
* Name  : move.h
* Brief : 着手関連の計算を行う
* Date  : 2016/02/01
****************************************************************************/

#include "stdafx.h"

#pragma once

#define MOVE_NONE 0xFF
#define NOMOVE  -1
#define PASS -2

/*! move representation */
typedef struct Move {
	UINT32 pos;
	UINT64 rev;                     /*< flipped discs */
	INT64  score;                   /*< score for this move */
} Move;

/*! (simple) list of a legal moves */
typedef struct MoveList {
	Move move;                /*!< a legal move */
	struct MoveList *next;    /*!< link to next legal move */
} MoveList;

UINT64 CreateMoves(UINT64 bk_p, UINT64 wh_p, UINT32 *p_count_p);
void StoreMovelist(MoveList *list, UINT64 bk, UINT64 wh, UINT64 moves);

UINT64 GetPotentialMoves(UINT64 P, UINT64 O);
INT32 get_potential_mobility(const UINT64 P, const UINT64 O);
INT32 get_weighted_mobility(const UINT64 P, const UINT64 O);

BOOL boardMoves(UINT64 *bk, UINT64 *wh, UINT64 move, INT32 pos);

void  GenerateMoveList(MoveList *moves, UINT64 blank);
MoveList *UpdateMoveList(MoveList *moves, INT32 pos);
void  RestoreMoveList(MoveList *moves, INT32 pos, MoveList *idx);
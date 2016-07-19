// GenerateKifu.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ai_core.h"
#include "move.h"
#include "board.h"
#include "eval.h"
#include "mt.h"

#define MAX_GAME_NUM 3000000
#define FIRST_BK 34628173824
#define FIRST_WH 68853694464

#define EXACT_SEARCH_DEPTH   20
#define MIDDLE_SEARCH_DEPTH   8

void swap(UINT64 *bk, UINT64 *wh)
{
	UINT64 temp;
	temp = *bk;
	*bk = *wh;
	*wh = temp;
}

BOOL boardMoves(UINT64 *bk, UINT64 *wh, UINT64 move, INT32 pos)
{

	if ((*bk & move) || (*wh & move))
	{
		return FALSE;
	}

	UINT64 rev = KZ_GetBoardChangeInfo(*bk, *wh, pos);

	if (rev == 0)
	{
		return FALSE;
	}

	*bk ^= (rev | move);
	*wh ^= rev;

	return TRUE;

}

int _tmain(int argc, _TCHAR* argv[])
{
	
	BOOL ret = KZ_LibInit();

	if (ret == FALSE)
	{
		return -1;
	}

	FILE *wfp;
	UINT32 addr, pass, move_cnt;
	UINT32 retry_count;
	UINT64 bk, wh, move;
	CPUCONFIG cpuConfig = {0};
	int score, pos, empty, modval, color, eval, rnd, randomPosTurn;
	char asciiLine[160];
	BOOL boolret;

	UINT32 error1cnt = 0;
	UINT32 error2cnt = 0;
	UINT32 error3cnt = 0;

	bk = FIRST_BK;
	wh = FIRST_WH;
	cpuConfig.exactDepth = EXACT_SEARCH_DEPTH;
	cpuConfig.winLossDepth = 0;
	cpuConfig.searchDepth = MIDDLE_SEARCH_DEPTH;
	cpuConfig.casheSize = (1 << 22);
	cpuConfig.tableFlag = TRUE;
	//cpuConfig.mpcFlag = TRUE;

	// 180729--->F5決め打ち
	// 250577--->insert space at random move
	for (int i = 258281; i < MAX_GAME_NUM; i++)
	{
		bk = FIRST_BK;
		wh = FIRST_WH;
		empty = 60;
		addr = 0;
		pass = 0;
		cpuConfig.color = BLACK;

		rnd = 52 - (genrand_int32() % 2); // 開始ランダム8手～9手
		// empty = (rnd+4)～21 random(about 85% random)
		randomPosTurn = (genrand_int32() % ((rnd + 4) - EXACT_SEARCH_DEPTH)) + EXACT_SEARCH_DEPTH + 1;

		g_pvline_len = 0;
		memset(g_pvline, 0, sizeof(g_pvline));
		printf("game = %d ", i);
#if 1
		while (true)
		{
			if (empty > rnd)
			{
				if (cpuConfig.color == BLACK)
				{
					if (empty == 60)
					{
						// 1手目F5決め打ち(どこ打っても同じなので)
						move = f5;
					}
					else
					{
						// 開始ランダム8手～9手
						move = RandomMove(bk, wh);

						if (move == 0xFFFF)
						{
							break;
						}
					}

					pos = KZ_CountBit((~move) & (move - 1));
					boardMoves(&bk, &wh, move, pos);
				}
				else
				{
					move = RandomMove(wh, bk);

					if (move == 0xFFFF)
					{
						break;
					}

					pos = KZ_CountBit((~move) & (move - 1));
					boardMoves(&wh, &bk, move, pos);
				}

				asciiLine[addr] = (pos / 8) + 'A';
				asciiLine[addr + 1] = (pos % 8) + '1';
				empty--;
				addr += 2;
				pass = 0;
			}
			else
			{
				if (empty == rnd)
				{
					// random 着手の終わり
					asciiLine[addr++] = ' ';
				}
				if (cpuConfig.color == BLACK)
				{
					if (CreateMoves(bk, wh, &move_cnt))
					{
						// 2%で次善手を選択する8手読み対局
						// 終盤20マス空きからExact探索
						if (empty == randomPosTurn)
						{
							move = SemiBestMove(bk, wh, empty, cpuConfig.color);
						}
						else
						{
							move = GetMoveFromAI(bk, wh, empty, &cpuConfig);
						}
						if (empty > EXACT_SEARCH_DEPTH)
						{
							pos = KZ_CountBit((~move) & (move - 1));
							boolret = boardMoves(&bk, &wh, move, pos);
							if (boolret == FALSE)
							{
								printf("MOVES ERROR\n");
								return 0;
							}
							asciiLine[addr] = (pos / 8) + 'A';
							asciiLine[addr + 1] = (pos % 8) + '1';
							addr += 2;
							if (empty == randomPosTurn)
							{
								asciiLine[addr++] = ' ';
							}
						}
						else
						{
							// Solveした時の色を保存する
							color = cpuConfig.color;
							break;
						}

						empty--;
						pass = 0;
					}
					else
					{
						pass++;
						if (pass == 2) break; //rare case (起こらない気がする)
					}
				}
				else
				{
					if (CreateMoves(wh, bk, &move_cnt))
					{
						// 2%で次善手を選択する8手読み対局
						// 終盤20マス空きからExact探索
						if (empty == randomPosTurn)
						{
							move = SemiBestMove(wh, bk, empty, cpuConfig.color);
						}
						else
						{
							move = GetMoveFromAI(wh, bk, empty, &cpuConfig);
						}
						if (empty > EXACT_SEARCH_DEPTH)
						{
							pos = KZ_CountBit((~move) & (move - 1));
							boolret = boardMoves(&wh, &bk, move, pos);
							if (boolret == FALSE)
							{
								printf("MOVES ERROR\n");
								return 0;
							}
							asciiLine[addr] = (pos / 8) + 'A';
							asciiLine[addr + 1] = (pos % 8) + '1';
							addr += 2;
							if (empty == randomPosTurn)
							{
								asciiLine[addr++] = ' ';
							}
						}
						else
						{
							// Solveした時の色を保存する
							color = cpuConfig.color;
							break;
						}

						empty--;
						pass = 0;
					}
					else
					{
						pass++;
						if (pass == 2) break; //rare case (起こらない気がする)
					}
				}
			}
			// 交代
			cpuConfig.color ^= 1;
		}
#else
		//0x3d103c1812020000ULL, 0x20f03070d1d3f3fULL
		//	45, 23, 46, 4, 2, 47, 31, 39, 22, 38, 63, 15, 7, 1, 55, 62,
		bk = 0x3d103c1812020000ULL;
		wh = 0x20f03070d1d3f3fULL;
		color = 0;
		empty = EXACT_SEARCH_DEPTH;
		move = GetMoveFromAI(bk, wh, empty, &cpuConfig);
#endif

		if (move == 0xFFFF) 
		{
			i--;
			printf("random move fault\n");
			continue;
		}

		if (pass != 2 && KZ_CountBit(~(bk | wh)) != EXACT_SEARCH_DEPTH)
		{
			printf("GAMES ERROR\n");
			return 0;
		}

#if 0
		printf("%llx, %llx\n", bk, wh);
		printf("pass = %d\n", pass);
		printf("g_pvline_len = %d\n", g_pvline_len);
		for (int k = 0; k < EXACT_SEARCH_DEPTH; k++)
		{
			printf("%d, ", g_pvline[k]);
		}
		printf("\n");
#endif

		printf("Exact board state = (0x%llxULL, 0x%llxULL)\n", bk, wh);
		// PV_LINEチェック
		if (pass != 2)
		{
			int j = 0;
			while (j < g_pvline_len)
			{
				if (cpuConfig.color == BLACK)
				{
					boolret = boardMoves(&bk, &wh, 1ULL << g_pvline[j], g_pvline[j]);
					if (boolret == TRUE)
					{
						pass = 0;
						cpuConfig.color ^= 1;
						asciiLine[addr] = (g_pvline[j] / 8) + 'A';
						asciiLine[addr + 1] = (g_pvline[j] % 8) + '1';
						addr += 2;
						j++;
					}
					else
					{
						cpuConfig.color ^= 1;
						pass++;
						if (pass == 2)
						{
							error1cnt++;
							// 間違っている
							printf("1 : PV_LINE ERROR[%d]\n", error1cnt);
							printf("%llx, %llx", bk, wh);
							break;
						}
					}
				}
				else
				{
					boolret = boardMoves(&wh, &bk, 1ULL << g_pvline[j], g_pvline[j]);
					if (boolret == TRUE)
					{
						pass = 0;
						cpuConfig.color ^= 1;
						asciiLine[addr] = (g_pvline[j] / 8) + 'A';
						asciiLine[addr + 1] = (g_pvline[j] % 8) + '1';
						addr += 2;
						j++;
					}
					else
					{
						cpuConfig.color ^= 1;
						pass++;
						if (pass == 2)
						{
							error1cnt++;
							// 間違っている
							printf("1 : PV_LINE ERROR[%d]\n", error1cnt);
							printf("%llx, %llx", bk, wh);
							break;
						}
					}
				}
			}

			if (pass == 2)
			{
				i--;
				continue;
			}

			// まだ打ててたらエラー
			if (CreateMoves(bk, wh, &move_cnt) != 0 || CreateMoves(wh, bk, &move_cnt) != 0)
			{
				// 間違っている
				error2cnt++;
				printf("2 : PV_LINE ERROR[%d]\n", error2cnt);
				printf("%llx, %llx", bk, wh);
				i--;
				continue;
			}

			// スコアを取得
			//printf("color = %d, cpuConfig.color = %d\n", color, cpuConfig.color);
			score = KZ_CountBit(bk) - KZ_CountBit(wh);

			// 評価値は偶数に補正されているため修正
			empty = KZ_CountBit(~(bk | wh));
			if (score > 0) modval = empty;
			else if (score < 0)  modval = -empty;
			else modval = 0;

			// 白がSolveしていたら評価値を反転
			eval = KZ_GetLastEvaluation() * (1 - 2 * color);
			if ((score + modval) != eval)
			{
				// 間違っている
				error3cnt++;
				printf("3 : PV_LINE ERROR[%d]\n", error3cnt);
				printf("%llx, %llx\n", bk, wh);
				printf("score = %d, g_eval = %d\n", score + modval, KZ_GetLastEvaluation());
				i--;
				continue;
			}
		}
		else
		{
			score = KZ_CountBit(bk) - KZ_CountBit(wh);
		}

		asciiLine[addr++] = ' ';
		addr += sprintf_s(&asciiLine[addr], sizeof(asciiLine) - addr, "%d", score);
		asciiLine[addr] = '\0';

		// PV-LINEをファイルに書き込み
#if 1
		retry_count = 5;
		while (retry_count--)
		{
			if (fopen_s(&wfp, "kifu.dat", "a") != 0)
			{
				printf("file cannot open!\n");
				Sleep(1000);
			}
			else
			{
				break;
			}
		}
		if (wfp != NULL)
		{
			fprintf(wfp, "%s\n", asciiLine);
			fclose(wfp);
			wfp = NULL;
		}
#else
		printf("%d : %s\n", i + 1, asciiLine);
#endif
	}

	return 0;
}


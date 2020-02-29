// GenerateKifu.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include "ai_core.h"
#include "move.h"
#include "board.h"
#include "eval.h"
#include "cpu.h"
#include "mt.h"

#define MAX_GAME_NUM 333333
#define FIRST_BK 34628173824
#define FIRST_WH 68853694464


#define EXACT_SEARCH_DEPTH 22
#define MIDDLE_SEARCH_DEPTH 10

#if 0
// 定石データベース
UINT64 database[][2] =
{
	{ 0x1c0c100000, 0x382070200000 }, // 兔
	{ 0x4081c0c080800, 0x302070300000 }, // Sローズ               
	{ 0x8081c0c000000, 0x302070380000 }, // Fローズ
	{ 0x64fc180000, 0x3c1800600000 }, // 手塚システム
	{ 0x10081c100000, 0x81020000000 }, // 馬定石
	{ 0x103c08100000, 0x80030200000 }, // 井上流、雪うさぎ分岐
	{ 0x1c08100000, 0x382030200000 }, // 雪兔
	{ 0x101c08100000, 0x482030200000 }, // 井上流
	{ 0xc0c100000, 0x10181030200000 }, // 横兔
	{ 0x1c0c00180000, 0x123e240000 }, // たまうさぎ
	{ 0x808040e100000, 0x20141a30200000 }, // たまうさぎ亜種
	{ 0x2c00000000, 0x381018100000 }, // 牛最善型
	{ 0x818380000, 0x3e7600000000 }, // 快速船
	{ 0x8080818380000, 0x367600000000 }, // 快速船外し
	{ 0x2418080000, 0x381804100000 }, // 快速船外し２
	{ 0x10002c00000000, 0x4381018100000 }, // こうもり
	{ 0x18002400000000, 0x43c1818100000 }, // こうもり亜種１
	{ 0x10002400000000, 0x3c1818100000 }, // こうもり亜種２
	{ 0x2c10200000, 0x381008100000 }, // 戦車
	{ 0xc00000000, 0x383038000000 }, // 大量取り
	{ 0xc1c2c0000, 0x40c383020000000 }, // 大量取り最善
	{ 0x10080c0c0c0000, 0x40c303030000000 }, // 大量取り最善２
	{ 0x7c0c0c0000, 0x40c380030000000 }, // 大量取り最善３
	{ 0xc08080000, 0x383030000000 }, // 大量取り変化
	{ 0x10103010000000, 0x280808000000 }, // 飛び出し牛
	{ 0x10702010000000, 0x85808000000 }, // 裏快速船
	{ 0x10702000000000, 0x85838000000 }, // 野イチゴ
	{ 0x60e010000000, 0x1010181808000000 }, // 大量取り最善２
	{ 0x1c0c100000, 0x382070200000 }, // 大量取り最善２
	{ 0x1c0c100000, 0x382070200000 }, // 大量取り最善２
	{ 0x60e010000000, 0x1010181808000000 }, // 裏ヨット
	{ 0x8002c40000000, 0x2030385018100000 }, // 岩崎流
	{ 0x10180410000000, 0x207808000000 }, // 裏こうもり
	{ 0x10100000000000, 0x283838000000 }, // リンゴ
	{ 0x20203010000000, 0x180808000000 }, // 蛇
	{ 0x203c2000000000, 0x1818100000 }, // 砂蛇
	{ 0x838202000000000, 0x181818100000 }, // マムシ
	{ 0x28342000000000, 0x10081c18100000 }, // バジリスク
	{ 0x20203000000000, 0x180818200000 }, // コブラ
	{ 0x20202000000000, 0x181838000000 }, // ツチノコ
	{ 0x3018040000, 0x380800000000 }, // バッファロー
	{ 0x1018040000, 0x382840000000 }, // 丸岡バッファロー
	{ 0x40818040000, 0x383020000000 }, // 谷田バッファロー
	{ 0x301c040000, 0x380800080000 }, // 北陸バッファロー
	{ 0x18040000, 0x387800000000 }, // e7バッファロー
	{ 0x8103010000000, 0x280808000000 }, // たぬき
	{ 0x43810000000, 0x380008000000 }, // 裏たぬき
	{ 0x10181c1e0000, 0x80020000400 }, // ローズビル
	{ 0x1018181a0000, 0x80424040400 }, // 酉
	{ 0x10181e0e0800, 0x80420300400 }, // 酉の分岐点
	{ 0x10181e0e0000, 0x80420300c04 }, // 酉フック
	{ 0x101016060000, 0x80c28380c08 }, // 酉ストレート
	{ 0x10181e0e0000, 0x80420301c00 }, // 酉アッパー
	{ 0x10180c160000, 0x10180c160000 }, // 金魚
	{ 0x101014060000, 0x80828180c00 }, // FJT
	{ 0x101010040000, 0x8082c180800 }, // バナナ
	{ 0x101818140000, 0x80024081000 }, // まこ虎
	{ 0x3018080c0000, 0x80034000000 }, // コンポス
	{ 0x7020180c0000, 0xc5824000000 }, // フラットコンポス
	{ 0x3020382c0000, 0xc5804000000 }, // ロジステロ流
	{ 0x301c0c0c0000, 0xc0030000000 }, // ピカチュウ
	{ 0x2008080c0000, 0x10181034000000 }, // ノーカン
	{ 0x2028080c0000, 0x10181034100000 }, // 縦取り殺され
	{ 0x301000040000, 0x8083c080800 }, // Kung
	{ 0x8083c0000, 0x10101030000000 }, // ミサイル
	{ 0x301808040000, 0x80030000000 }, // 龍
	{ 0x101808000000, 0x340c0400 }, // イエス流
	{ 0x101808040000, 0x34081000 }, // Berg Tiger
	{ 0x1038180c0000, 0x80024000000 }, // Leader's Tiger
	{ 0x100c0c0c0000, 0x81030000000 }, // ブライトウェル
	{ 0x100010180000, 0x8182c000000 }, // 猫
	{ 0x818280000, 0x10101020000000 }, // 燕１
	{ 0x201808080000, 0x10100030000000 }, // 燕２
	{ 0x102010080000, 0x8182c000000 }, // 羊
	{ 0xc1c000000, 0x381000000000 }, // 鼠
	{ 0x101c0c14020000, 0x420706a780000 }, // 鼠最善
	{ 0x40e04200000, 0x40e04200000 }, // 旅チュー

};


char *database_ascii[] =
{
	"F5D6C5F4E3C6D3F6E6D7" , // 兔
	"F5D6C5F4E3C6D3F6E6D7G3C4", // Sローズ
	"F5D6C5F4E3C6D3F6E6D7G4C4", // Fローズ
	"F5D6C5F4E3C6D3F6E6D7E7C7C4F3D8", // 手塚システム
	"F5D6C5F4D3", // 馬定石
	"f5d6c5f4e3c6e6", // 井上流、雪うさぎ分岐
	"f5d6c5f4e3c6e6f6", // 雪兔
	"f5d6c5f4e3c6e6f7", // 井上流
	"f5d6c5f4e3c6d3g5", // 横兔
	"f5d6c5f4e3c6d3e2f3c3c4d2", // たまうさぎ
	"f5d6c5f4e3c6d3e2g4f3d2g6", // たまうさぎ亜種
	"f5f6e6f4e3c5", // 牛最善型
	"f5f6e6f4e3c5c4e7c6e2f3f2", // 快速船
	{ 0x10002c00000000, 0x4381018100000 }, // こうもり
	{ 0x18002400000000, 0x43c1818100000 }, // こうもり亜種１
	{ 0x10002400000000, 0x3c1818100000 }, // こうもり亜種２
	{ 0x2c10200000, 0x381008100000 }, // 戦車
	{ 0xc00000000, 0x383038000000 }, // 大量取り
	{ 0xc1c2c0000, 0x40c383020000000 }, // 大量取り最善
	{ 0x10080c0c0c0000, 0x40c303030000000 }, // 大量取り最善２
	{ 0x7c0c0c0000, 0x40c380030000000 }, // 大量取り最善３
	{ 0xc08080000, 0x383030000000 }, // 大量取り変化
	{ 0x10103010000000, 0x280808000000 }, // 飛び出し牛
	{ 0x10702010000000, 0x85808000000 }, // 裏快速船
	{ 0x10702000000000, 0x85838000000 }, // 野イチゴ
	{ 0x60e010000000, 0x1010181808000000 }, // 大量取り最善２
	{ 0x1c0c100000, 0x382070200000 }, // 大量取り最善２
	{ 0x1c0c100000, 0x382070200000 }, // 大量取り最善２
	{ 0x60e010000000, 0x1010181808000000 }, // 裏ヨット
	{ 0x8002c40000000, 0x2030385018100000 }, // 岩崎流
	{ 0x10180410000000, 0x207808000000 }, // 裏こうもり
	{ 0x10100000000000, 0x283838000000 }, // リンゴ
	{ 0x20203010000000, 0x180808000000 }, // 蛇
	{ 0x203c2000000000, 0x1818100000 }, // 砂蛇
	{ 0x838202000000000, 0x181818100000 }, // マムシ
	{ 0x28342000000000, 0x10081c18100000 }, // バジリスク
	{ 0x20203000000000, 0x180818200000 }, // コブラ
	{ 0x20202000000000, 0x181838000000 }, // ツチノコ
	{ 0x3018040000, 0x380800000000 }, // バッファロー
	{ 0x1018040000, 0x382840000000 }, // 丸岡バッファロー
	{ 0x40818040000, 0x383020000000 }, // 谷田バッファロー
	{ 0x301c040000, 0x380800080000 }, // 北陸バッファロー
	{ 0x18040000, 0x387800000000 }, // e7バッファロー
	{ 0x8103010000000, 0x280808000000 }, // たぬき
	{ 0x43810000000, 0x380008000000 }, // 裏たぬき
	{ 0x10181c1e0000, 0x80020000400 }, // ローズビル
	{ 0x1018181a0000, 0x80424040400 }, // 酉
	{ 0x10181e0e0800, 0x80420300400 }, // 酉の分岐点
	{ 0x10181e0e0000, 0x80420300c04 }, // 酉フック
	{ 0x101016060000, 0x80c28380c08 }, // 酉ストレート
	{ 0x10181e0e0000, 0x80420301c00 }, // 酉アッパー
	{ 0x10180c160000, 0x10180c160000 }, // 金魚
	{ 0x101014060000, 0x80828180c00 }, // FJT
	{ 0x101010040000, 0x8082c180800 }, // バナナ
	{ 0x101818140000, 0x80024081000 }, // まこ虎
	{ 0x3018080c0000, 0x80034000000 }, // コンポス
	{ 0x7020180c0000, 0xc5824000000 }, // フラットコンポス
	{ 0x3020382c0000, 0xc5804000000 }, // ロジステロ流
	{ 0x301c0c0c0000, 0xc0030000000 }, // ピカチュウ
	{ 0x2008080c0000, 0x10181034000000 }, // ノーカン
	{ 0x2028080c0000, 0x10181034100000 }, // 縦取り殺され
	{ 0x301000040000, 0x8083c080800 }, // Kung
	{ 0x8083c0000, 0x10101030000000 }, // ミサイル
	{ 0x301808040000, 0x80030000000 }, // 龍
	{ 0x101808000000, 0x340c0400 }, // イエス流
	{ 0x101808040000, 0x34081000 }, // Berg Tiger
	{ 0x1038180c0000, 0x80024000000 }, // Leader's Tiger
	{ 0x100c0c0c0000, 0x81030000000 }, // ブライトウェル
	{ 0x100010180000, 0x8182c000000 }, // 猫
	{ 0x818280000, 0x10101020000000 }, // 燕１
	{ 0x201808080000, 0x10100030000000 }, // 燕２
	{ 0x102010080000, 0x8182c000000 }, // 羊
	{ 0xc1c000000, 0x381000000000 }, // 鼠
	{ 0x101c0c14020000, 0x420706a780000 }, // 鼠最善
	{ 0x40e04200000, 0x40e04200000 }, // 旅チュー

};
#endif


BOOL setMovesToBoard(UINT64 *bk, UINT64 *wh, UINT64 move, INT32 pos, UINT32 color)
{
	UINT64 l_bk, l_wh;
	UINT64 rev;

	l_bk = *bk;
	l_wh = *wh;

	if (color == WHITE) swap(&l_bk, &l_wh);

	if ((l_bk & move) || (l_wh & move))
	{
		return FALSE;
	}

	rev = KZ_GetBoardChangeInfo(l_bk, l_wh, pos);
	if (rev == 0) return FALSE;

	l_bk = l_bk ^ (1ULL << pos) | rev;
	l_wh = l_wh ^ rev;

	if (color == BLACK)
	{
		*bk = l_bk;
		*wh = l_wh;
	}
	else
	{
		*bk = l_wh;
		*wh = l_bk;
	}

	return TRUE;
}


UINT64 RandomMove(UINT64 bk, UINT64 wh, INT32 empty, UINT32 color)
{
	UINT32 count;
	UINT64 moves;
	UINT32 rnd;
	INT32  pos;
	UINT64 l_bk, l_wh;

	l_bk = bk;
	l_wh = wh;
	if (color == WHITE) swap(&l_bk, &l_wh);

	moves = CreateMoves(l_bk, l_wh, &count);
	if (count != 0)
	{
		rnd = genrand_int32() % count;
		do
		{
			pos = KZ_CountBit((~moves) & (moves - 1));
			moves ^= (1ULL << pos);
		} while (rnd--);
	}
	else return MOVE_PASS;

	return 1ULL << pos;
}


int main(int argc, char* argv[])
{
	FILE      *wfp;
	UINT32     addr, pass, move_cnt;
	UINT32     retry_count;
	UINT64     bk, wh, move;
	CPUCONFIG  cpuConfig = {0};
	INT32      score, pos, empty, modval, eval, rnd, solveColor;
	char       asciiLine[256];
	BOOL       boolret;
	//BOOL opening_fix_flag;

	UINT32 error1cnt = 0;
	UINT32 error2cnt = 0;
	UINT32 error3cnt = 0;

	BOOL ret = KZ_LibInit();
	if (ret == FALSE)
	{
		return -1;
	}

	bk = FIRST_BK;
	wh = FIRST_WH;
	cpuConfig.exactDepth = EXACT_SEARCH_DEPTH;
	cpuConfig.winLossDepth = 0;
	cpuConfig.searchDepth = MIDDLE_SEARCH_DEPTH;
	cpuConfig.casheSize = 128 * 1024 * 16;
	cpuConfig.tableFlag = TRUE;
	g_tableFlag = TRUE;
	cpuConfig.mpcFlag = TRUE;
	g_mpcFlag = TRUE;

	init_genrand((UINT32)time(NULL));

	// 自己対戦メインループ
	for (int i = 0; i < MAX_GAME_NUM; i++)
	{
		bk = FIRST_BK;
		wh = FIRST_WH;
		empty = 60;
		addr = 0;
		pass = 0;
		cpuConfig.color = BLACK;

		rnd = 10; // 開始ランダム
		//g_pvline_len = 0;
		//memset(g_pvline, 0, sizeof(g_pvline));
		printf("game = %d ", i);

		memset(asciiLine, 0x00, sizeof(asciiLine));
		// 1ゲームループ
		while (TRUE)
		{
			if (empty == 60)
			{
				move = f5;
				pos = 44;
			}
			else if((60 - empty) < rnd)
			{
				// ランダム着手
				move = RandomMove(bk, wh, empty, cpuConfig.color);
				if (move == MOVE_PASS)
				{
					printf("Random move fault...\n");
					move = 0xFFFF;
					break;
				}
			}
			else
			{
				// AI着手
				move = GetMoveFromAI(bk, wh, empty, &cpuConfig);
				solveColor = cpuConfig.color;
			}

			if (move == MOVE_PASS)
			{
				pass++;
				cpuConfig.color ^= 1;
				if (pass == 2)
				{
					// 終局
					break;
				}
			}
			else
			{
				pos = KZ_CountBit((~move) & (move - 1));
				boolret = setMovesToBoard(&bk, &wh, move, pos, cpuConfig.color);
				if (!boolret)
				{
					printf("BUG!!!\n");
					printf("pos = %d, move = %llx, bk = %llx, wh = %llx\n", pos, move, bk, wh);
					printf("%s\n", asciiLine);
					break;
				}
				asciiLine[addr] = (pos / 8) + 'A';
				asciiLine[addr + 1] = (pos % 8) + '1';
				empty--;
				addr += 2;
				pass = 0;
				cpuConfig.color ^= 1;
			}
		}

		// ランダム着手のみでゲーム終了したものは除外
		if (move == 0xFFFF) 
		{
			i--;
			continue;
		}

		// 万が一のAI不正をトラップ
		if (pass != 2)
		{
			printf("GAMES ERROR\n");
			fopen_s(&wfp, "errolog.txt", "a");
			fprintf(wfp, "%llx, %llx\n", bk, wh);
			fprintf(wfp, "%s\n", asciiLine);
			fclose(wfp);
			continue;
		}

		// まだ打ててたらエラー
		if (CreateMoves(bk, wh, &move_cnt) != 0 || CreateMoves(wh, bk, &move_cnt) != 0)
		{
			// 間違っている
			error2cnt++;
			printf("2 : Solve routine error[%d]\n", error2cnt);
			printf("%llx, %llx\n", bk, wh);
			i--;
			continue;
		}

		// スコアを取得
		score = KZ_CountBit(bk) - KZ_CountBit(wh);

		// 評価値を取得
		eval = KZ_GetLastEvaluation() * (1 - 2 * solveColor);
		// 完全読み切りの前に終了していた場合
		if (eval > 1280000)
		{
			modval = 0;
			eval -= 1280000;
		}
		else if (eval < -1280000)
		{
			modval = 0;
			eval += 1280000;
		}
		else
		{
			// 完全読み切りの評価値は偶数に補正されているため修正
			empty = KZ_CountBit(~(bk | wh));
			if (score > 0) modval = empty;
			else if (score < 0)  modval = -empty;
			else modval = 0;
		}

		if (abs(eval) != 64 && score + modval != eval)
		{
			// 間違っている
			error3cnt++;
			printf("3 : Solve routine error[%d]\n", error3cnt);
			printf("%llx, %llx\n", bk, wh);
			printf("score = %d, g_eval = %d\n", score + modval, eval);
			i--;
			continue;
		}

		asciiLine[addr++] = ' ';
		addr += sprintf_s(&asciiLine[addr], sizeof(asciiLine) - addr, "%d", score);
		addr += sprintf_s(&asciiLine[addr], sizeof(asciiLine) - addr, " %d", 10);
		asciiLine[addr] = '\0';
		printf("eval = %d, score = %d, %llx, %llx\n", eval, score, bk, wh);

		// asciiLineをファイルに書き込み
#if 1
		retry_count = 5;
		wfp = NULL;
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


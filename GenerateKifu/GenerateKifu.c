// GenerateKifu.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
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
// ��΃f�[�^�x�[�X
UINT64 database[][2] =
{
	{ 0x1c0c100000, 0x382070200000 }, // �\
	{ 0x4081c0c080800, 0x302070300000 }, // S���[�Y               
	{ 0x8081c0c000000, 0x302070380000 }, // F���[�Y
	{ 0x64fc180000, 0x3c1800600000 }, // ��˃V�X�e��
	{ 0x10081c100000, 0x81020000000 }, // �n���
	{ 0x103c08100000, 0x80030200000 }, // ��㗬�A�Ⴄ��������
	{ 0x1c08100000, 0x382030200000 }, // ��\
	{ 0x101c08100000, 0x482030200000 }, // ��㗬
	{ 0xc0c100000, 0x10181030200000 }, // ���\
	{ 0x1c0c00180000, 0x123e240000 }, // ���܂�����
	{ 0x808040e100000, 0x20141a30200000 }, // ���܂���������
	{ 0x2c00000000, 0x381018100000 }, // ���őP�^
	{ 0x818380000, 0x3e7600000000 }, // �����D
	{ 0x8080818380000, 0x367600000000 }, // �����D�O��
	{ 0x2418080000, 0x381804100000 }, // �����D�O���Q
	{ 0x10002c00000000, 0x4381018100000 }, // ��������
	{ 0x18002400000000, 0x43c1818100000 }, // �������舟��P
	{ 0x10002400000000, 0x3c1818100000 }, // �������舟��Q
	{ 0x2c10200000, 0x381008100000 }, // ���
	{ 0xc00000000, 0x383038000000 }, // ��ʎ��
	{ 0xc1c2c0000, 0x40c383020000000 }, // ��ʎ��őP
	{ 0x10080c0c0c0000, 0x40c303030000000 }, // ��ʎ��őP�Q
	{ 0x7c0c0c0000, 0x40c380030000000 }, // ��ʎ��őP�R
	{ 0xc08080000, 0x383030000000 }, // ��ʎ��ω�
	{ 0x10103010000000, 0x280808000000 }, // ��яo����
	{ 0x10702010000000, 0x85808000000 }, // �������D
	{ 0x10702000000000, 0x85838000000 }, // ��C�`�S
	{ 0x60e010000000, 0x1010181808000000 }, // ��ʎ��őP�Q
	{ 0x1c0c100000, 0x382070200000 }, // ��ʎ��őP�Q
	{ 0x1c0c100000, 0x382070200000 }, // ��ʎ��őP�Q
	{ 0x60e010000000, 0x1010181808000000 }, // �����b�g
	{ 0x8002c40000000, 0x2030385018100000 }, // ��藬
	{ 0x10180410000000, 0x207808000000 }, // ����������
	{ 0x10100000000000, 0x283838000000 }, // �����S
	{ 0x20203010000000, 0x180808000000 }, // ��
	{ 0x203c2000000000, 0x1818100000 }, // ����
	{ 0x838202000000000, 0x181818100000 }, // �}���V
	{ 0x28342000000000, 0x10081c18100000 }, // �o�W���X�N
	{ 0x20203000000000, 0x180818200000 }, // �R�u��
	{ 0x20202000000000, 0x181838000000 }, // �c�`�m�R
	{ 0x3018040000, 0x380800000000 }, // �o�b�t�@���[
	{ 0x1018040000, 0x382840000000 }, // �ۉ��o�b�t�@���[
	{ 0x40818040000, 0x383020000000 }, // �J�c�o�b�t�@���[
	{ 0x301c040000, 0x380800080000 }, // �k���o�b�t�@���[
	{ 0x18040000, 0x387800000000 }, // e7�o�b�t�@���[
	{ 0x8103010000000, 0x280808000000 }, // ���ʂ�
	{ 0x43810000000, 0x380008000000 }, // �����ʂ�
	{ 0x10181c1e0000, 0x80020000400 }, // ���[�Y�r��
	{ 0x1018181a0000, 0x80424040400 }, // ��
	{ 0x10181e0e0800, 0x80420300400 }, // �т̕���_
	{ 0x10181e0e0000, 0x80420300c04 }, // �уt�b�N
	{ 0x101016060000, 0x80c28380c08 }, // �уX�g���[�g
	{ 0x10181e0e0000, 0x80420301c00 }, // �уA�b�p�[
	{ 0x10180c160000, 0x10180c160000 }, // ����
	{ 0x101014060000, 0x80828180c00 }, // FJT
	{ 0x101010040000, 0x8082c180800 }, // �o�i�i
	{ 0x101818140000, 0x80024081000 }, // �܂���
	{ 0x3018080c0000, 0x80034000000 }, // �R���|�X
	{ 0x7020180c0000, 0xc5824000000 }, // �t���b�g�R���|�X
	{ 0x3020382c0000, 0xc5804000000 }, // ���W�X�e����
	{ 0x301c0c0c0000, 0xc0030000000 }, // �s�J�`���E
	{ 0x2008080c0000, 0x10181034000000 }, // �m�[�J��
	{ 0x2028080c0000, 0x10181034100000 }, // �c���E����
	{ 0x301000040000, 0x8083c080800 }, // Kung
	{ 0x8083c0000, 0x10101030000000 }, // �~�T�C��
	{ 0x301808040000, 0x80030000000 }, // ��
	{ 0x101808000000, 0x340c0400 }, // �C�G�X��
	{ 0x101808040000, 0x34081000 }, // Berg Tiger
	{ 0x1038180c0000, 0x80024000000 }, // Leader's Tiger
	{ 0x100c0c0c0000, 0x81030000000 }, // �u���C�g�E�F��
	{ 0x100010180000, 0x8182c000000 }, // �L
	{ 0x818280000, 0x10101020000000 }, // ���P
	{ 0x201808080000, 0x10100030000000 }, // ���Q
	{ 0x102010080000, 0x8182c000000 }, // �r
	{ 0xc1c000000, 0x381000000000 }, // �l
	{ 0x101c0c14020000, 0x420706a780000 }, // �l�őP
	{ 0x40e04200000, 0x40e04200000 }, // ���`���[

};


char *database_ascii[] =
{
	"F5D6C5F4E3C6D3F6E6D7" , // �\
	"F5D6C5F4E3C6D3F6E6D7G3C4", // S���[�Y
	"F5D6C5F4E3C6D3F6E6D7G4C4", // F���[�Y
	"F5D6C5F4E3C6D3F6E6D7E7C7C4F3D8", // ��˃V�X�e��
	"F5D6C5F4D3", // �n���
	"f5d6c5f4e3c6e6", // ��㗬�A�Ⴄ��������
	"f5d6c5f4e3c6e6f6", // ��\
	"f5d6c5f4e3c6e6f7", // ��㗬
	"f5d6c5f4e3c6d3g5", // ���\
	"f5d6c5f4e3c6d3e2f3c3c4d2", // ���܂�����
	"f5d6c5f4e3c6d3e2g4f3d2g6", // ���܂���������
	"f5f6e6f4e3c5", // ���őP�^
	"f5f6e6f4e3c5c4e7c6e2f3f2", // �����D
	{ 0x10002c00000000, 0x4381018100000 }, // ��������
	{ 0x18002400000000, 0x43c1818100000 }, // �������舟��P
	{ 0x10002400000000, 0x3c1818100000 }, // �������舟��Q
	{ 0x2c10200000, 0x381008100000 }, // ���
	{ 0xc00000000, 0x383038000000 }, // ��ʎ��
	{ 0xc1c2c0000, 0x40c383020000000 }, // ��ʎ��őP
	{ 0x10080c0c0c0000, 0x40c303030000000 }, // ��ʎ��őP�Q
	{ 0x7c0c0c0000, 0x40c380030000000 }, // ��ʎ��őP�R
	{ 0xc08080000, 0x383030000000 }, // ��ʎ��ω�
	{ 0x10103010000000, 0x280808000000 }, // ��яo����
	{ 0x10702010000000, 0x85808000000 }, // �������D
	{ 0x10702000000000, 0x85838000000 }, // ��C�`�S
	{ 0x60e010000000, 0x1010181808000000 }, // ��ʎ��őP�Q
	{ 0x1c0c100000, 0x382070200000 }, // ��ʎ��őP�Q
	{ 0x1c0c100000, 0x382070200000 }, // ��ʎ��őP�Q
	{ 0x60e010000000, 0x1010181808000000 }, // �����b�g
	{ 0x8002c40000000, 0x2030385018100000 }, // ��藬
	{ 0x10180410000000, 0x207808000000 }, // ����������
	{ 0x10100000000000, 0x283838000000 }, // �����S
	{ 0x20203010000000, 0x180808000000 }, // ��
	{ 0x203c2000000000, 0x1818100000 }, // ����
	{ 0x838202000000000, 0x181818100000 }, // �}���V
	{ 0x28342000000000, 0x10081c18100000 }, // �o�W���X�N
	{ 0x20203000000000, 0x180818200000 }, // �R�u��
	{ 0x20202000000000, 0x181838000000 }, // �c�`�m�R
	{ 0x3018040000, 0x380800000000 }, // �o�b�t�@���[
	{ 0x1018040000, 0x382840000000 }, // �ۉ��o�b�t�@���[
	{ 0x40818040000, 0x383020000000 }, // �J�c�o�b�t�@���[
	{ 0x301c040000, 0x380800080000 }, // �k���o�b�t�@���[
	{ 0x18040000, 0x387800000000 }, // e7�o�b�t�@���[
	{ 0x8103010000000, 0x280808000000 }, // ���ʂ�
	{ 0x43810000000, 0x380008000000 }, // �����ʂ�
	{ 0x10181c1e0000, 0x80020000400 }, // ���[�Y�r��
	{ 0x1018181a0000, 0x80424040400 }, // ��
	{ 0x10181e0e0800, 0x80420300400 }, // �т̕���_
	{ 0x10181e0e0000, 0x80420300c04 }, // �уt�b�N
	{ 0x101016060000, 0x80c28380c08 }, // �уX�g���[�g
	{ 0x10181e0e0000, 0x80420301c00 }, // �уA�b�p�[
	{ 0x10180c160000, 0x10180c160000 }, // ����
	{ 0x101014060000, 0x80828180c00 }, // FJT
	{ 0x101010040000, 0x8082c180800 }, // �o�i�i
	{ 0x101818140000, 0x80024081000 }, // �܂���
	{ 0x3018080c0000, 0x80034000000 }, // �R���|�X
	{ 0x7020180c0000, 0xc5824000000 }, // �t���b�g�R���|�X
	{ 0x3020382c0000, 0xc5804000000 }, // ���W�X�e����
	{ 0x301c0c0c0000, 0xc0030000000 }, // �s�J�`���E
	{ 0x2008080c0000, 0x10181034000000 }, // �m�[�J��
	{ 0x2028080c0000, 0x10181034100000 }, // �c���E����
	{ 0x301000040000, 0x8083c080800 }, // Kung
	{ 0x8083c0000, 0x10101030000000 }, // �~�T�C��
	{ 0x301808040000, 0x80030000000 }, // ��
	{ 0x101808000000, 0x340c0400 }, // �C�G�X��
	{ 0x101808040000, 0x34081000 }, // Berg Tiger
	{ 0x1038180c0000, 0x80024000000 }, // Leader's Tiger
	{ 0x100c0c0c0000, 0x81030000000 }, // �u���C�g�E�F��
	{ 0x100010180000, 0x8182c000000 }, // �L
	{ 0x818280000, 0x10101020000000 }, // ���P
	{ 0x201808080000, 0x10100030000000 }, // ���Q
	{ 0x102010080000, 0x8182c000000 }, // �r
	{ 0xc1c000000, 0x381000000000 }, // �l
	{ 0x101c0c14020000, 0x420706a780000 }, // �l�őP
	{ 0x40e04200000, 0x40e04200000 }, // ���`���[

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

	// ���ȑΐ탁�C�����[�v
	for (int i = 0; i < MAX_GAME_NUM; i++)
	{
		bk = FIRST_BK;
		wh = FIRST_WH;
		empty = 60;
		addr = 0;
		pass = 0;
		cpuConfig.color = BLACK;

		rnd = 10; // �J�n�����_��
		//g_pvline_len = 0;
		//memset(g_pvline, 0, sizeof(g_pvline));
		printf("game = %d ", i);

		memset(asciiLine, 0x00, sizeof(asciiLine));
		// 1�Q�[�����[�v
		while (TRUE)
		{
			if (empty == 60)
			{
				move = f5;
				pos = 44;
			}
			else if((60 - empty) < rnd)
			{
				// �����_������
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
				// AI����
				move = GetMoveFromAI(bk, wh, empty, &cpuConfig);
				solveColor = cpuConfig.color;
			}

			if (move == MOVE_PASS)
			{
				pass++;
				cpuConfig.color ^= 1;
				if (pass == 2)
				{
					// �I��
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

		// �����_������݂̂ŃQ�[���I���������̂͏��O
		if (move == 0xFFFF) 
		{
			i--;
			continue;
		}

		// �������AI�s�����g���b�v
		if (pass != 2)
		{
			printf("GAMES ERROR\n");
			fopen_s(&wfp, "errolog.txt", "a");
			fprintf(wfp, "%llx, %llx\n", bk, wh);
			fprintf(wfp, "%s\n", asciiLine);
			fclose(wfp);
			continue;
		}

		// �܂��łĂĂ���G���[
		if (CreateMoves(bk, wh, &move_cnt) != 0 || CreateMoves(wh, bk, &move_cnt) != 0)
		{
			// �Ԉ���Ă���
			error2cnt++;
			printf("2 : Solve routine error[%d]\n", error2cnt);
			printf("%llx, %llx\n", bk, wh);
			i--;
			continue;
		}

		// �X�R�A���擾
		score = KZ_CountBit(bk) - KZ_CountBit(wh);

		// �]���l���擾
		eval = KZ_GetLastEvaluation() * (1 - 2 * solveColor);
		// ���S�ǂݐ؂�̑O�ɏI�����Ă����ꍇ
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
			// ���S�ǂݐ؂�̕]���l�͋����ɕ␳����Ă��邽�ߏC��
			empty = KZ_CountBit(~(bk | wh));
			if (score > 0) modval = empty;
			else if (score < 0)  modval = -empty;
			else modval = 0;
		}

		if (abs(eval) != 64 && score + modval != eval)
		{
			// �Ԉ���Ă���
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

		// asciiLine���t�@�C���ɏ�������
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


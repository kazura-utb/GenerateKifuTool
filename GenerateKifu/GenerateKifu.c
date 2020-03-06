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
#include "rev.h"

#define MAX_GAME_NUM 500000
#define FIRST_BK 34628173824
#define FIRST_WH 68853694464


#define EXACT_SEARCH_DEPTH 22
#define MIDDLE_SEARCH_DEPTH 12

//#define RANDOM10_ENABLE
#define DATABASE_ENABLE

#ifdef DATABASE_ENABLE

char ggg[10528056];

UINT64 g_count_sum;
INT32  g_done_moves;

typedef struct
{
	char *line;
	char *name;
} st_db;

// ��΃f�[�^�x�[�X
st_db database_ascii[] =
{
	//// �\ ////
	{ "F5D6C5F4E3C6D3F6E6D7G3C4",                 "�\��{�`"},
    { "F5D6C5F4E3C6D3F6E6D7G3C4B4B3B5",           "਑����[�Y" },
	{ "F5D6C5F4E3C6D3F6E6D7G3C4B4B3B6",           "਑����[�Y2" },
	{ "F5D6C5F4E3C6D3F6E6D7G3C4B4B3G5",           "਑����[�Y3" },
	{ "F5D6C5F4E3C6D3F6E6D7G3C4E7",               "S���[�Y�E13-e7�@������^" },
	{ "F5D6C5F4E3C6D3F6E6D7G3C4G6",               "S���[�Y�E13-g6�@��[�ā[�����^" },
	{ "F5D6C5F4E3C6D3F6E6D7G3C4G5",               "S���[�Y�E13-g5�@��[�ā[�����^" },
	{ "F5D6C5F4E3C6D3F6E6D7G4C4",                 "F���[�Y�E��{�`" },
	{ "F5D6C5F4E3C6D3F6E6D7G4C4G5C3F7D2E7F2C8",   "F���[�Y�E13-g5�@��[�ā[�����^" },
	{ "F5D6C5F4E3C6D3F6E6D7G4C4G5C3F7D2E7F2E2",   "F���[�Y�E13-g5�@��[�ā[�����^�@19-e2�ω�" },
	{ "F5D6C5F4E3C6D3F6E6D7G4C4G5C3B4",           "F���[�Y�E13-g5 �� ��������^" },
	{ "F5D6C5F4E3C6D3F6E6D7G4C4E7C3G5",           "F���[�Y�E13-e7�@������^" },
	{ "F5D6C5F4E3C6D3F6E6D7G4C4G6",               "F���[�Y�E13-g6�@�i���㗬�j" },
	{ "F5D6C5F4E3C6D3F6E6D7E7C7C4F3D8",           "��˃V�X�e�����{�`" },
	{ "F5D6C5F4E3C6D3F6E6D7E7C7C4F3D8C8B8",       "��˃V�X�e�����C�ł��^" },
	{ "F5D6C5F4E3C6D3F6E6D7E7C7C4F3D8C8F7",       "��˃V�X�e���3�F1�Е�C�ł��^" },
	{ "F5D6C5F4E3C6D3F6E6D7E7C7C4F3D8C8G4",       "��˃V�X�e���17-g4�^" },
	{ "F5D6C5F4E3C6D3F6E6D7E7C7C4F8",             "��˃V�X�e���14-f8�^" },
	{ "F5D6C5F4D3",                               "�n��΁E��{�`" },
	{ "F5D6C5F4D3E3G4G5E6C4G3C6",                 "�n��΁E�őP�^" },
	{ "F5D6C5F4D3E3G4G5E6C4G3F6",                 "�n��΁E12-f6�ω�" },
	{ "F5D6C5F4D3E3G4G5E6C4E2",                   "�n��΁E11-e2�ω�" },
	{ "F5D6C5F4D3E3G4G5E6F6",                     "�n��΁E10-f6�ω�" },
	{ "F5D6C5F4D3C4",                             "�n��΁E�����P��Ή�" },
	{ "F5D6C5F4D7",                               "�ߒ��" },
	{ "F5D6C5F4E3C6F3",                           "�V���[�}�����" },
	{ "F5D6C5F4E3C6E6F6",                         "�Ⴄ����/��㗬���{�`" },
	{ "F5D6C5F4E3C6E6F6G4C4G5C3D7",               "�Ⴄ�����@9-g4��13-d7�^" },
	{ "F5D6C5F4E3C6E6F6G4C4G5C3F7",               "�Ⴄ�����@9-g4��13-f7�^" },
	{ "F5D6C5F4E3C6E6F6D7C7E7",                   "�Ⴄ�����@9-d7��11-e7�^" },
	{ "F5D6C5F4E3C6E6F6D7C7F7",                   "�Ⴄ�����@9-d7��11-f7�^" },
	{ "F5D6C5F4E3C6E6F7D7E8",                     "��㗬�E�őP�^" },
	{ "F5D6C5F4E3C6E6F7D7F6",                     "��㗬�E10-f6�ω�" },
	{ "F5D6C5F4E3C6E6F7D7C7",                     "��㗬�E10-c7�ω�" },
	{ "F5D6C5F4E3C6D3G5",                         "���������E��{�`" },
	{ "F5D6C5F4E3C6D3G5E6",                       "���������E�őP�^" },
	{ "F5D6C5F4E3C6D3G5F6F3G4",                   "���������E9-f6��11-g4�ω��^" },
	{ "F5D6C5F4E3C6D3G5F6F3G6",                   "���������E9-f6�ω��^" },
	{ "F5D6C5F4E3C6D3F3E6F6",                     "Ralle���" },
	{ "F5D6C5F4E3F6F3C4D3",                       "��a�v��" },
	{ "F5D6C5F4E3C6D3E2F3",                       "���܂�����9-f3�^" },
	{ "F5D6C5F4E3C6D3E2G4",                       "���܂�����9-g4�^" },
	{ "F5D6C5F4E3D3E6",                           "�O����7-e6�^" },
	{ "F5D6C5F4E3D3F3",                           "�O����7-f3�^" },
	{ "F5D6C6F4E6",                               "��e��΁E��{�`" },
	{ "F5D6C6F4E6G5D3",                           "��e��΁E7-d3�^" },
	{ "F5D6C6F4E6G5E3",                           "��e��΁E7-e3�^" },
	{ "F5D6C6F4E6G6",                             "��e��΁E6-g6�^" },

	//// �� ////

	{ "F5D6C3D3C4F4C5B3C2",                       "�Ԍ`�i���[�Y�r���j��{�`" },
	{ "F5D6C3D3C4F4C5B3C2E3D2C6B4A3",             "�уt�b�N�@14-a3�^" },
	{ "F5D6C3D3C4F4C5B3C2E3D2C6B4A4",             "�уX�g���[�g�@14-a4�^" },
	{ "F5D6C3D3C4F4C5B3C2E3D2C6B4B5",             "�уA�b�p�[�@14-b5�^" },
	{ "F5D6C3D3C4F4C5B3C2E6C6",                   "�������" },
	{ "F5D6C3D3C4F4C5B3C2E6B4",                   "��������@11-b4�ω��^" },
	{ "F5D6C3D3C4F4C5B3C2B4E3E6C6F6",             "FJT" },
	{ "F5D6C3D3C4F4C5B3C2B4E3E6C6G4",             "FJT�@14-g4�^" },
	{ "F5D6C3D3C4F4C5B3C2B4C6",                   "FJT�@11-c6�ω��^" },
	{ "F5D6C3D3C4F4C5B4B3",                       "�o�i�i���" },
	{ "F5D6C3D3C4F4C5B4B5",                       "�o�i�i��΁@9-b5�ω��^" },
	{ "F5D6C3D3C4F4C5B5",                         "�܂���" },
	{ "F5D6C3D3C4F4F6F3E6E7D7",                   "�R���|�X��{�`" },
	{ "F5D6C3D3C4F4F6F3E6E7D7G6F8F7",             "�R���|�X�F.A.T�^" },
	{ "F5D6C3D3C4F4F6F3E6E7D7G6F8C5",             "�R���|�X�@14-c5�ω��^" },
	{ "F5D6C3D3C4F4F6F3E6E7D7G6D8",               "D8�R���|�X�@13-d8�ω��^" },
	{ "F5D6C3D3C4F4F6F3E6E7D7G6G5C5B6",           "�V���[�v�R���|�X" },
	{ "F5D6C3D3C4F4F6F3E6E7D7G6G5C5C6",           "�t���b�g�R���|�X" },
	{ "F5D6C3D3C4F4F6F3E6E7F7",                   "���܂Ղ�[����{�^" },
	{ "F5D6C3D3C4F4F6F3E6E7F7C5B6G6G5",           "���܂Ղ�[���őP�^" },
	{ "F5D6C3D3C4F4F6F3E6E7F7C5B6G6E3",           "���܂Ղ�[���@15-e3�ω��^" },
	{ "F5D6C3D3C4F4F6F3E6E7F7C5E3",               "���܂Ղ�@13-d3�ω��^" },
	{ "F5D6C3D3C4F4F6F3E6E7C6",                   "���W�X�e������{�`" },
	{ "F5D6C3D3C4F4F6F3E6E7C6G6",                 "���W�X�e���őP�^" },
	{ "F5D6C3D3C4F4F6F3E6E7C6G5",                 "���W�X�e���@12-g5�^" },
	{ "F5D6C3D3C4F4F6F3E3",                       "���C�g�j���O�{���g�E�s�J�`���E" },
	{ "F5D6C3D3C4F4F6F3E3B4",                     "���C�g�j���O�{���g�E���őP�Ή��^" },
	{ "F5D6C3D3C4F4F6F3E3G5",                     "���C�g�j���O�{���g�E�����P�Ή��^" },
	{ "F5D6C3D3C4F4F6G5",                         "�m�[�J����{�`" },
	{ "F5D6C3D3C4F4F6G5E6D7E3C5F3E7H5",           "�m�[�J���őP1�@15-h5�^" },
	{ "F5D6C3D3C4F4F6G5E6D7E3C5F3E7B6",           "�m�[�J���őP2�@15-b6�^" },
	{ "F5D6C3D3C4F4F6G5E6D7E3C5F7E7",             "�m�[�J���@13-f7�ω��^" },
	{ "F5D6C3D3C4F4F6G5E6D7E3C5E7F3",             "�m�[�J���@13-e7�ω��^" },
	{ "F5D6C3D3C4F4F6G5E6D7F7C5B6",               "�m�[�J���@11-f7�ω��^�i�k�����j" },
	{ "F5D6C3D3C4F4F6G5E6D7E7C5F3",               "�m�[�J���@11-e7�ω��^" },
	{ "F5D6C3D3C4F4F6G5E6C5",                     "�m�[�J���@10-c5�^�i�c���E����j" },
	{ "F5D6C3D3C4F4F6G5E3",                       "�m�[�J���@9-e3�ω��^" },
	{ "F5D6C3D3C4F4F6B4F3",                       "Kung" },
	{ "F5D6C3D3C4F4F6B4C2",                       "Kung�@��9-c2�ω��^" },
	{ "F5D6C3G5C6C5C4",                           "�~�T�C�����" },
	{ "F5D6C3F4F6",                               "�����" },
	{ "F5D6C3F4F6D3F3B3C5",                       "���_��΁@9-c5�^" },
	{ "F5D6C3F4F6D3F3B3C7",                       "������΁@9-c7�^" },
	{ "F5D6C3D3C5",                               "�U���K�j���" },
	{ "F5D6C3D3C6",                               "�C�V���" },
	{ "F5D6C3D3C4B3C6F4",                         "�C�G�X�� 8-f4�^" },
	{ "F5D6C3D3C4B3C6B6",                         "�C�G�X�� 8-b6�^" },
	{ "F5D6C3D3C4B5",                             "Berg Tiger" },
	{ "F5D6C3D3C4F4E6B3E2C5C6E3",                 "Leader's Tiger" },
	{ "F5D6C3D3C4F4E6F6E3C5C6B5",                 "Leader's Tiger�@8-f6�ω��i�r����΁j" },
	{ "F5D6C3D3C4F4E6F6E3C5C6B6",                 "Leader's Tiger�@8-f6�ω��^�i������j" },
	{ "F5D6C3D3C4F4E3",                           "�u���C�g�E�F��" },
	//// �� ////
	{ "F5F6E6F4E3C5",                             "����{��΁E��{�`" },
	{ "F5F6E6F4E3C5C4E7C6E2F3F2",                 "�����D�E��b�`" },
	{ "F5F6E6F4E3C5C4E7C6E2F3F2G5",               "�����D�E�őP�`" },
	{ "F5F6E6F4E3C5C4E7C6E2F3F2G4",               "�����D�E13-g4�ω�" },
	{ "F5F6E6F4E3C5C4E7C6E2F3F2G6",               "�����D�E13-g6�ω�" },
	{ "F5F6E6F4E3C5C4E7C6E2F3D3",                 "�����D�O���@12-d3" },
	{ "F5F6E6F4E3C5C4D3F3E2G4G5F2",               "8-d3�ω��@���őP�Ή��^" },
	{ "F5F6E6F4E3C5C4D3C3F2E2D7E7D6",             "8-d3�ω��@�����P�Ή��^" },
	{ "F5F6E6F4E3C5C4E7B5E2F3",                   "���b�g���" },
	{ "F5F6E6F4E3C5C4E7B5E2F2",                   "���b�g��΁@11-f2�ω��^" },
	{ "F5F6E6F4E3C5C4D7C6",                       "���v�D" },
	{ "F5F6E6F4E3C5C4D6B5",                       "�őP�i�s�@9-b5�^" },
	{ "F5F6E6F4E3C5C4D6B6",                       "�őP�i�s�@9-b6�^" },
	{ "F5F6E6F4E3C5C4D6C6",                       "�őP�i�s�@9-c6�^" },
	{ "F5F6E6F4E3C5G5G3",                         "���������΁E��{�`" },
	{ "F5F6E6F4E3C5G5G3G4",                       "���������΁@9-g4�^" },
	{ "F5F6E6F4E3C5G5G3C6",                       "���������΁@9-c6�^" },
	{ "F5F6E6F4E3C5G5F3",                         "���������΁@8-f3�^" },
	{ "F5F6E6F4E3C5C6",                           "��Ԓ��" },
	{ "F5F6E6F4E3D6",                             "����ʎ��" },
	{ "F5F6E6F4E3D6G4D3C3H3C4G3C6",               "����ʁE�őP�i�s�@13-c6�^" },
	{ "F5F6E6F4E3D6G4D3C3H3C4G3G5",               "�őP�i�s�@13-g5�^" },
	{ "F5F6E6F4E3D6G4D3C3H3C4G3E7",               "�őP�i�s�@13-e7�^" },
	{ "F5F6E6F4E3D6C4",                           "7-c4�^" },
	{ "F5F6E6F4G5",                               "��o�����E��{�`" },
	{ "F5F6E6F4G5E7F7H5",                         "�������D�E��{�`" },
	{ "F5F6E6F4G5E7F7H5G4G6E3C5",                 "�������D�@9-g4�^ver1" },
	{ "F5F6E6F4G5E7F7H5G4G6E3H4",                 "�������D�@9-g4�^ver2" },
	{ "F5F6E6F4G5E7F7H5G4G6E3F3",                 "�������D�@9-g4�^ver3" },
	{ "F5F6E6F4G5E7F7H5E3D3G3G4",                 "�������D�@9-e3�^ver1" },
	{ "F5F6E6F4G5E7F7H5E3D3G3F3",                 "�������D�@9-e3�^ver2" },
	{ "F5F6E6F4G5E7F7H5E3D3G4",                   "�������D�@9-e3�ω��^�@�i���㗬�j" },
	{ "F5F6E6F4G5E7F7C5",                         "�������D�@8-c5�ω��^" },
	{ "F5F6E6F4G5E7F7D6",                         "�삢�������" },
	{ "F5F6E6F4G5E7F7H5",                         "�����b�g���" },
	{ "F5F6E6F4G5E7D7",                           "7-d7�ω��^�@�i��藬�j" },
	{ "F5F6E6F4G5E7E3",                           "����������E��{�`" },
	{ "F5F6E6F4G5E7E3G6D3",                       "���������� 8-g6�^��9-d3" },
	{ "F5F6E6F4G5E7E3G6D6",                       "���������� 8-g6�^��9-d6" },
	{ "F5F6E6F4G5E7E3F3",                         "���������� 8-f3�^" },
	{ "F5F6E6F4G5E7E3G4",                         "���������� 8-g4�ω��^" },
	{ "F5F6E6F4G5D6E7G6",                         "�����S���" },
	{ "F5F6E6F4G5D6E7G4",                         "�����S��� 8-g4�^" },
	{ "F5F6E6F4G6",                               "�֒�΁E��{�`" },
	{ "F5F6E6F4G6C5F3",                           "���ցE��{�`" },
	{ "F5F6E6F4G6C5F3E7C3",                       "���ց@9-c3�^" },
	{ "F5F6E6F4G6C5F3E7D6",                       "���ց@9-d6�^" },
	{ "F5F6E6F4G6C5F3G4",                         "���ց@8-g4�ω��^" },
	{ "F5F6E6F4G6C5F3G5",                         "���ց@8-g5�ω��^" },
	{ "F5F6E6F4G6C5G4G5H4",                       "�}���V��΁E��{�`" },
	{ "F5F6E6F4G6C5G4G5H4H3H2",                   "�}���V��΁@11-h2�^" },
	{ "F5F6E6F4G6C5G4G5H4H3F3",                   "�}���V��΁@11-f3�^" },
	{ "F5F6E6F4G6C5G4G5H4H6",                     "�}���V��΁@10-h6�^" },
	{ "F5F6E6F4G6C5G4G5F3E3",                     "�o�V���X�N��΁E��{�`" },
	{ "F5F6E6F4G6C5G4G5F3E3C4",                   "�o�V���X�N��΁@11-c4�^" },
	{ "F5F6E6F4G6C5G4G5F3E3H5",                   "�o�V���X�N��΁@11-h5�^" },
	{ "F5F6E6F4G6C6",                             "�R�u����΁E��{�`�iLysons�j" },
	{ "F5F6E6F4G6C6G5",                           "�R�u����΁@7-g5�^" },
	{ "F5F6E6F4G6C6F3G5",                         "�R�u����΁@7-f3�^" },
	{ "F5F6E6F4G6C6F3E7",                         "�R�u����΁@7-f3�^�@8-e7�ω�" },
	{ "F5F6E6F4G6D6",                             "���̂���΁E��{�`" },
	{ "F5F6E6F4G6D6E3",                           "���̂���΁@7-e3�^" },
	{ "F5F6E6F4G6D6G4",                           "���̂���΁@7-g4�^" },
	{ "F5F6E6F4G6D6C6",                           "���̂���΁@7-c6�^" },
	{ "F5F6E6F4C3",                               "�o�b�t�@���[��΁E��{�`" },
	{ "F5F6E6F4C3D7",                             "�ۉ��o�b�t�@���[�E��{�`" },
	{ "F5F6E6F4C3D7F3D6G5",                       "�ۉ��o�b�t�@���[�@7-f3��9-g5�^" },
	{ "F5F6E6F4C3D7F3D6E7",                       "�ۉ��o�b�t�@���[�@7-f3��9-e7�^" },
	{ "F5F6E6F4C3D7E3",                           "�ۉ��o�b�t�@���[�@7-e3�^" },
	{ "F5F6E6F4C3D6F3",                           "�J�c�o�b�t�@���[�E��{�`" },
	{ "F5F6E6F4C3D6F3C4",                         "�J�c�o�b�t�@���[�E�őP�^" },
	{ "F5F6E6F4C3D6F3E3",                         "�J�c�o�b�t�@���[�@8-e3�ω��^" },
	{ "F5F6E6F4C3D6F3C5",                         "�J�c�o�b�t�@���[�@8-c5�ω��^" },
	{ "F5F6E6F4C3C4D3",                           "�k���o�b�t�@���[�E��{�`" },
	{ "F5F6E6F4C3C4D3D6",                         "�k���o�b�t�@���[ 8-d6�^" },
	{ "F5F6E6F4C3C4D3C6",                         "�k���o�b�t�@���[ 8-c6�^" },
	{ "F5F6E6F4C3E7",                             "e7�o�b�t�@���[�E��{�`" },
	{ "F5F6E6F4C3E7F3F2",                         "e7�o�b�t�@���[�@8-f2�^" },
    { "F5F6E6F4C3E7F3E3",                         "e7�o�b�t�@���[�@8-e3�^" },
	{ "F5F6E6F4G3",                               "���P�b�g��΁E��{�`" },
	{ "F5F6E6F4G3D6",                             "���P�b�g��΁E�őP�^" },
	{ "F5F6E6F4G3F3",                             "���P�b�g��΁@6-f3�ω��^" },
	{ "F5F6E6F4D3",                               "���֒�΁E��{�`" },
	{ "F5F6E6F4D3D6F3",                           "���֒�΁E�őP�`" },
	{ "F5F6E6F4D3D6G5",                           "���֒�΁@7-g5�^" },
	{ "F5F6E6F4D3E7",                             "���֒�΁@6-e7�ω��^" },
	{ "F5F6E6F4G4",                               "�K��΁E��{�`" },
	{ "F5F6E6F4G4E7",                             "�K��΁E�őP�^" },
	{ "F5F6E6F4F3",                               "���K��΁E��{�`" },
	{ "F5F6E6F4F3C5",                             "���K��΁E�őP�^" },
	{ "F5F6E6F4G7",                               "�\������΁E��{�`" },

	//// �l ////
	{ "F5F4E3",                                   "�l�Y�~��΁E��{�`" },
	{ "F5F4E3F6",                                 "�l�Y�~��΁E�őP�^" },
	{ "F5F4E3F2",                                 "�l�Y�~��΁@4-f2�ω��^�i���`���[�j" },
	{ "F5F4E3D2",                                 "�l�Y�~��΁@4-f2�ω��^�iT���j" },
	{ "F5F4E3D6",                                 "�l�Y�~��΁@4-f2�ω��^�i�l�Y�~���j" },
	{ "F5F4F3",                                   "�׎Ԓ�΁E��{�`" },
	{ "F5F4F3D6",                                 "�׎Ԓ�΁E�׎ԗ�" },
	{ "F5F4F3F6",                                 "�׎Ԓ�΁E�S������" },
	{ "F5F4F3G4",                                 "�׎Ԓ�΁E�֎q��" },

	//// ���̑� ////
	{ "F5D6C4D3C5F4",                             "�L��΁E��{�`" },
	{ "F5D6C4D3C5F4E3F3E2",                       "�L��΁E�őP�^" },
	{ "F5D6C4D3C5F4E3F3C2",                       "�L��΁E�����" },
	{ "F5D6C4D3C5F4E3F3G4",                       "�L��΁@9-g4�^" },
	{ "F5D6C4G5",                                 "No Cat�E��{�`" },
	{ "F5D6C4G5C6C5D7D3B4C3E3B5",                 "�����" },
	{ "F5D6C4G5C6C5D7D3B4C3E3F3",                 "����΁@12-f3�^" },
	{ "F5D6C4G5F6",                               "���b�R���" },
	{ "F5D6C4D3E6F4E3F3",                         "�r��΁E��{�`" },
	{ "F5D6C4D3E6F4E3F3C6F6G5G6",                 "�r��΁E�őP�^" },
	{ "F5D6C4D3E6F4E3F3C6F6G5E7",                 "�r��΁@12-e7�ω��^" }
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

	//g_count_sum += count;
	//g_done_moves++;
	//printf("Num of random moves average = %lf\n", g_count_sum / (double)g_done_moves);

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


INT32 checkMove(UINT64 *bk, UINT64 *wh, int color, INT32 move)
{
	INT32  ret;
	UINT64 rev, l_bk, l_wh;

	l_bk = *bk;
	l_wh = *wh;

	ret = 0;

	if (color == WHITE) swap(&l_bk, &l_wh);

	rev = GetRev[move](l_bk, l_wh);
	/* ���p�X�H */
	if (rev == 0)
	{
		ret = -1;
	}
	else
	{
		l_bk ^= ((1ULL << move) | rev);
		l_wh ^= rev;
	}

	if (ret == 0)
	{
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
	}


	return ret;
}


#ifdef DATABASE_ENABLE
INT32 doDatabaseMoves(st_db* db, UINT64 *bk, UINT64 *wh)
{
	INT32  move, color;
	INT32  i;
	UINT64 l_bk, l_wh;

	color = BLACK;
	l_bk = *bk;
	l_wh = *wh;

	for (i = 0; i < strlen(db->line); i += 2)
	{
		move = (db->line[i] - 'A') * 8;
		move += db->line[i + 1] - '1';
		if (checkMove(&l_bk, &l_wh, color, move) == -1)
		{
			printf("databese failed!!![%s : %s]\n", db->name, db->line);
			break;
		}
		color ^= 1;
	}

	*bk = l_bk;
	*wh = l_wh;

	return i / 2;
}
#endif

int main(int argc, char* argv[])
{
	FILE      *wfp;
	UINT32     addr, pass, move_cnt;
	UINT32     retry_count;
	UINT64     bk, wh, move;
	CPUCONFIG  cpuConfig = {0};
	INT32      score, pos, empty, modval, eval, rnd, solveColor;
	INT32      rnd_b, rnd_w;
	char       asciiLine[256];
	BOOL       boolret;
	UINT32     error1cnt = 0;
	UINT32     error2cnt = 0;
	UINT32     error3cnt = 0;
	INT32      b_eval;
#ifdef DATABASE_ENABLE
	INT32      turn;
	st_db     *db_ptr;
	INT32      db_len;
#endif

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

#if 0
	// ���ȑ΋ǃV�~��
	int n = 0;
	int c = 0;
	int end = 0;
	while (end < 500000)
	{
		n++;
		end++;
		rnd = genrand_int32() % 10528056;
		if (ggg[rnd] == 0)
		{
			ggg[rnd] = 1;
		}
		else
		{
			c++;
			if ((c % 100) == 0)
			{
				printf("At %d, Collision rate = %lf%%, end = %d\n", n, (c / (double)n) * 100, end);
			}
			end--;
		}
	}
	printf("At %d, Collision rate = %lf%%, end = %d\n", n, (c / (double)n) * 100, end);
	printf("200���ǂ̎��W�܂łɂ����������[�v�� : %d\n", n);

	exit(1);
#endif

#ifdef DATABASE_ENABLE
	rnd = 0;
	// �f�[�^�x�[�X�`�F�b�N
	printf("------------------------------��Έꗗ------------------------------\n");
	while (rnd < (sizeof(database_ascii) / sizeof(st_db)) && (strlen(database_ascii[rnd].line) % 2) == 0)
	{
		printf("rnd = %d, line = %s, name = %s\n", rnd, database_ascii[rnd].line, database_ascii[rnd].name);
		rnd++;
	}
	if (rnd < (sizeof(database_ascii) / sizeof(st_db)))
	{
		printf("rnd = %d, line = %s, name = %s\n", rnd, database_ascii[rnd].line, database_ascii[rnd].name);
		return;
	}
	printf("--------------------------------------------------------------------\n");
#endif

	// ���ȑΐ탁�C�����[�v
	for (int i = 0; i < MAX_GAME_NUM; i++)
	{
		HashClear(g_hash);
		HashClear(g_pvHash);

		bk = FIRST_BK;
		wh = FIRST_WH;
		empty = 60;
		addr = 0;
		pass = 0;
		cpuConfig.color = BLACK;

#ifdef DATABASE_ENABLE
		rnd = genrand_int32() % (sizeof(database_ascii) / sizeof(st_db));
	    db_ptr = &database_ascii[rnd];
		db_len = strlen(db_ptr->line);
		printf("game = %d  (%s) --> ", i, db_ptr->name);
#else
		rnd = 10; // �J�n�����_��
		printf("game = %d ", i);
#endif

		memset(asciiLine, 0x00, sizeof(asciiLine));
		// 1�Q�[�����[�v
		while (TRUE)
		{
#ifdef RANDOM10_ENABLE
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
#else
			if (db_ptr != NULL)
			{
				// ��΃f�[�^���璅��
				turn = doDatabaseMoves(db_ptr, &bk, &wh);
				empty -= turn;
				strcpy_s(asciiLine, sizeof(asciiLine), db_ptr->line);
				addr += 2 * turn;
				pass = 0;
				cpuConfig.color ^= turn % 2;
				db_ptr = NULL;
				// ���Ɣ��̃����_������^�[�����̌���
				rnd_b = (genrand_int32() % (empty - EXACT_SEARCH_DEPTH)) + turn;
				while((rnd_w = (genrand_int32() % (empty - EXACT_SEARCH_DEPTH)) + turn) == rnd_b);
				continue;
			}
			else if (60 - empty == rnd_b || 60 - empty == rnd_w)
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
#endif
			else
			{
#if 0
				if (abs(rnd_b - rnd_w) == 1)
				{
					if ((60 - empty == rnd_b + 1 || 60 - empty == rnd_w + 1))
					{
						b_eval = KZ_GetLastEvaluation() * (1 - 2 * cpuConfig.color);
						move = GetMoveFromAI(bk, wh, empty, &cpuConfig);
						solveColor = cpuConfig.color;
						eval = KZ_GetLastEvaluation() * (1 - 2 * cpuConfig.color);
						printf("(bk&wh rnd) eval = %.2lf --> %.2lf ", b_eval / (double)EVAL_ONE_STONE, eval / (double)EVAL_ONE_STONE);
					}
					else
					{
						move = GetMoveFromAI(bk, wh, empty, &cpuConfig);
						solveColor = cpuConfig.color;
					}
				}
				else if (60 - empty == rnd_b + 1)
				{
					b_eval = KZ_GetLastEvaluation() * (1 - 2 * cpuConfig.color);
					move = GetMoveFromAI(bk, wh, empty, &cpuConfig);
					solveColor = cpuConfig.color;
					eval = KZ_GetLastEvaluation() * (1 - 2 * cpuConfig.color);
					printf("(bk rnd) eval = %.2lf --> %.2lf ", b_eval / (double)EVAL_ONE_STONE, eval / (double)EVAL_ONE_STONE);
				}
				else if(60 - empty == rnd_w + 1)
				{
					b_eval = KZ_GetLastEvaluation() * (1 - 2 * cpuConfig.color);
					move = GetMoveFromAI(bk, wh, empty, &cpuConfig);
					solveColor = cpuConfig.color;
					eval = KZ_GetLastEvaluation() * (1 - 2 * cpuConfig.color);
					printf("(wh rnd) eval = %.2lf --> %.2lf ", b_eval / (double)EVAL_ONE_STONE, eval / (double)EVAL_ONE_STONE);
				}
				else
				{
					move = GetMoveFromAI(bk, wh, empty, &cpuConfig);
					solveColor = cpuConfig.color;
				}
#else
				move = GetMoveFromAI(bk, wh, empty, &cpuConfig);
				solveColor = cpuConfig.color;
#endif
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
					fopen_s(&wfp, "errolog.txt", "a");
					fprintf(wfp, "%llx, %llx\n", bk, wh);
					fprintf(wfp, "%s\n", asciiLine);
					fclose(wfp);
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
		if (rnd_b > rnd_w) rnd = rnd_w;
		else rnd = rnd_b;
		addr += sprintf_s(&asciiLine[addr], sizeof(asciiLine) - addr, " %d", rnd); // rnd + 1 --> end random moves
		asciiLine[addr] = '\0';
		printf("score = %d\n", score);

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


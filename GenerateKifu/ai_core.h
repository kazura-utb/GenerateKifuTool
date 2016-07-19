
#include "stdafx.h"

#include "cpu.h"

/***************************************************************************
* Name  : KZ_LibInit
* Brief : �������������s��
* Return: TRUE/FALSE
****************************************************************************/
BOOL KZ_LibInit();
/***************************************************************************
* Name  : KZ_EnumGetCpuMove
* Brief : ����\���񋓂���
* Return: ����\�ʒu�̃r�b�g��
****************************************************************************/
UINT64 KZ_GetEnumMove(UINT64 bk_p, UINT64 wh_p, UINT32 *p_count_p);
/***************************************************************************
* Name  : KZ_EnumGetCpuMove
* Brief : �ω�����ӏ����v�Z���A�r�b�g��ɂ��ĕԋp����
* Return: �ω�����ӏ��̃r�b�g��
****************************************************************************/
UINT64 KZ_GetBoardChangeInfo(UINT64 bk, UINT64 wh, INT32 move);

/***************************************************************************
* Name  : KZ_GetCpuMove
* Brief : ��΂�]���l����CPU�̒�����v�Z����
* Args  : bk ���̔Ֆʏ��
*         wh ���̔Ֆʏ��
*         cpuConfig CPU�ݒ�N���X
* Return: ����\�ʒu�̃r�b�g��
****************************************************************************/
UINT64 KZ_GetCpuMove(UINT64 bk, UINT64 wh, CPUCONFIG *cpuConfig);

/***************************************************************************
* Name  : KZ_GetLastEvaluation
* Brief : ���O��CPU�̒���ɑΉ�����]���l���擾����
* Return: CPU�̎Z�o�������ߕ]���l
****************************************************************************/
INT32 KZ_GetLastEvaluation();

/***************************************************************************
* Name  : KZ_GetIsUseBook
* Brief : ���O��CPU�̒��肪��Ύ肩�ǂ������擾����
* Return: CPU�̎Z�o�������ߕ]���l
****************************************************************************/
BOOL KZ_GetIsUseBook();


/***************************************************************************
* Name  : KZ_GetCountNode
* Brief : �T���ς݂̃m�[�h�����擾����
* Return: CPU���T�������m�[�h��
****************************************************************************/
UINT64 KZ_GetCountNode();

/***************************************************************************
* Name  : KZ_SendAbort
* Brief : AI�X���b�h�ɒ��f���߂𑗐M
* Return: ����\�ʒu�̃r�b�g��
****************************************************************************/
void KZ_SendAbort();

/***************************************************************************
* Name  : KZ_CountBit
* Brief : �P�������Ă���r�b�g���𐔂���
* Args  : bit �P�������Ă���r�b�g�𐔂���Ώۂ̃r�b�g��
* Return: �P�������Ă���r�b�g��
****************************************************************************/
UINT32 KZ_CountBit(UINT64 bit);

/***************************************************************************
* Name  : KZ_ReleaseHash
* Brief : �n�b�V���e�[�u�����������
****************************************************************************/
void KZ_ReleaseHash();
/***************************************************************************
* Name  : KZ_ReleaseBook
* Brief : ��΃f�[�^���������
****************************************************************************/
void KZ_ReleaseBook();
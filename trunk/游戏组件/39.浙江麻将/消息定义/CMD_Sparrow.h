#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE

#include "..\\��Ϸ������\\GameLogic.h"
//////////////////////////////////////////////////////////////////////////

//�����궨��
#define KIND_ID						305						//��Ϸ I D
#define GAME_PLAYER					4									//��Ϸ����
#define GAME_NAME					TEXT("�㽭�齫")					//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����

//////////////////////////////////////////////////////////////////////////

//�������
struct CMD_WeaveItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	WORD							wProvideUser;						//��Ӧ�û�
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				101									//��������
#define SUB_S_OPERATE_CARD			102									//��������
#define SUB_S_SEND_CARD				103									//�����˿�
#define SUB_S_GAME_END				104									//��Ϸ����
#define SUB_S_SYS_MSG				105									//ϵͳ��Ϣ
#define SUB_S_USER_READY			106									//���׼��
#define SUB_S_STAKE					107									//ע��ı�

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lCellScore;							//�������
	LONG							lLeftScore[4];							//ʣ����
	WORD							wBankerUser;						//ׯ���û�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	LONG							lCellScore;							//��Ԫ����
	LONG							lLeftScore[4];							//ʣ����
	WORD							wSiceCount;							//���ӵ���
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�

	//״̬����
	BYTE							cbChihu;							//�Ժ���־
	BYTE							cbResponse;							//��Ӧ��־
	BYTE							cbLeftCardCount;					//ʣ����Ŀ

	//������Ϣ
	BYTE							cbOutCardCount;						//������Ŀ
	WORD							wOutCardUser[108];					//�����û�
	BYTE							cbOutCardData[108];					//�����˿�

	//�˿�����
	BYTE							cbKingCardData;						//�����˿�
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[14];						//�˿��б�

	//����˿�
	BYTE							bWeaveCount[4];						//�����Ŀ
	CMD_WeaveItem					WeaveItemArray[4][4];				//����˿�
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wSiceCount;							//���ӵ���
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�
	BYTE							cbCardData[14];						//�˿��б�
	BYTE							cbKingCardData;						//�����˿�
};

//��������
struct CMD_S_OutCard
{
	WORD							wCurrentUser;						//��ǰ�û�
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
	BYTE							cbCurrentCard;						//��ǰ�˿�
};

//��������
struct CMD_S_OperateCard
{
	WORD							wCurrentUser;						//��ǰ�û�
	WORD							wOperateUser;						//�����û�
	WORD							wProvideUser;						//�ṩ�û�
	BYTE							cbCurrentCard;						//�����˿�
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
};

//�����˿�
struct CMD_S_SendCard
{
	BYTE							cbCardData;							//�˿�����
	WORD							wCurrentUser;						//��ǰ�û�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	tagHuCourt						stHuCourt;							//��������
	LONG							lGameScore[4];						//��Ϸ����
	LONG							lLeftScore[4];						//ʣ�����
	BYTE							cbCardCount[4];						//�˿���Ŀ
	BYTE							cbCardData[4][14];					//�˿�����
	BYTE							cbSelfReady;						//�´�ready����
};

//ϵͳ��Ϣ
struct CMD_S_SysMsg
{
	BYTE							cbBuf[128];							//��Ϣ����
};

//���׼��
struct CMD_S_UserReady
{
	WORD							wChairID;							//׼���û�
};

//ע��ı�
struct CMD_S_Stake
{
	LONG							lCellScore;							//��λ���
	LONG							lLeftScore[4];							//ʣ����
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//��������
#define SUB_C_OPERATE_CARD			2									//�����˿�
#define	SUB_C_READY					3									//ready�ź�

//��������
struct CMD_C_OutCard
{
	BYTE							cbCardData;							//�˿�����
};

//��������
struct CMD_C_OperateCard
{
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
};

//////////////////////////////////////////////////////////////////////////

#endif
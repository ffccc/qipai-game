#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE
//////////////////////////////////////////////////////////////////////////
//�����궨��
#define MAX_COUNT					14									//�����Ŀ
#define KIND_ID						322									//��Ϸ I D
#define GAME_PLAYER					4									//��Ϸ����
#define GAME_REMAIN					18									//��������	
#define GAME_NAME					TEXT("�����齫")					//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH|GAME_GENRE_GOLD)	//��Ϸ����

//��Ϸ״̬
#define GS_MJ_FREE					GS_FREE								//����״̬
#define GS_MJ_PLAY					(GS_PLAYING+1)						//��Ϸ״̬

//////////////////////////////////////////////////////////////////////////

//�������
struct CMD_WeaveItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbPublicCard;						//������־
	WORD							wProvideUser;						//��Ӧ�û�
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				101									//��������
#define SUB_S_SEND_CARD				102									//�����˿�
#define SUB_S_LISTEN_CARD			103									//��������
#define SUB_S_OPERATE_NOTIFY		104									//������ʾ
#define SUB_S_OPERATE_RESULT		105									//��������
#define SUB_S_GAME_END				106									//��Ϸ����
#define SUB_S_TRUSTEE				107									//�û��й�
#define SUB_S_FORCE_SEND_CARD		108									//ǿ�ȷ���	
#define SUB_S_FORCE_OUT_CARD		109									//ǿ�ȳ���

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lCellScore;							//�������
	WORD							wBankerUser;						//ׯ���û�
	bool							bTrustee[GAME_PLAYER];						//�Ƿ��й�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	LONG							lCellScore;							//��Ԫ����
	WORD							wSiceCount;							//���ӵ���
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�

	//״̬����
	BYTE							cbActionCard;						//�����˿�
	BYTE							cbActionMask;						//��������
	BYTE							cbLeftCardCount;					//ʣ����Ŀ
	bool							bTrustee[GAME_PLAYER];						//�Ƿ��й�

	//������Ϣ
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
	BYTE							cbDiscardCount[4];					//������Ŀ
	BYTE							cbDiscardCard[4][65];				//������¼
	BYTE							cbUserWindCount[GAME_PLAYER];		//���Ƽ�¼
	BYTE							cbUserWindCardData[GAME_PLAYER][8]; //���Ƽ�¼

	//�˿�����
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�

	//����˿�
	BYTE							cbWeaveCount[4];					//�����Ŀ
	CMD_WeaveItem					WeaveItemArray[4][4];				//����˿�
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wSiceCount;							//���ӵ���
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�
	BYTE							cbUserAction;						//�û�����
	BYTE							cbSunWindCount;						//�ܻ���	
	BYTE							cbCardData[MAX_COUNT*GAME_PLAYER];	//�˿��б�
	BYTE							cbLeftCardCount;					//ʣ������
	bool							bFirst;								//�Ƿ��׷�
	BYTE							cbRepertoryCard[144];				//������
	bool							bTrustee[GAME_PLAYER];						//�Ƿ��й�
};

//��������
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
};

//�����˿�
struct CMD_S_SendCard
{
	BYTE							cbCardData;							//�˿�����
	BYTE							cbActionMask;						//��������
	WORD							wCurrentUser;						//��ǰ�û�
	bool							bGang;								//�Ƿ����
};

//��������
struct CMD_S_ListenCard
{
	WORD							wListenUser;						//�����û�
};

//������ʾ
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//��ԭ�û�
	BYTE							cbActionMask;						//��������
	BYTE							cbActionCard;						//�����˿�
};

//��������
struct CMD_S_OperateResult
{
	WORD							wOperateUser;						//�����û�
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG							lGameTax;							//��Ϸ˰��
	BYTE							cbProvideCard;						//�Ժ��˿�
	WORD							wProvideUser;						//�����û�
	BYTE							cbHaiDiCard;						//�����˿�
	LONG							lGameScore[4];						//��Ϸ����
	WORD							wChiHuKind[4];						//��������
	BYTE							cbCardCount[4];						//�˿���Ŀ
	BYTE							cbCardData[4][MAX_COUNT];			//�˿�����
	WORD							wChiHuRight[4];						//����Ȩλ
	WORD							wHuCount[4];						//���ƺ���
	BYTE							cbHua[4];							//���ƻ���	
};
//�û��й�
struct CMD_S_Trustee
{
	bool							bTrustee;							//�Ƿ��й�
	WORD							wChairID;							//�й��û�
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//��������
#define SUB_C_LISTEN_CARD			2									//��������
#define SUB_C_OPERATE_CARD			3									//�����˿�
#define SUB_C_FINISH				4									//ɸ�����
#define SUB_C_TRUSTEE				5									//�û��й�

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
//�û��й�
struct CMD_C_Trustee
{
	bool							bTrustee;							//�Ƿ��й�	
};
//////////////////////////////////////////////////////////////////////////

#endif
#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						481									//��Ϸ I D
#define GAME_PLAYER					4									//��Ϸ����
#define GAME_NAME					TEXT("��ˮ�齫��Ϸ")					//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH)	//��Ϸ����

//��Ϸ״̬
#define GS_MJ_FREE					GS_FREE								//����״̬
#define GS_MJ_PLAY					(GS_PLAYING+1)						//��Ϸ״̬
#define GS_MJ_HAI_DI				(GS_PLAYING+2)						//����״̬

//////////////////////////////////////////////////////////////////////////

//�������
struct CMD_WeaveItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����齫
	BYTE							cbPublicCard;						//������־
	WORD							wProvideUser;						//��Ӧ�û�
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				101									//��������
#define SUB_S_SEND_CARD				102									//�����齫
#define SUB_S_OPERATE_NOTIFY		103									//������ʾ
#define SUB_S_OPERATE_RESULT		104									//��������
#define SUB_S_GAME_END				106									//��Ϸ����
#define SUB_S_TING_CARD				107									//������Ϣ

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lCellScore;							//�������
	WORD							wBankerUser;						//ׯ���û�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	LONG							lCellScore;							//��Ԫ����
	WORD							wSiceCount;							//���ӵ���
	WORD							wSiceCount2;							//����2����
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�

	//״̬����
	BYTE							cbActionCard;						//�����齫
	BYTE							cbActionMask;						//��������
	BYTE							cbLeftCardCount;					//ʣ����Ŀ

	//������Ϣ
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����齫
	BYTE							cbDiscardCount[4];					//������Ŀ
	BYTE							cbDiscardCard[4][55];				//������¼

	//�齫����
	BYTE							cbCardCount;						//�齫��Ŀ
	BYTE							cbCardData[14];						//�齫�б�

	//����齫
	BYTE							cbWeaveCount[4];					//�����Ŀ
	CMD_WeaveItem					WeaveItemArray[4][4];				//����齫
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wSiceCount;							//���ӵ���
	WORD							wSiceCount2;							//����2����
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�
	BYTE							cbUserAction;						//�û�����
	BYTE							cbCardData[14];						//�齫�б�
};

//��������
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����齫
};

//�����齫
struct CMD_S_SendCard
{
	BYTE							cbCardData;							//�齫����
	BYTE							cbActionMask;						//��������
	WORD							wCurrentUser;						//��ǰ�û�
};

//������ʾ
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//��ԭ�û�
	BYTE							cbActionMask;						//��������
	BYTE							cbActionCard;						//�����齫
};

//��������
struct CMD_S_OperateResult
{
	WORD							wOperateUser;						//�����û�
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����齫
};

//��Ϸ����
struct CMD_S_GameEnd
{
	BYTE							cbChiHuCard;						//�Ժ��齫
	WORD							wProvideUser;						//�����û�
	LONG							lGameScore[4];						//��Ϸ����
	WORD							wChiHuKind[4];						//��������
	BYTE							cbCardCount[4];						//�齫��Ŀ
	BYTE							cbCardData[4][14];					//�齫����
};

//�û�����
struct CMD_S_GameTingCard
{
	WORD							wTingCardUser;						//�����û�
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//��������
#define SUB_C_OPERATE_CARD			2									//�����齫

//��������
struct CMD_C_OutCard
{
	BYTE							cbCardData;							//�齫����
};

//��������
struct CMD_C_OperateCard
{
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����齫
};

//////////////////////////////////////////////////////////////////////////

#endif
#ifndef CMD_PAOHUZI_HEAD_FILE
#define CMD_PAOHUZI_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						22									//��Ϸ I D
#define GAME_PLAYER					3									//��Ϸ����
#define GAME_NAME					TEXT("�ܺ�����Ϸ")					//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_GOLD)	//��Ϸ����

//////////////////////////////////////////////////////////////////////////

//��ֵ����
#define MAX_WEAVE					7									//������
#define MAX_INDEX					20									//�������
#define MAX_COUNT					21									//�����Ŀ
#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

//��������
#define ACK_NULL					0x00								//��
#define ACK_TI						0x01								//��
#define ACK_PAO						0x02								//��
#define ACK_WEI						0x04								//��
#define ACK_CHI						0x08								//��
#define ACK_CHI_EX					0x10								//��
#define ACK_PENG					0x20								//��
#define ACK_CHIHU					0x40								//��

//��������
#define CK_NULL						0x00								//��Ч����
#define CK_XXD						0x01								//СС���
#define CK_XDD						0x02								//С����
#define CK_EQS						0x04								//����ʮ��
#define CK_LEFT						0x10								//�������
#define CK_CENTER					0x20								//���ж���
#define CK_RIGHT					0x40								//���Ҷ���

//////////////////////////////////////////////////////////////////////////

//�������
struct tagWeaveItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbCardList[4];						//�˿��б�
};

//������Ϣ
struct tagChiCardInfo
{
	BYTE							cbChiKind;							//��������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbResultCount;						//�����Ŀ
	BYTE							cbCardData[3][3];					//�������
};

//��������
struct tagAnalyseItem
{
	BYTE							cbCardEye;							//�����˿�
	BYTE							cbHuXiCount;						//��Ϣ��Ŀ
	BYTE							cbWeaveCount;						//�����Ŀ
	tagWeaveItem					WeaveItemArray[MAX_WEAVE];			//����˿�
};

//������Ϣ
struct tagHuCardInfo
{
	BYTE							cbCardEye;							//�����˿�
	BYTE							cbHuXiCount;						//��Ϣ��Ŀ
	BYTE							cbWeaveCount;						//�����Ŀ
	tagWeaveItem					WeaveItemArray[MAX_WEAVE];			//����˿�
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_USER_TI_CARD			101									//�û�����
#define SUB_S_USER_PAO_CARD			102									//�û�����
#define SUB_S_USER_WEI_CARD			103									//�û�����
#define SUB_S_USER_PENG_CARD		104									//�û�����
#define SUB_S_USER_CHI_CARD			105									//�û�����
#define SUB_S_OPERATE_NOTIFY		106									//������ʾ
#define SUB_S_OUT_CARD_NOTIFY		107									//������ʾ
#define SUB_S_OUT_CARD				108									//�û�����
#define SUB_S_SEND_CARD				109									//��������
#define SUB_S_GAME_END				110									//��Ϸ����

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lCellScore;							//��������
	WORD							wBankerUser;						//ׯ���û�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ��Ϣ
	LONG							lCellScore;							//��������
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�

	//������Ϣ
	BYTE							bOutCard;							//���Ʊ�־
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�

	//�˿���Ϣ
	BYTE							bLeftCardCount;						//ʣ����Ŀ
	BYTE							cbCardIndex[MAX_INDEX];				//�û��˿�
	BYTE							bUserCardCount[GAME_PLAYER];		//�˿���Ŀ

	//�����Ϣ
	BYTE							bWeaveItemCount[GAME_PLAYER];		//�����Ŀ
	tagWeaveItem					WeaveItemArray[GAME_PLAYER][7];		//����˿�

	//������Ϣ
	BYTE							bResponse;							//��Ӧ��־
	BYTE							bUserAction;						//�û�����
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�
	BYTE							cbCardData[21];						//�˿��б�
};

//��������
struct CMD_S_UserTiCard
{
	WORD							wActionUser;						//�����û�
	BYTE							cbActionCard;						//�����˿�
	BYTE							cbRemoveCount;						//ɾ����Ŀ
};

//��������
struct CMD_S_UserPaoCard
{
	WORD							wActionUser;						//�����û�
	BYTE							cbActionCard;						//�����˿�
	BYTE							cbRemoveCount;						//ɾ����Ŀ
};

//��������
struct CMD_S_UserWeiCard
{
	WORD							wActionUser;						//�����û�
	BYTE							cbActionCard;						//�����˿�
};

//��������
struct CMD_S_UserPengCard
{
	WORD							wActionUser;						//�����û�
	BYTE							cbActionCard;						//�����˿�
};

//��������
struct CMD_S_UserChiCard
{
	WORD							wActionUser;						//�����û�
	BYTE							cbActionCard;						//�����˿�
	BYTE							cbResultCount;						//�����Ŀ
	BYTE							cbCardData[3][3];					//�������
};

//������ʾ
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//��ԭ�û�
	BYTE							cbActionCard;						//�����˿�
	BYTE							cbOperateCode;						//��������
};

//������ʾ
struct CMD_S_OutCardNotify
{
	BYTE							bOutCard;							//���Ʊ�־
	WORD							wCurrentUser;						//��ǰ�û�
};

//�û�����
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
};

//��������
struct CMD_S_SendCard
{
	BYTE							cbCardData;							//�����˿�
	WORD							wAttachUser;						//���û�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//������Ϣ
	BYTE							cbReason;							//����ԭ��
	BYTE							cbHuCard;							//�����˿�
	WORD							wWinUser;							//ʤ���û�
	WORD							wProvideUser;						//�����û�

	//�ɼ�����
	LONG							lGameTax;							//��Ϸ˰��
	LONG							lGameScore[3];						//��Ϸ����

	//�˿˱���
	BYTE							bCardCount[3];						//�˿���Ŀ
	BYTE							bCardData[63];						//�˿��б� 
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//��������
#define SUB_C_OPERATE_CARD			2									//�����˿�
#define SUB_C_CONTINUE_CARD			3									//��������

//��������
struct CMD_C_OutCard
{
	BYTE							cbCardData;							//�˿�����
};

//��������
struct CMD_C_OperateCard
{
	BYTE							cbChiKind;							//��������
	BYTE							cbOperateCode;						//��������
};

//////////////////////////////////////////////////////////////////////////

#endif
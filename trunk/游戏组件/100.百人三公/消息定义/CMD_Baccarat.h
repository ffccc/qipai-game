#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						31									//��Ϸ I D
#define GAME_PLAYER					100									//��Ϸ����
#define GAME_NAME					TEXT("��������")						//��Ϸ����

//״̬����
#define	GS_PLACE_JETTON				GS_PLAYING							//��ע״̬
#define	GS_GAME_END					GS_PLAYING+1						//����״̬

//�������
//#define ID_XIAN_JIA					1									//�м�����
//#define ID_PING_JIA					2									//ƽ������
//#define ID_ZHUANG_JIA				3									//ׯ������
//#define ID_XIAN_TIAN_WANG			4									//������
//#define ID_ZHUANG_TIAN_WANG			5									//ׯ����
//#define ID_TONG_DIAN_PING			6									//ͬ��ƽ
//#define ID_PLAYER_TWO_PAIR			7									//�ж���
//#define ID_BANKER_TWO_PAIR			8									//ׯ����

#define ID_TIAN_MARK				0x01
#define ID_DI_MARK					0x02
#define ID_XUAN_MARK				0x04
#define ID_HUANG_MARK				0x08

//���ʶ���
//#define RATE_TWO_PAIR				12									//��������

//��¼��Ϣ
struct tagServerGameRecord
{
	BYTE							cbWinner;						
//	BYTE							cbMePlaceRegion;
	//bool							bPlayerTwoPair;						//���ӱ�ʶ
	//bool							bBankerTwoPair;						//���ӱ�ʶ
	//BYTE							cbPlayerCount;						//�мҵ���
	//BYTE							cbBankerCount;						//ׯ�ҵ���
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_FREE				99									//��Ϸ����
#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_PLACE_JETTON			101									//�û���ע
#define SUB_S_GAME_END				102									//��Ϸ����
#define SUB_S_APPLY_BANKER			103									//����ׯ��
#define SUB_S_CHANGE_BANKER			104									//�л�ׯ��
#define SUB_S_CHANGE_USER_SCORE		105									//���»���
#define SUB_S_SEND_RECORD			106									//��Ϸ��¼
#define SUB_S_PLACE_JETTON_FAIL		107									//��עʧ��
#define SUB_S_CANCEL_BANKER			108									//ȡ������


//ʧ�ܽṹ
struct CMD_S_PlaceJettonFail
{
	BYTE							lJettonArea;						//��ע����
	LONG							lPlaceScore;						//��ǰ��ע
};

//���»���
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							//���Ӻ���
	DOUBLE							lScore;								//��һ���

	//ׯ����Ϣ
	WORD							wCurrentBankerChairID;				//��ǰׯ��
	BYTE							cbBankerTime;						//ׯ�Ҿ���
	DOUBLE							lCurrentBankerScore;				//ׯ�ҷ���
};

//����ׯ��
struct CMD_S_ApplyBanker
{
	WORD							wApplyUser;							//�������
};

//ȡ������
struct CMD_S_CancelBanker
{
	WORD							wCancelUser;						//ȡ�����
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//��ׯ���
	LONG							lBankerScore;						//ׯ������
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�����Ϣ
	LONG							lUserMaxScore;							//�������

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	LONG							lBankerWinScore;					//ׯ�ҳɼ�
	LONG							lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONG							lApplyBankerCondition;				//��������
	LONG							lAreaLimitScore;					//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ȫ����ע
	//LONG							lAllTieScore;						//��ƽ��ע
	//LONG							lAllBankerScore;					//��ׯ��ע
	//LONG							lAllPlayerScore;					//������ע
	//LONG							lAllTieSamePointScore;				//ƽ����ע
	//LONG							lAllBankerKingScore;				//ׯ����ע
	//LONG							lAllPlayerKingScore;				//������ע
	//LONG							lAllPlayerTwoPair;					//�мҶ���
	//LONG							lAllBankerTwoPair;					//ׯ�Ҷ���
	LONG							lAllTianScore;							//��
	LONG							lAllDiScore;							//��
	LONG							lAllXuanScore;							//��
	LONG							lAllHuangScore;							//��


	////�����ע
	//LONG							lUserTieScore;						//��ƽ��ע
	//LONG							lUserBankerScore;					//��ׯ��ע
	//LONG							lUserPlayerScore;					//������ע
	//LONG							lUserTieSamePointScore;				//ƽ����ע
	//LONG							lUserBankerKingScore;				//ׯ����ע
	//LONG							lUserPlayerKingScore;				//������ע
	//LONG							lUserPlayerTwoPair;					//�мҶ���
	//LONG							lUserBankerTwoPair;					//ׯ�Ҷ���
	LONG							lUserTianScore;							//��
	LONG							lUserDiScore;								//��
	LONG							lUserXuanScore;							//��
	LONG							lUserHuangScore;							//��

	//��һ���
	LONG							lUserMaxScore;						//�����ע							

	//������Ϣ
	LONG							lApplyBankerCondition;				//��������
	LONG							lAreaLimitScore;					//��������

	//�˿���Ϣ
// 	BYTE							cbCardCount[2];						//�˿���Ŀ
	BYTE							cbTableCardArray[6][3];				//�����˿�

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	LONG							lBankerWinScore;					//ׯ��Ӯ��
	LONG							lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONG							lEndBankerScore;					//ׯ�ҳɼ�
	LONG							lEndUserScore;						//��ҳɼ�
	LONG							lEndUserReturnScore;				//���ػ���
	LONG							lEndRevenue;						//��Ϸ˰��

	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	BYTE							cbGameStatus;						//��Ϸ״̬
};

//��Ϸ����
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ��λ��
	LONG							lBankerScore;						//ׯ������
	LONG							lUserMaxScore;						//�ҵ�����
	BYTE							cbTimeLeave;						//ʣ��ʱ��
};

//�û���ע
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//�û�λ��
	BYTE							cbJettonArea;						//��������
	LONG							lJettonScore;						//��ע��Ŀ
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//�¾���Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�˿���Ϣ
//	BYTE							cbCardCount[2];						//�˿���Ŀ
	BYTE							cbTableCardArray[6][3];				//�����˿�
 
	//ׯ����Ϣ
	LONG							lBankerScore;						//ׯ�ҳɼ�
	LONG							lBankerTotallScore;					//ׯ�ҳɼ�
	INT								nBankerTime;						//��ׯ����

	//��ҳɼ�
	LONG							lUserScore;							//��ҳɼ�
	LONG							lUserReturnScore;					//���ػ���

	//ȫ����Ϣ
	LONG							lRevenue;							//��Ϸ˰��
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��
#define SUB_C_CANCEL_BANKER			3									//ȡ������

//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//��������
	LONG							lJettonScore;						//��ע��Ŀ
};

//////////////////////////////////////////////////////////////////////////

#endif

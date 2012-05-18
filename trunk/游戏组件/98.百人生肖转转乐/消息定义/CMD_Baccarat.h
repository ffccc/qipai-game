#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#ifdef _DEBUG
	#define KIND_ID						27								    //��Ϸ I D
#else
	#define KIND_ID						28								    //��Ϸ I D
#endif

#define GAME_PLAYER					100									//��Ϸ����
#define GAME_NAME					TEXT("��Фתת��")					//��Ϸ����


//��Ϸ״̬
#define GS_BACCARAT_FREE            GS_FREE								//����
#define GS_BACCARAT_JETTON          GS_PLAYING							//��ע
#define GS_BACCARAT_OPENCARD        GS_PLAYING+1						//����


//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START            100                                 //��ʼ��ע
#define SUB_S_OPEN_CARD			    101									//��Ϸ��ʼ
#define SUB_S_PLACE_JETTON			102									//�û���ע
#define SUB_S_GAME_END				103									//��Ϸ����
#define SUB_S_APPLY_BANKER			104									//����ׯ��
#define SUB_S_CHANGE_BANKER			105									//�л�ׯ��
#define SUB_S_CHANGE_USER_SCORE		106									//���»���
#define SUB_S_CURRENT_BANKER		200
#define SUB_S_SEND_RECORD			107									//��Ϸ��¼
#define SUB_S_PLACE_JETTON_FAIL		108									//��עʧ��
#define SUB_S_GAME_SCORE			109									//���ͻ���
#define SUB_S_FULL					110									//��ע����
#define SUB_S_RESLUT				111									//���
#define SUB_S_FREE					112


#define	MAX_AREA					14
#define MAX_HISTORY_LENTH			20
#define MAX_HISTORY_DISPLAY			9


//��עʱ��
#ifdef _DEBUG
#define TIME_PLACE_JETTON			18 									//��עʱ��
#else
#define TIME_PLACE_JETTON			18									//��עʱ��
#endif

//����ʱ��
#ifdef _DEBUG
#define TIME_OPEN_CARD				22									//����ʱ��
#else
#define TIME_OPEN_CARD				22									//����ʱ��
#endif

//����ʱ��
#ifdef _DEBUG
#define TIME_GAME_FREE              6                                   //����ʱ��
#else
#define TIME_GAME_FREE              6                                   //����ʱ��
#endif


//ʧ�ܽṹ
struct CMD_S_PlaceJettonFail
{
	LONGLONG							lJettonArea;						//��ע����
	LONGLONG							lPlaceScore;						//��ǰ��ע
	LONGLONG							lMaxLimitScore;						//���ƴ�С
	LONGLONG							lFinishPlaceScore;					//����ע��
};

//���»���
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							//���Ӻ���
	LONGLONG						lScore;								//��һ���

	//ׯ����Ϣ
	WORD							wCurrentBankerChairID;				//��ǰׯ��
	BYTE							cbBankerTime;						//ׯ�Ҿ���
	LONGLONG						lCurrentBankerScore;				//ׯ�ҷ���
};

struct CMD_S_UserScore
{
	INT64							nScore;
	INT64							nBankerScore;
};

//����ׯ��
struct CMD_S_ApplyBanker
{
	CHAR							szAccount[32];					    //�������
	LONGLONG						lScore;								//��ҽ��
	bool							bApplyBanker;						//�����ʶ
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	WORD							wChairID;							//���Ӻ���
	BYTE							cbBankerTime;						//ׯ�Ҿ���
	LONGLONG						lBankerScore;						//ׯ�ҷ���
	LONGLONG						lBankerTreasure;					//ׯ�ҽ��

	//�ҵ���ע
	LONGLONG						lAreaLimitScore;					//��������
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//ׯ����Ϣ
	WORD							wCurrentBankerChairID;				//��ǰׯ��
	BYTE							cbBankerTime;						//ׯ�Ҿ���
	LONGLONG							lCurrentBankerScore;				//ׯ�ҷ���
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������
	LONGLONG						lBankerTreasure;					//ׯ�ҽ��
	LONGLONG                        lMeMaxScore;
	INT64							lBankerWin;
};

//��Ϸ״̬
struct CMD_S_StatusJetton
{
	//ȫ����Ϣ
	BYTE								cbTimeLeave;						//ʣ��ʱ��

	WORD								wCurrentBankerChairID;				//��ǰׯ��
	BYTE								cbBankerTime;						//ׯ�Ҿ���
	LONGLONG							lCurrentBankerScore;				//ׯ�ҷ���
	LONGLONG							lApplyBankerCondition;				//��������
	LONGLONG							lAreaLimitScore;					//��������
	LONGLONG							lBankerTreasure;					//ׯ�ҽ��
	INT64								lBankerWin;
};

struct CMD_S_StatusOpenCard
{
	BYTE		cbAnimalPos;
};

//��Ϸ��ʼ
struct CMD_S_OpenCard
{

};

//��ʼ��ע
struct CMD_S_GameStart
{
	BYTE                            cbBankerTimes;                        //ʣ��ʱ��
	WORD							wBanker;
};

//�û���ע
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//�û�λ��
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//�¾���Ϣ
	LONGLONG						lMeMaxScore;						//�����ע
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�ɼ���¼
	BYTE							cbWinner;							//ʤ�����
	BYTE							cbKingWinner;						//����ʤ��
	BYTE                            cbPlayerPair;                       //�мҶ���
	BYTE                            cbBankerPair;                       //ׯ�Ҷ���
	LONGLONG						lBankerTreasure;					//ׯ�ҽ��

	LONGLONG						lBankerTotalScore;					//ׯ�ҳɼ�
	LONGLONG						lBankerScore;						//ׯ�ҳɼ�
	INT								nBankerTime;						//��ׯ����
};

//��Ϸ�÷�
struct CMD_S_GameScore
{
	//�ɼ���¼
	BYTE							cbWinner;							//ʤ�����
	BYTE							cbKingWinner;						//����ʤ��
	BYTE                            cbPlayerPair;                       //�мҶ���
	BYTE                            cbBankerPair;                       //ׯ�Ҷ���
	LONGLONG						lMeGameScore;						//�ҵĳɼ�
	LONGLONG						lMeReturnScore;						//����ע��
	LONGLONG						lBankerScore;						//ׯ�ҳɼ�

};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��

//�û���ע
struct CMD_C_PlaceJetton
{	
	BYTE							cbViewJettonArea;					//�м�����
	BYTE							cbAnimal;							//0-13,12-13��С
	LONGLONG						lJettonScore;						//��ע��Ŀ
};

//����ׯ��
struct CMD_C_ApplyBanker
{
	bool							bApplyBanker;						//�����ʶ
};

//////////////////////////////////////////////////////////////////////////

#endif
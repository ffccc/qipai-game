#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE
#define RETYPELONG                        __int64
//////////////////////////////////////////////////////////////////////////
//�����궨��
#define UR_GAME_CONTROL             17907712                            //����ԱȨ�� 

#define KIND_ID						14									//��Ϸ I D
#define GAME_PLAYER					100									//��Ϸ����
#define GAME_NAME					TEXT("��˫")						//��Ϸ����

//�������
#define ID_ODD						1									//�µ�����
#define ID_EVEN						2									//��˫����

#define GS_WK_FREE					GS_FREE								//�ȴ���ʼ
#define GS_WK_GAME					GS_PLAYING							//��Ϸ״̬
#define GS_WK_JETTON				GS_PLAYING+2		
#define GS_WK_END					GS_PLAYING+3						//����״̬

//��¼��Ϣ
struct tagServerGameRecord
{
	WORD							wWinner;							//ʤ�����
	BYTE							cbDicePoint;						//���ӵ���
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_PLACE_JETTON			101									//�û���ע
#define SUB_S_GAME_END				102									//��Ϸ����
#define SUB_S_APPLY_BANKER			103									//����ׯ��
#define SUB_S_CHANGE_BANKER			104									//�л�ׯ��
#define SUB_S_CHANGE_USER_SCORE		105									//���»���
#define SUB_S_SEND_RECORD			106									//��Ϸ��¼
#define SUB_S_PLACE_JETTON_FAIL		107									//��עʧ��
#define SUB_S_GAME_SCORE			108									//���ͻ���
#define SUB_S_END_PLACE_JETTON		109									//������ע

//ʧ�ܽṹ
struct CMD_S_PlaceJettonFail
{
	RETYPELONG							lJettonArea;						//��ע����
	RETYPELONG							lPlaceScore;						//��ǰ��ע
	RETYPELONG							lMaxLimitScore;						//���ƴ�С
	RETYPELONG							lFinishPlaceScore;					//����ע��
	WORD								wPlaceUser;
};

//���»���
struct CMD_S_ChangeUserScore
{
	WORD								wChairID;							//���Ӻ���
	RETYPELONG							lScore;								//��һ���

	//ׯ����Ϣ
	WORD								wCurrentBankerChairID;				//��ǰׯ��
	BYTE								cbBankerTime;						//ׯ�Ҿ���
	RETYPELONG							lCurrentBankerScore;				//ׯ�ҷ���
};

//����ׯ��
struct CMD_S_ApplyBanker
{
	CHAR								szAccount[ 32 ];					//�������
	RETYPELONG							lScore;								//��ҽ��
	bool								bApplyBanker;						//�����ʶ
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	WORD								wChairID;							//���Ӻ���
	BYTE								cbBankerTime;						//ׯ�Ҿ���
	RETYPELONG							lBankerScore;						//ׯ�ҷ���
	RETYPELONG							lBankerTreasure;					//ׯ�ҽ��
	//�ҵ���ע
	RETYPELONG							lAreaLimitScore;					//��������
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE								cbTimeLeave;						//ʣ��ʱ��
	//��ע��Ϣ
	RETYPELONG							lOddcore;							//����ע
	RETYPELONG							lEvenScore;							//��˫��ע

	//�ҵ���ע
	RETYPELONG							lMeMaxScore;						//�����ע
	RETYPELONG							lMeOddScore;						//����ע
	RETYPELONG							lMeEvenScore;						//��˫��ע

	//ׯ����Ϣ
	WORD								wCurrentBankerChairID;				//��ǰׯ��
	BYTE								cbBankerTime;						//ׯ�Ҿ���
	RETYPELONG							lCurrentBankerScore;				//ׯ�ҷ���
	RETYPELONG							lApplyBankerCondition;				//��������
	RETYPELONG							lAreaLimitScore;					//��������
	RETYPELONG							lUserLimitScore;
	RETYPELONG							lBankerTreasure;					//ׯ�ҽ��
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��ע��Ϣ
	RETYPELONG							lOddcore;							//����ע
	RETYPELONG							lEvenScore;							//��˫��ע

	//�ҵ���ע
	RETYPELONG							lMeMaxScore;						//�����ע
	RETYPELONG							lMeOddScore;						//����ע
	RETYPELONG							lMeEvenScore;						//��˫��ע
	//ɫ����Ϣ
 	BYTE								cbDice[2];							//ɫ�ӵ���
	//ׯ����Ϣ
	WORD								wCurrentBankerChairID;				//��ǰׯ��
	BYTE								cbBankerTime;						//ׯ�Ҿ���
	RETYPELONG							lCurrentBankerScore;				//ׯ�ҷ���
	RETYPELONG							lApplyBankerCondition;				//��������
	RETYPELONG							lAreaLimitScore;					//��������
	RETYPELONG							lUserLimitScore;
	RETYPELONG							lBankerTreasure;					//ׯ�ҽ��
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
 	BYTE								cbGameTime;							//��Ϸʱ��
	WORD								wCurBankerID;
	WORD								wServerID;
};

//�û���ע
struct CMD_S_PlaceJetton
{
	WORD								wChairID;							//�û�λ��
	BYTE								cbJettonArea;						//��������
	RETYPELONG							lJettonScore;						//��ע��Ŀ
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//�¾���Ϣ
	RETYPELONG							lMeMaxScore;						//�����ע
	BYTE								cbTimeLeave;						//ʣ��ʱ��

	//�ɼ���¼
	BYTE								cbWinner;							//ʤ�����
	RETYPELONG							lBankerTreasure;					//ׯ�ҽ��

	RETYPELONG							lBankerTotalScore;					//ׯ�ҳɼ�
	RETYPELONG							lBankerScore;						//ׯ�ҳɼ�
	INT									nBankerTime;						//��ׯ����
};

//��Ϸ�÷�
struct CMD_S_GameScore
{
	//�ɼ���¼
	BYTE								cbWinner;							//ʤ�����
	RETYPELONG							lMeGameScore;						//�ҵĳɼ�
	RETYPELONG							lMeReturnScore;						//����ע��
	RETYPELONG							lBankerScore;						//ׯ�ҳɼ�

	//��ע��Ϣ
	RETYPELONG							lDrawOddScore;						//����ע
	RETYPELONG							lDrawEvenScore;						//��˫��ע

	//�ҵ���ע
	RETYPELONG							lMeOddScore;						//����ע
	RETYPELONG							lMeEvenScore;						//��˫��ע
};

//������ע
struct CMD_S_EndPlaceJetton
{
 	BYTE								cbGameTime;							//��Ϸʱ��
	BYTE								cbDice[2];							//ɫ������
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��
#define SUB_C_MANAGE_CONTROL		5									//ȡ������

//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE								cbJettonArea;						//��������
	RETYPELONG							lJettonScore;						//��ע��Ŀ
};

//����ׯ��
struct CMD_C_ApplyBanker
{
	bool								bApplyBanker;						//�����ʶ
};

//�û���ע
struct CMD_C_ManageControl
{
	int									nControlArea;							//�������
	int									nControlBanker;							//�������
	BOOL								bControlBanker;
	BOOL								bControlArea;
	TCHAR								m_UserName[256];
	BOOL								bUserWin;
};
//////////////////////////////////////////////////////////////////////////

#endif

// CMD_Game.h  ��Ϣ����

#pragma once

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						214								// ��Ϸ I D
#define GAME_PLAYER					100									// ��Ϸ����
#define GAME_NAME					TEXT("����")				        // ��Ϸ����

//״̬����
#define	GS_PLACE_JETTON				GS_PLAYING							//��ע״̬
#define	GS_GAME_END					GS_PLAYING+1						//����״̬

//�������
#define COUNT_AZIMUTH               52                                  // 52������
#define MAX_MULTIPLE                8                                   // �����

#define USER_WINNER                 0x01                                // Ӯ
#define USER_LOSER                  0xFF                                // ��
#define MAX_ANDROID_BANKER          3                                   // �����л����˵ĸ���

//////////////////////////////////////////////////////////////////////////
//��ʷ��¼
#define MAX_SCORE_HISTORY			30									//��ʷ����
#define SHOW_SCORE_HISTORY          16                                  // ��ʾ��¼��

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

#define MAX_COUNT_SICBO             0x03            // һ��3������
#define MAX_SICBO_NUMBER            0x06            // ���6��

// ����
enum E_SICBO_NUMBER
{
	enSicboNum_One=(int)1,          // ���ӵ���1
	enSicboNum_Two,                 // ���ӵ���2
	enSicboNum_Three,               // ���ӵ���3
	enSicboNum_Four,                // ���ӵ���4
	enSicboNum_Five,                // ���ӵ���5
	enSicboNum_Six                  // ���ӵ���6
};

// ����
enum E_CARD_TYPE
{
	enCardType_Illegal=(int)-1,        // �Ƿ�
	enCardType_Big,                    // ��
	enCardType_Small,                  // С
	enCardType_Single,                 // ��
	enCardType_Double,                 // ˫		

	enCardType_NumberFour=(int)4,      // 3�����ӵ�����Ϊ4
	enCardType_NumberFive,             // 3�����ӵ�����Ϊ5
	enCardType_NumberSix,              // 3�����ӵ�����Ϊ6
	enCardType_NumberSeven,            // 3�����ӵ�����Ϊ7
	enCardType_NumberEight,            // 3�����ӵ�����Ϊ8
	enCardType_NumberNine,             // 3�����ӵ�����Ϊ9
	enCardType_NumberTen,              // 3�����ӵ�����Ϊ10
	enCardType_NumberEleven,           // 3�����ӵ�����Ϊ11
	enCardType_NumberTwelve,           // 3�����ӵ�����Ϊ12
	enCardType_NumberThirteen,         // 3�����ӵ�����Ϊ13
	enCardType_NumberFourteen,         // 3�����ӵ�����Ϊ14
	enCardType_NumberFifteen,          // 3�����ӵ�����Ϊ15
	enCardType_NumberSixteen,          // 3�����ӵ�����Ϊ16
	enCardType_NumberSeventeen,        // 3�����ӵ�����Ϊ17

	enCardType_SicboOne,               // ����1
	enCardType_SicboTwo,               // ����2
	enCardType_SicboThree,             // ����3
	enCardType_SicboFour,              // ����4
	enCardType_SicboFive,              // ����5
	enCardType_SicboSix,               // ����6

	enCardType_SicboOneAndTwo,         // ����1,2
	enCardType_SicboOneAndThree,       // ����1,3
	enCardType_SicboOneAndFour,        // ����1,4
	enCardType_SicboOneAndFive,        // ����1,5
	enCardType_SicboOneAndSix,         // ����1,6
	enCardType_SicboTwoAndThree,       // ����2,3
	enCardType_SicboTwoAndFour,        // ����2,4
	enCardType_SicboTwoAndFive,        // ����2,5
	enCardType_SicboTwoAndSix,         // ����2,6
	enCardType_SicboThreeAndFour,      // ����3,4
	enCardType_SicboThreeAndFive,      // ����3,5
	enCardType_SicboThreeAndSix,       // ����3,6
	enCardType_SicboFourAndFive,       // ����4,5
	enCardType_SicboFourAndSix,        // ����4,6
	enCardType_SicboFiveAndSix,        // ����5,6

	enCardType_SicboDoubleOne,         // ����2��1
	enCardType_SicboDoubleTwo,         // ����2��2
	enCardType_SicboDoubleThree,       // ����2��3
	enCardType_SicboDoubleFour,        // ����2��4
	enCardType_SicboDoubleFive,        // ����2��5
	enCardType_SicboDoubleSix,         // ����2��6

	enCardType_SicboThreeOne,          // ����3��1
	enCardType_SicboThreeTwo,          // ����3��2
	enCardType_SicboThreeThree,        // ����3��3
	enCardType_SicboThreeFour,         // ����3��4
	enCardType_SicboThreeFive,         // ����3��5
	enCardType_SicboThreeSix,          // ����3��6

	enCardType_SicboThreeSame          // ����3����ͬ����
};


// ��¼��Ϣ
struct tagServerGameRecord
{
	// ������Ϸֻ��¼���ӵ���
	E_SICBO_NUMBER enCards[MAX_COUNT_SICBO];
};

//ʧ�ܽṹ
struct CMD_S_PlaceJettonFail
{
	BYTE							lJettonArea;						//��ע����
	__int64							lPlaceScore;						//��ǰ��ע
};

// ���»���
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							// ���Ӻ���
	__int64							lScore;								// ��һ���

	//ׯ����Ϣ
	WORD							wCurrentBankerChairID;				// ��ǰׯ��
	BYTE							cbBankerTime;						// ׯ�Ҿ���
	__int64							lCurrentBankerScore;				// ׯ�ҷ���
};

//����ׯ��
struct CMD_S_ApplyBanker
{
	DWORD                           dwUserID;                           // ���ID
	WORD							wApplyUser;							// �������
};

//ȡ������
struct CMD_S_CancelBanker
{
	DWORD                           dwUserID;                           // ���ID
	TCHAR							szCancelUser[32];					//ȡ�����
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	DWORD                           dwUserID;                           // ���ID
	WORD							wBankerUser;						//��ׯ���
	__int64							lBankerScore;						//ׯ�ҽ��
};

// ��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�����Ϣ
	__int64							lUserMaxScore;						//��ҽ��

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	__int64							lBankerWinScore;					//ׯ�ҳɼ�
	__int64							lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	__int64							lApplyBankerCondition;				//��������
	__int64							lAreaLimitScore;					//��������
};

// ��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ȫ����ע
	__int64							lAllScore[COUNT_AZIMUTH];			// ��,��������ע

	//�����ע
	__int64							lUserScore[COUNT_AZIMUTH];			// �����,��������ע

	//��һ���
	__int64							lUserMaxScore;						// �����ע
	__int64							lAllMaxScore;						// �����ע

	//������Ϣ
	__int64							lApplyBankerCondition;				//��������
	__int64							lAreaLimitScore;					//��������

	//�˿���Ϣ
 	E_SICBO_NUMBER					enCards[MAX_COUNT_SICBO];		    // ���ӵ���
	bool                            bWinner[COUNT_AZIMUTH];
	BYTE							bySicboIndex;				        // ��ǰҡ�����
	BYTE                            bySicboType;                        // ҡ��������

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	__int64							lBankerWinScore;					//ׯ��Ӯ��
	__int64							lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	__int64							lEndBankerScore;					//ׯ�ҳɼ�
	__int64							lEndUserScore;						//��ҳɼ�
	__int64							lEndUserReturnScore;				//���ػ���
	__int64							lEndRevenue;						//��Ϸ˰��

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
	__int64							lBankerScore;						//ׯ�ҽ��
	__int64							lUserMaxScore;						//�ҵĽ��
	__int64							lAllMaxScore;						//���������ע���
	BYTE							cbTimeLeave;						//ʣ��ʱ��
};

//�û���ע
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//�û�λ��
	BYTE							cbJettonArea;						//��������
	__int64							lJettonScore;						//��ע��Ŀ
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//�¾���Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�˿���Ϣ
	E_SICBO_NUMBER					enCards[MAX_COUNT_SICBO];		    // ���ӵ���
	bool                            bWinner[COUNT_AZIMUTH];              // ��Ӯ���

	//ׯ����Ϣ
	__int64							lBankerScore;						//ׯ�ҳɼ�
	__int64							lBankerTotallScore;					//ׯ�ҳɼ�
	INT								nBankerTime;						//��ׯ����

	//��ҳɼ�
	__int64							lUserScore;							//��ҳɼ�
	__int64							lUserReturnScore;					//���ػ���

	//ȫ����Ϣ
	__int64							lRevenue;							//��Ϸ˰��
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��
#define SUB_C_CANCEL_BANKER			3									//ȡ������
#define SUB_C_CHANGE_SICBO_TYPE     4                                   // ׯ��ѡ��ҡ���ӷ�ʽ
#define SUB_C_USE_WINNER            5                                   // ����ʤ��

#define  SICBOTYPE_COUNT            5   // ҡ������������
enum E_SICBO_TYPE
{
	enSicboType_Normal=0x00,             // ��ͨ��ҡ����
	enSicboType_Desktop,                 // ����ҡ����
	enSicboType_InMidair,                // ���ҡ����
	enSicboType_InTheAir,                // ����ҡ����
	enSicboType_Sicbo                    // ������Ϸ��ҡ���ӷ�ʽ
};

//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//��������
	__int64							lJettonScore;						//��ע��Ŀ
};

struct CMD_C_ChangeSicboType
{
	E_SICBO_TYPE enSicboType;
};

struct CMD_C_UseWinner
{
	BYTE              byArea;  // ����
	BYTE              byType;  // ����
};

//////////////////////////////////////////////////////////////////////////
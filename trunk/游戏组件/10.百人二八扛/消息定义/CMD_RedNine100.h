#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						10001								//��Ϸ I D
#define GAME_PLAYER					100									//��Ϸ����
#define GAME_NAME					TEXT("���˶��˿�")					//��Ϸ����

//״̬����
#define	GS_PLACE_JETTON				GS_PLAYING							//��ע״̬
#define	GS_GAME_END					GS_PLAYING + 1						//����״̬
#define	GS_MOVECARD_END				GS_PLAYING + 2						//����״̬

//��������
#define ID_TIAN_MEN					1									//˳��
#define ID_DI_MEN					2									//����
#define ID_XUAN_MEN					3									//����

//�������
#define BANKER_INDEX				0									//ׯ������
#define SHUN_MEN_INDEX				1									//˳������
#define DUI_MEN_INDEX				2									//��������
#define DAO_MEN_INDEX				3									//��������

#define AREA_COUNT					3									//������Ŀ

#define CARD_LOCATION_COUNT			4									//��λ����
#define CARD_COUNT_OF_ONE			2									//һλ����


struct CMD_S_AndroidArea
{
	BYTE bWinArea[AREA_COUNT];
	CMD_S_AndroidArea()
	{
		memset(this, 0, sizeof(*this));
	}
};

//���ʶ���
#define RATE_TWO_PAIR				1									//��������

//��¼��Ϣ
struct tagServerGameRecord
{
	bool							bWinShunMen;						//˳��ʤ��
	bool							bWinDuiMen;							//����ʤ��
	bool							bWinDaoMen;							//����ʤ��
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
#define SUB_S_ANDROA_AREA			109
#define SUB_S_WIND_RATE_INFO		110									//Ӯ����Ϣ


//ʧ�ܽṹ
struct CMD_S_PlaceJettonFail
{
	WORD								wPlaceUser;							//��ע���
	BYTE								lJettonArea;						//��ע����
	LONGLONG							lPlaceScore;						//��ǰ��ע
};

//���»���
struct CMD_S_ChangeUserScore
{
	WORD								wChairID;							//���Ӻ���
	LONGLONG							lScore;								//��һ���

	//ׯ����Ϣ
	WORD								wCurrentBankerChairID;				//��ǰׯ��
	BYTE								cbBankerTime;						//ׯ�Ҿ���
	LONGLONG							lCurrentBankerScore;				//ׯ�ҷ���
};

//����ׯ��
struct CMD_S_ApplyBanker
{
	WORD							wApplyUser;							//�������
};

//ȡ������
struct CMD_S_CancelBanker
{
	TCHAR							szCancelUser[32];					//ȡ�����
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//��ׯ���
	LONGLONG						lBankerScore;						//ׯ�ҽ��
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�����Ϣ
	LONGLONG						lUserMaxScore;						//��ҽ��

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	LONGLONG						lBankerWinScore;					//ׯ�ҳɼ�
	LONGLONG						lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ȫ����ע
	LONGLONG						lAllJettonScore[AREA_COUNT+1];		//ȫ����ע

	//�����ע
	LONGLONG						lUserJettonScore[AREA_COUNT+1];		//������ע

	//��һ���
	LONGLONG						lUserMaxScore;						//�����ע

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������

	//�˿���Ϣ
	BYTE							cbTableCardArray[CARD_LOCATION_COUNT][CARD_COUNT_OF_ONE];		//�����˿�

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	LONGLONG						lBankerWinScore;					//ׯ��Ӯ��
	LONGLONG						lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONGLONG						lEndBankerScore;					//ׯ�ҳɼ�
	LONGLONG						lEndUserScore;						//��ҳɼ�
	LONGLONG						lEndUserReturnScore;				//���ػ���
	LONGLONG						lEndRevenue;						//��Ϸ˰��

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
	LONGLONG						lBankerScore;						//ׯ�ҽ��
	LONGLONG						lUserMaxScore;						//�ҵĽ��
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	bool							bContiueCard;						//��������
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
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	BYTE							bcFirstCard[2];						//���ӵ���
	//�˿���Ϣ
	BYTE							cbTableCardArray[CARD_LOCATION_COUNT][CARD_COUNT_OF_ONE];				//�����˿�
	BYTE							cbLeftCardCount;					//�˿���Ŀ

	//ׯ����Ϣ
	LONGLONG						lBankerScore;						//ׯ�ҳɼ�
	LONGLONG						lBankerTotallScore;					//ׯ�ҳɼ�
	INT								nBankerTime;						//��ׯ����

	//��ҳɼ�
	LONGLONG						lUserScore;							//��ҳɼ�
	LONGLONG						lUserReturnScore;					//���ػ���

	//ȫ����Ϣ
	LONGLONG						lRevenue;							//��Ϸ˰��
};

struct CMD_S_Win_Rate_Info 
{
	DWORD							dwID[10];
	DWORD							dwRate[10];
	LONGLONG						llWinScore[10];
	LONGLONG						llLostScore[10];
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��
#define SUB_C_CANCEL_BANKER			3									//ȡ������
#define SUB_C_CONTINUE_CARD			4									//��������
#define SUB_C_GET_WIN_RATE			5									//��ȡӮ��
#define SUB_C_SET_WIN_RATE			6									//����Ӯ��


//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
};

struct TAndroidUser
{
	bool blAndroidUser;
	int  PlayCount;
	int  BankCount;
	TAndroidUser()
	{
		memset(this, 0, sizeof(*this));
	};
};

struct UserPlaceJetton
{
	WORD dwUserID;
	WORD dwChairID;
	UserPlaceJetton()
	{
		memset(this, 0, sizeof(*this));
	}
};

//////////////////////////////////////////////////////////////////////////
inline int  Findstr(char *buf, char findchar, int len)
{
	int i = 0;
	while (i < len)
	{
		if (buf[i++] == findchar)
			return (i - 1);
	}
	return -1;
}

inline char* strGetbuffer(char *buf, int ipos, char *OutBuf, int outLen)
{
	if (ipos < outLen)
		memcpy(OutBuf, buf, ipos);
	else
	{
		memcpy(OutBuf, buf, outLen);
	}
	return  OutBuf;

}

inline void MakeStirngToStirng(char *buf, int len, TCHAR outbuf[][128], int &ioutCount)
{

	int iMax = ioutCount;
	int iPos = 0;
	ioutCount = 0;
	while (1)
	{
		iPos = Findstr(buf, ',', (int)strlen(buf));
		if (iPos == -1)
		{
			char tmp[256] = {0};
			char *strtmp = buf;
			if (ioutCount < iMax - 1)
				memcpy(outbuf[ioutCount++], strtmp, (int)strlen(strtmp));
			break;
		}
		char tmp[256] = {0};
		char *strtmp = strGetbuffer(buf, iPos, tmp, 256);
		if (ioutCount < iMax - 1)
			memcpy(outbuf[ioutCount++], strtmp, (int)strlen(strtmp));
		buf = buf + iPos + 1;
	}
}

#endif

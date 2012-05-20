#ifndef CMD_SHOWHAND_HEAD_FILE
#define CMD_SHOWHAND_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID							ID_SD_MJ							//��Ϸ I D
#define GAME_PLAYER						4									//��Ϸ����
#define GAME_NAME						TEXT("ɽ���齫��Ϸ")						//��Ϸ����
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH|GAME_GENRE_SCORE)	//��Ϸ����


//��Ϸ״̬
#define GS_WK_FREE						GS_FREE								//�ȴ���ʼ
#define GS_WK_PLAYING					GS_PLAYING							//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//����������ṹ
#define SUB_S_SEND_MJ					100									//����
#define SUB_S_GAME_START				101									//��Ϸ��ʼ
#define	SUB_S_ADDFLOWER_MJ				102									//�û�����
#define SUB_S_HU_MJ						103									//�û�����
#define SUB_S_TING_MJ					104									//�û�����
#define SUB_S_GANG_MJ					105									//�û�����
#define SUB_S_PENG_MJ					106									//�û�����
#define SUB_S_CHI_MJ					107									//�û�����
#define SUB_S_GIVEUP					108									//�û�����
#define SUB_S_OUT_MJ					109									//�û�����
#define SUB_S_TOUCH_MJ					110									//�û�����
#define SUB_S_GAME_END					111									//��Ϸ����
#define SUB_S_SELECT_GANG				112									//����ѡ��ģʽ
#define SUB_S_SELECT_CHI				113									//����ѡ��ģʽ
#define SUB_S_END_EMPTY					114									//����
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define SUB_C_ADDFLOWER_MJ				1									//�û�����
#define SUB_C_HU_MJ						2									//�û�����
#define SUB_C_TING_MJ					3									//�û�����
#define SUB_C_GANG_MJ					4									//�û�����
#define SUB_C_PENG_MJ					5									//�û�����
#define SUB_C_CHI_MJ					6									//�û�����
#define SUB_C_GIVEUP					7									//�û�����
#define SUB_C_OUT_MJ					8									//�û�����
#define SUB_C_TOUCH_MJ					9									//�û�����
#define SUB_C_SELECT_GANG				10									//�û�ѡ�����
#define SUB_C_SELECT_CHI				11									//�û�ѡ�����

//////////////////////////////////////////////////////////////////////////

#define	HAND_MJ_MAX						14

#define ORG_MJ_MAX						136

#define BASE_SCORE						1									//�׷�
//��Ϸ״̬--����
struct CMD_S_StatusFree
{
	LONG 							lBaseScore;								//��������
	LONG							lBaseGold;								//�������
};

//��Ϸ״̬--����
struct CMD_S_StatusPlay
{
	LONG							lBaseScore;								//��������
	WORD							wCurrentUser;							//��ǰ���
	WORD							wLastOutUser;							//���ֳ������
	BYTE							byOutMj;								//����
	BYTE							byMjs[14];								//�����齫
};

//�����齫
struct CMD_S_SendMj
{
	BYTE							byMjs[14];								//�������齫��
	BYTE							byMjCount;								//�齫��Ŀ
	WORD							wBankerUser;							//��ǰׯ��
	int								nHu;									//�Ƿ����
	int								nGang;									//�Ƿ񰵸�
	BYTE							byStartMjPos;							//��ʼ������
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wBankerUser;							//ׯ��
	WORD							wQuanFeng;								//Ȧ��
	int								nSartNumber1;							//������
	int								nSartNumber2;							//������
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG							lGameTax;								//��Ϸ˰��
	LONG							lGameScore[4];							//��Ϸ����
	char							strEnd[300];							//��������
	BYTE							byMjs[4][14];							//�����齫
	//BYTE							byMjCount[4];							//�齫��Ŀ
	//BYTE							byMj[ORG_MJ_MAX];						//�齫�б�
};
struct CMD_S_GameEndEmpty
{
	BYTE							byMjs[4][14];							//�齫��
};
//�û�����
struct CMD_S_OutMj
{
	BYTE							byOutMj;								//�������
	//WORD							wCurrentUser;							//��ǰ���
	WORD							wOutMjUser;								//�������
	int								nHu;									//�ܷ��
	int								nGang;									//�ܷ��
	bool							bPeng;									//�ܷ���
	int								nChi;									//�ܷ��,���س�����Ϣ��0-���ɹ�,1-**@, 2-*@*, 3-@@*
	//int								nRight;								//Ȩ��,5-��,3-��,2-��,1-��
};

//�û�ת��
struct CMD_S_TransUser
{

	WORD							wCurrentUser;
};

//����
struct CMD_S_TouchMj
{
	BYTE							byTouchMj;								//����
	WORD							wCurrentUser;							//��ǰ���
	int								nHu;									//�Ƿ��ܺ���
	int								nGang;									//�Ƿ�������
};

//����ѡ��
struct CMD_S_GangMode
{
	BYTE							byMjGang[3];							//�ɸܵ���
	int								nGangType;								//��������
};
//����
struct CMD_S_Gang
{
	WORD							wActionUser;							//�������
	WORD							wOutMjUser;								//�������
	BYTE							mjFirst;								//���ƿ�����
	int								blockStyle;								//����
	int								gangType;								//��������,1-��,2-��,3-��
};
//����
struct CMD_S_Chi
{
	WORD							wActionUser;							//�������
	WORD							wOutMjUser;								//�������
	BYTE							mjFirst;								//���ƿ�����
	int								blockStyle;								//����
	int								nChiType;								//��������,0-��,1-@@*, 2-@*@, 4-*@@
};
//����ѡ��
struct CMD_S_ChiMode
{
	int								nChiType;								//��������
};
//����
struct CMD_S_Operation
{
	WORD							wActionUser;							//�������
	WORD							wOutMjUser;								//�������
	BYTE							mjFirst;								//���ƿ�����
	int								blockStyle;								//����
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
//�û�����
struct CMD_C_Operation
{
	WORD							wCurrentUser;							//��ǰ�û�
	WORD							wLastUser;								//�ϴγ����û�
	int								nRight;									//Ȩ��,6-��,4-��,3-��,2-��, 0-����			
};

//�Բ���
struct CMD_C_Chi
{
	int								nRight;									//Ȩ��
	int								nChiType;								//��������,0-��,1-@@*, 2-@*@, 3-*@@
};

//�ܲ���
struct CMD_C_Gang
{
	int								nRight;									//Ȩ��
	int								nGangType;								//��������,0-��, 1-����,2-����, 4-����
//	bool							bGangeMode;								//�Ƿ�������ѡ��ģʽ
};

//����ѡ��
struct CMD_C_SelectGang
{
	int								nGangType;								//��������
	BYTE							byGangMj;								//���ܵ���
};
//������
struct CMD_C_Hu
{
	int								nRight;									//����Ȩ��
};
#endif
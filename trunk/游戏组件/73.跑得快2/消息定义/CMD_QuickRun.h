#ifndef CMD_QUICKRUN_HEAD_FILE
#define CMD_QUICKRUN_HEAD_FILE

#pragma pack(push,1)
//////////////////////////////////////////////////////////////////////////
//�����궨��

#ifdef _TWICE
	#define KIND_ID							491									//��Ϸ I D
	#define GAME_NAME						TEXT("�ܵÿ���Ϸ�������ƣ�")		//��Ϸ����
	#define PISACOUNT						27									//ÿ����ҵ�����
#else
	#define KIND_ID							490									//��Ϸ I D
	#define GAME_NAME						TEXT("�ܵÿ���Ϸ")					//��Ϸ����
	#define PISACOUNT						13									//ÿ����ҵ�����
#endif
#define GAME_PLAYER						4									//��Ϸ����
#define GAME_GENRE						(GAME_GENRE_SCORE|GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����

#define WM_OUTPISA						WM_USER+147							//��ҳ�����Ϣ
#define WM_USERREADY					WM_USER+148							//���׼���õ���Ϣ


//��Ϸ״̬
#define GS_WK_FREE						GS_FREE								//�ȴ���ʼ
#define GS_WK_PLAYING					GS_PLAYING							//��Ϸ����

//�ƻ�ʽ����
struct tagPisa{
	int line;	//�У�4:���ң�3:���ң�2:÷����1:��Ƭ
	int row;	//�У�1-10��1-10��11��J��12��Q��13��K��[4][14]����ʾ������[3][14]����ʾС��
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ
#define MSG_SET_INITPISA				0x11			//��������������Լ���ʼ����
#define MSG_SET_FOCUS					0x12			//���õ�ǰ���Ƶ���
#define MSG_SET_OTHERPISA				0x13			//����������������ҵ���
#define MSG_OUTPISA_MSG					0x14			//����ύ������֮�󣬷������жϽ��
#define MSG_GAME_END					0x15			//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define MSG_SET_OUTPISA					0x06			//��ҳ�����

//////////////////////////////////////////////////////////////////////////

struct tagSetFocus{
	int nFoucsId;							//�����˵�ChairId
	int bClean;								//�Ƿ�������������ϵ���
};

struct tagSetInitPisa{
	int nOwnerIndex;						//�����Լ���Ե�����ֵ�����Լ���ChairId
	struct tagPisa tPisa[PISACOUNT];		//��ʼ����
};

struct tagSetOtherPisa{
	int nChairId;							//���Id
	int nCount;								//��ҳ�������Ŀ
	struct tagPisa tOutPisa[PISACOUNT];		//��ҳ�����
};

struct tagOutPisaMsg{
	int nResult;							//0:ͨ��,1:��ͨ��,���������Ϣ
	char chErrMsg[100];						//������Ϣ
};

struct tagGameEnd{
	LONG lGameTax;							//��Ϸ˰��
	LONG lGameScore[GAME_PLAYER];			//��Ϸ����
};

struct tagSetOutPisa{
	int nChairId;							//���Id
	int nCount;								//��ҳ���������Ŀ��Ϊ0��ʾPass
	struct tagPisa tOutPisa[PISACOUNT];		//��ҳ�����
};

#pragma pack(pop)

#endif
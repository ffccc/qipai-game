#ifndef CMD_BILLIARDS_HEAD_FILE
#define CMD_BILLIARDS_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						402									//��Ϸ I D
#define GAME_PLAYER					2									//��Ϸ����
#define GAME_NAME					TEXT("̨����Ϸ")					//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_GOLD)	//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//����������ṹ

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

//////////////////////////////////////////////////////////////////////////
/********************************************************************************/

// ̨����Ϸ��Ϣͷ�ļ�  3.0

/********************************************************************************/

#define		MAXCLIENTVER			MAKELONG(5,4)
#define		LESSCLIENTVER			0
#define		GS_TQ_WAIT_BEGIN					GS_FREE
#define		GS_GAME_PLAY						GS_PLAYING
#define		SUB_GAME_BEGIN						100		//��Ϸ��ʼ
#define     BALL_NUM                22       //��ĸ���

/********************************************************************************/

//	��Ϸ���ݰ�

/********************************************************************************/
//����Ϣ�ṹ

struct tagGS_PlaceInfo
{
	tagGS_PlaceInfo()
	{
		::memset(this, 0, sizeof(*this));
	}

	double				xPos;
	double				yPos;
	BYTE				bBallIndex;
};
//��Ϸ��������
struct tagGS_GameStation_1
{
	tagGS_GameStation_1()
	{
		::memset(this, 0, sizeof(*this));
	}
	BYTE bMatch;
};
//��Ϸ״̬���ݰ� ����Ϸ��ʹ�ã�
struct tagGS_GameStation_2
{
	tagGS_GameStation_2()
	{
		::memset(this, 0, sizeof(*this));
	}

	BYTE				bTargetBall;					//Ŀ����
	BYTE				bNowHitUser;					//��һ��������
	BYTE                mStarPos;
	BYTE                mStarPoint;               
	BYTE                bIsTakeWhite;
	BYTE				bInBallIndex[BALL_NUM];				//������������
	BYTE				bBallAccess[BALL_NUM];				//�ܷ����
	int					iUserPoint[BALL_NUM];					//��ǰ����
	tagGS_PlaceInfo		bPlaceInfo[BALL_NUM];					//�����Ϣ
};

struct tagGS_BeginStruct
{
	tagGS_BeginStruct()
	{
		::memset(this, 0, sizeof(*this));
		bIsTrain=FALSE;
	}
	BYTE				bFirstHit;						//�Ȼ�����
	BYTE                bIsTrain;
};
//��Ϸ�������ݰ�
struct tagGS_GameFinishStruct
{
	tagGS_GameFinishStruct()
	{
		::memset(this, 0, sizeof(*this));
	}
	BYTE			FinishMode;							//������ʽ(0����������1�Ƿ�����)
	int				Point[2];							//��¼��ҵķ���
	BYTE            m_ReSetRule;
	BYTE            m_NowHitUser;                       //������������
};


//������Ϣ
struct tagGS_HitInfoStruct
{
	tagGS_HitInfoStruct()
	{
		::memset(this, 0, sizeof(*this));
	}
	POINT						CursorPos;				//���λ��
	double                        iPower;
};

struct tagGS_CurrentPlayerStruct
{
	tagGS_CurrentPlayerStruct()
	{
		::memset(this, 0, sizeof(*this));
		Seat=0xff;
	}
	int					iPoint;
	BYTE				Seat;						//��ǰ������
};

//������

struct tagGS_HitBallStruct
{
	tagGS_HitBallStruct()
	{
		::memset(this, 0, sizeof(*this));
	}
	int					iPower;
	int					iXPole;
	int					iYPole;
	double				iXBallPoint;
	double				iYBallPoint;
	double              iPowePos;    
	double              iPoweAngle; 
};


//��һ������

struct tagGS_NextHitInfoStruct
{
	tagGS_NextHitInfoStruct()
	{
		::memset(this, 0, sizeof(*this));
	}
	int					iPoint;
	BYTE				bHitPeople;
	BYTE				bTargerBall;
	tagGS_PlaceInfo		bPlaceInfo[BALL_NUM];					//�����Ϣ
	BYTE                mStarPos;
	BYTE                mStarPoint;
	BYTE                bIsGetStarPoint;
};


//�������
struct tagGS_HitFinishStruct
{
	tagGS_HitFinishStruct()
	{
		::memset(this, 0, sizeof(*this));
	}
	BYTE				bFirstHitBall;
	BYTE				bInBallCount;
	BYTE				bInBallIndex[BALL_NUM];
	BYTE				bBallAccess[BALL_NUM];
	BYTE                bIsGetStar;
	double				iXBallStation[BALL_NUM];
	double				iYBallStation[BALL_NUM];
};


//��������Ϣ�������������

struct tagCS_PlaceBallStruct
{
	tagCS_PlaceBallStruct()
	{
		::memset(this, 0, sizeof(*this));
	}
	BYTE				bBallCount;
	tagGS_PlaceInfo			bPlaceInfo[BALL_NUM];
};

//��������Ϣ������ͻ��ˣ�
struct tagGS_PlaceInfoStruct
{
	tagGS_PlaceInfoStruct()
	{
		::memset(this, 0, sizeof(*this));
	}
	BYTE				bHitPeople;
	BYTE				bBallCount;
	BYTE				bTargerBall;
	tagGS_PlaceInfo			bPlaceInfo[BALL_NUM];
};


//GSQ:�������·�����Ϣ������ͻ��ˣ�
struct tagGS_ColorBallPlaceInfoStruct
{
	tagGS_ColorBallPlaceInfoStruct()
	{
		::memset(this, 0, sizeof(*this));
	}
	BYTE				bBallCount;			//Ҫ�����������
	BYTE				bInBallIndex[BALL_NUM];	//�������������
	BYTE				bPos;				//��˭�ò��������
	bool				BallError;			//�ǲ��ǽ�������
	small				BallValue;			//���������Ҫ�۵ķ���
	BYTE                Seat;
};
/********************************************************************************/

// ���ݰ���������ʶ

/********************************************************************************/

#define ASS_TQ_HIT_INFO			50
#define ASS_TQ_HIT_BALL			51
#define ASS_TQ_NEXT_HIT			52
#define ASS_TQ_TAKE_WHITE		53
#define ASS_TQ_PLACE_WHITE		54
#define	ASS_TQ_HIT_FINISH		55
#define ASS_TQ_PLACE_COLOR		56
#define ASS_TQ_GAME_FINISH		57
#define ASS_TQ_1				59
#define ASS_TQ_2				61
#define ASS_TQ_HIT_GIVEUP       62
#define ASS_TQ_HIT_REPLACE      63
#define ASS_TQ_HIT_CONESSION    64
#define ASS_TQ_AGREE		    65
#define ASS_TQ_NOAGREE          68

/********************************************************************************/


#endif
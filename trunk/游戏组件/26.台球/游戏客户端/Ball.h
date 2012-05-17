#ifndef BALL_HEAD_FILE
#define BALL_HEAD_FILE

#pragma once

#define GRATING				0.994					//Ħ����ϵ��
#define HIT_LOST			0.95					//��ײ��ʧ 
#define LESS_SPEED			0.18					//�����ٶ�
#define MAX_SPEED			10.0					//����ٶ�
#define BALL_COUNT			9						//������
#define SQR_RADII_2			14.14					//�뾶ƽ������
#define DESK_SIDE			40						//���ӱ�
#define BALL_RADII			11.9						//��뾶
#define BALL_RADII_A	    11.0						//��뾶
#define BAG_WIGHT			1.65*BALL_RADII			//����

#define BALL_BACK_COLOR		RGB(255,0,255)

//̨����

class CBall
{
	//�������� 
public:
	static double			m_Grating;				//Ħ����

public:
	BYTE					m_bColor;				//����ɫ
	BYTE					m_bIndex;				//������
	bool					m_bAccess;				//�Ƿ���Զ�ȡ
	double					m_xPos;					//X ����
	double					m_yPos;					//Y ����
	double					m_xSpeed;				//X �ٶ�
	double					m_ySpeed;				//Y �ٶ�
	double					m_xPreSpeed;			//X Ԥ�ٶ�
	double					m_yPreSpeed;			//Y Ԥ�ٶ�
	double					m_xTurnSpeed;			//X �����ٶ�
	double					m_yTurnSpeed;			//Y �����ٶ�
	double					m_xPreTurnSpeed;		//X ����Ԥ�ٶ�
	double					m_yPreTurnSpeed;		//Y ����Ԥ�ٶ�
	double					m_PreSina;				//X ���ӽǶ�
	double					m_PreCosa;				//Y ���ӽǶ�

	//�������� 
public:
	//���캯�� 
	CBall(void);
	//�ƶ����� 
	BOOL Move();
	//���º���
	BOOL UpdateSpeed();
	//����ת���ٶ�
	BOOL UpdateTurnSpeed();
	//����λ��
	BOOL SetStation(double xPos, double yPos);
	//�����ٶ� 
	BOOL SetSpeed(double xSpeed, double ySpeed, double xTurnSpeed=0.0, double yTurnSpeed=0.0);
	//����Ԥ�ٶ�
	BOOL SetPreSpeed(double xSpeed, double ySpeed);
	//��ײ���
	BOOL CollideBall(CBall & Ball);
	//������ײ
	BOOL TestCollide(double x, double y);
	
};

//���
class CBallBag
{
	//��������
public:
	bool					m_bNeedMove;				//�Ƿ���Ҫ�ƶ�
	int						m_iBagLength;				//�������
	int						m_iBallCount;				//����Ŀ
	int						m_bBallStation[BALL_NUM];			//��λ��
	CBall					* m_pBall[BALL_NUM];				//������

	//��������
public:
	//���캯��
	CBallBag();
	//����
	BOOL PushBall(CBall & Ball);
	//ȡ��
	CBall * TakeBall(BYTE bBallColor);
	//�ƶ�
	BOOL Move();
};

//̨������ 
class CBallDesk
{
	//��������
public:
	bool					m_bSound;					//����Ч��
	bool					m_bNeedMove;				//�Ƿ���Ҫ�ƶ�
	long int				m_dwWidth;					//������
	long int				m_dwHeight;					//������
	int						m_iBallCount;				//����Ŀ
	int						m_iInBagCount;				//����������Ŀ
	int						m_iFirstHitBag;				//��һ�μ��е�������
	CBall					* m_pBallList;				//���б�
	CBallBag				m_BallBag;					//���;
	CPoint					m_BagPoint[6];				//��λ��
	bool	bSoundInBag;
	bool	bSoundHitBall;
	bool 	bSoundHitDesk;
	BOOL    bIsGetStar;

	//�ַ���Ϣ
public:
	int						m_iTextPos;					//�ַ�λ��
	int						m_iScrollPos;				//��Ϣ������
	TCHAR					m_szText[101];				//�ַ�����

	//��������
public:
	//���캯�� 
	CBallDesk();
	//��ʼ��̨����
	virtual BOOL InitDesk()=0;

	//���ܺ���
public:
	//��ʱ������
	int OnTimer();
	//������
	void HitBall(int x, int y, double fPower, double xTurn, double yTurn,double HitDegree);
	//�Ƿ����
	bool IsBallInBag();
	//����λ��
	bool IsHitDesk(int iBallStation);
	//�����Ƿ���Է���
	bool CanPlaceBall(double x, double y, bool bWhiteBall);
	//������Ϣ�ַ� 
	BOOL SetMessageText(TCHAR * szMessage, int iScrollPos, int yPos,BOOL b=false);
};

//��ʽ̨��
class CAmericaDesk : public CBallDesk
{
	//�������� 
public:
	CBall					m_Ball[BALL_NUM];
	//���ô���
	BOOL SetBagWight(BYTE  GameLayer);
	//��������
public:
	//��ʼ��̨����
	virtual BOOL InitDesk();
};

#endif

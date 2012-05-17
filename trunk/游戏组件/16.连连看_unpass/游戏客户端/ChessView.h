#ifndef CHESSVIEW_HEAD_FILE
#define CHESSVIEW_HEAD_FILE
#pragma once

//�Զ�����Ϣ
#define					    ID_PROCESS                          1111            //��������Ϣ
#define						ID_GETNEXTBALLCOLOR					2222			//��ȡ��һ��С�����Ϣ
#define						ID_SUCCESS							3333			//�ɹ���ȥ
#define						ID_PSUCCESS							3334			//��ϰ�Ǹ�View���ɹ���Ϣ
#define						ID_FIRSTBALL						4444			//����
#define						ID_TAKE								5555			//����

#define						ID_GAMEOVER							6666			//��Ϸ����

#define     ERROR_POINT						100

// CChessView

class CChessView : public CWnd
{
public:
	CChessView();
	virtual ~CChessView();

public:
	CGameLogic      								m_Logic;								//��Ϸ�߼���
	bool											m_ExerciseMode;							//�Ƿ���ϰģʽ
	DBall											m_Chess[82];							//��Ϸ���̣���0Ԫ��û��ʹ��
	int												m_Result[82];							//�洢�����Ϣ
	int												m_tResult[82];
	BYTE											m_SuccessArray[9];						//�洢�ɹ���ȥʱ��λ��
	DBall											m_DeadBall[9];							//��Ҫ��ȥ��С��
	BYTE											m_StartPos;								//��ѯʱ��ʼλ��
	BYTE											m_EndPos;								//��ѯ�ǽ���λ��

	BYTE											m_firstball[5];						    //����
	DBall											m_firstballcolor[5];					//�������ɫ

	BYTE											m_nextball[3];							//��һ�ֵ�������;
	DBall											m_nextballcolor[3];						//��һ�ֵ���������ɫ;

	BYTE											m_tnextball[3];
	DBall											m_tnextballcolor[3];

	DBall											m_StartColor;							//��ʼλ����ɫ
	DBall											m_EndColor;								//����λ����ɫ
	DBall											m_ResultColor;							//

	bool											m_first;								//��һ�η���
    
	BYTE											m_result;								//����ʱ��ÿһ�����

	int												m_score;								//����	
	TCHAR											m_scorestring[40];

	bool											m_Bobm;									//�Ƿ��ͷ�ը��

	bool			m_BakeHit;								//�ϴ��Ƿ���ȥ
	int				m_BakeHitCount;							//��������

	bool			m_isLose;								//�Ƿ�ʧ��

	BYTE			m_GroupID;								//��Ӻ�

	TCHAR			m_class[40];
	TCHAR			m_per[40];
	int				m_Iper;
	
	void SuccessProcess(BYTE succ_array[], BYTE point);
	void TakeTheService();
	void DisplayTakeTheService(BYTE nball[3],DBall nballcolor[3]);
protected:	

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void InitChessView(void);
	bool IsMove(void);
	// �ͷ�ը��
	void BobmFunc(BYTE point,BYTE bombarray[]);
	// �ƶ��ͷ���
	void MoveBall(BYTE end, DBall scolor);
	afx_msg void OnTimer(UINT nIDEvent);
	// ���̿�λ���������
	bool BallRandFuc(DBall  wChess[], BYTE  wNextBall[], int num);
};
#endif
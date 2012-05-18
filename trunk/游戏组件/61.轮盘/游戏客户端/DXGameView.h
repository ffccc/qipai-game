#ifndef DX_GAME_VIEW_HEAD_FILE
#define DX_GAME_VIEW_HEAD_FILE
#pragma once
#include "GameLogic.h"
#include "ScoreView.h"

#include "DxDialog.h"
#include "SkinMesh.h"
#include "DxControl.h"
#include "Camera.h"
#include "SkinImageEx.h"
#include "SkinSprite.h"
/////////////////////////////////////////////////////////////////////


#define     MAX_CHIPS			1000//�����ע�������
#define		JETTON_COUNT		9	//��������
#define		HISTORY_NUMBER		13	//��ʷ��¼����

#define		MAX_MOVE_RADIUS		120	//������˶��뾶
#define		WIN_RADIUS			75	//ʤ���뾶
#define		JUMP_RADIUS			85	//��Ծ�뾶
#define		FALL_RADIUS			116	//����뾶

#define		WIN_CHIP_POS_X		380	//�����ƶ�
#define		WIN_CHIP_POS_Y		600	//�����ƶ�

#define		IDM_CHIP_IN			(WM_USER+300)	//��ʼ��Ϣ
#define		IDM_WIN_NUMBER		(WM_USER+301)	//ʤ������
#define		IDM_GET_STOP		(WM_USER+302)	//ʤ������

#define		STATUS_FREE			0	//����״̬
#define		STATUS_READY		1	//׼��״̬
#define		STATUS_PLAY			2	//��ת״̬

/////////////////////////////////////////////////////////////////////

//d3d������
class CDXGameView : public CDxDialog
{
	//���б���
protected:
	BYTE				m_bStatus;							//��Ϸ״̬
	bool				m_bZeroLight;						//0�����ֵ�
	bool				m_bNumberLight[3][12];				//1~36���ֵ�
	long				m_wAllMoney;						//������ע����
	long				m_wHistoryChipInfo[MAX_BET_POS];	//��ʷ��ע��Ϣ
	long				m_wChipInfo[MAX_BET_POS];			//������ע������Ϣ����
	long				m_wOtherUsersChips[MAX_BET_POS];	//�����û���ע
	int					m_nScreenWidth,m_nScreenHeight;		//dx���ڿ��
	TCHAR				m_TipsMessage[64];					//��ʾ��Ϣ
	BYTE				m_bHistoryInfo[HISTORY_NUMBER];		//��ʷ��Ϣ
	int					m_wCurrentChip;						//��ǰѡ�����
	LONG				lScoreIndex[JETTON_COUNT];			//��������

	//����Ʊ���
	float				m_Ball_MoveRadius;	//�˶��뾶
	float				m_Ball_StartHeight;	//��ʼ�߶�
	float				m_fBallMoveAngel;	//�ƶ��Ƕ�
	D3DXVECTOR3			m_vBallMovePath;	//�ƶ�·��
	D3DXVECTOR3			m_vCenterPos;		//���ĵ�λ��
	BYTE				m_bWinNumber;		//ʤ������
	BYTE				m_bTargetNumber;	//Ŀ������

	//���ƽ�����ʾ
public:
	BYTE				m_bShowEnd;//0:����ʾ 1:��ʾʤ�� 2:��ʾʧ��
	long				m_lGameEndScore;//��������
	long				m_lMinBetScore;//��С��ע
	long				m_lMaxBetWin;//���Ӯע
	long				m_lMaxUserScore;//����û�����
	bool				m_bDrawMoveChips;
	int					m_iMoveChipsCount;
	//////////////////////////////////////////////////////////////////////
	//���±����ڴ�������ʱ��Ҫ�ͷ�
	//ƽ��ͼƬ
protected:
	CSkinSprite			m_Chip1;			//���1����
	CSkinSprite			m_Chip5;			//���5����
	CSkinSprite			m_Chip10;			//���10����
	CSkinSprite			m_Chip50;			//���50����
	CSkinSprite			m_Chip100;			//���100����
	CSkinSprite			m_Chip500;			//���500����
	CSkinSprite			m_Chip1000;			//���1000����
	CSkinSprite			m_Chip5000;			//���5000����
	CSkinSprite			m_Chip10000;		//���10000����
	CSkinSprite			m_SprCurrentChip;	//��ǰѡ�����

	CSkinSprite			m_SprMoveChips[MAX_CHIPS];
	CSkinSprite			m_SprTimer[10];

	CSkinSprite			m_PlaneBall;		//ƽ����
	CSkinSprite			m_PlatePlane;		//ƽ������
	CSkinSprite			m_PlatePlaneMask;	//�����ڸ�

	CSkinSurface		m_SurfaceTable;		//��������
	CSkinSurface		m_SurfaceBack;		//��Χ����
	CSkinSprite			m_NumberMask;		//�����ӦЧ��
	//����ģ��
protected:
	CSkinMesh 			m_MeshPlate;		//�������ֵ���
	CSkinMesh 			m_MeshStock;		//��Χ������
	CSkinMesh			m_MeshShelfHigh;	//�м�ļ����ϲ�
	CSkinMesh			m_MeshShelfLow;		//�м�ļ����²�
	CSkinMesh			m_MeshRing;			//�ָ����ֵĻ�
	CSkinMesh			m_MeshBall;			//��
	LPDIRECT3DTEXTURE9	m_pReflect;			//��������
	LPDIRECT3DTEXTURE9	m_pBump;			//��͹����
	//////////////////////////////////////////////////////////////////////

	//���Ʊ���
public:
	CSkinButton			m_btChipIn;				//ȷ����ע��ʼ
	CSkinButton			m_btClearChip;			//�����ע��ť
	bool				m_bAllowBet;			//������ע
	BYTE				m_bTimeLeave;//ʣ��ʱ��

	//�ؼ�����
protected:
	CGameLogic			m_GameLogic;
	CScoreView			m_ScoreView;

	//��������
public:
	//���캯��
	CDXGameView(void);
	//��������
	virtual ~CDXGameView(void);

	//�̳к���
public:
	virtual void DrawGameView2D();
	virtual void DrawGameView3D();
	virtual void InitScene(int nWidth,int nHeight);
	virtual void ReleaseScene();
	virtual void UpDateControl();

	//��չ����
public:
	//���õƹ�
	void SetLight();
	//ȡ�ó�����Ϣ
	void GetChipsInfo();
	//�滭����
	void DrawChips(int x,int y,long money);
	//�滭�����ƶ�
	void SetMoveChips(int x,int y,long money);
	//�滭��ʷ
	void DrawHistory();
	//ȡ��ѹע���
	long GetAllMoney();
	//ȡ����ʷ��ע
	long GetHistoryMoney();
	//�����ע
	void ClearAllMoney();
	//��������˶�·��
	void calcPath(D3DXVECTOR3 *movePath);
	//�õ�����ͣ����
	BYTE GetBallStopNumber(float angel);
	//��ʷ����
	void SetHistoryNumber(BYTE bHistoryInfo[HISTORY_NUMBER]);
	//ȡ�ó���λ��
	void GetChipPosition(int index,CPoint *pPosition);

	//��Ϸ��Ϣ
public:
	void OnGameStart(BYTE bTargetNumber);
	void OnUserChip(long lAllChip[256]);
	void OnRollStop(BYTE bWinNumber);
	void OnGameEnd();
	void OnSetCellScore(long lCellScore);
	//��Ϣӳ��
protected:
	//����ƶ���Ϣ
	afx_msg void OnMouseMove(UINT nFlags,CPoint point);
	//���������Ϣ
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	//�Ҽ�������Ϣ
	afx_msg void OnRButtonDown(UINT nFlags,CPoint point);
	//��ʼ��ת
	afx_msg void OnBnClickedChipIn();
	//�����ע
	afx_msg void OnBnClickedClearChip();

	DECLARE_MESSAGE_MAP()
};

#endif

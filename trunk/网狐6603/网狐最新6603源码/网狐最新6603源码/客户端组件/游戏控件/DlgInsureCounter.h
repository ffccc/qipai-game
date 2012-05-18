#ifndef DLG_INSURE_COUNTER_HEAD_FILE
#define DLG_INSURE_COUNTER_HEAD_FILE

#pragma once

#include "MissionManager.h"
#include "PasswordControl.h"
#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
#define DT_PLAZA					0									//��������
#define DT_FRAME					1									//�������
#define DT_SERVER					2									//��������

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagUserInsureInfo
{
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
};

//////////////////////////////////////////////////////////////////////////////////

//���нӿ�
interface IInsureCounterAction
{
	//��ѯ�¼�
	virtual VOID PerformQueryInfo()=NULL;
	//�����¼�
	virtual VOID PerformSaveScore(SCORE lSaveScore)=NULL;
	//ȡ���¼�
	virtual VOID PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//���չ���
class SHARE_CONTROL_CLASS CDlgInsureItem : public CSkinDialog, public IInsureCounterAction
{
	//��������
protected:
	bool							m_bSaveMission;						//�������
	bool							m_bTakeMission;						//ȡ������
	bool							m_bQueryMission;					//��ѯ����

	//��ѯ����
protected:
	bool							m_bInitInfo;						//������Ϣ
	tagUserInsureInfo				m_UserInsureInfo;					//������Ϣ

	//�ؼ�����
protected:
	CSkinEditEx						m_edScore;							//�������
	CSkinButton						m_btQueryInfo;						//��ѯ��ť
	CSkinButton						m_btTakeScore;						//ȡ�ť
	CSkinButton						m_btSaveScore;						//��ť

	//�����ؼ�
protected:
	CSkinTabCtrl					m_TabControl;						//����ѡ��
	CSkinHyperLink					m_ForgetInsure;						//��������
	CPasswordControl				m_PasswordControl;					//�û�����

	//��������
public:
	//���캯��
	CDlgInsureItem();
	//��������
	virtual ~CDlgInsureItem();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();
	//ȷ����ť
	virtual VOID OnOK();

	//���ں���
protected:
	//�滭��Ϣ
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//�¼�����
protected:
	//��ʼ����
	VOID OnMissionStart();
	//��ֹ����
	VOID OnMissionConclude();

	//���ܺ���
protected:
	//��ѯ����
	VOID QueryUserInsureInfo();
	//��ȡ���
	SCORE GetUserControlScore();

	//�ڲ�����
protected:
	//�滭����
	VOID DrawNumberString(CDC * pDC, SCORE lScore, INT nXPos, INT nYPos);
	//ת���ַ�
	VOID SwitchScoreString(SCORE lScore, LPTSTR pszBuffer, WORD wBufferSize);
	//ת���ַ�
	VOID SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize);

	//��Ϣ����
protected:
	//������Ϣ
	VOID OnEnChangeScore();
	//��ѯ��ť
	VOID OnBnClickedQueryInfo();
	//��ť
	VOID OnBnClickedSaveScore();
	//ȡ�ť
	VOID OnBnClickedTakeScore();
	//��������
	VOID OnBnClickedForgetInsure();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//�㳡����
class SHARE_CONTROL_CLASS CDlgInsurePlaza : public CDlgInsureItem, public CMissionItem
{
	//��������
protected:
	SCORE							m_lScore;							//�����Ŀ
	TCHAR							m_szInsurePass[LEN_PASSWORD];		//��������

	//�������
protected:
	CMissionManager					m_MissionManager;					//�������

	//��������
public:
	//���캯��
	CDlgInsurePlaza();
	//��������
	virtual ~CDlgInsurePlaza();

	//ִ�к���
protected:
	//��ѯ�¼�
	virtual VOID PerformQueryInfo();
	//�����¼�
	virtual VOID PerformSaveScore(SCORE lSaveScore);
	//ȡ���¼�
	virtual VOID PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass);

	//�����¼�
private:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class SHARE_CONTROL_CLASS CDlgInsureServer : public CDlgInsureItem
{
	//��������
protected:
	ITCPSocket *					m_pITCPSocket;						//����ӿ�
	IClientUserItem *				m_pIMySelfUserItem;					//�Լ�ָ��

	//��������
public:
	//���캯��
	CDlgInsureServer();
	//��������
	virtual ~CDlgInsureServer();

	//ִ�к���
protected:
	//��ѯ�¼�
	virtual VOID PerformQueryInfo();
	//�����¼�
	virtual VOID PerformSaveScore(SCORE lSaveScore);
	//ȡ���¼�
	virtual VOID PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass);

	//���ܺ���
public:
	//��������
	VOID Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem);
	//������Ϣ
	bool OnServerInsureMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////////////

#endif
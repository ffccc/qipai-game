#pragma once

void MakeString(CString &strNum,LONGLONG lNumber);
void MakeStringToNum(CString str,LONGLONG &Num);
// CDlgBank dialog
#define  WM_CREADED						WM_USER+1
class CDlgBank : public CSkinDialogEx
{
	//��������
public:
	bool							m_bBankStorage;					//�洢��ʶ
	tagUserData						*m_pMeUserData;					//�û���Ϣ
	LONGLONG						m_lGameGold;					//������Ŀ
	LONGLONG						m_lStorageGold;					//�洢��Ŀ
	LONGLONG						m_lInCount;						//������Ϸ��
	LONGLONG						m_OrInCount;
	bool							m_bCanDraw;
	bool							m_blCanStore;
	//bool							m_bStorageAllGold;				//�������н��
	//bool							m_bDrawAllGold;					//ȡ�����н��
	//�ؼ�����
protected:
	CSkinButton						m_btOK;							//ȷ����ť
	CSkinButton						m_btCancel;						//ȡ����ť
	CEdit							m_Edit;
	CStatic							m_Static;

	bool							m_blUsingPassWord;



	//�ӿڱ���
protected:	
	IClientKernel					* m_pIClientKernel;				//�ں˽ӿ�


public:
	CDlgBank(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgBank();

	//��������
public:
	//���½���
	void UpdateView();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange* pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����ť
	afx_msg void OnBnClickedOk();
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//������Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//������Ϣ
	afx_msg void OnEnChangeInCount();

	afx_msg void OnEnRadioStore();

	afx_msg void OnEnRadioGet();

    int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�ӿں���
public:
	//���ÿؼ�
	void ShowItem();
	//����ȡ��
	void AllowDraw(bool bAllow);
	//������
	void AllowStorage(bool bAllow);
	//����λ��
	void SetPostPoint(CPoint Point);
	//��������
	void SetBankerActionType(bool bStorage);
	//���ܺ���
public:
	//�Ƿ�ѡ��
	bool IsButtonSelected(UINT uButtonID);
	//ѡ�а�ť
	void SetButtonSelected(UINT uButtonID, bool bSelected);
	//�������
	void SetSendInfo(IClientKernel *pClientKernel,tagUserData const*pMeUserDatas);
	//������Ϣ
	void SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CDlgBank)
protected:
	virtual void OnCancel();
};

#ifndef CUSTOM_FACE_HEAD_FILE
#define CUSTOM_FACE_HEAD_FILE

#pragma once
////////////////////////////////////////////////////////////////////////////////////

//����״̬
enum enOperateStatus
{
	enOperateStatus_NULL,
	enOperateStatus_Upload,
	enOperateStatus_Delete
};

////////////////////////////////////////////////////////////////////////////////////

//�Զ���ͷ��
class CDlgCustomFace : public CDialog, CSkinWndObject
{
	//��������
protected:
	CImage						m_Image;							//ͼƬ����
	enOperateStatus				m_enOperateStatus;					//״̬����
	CBrush						m_brBkground;						//����ˢ��

	//�ؼ�����
protected:
	CSkinButton					m_btBrowser;						//�����ť
	CSkinButton					m_btUpload;							//�ϴ���ť
	CSkinButton					m_btDelete;							//ɾ����ť
	ITCPSocket					* m_pIClientSocket;					//�������
	//tagUserData					* m_pMeUserData;					//�û���Ϣ

	//��������
public:
	//���캯��
	CDlgCustomFace(CWnd* pParent = NULL);
	//�鹹����
	virtual ~CDlgCustomFace();

	//��������
public:
	//��������
	bool SendData();

	//��������
protected:
	//��������
	virtual void DoDataExchange(CDataExchange* pDX);
	//��ʼ����
	virtual BOOL OnInitDialog();

	//��������
public:
	//��ʾ��Ϣ
	void ShowMessage(TCHAR *pMessage);
	//��������
	bool ConnectServer();
	//���¿ؼ�
	void UpdateControls();
	//����״̬
	void SetStatus(enOperateStatus enOperateStatus){m_enOperateStatus = enOperateStatus;}

	//��Ϣ����
protected:
	//���ͼƬ
	afx_msg void OnBnClickedBrowser();
	//�ϴ�ͼƬ
	afx_msg void OnBnClickedUpload();
	//ɾ��ͼƬ
	afx_msg void OnBnClickedDelete();
	//�滭����
	afx_msg void OnPaint();
	//��ɫ����
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//��ʾ��Ϣ
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//������Ϣ
	afx_msg void OnClose();
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////

#endif
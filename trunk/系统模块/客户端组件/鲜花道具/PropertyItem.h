#ifndef PROPERTY_ITEM_HEAD_FILE
#define PROPERTY_ITEM_HEAD_FILE

#pragma once

#include "PropertyBar.h"
#include "PropertyModule.h"
//////////////////////////////////////////////////////////////////////////

//������
class PROPERTY_MODULE_CLASS CPropertyItem : public CSkinDialogEx
{
	//��������
protected:
	bool								m_bPlazaRoom;						//��������
	int									m_nPropertyType;					//��������
	int									m_nPropertyID;						//����ID

	//�󶨱���
protected:
	CComboBox							m_ComboBoxTargetUser;				//Ŀ�����
	CComboBox							m_ComboBoxPropType;					//��������
	int									m_nPropCount;						//������Ŀ
	CString								m_strNormalUserPrice;				//��ͨ�۸�
	CString								m_strMemberUserPrice;				//��Ա�۸�
	CString								m_strGoldDescribe;					//��Ϸ�����
	CString								m_strProContent;					//��������
	CSkinButton							m_btOK;								//ȷ����ť
	CSkinButton							m_btCancel;							//�رհ�ť

	//�������
protected:
	IPurchaseInfo						* m_pIPurchaseInfo;					//��Ϣ�ӿ�

	//��������
public:
	//���캯��
	CPropertyItem(CWnd* pParent = NULL);
	//��������
	virtual ~CPropertyItem();

protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange* pDX);

	//���ܺ���
public:
	//���õ���
	void SetPropertyID(int nPropertyID);
	//���ýӿ�
	void SetPurchaseInfoSink(IPurchaseInfo *pIPurchaseInfo);
	//��ʾ��Ϣ
	void ShowMessage(TCHAR *pszMessage);
	//���½���
	void UpdateView();
	//ѡ���û�
	void SetUserDataInfo(tagUserData *pUserData);

	//�ڲ�����
private:
	//��������
	DWORD CalcDays(DWORD dwTime);
	//����Сʱ��
	DWORD CalcHours(DWORD dwTime);
	//���������
	DWORD CalcMinutes(DWORD dwTime);
	//��������
	DWORD CalcSeconds(DWORD dwTime);
	//��ȡ�۸�
	void GetPropertyPrice(LONG &lNormalPrice, LONG &lMemberPrice);

	//��Ϣ��Ӧ
private:
	//�滭����
	afx_msg void OnPaint();
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	afx_msg void OnBnClickedOk();
	//�ؼ���ɫ
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//��ť��Ϣ
	afx_msg void OnCbnSelchangeTargetUser();
	//��ť��Ϣ
	afx_msg void OnCbnSelchangePropType();
	//������Ϣ
	afx_msg void OnEnChangePurchaseCount();
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//������Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//��Ϣӳ��
	DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////////////////////////////////////

//������
class PROPERTY_MODULE_CLASS CFlowerSetting :public CSkinDialogEx
{
	//��������
public:
	bool							m_bPlazaRoom;						//��������
	int								m_nFlowerCount;						//�ʻ���Ŀ
	int								m_nFlowerID;						//�ʻ�ID

	//�󶨱���
public:
	CComboBox						m_ComboBoxTargetUser;				//Ŀ�����
	CString							m_strDescribe;						//��ͨ�۸�
	CString							m_strNormalUserPrice;				//��ͨ�۸�
	CString							m_strMemberUserPrice;				//��Ա�۸�
	CString							m_strGoldDescribe;					//��Ϸ�����

	//�ؼ�����
protected:
	CSkinButton						m_btOK;								//ȷ����ť
	CSkinButton						m_btCancel;							//ȡ����ť

	//����ӿ�
protected:
	IPurchaseInfo					* m_pIPurchaseInfo;					//��Ϣ�ӿ�

	//��������
public:
	//���캯��
	CFlowerSetting(CWnd* pParent = NULL);
	//��������
	virtual ~CFlowerSetting();

	//��������
public:
	//���ýӿ�
	void SetPurchaseInfoSink(IPurchaseInfo *pIPurchaseInfo);
	//����ID
	void SetFlowerID(int nFlowerID) ;
	//���½���
	void UpdateView();
	//����Ŀ��
	//void SetTargetUserID(DWORD dwTargetUserID){m_dwTargetUserID = dwTargetUserID;}
	//ѡ���û�
	void SetUserDataInfo(tagUserData *pUserData);

	//�̳к���
protected:
	//�󶨺���
	virtual void DoDataExchange(CDataExchange* pDX);

	//��Ϣ����
public:
	//�滭����
	afx_msg void OnPaint();
	//��ʼ������
	virtual BOOL OnInitDialog();
	//��ť��Ϣ
	afx_msg void OnBnClickedOk();
	//�ر���Ϣ
	afx_msg void OnBnClickedCancel();
	//������ɫ
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//������Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//������Ϣ
	afx_msg void OnEnChangeFlowerCount();

	//��Ϣӳ��
	DECLARE_MESSAGE_MAP()
};
///////////////////////////////////////////////////////////////////////////////////
#endif
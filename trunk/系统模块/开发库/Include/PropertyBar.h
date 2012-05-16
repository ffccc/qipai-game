#ifndef PROPERTY_BAR_HEAD_FILE
#define PROPERTY_BAR_HEAD_FILE

#pragma once

#include "PropertyModule.h"
///////////////////////////////////////////////////////////////////////////////////

//��˵��
class CPropertyItem;
class CFlowerSetting;

//���߿ؼ�
class PROPERTY_MODULE_CLASS CPropertyBar : public CDialog
{
	//��Դ����
public:
	HINSTANCE							m_ReshInstance;						//��Դ���
	tagPropertyViewImage				m_PropertyViewImage;				//������Դ

	//λ�ñ���
protected:
	int									m_nXoriginal;						//��ʼλ��
	int									m_nButtonCount;						//��ť��Ŀ
	int									m_nTopButtonHead;					//��ʼ��ť
	int									m_nBtmButtonHead;					//��ʼ��ť

	//���߱���
public:
	int									m_nPropertyCount;					//������Ŀ
	int									m_nFlowerCount;						//�ʻ���Ŀ

	//״̬����
protected:
	bool								m_bInitDialog;						//��ʼ����

	//��̬����
public:
	static tagPropertyInfo				m_PropertyInfo[PROPERTY_COUNT];		//������Ϣ
	static tagFlowerInfo				m_FlowerInfo[FLOWER_COUNT];			//�ʻ���Ϣ
	static CPropertyBar					* m_pPropertyBar;					//���ƶ���

	//��Դ����
protected:
	CSkinImage							m_ImageL;							//���ͼƬ
	CSkinImage							m_ImageM;							//�м�ͼƬ
	CSkinImage							m_ImageR;							//�ұ�ͼƬ

	//�����ʻ�
protected:
	CSkinButton							m_BtProperty[PROPERTY_COUNT];		//���߰�ť
	CSkinButton							m_BtFlower[FLOWER_COUNT];			//�ʻ���ť

	//���ư�ť
protected:
	CSkinButton							m_btTopMoveToL;						//���ư�ť
	CSkinButton							m_btTopMoveToR;						//���ư�ť
	CSkinButton							m_btBtmMoveToL;						//���ư�ť
	CSkinButton							m_btBtmMoveToR;						//���ư�ť
	CToolTipCtrl						m_ToolTipCtrl;						//��ʾ�ؼ�

	//�������
protected:
	CPropertyItem						*m_pPropertyItem;					//���߶Ի���
	CFlowerSetting						*m_pFlowerSetting;					//����Ի���
	IPurchaseInfo						*m_pIPurchaseInfo;					//��Ϣ�ӿ�

	//��������
public:
	//���캯��
	CPropertyBar(CWnd* pParent = NULL);
	//��������
	virtual ~CPropertyBar();

	//���ƺ���
public:
	//��������
	void CreateBar(CWnd *pParent);
	//���ٺ���
	void DestroyBar();

	//��������
public:
	//��ȡ�߶�
	int GetHeight();
	//������Դ
	void SetSkinResource(HINSTANCE ReshInstance,tagPropertyViewImage *pPropertyViewImage);
	//������Դ
	bool UpdateSkinResource();
	//���¿ؼ�
	void UpdateTopButton(INT nTopButtonHead);
	//���¿ؼ�
	void UpdateBtmButton(INT nBtmButtonHead);
	//�������
	void SetPurchaseInfoSink(IPurchaseInfo *pIPurchaseInfo);
	//��ʼ����
	void SetPropertyInfo(int nPropID, tagPropertyInfo const & PropertyInfo);
	//��ʼ����
	void SetFlowerInfo (int nFlowerID, tagFlowerInfo const & FlowerInfo);
	//ѡ���û�
	void SetUserDataInfo(tagUserData *pUserData);

	//��̬����
public:
	static CPropertyBar * GetInstance() { return m_pPropertyBar; }

	//�ڲ�����
protected:
	//�����ؼ�
	void RectifyControl(int nWidth, int nHeight);
	//�ʻ��¼�
	void FlowerEvent(UINT nButtonID);
	//�����¼�
	void PropertyEvent(int nPropertyID);

	//�̳к���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange* pDX);

	//��Ϣ����
public:
	//�滭����
	afx_msg void OnPaint();
	//��ʼ����
	virtual BOOL OnInitDialog();
	//���ڸ���
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//��д����
protected:
	//������Ϣ
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//�ƶ���Ϣ
private:
	afx_msg void OnBnClickedTopMoveToR();
	afx_msg void OnBnClickedTopMoveToL();
	afx_msg void OnBnClickedBtmMoveToR();
	afx_msg void OnBnClickedBtmMoveToL();

	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//��Ϣӳ��
	DECLARE_MESSAGE_MAP()
};
///////////////////////////////////////////////////////////////////////////////////
#endif
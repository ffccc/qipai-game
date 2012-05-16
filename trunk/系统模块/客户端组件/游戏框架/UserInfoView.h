#ifndef USER_INFO_VIEW_HEAD_FILE
#define USER_INFO_VIEW_HEAD_FILE

#pragma once

#include "StdAfx.h"
#include "GameFrame.h"

//////////////////////////////////////////////////////////////////////////

//�û���Ϣ
class CUserInfoView : public CWnd
{
	//�ӿڱ���
protected:
	IClientKernel *					m_pIClientKernel;					//�ں˽ӿ�
	tagUserData const*				m_pCurrentUserData;					//��ǰ�û�

	//���ָ��
private:
	IUserFaceRes					* m_pIUserFaceRes;					//ͷ��ӿ�

	//��Դ����
protected:
	CSkinImage						m_ImageMaleFace;					//ͷ��ͼƬ
	CSkinImage						m_ImageFemaleFace;					//ͷ��ͼƬ

	//�ؼ�����
protected:
	CSkinButton						m_btExchange;						//�һ���ť

	//��������
public:
	//���캯��
	CUserInfoView();
	//��������
	virtual ~CUserInfoView();

	//���ú���
public:
	//���ýӿ�
	bool SetUserFaceRes(IUnknownEx * pIUnknownEx);

	//���ܺ���
public:
	//������Դ
	bool UpdateSkinResource();
	//�����û�
	VOID SetUserViewInfo(tagUserData const* pCurrentUserData);
	//�����ں�
	void SetClientKernel(IClientKernel *pIClientKernel);
	//���º���
	void UpdateUserInfo();
	//��ȡ����
	tagUserData const *GetCurrentUser();

	//�ڲ�����
protected:
	//��������
	void RectifyControl(int nWidth, int nHeight);
	//���¿ؼ�
	void UpdateControls();

	//��Ϣӳ��
protected:
	//�滭����
	VOID OnPaint();
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�һ���Ϣ
	afx_msg void OnBtnClickedExchange();
	//���ڴ�С
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
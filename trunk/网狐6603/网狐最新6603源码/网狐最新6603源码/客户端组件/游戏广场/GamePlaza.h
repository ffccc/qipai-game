#pragma once

//ϵͳ�ļ�
#include "Stdafx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "PlatformEvent.h"
#include "ServerListData.h"

//�����ļ�
#include "Other\IDispimp.h"
#include "Other\Custsite.h"

//////////////////////////////////////////////////////////////////////////////////

//Ӧ�ó���
class CGamePlazaApp : public CWinApp
{
	//��������
public:
	CImpIDispatch *					m_pIDispatch;						//�Զ��ӿ�
	CCustomOccManager *				m_pCustomOccManager;				//�������

	//ƽ̨����
protected:
	CGlobalUnits					m_GlobalUnits;						//ȫ�ֵ�Ԫ
	CGlobalUserInfo					m_GlobalUserInfo;					//�û���Ϣ

	//ȫ�����
protected:
	CPlatformEvent					m_PlatformEvent;					//ƽ̨�¼�
	CParameterGlobal				m_ParameterGlobal;					//ȫ������
	CUserItemElement				m_UserItemElement;					//�û�Ԫ��
	CUserInformation				m_UserInformation;					//�û���Ϣ
	CExpressionManager				m_ExpressionManager;				//�������
	CCustomFaceManager				m_CustomFaceManager;				//ͷ�����
	CGamePropertyManager			m_GamePropertyManager;				//���߹���

	//��Դ���
public:
	CSkinRenderManager				m_SkinRenderManager;				//��Ⱦ����
	CSkinResourceManager			m_SkinResourceManager;				//��Դ����

	//�������
public:
	CFaceItemControlHelper			m_FaceItemControlModule;			//ͷ�����
	CUserOrderParserHelper			m_UserOrderParserModule;			//�ȼ����

	//��������
public:
	//���캯��
	CGamePlazaApp();

	//���غ���
public:
	//�˳�����
	virtual INT ExitInstance();
	//���뺯��
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//�������
extern CGamePlazaApp theApp;

//////////////////////////////////////////////////////////////////////////////////

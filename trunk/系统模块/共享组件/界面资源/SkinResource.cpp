#include "StdAfx.h"
#include "Resource.h"
#include "SkinResource.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CSkinResource::CSkinResource()
{
	m_cbResourceType=0;
}

//��������
CSkinResource::~CSkinResource()
{
}

//�ӿڲ�ѯ
VOID * __cdecl CSkinResource::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ISkinResource,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ISkinResource,Guid,dwQueryVer);
	return NULL;
}

//��������
bool __cdecl CSkinResource::CreateDefaultFont(CFont & Font)
{
	//�ͷŶ���
	ASSERT(Font.GetSafeHandle()==NULL);
	if (Font.GetSafeHandle()!=NULL) Font.DeleteObject();

	//��������
	Font.CreateFont(-12,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));
	ASSERT(Font.GetSafeHandle()!=NULL);

	return (Font.GetSafeHandle()!=NULL);
}

//��ȡ��Ϣ
bool __cdecl CSkinResource::GetResourceInfo(tagSkinEditResource & SkinEditResource)
{
	//��������
	HINSTANCE hInstance=GetModuleHandle(SKIN_RESOURCE_DLL_NAME);

	//��ɫ����
	SkinEditResource.crFocusTX=RGB(255,255,255);
	SkinEditResource.crFocusBK=RGB(0,128,250);
	SkinEditResource.crNoFocusTX=RGB(10,10,10);
	SkinEditResource.crNoFocusBK=RGB(255,255,255);
	SkinEditResource.crDisFocusTX=RGB(50,50,50);
	SkinEditResource.crDisFocusBK=RGB(125,125,125);

	return true;
}

//��ȡ��Ϣ
bool __cdecl CSkinResource::GetResourceInfo(tagSkinDialogResource & SkinDialogResource)
{
	//��������
	HINSTANCE hInstance=GetModuleHandle(SKIN_RESOURCE_DLL_NAME);

	switch(m_cbResourceType)
	{
	case RESOURCE_DEFAULT:
		{
			break;
		}
	case RESOURCE_BLACK:
		{
			break;
		}
	case RESOURCE_RED:
		{
			break;
		}
	}
	//�߿���Ϣ
	SkinDialogResource.crTitleText=RGB(200,200,200);
	SkinDialogResource.crControlText=RGB(0,0,0);
	SkinDialogResource.crBackGround=RGB(229,242,255);
	SkinDialogResource.crInsideBorder=RGB(85,132,174);
	SkinDialogResource.crBackFrame=RGB(229,242,255);

	//������Ϣ
	SkinDialogResource.LoadInfoImageTL.hResourceDLL=hInstance;
	SkinDialogResource.LoadInfoImageTM.hResourceDLL=hInstance;
	SkinDialogResource.LoadInfoImageTR.hResourceDLL=hInstance;
	SkinDialogResource.LoadInfoImageTL.uResourceID=DEF_IDB_SKIN_WND_TL;
	SkinDialogResource.LoadInfoImageTM.uResourceID=DEF_IDB_SKIN_WND_TM;
	SkinDialogResource.LoadInfoImageTR.uResourceID=DEF_IDB_SKIN_WND_TR;

	//��ť��Ϣ
	SkinDialogResource.LoadInfoButtonMin.hResourceDLL=hInstance;
	SkinDialogResource.LoadInfoButtonMax.hResourceDLL=hInstance;
	SkinDialogResource.LoadInfoButtonRes.hResourceDLL=hInstance;
	SkinDialogResource.LoadInfoButtonClose.hResourceDLL=hInstance;
	SkinDialogResource.LoadInfoButtonMin.uResourceID=DEF_IDB_SKIN_WND_MIN;
	SkinDialogResource.LoadInfoButtonMax.uResourceID=DEF_IDB_SKIN_WND_MAX;
	SkinDialogResource.LoadInfoButtonRes.uResourceID=DEF_IDB_SKIN_WND_RESORE;
	SkinDialogResource.LoadInfoButtonClose.uResourceID=DEF_IDB_SKIN_WND_CLOSE;

	return true;
}

//��ȡ��Ϣ
bool __cdecl CSkinResource::GetResourceInfo(tagSkinButtonResource & SkinButtonResource)
{
	//��������
	HINSTANCE hInstance=GetModuleHandle(SKIN_RESOURCE_DLL_NAME);

	switch(m_cbResourceType)
	{
	case RESOURCE_DEFAULT:
		{
			break;
		}
	case RESOURCE_BLACK:
		{
			break;
		}
	case RESOURCE_RED:
		{
			break;
		}
	}
	//���ñ���
	SkinButtonResource.crButtonText=RGB(10,10,10);
	SkinButtonResource.LoadInfoButton.hResourceDLL=hInstance;
	SkinButtonResource.LoadInfoButton.uResourceID=DEF_IDB_SKIN_BUTTON_BACK;

	return true;
}

//��ȡ��Ϣ
bool __cdecl CSkinResource::GetResourceInfo(tagSkinSplitterResource & SkinSplitterResource)
{
	//��������
	HINSTANCE hInstance=GetModuleHandle(SKIN_RESOURCE_DLL_NAME);

	switch(m_cbResourceType)
	{
	case RESOURCE_DEFAULT:
		{
			break;
		}
	case RESOURCE_BLACK:
		{
			break;
		}
	case RESOURCE_RED:
		{
			break;
		}
	}
	//������Ϣ
	SkinSplitterResource.LoadInfoVor.hResourceDLL=hInstance;
	SkinSplitterResource.LoadInfoHor.hResourceDLL=hInstance;
	SkinSplitterResource.LoadInfoVor.uResourceID=DEF_IDB_SKIN_SPLITTER_VOR;
	SkinSplitterResource.LoadInfoHor.uResourceID=DEF_IDB_SKIN_SPLITTER_HOR;

	return true;
}

//��ȡ��Ϣ
bool __cdecl CSkinResource::GetResourceInfo(tagSkinListCtrlResource & SkinListCtrlResource)
{
	//��������
	HINSTANCE hInstance=GetModuleHandle(SKIN_RESOURCE_DLL_NAME);

	switch(m_cbResourceType)
	{
	case RESOURCE_DEFAULT:
		{
			break;
		}
	case RESOURCE_BLACK:
		{
			break;
		}
	case RESOURCE_RED:
		{
			break;
		}
	}
	//��ɫ����
	SkinListCtrlResource.crListTX=RGB(0,18,87);

	//��Դ��Ϣ
	SkinListCtrlResource.LoadInfoImageBack.hResourceDLL=hInstance;
	SkinListCtrlResource.LoadInfoImageBack.uResourceID=DEF_IDB_SKIN_HEAD_TITLE;

	return true;
}

//��ȡ��Ϣ
bool __cdecl CSkinResource::GetResourceInfo(tagSkinHyperLinkResource & SkinHyperLinkResource)
{
	switch(m_cbResourceType)
	{
	case RESOURCE_DEFAULT:
		{
			break;
		}
	case RESOURCE_BLACK:
		{
			break;
		}
	case RESOURCE_RED:
		{
			break;
		}
	}
	//���ñ���
	SkinHyperLinkResource.crHoverText=RGB(220,0,0);
	SkinHyperLinkResource.crNormalText=RGB(0,0,230);
	SkinHyperLinkResource.crVisitedText=RGB(0,0,100);
	SkinHyperLinkResource.crBackColor=RGB(229,242,255);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////

//�����������
DECLARE_CREATE_MODULE(SkinResource);

//SkinButtonResource.LoadInfoButton.pszResource=MAKEINTRESOURCE(DEF_IDB_SKIN_BUTTON_BACK);
//////////////////////////////////////////////////////////////////////////////////////

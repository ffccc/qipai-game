#ifndef SKIN_RESOURCE_HEAD_FILE
#define SKIN_RESOURCE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "SkinResourceModule.h"

//////////////////////////////////////////////////////////////////////////

//��Դ������
class CSkinResource : public ISkinResource
{
	//Ƥ������
public:
	BYTE						m_cbResourceType;						//��Դ����

	//��������
public:
	//���캯��
	CSkinResource();
	//��������
	virtual ~CSkinResource();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID __cdecl Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * __cdecl QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//��Դ�ӿ�
public:
	//��������
	virtual bool __cdecl CreateDefaultFont(CFont & Font);

	//��Դ�ӿ�
public:
	//��ȡ��Ϣ
	virtual bool __cdecl GetResourceInfo(tagSkinEditResource & SkinEditResource);
	//��ȡ��Ϣ
	virtual bool __cdecl GetResourceInfo(tagSkinDialogResource & SkinDialogResource);
	//��ȡ��Ϣ
	virtual bool __cdecl GetResourceInfo(tagSkinButtonResource & SkinButtonResource);
	//��ȡ��Ϣ
	virtual bool __cdecl GetResourceInfo(tagSkinSplitterResource & SkinSplitterResource);
	//��ȡ��Ϣ
	virtual bool __cdecl GetResourceInfo(tagSkinListCtrlResource & SkinListCtrlResource);
	//��ȡ��Ϣ
	virtual bool __cdecl GetResourceInfo(tagSkinHyperLinkResource & SkinHyperLinkResource);
};

//////////////////////////////////////////////////////////////////////////

#endif
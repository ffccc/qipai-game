#ifndef PLATFORM_RESOURCE_HEAD_FILE
#define PLATFORM_RESOURCE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "SkinResource.h"
#include "SkinResourceModule.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ��Դ
class CPlatformResource : public IPlatformResource
{
	//��Դ����
protected:
	tagPlazaViewImage				*m_pPlazaViewImage ;
	tagServerViewImage				*m_pServerViewImage ;
	tagPlatformFrameImage			*m_pPlatformFrameImage;
	tagWhisperViewImage				*m_pWhisperViewImage ;
	tagControlViewImage				*m_pControlViewImage ;
	tagPropertyViewImage			*m_pPropertyViewImage ;
	tagChatExpViewImage				*m_pChatExpViewImage;
	tagGameFrameImage				*m_pGameFrameImage;
	tagVideoFrameImage				*m_pVideoFrameImage ;
	tagPropertyMenuImage			*m_pPropertyMenuImage ;
	tagGiftFlashImage				*m_pGiftFlashImage ;


	//Ƥ������
public:
	BYTE							m_cbResourceType;					//��Դ����

	//�ں˱���
protected:
	HINSTANCE						m_hInstance;						//��Դ���
	CSkinResource					m_SkinResource;						//������Դ

	//��������
public:
	//���캯��
	CPlatformResource();
	//��������
	virtual ~CPlatformResource();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * __cdecl QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//�ں˽ӿ�
public:
	//��Դ���
	virtual HINSTANCE __cdecl GetResInstance();
	//��ȡԤ��
	virtual bool __cdecl GetSkinPreview(CImage & ImagePreview);

	//��Դ����
public:
	//��Դ����
	virtual void __cdecl SetResourceType(BYTE cbResourceType);

	//������Դ
public:
	//������Դ
	virtual VOID * __cdecl GetSkinResource(REFGUID Guid, DWORD dwQueryVer);

	//������Դ
public:
	//�㳡��Դ
	virtual bool __cdecl GetPlazaViewImage(tagPlazaViewImage * PlazaViewImage);
	//������Դ
	virtual bool __cdecl GetServerViewImage(tagServerViewImage * ServerViewImage);
	//�����Դ
	virtual bool __cdecl GetPlatformFrameImage(tagPlatformFrameImage * PlatformFrameImage);

	//������Դ
public:
	//˽����Դ
	virtual bool __cdecl GetWhisperViewImage(tagWhisperViewImage * WhisperViewImage);
	//�ؼ���Դ
	virtual bool __cdecl GetControlViewImage(tagControlViewImage * ControlViewImage);
	//������Դ
	virtual bool __cdecl GetPropertyViewImage(tagPropertyViewImage * PropertyViewImage);
	//������Դ
	virtual bool __cdecl GetChatExpViewImage(tagChatExpViewImage * ChatExpViewImage);
	//��Ϸ���
	virtual bool __cdecl GetGameFrameImage(tagGameFrameImage * GameFrameImage);
	//��Ƶ��Դ
	virtual bool __cdecl GetVideoFrameImage(tagVideoFrameImage * VideoFrameImage);
	//���߲˵�
	virtual bool __cdecl GetPropertyMenuImage(tagPropertyMenuImage * PropertyMenuImage);
	//������Դ
	virtual bool __cdecl GetGiftFlashImage(tagGiftFlashImage * GiftFlashImage);
};

//////////////////////////////////////////////////////////////////////////

#endif
#ifndef FACE_ITEM_CONTROL_HEAD_FILE
#define FACE_ITEM_CONTROL_HEAD_FILE

#pragma once

#include "AvatarControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//ͷ��ؼ�
class AVATAR_CONTROL_CLASS CFaceItemControl : public IFaceItemControl
{
	//��������
protected:
	WORD							m_wLineCount;						//ͷ����Ŀ
	WORD							m_wFullCount;						//ͷ����Ŀ
	CPngImage						m_ImageUserFace;					//�û�ͷ��

	//ͷ����
protected:
	CSize							m_SizeItemFrame;					//��ܴ�С
	CPngImage						m_ImageItemFrame;					//ͷ����

	//����ӿ�
protected:
	static CFaceItemControl *		m_pFaceItemControl;					//ͷ��ӿ�

	//��������
public:
	//���캯��
	CFaceItemControl();
	//��������
	virtual ~CFaceItemControl();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//��Ϣ�ӿ�
public:
	//ͷ����Ŀ
	virtual WORD GetFaceCount();
	//��ȡ�ߴ�
	virtual VOID GetFaceItemSize(CSize & SizeFace);

	//��Դ�ӿ�
public:
	//��ȡ��Դ
	virtual VOID GetAvatarResource(tagAvatarResource & AvatarResource);
	//�滭���
	virtual VOID DrawFaceItemFrame(CDC * pDC, INT nXPos, INT nYPos, BYTE cbFrameKind);

	//ϵͳͷ��
public:
	//�滭ͷ��
	virtual VOID DrawFaceNormal(CDC * pDC, INT nXPos, INT nYPos, WORD wFaceID);
	//�滭ͷ��
	virtual VOID DrawFaceOffLine(CDC * pDC, INT nXPos, INT nYPos, WORD wFaceID);

	//�Զ�ͷ��
public:
	//�滭ͷ��
	virtual VOID DrawFaceNormal(CDC * pDC, INT nXPos, INT nYPos, DWORD dwCustomFace[FACE_CX*FACE_CY]);
	//�滭ͷ��
	virtual VOID DrawFaceOffLine(CDC * pDC, INT nXPos, INT nYPos, DWORD dwCustomFace[FACE_CX*FACE_CY]);

	//ϵͳͷ��
public:
	//�滭ͷ��
	virtual VOID DrawFaceNormal(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight, WORD wFaceID);
	//�滭ͷ��
	virtual VOID DrawFaceOffLine(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight, WORD wFaceID);

	//��̬����
public:
	//��ȡ����
	static CFaceItemControl * GetInstance() { return m_pFaceItemControl; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif
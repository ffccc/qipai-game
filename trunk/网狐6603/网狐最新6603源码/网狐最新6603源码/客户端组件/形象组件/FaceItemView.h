#ifndef FACE_ITEM_VIEW_HEAD_FILE
#define FACE_ITEM_VIEW_HEAD_FILE

#pragma once

#include "AvatarControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//ͷ�񴰿�
class AVATAR_CONTROL_CLASS CFaceItemView : public CStatic
{
	//��������
protected:
	WORD							m_wFaceID;							//ͷ���ʶ
	DWORD							m_dwCustonID;						//�Զ���ʶ
	DWORD							m_dwCustomFace[FACE_CX*FACE_CY];	//ͼƬ��Ϣ

	//��ɫ����
protected:
	COLORREF						m_rcBackGround;						//������ɫ

	//��������
public:
	//���캯��
	CFaceItemView();
	//��������
	virtual ~CFaceItemView();

	//���ú���
public:
	//������ɫ
	VOID SetControlColor(COLORREF rcBackGround);
	
	//ͷ���ʶ
public:
	//����ͷ��
	VOID SetSystemFace(WORD wFaceID);
	//����ͷ��
	VOID SetCustomFace(DWORD dwCustonID, DWORD dwCustomFace[FACE_CX*FACE_CY]);

	//��Ϣ����
protected:
	//�ػ�����
	VOID OnPaint();
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif
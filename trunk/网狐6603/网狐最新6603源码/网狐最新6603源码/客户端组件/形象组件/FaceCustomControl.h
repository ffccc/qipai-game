#ifndef FACE_CUSTOM_CONTROL_HEAD_FILE
#define FACE_CUSTOM_CONTROL_HEAD_FILE

#pragma once

#include "FaceItemControl.h"
#include "AvatarControlHead.h"
#include "ImageEditorControl.h"

//////////////////////////////////////////////////////////////////////////////////

//�Զ�ͷ��
class AVATAR_CONTROL_CLASS CFaceItemCustomWnd : public CWnd
{
	//���ư�ť
protected:
	CSkinButton			 			m_btTurnLeft;						//��ת��ť		
	CSkinButton			 			m_btTurnRight;						//��ת��ť
	CSkinButton			 			m_btShowVLine;						//���߰�ť

	//���ư�ť
protected:
	CSkinButton			 			m_btLoadImage;						//����ͼƬ
	CSkinButton			 			m_btLoadCamera;						//��������

	//�ؼ�����
protected:
	CImageEditorControl    			m_ImageEditorControl;				//�༭�ؼ�

	//��������
public:
	//���캯��
	CFaceItemCustomWnd();
	//��������
	virtual ~CFaceItemCustomWnd();

	//���ú���
public:
	//���ýӿ�
	VOID SetImageEditorEvent(IImageEditorEvent * pIImageEditorEvent);

	//ͼƬ����
public:
	//�����ж�
	bool IsNull();
	//����ͼƬ
	VOID DestoryImage();
	//��ȡͼƬ
	VOID DrawEditImage(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//��Ϣӳ��
protected:
	//�滭��Ϣ
	VOID OnPaint();
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);

	//��ť��Ϣ
protected:
	//��תͼ��
	VOID OnBnClickedTurnLeft();
	//��תͼ��
	VOID OnBnClickedTurnRight();
	//��ʾ����
	VOID OnBnClickedShowVLine();
	//����ͼ��
	VOID OnBnClickedLoadImage();
	//����ͼ��
	VOID OnBnClickedLoadCamera();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif
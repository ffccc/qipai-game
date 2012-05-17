#ifndef SKIN_TAB_CTRL_HEAD_FILE
#define SKIN_TAB_CTRL_HEAD_FILE

#pragma once

#include "SkinControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//ѡ��ؼ�
class SKIN_CONTROL_CLASS CSkinTabCtrl : public CTabCtrl
{
	//״̬����
protected:
	bool							m_bRenderImage;						//��Ⱦ����

	//��������
protected:
	COLORREF						m_crBackGround;						//������ɫ
	COLORREF						m_crControlBorder;					//�߿���ɫ

	//��������
public:
	//���캯��
	CSkinTabCtrl();
	//��������
	virtual ~CSkinTabCtrl();

	//���ܺ���
public:
	//������Ⱦ
	VOID SetRenderImage(bool bRenderImage);
	//������ɫ
	VOID SetTabCtrlColor(COLORREF crBackGround, COLORREF crControlBorder);

	//��Ϣ����
protected:
	//�ػ�����	
	VOID OnPaint();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif
#ifndef IMAGE_EDITOR_CONTROL_HEAD_FILE
#define IMAGE_EDITOR_CONTROL_HEAD_FILE

#pragma once

#include "CameraControl.h"
#include "AvatarControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��ת����
#define ROUND_TYPE_LEFT				0									//������ת
#define ROUND_TYPE_RIGHT			1									//������ת

//////////////////////////////////////////////////////////////////////////////////

//�ӿڶ���
interface IImageEditorEvent
{
	//ͼ���¼�
	virtual VOID OnEventImageSelect(bool bLoadImage, bool bOperateing)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//�༭�ؼ�
class AVATAR_CONTROL_CLASS CImageEditorControl : public CWnd
{
	//��������
protected:
	BYTE							m_cbHitTest;						//������
	CPoint							m_ptLockPoint;						//����λ��
	CPoint							m_ptDragOrigin;						//����ԭ��

	//״̬����
protected:
	bool							m_bVShowLine;						//�߿�ģʽ
	BYTE							m_cbConsultType;					//������ʽ
	BYTE							m_cbOperateType;					//��������
	CRect							m_rcImageSelect;					//ѡ������

	//״̬����
protected:
	CSize							m_SizeSource;						//ԭͼ��С
	CSize							m_SizeTarget;						//Ŀ���С
	DOUBLE							m_dZoomPercent;						//���ű���

	//�ں˱���
protected:
	CImage							m_ImageSource;						//ԭͼͼƬ
	IImageEditorEvent *				m_pIImageEditorEvent;				//�ص��ӿ�

	//��������
public:
	//���캯��
	CImageEditorControl();
	//��������
	virtual ~CImageEditorControl();

	//���ܺ���
public:
	//����ͼƬ
	bool DestoryImage();
	//����ͼƬ
	bool LoadEditImage(CImage & Image);
	//����ͼƬ
	bool LoadEditImage(LPCTSTR pszImage);

	//���ƺ���
public:
	//�����ж�
	bool IsNull();
	//��תͼƬ
	VOID SetRoundImage(BYTE cbRoundType);
	//��ȡͼƬ
	VOID DrawEditImage(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//���ú���
public:
	//���ýӿ�
	VOID SetImageEditorEvent(IImageEditorEvent * pIImageEditorEvent) { m_pIImageEditorEvent=pIImageEditorEvent; }

	//���߿���
public:
	//��������
	VOID SetVLineStatus(bool bShowVLine);
	//����״̬
	bool GetVLineStatus() { return m_bVShowLine; }

	//��������
protected:
	//�滭����
	VOID DrawLineFrame(CDC * pDC);
	//�������
	BYTE OnEventHitTest(CPoint & Point);

	//��Ϣ��Ӧ
public:
	//���Ʊ���
	BOOL OnEraseBkgnd(CDC * pDC);
	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint MousePoint);
	//�������
	VOID OnLButtonUp(UINT nFlags, CPoint MousePoint);
	//�������
	VOID OnLButtonDown(UINT nFlags, CPoint MousePoint);
	//���ù��
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif


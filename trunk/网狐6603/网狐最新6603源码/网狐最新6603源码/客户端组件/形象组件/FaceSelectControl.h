#ifndef FACE_SELECT_CONTRIL_HEAD_FILE
#define FACE_SELECT_CONTRIL_HEAD_FILE

#pragma once

#include "FaceItemControl.h"
#include "AvatarControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//�ӿڶ���
interface IFaceItemSelectEvent
{
	//ͼ���¼�
	virtual VOID OnEventFaceSelect(WORD wFaceID)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//ͷ��ѡ��
class AVATAR_CONTROL_CLASS CFaceItemSelectWnd : public CWnd
{
	//������Ϣ
protected:
	INT								m_nMaxPels;							//�������
	INT								m_nPagePels;						//ҳ������
	INT								m_nLineCount;						//������Ŀ
	INT								m_nWidthCount;						//������Ŀ
	INT								m_nCurrentPos;						//��ǰλ��
	INT								m_nXExcursion;						//����ƫ��

	//ͷ�����
protected:
	WORD							m_wFaceCount;						//ͷ����Ŀ
	WORD							m_wSelectFace;						//ѡ������

	//�������
protected:
	CSkinScrollBar					m_SkinScrollBar;					//�����ؼ�
	IFaceItemSelectEvent *			m_pIFaceItemSelectEvent;			//�ص��ӿ�

	//��������
public:
	//���캯��
	CFaceItemSelectWnd();
	//��������
	virtual ~CFaceItemSelectWnd();

	//ѡ����
public:
	//ѡ����Ŀ
	VOID SetAllowItemFull();
	//ѡ����Ŀ
	VOID SetAllowItemCount(WORD wFaceCount);

	//���ܺ���
public:
	//��ȡѡ��
	WORD GetSelectFace() { return m_wSelectFace; }
	//���ýӿ�
	VOID SetFaceItemSelectEvent(IFaceItemSelectEvent * pIFaceItemSelectEvent) { m_pIFaceItemSelectEvent=pIFaceItemSelectEvent; }

	//��������
protected:
	//��������
	WORD MouseHitTest(CPoint MousePoint);
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);
	//�滭ͷ��
	VOID DrawFaceItem(CDC * pDC, WORD wIndex, INT nXPos, INT nYPos, bool bHover);

	//��Ϣӳ��
protected:
	//�滭��Ϣ
	VOID OnPaint();
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//������Ϣ
	VOID OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//������Ϣ
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint Point);
	//������Ϣ
	VOID OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//ͷ��ѡ��
class CFaceSelectControl : public CSkinDialog, public IFaceSelectControl, public IFaceItemSelectEvent
{
	//�������
protected:
	CFaceItemSelectWnd				m_FaceItemSelectWnd;				//ѡ�񴰿�

	//��������
public:
	//���캯��
	CFaceSelectControl();
	//��������
	virtual ~CFaceSelectControl();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//����ѡ��
public:
	//ѡ����Ŀ
	virtual VOID SetAllowItemFull() { m_FaceItemSelectWnd.SetAllowItemFull(); }
	//ѡ����Ŀ
	virtual VOID SetAllowItemCount(WORD wFaceCount) { m_FaceItemSelectWnd.SetAllowItemCount(wFaceCount); }

	//��ȡѡ��
public:
	//��ȡѡ��
	virtual bool GetSelectFaceID(WORD & wFaceID);

	//�༭�ӿ�
public:
	//ͼ���¼�
	virtual VOID OnEventFaceSelect(WORD wFaceID);

	//���غ���
public:
	//���ú���
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif
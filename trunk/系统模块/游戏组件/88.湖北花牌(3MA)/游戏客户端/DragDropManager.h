#pragma once

#include "CardControl.h"

extern CCardResource g_CardResource;

//////////////////
// ע����Ϸŵ���Ϣ
extern const UINT WM_DD_DRAGENTER; // ��ʼ��
extern const UINT WM_DD_DRAGOVER;  // ������
extern const UINT WM_DD_DRAGDROP;  // ��
extern const UINT WM_DD_DRAGABORT; // ������

// ���崰�ڵ�����
const UINT DDW_SOURCE = 0x01;   // �Ϸŵ�Դ����
const UINT DDW_TARGET = 0x02;	// �Ϸŵ�Ŀ�괰��

///////////////////////////////////////////////
// �����ϷŶ���
class CDragDropData {
protected:
	CBitmap			m_bitmap;	   // ���ڻ�ͼ��λͼ

	BYTE            m_cbCardData;  //�����Ƶ�����
	//���캯������������
public:
	CDragDropData(BYTE cbCardData) { m_cbCardData = cbCardData; }
	~CDragDropData() { }

	BYTE GetDragData() { return m_cbCardData; }

	//����һ��λͼ�б�
	CImageList* CreateDragImage(CWnd* pWnd, CRect& rc);

	//���ؼ��Ϸ�ʱ��ʾͼƬ�Ĵ�С
	CSize OnGetDragSize(CDC& dc);

	//ʵ�־���ͼƬ����ʾ
	void  OnDrawData(CDC& dc, CRect& rc);
};
/////////////////////////////////////////////////////////////
//�����Ϸ���Ϣ�Ľṹ��
struct DRAGDROPINFO {
	HWND hwndSource;		// Դ����
	HWND hwndTarget;		// Ŀ�괰��
	CDragDropData* data;	// ���Ϸŵ�����
	POINT pt;				// ��ǰ�Ĺ�����ڵĵ�(�ͻ���������)
};
class CDragDropManager
{
protected:
	//�����ϷŹ�������״̬
	enum { NONE=0, CAPTURED, DRAGGING };
	CWnd*			m_pMainWnd;				 // ������
	HWND            m_hWnd[9];               // ����9���������Ĵ��ھ��
	HCURSOR			m_hCursorDrop;			 // ��ʱ�Ĺ��
	HCURSOR			m_hCursorNo;			 // û��ʱ�Ĺ��

	DRAGDROPINFO	m_info;					 // �Ϸ�ʱ������
	UINT			m_iState;				 // �ϷŹ�����=������״̬
	HWND			m_hwndTracking;		     // Դ����
	CPoint			m_ptOrg;				 // ��ʼ�ϵ���ʼ��
	CImageList*		m_pDragImage;			 // ��ʾ����ʱ��ͼ���б�
	HCURSOR			m_hCursorSave;			 // ����Ĺ��

public:
	CDragDropManager(void);
	~CDragDropManager(void);

	//���������Ϣ
	BOOL OnLButtonDown(const MSG& msg);
	BOOL OnMouseMove(const MSG& msg);
	BOOL OnLButtonUp(const MSG& msg);

	void SetState(UINT iState);
	BOOL IsCaptured() { return m_iState>=CAPTURED; }
	BOOL IsDragging() { return m_iState>=DRAGGING; }
	BOOL IsDragDropWnd(HWND hWnd);

	void SetCursors(HCURSOR hCursorDrop, HCURSOR hCursorNo)
	{
		m_hCursorDrop = hCursorDrop;
		m_hCursorNo = hCursorNo;
	}

	void InitDragDropManager(CWnd *pMainWnd,HWND hWnd[9]);

	BOOL PreTranslateMessage(MSG* pMsg);
};

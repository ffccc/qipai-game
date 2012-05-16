#ifndef USER_FACE_RES_HEAD_FILE
#define USER_FACE_RES_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include "AtlImage.h"
#include "UserFace.h"

//�û�ͷ����Դ��
class CUserFaceRes : public IUserFaceRes
{
	//��������
protected:
	TCHAR								m_szDirWork[MAX_PATH];			//����Ŀ¼
	WORD								m_wCount;						//ͷ����Ŀ
	CSize								m_NormalSize;					//ͷ���С
	CImage								m_NormalFace;					//��ͨͷ��

	//��������
public:
	//���캯��
	CUserFaceRes(void);
	//��������
	virtual ~CUserFaceRes(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//�ӿں���
public:
	//ͷ����Ŀ
	virtual WORD __cdecl GetFaceCount() { return m_wCount; }
	//��ȡ��С
	virtual const CSize & __cdecl GetNormalFaceSize() { return m_NormalSize; }

	//���ܺ���
public:
	//����б�
	virtual WORD __cdecl FillImageList(CImageList & ImageList);
	//�滭ͷ��
	virtual void __cdecl DrawNormalFace(CDC * pDC, int nXPos, int nYPos, WORD wFaceID, DWORD dwUserID,DWORD dwCustomFace);
	//�滭ͷ��
	virtual void __cdecl DrawCustomBigFace(CDC * pDC, int nXPos, int nYPos, DWORD dwUserID,DWORD dwCustomFace,UINT nWidth=BIG_FACE_WIDTH,UINT nHeight=BIG_FACE_HEIGHT);

	//�ڲ�����
protected:
	//͸���滭
	bool AlphaDrawImage(CDC * pDestDC, int xDest, int yDest, int cxDest, int cyDest, CDC * pScrDC, int xSrc, int ySrc);
};

//////////////////////////////////////////////////////////////////////////

#endif
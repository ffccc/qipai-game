#ifndef FACTORY_HEAD_FILE
#define FACTORY_HEAD_FILE

#pragma once

#include "Array.h"

//////////////////////////////////////////////////////////////////////////

//�궨��
#define INFINITY_COUNT				-1									//������Ŀ

//����󹤳�
template <class TYPE> class CFactoryTemplate
{
	//��˵��
	typedef CArrayTemplate<TYPE *> CItemPtrArray;

	//��������
protected:
	INT_PTR							m_nMaxCount;						//�����Ŀ
	CItemPtrArray					m_FreeItem;							//���ж���
	CItemPtrArray					m_ActiveItem;						//�����
	CItemPtrArray					m_StorageItem;						//�洢����

	//��������
public:
	//���캯��
	CFactoryTemplate(void);
	//��������
	virtual ~CFactoryTemplate(void);

	//��Ϣ����
public:
	//�����Ŀ
	INT_PTR GetMaxCount();
	//������Ŀ
	INT_PTR GetFreeCount();
	//���Ŀ
	INT_PTR GetActiveCount();
	//�洢��Ŀ
	INT_PTR GetStorageCount();
	//ö�ٶ���
	TYPE * EnumActiveObject(INT_PTR nIndex);
	//ö�ٶ���
	TYPE * EnumStorageObject(INT_PTR nIndex);

	//���ƺ���
public:
	//���ƿ���
	void CopyFreeItem(CItemPtrArray & ItemPtrArray);
	//���ƻ
	void CopyActiveItem(CItemPtrArray & ItemPtrArray);
	//���ƴ洢
	void CopyStorageItem(CItemPtrArray & ItemPtrArray);

	//������
public:
	//��������
	TYPE * ActiveItem();
	//ɾ������
	bool FreeItem(TYPE * pObject);
	//ɾ������
	void FreeAllItem();
	//�ͷ��ڴ�
	void FreeExtra();
	//������Ŀ
	void SetMaxCount(INT_PTR nMaxCount=INFINITY_COUNT);
};

//////////////////////////////////////////////////////////////////////////
// CFactoryTemplate<TYPE> ��������

//�����Ŀ
template<class TYPE> AFX_INLINE INT_PTR CFactoryTemplate<TYPE>::GetMaxCount()
{
	return m_nMaxCount;
}

//������Ŀ
template<class TYPE> AFX_INLINE INT_PTR CFactoryTemplate<TYPE>::GetFreeCount()
{
	return m_FreeItem.GetCount();
}

//���Ŀ
template<class TYPE> AFX_INLINE INT_PTR CFactoryTemplate<TYPE>::GetActiveCount()
{
	return m_ActiveItem.GetCount();
}

//�洢��Ŀ
template<class TYPE> AFX_INLINE INT_PTR CFactoryTemplate<TYPE>::GetStorageCount()
{
	return m_StorageItem.GetCount();
}

//������Ŀ
template<class TYPE> AFX_INLINE void CFactoryTemplate<TYPE>::SetMaxCount(INT_PTR nMaxCount)
{
	m_nMaxCount=nMaxCount;
}

//���ƿ���
template<class TYPE> AFX_INLINE void CFactoryTemplate<TYPE>::CopyFreeItem(CItemPtrArray & ItemPtrArray)
{
	ItemPtrArray.Copy(m_FreeItem);
	return;
}

//���ƻ
template<class TYPE> AFX_INLINE void CFactoryTemplate<TYPE>::CopyActiveItem(CItemPtrArray & ItemPtrArray)
{
	ItemPtrArray.Copy(m_ActiveItem);
	return;
}

//���ƴ洢
template<class TYPE> AFX_INLINE void CFactoryTemplate<TYPE>::CopyStorageItem(CItemPtrArray & ItemPtrArray)
{
	ItemPtrArray.Copy(m_StorageItem);
	return;
}

//////////////////////////////////////////////////////////////////////////
// CFactoryTemplate<TYPE> ��������

//���캯��
template<class TYPE> CFactoryTemplate<TYPE>::CFactoryTemplate()
{
	m_nMaxCount=INFINITY_COUNT;
	return;
}

//���캯��
template<class TYPE> CFactoryTemplate<TYPE>::~CFactoryTemplate()
{
	//�������
	TYPE * pObject=NULL;
	for (INT_PTR i=0;i<m_StorageItem.GetCount();i++)
	{
		pObject=m_StorageItem[i];
		ASSERT(pObject!=NULL);
		SafeDelete(pObject);
	}
	m_FreeItem.RemoveAll();
	m_ActiveItem.RemoveAll();
	m_StorageItem.RemoveAll();

	return;
}

//��������
template<class TYPE> TYPE * CFactoryTemplate<TYPE>::ActiveItem()
{
	//��ȡ���ж���
	TYPE * pObject=NULL;
	if (m_FreeItem.GetCount()>0)
	{
		INT_PTR nItemPostion=m_FreeItem.GetCount()-1;
		pObject=m_FreeItem[nItemPostion];
		ASSERT(pObject!=NULL);
		m_FreeItem.RemoveAt(nItemPostion);
		m_ActiveItem.Add(pObject);
	}

	//�����¶���
	if (pObject==NULL)
	{
		if ((m_nMaxCount==INFINITY_COUNT)||(m_StorageItem.GetCount()<m_nMaxCount))
		{
			try
			{
				pObject=new TYPE;
				if (pObject==NULL) return NULL;
				m_StorageItem.Add(pObject);
				m_ActiveItem.Add(pObject);
			}
			catch (...) { return NULL; }
		}
	}

	return pObject;
}

//ɾ������
template<class TYPE> void CFactoryTemplate<TYPE>::FreeAllItem()
{
	m_FreeItem.Append(m_ActiveItem);
	m_ActiveItem.RemoveAll();
	return;
}

//�ͷ��ڴ�
template<class TYPE> void CFactoryTemplate<TYPE>::FreeExtra()
{
	TYPE * pObject=NULL;
	for (INT_PTR i=0;i<m_StorageItem.GetCount();i++)
	{
		pObject=m_StorageItem[i];
		ASSERT(pObject!=NULL);
		delete pObject;
	}
	m_StorageItem.RemoveAll();

	return;
}

//ɾ������
template<class TYPE> bool CFactoryTemplate<TYPE>::FreeItem(TYPE * pObject)
{
	//Ч�����
	ASSERT(pObject!=NULL);

	//�ͷŶ���
	INT_PTR nActiveCount=m_ActiveItem.GetCount();
	for (INT_PTR i=0;i<nActiveCount;i++)
	{
		if (pObject==m_ActiveItem[i])
		{
			m_ActiveItem.RemoveAt(i);
			m_FreeItem.Add(pObject);
			return true;
		}
	}

	//ɾ��ʧ��
	ASSERT(FALSE);
	return false;
}

//ö�ٶ���
template<class TYPE> TYPE * CFactoryTemplate<TYPE>::EnumActiveObject(INT_PTR nIndex)
{
	//Ч�����
	ASSERT(nIndex>=0);
	if (nIndex<0) return NULL;

	//��ȡ����
	if (nIndex<m_ActiveItem.GetCount())
	{
		TYPE * pObject=m_ActiveItem[nIndex];
		ASSERT(pObject!=NULL);
		return pObject;
	}

	return NULL;
}

//ö�ٶ���
template<class TYPE> TYPE * CFactoryTemplate<TYPE>::EnumStorageObject(INT_PTR nIndex)
{
	//Ч�����
	ASSERT(nIndex>=0);
	if (nIndex<0) return NULL;

	//��ȡ����
	if (nIndex<m_StorageItem.GetCount())
	{
		TYPE * pObject=m_StorageItem[nIndex];
		ASSERT(pObject!=NULL);
		return pObject;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////

#endif
#include "StdAfx.h"
#include "ModuleListControl.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CModuleListControl::CModuleListControl()
{
}

//��������
CModuleListControl::~CModuleListControl()
{
}

//�����б�
VOID CModuleListControl::InitListControl()
{
	//��������
	INT nColIndex=0;

	//�����б�
	InsertColumn(nColIndex++,TEXT("��Ϸ����"),LVCFMT_CENTER,80);
	InsertColumn(nColIndex++,TEXT("���״̬"),LVCFMT_CENTER,80);
	InsertColumn(nColIndex++,TEXT("���ݿ���"),LVCFMT_LEFT,100);
	InsertColumn(nColIndex++,TEXT("���ӵ�ַ"),LVCFMT_LEFT,100);
	InsertColumn(nColIndex++,TEXT("��������"),LVCFMT_LEFT,140);
	InsertColumn(nColIndex++,TEXT("�ͻ�����"),LVCFMT_LEFT,120);

	return;
}

//��������
INT CModuleListControl::SortListItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex)
{
	//��������
	tagGameModuleInfo * pGameModuleInfo1=(tagGameModuleInfo *)lParam1;
	tagGameModuleInfo * pGameModuleInfo2=(tagGameModuleInfo *)lParam2;

	//��װ����
	bool bInstall1=(pGameModuleInfo1->dwNativeVersion!=0L);
	bool bInstall2=(pGameModuleInfo2->dwNativeVersion!=0L);
	if (bInstall1!=bInstall2) return (bInstall1==true)?1:-1;

	//��������
	switch (wColumnIndex)
	{
	case 0:		//��Ϸ����
		{
			return lstrcmp(pGameModuleInfo1->szGameName,pGameModuleInfo2->szGameName);
		}
	case 1:		//���״̬
		{
			//���״̬
			BYTE cbStatus1=0;
			if (pGameModuleInfo1->dwNativeVersion!=0)
			{
				cbStatus1=(pGameModuleInfo1->dwNativeVersion==pGameModuleInfo1->dwServerVersion)?2:1;
			}

			//���״̬
			BYTE cbStatus2=0;
			if (pGameModuleInfo2->dwNativeVersion!=0)
			{
				cbStatus2=(pGameModuleInfo2->dwNativeVersion==pGameModuleInfo2->dwServerVersion)?2:1;
			}

			return (cbStatus1>cbStatus2)?SORT_POSITION_AFTER:SORT_POSITION_FRONT;
		}
	case 2:		//���ݿ���
		{
			return lstrcmp(pGameModuleInfo1->szDataBaseName,pGameModuleInfo2->szDataBaseName);
		}
	case 3:		//���ӵ�ַ
		{
			return lstrcmp(pGameModuleInfo1->szDataBaseAddr,pGameModuleInfo2->szDataBaseAddr);
		}
	case 4:		//��������
		{
			return lstrcmp(pGameModuleInfo1->szServerDLLName,pGameModuleInfo2->szServerDLLName);
		}
	case 5:		//�ͻ�����
		{
			return lstrcmp(pGameModuleInfo1->szClientEXEName,pGameModuleInfo2->szClientEXEName);
		}
	}

	return 0;
}

//��ȡ��ɫ
VOID CModuleListControl::GetListItemColor(LPARAM lItemParam, UINT uItemStatus, tagListItemColor & ListItemColor)
{
	//��������
	ASSERT(lItemParam!=NULL);
	tagGameModuleInfo * pGameModuleInfo=(tagGameModuleInfo *)lItemParam;

	//û�а�װ
	if (pGameModuleInfo->dwNativeVersion==0L)
	{
		//������ɫ
		ListItemColor.rcTextColor=RGB(125,125,125);
		ListItemColor.rcBackColor=(uItemStatus&ODS_SELECTED)?RGB(50,50,50):CR_NORMAL_BK;

		return;
	}

	//���ڸ���
	if (pGameModuleInfo->dwNativeVersion!=pGameModuleInfo->dwServerVersion)
	{
		//������ɫ
		ListItemColor.rcTextColor=RGB(128,0,0);
		ListItemColor.rcBackColor=(uItemStatus&ODS_SELECTED)?RGB(230,230,0):CR_NORMAL_BK;

		return;
	}

	//�汾һ��
	if (pGameModuleInfo->dwNativeVersion==pGameModuleInfo->dwServerVersion)
	{
		//������ɫ
		ListItemColor.rcBackColor=(uItemStatus&ODS_SELECTED)?RGB(0,0,128):CR_NORMAL_BK;
		ListItemColor.rcTextColor=(uItemStatus&ODS_SELECTED)?RGB(255,255,255):RGB(0,0,0);

		return;
	}

	return;
}

//�����б�
bool CModuleListControl::InsertModuleInfo(tagGameModuleInfo * pGameModuleInfo)
{
	//��������
	LVFINDINFO FindInfo;
	ZeroMemory(&FindInfo,sizeof(FindInfo));

	//���ñ���
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pGameModuleInfo;

	//�����ж�
	INT nInsertItem=FindItem(&FindInfo);
	if (nInsertItem!=LB_ERR) return true;

	//�����б�
	for (WORD i=0;i<m_ListHeaderCtrl.GetItemCount();i++)
	{
		if (i==0)
		{
			//��������
			INT nIndex=GetInsertIndex(pGameModuleInfo);
			LPCTSTR pszDescribe=GetDescribeString(pGameModuleInfo,i);
			nInsertItem=InsertItem(LVIF_TEXT|LVIF_PARAM,nIndex,pszDescribe,0,0,0,(LPARAM)pGameModuleInfo);
		}
		else
		{
			//�ַ�����
			SetItem(nInsertItem,i,LVIF_TEXT,GetDescribeString(pGameModuleInfo,i),0,0,0,0);
		}
	}

	return true;
}

//��������
WORD CModuleListControl::GetInsertIndex(tagGameModuleInfo * pGameModuleInfo)
{
	//��������
	INT nItemCount=GetItemCount();
	tagGameModuleInfo * pGameModuleTemp=NULL;

	//��ȡλ��
	for (INT i=0;i<nItemCount;i++)
	{
		//��ȡ����
		pGameModuleTemp=(tagGameModuleInfo *)GetItemData(i);

		//��װ�ж�
		if ((pGameModuleInfo->dwNativeVersion==0)&&(pGameModuleTemp->dwNativeVersion!=0))
		{
			continue;
		}
		
		//�����ж�
		if (lstrcmp(pGameModuleInfo->szGameName,pGameModuleTemp->szGameName)<0)
		{
			return i;
		}
	}

	return nItemCount;
}

//�����ַ�
LPCTSTR CModuleListControl::GetDescribeString(tagGameModuleInfo * pGameModuleInfo, WORD wColumnIndex)
{
	//�����ַ�
	switch (wColumnIndex)
	{
	case 0:		//��Ϸ����
		{
			return pGameModuleInfo->szGameName;
		}
	case 1:		//���״̬
		{
			if (pGameModuleInfo->dwNativeVersion==0L) return TEXT("û�а�װ");
			if (pGameModuleInfo->dwServerVersion!=pGameModuleInfo->dwNativeVersion) return TEXT("���ڸ���");
			return TEXT("�Ѿ���װ");
		}
	case 2:		//���ݿ���
		{
			return pGameModuleInfo->szDataBaseName;
		}
	case 3:		//���ӵ�ַ
		{
			return pGameModuleInfo->szDataBaseAddr;
		}
	case 4:		//��������
		{
			return pGameModuleInfo->szServerDLLName;
		}
	case 5:		//�ͻ�����
		{
			return pGameModuleInfo->szClientEXEName;
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////

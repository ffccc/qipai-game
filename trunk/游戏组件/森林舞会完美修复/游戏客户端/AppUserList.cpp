#include "StdAfx.h"
#include ".\appuserlist.h"

CAppUserList::CAppUserList(void):
m_index(0)
{
}

CAppUserList::~CAppUserList(void)
{
}


void CAppUserList::AddOrDeletUser(tagApplyUser ApplyUser,bool bo)
{

	vector<tagApplyUser>::iterator it;
	if (bo)
	{

		if (FindUser(ApplyUser.dwUserID))
		{
			//printf("存在\n");
			return;
		}
		m_AppList.push_back(ApplyUser);

	}
	else
	{

		for(it=m_AppList.begin();it!=m_AppList.end();++it)
		{
			if((*it).dwUserID==ApplyUser.dwUserID)
			{		

				it=m_AppList.erase(it);				
				break;
			}
		}
	}
	
}

// 更新申请上庄用户分数
void CAppUserList::UpdateAppList(tagApplyUser ApplyUser)
{

	vector<tagApplyUser>::iterator it;
	for(it=m_AppList.begin();it!=m_AppList.end();++it)
	{
		if((*it).dwUserID==ApplyUser.dwUserID)
		{
			(*it).lUserScore = ApplyUser.lUserScore;
			break;
		}
	}
}

// 检查是否存在这个用户
bool CAppUserList::FindUser(DWORD UserID)
{
	vector<tagApplyUser>::iterator it;
	for(it=m_AppList.begin();it!=m_AppList.end();++it)
	{

		if ((*it).dwUserID == UserID)
		{
			return true;
		}
	}	
	return false;
}


void CAppUserList::DrawScore(SURFACE* psurfTarget,int nX,int nY)
{

	//绘制庄家申请列表
	vector<tagApplyUser>::iterator it;
	int listy=0 ,inx=0;

	for (it = m_AppList.begin();it!=m_AppList.end();++it)
	{
		if (inx >= m_index)
		{
			char name[30];
			sprintf(name,"%s",(*it).stAname);
			psurfTarget->DrawText(nX,nY+listy*18,PIXEL(255,0,0),name);
			sprintf(name,"%I64d",(*it).lUserScore);
			psurfTarget->DrawText(nX +110-strlen(inst(name))*6,nY+(listy++)*18,PIXEL(255,0,0),name);
			if (listy > 5)return;
		}
		inx++;
		//break;
	}
}
// 清空容器
void CAppUserList::clearVector(void)
{
	m_AppList.clear();
}

TCHAR* CAppUserList::inst(TCHAR* szBuffer)
{

	int cu=0;
	string s1=szBuffer;
	int k = 1;	
	for (int i=strlen(s1.c_str())-1;i>=k;i--)
	{
		cu++;
		if (cu%4 == 0)				
			s1.insert(i,",");		
	}
	strcpy(szBuffer,s1.c_str());
	return szBuffer;
}

//列表偏移
void CAppUserList::listPY(bool bo)
{
	if (bo)//为真向上偏移
	{
		if (m_index >0 )m_index--;
	}
	else//为真向下偏移
		if(m_AppList.size() - m_index > 5)m_index++;
}
#include "Stdafx.h"
#include "HtmlBrowser.h"

BEGIN_MESSAGE_MAP(CHtmlBrower, CHtmlView)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CHtmlBrower::CHtmlBrower()
{
	m_bEnableBrowser=false;
}

//��������
CHtmlBrower::~CHtmlBrower()
{
}

//ת����ҳ
void CHtmlBrower::Navigate(LPCTSTR pszURL)
{
	ASSERT(pszURL!=NULL);
	m_strNavigateUrl=pszURL;
	if (m_bEnableBrowser) __super::Navigate(m_strNavigateUrl,NULL,NULL,NULL,NULL);
	return;
}

//������ҳ
bool CHtmlBrower::EnableBrowser(bool bEnableBrowser)
{
	if (m_bEnableBrowser!=bEnableBrowser)
	{
		m_bEnableBrowser=bEnableBrowser;
		if (m_bEnableBrowser==true)
		{
			if ((GetSafeHwnd()!=NULL)&&(m_strNavigateUrl.IsEmpty()==FALSE))
			{
				__super::Navigate(m_strNavigateUrl,NULL,NULL,NULL,NULL);
			}
		}
		else m_strNavigateUrl.Empty();
	}
	return m_bEnableBrowser;
}

//////////////////////////////////////////////////////////////////////////

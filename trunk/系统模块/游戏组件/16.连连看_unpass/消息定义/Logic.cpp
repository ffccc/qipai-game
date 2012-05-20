#include "StdAfx.h"
#include "Logic.h"

CLogic::CLogic(void)
{
}

CLogic::~CLogic(void)
{
}


//�ж��Ƿ��������
bool CLogic::IsLink(BYTE  chess[CHESSCOUNT_H][CHESSCOUNT_W], CPoint first, CPoint second, CArrayTemplate<CPoint> *v,CPoint *ipoint1,CPoint *ipoint2)
{
	CArrayTemplate<CPoint> f,s;
	CPoint samepoint;
	
	//�������Ƿ�����
	if(first.x == second.x && abs(first.y - second.y)==1 ||
		first.y == second.y && abs(first.x - second.x) == 1)
	{
		if(chess[first.y][first.x] == chess[second.y][second.x])
		{
			v->InsertAt(v->GetCount(),first);
			v->InsertAt(v->GetCount(),second);
            return true;
		}
		else
			return false;
	}

//һ��ֱ�߿ɷ�����
	FindMap(&f,chess,first,WAY_UP,false);
	FindMap(&f,chess,first,WAY_DOWN,false);
	FindMap(&f,chess,first,WAY_LEFT,false);
	FindMap(&f,chess,first,WAY_RIGHT,false);

	FindMap(&s,chess,second,WAY_UP,false);
	FindMap(&s,chess,second,WAY_DOWN,false);
	FindMap(&s,chess,second,WAY_LEFT,false);
	FindMap(&s,chess,second,WAY_RIGHT,false);

	//�ж��Ƿ��й�ͬ�ĵ�
	if( AssertHaveSame(&f,&s,&samepoint) )
	{
		//�ҳ�·��
		ShortPath(chess,v,first,samepoint,NULL);
		ShortPath(chess,v,samepoint,second,NULL);
		v->InsertAt(v->GetCount(),second);

		//�洢����
		if(first.x == samepoint.x && samepoint.x == second.x || first.y == samepoint.y && samepoint.y == second.y)
			;
		else
		{
			ipoint1->x = samepoint.x;
			ipoint1->y = samepoint.y;
		}
            
		return true;		
	}
	f.RemoveAll();
	s.RemoveAll();

//������������ֱ���Ƿ��������
	FindMap(&f,chess,first,WAY_UP,true);
	FindMap(&f,chess,first,WAY_DOWN,true);
	FindMap(&f,chess,first,WAY_LEFT,true);
	FindMap(&f,chess,first,WAY_RIGHT,true);

	FindMap(&s,chess,second,WAY_UP,false);
	FindMap(&s,chess,second,WAY_DOWN,false);
	FindMap(&s,chess,second,WAY_LEFT,false);
	FindMap(&s,chess,second,WAY_RIGHT,false);

	//�ж��Ƿ��й�ͬ�ĵ�
	if( AssertHaveSame(&f,&s,&samepoint) )
	{
		ShortPath(chess,v,first,samepoint,ipoint1);
		
		//�洢����
		ipoint2->x = samepoint.x;
		ipoint2->y = samepoint.y;

		ShortPath(chess,v,samepoint,second,NULL);
		v->InsertAt(v->GetCount(),second);

		return true;
	}
	return false;
	
}

	// �Զ�����
bool CLogic::FindMap(CArrayTemplate<CPoint> *v, BYTE  chess[CHESSCOUNT_H][CHESSCOUNT_W],CPoint point, int way, bool isFirst)
{
	switch(way)
	{
	case WAY_UP:
		{
			point.y -= 1;
			while(InChess(chess,point.x,point.y))
			{
				if(chess[point.y][point.x]==ERROR_ALL )
				{
					v->InsertAt(v->GetCount(),point);
					if(isFirst)
					{
						FindMap(v,chess,point,WAY_LEFT,false);
						FindMap(v,chess,point,WAY_RIGHT,false);
					}
					point.y-=1;
				}
				else
                    return true;
			}
			break;
		}
	case WAY_DOWN:
		{
			point.y += 1;
			while(InChess(chess,point.x,point.y))
			{
				if(chess[point.y][point.x]==ERROR_ALL )
				{
					v->InsertAt(v->GetCount(),point);
					if(isFirst)
					{
						FindMap(v,chess,point,WAY_LEFT,false);
						FindMap(v,chess,point,WAY_RIGHT,false);
					}
					point.y+=1;
				}
				else
                    return true;
			}
			break;
		}
	case WAY_LEFT:
		{
			point.x -= 1;
			while(InChess(chess,point.x,point.y))
			{
				if(chess[point.y][point.x]==ERROR_ALL )
				{
					v->InsertAt(v->GetCount(),point);
					if(isFirst)
					{
						FindMap(v,chess,point,WAY_UP,false);
						FindMap(v,chess,point,WAY_DOWN,false);
					}
					point.x-=1;
				}
				else
                    return true;
			}
			break;
		}
	case WAY_RIGHT:
		{
			point.x += 1;
			while(InChess(chess,point.x,point.y))
			{
				if(chess[point.y][point.x]==ERROR_ALL )
				{
					v->InsertAt(v->GetCount(),point);
					if(isFirst)
					{
						FindMap(v,chess,point,WAY_UP,false);
						FindMap(v,chess,point,WAY_DOWN,false);
					}
					point.x+=1;
				}
				else
                    return true;
			}
			break;
		}
	}
	
	return true;
}

// �Ƿ���������
bool CLogic::InChess(BYTE  chess[CHESSCOUNT_H][CHESSCOUNT_W],int w, int h)
{
	if(w>=0 && w<= CHESSCOUNT_W && h>=0 && h<= CHESSCOUNT_H)
		return true;
	else
        return false;
}

// �жϾ����Ƿ��ཻ
bool CLogic::AssertHaveSame(CArrayTemplate<CPoint> * f, CArrayTemplate<CPoint> * s, CPoint *same)
{
	for(int i=0; i<f->GetCount(); i++)
	{
		for(int j = 0; j< s->GetCount(); j++)
		{
			if(f->ElementAt(i).x == s->ElementAt(j).x  && f->ElementAt(i).y == s->ElementAt(j).y )
			{
				same->SetPoint(f->ElementAt(i).x,f->ElementAt(i).y);
				return true;
			}
		}
	}
	return false;
}

// Ѱ�ҷ������������·��
void CLogic::ShortPath(BYTE  chess[CHESSCOUNT_H][CHESSCOUNT_W],CArrayTemplate<CPoint> * r, CPoint one , CPoint two, CPoint* ipoint)
{
	CArrayTemplate<CPoint> t;
	CPoint hotpoint;
	bool blog = true;


	if( one.x !=two.x)
	{
		for(int i= one.x,j=one.y; i!=two.x; one.x<two.x?i++:i--)
		{
			if(i!=one.x && chess[j][i]!=ERROR_ALL)
			{
				blog = false;
				break;
			}
			else
			{
				t.InsertAt(t.GetCount(),CPoint(i,j));
			}
		}
	}
	if(one.y != two.y)
	{
		hotpoint.x = two.x;
		hotpoint.y = one.y;
		for(int i=two.x,j = one.y;j!=two.y; one.y<two.y ? j++:j--)
		{
			if(chess[j][i]!=ERROR_ALL)
			{
				blog = false;
				break;
			}
			else
				{
					t.InsertAt(t.GetCount(),CPoint(i,j));
				}
		}
	}
	if(blog)
	{
		for(int i=0; i<t.GetCount(); ++i)
		{
			r->InsertAt(r->GetCount(),CPoint(t.ElementAt(i).x,t.ElementAt(i).y));
			if(ipoint!=NULL)
			{
				ipoint->x = hotpoint.x;
				ipoint->y = hotpoint.y;
			}
		}
		return;
	}
	else
	{
		t.RemoveAll();
		hotpoint.x = ERROR_ALL;
		hotpoint.y = ERROR_ALL;
		blog = true;
	}

	if(one.y != two.y)
	{
		for(int i=one.x,j = one.y;j!= two.y; one.y<two.y ? j++:j--)
		{
			if(j!= one.y&&chess[j][i]!=ERROR_ALL)
			{
				t.RemoveAll();
				blog = false;
				break;
			}
			else
				{
					t.InsertAt(t.GetCount(),CPoint(i,j));
				}
		}
	}
	if(one.x != two.x)
	{
		hotpoint.x = one.x;
		hotpoint.y = two.y;
		for(int i= one.x,j=two.y; i!=two.x; one.x<two.x?i++:i--)
		{
			if(chess[j][i]!=ERROR_ALL)
			{
				t.RemoveAll();
				blog = false;
				break;
			}
			else
				{
					t.InsertAt(t.GetCount(),CPoint(i,j));
				}
		}
	}

	if(blog)
	{
		for(int i=0; i<t.GetCount(); ++i)
		{
			r->InsertAt(r->GetCount(),CPoint(t.ElementAt(i).x,t.ElementAt(i).y));
			if(ipoint!=NULL)
			{
				ipoint->x = hotpoint.x;
				ipoint->y = hotpoint.y;
			}
		}
		return;
	}
	return;
}

// ������ʾ
bool CLogic::LinkPrompt(BYTE  chess[CHESSCOUNT_H][CHESSCOUNT_W], CPoint * first, CPoint * end)
{
	CPoint t1,t2;
	CArrayTemplate<CPoint> v;

	for(int i=0; i<CHESSCOUNT_H; i++)
	{
		for(int j=0; j<CHESSCOUNT_W; j++)
		{
			for(int _i=0; _i<CHESSCOUNT_H; _i++)
			{
				for(int _j=0; _j<CHESSCOUNT_W; _j++)
				{
					if(i!=_i || j!=_j)
					{
						v.RemoveAll();
						if(chess[i][j]!= ERROR_ALL && chess[i][j] == chess[_i][_j] && IsLink(chess,CPoint(j,i),CPoint(_j,_i),&v,&t1,&t2))
						{
							first->x = j; first->y = i;
							end->x = _j; end->y = _i;
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
// ��������
void CLogic::Realign(BYTE  chess[CHESSCOUNT_H][CHESSCOUNT_W])
{
	CArrayTemplate<CPoint> pos;
	CArrayTemplate<BYTE> data;
	int temp,temp2;

	for(int i=0; i<CHESSCOUNT_H; i++)
	for(int j=0; j<CHESSCOUNT_W; j++)
	{
		if(chess[i][j] != ERROR_ALL)
		{
			pos.InsertAt(pos.GetCount(),CPoint(i,j));
			data.InsertAt(data.GetCount(),chess[i][j]);
		}
		chess[i][j] = ERROR_ALL;
	}
	srand((unsigned int)time(NULL));
	for(; pos.GetCount()>0;)
	{
		temp = rand()%((int)pos.GetCount());
		temp2 = rand()%((int)data.GetCount());
		chess[pos.ElementAt(temp).x][pos.ElementAt(temp).y] = data[temp2];
		pos.RemoveAt(temp);
		data.RemoveAt(temp2);
	}
}

//����ȼ�
LPCTSTR CLogic::AssertUserClass(LONG score)
{
	LONG lScoreInfo_2[18]=	{		0L,50L,100L,200L,500L,800L,1200L,2000L,3000L,5000L,8000L,12000L,15000L,18000L,25000L,40000L,60000L,	};	 LPCTSTR pszOrderName_2[18]=	{		TEXT("����"),		TEXT("������"),		TEXT("����"),TEXT("����"),TEXT("Ѽ����"),TEXT("˫����"),		TEXT("Ѹ��"),TEXT("����"),TEXT("����"),TEXT("����"),		TEXT("����"),TEXT("�ƽ���"),TEXT("������"),TEXT("����ڤ��"),		TEXT("�Ϻ�����"),TEXT("����ħ��"),TEXT("��������"),TEXT("�л�ʥ��"),	};	for (int i=0;i<CountArray(lScoreInfo_2);i++)	{		if (score<lScoreInfo_2[i]) return pszOrderName_2[i];	}	return pszOrderName_2[17];
}

float CLogic::AssertUserPer(LONG score)
{
	LONG lScoreInfo_2[18]=	{		0L,50L,100L,200L,500L,800L,1200L,2000L,3000L,5000L,8000L,12000L,15000L,18000L,25000L,40000L,60000L,	};	for (int i=0;i<CountArray(lScoreInfo_2);i++)	{		if (score<lScoreInfo_2[i]&&i!=0) 			return (float)(100.0*(score-lScoreInfo_2[i-1])/(lScoreInfo_2[i]-lScoreInfo_2[i-1]));		else		if (score<lScoreInfo_2[i]&&score<0&&i==0)			return 0.0;	}	return 100.0;
}

void CLogic::OpenUrl(CString url)
{
	HRESULT hr;
	IWebBrowserApp* pWBApp = NULL; // Derived from IWebBrowser
	BSTR bstrURL = NULL, bstrHeaders = NULL;
	VARIANT vFlags = {0},
		vTargetFrameName = {0},
		vPostData = {0},
		vHeaders = {0};

	if (FAILED(hr = CoInitialize(NULL)))
	{
		return;
	}

	if (FAILED(hr = CoCreateInstance(CLSID_InternetExplorer,
		NULL,
		CLSCTX_SERVER,
		IID_IWebBrowserApp,
		(LPVOID*)&pWBApp)))
	{
		if (bstrURL) SysFreeString(bstrURL);
		if (bstrHeaders) SysFreeString(bstrHeaders);
		VariantClear(&vPostData);
		if (pWBApp) pWBApp->Release();
		CoUninitialize();
		return;
	}

	bstrURL = url.AllocSysString();
	if (!bstrURL)
	{
		if (bstrURL) SysFreeString(bstrURL);
		if (bstrHeaders) SysFreeString(bstrHeaders);
		VariantClear(&vPostData);
		if (pWBApp) pWBApp->Release();
		CoUninitialize();
		return;
	}

	bstrHeaders = SysAllocString(
		L"Content-Type: application/x-www-form-urlencoded\r\n");
	if (!bstrHeaders)
	{
		if (bstrURL) SysFreeString(bstrURL);
		if (bstrHeaders) SysFreeString(bstrHeaders);
		VariantClear(&vPostData);
		if (pWBApp) pWBApp->Release();
		CoUninitialize();
		return;
	}

	V_VT(&vHeaders) = VT_BSTR;
	V_BSTR(&vHeaders) = bstrHeaders;

	hr = pWBApp->Navigate(bstrURL, &vFlags,
		&vTargetFrameName, &vPostData, &vHeaders);
	
	pWBApp->put_Visible(VARIANT_TRUE);
	
	if (bstrURL) SysFreeString(bstrURL);
	if (bstrHeaders) SysFreeString(bstrHeaders);
	VariantClear(&vPostData);
	if (pWBApp) pWBApp->Release();
	CoUninitialize();
}

// �����Ƿ�Ϊ��
bool CLogic::ChessEmpty(BYTE  chess[CHESSCOUNT_H][CHESSCOUNT_W])
{
	int count=0;
	for(int i=0; i<CHESSCOUNT_H; i++)
	for(int j=0; j<CHESSCOUNT_W; j++)
	{
		if(chess[i][j] != ERROR_ALL)
			count ++;
	}
	if(count == 0)
		return false;
	else
		return true;
}
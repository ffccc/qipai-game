#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
//��̬����
RegionInfo BetNumberArea[3][12]={};
RegionInfo CrossBetArea[3][12]={};
RegionInfo WideSideCrossArea[3][12]={};
RegionInfo HeightSideCrossArea[3][12]={};
RegionInfo Cross0Area[6]={};
//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
	m_nScreenWidth=0;
	m_nScreenHeight=0;
	calcPoint();
}

//��������
CGameLogic::~CGameLogic()
{

}
//ȡ��������ɫ
BYTE CGameLogic::GetNumberColor(BYTE bNumber)
{
	if(bNumber==0) return COLOR_0;
	if(bNumber==1||bNumber==3||bNumber==5||bNumber==7||bNumber==9||bNumber==12||bNumber==14||bNumber==16||bNumber==18||bNumber==19||bNumber==21
		||bNumber==23||bNumber==25||bNumber==27||bNumber==30||bNumber==32||bNumber==34||bNumber==36)
		return COLOR_RED;
	return COLOR_BLACK;
}

//����÷�
long CGameLogic::CountScore(long ChipsInfo[256],int index[256],int count)
{
	//�ۼƵ÷�
	long lAllScore=0;
	for(int i=0;i<count;i++)
	{
		int areaIndex=index[i];
		if(areaIndex>=255) continue;
		//��һ������
		if(areaIndex>=1&&areaIndex<=36)							lAllScore+=ChipsInfo[areaIndex]*36;
		//������
		if(areaIndex>=37&&areaIndex<=39) 						lAllScore+=ChipsInfo[areaIndex]*3;
		//������
		if(areaIndex>=40&&areaIndex<=42) 						lAllScore+=ChipsInfo[areaIndex]*3;
		//1~18��19~36
		if(areaIndex>=43&&areaIndex<=44) 						lAllScore+=ChipsInfo[areaIndex]*2;
		//��˫�����
		if(areaIndex>=45&&areaIndex<=48) 						lAllScore+=ChipsInfo[areaIndex]*2;
		//ʮ�ֽ��������
		if(areaIndex>=53&&areaIndex<=83&&(areaIndex-53)%3==0)	lAllScore+=ChipsInfo[areaIndex]*6;
		//ʮ�ֽ���
		if(areaIndex>=53&&areaIndex<=85&&(areaIndex-53)%3!=0)	lAllScore+=ChipsInfo[areaIndex]*9;
		//���ཻ�������
		if(areaIndex>=90&&areaIndex<=123&&areaIndex%3==0)		lAllScore+=ChipsInfo[areaIndex]*12;
		//���ཻ��
		if(areaIndex>=90&&areaIndex<=125&&areaIndex%3!=0)		lAllScore+=ChipsInfo[areaIndex]*18;
		//���ཻ��
		if(areaIndex>=130&&areaIndex<=162)						lAllScore+=ChipsInfo[areaIndex]*18;
		//������
		if(areaIndex==200)										lAllScore+=ChipsInfo[areaIndex]*36;
		if(areaIndex==201)										lAllScore+=ChipsInfo[areaIndex]*9;
		if(areaIndex==203||areaIndex==205)						lAllScore+=ChipsInfo[areaIndex]*12;
		if(areaIndex==202||areaIndex==204||areaIndex==206)		lAllScore+=ChipsInfo[areaIndex]*18;
	}
	return lAllScore;
}
//����÷�
long CGameLogic::ComputeScore(long ChipsInfo[256],BYTE WinNumber)
{
	switch(WinNumber)
	{
	case 0: {int temp[]={200,201,202,203,204,205,206};return CountScore(ChipsInfo,temp,7);}
	case 1: {int temp[]={201,202,203,90,1,91,53,130,54,40,43,37,47,45};return CountScore(ChipsInfo,temp,14);}
	case 2: {int temp[]={201,203,204,205,91,2,92,54,131,55,90,53,38,43,40,46,48};return CountScore(ChipsInfo,temp,17);}
	case 3: {int temp[]={201,205,206,90,92,3,53,55,132,40,43,39,45,47};return CountScore(ChipsInfo,temp,15);}
	case 4:	{int temp[]={53,130,54,93,4,94,56,133,57,40,43,37,46,48};return CountScore(ChipsInfo,temp,14);}
	case 5:	{int temp[]={53,54,131,55,93,94,5,95,56,57,134,58,40,43,38,47,45};return CountScore(ChipsInfo,temp,17);}
	case 6:	{int temp[]={53,55,132,93,95,6,56,58,135,39,40,43,46,48};return CountScore(ChipsInfo,temp,14);}
	case 7:	{int temp[]={56,133,57,96,7,97,59,136,60,40,43,37,45,47};return CountScore(ChipsInfo,temp,14);}
	case 8:	{int temp[]={56,57,134,58,96,97,8,98,59,60,137,61,40,43,38,48,46};return CountScore(ChipsInfo,temp,17);}
	case 9:	{int temp[]={56,58,135,96,98,9,59,61,138,40,43,45,47,39};return CountScore(ChipsInfo,temp,14);}
	case 10:{int temp[]={59,136,60,99,10,100,62,139,63,40,43,46,37,48};return CountScore(ChipsInfo,temp,14);}

	case 11:{int temp[]={59,60,137,61,99,100,11,101,62,63,140,64,38,40,43,45,48};return CountScore(ChipsInfo,temp,17);}
	case 12:{int temp[]={59,61,138,99,101,12,62,64,141,39,46,47,40,43};return CountScore(ChipsInfo,temp,14);}
	case 13:{int temp[]={62,139,63,102,13,103,65,142,66,37,41,43,45,48};return CountScore(ChipsInfo,temp,14);}
	case 14:{int temp[]={62,63,140,64,102,103,14,104,65,66,143,67,38,41,47,43,46};return CountScore(ChipsInfo,temp,17);}
	case 15:{int temp[]={62,64,141,102,104,15,65,67,144,39,41,43,45,48};return CountScore(ChipsInfo,temp,14);}
	case 16:{int temp[]={65,142,66,105,16,106,68,145,69,37,41,46,43,47};return CountScore(ChipsInfo,temp,14);}
	case 17:{int temp[]={65,66,143,67,105,106,107,17,68,69,70,146,38,41,43,45,48};return CountScore(ChipsInfo,temp,17);}
	case 18:{int temp[]={65,67,144,105,107,18,68,70,147,39,41,47,43,46};return CountScore(ChipsInfo,temp,14);}
	case 19:{int temp[]={68,145,69,108,19,109,71,148,72,37,41,45,47,44};return CountScore(ChipsInfo,temp,14);}
	case 20:{int temp[]={68,69,146,70,108,109,110,20,71,72,149,73,38,41,48,44,46};return CountScore(ChipsInfo,temp,17);}

	case 21:{int temp[]={68,70,147,108,110,21,71,73,150,39,41,44,45,47};return CountScore(ChipsInfo,temp,14);}
	case 22:{int temp[]={71,148,72,111,22,112,74,151,75,37,41,44,46,48};return CountScore(ChipsInfo,temp,14);}
	case 23:{int temp[]={71,72,149,73,111,112,23,113,74,75,152,76,38,41,44,45,47};return CountScore(ChipsInfo,temp,17);}
	case 24:{int temp[]={71,73,150,111,113,24,74,76,153,39,41,44,46,48};return CountScore(ChipsInfo,temp,14);}
	case 25:{int temp[]={74,151,75,114,25,115,77,154,78,37,42,44,45,47};return CountScore(ChipsInfo,temp,14);}
	case 26:{int temp[]={74,75,152,76,114,115,26,116,77,78,155,79,38,42,44,46,48};return CountScore(ChipsInfo,temp,17);}
	case 27:{int temp[]={74,76,153,114,116,27,77,79,156,39,42,44,45,47};return CountScore(ChipsInfo,temp,14);}
	case 28:{int temp[]={77,154,78,117,28,118,80,157,81,37,42,44,46,48};return CountScore(ChipsInfo,temp,14);}
	case 29:{int temp[]={77,78,155,79,117,118,29,119,80,81,158,82,38,42,44,45,48};return CountScore(ChipsInfo,temp,17);}
	case 30:{int temp[]={77,79,156,117,119,30,80,82,159,39,42,44,46,47};return CountScore(ChipsInfo,temp,14);}

	case 31:{int temp[]={80,157,81,120,31,121,83,160,84,37,42,44,45,48};return CountScore(ChipsInfo,temp,14);}
	case 32:{int temp[]={80,81,158,82,120,121,32,122,83,84,161,85,38,42,44,46,47};return CountScore(ChipsInfo,temp,17);}
	case 33:{int temp[]={80,82,159,120,122,33,83,85,162,39,42,44,45,48};return CountScore(ChipsInfo,temp,14);}
	case 34:{int temp[]={83,160,84,123,34,124,37,42,44,46,47};return CountScore(ChipsInfo,temp,11);}
	case 35:{int temp[]={83,84,161,85,123,124,35,125,38,42,44,45,48};return CountScore(ChipsInfo,temp,13);}
	case 36:{int temp[]={83,85,162,123,125,36,39,42,44,46,47};return CountScore(ChipsInfo,temp,11);}
	default:return 0;
	}
	return 0;
}

//���ô��ڴ�С
void CGameLogic::SetDxWindowSize(int nWidth,int nHeight)
{
	m_nScreenWidth=nWidth;
	m_nScreenHeight=nHeight;
}

//���ص��λ��
WORD CGameLogic::GetMouseClickRegion(CPoint point)
{	
	//ѡ�������
	for(int i=0;i<6;i++)
	{
		if(IsInRegion(ChooseChipArea[i].BasePoint,ChooseChipArea[i].VertexTop,ChooseChipArea[i].VertexButtom,ChooseChipArea[i].VertexRight,point)) 
		{
			return 900+i;
		}
	}

	//0��
	if(IsInRegion(Bet0Area.BasePoint,Bet0Area.VertexTop,Bet0Area.VertexButtom,Bet0Area.VertexRight,point,1))
	{
		return 200;
	}
	for(int i=0;i<6;i++)
	{
		if(IsInRegion(Cross0Area[i].BasePoint,Cross0Area[i].VertexTop,Cross0Area[i].VertexButtom,Cross0Area[i].VertexRight,point))
		{
			return i+201;
		}
	}
	//������
	if(IsInRegion(BetSingleArea.BasePoint,BetSingleArea.VertexTop,BetSingleArea.VertexButtom,BetSingleArea.VertexRight,point))		return BET_IN_SINGLE;
	//˫����
	if(IsInRegion(BetDoubleArea.BasePoint,BetDoubleArea.VertexTop,BetDoubleArea.VertexButtom,BetDoubleArea.VertexRight,point))		return BET_IN_DOUBLE;
	//��ɫ��
	if(IsInRegion(BetRedArea.BasePoint,BetRedArea.VertexTop,BetRedArea.VertexButtom,BetRedArea.VertexRight,point))					return BET_IN_RED;
	//��ɫ��
	if(IsInRegion(BetBlackArea.BasePoint,BetBlackArea.VertexTop,BetBlackArea.VertexButtom,BetBlackArea.VertexRight,point))			return BET_IN_BLACK;
	//1~18��
	if(IsInRegion(Bet1to18Area.BasePoint,Bet1to18Area.VertexTop,Bet1to18Area.VertexButtom,Bet1to18Area.VertexRight,point))			return BET_IN_1TO18;
	//19~36��
	if(IsInRegion(Bet19to36Area.BasePoint,Bet19to36Area.VertexTop,Bet19to36Area.VertexButtom,Bet19to36Area.VertexRight,point))		return BET_IN_19TO36;
	//���µ�2����
	for(int i=0;i<3;i++)
	{
		if(IsInRegion(BetLineArea[i].BasePoint,BetLineArea[i].VertexTop,BetLineArea[i].VertexButtom,BetLineArea[i].VertexRight,point))		
		{
			if(i==0) return BET_IN_LINE1;
			if(i==1) return BET_IN_LINE2;
			if(i==2) return BET_IN_LINE3;
		}
	}
	//���3������
	for(int i=0;i<3;i++)
	{
		if(IsInRegion(BetDozenArea[i].BasePoint,BetDozenArea[i].VertexTop,BetDozenArea[i].VertexButtom,BetDozenArea[i].VertexRight,point)) 
		{
			if(i==0) return BET_IN_1TO12;
			if(i==1) return BET_IN_13TO24;
			if(i==2) return BET_IN_25TO36;
		}
	}
	//��������
	for(int i=0;i<3;i++)
	for(int j=0;j<12;j++)
	{
		if(IsInRegion(CrossBetArea[i][j].BasePoint,CrossBetArea[i][j].VertexTop,CrossBetArea[i][j].VertexButtom,CrossBetArea[i][j].VertexRight,point)) 
		{
			return (j*3+i)+50;
		}
	}
	//���ཻ����
	for(int i=0;i<3;i++)
	for(int j=0;j<12;j++)
	{
		//��һ��
		if(IsInRegion(WideSideCrossArea[i][j].BasePoint,WideSideCrossArea[i][j].VertexTop,WideSideCrossArea[i][j].VertexButtom,WideSideCrossArea[i][j].VertexRight,point))
		{
			return (j*3+i)+90;
		}
	}
	//���ཻ����
	for(int i=0;i<3;i++)
	for(int j=0;j<11;j++)
	{
		if(IsInRegion(HeightSideCrossArea[i][j].BasePoint,HeightSideCrossArea[i][j].VertexTop,HeightSideCrossArea[i][j].VertexButtom,HeightSideCrossArea[i][j].VertexRight,point))
		{
			return (j*3+i)+130;
		}
	}
	//������
	for(int x=0;x<3;x++)
	for(int y=0;y<12;y++)
	{
		if(IsInRegion(BetNumberArea[x][y].BasePoint,BetNumberArea[x][y].VertexTop,BetNumberArea[x][y].VertexButtom,BetNumberArea[x][y].VertexRight,point)) 
		{
			return (y*3+x)+1;//��������
		}
	}
	return BET_IN_ERROR;
}

//�ж������������
bool CGameLogic::RespondMouseInRegionX(CPoint point,bool Light[3][12],bool& ZeroLight,LPTSTR TipsMessage)
{
	//ѡ�������
	for(int i=0;i<6;i++)
	{
		if(IsInRegion(ChooseChipArea[i].BasePoint,ChooseChipArea[i].VertexTop,ChooseChipArea[i].VertexButtom,ChooseChipArea[i].VertexRight,point)) 
		{
			lstrcpyn(TipsMessage,TEXT("�����������"),sizeof(TCHAR)*64);
			return true;
		}
	}

	//0��
	if(IsInRegion(Bet0Area.BasePoint,Bet0Area.VertexTop,Bet0Area.VertexButtom,Bet0Area.VertexRight,point,1))
	{
		ZeroLight=true;
		lstrcpyn(TipsMessage,TEXT("��ע 0  ( 1 �� 35 )"),sizeof(TCHAR)*64);
		return true;
	}
	for(int i=0;i<6;i++)
	{
		if(IsInRegion(Cross0Area[i].BasePoint,Cross0Area[i].VertexTop,Cross0Area[i].VertexButtom,Cross0Area[i].VertexRight,point))
		{
			if(i==0) {ZeroLight=true;Light[0][0]=true;Light[1][0]=true;Light[2][0]=true;lstrcpyn(TipsMessage,TEXT("��ע 0,1,2,3  ( 1 �� 8 )"),sizeof(TCHAR)*64);return true;}
			if(i==1) {ZeroLight=true;Light[0][0]=true;lstrcpyn(TipsMessage,TEXT("��ע 0,1  ( 1 �� 17 )"),sizeof(TCHAR)*64);return true;}
			if(i==2) {ZeroLight=true;Light[0][0]=true;Light[1][0]=true;lstrcpyn(TipsMessage,TEXT("��ע 0,1,2  ( 1 �� 11 )"),sizeof(TCHAR)*64);return true;}
			if(i==3) {ZeroLight=true;Light[1][0]=true;lstrcpyn(TipsMessage,TEXT("��ע 0,2  ( 1 �� 17 )"),sizeof(TCHAR)*64);return true;}
			if(i==4) {ZeroLight=true;Light[1][0]=true;Light[2][0]=true;lstrcpyn(TipsMessage,TEXT("��ע 0,2,3  ( 1 �� 11 )"),sizeof(TCHAR)*64);return true;}
			if(i==5) {ZeroLight=true;Light[2][0]=true;lstrcpyn(TipsMessage,TEXT("��ע 0,3  ( 1 �� 17 )"),sizeof(TCHAR)*64);return true;}
			return true;
		}
	}
	//������
	if(IsInRegion(BetSingleArea.BasePoint,BetSingleArea.VertexTop,BetSingleArea.VertexButtom,BetSingleArea.VertexRight,point))
	{
		bool bTemp[36]={1,0,1,0,1,0,1,0,1,0,1,0,    0,1,0,1,0,1,0,1,0,1,0,1,    1,0,1,0,1,0,1,0,1,0,1,0,};
		CopyMemory(Light,bTemp,sizeof(bTemp));
		lstrcpyn(TipsMessage,TEXT("��ע���е���  ( 1 �� 1 )"),sizeof(TCHAR)*64);
		return true;
	}
	//˫����
	if(IsInRegion(BetDoubleArea.BasePoint,BetDoubleArea.VertexTop,BetDoubleArea.VertexButtom,BetDoubleArea.VertexRight,point))
	{
		bool bTemp[36]={0,1,0,1,0,1,0,1,0,1,0,1,     1,0,1,0,1,0,1,0,1,0,1,0,    0,1,0,1,0,1,0,1,0,1,0,1,};
		CopyMemory(Light,bTemp,sizeof(bTemp));
		lstrcpyn(TipsMessage,TEXT("��ע����˫��  ( 1 �� 1 )"),sizeof(TCHAR)*64);
		return true; 
	}
	//��ɫ��
	if(IsInRegion(BetRedArea.BasePoint,BetRedArea.VertexTop,BetRedArea.VertexButtom,BetRedArea.VertexRight,point))
	{
		bool bTemp[36]={1,0,1,0,0,1,1,0,1,0,0,1,0,1,0,0,1,0,0,1,0,0,1,0,1,0,1,1,0,1,1,0,1,1,0,1};
		CopyMemory(Light,bTemp,sizeof(bTemp));	
		lstrcpyn(TipsMessage,TEXT("��ע���к�ɫ����  ( 1 �� 1 )"),sizeof(TCHAR)*64);
		return true;
	}
	//��ɫ��
	if(IsInRegion(BetBlackArea.BasePoint,BetBlackArea.VertexTop,BetBlackArea.VertexButtom,BetBlackArea.VertexRight,point))
	{
		bool bTemp[36]={0,1,0,1,1,0,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,0,1,0,0,1,0,0,1,0};
		CopyMemory(Light,bTemp,sizeof(bTemp));	
		lstrcpyn(TipsMessage,TEXT("��ע���к�ɫ����  ( 1 �� 1 )"),sizeof(TCHAR)*64);
		return true;
	}
	//1~18��
	if(IsInRegion(Bet1to18Area.BasePoint,Bet1to18Area.VertexTop,Bet1to18Area.VertexButtom,Bet1to18Area.VertexRight,point)) 
	{
		bool bTemp[36]={1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,};
		CopyMemory(Light,bTemp,sizeof(bTemp));
		lstrcpyn(TipsMessage,TEXT("��ע���� 1~18  ( 1 �� 1 )"),sizeof(TCHAR)*64);
		return true;
	}
	//19~36��
	if(IsInRegion(Bet19to36Area.BasePoint,Bet19to36Area.VertexTop,Bet19to36Area.VertexButtom,Bet19to36Area.VertexRight,point)) 
	{
		bool bTemp[36]={0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,};
		CopyMemory(Light,bTemp,sizeof(bTemp));
		lstrcpyn(TipsMessage,TEXT("��ע���� 19~36  ( 1 �� 1 )"),sizeof(TCHAR)*64);
		return true;
	}
	//���µ�2����
	for(int i=0;i<3;i++)
	{
		if(IsInRegion(BetLineArea[i].BasePoint,BetLineArea[i].VertexTop,BetLineArea[i].VertexButtom,BetLineArea[i].VertexRight,point)) 
		{
			for(int j=0;j<12;j++) Light[i][j]=true;	
			if(i==0) lstrcpyn(TipsMessage,TEXT("��ע���� ��һ��  ( 1 �� 2 )"),sizeof(TCHAR)*64);
			if(i==1) lstrcpyn(TipsMessage,TEXT("��ע���� �ڶ���  ( 1 �� 2 )"),sizeof(TCHAR)*64);
			if(i==2) lstrcpyn(TipsMessage,TEXT("��ע���� ������  ( 1 �� 2 )"),sizeof(TCHAR)*64);
			return true;
		}
	}
	//���3������
	for(int i=0;i<3;i++)
	{
		if(IsInRegion(BetDozenArea[i].BasePoint,BetDozenArea[i].VertexTop,BetDozenArea[i].VertexButtom,BetDozenArea[i].VertexRight,point)) 
		{
			for(int j=4*i;j<4*i+4;j++) {Light[0][j]=true;	Light[1][j]=true;Light[2][j]=true;}
			if(i==0) lstrcpyn(TipsMessage,TEXT("��ע���� 1~12  ( 1 �� 2 )"),sizeof(TCHAR)*64);
			if(i==1) lstrcpyn(TipsMessage,TEXT("��ע���� 13~24  ( 1 �� 2 )"),sizeof(TCHAR)*64);
			if(i==2) lstrcpyn(TipsMessage,TEXT("��ע���� 25~36  ( 1 �� 2 )"),sizeof(TCHAR)*64);
			return true;
		}
	}
	//��������
	for(int j=1;j<12;j++)
	{
		//��һ��
		if(IsInRegion(CrossBetArea[0][j].BasePoint,CrossBetArea[0][j].VertexTop,CrossBetArea[0][j].VertexButtom,CrossBetArea[0][j].VertexRight,point)) 
		{
			Light[0][j]=true;	Light[1][j]=true;	Light[2][j]=true;
			Light[0][j-1]=true;	Light[1][j-1]=true;	Light[2][j-1]=true;
						
			TCHAR szBuffer[64];
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("��ע���� %d,%d,%d, %d,%d,%d ( 1 �� 5 )"),(j-1)*3+1,(j-1)*3+2,(j-1)*3+3,j*3+1,j*3+2,j*3+3 );
			lstrcpyn(TipsMessage,szBuffer,sizeof(TCHAR)*64);
			return true;
		}
		//�ڶ���
		if(IsInRegion(CrossBetArea[1][j].BasePoint,CrossBetArea[1][j].VertexTop,CrossBetArea[1][j].VertexButtom,CrossBetArea[1][j].VertexRight,point)) 
		{
			Light[1][j]=true;		Light[1][j-1]=true;
			Light[0][j]=true;		Light[0][j-1]=true;

			TCHAR szBuffer[64];
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("��ע���� %d, %d, %d, %d ( 1 �� 8 )"),(j-1)*3+1,(j-1)*3+2,j*3+1,j*3+2 );
			lstrcpyn(TipsMessage,szBuffer,sizeof(TCHAR)*64);
			return true;
		}
		//������
		if(IsInRegion(CrossBetArea[2][j].BasePoint,CrossBetArea[2][j].VertexTop,CrossBetArea[2][j].VertexButtom,CrossBetArea[2][j].VertexRight,point)) 
		{
			Light[2][j]=true;		Light[2][j-1]=true;
			Light[1][j]=true;		Light[1][j-1]=true;

			TCHAR szBuffer[64];
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("��ע���� %d, %d, %d, %d ( 1 �� 8 )"),(j-1)*3+2,(j-1)*3+3,j*3+2,j*3+3 );
			lstrcpyn(TipsMessage,szBuffer,sizeof(TCHAR)*64);
			return true;
		}
	}
	//���ཻ����
	for(int j=0;j<12;j++)
	{
		//��һ��
		if(IsInRegion(WideSideCrossArea[0][j].BasePoint,WideSideCrossArea[0][j].VertexTop,WideSideCrossArea[0][j].VertexButtom,WideSideCrossArea[0][j].VertexRight,point))
		{
			Light[0][j]=true;Light[1][j]=true;Light[2][j]=true;
			TCHAR szBuffer[64];
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("��ע���� %d, %d, %d ( 1 �� 11 )"),j*3+1,j*3+2,j*3+3 );
			lstrcpyn(TipsMessage,szBuffer,sizeof(TCHAR)*64);
			return true;
		}
		//�ڶ���
		if(IsInRegion(WideSideCrossArea[1][j].BasePoint,WideSideCrossArea[1][j].VertexTop,WideSideCrossArea[1][j].VertexButtom,WideSideCrossArea[1][j].VertexRight,point))
		{
			Light[0][j]=true;Light[1][j]=true;
			TCHAR szBuffer[64];
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("��ע���� %d, %d ( 1 �� 17 )"),j*3+1,j*3+2 );
			lstrcpyn(TipsMessage,szBuffer,sizeof(TCHAR)*64);
			return true;
		}
		//������
		if(IsInRegion(WideSideCrossArea[2][j].BasePoint,WideSideCrossArea[2][j].VertexTop,WideSideCrossArea[2][j].VertexButtom,WideSideCrossArea[2][j].VertexRight,point))
		{
			Light[2][j]=true;Light[1][j]=true;
			TCHAR szBuffer[64];
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("��ע���� %d, %d ( 1 �� 17 )"),j*3+2,j*3+3 );
			lstrcpyn(TipsMessage,szBuffer,sizeof(TCHAR)*64);
			return true;
		}
	}
	//���ཻ����
	for(int i=0;i<3;i++)
	for(int j=0;j<11;j++)
	{
		if(IsInRegion(HeightSideCrossArea[i][j].BasePoint,HeightSideCrossArea[i][j].VertexTop,HeightSideCrossArea[i][j].VertexButtom,HeightSideCrossArea[i][j].VertexRight,point))
		{
			Light[i][j]=true;Light[i][j+1]=true;
			TCHAR szBuffer[64];
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("��ע���� %d, %d ( 1 �� 17 )"),j*3+i+1,j*3+i+4 );
			lstrcpyn(TipsMessage,szBuffer,sizeof(TCHAR)*64);
			return true;
		}
	}
	//ѭ�����������
	bool InNumberArea=false;
	for(int x=0;x<3;x++)
	for(int y=0;y<12;y++)
	{
		if(IsInRegion(BetNumberArea[x][y].BasePoint,BetNumberArea[x][y].VertexTop,BetNumberArea[x][y].VertexButtom,BetNumberArea[x][y].VertexRight,point)) 
		{
			Light[x][y]=InNumberArea=true;
			TCHAR szBuffer[64];
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("��ע���� %d ( 1 �� 35 )"),y*3+x+1 );
			lstrcpyn(TipsMessage,szBuffer,sizeof(TCHAR)*64);
		}
	}

	return InNumberArea;
}

bool CGameLogic::IsInRegion(CPoint BasePoint,CPoint VertexTop,CPoint VertexButtom,CPoint VertexRight,CPoint point,bool bReverseSlop4)
{
	//��׼�������Ǹõ��ڴ��ڵ����ꣻ
	//������������궼������ڻ�׼������ꣻ
	//���λ��Ҳת��Ϊ������׼������ꣻ
	//
	//							������һ��
	//								��
	//								*
	//                slope1    *		*	  slope3
	//						*				*
	//					*						*
	//����׼�㣩��  *								*  ������������
	//					*						*
	//						*				*
	//			      slope2	*		*	  slope4
	//								*
	//								��
	//							���������

	//ת����׼������Ϊ��Ļ��������
	BasePoint.x=m_nScreenWidth/2+BasePoint.x;
	BasePoint.y=m_nScreenHeight/2+BasePoint.y;

	//ת���������Ϊ����ڻ�׼������
	MousePoint.x=point.x-BasePoint.x;
	MousePoint.y=point.y-BasePoint.y;

	//���������ߵ�б��
	slope1=(float)(VertexTop.y)/(VertexTop.x);
	slope2=(float)(VertexButtom.y)/(VertexButtom.x);
	slope3=(float)(VertexRight.y-VertexTop.y)/(VertexRight.x-VertexTop.x);
	slope4=(float)(VertexRight.y-VertexButtom.y)/(VertexRight.x-VertexButtom.x);

	//�����ұ������ߵĽ���
	float b3=(VertexRight.y)-slope3*VertexRight.x;
	float b4=(VertexRight.y)-slope4*VertexRight.x;

	//0��λ�õ������ı���,��4���߷�ת����
	if(bReverseSlop4)
	{
		if( MousePoint.x>0 && MousePoint.y > MousePoint.x*slope1)	//y��slope1�ұ�  
		if( MousePoint.x>0 && MousePoint.y < MousePoint.x*slope2)	//y��slope2�ұ�
		if( MousePoint.x<(VertexButtom.x)&&MousePoint.y>(MousePoint.x*slope3+b3))//y��slope3���
		if( MousePoint.x<(VertexButtom.x)&&MousePoint.y>(MousePoint.x*slope4+b4))//y��slope4���
		{
			return true;
		}
	}

	//�ж������Ƿ��ڴ��ı���������
	if( MousePoint.x>0 && MousePoint.y > MousePoint.x*slope1)	//y��slope1�ұ�  
	if( MousePoint.x>0 && MousePoint.y < MousePoint.x*slope2)	//y��slope2�ұ�
	if( MousePoint.x<(VertexRight.x)&&MousePoint.y>(MousePoint.x*slope3+b3))//y��slope3���
	if( MousePoint.x<(VertexRight.x)&&MousePoint.y<(MousePoint.x*slope4+b4))//y��slope4���
	{
		return true;
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////

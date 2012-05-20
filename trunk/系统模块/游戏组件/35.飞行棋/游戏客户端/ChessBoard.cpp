#include "StdAfx.h"

#include "ChessBoard.h"

//////////////////////////////////////////////////////////////////////////
//宏定义




//////////////////////////////////////////////////////////////////////////
struct tempTileItemStruct
{
	USHORT							usXPos;						//棋盘格子坐标
	USHORT							usYPos;							//棋盘格子坐标
	USHORT							usTileID;							//Tile ID
};

//构造函数
CChessBoard::CChessBoard()
{

	m_ChessMoveMap[0].shXDirection = 1;
	m_ChessMoveMap[0].shYDirection = 0;

	m_ChessMoveMap[1].shXDirection = 1;
	m_ChessMoveMap[1].shYDirection = 1;

	m_ChessMoveMap[2].shXDirection = 0;
	m_ChessMoveMap[2].shYDirection = 1;

	m_ChessMoveMap[3].shXDirection = -1;
	m_ChessMoveMap[3].shYDirection = 1;

	m_ChessMoveMap[4].shXDirection = -1;
	m_ChessMoveMap[4].shYDirection = 0;

	m_ChessMoveMap[5].shXDirection = -1;
	m_ChessMoveMap[5].shYDirection = -1;

	m_ChessMoveMap[6].shXDirection = 0;
	m_ChessMoveMap[6].shYDirection = -1;

	m_ChessMoveMap[7].shXDirection = 1;
	m_ChessMoveMap[7].shYDirection = -1;
	m_usSpeed = 128;
	memset(m_bPlayStatus, US_FREE, sizeof(m_bPlayStatus));
	memset(m_bViewSeatID, 0, sizeof(m_bViewSeatID));

	return;
}

//析构函数
CChessBoard::~CChessBoard()
{
	CleanUpTexture();
	m_ChessBoard.RemoveAll();
	m_ChessMove.RemoveAll();
	m_CurActiveChess.RemoveAll();
}

//初始化背景
BOOL CChessBoard::InitBackGround(cGraphics *pGraphics){
	return m_ChessBoardTexture.LoadRenderTextureFromResource(pGraphics, IDB_CHESSBOARD_BKG, 750, 720);
}


//初始化棋盘
void CChessBoard::InitChessBoard(cGraphics *pGraphics){
	tagClientTileItem temp;
	//RetailOutput("\nm_CurMeChairID = %d", m_CurMeChairID);

	CArrayTemplate<tempTileItemStruct> tempChessTileArray, tempBaseArray, tempAngleArray, tempArrowArray;
	tempTileItemStruct temp2;
	for(int i=0; i<CHESSCOUNT_H; i++){
		for(int j=0; j<CHESSCOUNT_W; j++){
			if(FLIGHTCHESS_MAP[i][j] < 200 ){
				temp2.usXPos = j * TILE_W;
				temp2.usYPos = i * TILE_H;
				temp2.usTileID = FLIGHTCHESS_MAP[i][j];
				tempChessTileArray.Add(temp2);
			} else if(FLIGHTCHESS_MAP[i][j] >= 200 && FLIGHTCHESS_MAP[i][j] <210) {
				temp2.usXPos = j * TILE_W;
				temp2.usYPos = i * TILE_H;
				temp2.usTileID = FLIGHTCHESS_MAP[i][j] -200;
				tempBaseArray.Add(temp2);
			} else if(FLIGHTCHESS_MAP[i][j] >= 210 && FLIGHTCHESS_MAP[i][j] <220) {
				temp2.usXPos = j * TILE_W;
				temp2.usYPos = i * TILE_H;
				temp2.usTileID = FLIGHTCHESS_MAP[i][j] -210;
				tempAngleArray.Add(temp2);
			} else if(FLIGHTCHESS_MAP[i][j] >= 220 && FLIGHTCHESS_MAP[i][j] != NONE_TILE) {
				temp2.usXPos = j * TILE_W;
				temp2.usYPos = i * TILE_H;
				temp2.usTileID = FLIGHTCHESS_MAP[i][j] -220;
				tempArrowArray.Add(temp2);
			} 
		}
	}
	m_ChessBoard.SetSize(tempChessTileArray.GetCount());
	//int tempNormalChessCount; 
	int tempUpSkyChessCount;
	BYTE MinUpSkyPt = 255, MaxUpSkyPt = 0;
	int i;
	for(/*int*/ i= 0; i<GAME_PLAYER; i++) {
		if(SPECIALTONEXT[i][1] < MinUpSkyPt ) MinUpSkyPt = SPECIALTONEXT[i][1];
		if(SPECIALTONEXT[i][1] > MaxUpSkyPt ) MaxUpSkyPt = SPECIALTONEXT[i][1];
	}
	//tempNormalChessCount =  MinUpSkyPt /GAME_PLAYER;
	tempUpSkyChessCount = (MaxUpSkyPt - MinUpSkyPt) / 3;
//	ASSERT(tempNormalChessCount * 4 +tempUpSkyChessCount*4== tempChessTileArray.GetCount());

	//服务器客户端棋盘统一格式
	for( i= 0; i<= MAX_ROADWAY; i++){
		temp.usXPos = tempChessTileArray.GetAt(i).usXPos;
		temp.usYPos = tempChessTileArray.GetAt(i).usYPos;		
		temp.cbColor = tempChessTileArray.GetAt(i).usTileID <MinUpSkyPt? (tempChessTileArray.GetAt(i).usTileID+m_CurMeChairID +3) %GAME_PLAYER:((tempChessTileArray.GetAt(i).usTileID -MinUpSkyPt) /tempUpSkyChessCount+m_CurMeChairID) %GAME_PLAYER;
		temp.cbChessCount = 0;
		ASSERT(temp.cbColor <= BLUE && temp.cbColor >= RED);
		m_ChessBoard.SetAt(tempChessTileArray.GetAt(i).usTileID <MinUpSkyPt? (tempChessTileArray.GetAt(i).usTileID +m_CurMeChairID*PER_NORMAL_ROADWAY) %MinUpSkyPt:MAX_NORMAL_ROADWAY+((tempChessTileArray.GetAt(i).usTileID -MinUpSkyPt) +m_CurMeChairID*tempUpSkyChessCount)%(MAX_ROADWAY - MAX_NORMAL_ROADWAY)+1, temp);
	}
	CArrayTemplate<tagClientTileItem> tempBases, tempAngles, tempArrows;
	tempBases.SetSize(tempBaseArray.GetCount());
	for( i= 0; i<tempBaseArray.GetCount(); i++){
		temp.cbColor = (tempBaseArray.GetAt(i).usTileID + m_CurMeChairID) %GAME_PLAYER;
		m_BasePosition[temp.cbColor].x =temp.usXPos = tempBaseArray.GetAt(i).usXPos;
		m_BasePosition[temp.cbColor].y = temp.usYPos = tempBaseArray.GetAt(i).usYPos;	
		tempBases.SetAt(temp.cbColor, temp);//服务器客户端棋盘统一格式
	}
	tempAngles.SetSize(tempAngleArray.GetCount());
	for( i= 0; i<tempAngleArray.GetCount(); i++){
		temp.usXPos = tempAngleArray.GetAt(i).usXPos;
		temp.usYPos = tempAngleArray.GetAt(i).usYPos;		
		temp.cbColor = (tempAngleArray.GetAt(i).usTileID) %GAME_PLAYER +2;
		tempAngles.SetAt(tempAngleArray.GetAt(i).usTileID, temp);
	}
	tempArrows.SetSize(tempArrowArray.GetCount());
	for( i= 0; i<tempArrowArray.GetCount(); i++){
		temp.usXPos = tempArrowArray.GetAt(i).usXPos;
		temp.usYPos = tempArrowArray.GetAt(i).usYPos;		
		temp.cbColor = (tempArrowArray.GetAt(i).usTileID) /GAME_PLAYER + m_CurMeChairID +2;
		tempArrows.SetAt(tempArrowArray.GetAt(i).usTileID, temp);
	}
	/*cSurface tempSurface; 
	tempSurface.LoadSurfaceFromResource(pGraphics, IDB_CHESSBOARD_BKG, 728, 699);
	m_ChessBoardSurface.CreateSurface(pGraphics, 728, 699);
	if(!m_ChessBoardSurface.CopySurface(&tempSurface,CRect(0,0,728,699),CRect(0,0,728,699))) {
		MessageBox(NULL, TEXT("场景图片加载失败!"),"",MB_OK);return ;
	}	*/
	//m_ChessBoardSurface.LoadSurfaceFromResource(pGraphics, IDB_CHESSBOARD_BKG, 728, 699);



	CArrayTemplate<tagClientTileItem> tempChessBaseArray;
	BYTE bXOffset, bYOffset;
	SHORT shXStartOffset, shYStartOffset;
	tagClientTileItem * pTempBases;
	tempChessBaseArray.SetSize(GAME_PLAYER * PLAYER_CHESS_COUNT+GAME_PLAYER);
	for(int i=0; i< GAME_PLAYER; i++){
		pTempBases = &tempBases.GetAt(i);
		switch(abs(GAME_PLAYER+pTempBases->cbColor -m_CurMeChairID) %GAME_PLAYER){
		case 0:
			temp.usXPos = pTempBases->usXPos + 6 -2*PLANE_TILE_XOFFSET;
			temp.usYPos = pTempBases->usYPos + PLANE_H -2*PLANE_TILE_YOFFSET;
			bXOffset = PLANE_W + 4;
			bYOffset = PLANE_H + 4;
			shXStartOffset = 2*PLANE_H + 10 -2*PLANE_TILE_XOFFSET;
			shYStartOffset = -2*PLANE_TILE_YOFFSET;
			break;
		case 1:
			temp.usXPos = pTempBases->usXPos + 6 -2*PLANE_TILE_XOFFSET;
			temp.usYPos = pTempBases->usYPos + 6 -2*PLANE_TILE_YOFFSET;
			bXOffset = PLANE_W + 4;
			bYOffset = PLANE_H + 4;
			shXStartOffset = 2*PLANE_H + 16 -2*PLANE_TILE_XOFFSET;
			shYStartOffset = 2*PLANE_H + 10 -2*PLANE_TILE_YOFFSET;
			break;
		case 2:
			temp.usXPos = pTempBases->usXPos + PLANE_W +1 -2*PLANE_TILE_XOFFSET;
			temp.usYPos = pTempBases->usYPos +4 -2*PLANE_TILE_YOFFSET;
			bXOffset = PLANE_W + 4;
			bYOffset = PLANE_H + 4;
			shXStartOffset = 2 -2*PLANE_TILE_XOFFSET;
			shYStartOffset = 2*PLANE_H + 12 -2*PLANE_TILE_YOFFSET;
			break;
		case 3:
			temp.usXPos = pTempBases->usXPos + PLANE_W -2*PLANE_TILE_XOFFSET;
			temp.usYPos = pTempBases->usYPos + PLANE_H -2*PLANE_TILE_YOFFSET;
			bXOffset = PLANE_W + 4;
			bYOffset = PLANE_H + 4;
			shXStartOffset = -2*PLANE_TILE_XOFFSET;
			shYStartOffset = -2*PLANE_TILE_YOFFSET;
			break;
		}
		temp.cbColor = tempBases.GetAt(i).cbColor ;
		temp.cbChessCount = 0;
		tempChessBaseArray.SetAt(temp.cbColor * GAME_PLAYER, temp);
		temp.usXPos += bXOffset;
		tempChessBaseArray.SetAt(temp.cbColor * GAME_PLAYER +1, temp);
		temp.usXPos -= bXOffset;
		temp.usYPos += bYOffset;
		tempChessBaseArray.SetAt(temp.cbColor * GAME_PLAYER +2, temp);
		temp.usXPos += bXOffset;
		tempChessBaseArray.SetAt(temp.cbColor * GAME_PLAYER +3, temp);

		temp.usXPos = pTempBases->usXPos + shXStartOffset;
		temp.usYPos = pTempBases->usYPos + shYStartOffset;
		temp.cbChessCount = 0;
		tempChessBaseArray.SetAt(GAME_PLAYER * PLAYER_CHESS_COUNT+temp.cbColor, temp);
	}


	LPDIRECT3DSURFACE9 pChessBoardSurface = NULL, pBackBuffer = NULL;
	cTexture tempChessTileTexture, tempBaseTexture, tempAngleTexture, tempArrowTexture, tempCentreTexture;
	D3DSURFACE_DESC d3dDesc;
	int ElementWidth, ElementHeight, iArrowXIncrement1, iArrowYIncrement1, iArrowXIncrement2, iArrowYIncrement2;
	
	tempChessTileTexture.LoadRenderTextureFromResource(pGraphics, IDB_CHESS_TILE, 136, 34);
	tempBaseTexture.LoadTextureFromResource(pGraphics, IDB_HOME_DARK, 564, 564, 0xFFFF00FF);
	tempAngleTexture.LoadTextureFromResource(pGraphics, IDB_ANGLE, 136, 34, 0xFFFF00FF);
	tempArrowTexture.LoadTextureFromResource(pGraphics, IDB_ARROW, 44, 44, 0xFFFF00FF);
	tempCentreTexture.LoadTextureFromResource(pGraphics, IDB_CENTRE, 408, 102, 0xFFFF00FF);

	pGraphics->GetDeviceCOM()->GetRenderTarget(0,&pBackBuffer); 
	m_ChessBoardTexture.GetTextureCOM()->GetSurfaceLevel(0, &pChessBoardSurface);
	pGraphics->GetDeviceCOM()->SetRenderTarget(0,pChessBoardSurface);
	
	pGraphics->GetDeviceCOM()->BeginScene();
	pGraphics->BeginSprite(D3DXSPRITE_ALPHABLEND);

	for(int i= 0; i<m_ChessBoard.GetCount(); i++){
		//m_ChessBoardSurface.CopySurface(&m_ChessTileSurface,, \
		//	&CRect( , m_ChessBoard.GetAt(i).usXPos + TILE_W, m_ChessBoard.GetAt(i).usYPos + TILE_H));
		tempChessTileTexture.Blit(&CRect(m_ChessBoard.GetAt(i).cbColor  * TILE_W, 0, m_ChessBoard.GetAt(i).cbColor * TILE_W + TILE_W, TILE_H), m_ChessBoard.GetAt(i).usXPos, m_ChessBoard.GetAt(i).usYPos);

	}
	if( tempBaseTexture.GetTextureCOM()->GetLevelDesc(0, &d3dDesc) !=D3D_OK){
		ASSERT(FALSE);
	}
	//棋盘飞机基地信息
	ElementWidth = d3dDesc.Width /GAME_PLAYER;
	ElementHeight = d3dDesc.Height /GAME_PLAYER;
	for(int i= 0; i< GAME_PLAYER; i++){
		//m_ChessBoardSurface.GetSurfaceFromSurface(&CRect( tempBases.GetAt(i).usXPos, tempBases.GetAt(i).usYPos, tempBases.GetAt(i).usXPos  + ElementWidth, tempBases.GetAt(i).usYPos + ElementHeight),\
		//	&m_BaseSurface, ,0);
		//m_ChessBoardSurface.CopySurface(&m_BaseSurface,  ,\
		//	);
		tempBaseTexture.Blit(&CRect((GAME_PLAYER+tempBases.GetAt(i).cbColor -m_CurMeChairID +1) %GAME_PLAYER * ElementWidth, tempBases.GetAt(i).cbColor * ElementHeight, abs(GAME_PLAYER+tempBases.GetAt(i).cbColor -m_CurMeChairID +1) %GAME_PLAYER * ElementWidth +ElementWidth, tempBases.GetAt(i).cbColor * ElementHeight +ElementHeight ), \
			tempBases.GetAt(i).usXPos, tempBases.GetAt(i).usYPos);
	}

	if( tempAngleTexture.GetTextureCOM()->GetLevelDesc(0, &d3dDesc) !=D3D_OK){
		ASSERT(FALSE);
	}
	ElementWidth = d3dDesc.Width /GAME_PLAYER;
	ElementHeight = d3dDesc.Height;
	for(int i= 0; i< GAME_PLAYER; i++){
		//m_ChessBoardSurface.GetSurfaceFromSurface(&CRect( , tempAngles.GetAt(i).usXPos  + ElementWidth, tempAngles.GetAt(i).usYPos + ElementHeight),\
		//	&m_AngleSurface,  ,0);
		//m_ChessBoardSurface.CopySurface(&, 
		//	);
		tempAngleTexture.Blit(&CRect(((tempAngles.GetAt(i).cbColor) %GAME_PLAYER) * ElementWidth, 0, ((tempAngles.GetAt(i).cbColor) %GAME_PLAYER) * ElementWidth +ElementWidth, ElementHeight ),\
			tempAngles.GetAt(i).usXPos, tempAngles.GetAt(i).usYPos);
	}

	if( tempArrowTexture.GetTextureCOM()->GetLevelDesc(0, &d3dDesc) !=D3D_OK){
		ASSERT(FALSE);
	}
	ElementWidth = d3dDesc.Width /GAME_PLAYER;
	ElementHeight = d3dDesc.Height /GAME_PLAYER;
	for(int i= 0; i< tempArrows.GetCount(); i++){
		if((tempArrows.GetAt(i).cbColor +m_CurMeChairID) %2){
			iArrowXIncrement1 = (TILE_H - ElementHeight) /2;
			iArrowYIncrement1 = (2*TILE_W - ElementWidth) /3;
			iArrowXIncrement2 = (TILE_H - ElementHeight) /2;
			iArrowYIncrement2 = (TILE_W - 2*ElementWidth) /3;
		} else {
			iArrowXIncrement1 = (2*TILE_W - ElementWidth) /3;
			iArrowYIncrement1 = (TILE_H - ElementHeight) /2;
			iArrowXIncrement2 = (TILE_W - 2*ElementWidth) /3;
			iArrowYIncrement2 = (TILE_H - ElementHeight) /2;
		}
	//	m_ChessBoardSurface.GetSurfaceFromSurface(&CRect( tempArrows.GetAt(i).usXPos +iArrowXIncrement1, tempArrows.GetAt(i).usYPos +iArrowYIncrement1, tempArrows.GetAt(i).usXPos  + ElementWidth +iArrowXIncrement1, tempArrows.GetAt(i).usYPos + ElementHeight +iArrowYIncrement1),\
		//	&m_ArrowSurface, , 0);
	//	m_ChessBoardSurface.CopySurface,  ,\
	//		);
		//m_ChessBoardSurface.GetSurfaceFromSurface(&CRect( tempArrows.GetAt(i).usXPos +iArrowXIncrement1, tempArrows.GetAt(i).usYPos +iArrowYIncrement1, tempArrows.GetAt(i).usXPos  + ElementWidth +iArrowXIncrement1, tempArrows.GetAt(i).usYPos + ElementHeight +iArrowYIncrement1),\
		//	&m_ArrowSurface, &CRect(((i/4) %GAME_PLAYER) * ElementWidth, ((tempArrows.GetAt(i).cbColor +1)%GAME_PLAYER) * ElementHeight, ((i/4) %GAME_PLAYER) * ElementWidth +ElementWidth, ((tempArrows.GetAt(i).cbColor +1)%GAME_PLAYER) * ElementHeight +ElementHeight ));
		tempArrowTexture.Blit(&CRect(((i/4) %GAME_PLAYER) * ElementWidth, ((tempArrows.GetAt(i).cbColor +1)%GAME_PLAYER) * ElementHeight, ((i/4) %GAME_PLAYER) * ElementWidth +ElementWidth, ((tempArrows.GetAt(i).cbColor +1)%GAME_PLAYER) * ElementHeight +ElementHeight ),\
			tempArrows.GetAt(i).usXPos +iArrowXIncrement1, tempArrows.GetAt(i).usYPos +iArrowYIncrement1);
		//m_ChessBoardSurface.GetSurfaceFromSurface(&CRect( , tempArrows.GetAt(i).usXPos  + ElementWidth +iArrowXIncrement2, tempArrows.GetAt(i).usYPos + ElementHeight  +iArrowYIncrement2), \
		//	&m_ArrowSurface, &CRect(((i/4) %GAME_PLAYER) * ElementWidth, ((tempArrows.GetAt(i).cbColor +1)%GAME_PLAYER) * ElementHeight, ((i/4) %GAME_PLAYER) * ElementWidth +ElementWidth, ((tempArrows.GetAt(i).cbColor +1)%GAME_PLAYER) * ElementHeight +ElementHeight ) ,0);
		tempArrowTexture.Blit(&CRect(((i/4) %GAME_PLAYER) * ElementWidth, ((tempArrows.GetAt(i).cbColor +1)%GAME_PLAYER) * ElementHeight, ((i/4) %GAME_PLAYER) * ElementWidth +ElementWidth, ((tempArrows.GetAt(i).cbColor +1)%GAME_PLAYER) * ElementHeight +ElementHeight ),\
			tempArrows.GetAt(i).usXPos +iArrowXIncrement2, tempArrows.GetAt(i).usYPos +iArrowYIncrement2);
	}
	tempCentreTexture.Blit(&CRect(m_CurMeChairID * 102, 0, m_CurMeChairID * 102 + 102, 102), 306, 272);

	pGraphics->EndSprite();
	pGraphics->GetDeviceCOM()->EndScene();
	tempChessTileTexture.Free();
	tempBaseTexture.Free();
	tempAngleTexture.Free();
	tempArrowTexture.Free();
	tempCentreTexture.Free();


	//cSurface m_ChessTileSurface, m_BaseSurface,  m_AngleSurface, m_ArrowSurface;

	//m_ChessTileSurface.LoadSurfaceFromResource(pGraphics, IDB_CHESS_TILE, 136, 34, D3DCOLOR_COLORVALUE(1.0f,0.0f,1.0f,0.0f));
	//m_BaseSurface.LoadSurfaceFromResource(pGraphics, IDB_HOME_DARK, 564, 564, D3DCOLOR_COLORVALUE(1.0f,0.0f,1.0f,0.0f));
	
	//m_BaseSurface.CreateSurface(pGraphics, 564, 564);
	//m_BaseSurface.GetSurfaceFromSurface(&CRect(0,0,564, 564), &m_BaseSurface_temp, &CRect(0,0,564, 564),D3DCOLOR_COLORVALUE(1.0f,0.0f,1.0f,0.0f));

	//m_AngleSurface.LoadSurfaceFromResource(pGraphics, IDB_ANGLE, 136, 34, D3DCOLOR_COLORVALUE(1.0f,0.0f,1.0f,0.0f));
	//m_ArrowSurface.LoadSurfaceFromResource(pGraphics, IDB_ARROW, 44, 44, D3DCOLOR_COLORVALUE(1.0f,0.0f,1.0f,1.0f));
	//m_PlaneSurface_temp.LoadSurfaceFromResource(pGraphics, IDB_PLANE, 344, 172, D3DCOLOR_COLORVALUE(1.0f,0.0f,1.0f,0.0f));
	//m_PlaneTexture.LoadSurfaceFromResource(pGraphics, IDB_PLANE, 344, 172, D3DCOLOR_COLORVALUE(1.0f,0.0f,1.0f,0.0f));

	cTexture tempPlaneTexture1, tempPlaneTexture2, tempReturnStar;
	LPDIRECT3DSURFACE9 ptempPlaneSurface, ptempFinishPlaneSurface;
	USHORT usWidth, usHeight;
	m_PlaneTexture.LoadRenderTextureFromResource(pGraphics, IDB_PLANE, 344, 172, 0xFFFF00FF);
	//LPDIRECT3DSURFACE9  pPlaneSurface9;
	tempPlaneTexture2.LoadTextureFromResource(pGraphics, IDB_PLANE, 344, 172, 0xFFFF00FF);

	if(tempPlaneTexture1.LoadDynamicTextureFromResource(pGraphics, IDB_PLANE, 344, 172, 0xFFFF00FF)){
		//m_PlaneTexture.GetTextureCOM()->GetSurfaceLevel(0, &pPlaneSurface9);
		//m_BkChessBoardSurface.CreateSurface(pGraphics, 728, 699);
		//m_BkChessBoardSurface.CopySurface(&m_ChessBoardSurface, CRect(0, 0, 728, 699), CRect(0, 0, 728, 699));
		//pPlaneSurface.SetSurface(pGraphics, pPlaneSurface9);
		//pPlaneSurface.BlackSurface(&CRect(0, 0, 344, 172));
		tempPlaneTexture1.BlackTexture(&CRect(0, 0, 344, 172));
		m_PlaneTexture.GetTextureCOM()->GetSurfaceLevel(0, &ptempPlaneSurface);

		pGraphics->GetDeviceCOM()->SetRenderTarget(0, ptempPlaneSurface); 
		pGraphics->GetDeviceCOM()->BeginScene();

		pGraphics->BeginSprite(D3DXSPRITE_ALPHABLEND);

		for(int i= 0; i< 8; i++){
			usWidth = i*PLANE_W;
			for(int j= 0; j< 4; j++){
				usHeight = j*PLANE_H;
				tempPlaneTexture1.Blit(&CRect(usWidth, usHeight, usWidth +43, usHeight +43), usWidth +2, usHeight +1);
				tempPlaneTexture2.Blit(&CRect(usWidth, usHeight, usWidth +43, usHeight +43), usWidth, usHeight);
				//m_PlaneTexture.GetTextureFromTexture(&CRect(i*43 +3, j*43 +3, i*43 +43, j*43+43), tempPlaneTexture, &CRect(i*43, j*43, i*43 +43, j*43+43));
			}
		}	
		pGraphics->EndSprite();
		pGraphics->GetDeviceCOM()->EndScene();
	} 


	tempReturnStar.LoadRenderTextureFromResource(pGraphics, IDB_RETURN_STAR, 26, 25, 0xFFFF00FF);
	m_FinishPlaneTexture.CreateRenderTexture(pGraphics, PLANE_W*4, PLANE_H);
	m_FinishPlaneTexture.GetTextureCOM()->GetSurfaceLevel(0, &ptempFinishPlaneSurface);

	pGraphics->GetDeviceCOM()->SetRenderTarget(0, ptempFinishPlaneSurface);
	pGraphics->GetDeviceCOM()->BeginScene();
	pGraphics->GetDeviceCOM()->Clear(0,NULL,D3DCLEAR_TARGET ,0x00000000,1.0f,0); //clear texture
	pGraphics->BeginSprite(D3DXSPRITE_ALPHABLEND);
	
	for(int i =0; i< GAME_PLAYER; i++){
		usWidth = 2*abs(GAME_PLAYER+i - m_CurMeChairID -1) %(2*GAME_PLAYER) * PLANE_W;
		usHeight = i*PLANE_H;
		tempPlaneTexture2.Blit(&CRect(usWidth, usHeight, usWidth +PLANE_W, usHeight +PLANE_H), usHeight, 0);
		tempReturnStar.Blit(&CRect(0, 0, 26, 25), usHeight +8, 9);
	}
	pGraphics->EndSprite();
	pGraphics->GetDeviceCOM()->EndScene();
	//m_PlaneTexture.GetSurfaceFromSurface(&CRect( 0, 0, 344 -3, 172 -3), &m_PlaneSurface_temp, &CRect( 3, 3, 344, 172), 0);

	/*pGraphics->BeginSprite(D3DXSPRITE_ALPHABLEND);
	for(int i= 0; i< tempChessBaseArray.GetCount() -GAME_PLAYER; i++) {
		//m_ChessBoardSurface.GetSurfaceFromSurface(&CRect( tempChessBaseArray.GetAt(i).usXPos+3, tempChessBaseArray.GetAt(i).usYPos+3, tempChessBaseArray.GetAt(i).usXPos + PLANE_W+3, tempChessBaseArray.GetAt(i).usYPos + PLANE_H+3),\
		//  &m_PlaneSurface_temp, &CRect((2*(3+i/GAME_PLAYER) %(2*GAME_PLAYER))* PLANE_W, ((tempChessBaseArray.GetAt(i).cbColor)%GAME_PLAYER) * PLANE_H, (2*(i/GAME_PLAYER+3)%(2*GAME_PLAYER)) * PLANE_W + PLANE_W, ((tempChessBaseArray.GetAt(i).cbColor)%GAME_PLAYER) * PLANE_H + PLANE_H), 0);
		/*pGraphics->GetSpriteCOM()->Draw(m_PlaneTexture.GetTextureCOM(), \
			&CRect((2*abs(GAME_PLAYER+m_ClientChessItem[i/GAME_PLAYER][i%PLAYER_CHESS_COUNT].cbColor - m_CurMeChairID -1) %(2*GAME_PLAYER))* PLANE_W, m_ClientChessItem[i/GAME_PLAYER][i%PLAYER_CHESS_COUNT].cbColor * PLANE_H, (2*abs(GAME_PLAYER+m_ClientChessItem[i/GAME_PLAYER][i%PLAYER_CHESS_COUNT].cbColor - m_CurMeChairID -1)%(2*GAME_PLAYER)) * PLANE_W + PLANE_W, m_ClientChessItem[i/GAME_PLAYER][i%PLAYER_CHESS_COUNT].cbColor * PLANE_H + PLANE_H),\
			NULL, &D3DXVECTOR3((int)m_ClientChessItem[i/GAME_PLAYER][i%PLAYER_CHESS_COUNT].fXPos, (int)m_ClientChessItem[i/GAME_PLAYER][i%PLAYER_CHESS_COUNT].fYPos, 0), 0xFFFFFFFF);
		
		m_PlaneTexture.Blit(&CRect((2*abs(GAME_PLAYER+m_ClientChessItem[i/GAME_PLAYER][i%PLAYER_CHESS_COUNT].cbColor - m_CurMeChairID -1) %(2*GAME_PLAYER))* PLANE_W, m_ClientChessItem[i/GAME_PLAYER][i%PLAYER_CHESS_COUNT].cbColor * PLANE_H, (2*abs(GAME_PLAYER+m_ClientChessItem[i/GAME_PLAYER][i%PLAYER_CHESS_COUNT].cbColor - m_CurMeChairID -1)%(2*GAME_PLAYER)) * PLANE_W + PLANE_W, m_ClientChessItem[i/GAME_PLAYER][i%PLAYER_CHESS_COUNT].cbColor * PLANE_H + PLANE_H), \
			(int)m_ClientChessItem[i/GAME_PLAYER][i%PLAYER_CHESS_COUNT].fXPos, (int)m_ClientChessItem[i/GAME_PLAYER][i%PLAYER_CHESS_COUNT].fYPos);
	}
	pGraphics->EndSprite();*/
	//m_PlaneSurface_temp.GreySurface()
	pGraphics->GetDeviceCOM()->SetRenderTarget(0,pBackBuffer); //set back buffer
	m_ChessBoard.Append(tempChessBaseArray);
	tempChessBaseArray.RemoveAll();
	tempChessTileArray.RemoveAll(); 
	tempBaseArray.RemoveAll(); 
	tempAngleArray.RemoveAll(); 
	tempArrowArray.RemoveAll();

	tempPlaneTexture1.Free();
	tempPlaneTexture2.Free();
	tempReturnStar.Free();
	//pPlaneSurface.Free();
	//pPlaneSurface9->Release();
	//tempPlaneTexture.Free();

}


//////////////////////////////////////////////////////////////////////////


/*void CChessBoard::PrepareMoveChess(BYTE randNum){

	ChessRouteGen( randNum);

}*/

void CChessBoard::MakeChessMove(DWORD ElapseTime)
{
	float fBaseSpeed = (float)(m_usSpeed * ElapseTime /1000.0);
	USHORT usLastXPos, usLastYPos;
	BOOL bIsAllFinished = TRUE;
	ChessMoveRoute *pTempChessMoveRoute;
	float fNextXPos, fNextYPos;
	ChessMove *pChessMove;
	for(int i=0; i< m_ChessMove.GetCount(); i++)
	{	
		pChessMove = &m_ChessMove.GetAt(i);
		if(pChessMove->m_MoveStatus ==CHESSMOVE_NOT_FINISH){
			bIsAllFinished = FALSE;
			if(pChessMove->m_CurFrame < m_ChessMove[i].m_ChessMoveRoute.GetCount()){
				pTempChessMoveRoute = &(pChessMove->m_ChessMoveRoute.GetAt(pChessMove->m_CurFrame));
				/*RetailOutput("\n=%d, =%d, =%d, =%d", (int)((m_ClientChessItem[m_CurPlayerID][pChessMove->m_cbChessID].fXPos + m_ChessMoveMap[pTempChessMoveRoute->cbMoveMapNum].shXDirection * fBaseSpeed ) * m_ChessMoveMap[pTempChessMoveRoute->cbMoveMapNum].shXDirection), \
				(int)((m_ClientChessItem[m_CurPlayerID][pChessMove->m_cbChessID].fYPos + m_ChessMoveMap[pTempChessMoveRoute->cbMoveMapNum].shYDirection * fBaseSpeed ) * m_ChessMoveMap[pTempChessMoveRoute->cbMoveMapNum].shYDirection), \
				(int)(pTempChessMoveRoute->usXPos * m_ChessMoveMap[pTempChessMoveRoute->cbMoveMapNum].shXDirection), \
				(int)(pTempChessMoveRoute->usYPos * m_ChessMoveMap[pTempChessMoveRoute->cbMoveMapNum].shYDirection));
				RetailOutput("\n=%d, =%d", pTempChessMoveRoute->usXPos, pTempChessMoveRoute->usYPos);*/
				fNextXPos = m_ClientChessItem[pChessMove->m_wPlayerID][pChessMove->m_cbChessID].fXPos + m_ChessMoveMap[pTempChessMoveRoute->cbMoveMapNum].shXDirection * fBaseSpeed * pTempChessMoveRoute->fXIncrement;
				fNextYPos = m_ClientChessItem[pChessMove->m_wPlayerID][pChessMove->m_cbChessID].fYPos + m_ChessMoveMap[pTempChessMoveRoute->cbMoveMapNum].shYDirection * fBaseSpeed * pTempChessMoveRoute->fYIncrement;
				if((int)( fNextXPos * m_ChessMoveMap[pTempChessMoveRoute->cbMoveMapNum].shXDirection )>= (int)(pTempChessMoveRoute->usXPos * m_ChessMoveMap[pTempChessMoveRoute->cbMoveMapNum].shXDirection) &&\
					(int)( fNextYPos * m_ChessMoveMap[pTempChessMoveRoute->cbMoveMapNum].shYDirection) >= (int)(pTempChessMoveRoute->usYPos * m_ChessMoveMap[pTempChessMoveRoute->cbMoveMapNum].shYDirection) ){
						pChessMove->m_CurFrame ++;

						usLastXPos = (USHORT)m_ClientChessItem[pChessMove->m_wPlayerID][pChessMove->m_cbChessID].fXPos;
						usLastYPos = (USHORT)m_ClientChessItem[pChessMove->m_wPlayerID][pChessMove->m_cbChessID].fYPos;
						m_ClientChessItem[pChessMove->m_wPlayerID][pChessMove->m_cbChessID].fXPos = pTempChessMoveRoute->usXPos;
						m_ClientChessItem[pChessMove->m_wPlayerID][pChessMove->m_cbChessID].fYPos = pTempChessMoveRoute->usYPos;
						m_ClientChessItem[pChessMove->m_wPlayerID][pChessMove->m_cbChessID].usDirection = pTempChessMoveRoute->cbMoveMapNum * PLANE_W;
						if(pTempChessMoveRoute->cbChessMoveFlag == CHESSMOVE_TO_FINISH){
							::SendMessage(AfxGetMainWnd()->m_hWnd,IDM_SOUND_MUSIC,IDM_SOUND_ARRIVE,0);
							m_ClientChessItem[pChessMove->m_wPlayerID][pChessMove->m_cbChessID].cbChessStatus = CHESS_FINISHED;
						} else if(pTempChessMoveRoute->cbChessMoveFlag != CHESSMOVE_NOT_DEFINE){
							::SendMessage(AfxGetMainWnd()->m_hWnd,IDM_SOUND_MUSIC,IDM_SOUND_CRASH,0);
							for(BYTE j =pTempChessMoveRoute->cbRefChessMove; j < pTempChessMoveRoute->cbRefChessMove + pTempChessMoveRoute->cbChessMoveFlag; j++){
						
								if(!m_ChessMove.GetAt(j).m_cbBeRefedCount){
									m_ChessMove.GetAt(j).m_MoveStatus = CHESSMOVE_NOT_FINISH;
								} else if(!--m_ChessMove.GetAt(j).m_cbBeRefedCount){
									m_ChessMove.GetAt(j).m_MoveStatus = CHESSMOVE_NOT_FINISH;
								}
							}
						}
					} else {
						usLastXPos = (USHORT)m_ClientChessItem[pChessMove->m_wPlayerID][pChessMove->m_cbChessID].fXPos;
						usLastYPos = (USHORT)m_ClientChessItem[pChessMove->m_wPlayerID][pChessMove->m_cbChessID].fYPos;
						m_ClientChessItem[pChessMove->m_wPlayerID][pChessMove->m_cbChessID].fXPos = fNextXPos;
						m_ClientChessItem[pChessMove->m_wPlayerID][pChessMove->m_cbChessID].fYPos = fNextYPos;
						m_ClientChessItem[pChessMove->m_wPlayerID][pChessMove->m_cbChessID].usDirection = pTempChessMoveRoute->cbMoveMapNum * PLANE_W;
						//RetailOutput("\nusLastXPos= %d, usLastYPos = %d, nextXPos= %d, nextYPos=%d", usLastXPos, usLastYPos,m_ClientChessItem[pChessMove->m_cbPlayerID][pChessMove->m_cbChessID].fXPos ,m_ClientChessItem[pChessMove->m_cbPlayerID][pChessMove->m_cbChessID].fYPos );
					}
					//RetailOutput("\ncbMoveMapNum=%d, =%d", pTempChessMoveRoute->cbMoveMapNum, m_ChessMoveMap[pTempChessMoveRoute->cbMoveMapNum].shXDirection );


			} else {
				pChessMove->m_MoveStatus =  CHESSMOVE_HAS_FINISHED;
				if(pChessMove->m_cbRefChessMove != CHESSMOVE_NOT_DEFINE) {

					if(!m_ChessMove.GetAt(pChessMove->m_cbRefChessMove).m_cbBeRefedCount)
						m_ChessMove.GetAt(pChessMove->m_cbRefChessMove).m_MoveStatus = CHESSMOVE_NOT_FINISH;
					else if(!--m_ChessMove.GetAt(pChessMove->m_cbRefChessMove).m_cbBeRefedCount){
						m_ChessMove.GetAt(pChessMove->m_cbRefChessMove).m_MoveStatus = CHESSMOVE_NOT_FINISH;
					}
				}
				if(m_ClientChessItem[pChessMove->m_wPlayerID][pChessMove->m_cbChessID].cbTileIndex > MAX_ROADWAY && m_ClientChessItem[pChessMove->m_wPlayerID][pChessMove->m_cbChessID].cbTileIndex <= MAX_ROADWAY_BASE){
					m_ClientChessItem[pChessMove->m_wPlayerID][pChessMove->m_cbChessID].usDirection = 2*(GAME_PLAYER+pChessMove->m_wPlayerID - m_CurMeChairID -1) %(2*GAME_PLAYER) * PLANE_W;
				}
			}
		}
	}
	if(bIsAllFinished == TRUE)	m_ChessMove.RemoveAll();
	//Debuglog.LogOutput("\nMakeChessMove m_CurPlayerID = %d", m_CurPlayerID);
}

void CChessBoard::BlitChess()
{
	for(int i= 0; i< GAME_PLAYER; i++) {
		if( m_bPlayStatus[i] == US_FREE) continue;
		for(int j= 0; j < GAME_PLAYER; j++){
			if(m_ClientChessItem[i][j].cbChessStatus == CHESS_NOT_FINISHED){
				m_PlaneTexture.Blit(&CRect(m_ClientChessItem[i][j].usDirection, m_ClientChessItem[i][j].cbColor * PLANE_H, m_ClientChessItem[i][j].usDirection + PLANE_W, m_ClientChessItem[i][j].cbColor * PLANE_H + PLANE_H), \
					(int)m_ClientChessItem[i][j].fXPos, (int)m_ClientChessItem[i][j].fYPos);
			} else m_FinishPlaneTexture.Blit(&CRect(m_ClientChessItem[i][j].cbColor * PLANE_W, 0, m_ClientChessItem[i][j].cbColor * PLANE_W + PLANE_W, PLANE_H), (int)m_ClientChessItem[i][j].fXPos, (int)m_ClientChessItem[i][j].fYPos);
		}
	}
}

void CChessBoard::SwitchCurrentUser(WORD wPlayerID)
{
	m_CurPlayerID = wPlayerID;

}


void CChessBoard::SwitchCurrentChess(BYTE cbChessID)
{
	m_CurChessID = cbChessID;

}
void CChessBoard::SetCurMeChairID(BYTE cbChairID)
{
	m_CurMeChairID = cbChairID;
	for(BYTE i =0; i< GAME_PLAYER; i++){
		m_bViewSeatID[i] = (GAME_PLAYER + i - cbChairID) %GAME_PLAYER;
	}
}


BOOL CChessBoard::ChessRouteGen( BYTE randNum ){
	BYTE cbCurRouteTileID, cbLastRouteTileID;
	ChessMoveRoute tempChessMovePoint;
	tagClientChessItem &tcciChess = m_ClientChessItem[m_CurPlayerID][m_CurChessID];
	//if(tcciChess.cbChessStatus == CHESS_FINISHED) return TRUE;
	BYTE cbUpSkySpecial =0, cbCurMoveMapNum, cbLastMoveMapNum, cbDestination, cbUpSkyDownSpecial =0, cbIsFinished = false;
//	Debuglog.LogOutput("\nrandNum = %d, m_CurPlayerID = %d, tcciChess.cbChessID = %d, tcciChess.cbTileIndex = %d",randNum, m_CurPlayerID, tcciChess.cbChessID, tcciChess.cbTileIndex );
	//if(randNum < 1) return FALSE;
	ASSERT(randNum >= 1 && randNum <= MAX_RANDNUM);
	cbLastRouteTileID= tcciChess.cbTileIndex;

	if(cbLastRouteTileID >MAX_ROADWAY && cbLastRouteTileID <= MAX_ROADWAY_BASE){
		if(randNum == MAX_RANDNUM ){
			::SendMessage(AfxGetMainWnd()->m_hWnd,IDM_SOUND_MUSIC,IDM_SOUND_LANDOFF,0);
			cbCurRouteTileID = MAX_ROADWAY_BASE + tcciChess.cbColor +1;
			IncrementGen(m_ChessBoard.GetAt(cbLastRouteTileID), m_ChessBoard.GetAt(cbCurRouteTileID), tempChessMovePoint.fXIncrement, tempChessMovePoint.fYIncrement);
			cbDestination =cbCurRouteTileID;

			randNum =0;
			if(m_CurMeChairID == tcciChess.cbColor) {
				m_CurActiveChess.Add(m_CurChessID);
			}
		} else return TRUE;

	} else if(cbLastRouteTileID > MAX_ROADWAY_BASE) {
		cbCurRouteTileID = tcciChess.cbColor * PER_NORMAL_ROADWAY;
		IncrementGen(m_ChessBoard.GetAt(cbLastRouteTileID), m_ChessBoard.GetAt(cbCurRouteTileID), tempChessMovePoint.fXIncrement, tempChessMovePoint.fYIncrement);
		randNum--;

	} else {
		if(cbLastRouteTileID ==SPECIALPULL[tcciChess.cbColor][1]){
			cbCurRouteTileID = SPECIALTONEXT[tcciChess.cbColor][1];
		} else {
			cbCurRouteTileID = cbLastRouteTileID>MAX_NORMAL_ROADWAY?cbLastRouteTileID+1:(cbLastRouteTileID +1) %(MAX_NORMAL_ROADWAY+1);
		}

		tempChessMovePoint.fXIncrement = tempChessMovePoint.fYIncrement = 1.0f;
		randNum--;

	}
	tempChessMovePoint.cbMoveMapNum = cbLastMoveMapNum = DirectionMapNum(m_ChessBoard.GetAt(cbLastRouteTileID), m_ChessBoard.GetAt(cbCurRouteTileID));
	cbLastRouteTileID = cbCurRouteTileID;


	BYTE tempChessMoveCount = (BYTE)m_ChessMove.GetCount(), cbTempCount;
	SHORT shBeEatedTileIndex = -1;
	m_ChessMove.SetSize(tempChessMoveCount +1);
	m_ChessMove.GetAt(tempChessMoveCount).m_cbChessID = m_CurChessID;
	m_ChessMove.GetAt(tempChessMoveCount).m_wPlayerID = m_CurPlayerID;
	m_ChessMove.GetAt(tempChessMoveCount).m_cbRefChessMove =CHESSMOVE_NOT_DEFINE;
	CArrayTemplate<ChessMoveRoute> &pRoute = m_ChessMove.GetAt(m_ChessMove.GetUpperBound()).m_ChessMoveRoute;
	CArrayTemplate<BeEatedTile> aBeEatedTile;
	BYTE i;
	for(/*BYTE*/ i =0; i< tempChessMoveCount; i++){
		if(m_ChessMove.GetAt(i).m_cbChessID == m_CurChessID && m_ChessMove.GetAt(i).m_wPlayerID == m_CurPlayerID && m_ChessMove.GetAt(i).m_cbRefChessMove == CHESSMOVE_NOT_DEFINE && m_ChessMove.GetAt(i).m_MoveStatus !=CHESSMOVE_HAS_FINISHED ){
			break;
		}
	}

	if(i == tempChessMoveCount)	{
		m_ChessMove.GetAt(tempChessMoveCount).m_MoveStatus = CHESSMOVE_NOT_FINISH;
		m_ChessMove.GetAt(tempChessMoveCount).m_cbBeRefedCount = 0;
	}
	else {
		m_ChessMove.GetAt(i).m_cbRefChessMove = tempChessMoveCount;
		m_ChessMove.GetAt(tempChessMoveCount).m_cbBeRefedCount = 1;
		m_ChessMove.GetAt(tempChessMoveCount).m_MoveStatus = CHESSMOVE_NOT_ACTIVE;
	}


	if(cbLastRouteTileID <=MAX_NORMAL_ROADWAY){
	for(BYTE i= 0; i<= randNum; i++){
		if((cbLastRouteTileID + i)%(MAX_NORMAL_ROADWAY+1) == SPECIALPULL[tcciChess.cbColor][1] ){
			cbUpSkySpecial = SPECIALPULL[tcciChess.cbColor][1];
			break;
		}
	}
	}
	if(cbUpSkySpecial){
		cbDestination = SPECIALTONEXT[tcciChess.cbColor][1]+(randNum -1 - cbUpSkySpecial+cbLastRouteTileID);
		if(cbDestination == SPECIALTONEXT[tcciChess.cbColor][1] -1) cbDestination =SPECIALPULL[tcciChess.cbColor][1];
	}
	else {
		if(cbLastRouteTileID <=MAX_NORMAL_ROADWAY){
			cbDestination = (cbLastRouteTileID+randNum) %(MAX_NORMAL_ROADWAY+1);
			if(m_ChessBoard.GetAt(cbDestination).cbChessCount > 0){
				cbTempCount = BeKilledRouteGen(cbDestination, m_ChessBoard.GetAt(cbDestination).cbChessCount, tcciChess.cbColor);
				if(cbTempCount){
					aBeEatedTile.Add(BeEatedTile(cbTempCount, cbDestination, (BYTE)(m_ChessMove.GetCount() - cbTempCount)));
				}
			}
			if(cbDestination == SPECIALPULL[tcciChess.cbColor][0]){
				if(m_ChessBoard.GetAt(SPECIALTONEXT[tcciChess.cbColor][2]).cbChessCount > 0){
					cbTempCount = BeKilledRouteGen(SPECIALTONEXT[tcciChess.cbColor][2], m_ChessBoard.GetAt(SPECIALTONEXT[tcciChess.cbColor][2]).cbChessCount, tcciChess.cbColor);
					if(cbTempCount){
						aBeEatedTile.Add(BeEatedTile(cbTempCount, SPECIALTONEXT[tcciChess.cbColor][2], (BYTE)(m_ChessMove.GetCount() - cbTempCount)));
					}
				}
				cbDestination =SPECIALTONEXT[tcciChess.cbColor][0];
				if(m_ChessBoard.GetAt(cbDestination).cbChessCount > 0){
					cbTempCount = BeKilledRouteGen(cbDestination, m_ChessBoard.GetAt(cbDestination).cbChessCount, tcciChess.cbColor);
					if(cbTempCount){
						aBeEatedTile.Add(BeEatedTile(cbTempCount, cbDestination, (BYTE)(m_ChessMove.GetCount() - cbTempCount)));
					}
				} 
			}
			if(m_ChessBoard.GetAt(cbDestination).cbColor == tcciChess.cbColor){
				cbDestination = (cbDestination+ 4)%(MAX_NORMAL_ROADWAY+1);
				if(m_ChessBoard.GetAt(cbDestination).cbChessCount > 0){
					cbTempCount = BeKilledRouteGen(cbDestination, m_ChessBoard.GetAt(cbDestination).cbChessCount, tcciChess.cbColor);
					if(cbTempCount){
						aBeEatedTile.Add(BeEatedTile(cbTempCount, cbDestination,(BYTE)(m_ChessMove.GetCount() - cbTempCount)));
					}
				}
			}
			if(cbDestination == SPECIALPULL[tcciChess.cbColor][0]){
				if(m_ChessBoard.GetAt(SPECIALTONEXT[tcciChess.cbColor][2]).cbChessCount > 0){
					cbTempCount = BeKilledRouteGen(SPECIALTONEXT[tcciChess.cbColor][2], m_ChessBoard.GetAt(SPECIALTONEXT[tcciChess.cbColor][2]).cbChessCount, tcciChess.cbColor);
					if(cbTempCount){
						aBeEatedTile.Add(BeEatedTile(cbTempCount, SPECIALTONEXT[tcciChess.cbColor][2], (BYTE)(m_ChessMove.GetCount() - cbTempCount)));
					}
				}
				cbDestination = SPECIALTONEXT[tcciChess.cbColor][0];
				if(m_ChessBoard.GetAt(cbDestination).cbChessCount > 0){
					cbTempCount = BeKilledRouteGen(cbDestination, m_ChessBoard.GetAt(cbDestination).cbChessCount, tcciChess.cbColor);
					if(cbTempCount){
						aBeEatedTile.Add(BeEatedTile(cbTempCount, cbDestination, (BYTE)(m_ChessMove.GetCount() - cbTempCount)));
					}
				}
			}
		} else if(cbLastRouteTileID <= MAX_ROADWAY){
			if(cbLastRouteTileID+randNum > SPECIALTONEXT[tcciChess.cbColor][1] +UP_SKY_STEP-1){
				cbUpSkyDownSpecial = 2*SPECIALTONEXT[tcciChess.cbColor][1] + 2*UP_SKY_STEP-2 - randNum  -cbLastRouteTileID; 
				cbDestination = cbUpSkyDownSpecial;
			} else {
				cbDestination = cbLastRouteTileID+randNum;
			}
		}

	}

//	Debuglog.LogOutput("\ncbUpSkyDownSpecial = %d, cbUpSkySpecial = %d, cbDestination = %d",cbUpSkyDownSpecial, cbUpSkySpecial, cbDestination );
	//飞机没有倒退路径的出现
	if(!cbUpSkyDownSpecial){
	if(aBeEatedTile.GetCount()) shBeEatedTileIndex =0;
	while(cbLastRouteTileID != cbDestination){
		//飞机要转入起飞线路
		if (cbUpSkySpecial){
			if(cbLastRouteTileID == cbUpSkySpecial){
				cbUpSkySpecial = 0;
				cbCurRouteTileID = SPECIALTONEXT[tcciChess.cbColor][1];
				continue;
			}
			cbCurRouteTileID = (cbLastRouteTileID +1) %(MAX_NORMAL_ROADWAY+1);
			cbCurMoveMapNum = DirectionMapNum(m_ChessBoard.GetAt(cbLastRouteTileID), m_ChessBoard.GetAt(cbCurRouteTileID));
			if( cbLastMoveMapNum != cbCurMoveMapNum){
				tempChessMovePoint.usXPos = m_ChessBoard.GetAt(cbLastRouteTileID).usXPos + PLANE_TILE_XOFFSET;
				tempChessMovePoint.usYPos = m_ChessBoard.GetAt(cbLastRouteTileID).usYPos + PLANE_TILE_YOFFSET;
				tempChessMovePoint.cbChessMoveFlag = CHESSMOVE_NOT_DEFINE;

				pRoute.Add(tempChessMovePoint);
				tempChessMovePoint.cbMoveMapNum = cbCurMoveMapNum;
				tempChessMovePoint.fXIncrement = 1.0f;
				tempChessMovePoint.fYIncrement = 1.0f;

				cbLastMoveMapNum = cbCurMoveMapNum;
			} 
			cbLastRouteTileID = cbCurRouteTileID;

		} else {//飞机还没有进入起飞线路
			if(cbCurRouteTileID <= MAX_NORMAL_ROADWAY || cbDestination == SPECIALTONEXT[tcciChess.cbColor][0] || cbDestination == SPECIALTONEXT[tcciChess.cbColor][0] +4){
				if((cbDestination == SPECIALTONEXT[tcciChess.cbColor][0] || cbDestination == SPECIALTONEXT[tcciChess.cbColor][0] +4)&& cbLastRouteTileID == SPECIALPULL[tcciChess.cbColor][0]){
					cbCurRouteTileID = SPECIALTONEXT[tcciChess.cbColor][2];
				} else if(cbLastRouteTileID == SPECIALTONEXT[tcciChess.cbColor][2]) cbCurRouteTileID =SPECIALTONEXT[tcciChess.cbColor][0];
				else cbCurRouteTileID = (cbLastRouteTileID +1) %(MAX_NORMAL_ROADWAY+1);
			} else {
				if(cbLastRouteTileID == cbCurRouteTileID ){
					cbCurRouteTileID = cbLastRouteTileID +1;
				}
			}
			cbCurMoveMapNum = DirectionMapNum(m_ChessBoard.GetAt(cbLastRouteTileID), m_ChessBoard.GetAt(cbCurRouteTileID));
			if(shBeEatedTileIndex != -1 && shBeEatedTileIndex <aBeEatedTile.GetCount() && cbLastRouteTileID == aBeEatedTile.GetAt(shBeEatedTileIndex).cbTileIndex) {
				tempChessMovePoint.usXPos = m_ChessBoard.GetAt(cbLastRouteTileID).usXPos + PLANE_TILE_XOFFSET;
				tempChessMovePoint.usYPos = m_ChessBoard.GetAt(cbLastRouteTileID).usYPos + PLANE_TILE_YOFFSET;
				tempChessMovePoint.cbChessMoveFlag = aBeEatedTile.GetAt(shBeEatedTileIndex).cbChessCount;
				tempChessMovePoint.cbRefChessMove = aBeEatedTile.GetAt(shBeEatedTileIndex).cbRefChessMove;

				pRoute.Add(tempChessMovePoint);
				tempChessMovePoint.cbMoveMapNum = cbCurMoveMapNum;
				tempChessMovePoint.fXIncrement = 1.0f;
				tempChessMovePoint.fYIncrement = 1.0f;

				cbLastMoveMapNum = cbCurMoveMapNum;
				shBeEatedTileIndex ++;
			}  else{

				if( cbLastMoveMapNum != cbCurMoveMapNum){
					tempChessMovePoint.usXPos = m_ChessBoard.GetAt(cbLastRouteTileID).usXPos + PLANE_TILE_XOFFSET;
					tempChessMovePoint.usYPos = m_ChessBoard.GetAt(cbLastRouteTileID).usYPos + PLANE_TILE_YOFFSET;
					tempChessMovePoint.cbChessMoveFlag = CHESSMOVE_NOT_DEFINE;

					pRoute.Add(tempChessMovePoint);
					tempChessMovePoint.cbMoveMapNum = cbCurMoveMapNum;
					tempChessMovePoint.fXIncrement = 1.0f;
					tempChessMovePoint.fYIncrement = 1.0f;

					cbLastMoveMapNum = cbCurMoveMapNum;
				} 
			}
			cbLastRouteTileID = cbCurRouteTileID;
		}
	}
	if(cbDestination == SPECIALTONEXT[tcciChess.cbColor][1] + UP_SKY_STEP-1){
		cbLastRouteTileID = SPECIALTONEXT[tcciChess.cbColor][1] + UP_SKY_STEP-1;
		tempChessMovePoint.usXPos = m_ChessBoard.GetAt(cbLastRouteTileID).usXPos + PLANE_TILE_XOFFSET;
		tempChessMovePoint.usYPos = m_ChessBoard.GetAt(cbLastRouteTileID).usYPos + PLANE_TILE_YOFFSET;
		tempChessMovePoint.cbChessMoveFlag = CHESSMOVE_NOT_DEFINE;
		pRoute.Add(tempChessMovePoint);
		cbLastRouteTileID = cbCurRouteTileID;
		cbCurRouteTileID = cbDestination= MAX_ROADWAY + tcciChess.cbColor * PLAYER_CHESS_COUNT + tcciChess.cbChessID +1;
		tempChessMovePoint.cbMoveMapNum = DirectionMapNum(m_ChessBoard.GetAt(cbLastRouteTileID), m_ChessBoard.GetAt(cbCurRouteTileID));
		IncrementGen(m_ChessBoard.GetAt(cbLastRouteTileID), m_ChessBoard.GetAt(cbCurRouteTileID), tempChessMovePoint.fXIncrement, tempChessMovePoint.fYIncrement); 
		if(m_CurMeChairID == m_CurPlayerID){
			for(BYTE i =0; i < m_CurActiveChess.GetCount(); i++){
				if(m_CurActiveChess.GetAt(i) == tcciChess.cbChessID){
					m_CurActiveChess.RemoveAt(i);
					//Debuglog.LogOutput("\nRemove at %d.", l);
				}
			}
		}
		cbIsFinished = true;
	}
	} else {
		cbCurRouteTileID = SPECIALTONEXT[tcciChess.cbColor][1] + UP_SKY_STEP-1;
		tempChessMovePoint.usXPos = m_ChessBoard.GetAt(cbCurRouteTileID).usXPos + PLANE_TILE_XOFFSET;
		tempChessMovePoint.usYPos = m_ChessBoard.GetAt(cbCurRouteTileID).usYPos + PLANE_TILE_YOFFSET;
		tempChessMovePoint.cbChessMoveFlag = CHESSMOVE_NOT_DEFINE;
		pRoute.Add(tempChessMovePoint);
		cbLastRouteTileID = cbCurRouteTileID;
		cbCurRouteTileID = cbUpSkyDownSpecial;
		tempChessMovePoint.cbMoveMapNum = DirectionMapNum(m_ChessBoard.GetAt(cbLastRouteTileID), m_ChessBoard.GetAt(cbCurRouteTileID));
		tempChessMovePoint.fXIncrement = tempChessMovePoint.fYIncrement = 1.0f;

	}

	

	if(shBeEatedTileIndex != -1 && shBeEatedTileIndex <aBeEatedTile.GetCount() && cbDestination == aBeEatedTile.GetAt(shBeEatedTileIndex).cbTileIndex) {
		tempChessMovePoint.cbChessMoveFlag = aBeEatedTile.GetAt(shBeEatedTileIndex).cbChessCount;
		tempChessMovePoint.cbRefChessMove = aBeEatedTile.GetAt(shBeEatedTileIndex).cbRefChessMove;
		shBeEatedTileIndex ++;
	} else {
		if(cbIsFinished){
			tempChessMovePoint.cbChessMoveFlag = CHESSMOVE_TO_FINISH;
		} else tempChessMovePoint.cbChessMoveFlag = CHESSMOVE_NOT_DEFINE;
	}
	

	tempChessMovePoint.usXPos = m_ChessBoard.GetAt(cbDestination).usXPos + PLANE_TILE_XOFFSET;
	tempChessMovePoint.usYPos = m_ChessBoard.GetAt(cbDestination).usYPos + PLANE_TILE_YOFFSET;

	pRoute.Add(tempChessMovePoint);
	m_ChessBoard.GetAt(tcciChess.cbTileIndex).cbChessCount--;
	tcciChess.cbTileIndex = cbDestination;
	m_ChessBoard.GetAt(cbDestination).cbChessCount ++;
//	TestChessMove();
	ASSERT(shBeEatedTileIndex == -1 || shBeEatedTileIndex == aBeEatedTile.GetCount());
	aBeEatedTile.RemoveAll();
	//Debuglog.LogOutput("\nm_CurPlayerID = %d, m_CurChessID = %d, CurcbTileItem = %d, CurChessStatus = %d", m_CurPlayerID, m_CurChessID, m_ClientChessItem[m_CurPlayerID][m_CurChessID].cbTileIndex, m_ClientChessItem[m_CurPlayerID][m_CurChessID].cbChessStatus);
	//Debuglog.LogOutput("\nsizeof(m_CurActiveChess) = %d", m_CurActiveChess.GetCount());
	//TestChessMove();
	//TestActiveChess();
	return TRUE;

}


/*
void CChessBoard::TestChessMove()
{
	TCHAR temp[40],tempAll[2048];
	ChessMove *pChessMove;
	for(BYTE i =0; i< m_ChessMove.GetCount(); i++){
		pChessMove = &m_ChessMove.GetAt(i);
		wsprintf(tempAll, "ChessMove: m_wPlayerID = %d, m_cbChessID = %d, m_cbRefChessMove = %d, m_cbBeRefedCount = %d, m_MoveStatus = %d", pChessMove->m_wPlayerID, pChessMove->m_cbChessID, pChessMove->m_cbRefChessMove, pChessMove->m_cbBeRefedCount, pChessMove->m_MoveStatus);
		for(BYTE j =0; j < pChessMove->m_ChessMoveRoute.GetCount(); j++){
			wsprintf(temp, "(%d, %d, %d, %d)", pChessMove->m_ChessMoveRoute.GetAt(j).usXPos, pChessMove->m_ChessMoveRoute.GetAt(j).usYPos,  pChessMove->m_ChessMoveRoute.GetAt(j).cbChessMoveFlag,  pChessMove->m_ChessMoveRoute.GetAt(j).cbRefChessMove);
			wsprintf(tempAll, "%s, %s", tempAll, temp);
		}
		Debuglog.LogOutput("\n%s",tempAll);

	}
}

void CChessBoard::TestActiveChess()
{
	TCHAR temp[4],tempAll[32];
	tempAll[0] = '\0';
	for(BYTE i =0; i< m_CurActiveChess.GetCount(); i++){
		wsprintf(temp, "%d", m_CurActiveChess.GetAt(i));
		if(tempAll[0])wsprintf(tempAll, "%s, %s", tempAll, temp);
		else wsprintf(tempAll, "CurActiveChess: %s", temp);
	}
	Debuglog.LogOutput("\n%s",tempAll);
}*/



BYTE CChessBoard::DirectionMapNum(const tagClientTileItem & origin, const tagClientTileItem & target){
	BYTE cbRet =0;
	if(target.usYPos > origin.usYPos) cbRet = 0x02;
	else if(target.usYPos < origin.usYPos) cbRet = 0x01;
	if(target.usXPos > origin.usXPos) cbRet += 0x20;
	else if(target.usXPos < origin.usXPos) cbRet += 0x10;
	switch(cbRet){
		case 0x20: cbRet = 0;
			break;
		case 0x22: cbRet = 1;
			break;
		case 0x02: cbRet = 2;
			break;
		case 0x12: cbRet = 3;
			break;
		case 0x10: cbRet = 4;
			break;
		case 0x11: cbRet = 5;
			break;
		case 0x01: cbRet = 6;
			break;
		case 0x21: cbRet = 7;
			break;
		case 0x00: ASSERT(false);
			break;
		default:
			MessageBox(NULL, TEXT("Impossibility!!"), TEXT(""), MB_OK);
	}
	return cbRet;

}

void CChessBoard::IncrementGen(const tagClientTileItem & origin, const tagClientTileItem & target, float &fXIncrement, float &fYIncrement){

	double dHypotenuse = sqrt(pow(target.usXPos - origin.usXPos, 2.0) + pow(target.usYPos - origin.usYPos, 2.0));
	fXIncrement = (float)(abs(target.usXPos - origin.usXPos) / dHypotenuse);
	fYIncrement = (float)(abs(target.usYPos - origin.usYPos) / dHypotenuse);
}

BYTE CChessBoard::BeKilledRouteGen(BYTE cbTileID, BYTE &cbChessCount, BYTE cbColor){
	BYTE tempChessMoveCount = (BYTE)m_ChessMove.GetCount();
	BYTE cbDestination, cbRetCount = 0, cbSameChessCount = cbChessCount;
	ChessMoveRoute tempChessMovePoint;

	for(BYTE i= 0; i<GAME_PLAYER; i ++){
		for(BYTE j= 0; j < PLAYER_CHESS_COUNT; j++){
			if(m_ClientChessItem[i][j].cbTileIndex == cbTileID){
				if(m_ClientChessItem[i][j].cbColor != cbColor){
					m_ChessMove.SetSize(tempChessMoveCount +1);
					m_ChessMove.GetAt(tempChessMoveCount).m_cbChessID = j;
					m_ChessMove.GetAt(tempChessMoveCount).m_wPlayerID = i;
					m_ChessMove.GetAt(tempChessMoveCount).m_MoveStatus = CHESSMOVE_NOT_ACTIVE;
					m_ChessMove.GetAt(tempChessMoveCount).m_cbRefChessMove =CHESSMOVE_NOT_DEFINE;
					
					BYTE l;
					for(/*BYTE*/ l =0; l< tempChessMoveCount; l++){
						if(m_ChessMove.GetAt(l).m_cbChessID == j && m_ChessMove.GetAt(l).m_wPlayerID == i && m_ChessMove.GetAt(l).m_cbRefChessMove == CHESSMOVE_NOT_DEFINE && m_ChessMove.GetAt(l).m_MoveStatus !=CHESSMOVE_HAS_FINISHED){
							break;
						}
					}

					if(l == tempChessMoveCount) m_ChessMove.GetAt(tempChessMoveCount).m_cbBeRefedCount = 1;
					else {
						m_ChessMove.GetAt(l).m_cbRefChessMove = tempChessMoveCount;
						m_ChessMove.GetAt(tempChessMoveCount).m_cbBeRefedCount = 2;
					}  


					cbDestination = MAX_ROADWAY + m_ClientChessItem[i][j].cbColor * PLAYER_CHESS_COUNT + m_ClientChessItem[i][j].cbChessID +1;
					tempChessMovePoint.cbMoveMapNum = DirectionMapNum(m_ChessBoard.GetAt(cbTileID), m_ChessBoard.GetAt(cbDestination));
					IncrementGen(m_ChessBoard.GetAt(cbTileID), m_ChessBoard.GetAt(cbDestination), tempChessMovePoint.fXIncrement, tempChessMovePoint.fYIncrement); 
					tempChessMovePoint.usXPos = m_ChessBoard.GetAt(cbDestination).usXPos + PLANE_TILE_XOFFSET;
					tempChessMovePoint.usYPos = m_ChessBoard.GetAt(cbDestination).usYPos + PLANE_TILE_YOFFSET;
					tempChessMovePoint.cbChessMoveFlag = CHESSMOVE_NOT_DEFINE;
					m_ChessMove.GetAt(tempChessMoveCount).m_ChessMoveRoute.Add(tempChessMovePoint);
					m_ClientChessItem[i][j].cbTileIndex = cbDestination;
					m_ChessBoard.GetAt(cbDestination).cbChessCount ++;
					cbRetCount ++;
					//Debuglog.LogOutput("\nm_CurMeChairID = %d, iPlayer= %d, jPlayer = %d", m_CurMeChairID, i, j);
					if(m_CurMeChairID == i) {
						for(BYTE l=0; l < m_CurActiveChess.GetCount(); l++){
							if(m_CurActiveChess.GetAt(l) == j){
								m_CurActiveChess.RemoveAt(l);
								//Debuglog.LogOutput("\nRemove at %d.", l);
							}
						}
					}
					cbChessCount --;
					if(!cbChessCount) break;
					tempChessMoveCount ++;
				} else {
					cbSameChessCount --;
					if(!cbSameChessCount)return 0;
				}
			}
		}
		if(!cbChessCount) break;
	}
	ASSERT(cbChessCount == 0);
	return cbRetCount;

}

void CChessBoard::CleanUpTexture(){
	m_ChessBoardTexture.Free();								
	m_PlaneTexture.Free();									
	m_FinishPlaneTexture.Free();								
}

void CChessBoard::TestChess(BYTE cbTestFlag){
	BYTE cbTemp1, cbTemp2;
	switch(cbTestFlag)
	{
	case 1:
		{
			for(BYTE i = MAX_NORMAL_ROADWAY +1; i<= MAX_ROADWAY; i++){
				cbTemp1 = (i - MAX_NORMAL_ROADWAY - 1) / UP_SKY_STEP;
				cbTemp2 = (i - MAX_NORMAL_ROADWAY - 1) % UP_SKY_STEP;
				if(cbTemp2 < PLAYER_CHESS_COUNT) {
					m_ClientChessItem[cbTemp1][cbTemp2].cbColor = cbTemp1;
					m_ClientChessItem[cbTemp1][cbTemp2].cbTileIndex = i;
					m_ChessBoard.GetAt(i).cbChessCount = 1;
					m_ClientChessItem[cbTemp1][cbTemp2].cbChessID = cbTemp2;
					m_ClientChessItem[cbTemp1][cbTemp2].cbChessStatus = CHESS_NOT_FINISHED;

					m_ClientChessItem[cbTemp1][cbTemp2].fXPos = (float)m_ChessBoard.GetAt(i).usXPos + PLANE_TILE_XOFFSET;
					m_ClientChessItem[cbTemp1][cbTemp2].fYPos = (float)m_ChessBoard.GetAt(i).usYPos + PLANE_TILE_YOFFSET;
					m_ClientChessItem[cbTemp1][cbTemp2].usDirection = 2*abs(GAME_PLAYER +GAME_PLAYER+cbTemp1 - m_CurMeChairID -2) %(2*GAME_PLAYER) * PLANE_W;

				}
			}
			m_CurActiveChess.RemoveAll();
			for(BYTE i = 0; i <GAME_PLAYER; i++) m_CurActiveChess.Add(i);
			break;
		}
	case 2:
		{
			for(BYTE i = MAX_NORMAL_ROADWAY +1; i<= MAX_ROADWAY; i++){
				cbTemp1 = (i - MAX_NORMAL_ROADWAY - 1) / UP_SKY_STEP;
				cbTemp2 = (i - MAX_NORMAL_ROADWAY - 1) % UP_SKY_STEP;
				if(cbTemp2 == PLAYER_CHESS_COUNT -1) {
					m_ClientChessItem[cbTemp1][cbTemp2].cbColor = cbTemp1;
					m_ClientChessItem[cbTemp1][cbTemp2].cbTileIndex = i;
					m_ChessBoard.GetAt(i).cbChessCount = 1;
					m_ClientChessItem[cbTemp1][cbTemp2].cbChessID = cbTemp2;
					m_ClientChessItem[cbTemp1][cbTemp2].cbChessStatus = CHESS_NOT_FINISHED;

					m_ClientChessItem[cbTemp1][cbTemp2].fXPos = (float)m_ChessBoard.GetAt(i).usXPos + PLANE_TILE_XOFFSET;
					m_ClientChessItem[cbTemp1][cbTemp2].fYPos = (float)m_ChessBoard.GetAt(i).usYPos + PLANE_TILE_YOFFSET;
					m_ClientChessItem[cbTemp1][cbTemp2].usDirection = 2*abs(GAME_PLAYER +GAME_PLAYER+cbTemp1 - m_CurMeChairID -2) %(2*GAME_PLAYER) * PLANE_W;

				}
			}
			for(int i =MAX_ROADWAY +1; i< MAX_ROADWAY_BASE; i++){
				cbTemp1 = (i - MAX_ROADWAY - 1) / GAME_PLAYER;
				cbTemp2 = (i - MAX_ROADWAY - 1) % PLAYER_CHESS_COUNT;
				if(cbTemp2 < PLAYER_CHESS_COUNT -1){
					m_ClientChessItem[cbTemp1][cbTemp2].cbChessID = cbTemp2;
					m_ClientChessItem[cbTemp1][cbTemp2].cbColor = cbTemp1;
					m_ClientChessItem[cbTemp1][cbTemp2].cbTileIndex = i;
					m_ChessBoard.GetAt(i).cbChessCount = 1;
					m_ClientChessItem[cbTemp1][cbTemp2].fXPos = (float)m_ChessBoard.GetAt(i).usXPos+ PLANE_TILE_XOFFSET;
					m_ClientChessItem[cbTemp1][cbTemp2].fYPos = (float)m_ChessBoard.GetAt(i).usYPos+ PLANE_TILE_YOFFSET;
					m_ClientChessItem[cbTemp1][cbTemp2].cbChessStatus = CHESS_FINISHED;
					m_ClientChessItem[cbTemp1][cbTemp2].usDirection = 2*abs(GAME_PLAYER+cbTemp1 - m_CurMeChairID -1) %(2*GAME_PLAYER) * PLANE_W;
				}
			}

			m_CurActiveChess.RemoveAll();
			m_CurActiveChess.Add(PLAYER_CHESS_COUNT -1);
			break;
		}
	case 3:
		{
			for(BYTE i = 0; i< GAME_PLAYER; i ++){
				switch(i)
				{
				case 0:
					{
						BYTE j;
						for(/*BYTE*/ j= 0; j< PLAYER_CHESS_COUNT -1; j++){
							m_ClientChessItem[i][j].cbColor = i;
							m_ClientChessItem[i][j].cbTileIndex = MAX_ROADWAY + i*PLAYER_CHESS_COUNT +j +1;
							m_ChessBoard.GetAt(m_ClientChessItem[i][j].cbTileIndex).cbChessCount = 1;
							m_ClientChessItem[i][j].cbChessID = j;
							m_ClientChessItem[i][j].cbChessStatus = CHESS_NOT_FINISHED;

							m_ClientChessItem[i][j].fXPos = (float)m_ChessBoard.GetAt(m_ClientChessItem[i][j].cbTileIndex).usXPos + PLANE_TILE_XOFFSET;
							m_ClientChessItem[i][j].fYPos = (float)m_ChessBoard.GetAt(m_ClientChessItem[i][j].cbTileIndex).usYPos + PLANE_TILE_YOFFSET;
							m_ClientChessItem[i][j].usDirection = 2*(GAME_PLAYER +GAME_PLAYER+i - m_CurMeChairID -1) %(2*GAME_PLAYER) * PLANE_W;
						}
						j = PLAYER_CHESS_COUNT -1;
						m_ClientChessItem[i][j].cbColor = i;
						m_ClientChessItem[i][j].cbTileIndex = 16;
						m_ChessBoard.GetAt(m_ClientChessItem[i][j].cbTileIndex).cbChessCount = 1;
						m_ClientChessItem[i][j].cbChessID = j;
						m_ClientChessItem[i][j].cbChessStatus = CHESS_NOT_FINISHED;

						m_ClientChessItem[i][j].fXPos = (float)m_ChessBoard.GetAt(m_ClientChessItem[i][j].cbTileIndex).usXPos + PLANE_TILE_XOFFSET;
						m_ClientChessItem[i][j].fYPos = (float)m_ChessBoard.GetAt(m_ClientChessItem[i][j].cbTileIndex).usYPos + PLANE_TILE_YOFFSET;
						m_ClientChessItem[i][j].usDirection = 2*(GAME_PLAYER +GAME_PLAYER+i - m_CurMeChairID -2) %(2*GAME_PLAYER) * PLANE_W;
						if(m_CurMeChairID == i){
							m_CurActiveChess.RemoveAll();
							m_CurActiveChess.Add(PLAYER_CHESS_COUNT -1);
						}
						break;
					}
				case 2:
					{
						BYTE aTemp[PLAYER_CHESS_COUNT] = {29, 66, 17, 33};
						for(BYTE j= 0; j< PLAYER_CHESS_COUNT; j++){
							m_ClientChessItem[i][j].cbColor = i;
							m_ClientChessItem[i][j].cbTileIndex = aTemp[j];
							m_ChessBoard.GetAt(m_ClientChessItem[i][j].cbTileIndex).cbChessCount = 1;
							m_ClientChessItem[i][j].cbChessID = j;
							m_ClientChessItem[i][j].cbChessStatus = CHESS_NOT_FINISHED;

							m_ClientChessItem[i][j].fXPos = (float)m_ChessBoard.GetAt(m_ClientChessItem[i][j].cbTileIndex).usXPos + PLANE_TILE_XOFFSET;
							m_ClientChessItem[i][j].fYPos = (float)m_ChessBoard.GetAt(m_ClientChessItem[i][j].cbTileIndex).usYPos + PLANE_TILE_YOFFSET;
							m_ClientChessItem[i][j].usDirection = 2*(GAME_PLAYER +GAME_PLAYER+i - m_CurMeChairID -2) %(2*GAME_PLAYER) * PLANE_W;

						}
						if(m_CurMeChairID == i){
							m_CurActiveChess.RemoveAll();
							for(BYTE j=0; j< GAME_PLAYER; j++) m_CurActiveChess.Add(j);
						}
					break;
					   }
				case 1:{}
				case 3:
					{
						for(BYTE j= 0; j< PLAYER_CHESS_COUNT; j++){
							m_ClientChessItem[i][j].cbColor = i;
							m_ClientChessItem[i][j].cbTileIndex = MAX_ROADWAY + i*PLAYER_CHESS_COUNT +j +1;
							m_ChessBoard.GetAt(m_ClientChessItem[i][j].cbTileIndex).cbChessCount = 1;
							m_ClientChessItem[i][j].cbChessID = j;
							m_ClientChessItem[i][j].cbChessStatus = CHESS_NOT_FINISHED;

							m_ClientChessItem[i][j].fXPos = (float)m_ChessBoard.GetAt(m_ClientChessItem[i][j].cbTileIndex).usXPos + PLANE_TILE_XOFFSET;
							m_ClientChessItem[i][j].fYPos = (float)m_ChessBoard.GetAt(m_ClientChessItem[i][j].cbTileIndex).usYPos + PLANE_TILE_YOFFSET;
							m_ClientChessItem[i][j].usDirection = 2*(GAME_PLAYER +GAME_PLAYER+i - m_CurMeChairID -1) %(2*GAME_PLAYER) * PLANE_W;
						}
					 
					break;
					  }
				}
			}

		}
	}
}

void CChessBoard::InitChess(){
	BYTE cbTemp1, cbTemp2;
	for(int i =MAX_ROADWAY +1; i<= MAX_ROADWAY_BASE; i++){
		cbTemp1 = (i - MAX_ROADWAY - 1) / GAME_PLAYER;
		cbTemp2 = (i - MAX_ROADWAY - 1) % PLAYER_CHESS_COUNT;
		m_ClientChessItem[cbTemp1][cbTemp2].cbChessID = cbTemp2;
		m_ClientChessItem[cbTemp1][cbTemp2].cbColor = cbTemp1;
		m_ClientChessItem[cbTemp1][cbTemp2].cbTileIndex = i;
		m_ChessBoard.GetAt(i).cbChessCount = 1;
		m_ClientChessItem[cbTemp1][cbTemp2].fXPos = (float)m_ChessBoard.GetAt(i).usXPos+ PLANE_TILE_XOFFSET;
		m_ClientChessItem[cbTemp1][cbTemp2].fYPos = (float)m_ChessBoard.GetAt(i).usYPos+ PLANE_TILE_YOFFSET;
		m_ClientChessItem[cbTemp1][cbTemp2].cbChessStatus = CHESS_NOT_FINISHED;
		m_ClientChessItem[cbTemp1][cbTemp2].usDirection = 2*(GAME_PLAYER+cbTemp1 - m_CurMeChairID -1) %(2*GAME_PLAYER) * PLANE_W;
	}
}

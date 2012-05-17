#ifndef __FISH_FISH_LAYER_H__
#define __FISH_FISH_LAYER_H__

//////////////////////////////////////////////////////////////////
#include "xlbe.h"

#include "..\消息定义\CMD_Fish.h"
#include "Path.h"

#include "GameCore.h"
#include "ObjectExtend.h"

#include "FrameAction.h"


/////////////////////////////////////////////////////////////////
class CFishLayer : public Layer
{
public:
    CFishLayer();
    virtual ~CFishLayer ();

    virtual void update(float dt);
    virtual void draw();

public:
    bool NetAddLinePathFish(CMD_S_Send_Line_Path_Fish *pAddLineFish);
    bool NetAddGroupPointPathFish(CMD_S_Send_Group_Point_Path_Fish *pAddGroupPointFish);
    bool NetAddPointPathFish(CMD_S_Send_Point_Path_Fish *pAddPointFish);
    bool NetAddSpecialPointPathFish(CMD_S_Send_Special_Point_Path *pSendSpecialPointPathFish);

    void SpeedUpFishObject(float fSpeed);

    bool FishEnd(Node *node, int tag);
    bool FishDestory(Node *node, int tag);
    int FishCaptured(WORD wId,  WORD wChairID);
    bool CoinAlpha(Node *node, int tag);


private:
    std::vector<Node *> m_NodeDelete;
    std::vector<Key_Frames> m_SmallFishPaths;
    std::vector<Key_Frames> m_BigFishPaths;
    std::vector<Key_Frames> m_HugeFishPaths;
    std::vector<Key_Frames> m_SpecialFishPaths;

};

//////////////////////////////////////////////////////////////////////////
#endif
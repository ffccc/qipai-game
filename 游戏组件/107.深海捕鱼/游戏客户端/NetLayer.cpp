#include "NetLayer.h"
#include "GameScene.h"

#include <sstream>

////////////////////////////////////////////////////////////
CNetLayer::CNetLayer()
{

}

CNetLayer::~CNetLayer()
{
}

void CNetLayer::update(float dt)
{
    __super::update(dt);
}

void CNetLayer::draw()
{
    __super::draw(); 
}

bool CNetLayer::NetFire(const Point &pt, WORD wChairID, CGameCore::enCannonType CannonType)
{
    CNetObjectExtend *pNetObjectExtend = new CNetObjectExtend;

    pNetObjectExtend->wChairID = wChairID;
    pNetObjectExtend->CannonType = CannonType;

    int nFirst=0;
    if (CannonType > CGameCore::CannonType_4)
    {
        nFirst = 1;
    }

    Size szScale;
    switch(CannonType)
    {
    case CGameCore::CannonType_0: { szScale.width_=0.5; szScale.height_=0.5; break; }
    case CGameCore::CannonType_1: { szScale.width_=0.6; szScale.height_=0.6; break; }
    case CGameCore::CannonType_2: { szScale.width_=0.7; szScale.height_=0.7; break; }
    case CGameCore::CannonType_3: { szScale.width_=0.8; szScale.height_=0.8; break; }
    case CGameCore::CannonType_4: { szScale.width_=0.9; szScale.height_=0.9; break; }
    case CGameCore::CannonType_5: { szScale.width_=0.9; szScale.height_=0.9; break; }
    case CGameCore::CannonType_6: { szScale.width_=1.0; szScale.height_=1.0; break; }
    }


    std::ostringstream ostr;
    ostr << "net_" <<nFirst << "_"<< (int)wChairID ;

    Sprite *sprNet = new Sprite(Root::instance()->imageset_manager()->imageset(ostr.str())->image("0"));
    sprNet->set_node_extend(pNetObjectExtend);

    sprNet->set_position(pt);
    sprNet->set_scale(szScale);
    add_child(sprNet);

    Animation *aniBullet = Root::instance()->animation_manager()->animation(ostr.str());
    sprNet->run_action(new Action_Sequence(new Action_Animation(0.06,aniBullet,false), new Action_Func(rak::mem_fn(this, &CNetLayer::NetEnd)), 0));

    try
    {
        Sound_Instance *pSound = Root::instance()->sound_manager()->sound_instance(nFirst+7);
        pSound->play(false, true);
    }
    catch(...)
    {
    }

    return true;
}

bool CNetLayer::NetEnd(Node *node, int tag)
{
    Point ptNet;
    Point ptTNet;
    Point ptFish;
    Size szFish;
    Point ptDifference;
    Rect rcScreen(0,0,1280,738);
    float sint, cost;


    CGameScene *pGameScene = (CGameScene *)parent();
    CNetObjectExtend *pNetObjectExtend = (CNetObjectExtend *)node->node_extend();

    if (pNetObjectExtend->wChairID == pGameScene->GetMeChairID() || pGameScene->IsAndroidLogicChairID())
    {
        CMD_C_Cast_Net CastNet;
		CastNet.wChairID = pNetObjectExtend->wChairID;
        CastNet.cbCount = 0;

        ptNet = node->position();

        Nodes::iterator j=pGameScene->m_layFishObject->child_begin();
        for (; j!=pGameScene->m_layFishObject->child_end(); ++j)
        {
			if (CastNet.cbCount >= MAX_FISH_IN_NET)
			    break;

            CFishObjectExtend *pFishObjectExtend = (CFishObjectExtend *)(*j)->node_extend();

            if (pFishObjectExtend->wID == INVALID_WORD)
                continue;

            ptFish = (*j)->position();
            szFish = pFishObjectExtend->GetFishObjectSize();

            cost = std::cosf((*j)->rotation());
            sint = std::sinf((*j)->rotation());
            ptTNet.x_ = (ptNet.x_-ptFish.x_)*cost + (ptNet.y_-ptFish.y_)*sint;
            ptTNet.y_ = -(ptNet.x_-ptFish.x_)*sint + (ptNet.y_-ptFish.y_)*cost;

            if (CFishObjectExtend::ComputeCollision(szFish.width_, szFish.height_, pNetObjectExtend->GetNetRadius(), ptTNet.x_, ptTNet.y_ ))
            {
                CastNet.FishNetObjects[CastNet.cbCount].wID = pFishObjectExtend->wID;
                CastNet.FishNetObjects[CastNet.cbCount].wRoundID = pFishObjectExtend->wRoundID;
                CastNet.FishNetObjects[CastNet.cbCount].wType = pFishObjectExtend->FishType;
                CastNet.FishNetObjects[CastNet.cbCount].dwTime = pFishObjectExtend->dwTime;

                CastNet.cbCount++;
                
            }
        }

		CClientKernel *pClientKernel = pGameScene->GetClientKernel();

		if (pClientKernel != NULL)
			pClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_CAST_NET, &CastNet, sizeof(CMD_C_Cast_Net)-(MAX_FISH_IN_NET-CastNet.cbCount)*sizeof(Fish_Net_Object));
    }

    remove_child(node);

    return true;
}



////////////////////////////////////////////////////////////
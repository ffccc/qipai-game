#include "BulletLayer.h"
#include "GameScene.h"

#include <sstream>

////////////////////////////////////////////////////////////
CBulletLayer::CBulletLayer()
{

}

CBulletLayer::~CBulletLayer()
{
}

void CBulletLayer::update(float dt)
{
    __super::update(dt);

    CGameScene *pGameScene = (CGameScene *)parent();

    Point ptBullet;
    Point ptTBullet;
    Point ptFish;
    Size szFish;
    float sint, cost;
    Rect rcScreen(0,0,1280,738);
    Nodes::iterator i=child_begin();
    for (; i!=child_end(); ++i)
    {
        CBulletObjectExtend *pBulletObjectExtend = (CBulletObjectExtend *)(*i)->node_extend();

        ptBullet = (*i)->position();
        if (!rcScreen.pt_in_rect(ptBullet))
        {
            m_NodeDelete.push_back((*i));
            continue;
        }

        Nodes::iterator j=pGameScene->m_layFishObject->child_begin();
        for (; j!=pGameScene->m_layFishObject->child_end(); ++j)
        {
            CFishObjectExtend *pFishObjectExtend = (CFishObjectExtend *)(*j)->node_extend();
            if (pFishObjectExtend->wID == INVALID_WORD)
                continue;

            ptFish = (*j)->position();
            szFish = pFishObjectExtend->GetFishObjectSize();

            cost = std::cosf((*j)->rotation());
            sint = std::sinf((*j)->rotation());
            ptTBullet.x_ = (ptBullet.x_-ptFish.x_)*cost + (ptBullet.y_-ptFish.y_)*sint;
            ptTBullet.y_ = -(ptBullet.x_-ptFish.x_)*sint + (ptBullet.y_-ptFish.y_)*cost;

            if (CFishObjectExtend::ComputeCollision(szFish.width_, szFish.height_, 10, ptTBullet.x_, ptTBullet.y_ ))
            {
                pGameScene->m_layNetObject->NetFire(ptBullet, pBulletObjectExtend->wChairID, pBulletObjectExtend->CannonType);
                m_NodeDelete.push_back((*i));
                break;
            }
        }

    }

    std::vector<Node*>::iterator k=m_NodeDelete.begin();
    for (; k!=m_NodeDelete.end(); ++k)
    {
        remove_child((*k));
        delete (*k)->node_extend();
        delete (*k);
    }

    m_NodeDelete.clear();
}

void CBulletLayer::draw()
{
    __super::draw(); 
}

bool CBulletLayer::BulletFire(const Point &ptStart, float rotation, WORD wChairID, CGameCore::enCannonType CannonType)
{
    CBulletObjectExtend *BulletObjectExtend = new CBulletObjectExtend;

    BulletObjectExtend->wChairID = wChairID;
    BulletObjectExtend->CannonType = CannonType;

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
    ostr << "shot_" <<nFirst << "_"<< (int)wChairID ;

    Sprite *sprBullet = new Sprite(Root::instance()->imageset_manager()->imageset(ostr.str())->image("0"));
    sprBullet->set_node_extend(BulletObjectExtend);

    Point pt (ptStart.x_+56*std::cos(rotation-M_PI_2), ptStart.y_+56*std::sin(rotation-M_PI_2));
    sprBullet->set_position(pt);
    sprBullet->set_rotation(rotation);
    sprBullet->set_scale(szScale);
    add_child(sprBullet);

    Action *actBullet = new Action_Move_By(6, Point(2000*std::cos(rotation-M_PI_2),2000*std::sin(rotation-M_PI_2)));                           
    sprBullet->run_action(actBullet);

    Animation *aniBullet = Root::instance()->animation_manager()->animation(ostr.str());
    sprBullet->run_action(new Action_Repeat_Forever (new Action_Animation(0.06,aniBullet,false)));

    CGameScene *pGameScene = (CGameScene *)parent();
    if (wChairID == pGameScene->GetMeChairID())
    {
        CMD_C_Fire Fire;
        Fire.fRote = rotation;
        pGameScene->GetClientKernel()->SendSocketData(MDM_GF_GAME, SUB_C_FIRE, &Fire, sizeof(CMD_C_Fire));
    }

    try
    {
        Sound_Instance *pSound = Root::instance()->sound_manager()->sound_instance(nFirst+5);
        pSound->play(false, true);
    }
    catch(...)
    {
    }

    return true;
}



////////////////////////////////////////////////////////////
#include "FishLayer.h"
#include "ClientKernel.h"
#include "GameScene.h"
#include "AccountLayer.h"
#include <sstream>

////////////////////////////////////////////////////////////
CFishLayer::CFishLayer()
{
    int ptX,ptY,nStaff;
    float fAngle;
    char szPath[100];
    Pack_File *pf;
    std::ostringstream ostr;

    for (int i=0; i<208; i++)
    {
        Key_Frames frames;

        ostr.str("");
        ostr <<  "small\\" << i << ".dat";
        pf = pfopen(ostr.str().c_str(), "rb");
        if (pf == 0)
        {
            MessageBox(0,"CFishLayer::CFishLayer()","",0);
        }

        pfgets(szPath, 100, pf);

        while(pfgets(szPath, 100, pf))
        {
            std::sscanf(szPath, "(%d,%d,%f,%d)", &ptX, &ptY, &fAngle, &nStaff);
            frames.push_back(Key_Frame(Point(ptX, ptY), fAngle));
        }

        m_SmallFishPaths.push_back(frames);

    }

    for (int i=0; i<130; i++)
    {
        Key_Frames frames;

        ostr.str("");
        ostr <<  "big\\" << i << ".dat";
        pf = pfopen(ostr.str().c_str(), "rb");
        if (pf == 0)
        {
            MessageBox(0,"CFishLayer::CFishLayer()","",0);
        }

        pfgets(szPath, 100, pf);

        while(pfgets(szPath, 100, pf))
        {
            std::sscanf(szPath, "(%d,%d,%f,%d)", &ptX, &ptY, &fAngle, &nStaff);
            frames.push_back(Key_Frame(Point(ptX, ptY), fAngle));
        }

        m_BigFishPaths.push_back(frames);

    }


    for (int i=0; i<62; i++)
    {
        Key_Frames frames;

        ostr.str("");
        ostr <<  "huge\\" << i << ".dat";
        pf = pfopen(ostr.str().c_str(), "rb");
        if (pf == 0)
        {
            MessageBox(0,"CFishLayer::CFishLayer()","",0);
        }

        pfgets(szPath, 100, pf);

        while(pfgets(szPath, 100, pf))
        {
            std::sscanf(szPath, "(%d,%d,%f,%d)", &ptX, &ptY, &fAngle, &nStaff);
            frames.push_back(Key_Frame(Point(ptX, ptY), fAngle));
        }

        m_HugeFishPaths.push_back(frames);

    }

    for (int i=0; i<28; i++)
    {
        Key_Frames frames;

        ostr.str("");
        ostr <<  "special\\" << i << ".dat";
        pf = pfopen(ostr.str().c_str(), "rb");
        if (pf == 0)
        {
            MessageBox(0,"CFishLayer::CFishLayer()","",0);
        }

        pfgets(szPath, 100, pf);

        while(pfgets(szPath, 100, pf))
        {
            std::sscanf(szPath, "(%d,%d,%f,%d)", &ptX, &ptY, &fAngle, &nStaff);
            frames.push_back(Key_Frame(Point(ptX, ptY), fAngle));
        }

        m_SpecialFishPaths.push_back(frames);

    }

}

CFishLayer::~CFishLayer()
{
}

void CFishLayer::update(float dt)
{
    __super::update(dt);

    DWORD dwTime = (DWORD)time(NULL);
    Nodes::iterator i=child_begin();
    for (; i!=child_end(); ++i)
    {
        CFishObjectExtend *pFishObjectExtend = (CFishObjectExtend *)(*i)->node_extend();
        
        if ((dwTime-pFishObjectExtend->dwTime) >= FISH_DESTROY_TIME)
        {
            m_NodeDelete.push_back((*i));
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

void CFishLayer::draw()
{
    __super::draw();

   
}

bool CFishLayer::NetAddLinePathFish(CMD_S_Send_Line_Path_Fish *pAddLinePathFish)
{
    int nNetFishCount = pAddLinePathFish->cbCount;
    for (int i=0; i<nNetFishCount; i++)
    {
            CFishObjectExtend *pNodeExtend = new CFishObjectExtend;
            pNodeExtend->wID = pAddLinePathFish->FishPaths[i].FishNetObject.wID;
            pNodeExtend->wRoundID = pAddLinePathFish->FishPaths[i].FishNetObject.wRoundID;
            pNodeExtend->FishType = CGameCore::WordToFishType(pAddLinePathFish->FishPaths[i].FishNetObject.wType);
            pNodeExtend->dwTime = (DWORD)time(NULL);//pAddLinePathFish->FishPaths[i].FishNetObject.dwTime;

            std::ostringstream ostr;
            ostr << "fish_" << (int)pNodeExtend->FishType;

            Sprite *sprFish = new Sprite(Root::instance()->imageset_manager()->imageset(ostr.str())->image("0"));
            sprFish->set_node_extend(pNodeExtend);
            add_child(sprFish);

            ostr.str("");
            ostr << "fish_" << (int)pNodeExtend->FishType << "_move";
            Animation *aniFish = Root::instance()->animation_manager()->animation(ostr.str());
            Action *actFish = new Action_Repeat_Forever(new Action_Animation(0.06, aniFish, false));
            sprFish->run_action(actFish);

            sprFish->set_position(Point(pAddLinePathFish->FishPaths[i].LinePath.spStart.x, pAddLinePathFish->FishPaths[i].LinePath.spStart.y));
            Action *actMove= new Action_Move_To(pAddLinePathFish->FishPaths[i].LinePath.dwTime, Point(pAddLinePathFish->FishPaths[i].LinePath.spEnd.x, pAddLinePathFish->FishPaths[i].LinePath.spEnd.y));
            actMove->set_tag(0);
            sprFish->run_action(actMove);

            float rote;
            float length;
            Point tv( Point(pAddLinePathFish->FishPaths[i].LinePath.spEnd.x, pAddLinePathFish->FishPaths[i].LinePath.spEnd.y)-sprFish->position() );
            length = std::sqrt(tv.x_*tv.x_+tv.y_*tv.y_);

            if (length > 0)
            {
                if (tv.y_>=0)
                {
                    rote= std::acos(tv.x_/length);
                }
                else
                {
                    rote= -std::acos(tv.x_/length);
                }
                sprFish->set_rotation(rote-M_PI_2);
            }
    }



    return true;
}

bool CFishLayer::NetAddPointPathFish(CMD_S_Send_Point_Path_Fish *pAddPointFish)
{
    int nNetFishCount = pAddPointFish->cbCount;
    for (int i=0; i<nNetFishCount; i++)
    {
            CFishObjectExtend *pNodeExtend = new CFishObjectExtend;
            pNodeExtend->wID = pAddPointFish->FishPaths[i].FishNetObject.wID;
            pNodeExtend->wRoundID = pAddPointFish->FishPaths[i].FishNetObject.wRoundID;
            pNodeExtend->FishType = CGameCore::WordToFishType(pAddPointFish->FishPaths[i].FishNetObject.wType);
            pNodeExtend->dwTime = (DWORD)time(NULL);//pAddPointFish->FishPaths[i].FishNetObject.dwTime;

            std::ostringstream ostr;
            ostr << "fish_" << (int)pNodeExtend->FishType;

            Sprite *sprFish = new Sprite(Root::instance()->imageset_manager()->imageset(ostr.str())->image("0"));
            sprFish->set_node_extend(pNodeExtend);
            add_child(sprFish);

            ostr.str("");
            ostr << "fish_" << (int)pNodeExtend->FishType << "_move";
            Animation *aniFish = Root::instance()->animation_manager()->animation(ostr.str());
            Action *actFish = new Action_Repeat_Forever(new Action_Animation(0.06, aniFish, false));
            sprFish->run_action(actFish);

            Key_Frames *pFrames;
            if (pNodeExtend->FishType >= 7)
            {
                pFrames = &m_HugeFishPaths[pAddPointFish->FishPaths[i].wPath];
            }
            else if (pNodeExtend->FishType > 3 && pNodeExtend->FishType < 7)
            {
                pFrames = &m_BigFishPaths[pAddPointFish->FishPaths[i].wPath];
            }
            else
            {
                pFrames = &m_SmallFishPaths[pAddPointFish->FishPaths[i].wPath];
            }

          
            Action *act = new Action_Sequence(new Action_Key_Frame(0.05, *pFrames, Point(0,0)), new Action_Func(rak::mem_fn(this, &CFishLayer::FishDestory)), 0);
            act->set_tag(0);
            sprFish->run_action(act);
    }

    return true;
}

bool CFishLayer::NetAddSpecialPointPathFish(CMD_S_Send_Special_Point_Path *pSendSpecialPointPathFish)
{
    int nNetFishCount = pSendSpecialPointPathFish->cbCount;
    for (int i=0; i<nNetFishCount; i++)
    {
            CFishObjectExtend *pNodeExtend = new CFishObjectExtend;
            pNodeExtend->wID = pSendSpecialPointPathFish->SpecialPointPath[i].FishNetObject.wID;
            pNodeExtend->wRoundID = pSendSpecialPointPathFish->SpecialPointPath[i].FishNetObject.wRoundID;
            pNodeExtend->FishType = CGameCore::WordToFishType(pSendSpecialPointPathFish->SpecialPointPath[i].FishNetObject.wType);
            pNodeExtend->dwTime = (DWORD)time(NULL);//pAddPointFish->FishPaths[i].FishNetObject.dwTime;

            std::ostringstream ostr;
            ostr << "fish_" << (int)pNodeExtend->FishType;

            Sprite *sprFish = new Sprite(Root::instance()->imageset_manager()->imageset(ostr.str())->image("0"));
            sprFish->set_node_extend(pNodeExtend);
            sprFish->set_visible(false);
            add_child(sprFish);

            ostr.str("");
            ostr << "fish_" << (int)pNodeExtend->FishType << "_move";
            Animation *aniFish = Root::instance()->animation_manager()->animation(ostr.str());
            Action *actFish = new Action_Repeat_Forever(new Action_Animation(0.06, aniFish, false));
            sprFish->run_action(actFish);

            Key_Frames *pFrames;
            pFrames = &m_SpecialFishPaths[pSendSpecialPointPathFish->SpecialPointPath[i].wPath];
          
            Action *act = new Action_Sequence(new Action_Delay(pSendSpecialPointPathFish->SpecialPointPath[i].fDelay), new Action_Show(), new Action_Key_Frame(0.05, *pFrames, Point(0,0)), new Action_Func(rak::mem_fn(this, &CFishLayer::FishDestory)), 0);
            act->set_tag(0);
            sprFish->run_action(act);
    }

    return true;
}

bool CFishLayer::NetAddGroupPointPathFish(CMD_S_Send_Group_Point_Path_Fish *pAddGroupPointFish)
{
     Point ptOffset;
     float fDelay = 0.05;
    int nNetFishCount = pAddGroupPointFish->cbCount;
    for (int i=0; i<nNetFishCount; i++)
    {
            CFishObjectExtend *pNodeExtend = new CFishObjectExtend;
            pNodeExtend->wID = pAddGroupPointFish->FishNetObject[i].wID;
            pNodeExtend->wRoundID = pAddGroupPointFish->FishNetObject[i].wRoundID;
            pNodeExtend->FishType = CGameCore::WordToFishType(pAddGroupPointFish->FishNetObject[i].wType);
            pNodeExtend->dwTime = (DWORD)time(NULL);//pAddGroupPointFish->FishNetObject[i].dwTime;

            std::ostringstream ostr;
            ostr << "fish_" << (int)pNodeExtend->FishType;

            Sprite *sprFish = new Sprite(Root::instance()->imageset_manager()->imageset(ostr.str())->image("0"));
            sprFish->set_node_extend(pNodeExtend);
            add_child(sprFish);

            ostr.str("");
            ostr << "fish_" << (int)pNodeExtend->FishType << "_move";
            Animation *aniFish = Root::instance()->animation_manager()->animation(ostr.str());
            Action *actFish = new Action_Repeat_Forever(new Action_Animation(0.06, aniFish, false));
            sprFish->run_action(actFish);

            Key_Frames *pFrames;
    
            pFrames = &m_SmallFishPaths[pAddGroupPointFish->wPath];
           
            if (pAddGroupPointFish->cbPahtType == 0)
            {
                Action *act = new Action_Sequence(new Action_Delay(i*0.3+0.05), new Action_Key_Frame(0.05, *pFrames, Point(0,0)), new Action_Func(rak::mem_fn(this, &CFishLayer::FishDestory)), 0);
                act->set_tag(0);
                sprFish->run_action(act);
            }
            else  if(pAddGroupPointFish->cbPahtType == 1)
            {
                if (i%4==0)
                {
                }
                else if (i%4==1)
                {
                    ptOffset.x_ = 0;
                    ptOffset.y_ = 0;

                    fDelay += 0.3;
                }
                else if (i%4==2)
                {
                    ptOffset.x_ = -14;
                    ptOffset.y_ = -18;

                    fDelay += 0.2;
                }
                else if (i%4==3)
                {
                    ptOffset.x_ = 14;
                    ptOffset.y_ = 18;
                }

                Action *act = new Action_Sequence(new Action_Delay(fDelay), new Action_Key_Frame(0.05, *pFrames, ptOffset), new Action_Func(rak::mem_fn(this, &CFishLayer::FishDestory)), 0);
                act->set_tag(0);
                sprFish->run_action(act);
            }
            else 
            {
                if (i==0)
                {
                }
                else if (i%2==0)
                {
                    ptOffset.x_ = 14;
                    ptOffset.y_ = 14;

                    fDelay += 0.3;
                }
                else 
                {
                    ptOffset.x_ = -14;
                    ptOffset.y_ = -14;
                }
               

                Action *act = new Action_Sequence(new Action_Delay(fDelay), new Action_Key_Frame(0.05, *pFrames, ptOffset), new Action_Func(rak::mem_fn(this, &CFishLayer::FishDestory)), 0);
                act->set_tag(0);
                sprFish->run_action(act);
            }
    }

    return true;
}

bool CFishLayer::FishDestory(Node *node, int tag)
{
    remove_child(node);
    delete node->node_extend();
    delete node;
    return true;
}

void CFishLayer::SpeedUpFishObject(float fSpeed)
{
    Nodes::iterator i=child_begin();
    for (; i!=child_end(); ++i)
    {
        Action *actMove = (*i)->action_by_tag(0);

        if (actMove)
        {
            actMove->set_speed(fSpeed);
        }
        

    }
}

bool CFishLayer::FishEnd(Node *node, int tag)
{
    try
    {
        Sound_Instance *pSound = Root::instance()->sound_manager()->sound_instance(9);
        pSound->play(false, true);
    }
    catch(...)
    {
    }

    CGameScene *pGameScene = (CGameScene *)parent();
    CFishObjectExtend *pFishObjectExtend = (CFishObjectExtend *)node->node_extend();

    int nCoinCount = pFishObjectExtend->GetFishGoldByStyle();
    int nCointTen = nCoinCount/10;
    int nCointOne = nCoinCount%10;

    Point ptCoin;
    Point ptNode;
    Point ptRole = pGameScene->m_layRoles[tag]->GetCannonPosition();

    if (nCointTen == 1)
    {
        ptNode = node->position();
    }
    else if (nCointTen<=5)
    {
        ptNode = node->position()+Point(-((float)(nCointTen))/2*48, -(nCointTen/5)*48);
    }
    else
    {
        ptNode = node->position()+Point(-5.0f/2*48, -(nCointTen/5)*48);
    }

    for (int i=0; i<nCointTen; i++)
    {
        if (i>4)
        {
            ptCoin = ptNode + Point((i%5)*48, +48);
        }
        else
        {
            ptCoin = ptNode + Point(i*48, 0);
        }
        

        Sprite *sprTenCoin = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image("coin_gold_0"));
        Sprite *sprTenCoin1 = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image("coin_gold_0"));
        Sprite *sprTenCoin2 = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image("coin_gold_0"));

        sprTenCoin->set_position(ptCoin);
        sprTenCoin1->set_position(ptCoin+Point(0, 4));
        sprTenCoin2->set_position(ptCoin+Point(0, 8));

        sprTenCoin1->set_visible(false);
        sprTenCoin2->set_visible(false);

        Color_Rect ColorRect = sprTenCoin1->color();
        ColorRect.set_alpha(150);
        sprTenCoin1->set_color(ColorRect);
        ColorRect.set_alpha(80);
        sprTenCoin2->set_color(ColorRect);

        pGameScene->m_layCoinObject->add_child(sprTenCoin2);
        pGameScene->m_layCoinObject->add_child(sprTenCoin1);
        pGameScene->m_layCoinObject->add_child(sprTenCoin);

        Animation *aniCoin = Root::instance()->animation_manager()->animation("coin_gold");
        sprTenCoin->run_action(new Action_Repeat_Forever(new Action_Animation(0.06, aniCoin, false)));

        Action *actSequence;
        Action *actMove[5];

        actMove[0] = new Action_Move_To(0.3, ptCoin+Point(0, -150));
        actMove[1] = new Action_Move_To(0.3, ptCoin+Point(0, 0));
        actMove[2] = new Action_Move_To(0.1, ptCoin+Point(0, -15));
        actMove[3] = new Action_Move_To(0.1, ptCoin+Point(0, 0));
        actMove[4] = new Action_Move_To(0.4, ptRole);

        Action *actFunc = new Action_Func(rak::mem_fn(pGameScene->m_layCoinObject, &CCoinLayer::CoinGoldEnd));
        actFunc->set_tag(tag);

        actSequence = new Action_Sequence(actMove[0], actMove[1], actMove[2], actMove[3], actMove[4], actFunc, 0);
        sprTenCoin->run_action(actSequence);

        actMove[0] = new Action_Move_To(0.3, ptCoin+Point(0, -150));
        actMove[1] = new Action_Move_To(0.3, ptCoin+Point(0, 0));
        actMove[2] = new Action_Move_To(0.1, ptCoin+Point(0, -15));
        actMove[3] = new Action_Move_To(0.1, ptCoin+Point(0, 0));
        actMove[4] = new Action_Move_To(0.4, ptRole);
        actSequence = new Action_Sequence(new Action_Delay(0.04), actMove[0], actMove[1], actMove[2], actMove[3], new Action_Func(rak::mem_fn(this, &CFishLayer::CoinAlpha)), actMove[4], new Action_Func(rak::mem_fn(pGameScene->m_layCoinObject, &CCoinLayer::CoinGoldEnd)), 0);
        /*actSequence->set_tag(tag);*/
        sprTenCoin1->run_action(actSequence);

        actMove[0] = new Action_Move_To(0.3, ptCoin+Point(0, -150));
        actMove[1] = new Action_Move_To(0.3, ptCoin+Point(0, 0));
        actMove[2] = new Action_Move_To(0.1, ptCoin+Point(0, -15));
        actMove[3] = new Action_Move_To(0.1, ptCoin+Point(0, 0));
        actMove[4] = new Action_Move_To(0.4, ptRole);
        actSequence = new Action_Sequence(new Action_Delay(0.08), actMove[0], actMove[1], actMove[2], actMove[3], new Action_Func(rak::mem_fn(this, &CFishLayer::CoinAlpha)), actMove[4], new Action_Func(rak::mem_fn(pGameScene->m_layCoinObject, &CCoinLayer::CoinGoldEnd)), 0);
        /*actSequence->set_tag(tag);*/
        sprTenCoin2->run_action(actSequence);
        
    }

    if (nCointOne == 1)
    {
        ptNode = node->position();
    }
    else if (nCointOne<=5)
    {
        ptNode = node->position()+Point(-((float)(nCointOne))/2*36, -(nCointOne/5)*36);
    }
    else
    {
        ptNode = node->position()+Point(-5.0f/2*36, -(nCointOne/5)*36);
    }

    for (int i=0; i<nCointOne; i++)
    {
        if (i>4)
        {
            ptCoin = ptNode + Point((i%5)*36, 36);
        }
        else
        {
            ptCoin = ptNode + Point(i*36, 0);
        }

        Sprite *sprTenCoin = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image("coin_silver_0"));
        Sprite *sprTenCoin1 = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image("coin_silver_0"));
        Sprite *sprTenCoin2 = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image("coin_silver_0"));

        sprTenCoin->set_position(ptCoin);
        sprTenCoin1->set_position(ptCoin+Point(0, 4));
        sprTenCoin2->set_position(ptCoin+Point(0, 8));

        sprTenCoin1->set_visible(false);
        sprTenCoin2->set_visible(false);

        Color_Rect ColorRect = sprTenCoin1->color();
        ColorRect.set_alpha(150);
        sprTenCoin1->set_color(ColorRect);
        ColorRect.set_alpha(80);
        sprTenCoin2->set_color(ColorRect);

        pGameScene->m_layCoinObject->add_child(sprTenCoin2);
        pGameScene->m_layCoinObject->add_child(sprTenCoin1);
        pGameScene->m_layCoinObject->add_child(sprTenCoin);
        


        Animation *aniCoin = Root::instance()->animation_manager()->animation("coin_silver");
        sprTenCoin->run_action(new Action_Repeat_Forever(new Action_Animation(0.06, aniCoin, false)));

        Action *actSequence;
        Action *actMove[5];

        actMove[0] = new Action_Move_To(0.3, ptCoin+Point(0, -150));
        actMove[1] = new Action_Move_To(0.3, ptCoin+Point(0, 0));
        actMove[2] = new Action_Move_To(0.1, ptCoin+Point(0, -15));
        actMove[3] = new Action_Move_To(0.1, ptCoin+Point(0, 0));
        actMove[4] = new Action_Move_To(0.4, ptRole);

        Action *actFunc = new Action_Func(rak::mem_fn(pGameScene->m_layCoinObject, &CCoinLayer::CoinSilverEnd));
        actFunc->set_tag(tag);

        actSequence = new Action_Sequence(actMove[0], actMove[1], actMove[2], actMove[3], actMove[4], actFunc, 0);
        sprTenCoin->run_action(actSequence);

        actMove[0] = new Action_Move_To(0.3, ptCoin+Point(0, -150));
        actMove[1] = new Action_Move_To(0.3, ptCoin+Point(0, 0));
        actMove[2] = new Action_Move_To(0.1, ptCoin+Point(0, -15));
        actMove[3] = new Action_Move_To(0.1, ptCoin+Point(0, 0));
        actMove[4] = new Action_Move_To(0.4, ptRole);
        actSequence = new Action_Sequence(new Action_Delay(0.04), actMove[0], actMove[1], actMove[2], actMove[3], new Action_Func(rak::mem_fn(this, &CFishLayer::CoinAlpha)), actMove[4], new Action_Func(rak::mem_fn(pGameScene->m_layCoinObject, &CCoinLayer::CoinSilverEnd)), 0);
        /*actSequence->set_tag(tag);*/
        sprTenCoin1->run_action(actSequence);

        actMove[0] = new Action_Move_To(0.3, ptCoin+Point(0, -150));
        actMove[1] = new Action_Move_To(0.3, ptCoin+Point(0, 0));
        actMove[2] = new Action_Move_To(0.1, ptCoin+Point(0, -15));
        actMove[3] = new Action_Move_To(0.1, ptCoin+Point(0, 0));
        actMove[4] = new Action_Move_To(0.4, ptRole);
        actSequence = new Action_Sequence(new Action_Delay(0.08), actMove[0], actMove[1], actMove[2], actMove[3], new Action_Func(rak::mem_fn(this, &CFishLayer::CoinAlpha)), actMove[4], new Action_Func(rak::mem_fn(pGameScene->m_layCoinObject, &CCoinLayer::CoinSilverEnd)), 0);
        /*actSequence->set_tag(tag);*/
        sprTenCoin2->run_action(actSequence);
        
    }

    int nOffset;
    std::string strPrex;
    bool bGotHead = false;
    int nSingleNumber = 0;
    Sprite *spValue[4]={0};
    std::ostringstream ostr;

    if (tag==pGameScene->GetMeChairID())
    {
        nOffset = 33;
        strPrex = "gold_number_";
    }
    else
    {
        nOffset = 28;
        strPrex = "silver_number_";
    }

    ostr << strPrex << 10;
    spValue[0] = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image(ostr.str()));

    nSingleNumber = (int)(nCoinCount/100);

    if (nSingleNumber>0)
    {
        bGotHead = true;
        ostr.str("");
        ostr << strPrex << nSingleNumber;
        spValue[1] = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image(ostr.str()));
    }

    nSingleNumber = (int)(nCoinCount%100/10);
    if (nSingleNumber>0 || bGotHead)
    {
        bGotHead = true;
        ostr.str("");
        ostr << strPrex << nSingleNumber;
        spValue[2] = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image(ostr.str()));
    }

    nSingleNumber = (int)(nCoinCount%10);
    ostr.str("");
    ostr << strPrex << nSingleNumber;
    spValue[3] = new Sprite(Root::instance()->imageset_manager()->imageset("role")->image(ostr.str()));

    Point pt = node->position();
    for (int i=0; i<4; i++)
    {
        if (spValue[i] != 0)
        {
            pt.x_ += nOffset;
            spValue[i]->set_position(pt);
            pGameScene->m_layCoinObject->add_child(spValue[i]);
            Action *act = new Action_Sequence(new Action_Scale_To(0.25, 1.25), new Action_Scale_To(0.25, 1.0), new Action_Delay(0.5), new Action_Func(rak::mem_fn(pGameScene->m_layCoinObject, &CCoinLayer::CoinValueEnd)), 0);
            spValue[i]->run_action(act);   
        }

    }

    remove_child(node);
    delete node->node_extend();
    delete node;

    return true;
}

int CFishLayer::FishCaptured(WORD wId, WORD wChairID)
{
    CGameScene *pGameScene = (CGameScene *)parent();

    Nodes::iterator i=child_begin();
    for (; i!=child_end(); ++i)
    {
        CFishObjectExtend *pFishObjectExtend = (CFishObjectExtend *)(*i)->node_extend();

        if ((pFishObjectExtend->wID) == wId)
        {
            if (pGameScene->GetMeChairID() == wChairID)
            {
                pGameScene->m_layAccount->m_CaptureFishs[(int)pFishObjectExtend->FishType]++;
            }

            std::ostringstream ostr;
            ostr << "fish_" << (int)pFishObjectExtend->FishType <<"_capture";

            Animation *aniDead = Root::instance()->animation_manager()->animation(ostr.str());
            Action *actDead = new Action_Animation(0.06, aniDead, false);

            Action *actFun = new Action_Func(rak::mem_fn(this, &CFishLayer::FishEnd));
            actFun->set_tag(wChairID);

            (*i)->stop_all_action();
            (*i)->run_action(new Action_Sequence(actDead, actFun, 0));

            pFishObjectExtend->wID = INVALID_WORD;

            if ((int)pFishObjectExtend->FishType > 6)
            {
                try
                {
                    Sound_Instance *pSound = Root::instance()->sound_manager()->sound_instance((int)pFishObjectExtend->FishType+3);
                    pSound->play(false, true);
                }
                catch(...)
                {
                }
            }

           pGameScene->m_layRoles[wChairID]->AddCookFish((int)pFishObjectExtend->FishType);

           return pFishObjectExtend->GetFishGoldByStyle();
        }
    }

    return 0;
}

bool CFishLayer::CoinAlpha(Node *node, int tag)
{
    node->set_visible(true);
   
    return true;
}
////////////////////////////////////////////////////////////
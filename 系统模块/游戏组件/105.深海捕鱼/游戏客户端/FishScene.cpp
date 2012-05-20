#include "FishApplication.h"
#include "FishScene.h"

#include "path.h"

#include <fstream>
#include <cmath>
#include <algorithm>

/////////////////////////////////////////////////////////////
CFishScene::CFishScene()
{
    //logic
    m_ClientKernel = NULL;

    m_nScene = 0;   
    m_nLastFire = 0;
    m_Timer.reset();

    for (int i=0; i<GAME_PLAYER; i++)
    {
        m_RoleObject[i].nID = INVALID_CHAIR;
        m_RoleObject[i].nBonus = 0;
        m_RoleObject[i].nCannonStyle = 0;
        m_RoleObject[i].nBulletStyle = 0;
        m_RoleObject[i].nBulletCount = 0;
    }

    char szPath[20];
    for (int i=1; i<111; i++)
    {
        sprintf(szPath, "path\\path%02d.pth", i);
        PathManager::instance()->add_path(szPath);
    }

    //frame
    Window_Event_Utilities::add_event_listener(this);
    INPUT_MANAGER()->add_input_listener(this);

    //create scene
    CreateScene(); 
    CreateUI();

    Root::instance()->add_frame_listener(this);
}

CFishScene::~CFishScene()
{
}

void CFishScene::enter()
{
    __super::enter();

    HWND hWnd = RENDER_WINDOW()->window();  

    m_ClientKernel = new CClientKernel;
    m_ClientKernel->InitClientKernel(hWnd, pApp->cmd_line(), this);
}

void CFishScene::visit(float dt)
{
    //delete 
    std::vector<Scene_Node*>::iterator k=m_NodeDelete.begin();
    for (; k!=m_NodeDelete.end(); ++k)
    {
        Scene_Node *parent = (*k)->parent();
        parent->remove_child((*k));
        (*k)->stop_all_action();
        delete (*k);
    }

    m_NodeDelete.clear();



    bool bFire = false;
    Scene_Nodes::iterator i=m_layBullet->child_begin();
    for (; i!=m_layBullet->child_end(); ++i)
    {
        Scene_Nodes::iterator j=m_layFish->child_begin();
        for (; j!=m_layFish->child_end(); ++j)
        {
            if (((*j)->tag()!=-1) && ((*j)->boundbox().pt_in_rect((*i)->position_absolute())))
            {
                bFire = true;
                FireFishnet((*i)->tag(), (*i)->position_absolute());
                
            }
        }

        if (bFire)
        {
            bFire = false;
            m_NodeDelete.push_back((*i));;
        }
    }

    //std::vector<Scene_Node*>::iterator m=m_NodeDelete.begin();

    //for (; m!=m_NodeDelete.end(); ++m)
    //{
    //    Scene_Node *parent = (*m)->parent();
    //    parent->remove_child((*m));
    //    (*m)->stop_all_action();
    //    delete (*m);
    //}

    //m_NodeDelete.clear();

    __super::visit(dt);
}

void CFishScene::window_moved(Render_Window* rw) 
{
}

void CFishScene::window_focus_change(Render_Window* rw) 
{
}

void CFishScene::window_resized(Render_Window* rw)
{
}

void CFishScene::window_closed(Render_Window* rw)
{
    //Window_Event_Utilities::remove_event_listener(this);
    //INPUT_MANAGER()->remove_input_listener(this);

   if (m_ClientKernel)
   {
        delete m_ClientKernel;
        m_ClientKernel = NULL;
   }
}

bool CFishScene::frame_started(const Frame_Event& evt)
{
    return true;
}

bool CFishScene::frame_render_targets(const Frame_Event& evt)
{
    CEGUI::System::getSingleton().renderGUI(); 
    return true;
}

bool CFishScene::frame_ended(const Frame_Event& evt)
{
    return true;
}

bool CFishScene::keyPressed( const OIS::KeyEvent &arg )
{
    switch (arg.key)
    {        
       case OIS::KC_1:
       case OIS::KC_2:
       case OIS::KC_3:
       case OIS::KC_4:
       case OIS::KC_5:
           {
               if (m_ClientKernel==NULL)
                   return true;

               int nChairID = m_ClientKernel->GetMeChairID();
               if (INVALID_CHAIR == nChairID)
                   return true;

               ChangeCannon(nChairID, arg.key-OIS::KC_1);
               SendChangeCannon(arg.key-OIS::KC_1);
               break;
           }
    }

    return true;
}

bool CFishScene::keyReleased( const OIS::KeyEvent &arg ) 
{ 
    return true;
}

bool CFishScene::mouseMoved( const OIS::MouseEvent &arg )
{ 
    if (m_ClientKernel==NULL)
        return true;

    int nChairID = m_ClientKernel->GetMeChairID();
    if (INVALID_CHAIR == nChairID)
        return true;

    CEGUI::System::getSingleton().injectMousePosition(arg.state.X.abs, arg.state.Y.abs);

    Point pt(arg.state.X.abs, arg.state.Y.abs);
    RotateCannon(nChairID, pt);



    return true;
}

bool CFishScene::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) 
{ 
    if (m_ClientKernel==NULL) 
        return true;

    WORD wChairId = m_ClientKernel->GetMeChairID();
    if (wChairId == INVALID_CHAIR)
        return true;

    CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MouseButton(id));

    unsigned long time = m_Timer.get_milli_seconds();
    if (time < m_GameLogic.GetFireInterval(m_RoleObject[wChairId].nBulletStyle))
        return true;
    m_Timer.reset();

    if (m_RoleObject[wChairId].nBulletCount <= 0)
    {
        CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();
        winmgr.getWindow("Root/BulletBuy/Buy")->enable();
        winmgr.getWindow("Root/BulletBuy")->setVisible(true);
    }
    else
    {

        FireBullet(wChairId, m_spCannon[wChairId]->rotation());
        SendFireBullet();
    }

    

    return true;
}

bool CFishScene::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{ 
    CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MouseButton(id));
    return true;
}

void CFishScene::ResetGameFrame() 
{
}

void CFishScene::CloseGameFrame() 
{
}

bool CFishScene::OnEventSocket(const CMD_Command & Command, const void * pBuffer, WORD wDataSize)
{ 
    if (MDM_GF_GAME !=Command.wMainCmdID) 
        return false;

    if (Command.wSubCmdID == SUB_S_FIRE)
    {
        return OnSubGameFire(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_BUY_BULLET)
    {
        return OnSubGameBuyBullet(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_CHANGE_CANNON)
    {
        return OnSubGameChangeCannon(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_ADD_FISH)
    {
        return OnSubGameAddFish(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_CAPTURE)
    {
        return OnSubGameCapture(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_CHANGE_SCENE)
    {
        return OnSubGameChangeScene(pBuffer, wDataSize);
    }

    return true;
}

bool CFishScene::OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)
{ 
    //效验数据
    if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

    //消息处理
    CMD_S_StatusFree *pStatusFree = (CMD_S_StatusFree *)pBuffer;

    m_nScene = pStatusFree->nScene;
    CopyMemory(m_RoleObject, pStatusFree->Role, GAME_PLAYER*sizeof(RoleNetObject));

    ChangeScene();

    return true; 
}

void CFishScene::OnEventUserMemberOrder(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
}

void CFishScene::OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
}

bool CFishScene::OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{ 
    m_RoleObject[wChairID].nID = INVALID_CHAIR;
    m_RoleObject[wChairID].nBonus = 0;
    m_RoleObject[wChairID].nCannonStyle = 0;
    m_RoleObject[wChairID].nBulletStyle = 0;
    m_RoleObject[wChairID].nBulletCount = 0;

    m_layRole->remove_child(m_spBoat[wChairID]);
    m_layRole->remove_child(m_spCannon[wChairID]); 

    char szPrex[32];
    char szDetial[32];

    sprintf(szPrex, "Root/Role%d/", wChairID);

    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();

    sprintf(szDetial, "%sNickname", szPrex);
    winmgr.getWindow(szDetial)->setVisible(false);
    sprintf(szDetial, "%sGold", szPrex);
    winmgr.getWindow(szDetial)->setVisible(false);
    sprintf(szDetial, "%sTicket", szPrex);
    winmgr.getWindow(szDetial)->setVisible(false);
    sprintf(szDetial, "%sBonus", szPrex);
    winmgr.getWindow(szDetial)->setVisible(false);
    sprintf(szDetial, "%sBulletText", szPrex);
    winmgr.getWindow(szDetial)->setVisible(false);
    winmgr.getWindow(szDetial)->setText("0");
    sprintf(szDetial, "%sBonusText", szPrex);
    winmgr.getWindow(szDetial)->setVisible(false);
    winmgr.getWindow(szDetial)->setText("0");
    sprintf(szDetial, "%sBullet0", szPrex);
    winmgr.getWindow(szDetial)->setVisible(false);
    sprintf(szDetial, "%sBullet1", szPrex);
    winmgr.getWindow(szDetial)->setVisible(false);
    sprintf(szDetial, "%sBullet2", szPrex);
    winmgr.getWindow(szDetial)->setVisible(false);

    return true; 
}

bool CFishScene::OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUserr)
{ 
    m_RoleObject[wChairID].nID = wChairID;
    m_RoleObject[wChairID].nBonus = 0;
    m_RoleObject[wChairID].nCannonStyle = 0;
    m_RoleObject[wChairID].nBulletStyle = 0;
    m_RoleObject[wChairID].nBulletCount = 0;

    m_layRole->add_child(m_spBoat[wChairID]);
    m_layRole->add_child(m_spCannon[wChairID]); 

    char szPrex[32];
    char szDetial[32];

    sprintf(szPrex, "Root/Role%d/", wChairID);

    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();

    if (wChairID == m_ClientKernel->GetMeChairID())
    {
        sprintf(szDetial, "%sNickname", szPrex);
        winmgr.getWindow(szDetial)->setVisible(true);
        sprintf(szDetial, "%sNickname/Text", szPrex);
        winmgr.getWindow(szDetial)->setText(pUserData->szName);

        sprintf(szDetial, "%sGold", szPrex);
        winmgr.getWindow(szDetial)->setVisible(false);
        sprintf(szDetial, "%sTicket", szPrex);
        winmgr.getWindow(szDetial)->setVisible(false);
    }
    else
    {
        sprintf(szDetial, "%sNickname", szPrex);
        winmgr.getWindow(szDetial)->setVisible(false);
        sprintf(szDetial, "%sNickname/Text", szPrex);
        winmgr.getWindow(szDetial)->setText(pUserData->szName);

        sprintf(szDetial, "%sGold", szPrex);
        winmgr.getWindow(szDetial)->setVisible(true);
        sprintf(szDetial, "%sTicket", szPrex);
        winmgr.getWindow(szDetial)->setVisible(true);
    }

    sprintf(szDetial, "%sBonus", szPrex);
    winmgr.getWindow(szDetial)->setVisible(true);
    sprintf(szDetial, "%sBulletText", szPrex);
    winmgr.getWindow(szDetial)->setVisible(true);
    winmgr.getWindow(szDetial)->setText("0");
    sprintf(szDetial, "%sBonusText", szPrex);
    winmgr.getWindow(szDetial)->setVisible(true);
    winmgr.getWindow(szDetial)->setText("0");

    sprintf(szDetial, "%sBullet0", szPrex);
    winmgr.getWindow(szDetial)->setVisible(true);
    sprintf(szDetial, "%sBullet1", szPrex);
    winmgr.getWindow(szDetial)->setVisible(false);
    sprintf(szDetial, "%sBullet2", szPrex);
    winmgr.getWindow(szDetial)->setVisible(false);

    return true; 
}

void CFishScene::OnEventUserScore(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
    char szPrex[32];
    char szDetial[32];
    char szGoldValue[32];
    sprintf(szGoldValue, "%d", pUserData->lScore);
    sprintf(szPrex, "Root/Role%d/", wChairID);
    sprintf(szDetial, "%sGold/Text", szPrex);

    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();
    winmgr.getWindow(szDetial)->setText(szGoldValue);

}

bool CFishScene::OnSubGameFire(const void * pBuffer, WORD wDataSize)
{
    if (m_ClientKernel==NULL) return true;

    if (wDataSize!=sizeof(CMD_S_FIRE)) return false;

    //消息处理
    CMD_S_FIRE * pGameFire=(CMD_S_FIRE *)pBuffer;

    WORD wChairId = pGameFire->nChairID;

    if (pGameFire->nChairID == m_ClientKernel->GetMeChairID())
    {
        return true;
    }

    m_spCannon[pGameFire->nChairID]->set_rotation(pGameFire->fRote);
    FireBullet(pGameFire->nChairID, pGameFire->fRote);

    return true;
}

bool CFishScene::OnSubGameBuyBullet(const void * pBuffer, WORD wDataSize)
{
    if (wDataSize!=sizeof(CMD_S_BUY_BULLET)) return false;

    //消息处理
    CMD_S_BUY_BULLET * pGameBuyBullet=(CMD_S_BUY_BULLET *)pBuffer;

    if (pGameBuyBullet->bSuccess = false)
    {
        CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();
        winmgr.getWindow("Root/BulletBuy")->enable();

        return true;
    }

    WORD wChairID = pGameBuyBullet->nChairID;
    m_RoleObject[wChairID].nBulletStyle = pGameBuyBullet->nStyle;
    m_RoleObject[wChairID].nBulletCount = pGameBuyBullet->nCount;

    char szPrex[32];
    char szDetial[32];
    char szBulletCount[32];

    sprintf(szPrex, "Root/Role%d/", wChairID);
    sprintf(szBulletCount, "%d", pGameBuyBullet->nCount);

    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();

    if (pGameBuyBullet->nStyle == 0)
    {
        sprintf(szDetial, "%sBullet0", szPrex);
        winmgr.getWindow(szDetial)->setVisible(true);
        sprintf(szDetial, "%sBullet1", szPrex);
        winmgr.getWindow(szDetial)->setVisible(false);
        sprintf(szDetial, "%sBullet2", szPrex);
        winmgr.getWindow(szDetial)->setVisible(false);
    }
    else if (pGameBuyBullet->nStyle == 1)
    {
        sprintf(szDetial, "%sBullet0", szPrex);
        winmgr.getWindow(szDetial)->setVisible(false);
        sprintf(szDetial, "%sBullet1", szPrex);
        winmgr.getWindow(szDetial)->setVisible(true);
        sprintf(szDetial, "%sBullet2", szPrex);
        winmgr.getWindow(szDetial)->setVisible(false);
    }
    else
    {
        sprintf(szDetial, "%sBullet0", szPrex);
        winmgr.getWindow(szDetial)->setVisible(false);
        sprintf(szDetial, "%sBullet1", szPrex);
        winmgr.getWindow(szDetial)->setVisible(false);
        sprintf(szDetial, "%sBullet2", szPrex);
        winmgr.getWindow(szDetial)->setVisible(true);
    }

    sprintf(szDetial, "%sBulletText", szPrex);
    winmgr.getWindow(szDetial)->setText(szBulletCount);

    winmgr.getWindow("Root/BulletBuy")->setVisible(false);

    return true;
}

bool CFishScene::OnSubGameChangeCannon(const void * pBuffer, WORD wDataSize)
{
    if (m_ClientKernel==NULL) return true;

    if (wDataSize!=sizeof(CMD_S_CHANGE_CANNON)) return false;

    //消息处理
    CMD_S_CHANGE_CANNON * pGameChangeCannon=(CMD_S_CHANGE_CANNON *)pBuffer;

    WORD wChairID = pGameChangeCannon->nChairID;
    int nCannonStyle = pGameChangeCannon->nStyle;

    m_RoleObject[wChairID].nCannonStyle = nCannonStyle;

    if (pGameChangeCannon->nChairID == m_ClientKernel->GetMeChairID())
    {
        return true;
    }

    ChangeCannon(pGameChangeCannon->nChairID, pGameChangeCannon->nStyle);

    return true;
}

bool CFishScene::OnSubGameAddFish(const void * pBuffer, WORD wDataSize)
{
    if (m_ClientKernel==NULL) return true;

    if (wDataSize!=sizeof(CMD_S_ADD_FISH)) return false;

    //消息处理
    CMD_S_ADD_FISH * pGameAddFish=(CMD_S_ADD_FISH *)pBuffer;


    for (int i=0; i<10; i++)
    {
        AddFish(pGameAddFish->Fish[i].nID, pGameAddFish->Fish[i].nStyle, pGameAddFish->Fish[i].nPath);
    }

    
    
    return true;
}

bool CFishScene::OnSubGameCapture(const void * pBuffer, WORD wDataSize)
{
    //效验数据
    if (wDataSize!=sizeof(CMD_S_CAPTURE)) return false;

    //消息处理
    CMD_S_CAPTURE *pCapture = (CMD_S_CAPTURE *)pBuffer;
    if (pCapture->bSuccess == false)
        return true;

    Sprite *Fish = (Sprite *)m_layFish->child_by_tag(pCapture->nFishID);
    if (Fish==0)
        return true;

    if (Fish->tag() == -1)
        return true;


    Fish->stop_all_action();

    Action_Finit_Time *act1 = new Action_Animation(0.05, Fish->animation_by_tag(1), false);
    Action_Finit_Time *act2 = new Action_Func(rak::mem_fn(this, &CFishScene::FishCaptureEnd));
    act2->set_tag(pCapture->nChairID);
    Action *action = new Action_Sequence(act1,act2, NULL);

    Fish->run_action(action);   
    ACTION_MANAGER()->resume_targer(Fish);
    Fish->set_tag(-1);

    return true;
}

bool CFishScene::OnSubGameChangeScene(const void * pBuffer, WORD wDataSize)
{
    //效验数据
    if (wDataSize!=sizeof(CMD_S_CHANGE_SCENE)) return false;

    //消息处理
    CMD_S_CHANGE_SCENE *pChangeScene = (CMD_S_CHANGE_SCENE *)pBuffer;

    if (m_nScene != pChangeScene->nScene)
    {
        m_nScene = pChangeScene->nScene;

        ChangeScene();

        m_spTideWater->set_position(Point(1024+262/2, 576/2));
        Action *act = new Action_Move_To(3, Point(-262/2, 576/2));
        m_spTideWater->run_action(act);
        ACTION_MANAGER()->resume_targer(m_spTideWater);
    }

    return true;
}

bool CFishScene::SendBuyBullet(int nBulletStyle)
{
    if (m_ClientKernel==NULL)
        return true;

    int nChairID = m_ClientKernel->GetMeChairID();
    if (m_RoleObject[nChairID].nBulletCount > 0)
        return true;

    CMD_C_BUY_BULLET BuyBullet;

    if (nBulletStyle == 0)
    {
        BuyBullet.nStyle = nBulletStyle;
        BuyBullet.nCount = 100;
    }
    else if (nBulletStyle == 1)
    {
        BuyBullet.nStyle = nBulletStyle;
        BuyBullet.nCount = 600;
    }
    else
    {
        BuyBullet.nStyle = nBulletStyle;
        BuyBullet.nCount = 800;
    }

    return m_ClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_BUY_BULLET, &BuyBullet, sizeof(CMD_C_BUY_BULLET));
}

bool CFishScene::SendChangeCannon(int nCannonStyle)
{
    if (m_ClientKernel==NULL)
        return true;

    CMD_C_CHANGE_CANNON ChangeCannon;
    ChangeCannon.nStyle = nCannonStyle;

    return m_ClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_CHANGE_CANNON, &ChangeCannon, sizeof(CMD_C_CHANGE_CANNON));
}

bool CFishScene::SendFireBullet()
{
    WORD wChairId = m_ClientKernel->GetMeChairID();
    if (wChairId == INVALID_CHAIR)
        return true;

    CMD_C_FIRE Fire;
    Fire.fRote = m_spCannon[wChairId]->rotation();
    return m_ClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_FIRE, &Fire, sizeof(CMD_C_FIRE));
}

bool CFishScene::SendCaptureFish(int nFishID)
{    
    Sprite *fish = (Sprite *)m_layFish->child_by_tag(nFishID);

    Animation *ani = fish->animation_by_tag(0);
    std::string szAni = ani->name();

    int nStyle;
    std::sscanf(szAni.c_str(), "Fish%d", &nStyle);

    CMD_C_CAPTURE Capture;
    Capture.nFishID = nFishID;
    Capture.nFishStyle = nStyle; 

    return m_ClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_CAPTURE, &Capture, sizeof(CMD_C_CAPTURE));
}

bool CFishScene::BulletEnd(Scene_Node *node, int tag)
{
    FireFishnet(node->tag(),node->position_absolute());

    m_NodeDelete.push_back(node);

    return true;
}

bool CFishScene::FishNetEnd(Scene_Node *node, int tag)
{   
    if (node->tag() == m_ClientKernel->GetMeChairID())
    {
        Scene_Nodes::iterator i=m_layFish->child_begin();
        for (; i!=m_layFish->child_end(); ++i)
        {
            if (((*i)->tag()!=-1) && ((*i)->boundbox().collision(node->boundbox())))
            {
                SendCaptureFish((*i)->tag());
            }
        }
    }

    m_NodeDelete.push_back(node);

    return true;
}

bool CFishScene::FishCaptureEnd(Scene_Node *node, int tag)
{ 
    Animation *ani = ((Sprite*)node)->animation_by_tag(0);
    std::string szAni = ani->name();

    int nStyle;
    std::sscanf(szAni.c_str(), "Fish%d", &nStyle);
   
    for (int i=0; i<nStyle; i++)
    {
        if (i>3)
            break;

        Point pt = node->position_absolute();
        pt.x_ += i*50;

        Sprite *spCoin = new Sprite(RESOURCE_MANAGER()->animation("Coin"));
        spCoin->set_tag(node->tag());
        spCoin->set_position(pt);
        spCoin->set_scalex(0.7);
        spCoin->set_scaley(0.7);

        m_layCoin->add_child(spCoin);

        Action *actionCoin1 = new Action_Sequence(new Action_Move_To(1, m_spCannon[tag]->position_absolute()),
                                                  new Action_Func(rak::mem_fn(this, &CFishScene::CoinEnd)), NULL);
        Action *actionCoin2 = new Action_Repeat_Forever(new Action_Animation(0.05, spCoin->animation_by_name("Coin"), false));
        Action *actionCoin3 = new Action_Scale_To(1, 1.5, 1.5);
        spCoin->run_action(actionCoin1);   
        spCoin->run_action(actionCoin2);
        spCoin->run_action(actionCoin3);
        ACTION_MANAGER()->resume_targer(spCoin);
    }

    m_NodeDelete.push_back(node);
    return true;
}

bool CFishScene::CoinEnd(Scene_Node *node, int tag)
{
    m_NodeDelete.push_back(node);

    return true;
}

bool CFishScene::FishEnd(Scene_Node *node, int tag)
{
    m_NodeDelete.push_back(node);

    return true;
}

void CFishScene::CreateScene()
{
    m_layBackground[0] = new Scene_Layer;
    m_layBackground[1] = new Scene_Layer;

    m_layFish = new Scene_Layer;
    m_layBullet = new Scene_Layer;
    m_layCoin = new Scene_Layer;
    m_layFishNet = new Scene_Layer;
    m_layRole = new Scene_Layer;

    add_child(m_layFish,2);
    add_child(m_layBullet,3);
    add_child(m_layFishNet,4);
    add_child(m_layCoin,5);
    add_child(m_layRole,6);

    Sprite *sprite;
    Action *action;
    Animation *animation;
    Sprite_Frame* frame;

    //background layer 1
    frame = RESOURCE_MANAGER()->sprite_frame("Bkground1");
    sprite = new Sprite(frame);
    sprite->set_position(Point(512,288));
    m_layBackground[0]->add_child(sprite);

    

    animation = RESOURCE_MANAGER()->animation("Seaweed1");
    sprite = new Sprite(animation);
    action = new Action_Repeat_Forever(new Action_Animation(0.15, animation, false));
    sprite->set_position(Point(608,170));
    sprite->run_action(action);
    m_layBackground[0]->add_child(sprite);

    animation = RESOURCE_MANAGER()->animation("Seaweed2");
    sprite = new Sprite(animation);
    action  = new Action_Repeat_Forever(new Action_Animation(0.15, animation, false));
    sprite->set_position(Point(754,384));
    sprite->run_action(action);
    m_layBackground[0]->add_child(sprite);

    animation = RESOURCE_MANAGER()->animation("Seaweed3");
    sprite = new Sprite(animation);
    action  = new Action_Repeat_Forever(new Action_Animation(0.15, animation, false));
    sprite->set_position(Point(455,436));
    sprite->run_action(action);
    m_layBackground[0]->add_child(sprite);

    animation = RESOURCE_MANAGER()->animation("Starfish");
    sprite  = new Sprite(animation);
    action  = new Action_Repeat_Forever(new Action_Animation(0.15, animation, false));
    sprite->set_position(Point(105,285)); 
    sprite->run_action(action);
    m_layBackground[0]->add_child(sprite);

    animation = RESOURCE_MANAGER()->animation("Starfish");
    sprite  = new Sprite(animation);
    action  = new Action_Repeat_Forever(new Action_Animation(0.15, animation, false));
    sprite->set_position(Point(164,555)); 
    sprite->run_action(action);
    m_layBackground[0]->add_child(sprite);

    animation = RESOURCE_MANAGER()->animation("Starfish");
    sprite  = new Sprite(animation);
    action  = new Action_Repeat_Forever(new Action_Animation(0.15, animation, false));
    sprite->set_position(Point(945,555)); 
    sprite->run_action(action);
    m_layBackground[0]->add_child(sprite);

    animation = RESOURCE_MANAGER()->animation("Starfish");
    sprite  = new Sprite(animation);
    action  = new Action_Repeat_Forever(new Action_Animation(0.15, animation, false));
    sprite->set_position(Point(1024,255)); 
    sprite->run_action(action);
    m_layBackground[0]->add_child(sprite);

    //background layer 2

    frame = RESOURCE_MANAGER()->sprite_frame("Bkground2");
    sprite= new Sprite(frame);
    sprite->set_position(Point(512,288));
    m_layBackground[1]->add_child(sprite);

    animation = RESOURCE_MANAGER()->animation("Grass");
    sprite = new Sprite(animation);
    action  = new Action_Repeat_Forever(new Action_Animation(0.15, animation, false));
    sprite->run_action(action);
    m_layBackground[1]->add_child(sprite);
    sprite->set_rotation(3.0);
    sprite->set_position(Point(140,100));

    animation = RESOURCE_MANAGER()->animation("Grass");
    sprite = new Sprite(animation);
    action  = new Action_Repeat_Forever(new Action_Animation(0.15, animation, false));
    sprite->run_action(action);
    m_layBackground[1]->add_child(sprite);
    sprite->set_rotation(2.8);
    sprite->set_position(Point(30,180));

    animation = RESOURCE_MANAGER()->animation("Grass");
    sprite = new Sprite(animation);
    action  = new Action_Repeat_Forever(new Action_Animation(0.15, animation, false));
    sprite->run_action(action);
    m_layBackground[1]->add_child(sprite);
    sprite->set_scalex(0.7);
    sprite->set_scaley(0.7);
    sprite->set_rotation(2.9);
    sprite->set_position(Point(180,240));

    animation = RESOURCE_MANAGER()->animation("Grass");
    sprite = new Sprite(animation);
    action  = new Action_Repeat_Forever(new Action_Animation(0.15, animation, false));
    sprite->run_action(action);
    m_layBackground[1]->add_child(sprite);
    sprite->set_scalex(0.8);
    sprite->set_scaley(0.6);
    sprite->set_rotation(2.8);
    sprite->set_position(Point(40,310));

    animation = RESOURCE_MANAGER()->animation("Grass");
    sprite = new Sprite(animation);
    action  = new Action_Repeat_Forever(new Action_Animation(0.15, animation, false));
    sprite->run_action(action);
    m_layBackground[1]->add_child(sprite);
    sprite->set_scalex(0.8);
    sprite->set_scaley(0.6);
    sprite->set_rotation(2.6);
    sprite->set_position(Point(60,410));

    animation = RESOURCE_MANAGER()->animation("Grass");
    sprite = new Sprite(animation);
    action  = new Action_Repeat_Forever(new Action_Animation(0.15, animation, false));
    sprite->run_action(action);
    m_layBackground[1]->add_child(sprite);
    //sprite->set_scalex(0.8);
    //sprite->set_scaley(0.6);
    sprite->set_rotation(1.57);
    sprite->set_position(Point(410,599));

    animation = RESOURCE_MANAGER()->animation("Grass");
    sprite = new Sprite(animation);
    action  = new Action_Repeat_Forever(new Action_Animation(0.15, animation, false));
    sprite->run_action(action);
    m_layBackground[1]->add_child(sprite);
    sprite->set_scalex(1.2);
    sprite->set_scaley(1.2);
    sprite->set_rotation(1.3);
    sprite->set_position(Point(730,620));

    animation = RESOURCE_MANAGER()->animation("Grass");
    sprite = new Sprite(animation);
    action  = new Action_Repeat_Forever(new Action_Animation(0.15, animation, false));
    sprite->run_action(action);
    m_layBackground[1]->add_child(sprite);
    //sprite->set_scalex(1.2);
    //sprite->set_scaley(1.2);
    sprite->set_rotation(0.4);
    sprite->set_position(Point(880,460));

    animation = RESOURCE_MANAGER()->animation("Grass");
    sprite = new Sprite(animation);
    action  = new Action_Repeat_Forever(new Action_Animation(0.15, animation, false));
    sprite->run_action(action);
    m_layBackground[1]->add_child(sprite);
    //sprite->set_scalex(1.2);
    //sprite->set_scaley(1.2);
    //sprite->set_rotation(0.4);
    sprite->set_position(Point(980,180));

    animation = RESOURCE_MANAGER()->animation("Grass");
    sprite = new Sprite(animation);
    action  = new Action_Repeat_Forever(new Action_Animation(0.15, animation, false));
    sprite->run_action(action);
    m_layBackground[1]->add_child(sprite);
    //sprite->set_scalex(1.2);
    //sprite->set_scaley(1.2);
    sprite->set_rotation(3.9);
    sprite->set_position(Point(630,-10));

    animation = RESOURCE_MANAGER()->animation("Grass");
    sprite = new Sprite(animation);
    action  = new Action_Repeat_Forever(new Action_Animation(0.15, animation, false));
    sprite->run_action(action);
    m_layBackground[1]->add_child(sprite);
    sprite->set_scalex(1.3);
    sprite->set_scaley(1.3);
    sprite->set_rotation(3.9);
    sprite->set_position(Point(340,0));

    frame = RESOURCE_MANAGER()->sprite_frame("Tidewater");
    m_spTideWater = new Sprite(frame);
    m_spTideWater->set_position(Point(1024+262/2, 576/2));
    m_layCoin->add_child(m_spTideWater);
    

    // role layer
    for (int i=0; i<4; i++)
    {
       animation = RESOURCE_MANAGER()->animation("CannonYellow");
       animation->set_tag(0);
       m_spCannon[i] = new Sprite(animation);
       animation = RESOURCE_MANAGER()->animation("CannonGreen");
       animation->set_tag(1);
       m_spCannon[i]->add_animation(animation);
       animation = RESOURCE_MANAGER()->animation("CannonBlue");
       animation->set_tag(2);
       m_spCannon[i]->add_animation(animation);
       animation = RESOURCE_MANAGER()->animation("CannonPurple");
       animation->set_tag(3);
       m_spCannon[i]->add_animation(animation);
       animation = RESOURCE_MANAGER()->animation("CannonRed");
       animation->set_tag(4);
       m_spCannon[i]->add_animation(animation);
       m_spCannon[i]->set_hoty(0.64);
    }

    m_spCannon[0]->set_rotation(M_PI);
    m_spCannon[0]->set_position(Point(250,32));
    m_spCannon[1]->set_rotation(M_PI);
    m_spCannon[1]->set_position(Point(774,32));
    m_spCannon[2]->set_position(Point(774,544));
    m_spCannon[3]->set_position(Point(250,544));

    for (int i=0; i<4; i++)
    {
        animation = RESOURCE_MANAGER()->animation("Boat");
        m_spBoat[i] = new Sprite(animation);
    }

    m_spBoat[0]->set_rotation(M_PI);
    m_spBoat[0]->set_position(Point(250,28));
    m_spBoat[1]->set_rotation(M_PI);
    m_spBoat[1]->set_position(Point(774,28));
    m_spBoat[2]->set_position(Point(774,548));
    m_spBoat[3]->set_position(Point(250,548));
}

void CFishScene::CreateUI()
{
    m_ceguiRenderer =new CEGUI::DirectX9Renderer( Root::instance()->render_system()->device(), 0 ); 
    m_ceguiSystem =new CEGUI::System( m_ceguiRenderer );
    CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>   
        (CEGUI::System::getSingleton().getResourceProvider());    

    rp->setResourceGroupDirectory("schemes", "datafiles/schemes/");    
    rp->setResourceGroupDirectory("imagesets",     
        "datafiles/imagesets/");    
    rp->setResourceGroupDirectory("fonts", "datafiles/fonts/");    
    rp->setResourceGroupDirectory("layouts", "datafiles/layouts/");    
    rp->setResourceGroupDirectory("looknfeels", "datafiles/looknfeel/");    
    rp->setResourceGroupDirectory("lua_scripts", "datafiles/lua_scripts/"); 

    CEGUI::Imageset::setDefaultResourceGroup("imagesets");    
    CEGUI::Font::setDefaultResourceGroup("fonts");    
    CEGUI::Scheme::setDefaultResourceGroup("schemes");    
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");    
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");    
    CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");  

    CEGUI::SchemeManager::getSingleton().loadScheme("WindowsLook.scheme");

    if (!CEGUI::FontManager::getSingleton().isFontPresent("Commonwealth-10"))
        CEGUI::FontManager::getSingleton().createFont("Commonwealth-10.font");

    if (!CEGUI::FontManager::getSingleton().isFontPresent("simsun"))
        CEGUI::FontManager::getSingleton().createFont("simsun.font");

    CEGUI::System::getSingleton().setDefaultMouseCursor("WindowsLook", "MouseArrow");
    CEGUI::System::getSingleton().setDefaultFont("simsun");

    CEGUI::ImagesetManager::getSingleton().createImageset("UI.imageset");

    CEGUI::System::getSingleton().setGUISheet(
        CEGUI::WindowManager::getSingleton().loadWindowLayout("fish.layout"));

    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();

    CEGUI::PushButton *btnBulletBuyBuy = static_cast<CEGUI::PushButton *>(winmgr.getWindow("Root/BulletBuy/Buy"));
    btnBulletBuyBuy->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CFishScene::HandleBtnBulletBuyBuy, this));

    CEGUI::PushButton *btnBulletBuyExit = static_cast<CEGUI::PushButton *>(winmgr.getWindow("Root/BulletBuy/Exit"));
    btnBulletBuyExit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CFishScene::HandleBtnBulletBuyExit, this));

    CEGUI::PushButton *btnControlHelp = static_cast<CEGUI::PushButton *>(winmgr.getWindow("Root/WindowMenu/BtnHelp"));
    btnControlHelp->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CFishScene::HandleBtnControlHelp, this));

    CEGUI::PushButton *btnControlBullet = static_cast<CEGUI::PushButton *>(winmgr.getWindow("Root/WindowMenu/BtnBullet"));
    btnControlBullet->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CFishScene::HandleBtnControlBullet, this));

    CEGUI::Window*  windHelp = winmgr.getWindow("Root/Help");
    windHelp->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&CFishScene::HandleCloseWindHelp, this));

    CEGUI::Window*  winBulletBuy = winmgr.getWindow("Root/BulletBuy");
    winBulletBuy->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&CFishScene::HandleCloseWinBulletBuy, this));

    CEGUI::RadioButton *rb100 = static_cast<CEGUI::RadioButton *>(winmgr.getWindow("Root/BulletBuy/Check100"));
    rb100->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&CFishScene::HandleCheckBullet, this));
    rb100->setSelected(true);

    CEGUI::RadioButton *rb600 = static_cast<CEGUI::RadioButton *>(winmgr.getWindow("Root/BulletBuy/Check600"));
    rb100->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&CFishScene::HandleCheckBullet, this));

    CEGUI::RadioButton *rb800 = static_cast<CEGUI::RadioButton *>(winmgr.getWindow("Root/BulletBuy/Check800"));
    rb800->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&CFishScene::HandleCheckBullet, this));

    winmgr = CEGUI::WindowManager::getSingleton ();
    winmgr.getWindow("Root/Help")->setVisible(false);
}

void CFishScene::ChangeScene()
{
    if (m_nScene == 0)
    {
        add_child(m_layBackground[0],1);

        for (Scene_Nodes::iterator i=m_layBackground[0]->child_begin(); i!=m_layBackground[0]->child_end(); ++i)
        {
            ACTION_MANAGER()->resume_targer(*i);
        }
        for (Scene_Nodes::iterator i=m_layBackground[1]->child_begin(); i!=m_layBackground[1]->child_end(); ++i)
        {
            ACTION_MANAGER()->pause_target(*i);
        }
    }
    else
    {
        for (Scene_Nodes::iterator i=m_layBackground[0]->child_begin(); i!=m_layBackground[0]->child_end(); ++i)
        {
            ACTION_MANAGER()->pause_target(*i);
        }
        for (Scene_Nodes::iterator i=m_layBackground[1]->child_begin(); i!=m_layBackground[1]->child_end(); ++i)
        {
            //ACTION_MANAGER()->resume_targer(*i);
        }

        add_child(m_layBackground[1],1);
    }
}

void CFishScene::ChangeCannon(int nChairID, int nCannonStyle)
{
    Animation *ani;
    ani = m_spCannon[nChairID]->animation_by_tag(nCannonStyle);

    m_spCannon[nChairID]->set_display_frame(ani->at(0));
}

void CFishScene::RotateCannon(int nChairID, const Point &ptMouse)
{
    Point pt(ptMouse);
    float fRorate;

    if (nChairID<2)
    {
        pt = pt-m_spCannon[nChairID]->position_absolute();
        if (pt.y_>=0)
        {
            fRorate= std::acos(pt.x_/std::sqrt(pt.x_*pt.x_+pt.y_*pt.y_))+M_PI_2;
        }
        else
        {
            if (pt.x_ >= 0)
                fRorate= M_PI_2;
            else
                fRorate = M_PI_2+M_PI;
        }
    }
    else
    {
        pt = m_spCannon[nChairID]->position_absolute() - pt;
        if (pt.y_>=0)
        {
            fRorate= std::acos(pt.x_/std::sqrt(pt.x_*pt.x_+pt.y_*pt.y_))-M_PI_2;
        }
        else
        {
            if (pt.x_ <= 0)
                fRorate= M_PI_2;
            else
                fRorate = M_PI_2+M_PI;
        }
    }

    m_spCannon[nChairID]->set_rotation(fRorate);

}

void CFishScene::FireBullet(int nChairID, float fRote)
{
    if (m_RoleObject[nChairID].nBulletCount <= 0)
    {
        return ;
    }

    Point pt = m_spCannon[nChairID]->position_absolute();
    float fRadius = m_GameLogic.GetBulletRadius(m_RoleObject[nChairID].nCannonStyle);
    Point ptStart (pt.x_+56*std::cos(fRote-M_PI_2), pt.y_+56*std::sin(fRote-M_PI_2));
    Point ptEnd (pt.x_+fRadius*std::cos(fRote-M_PI_2), pt.y_+fRadius*std::sin(fRote-M_PI_2));

    Sprite *spBullet = new Sprite(RESOURCE_MANAGER()->sprite_frame("Bullet"));
    spBullet->set_tag(nChairID);
    spBullet->set_position(ptStart);
    spBullet->set_rotation(fRote-M_PI_2);
    m_layBullet->add_child(spBullet);

    Action *act = new Action_Sequence(new Action_Move_To(2, ptEnd), 
                                      new Action_Func(rak::mem_fn(this, &CFishScene::BulletEnd)), NULL); 
    spBullet->run_action(act);
    ACTION_MANAGER()->resume_targer(spBullet);

    m_RoleObject[nChairID].nBulletCount--;

    char szPrex[32];
    char szDetial[32];
    char szBulletCount[32];

    sprintf(szPrex, "Root/Role%d/", nChairID);
    sprintf(szBulletCount, "%d", m_RoleObject[nChairID].nBulletCount);

    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();

    sprintf(szDetial, "%sBulletText", szPrex);
    winmgr.getWindow(szDetial)->setText(szBulletCount);
}

void CFishScene::FireFishnet(int nChairID, Point ptPosition)
{
    Animation *animation = RESOURCE_MANAGER()->animation("Fishnet");
    Sprite *spFishNet = new Sprite(animation);
    spFishNet->set_tag(nChairID);
    spFishNet->set_position(ptPosition);
    spFishNet->set_color(m_GameLogic.GetFishNetColor(m_RoleObject[nChairID].nCannonStyle));
    m_layFishNet->add_child(spFishNet);

    Action *action = new Action_Sequence(new Action_Animation(0.05, spFishNet->animation_by_name("Fishnet"), false), 
                                         new Action_Func(rak::mem_fn(this, &CFishScene::FishNetEnd)), NULL);
    spFishNet->run_action(action);
    ACTION_MANAGER()->resume_targer(spFishNet);
}

void CFishScene::AddFish(int nID, int nStyle, int nPath)
{
    char szAnimation[20];
    char szAnimationCapture[20];
    Path *path = PathManager::instance()->at(nPath);
    Animation *animation;
   
    sprintf(szAnimation, "Fish%d", nStyle);
    sprintf(szAnimationCapture, "Fish%dCapture", nStyle);

    animation = RESOURCE_MANAGER()->animation(szAnimation);
    Sprite *spFish = new Sprite(animation);
    animation->set_tag(0);
    Action *action = new Action_Repeat_Forever(new Action_Animation(0.05, animation, false));
    action->set_tag(0);
    spFish->run_action(action);

    animation = RESOURCE_MANAGER()->animation(szAnimationCapture);
    animation->set_tag(1);
    spFish->add_animation(animation);

    spFish->set_tag(nID);
    spFish->set_position(path->at(0));
    m_layFish->add_child(spFish);

    Action_Finit_Time *actPath[5]={0,0,0,0,0};
    int nActCount = 0;
    Point Bezier[3];

    for (int i=1; i<path->size(); i++)
    {
        int nEnd = (i-1)%3;      
        Bezier[nEnd] = path->at(i);

        if (nEnd == 2)
        {
            actPath[nActCount] = new Action_Bezier_By(8, Bezier[0],Bezier[1], Bezier[2]);
            nActCount++;

            if (nActCount>3)
            {
                break;
            }
        }
    }

    actPath[nActCount] = new Action_Func(rak::mem_fn(this, &CFishScene::FishEnd));

    Action *actPaths = new Action_Sequence(actPath[0], actPath[1], actPath[2], actPath[3], actPath[4],  NULL);
    actPaths->set_tag(1);
    spFish->run_action(actPaths);
    ACTION_MANAGER()->resume_targer(spFish);

}

bool CFishScene::HandleBtnBulletBuyBuy(const CEGUI::EventArgs& args)
{
    if (m_ClientKernel==NULL)
        return true;

    int nChairID = m_ClientKernel->GetMeChairID();
    if (INVALID_CHAIR == nChairID)
        return true;

    if (m_RoleObject[nChairID].nBulletCount > 0)
        return true;

    int nBulletStyle = -1;

    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();

    CEGUI::RadioButton *rb100 = static_cast<CEGUI::RadioButton *>(winmgr.getWindow("Root/BulletBuy/Check100"));
    rb100->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&CFishScene::HandleCheckBullet, this));

    CEGUI::RadioButton *rb600 = static_cast<CEGUI::RadioButton *>(winmgr.getWindow("Root/BulletBuy/Check600"));
    rb100->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&CFishScene::HandleCheckBullet, this));

    CEGUI::RadioButton *rb800 = static_cast<CEGUI::RadioButton *>(winmgr.getWindow("Root/BulletBuy/Check800"));
    rb800->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&CFishScene::HandleCheckBullet, this));

    if (rb100->isSelected())
    {
        nBulletStyle = 0;
    }
    else if (rb600->isSelected())
    {
        nBulletStyle = 1;
    }
    else if (rb800->isSelected())
    {
        nBulletStyle = 2;
    }

    if (nBulletStyle < 0)
        return true;

    SendBuyBullet(nBulletStyle);

    static_cast<const CEGUI::WindowEventArgs&> (args).window->disable();

    return true;
}

bool CFishScene::HandleBtnBulletBuyExit(const CEGUI::EventArgs& args) // 结算消息
{
    return true;
}

bool CFishScene::HandleBtnControlHelp(const CEGUI::EventArgs& args)
{
    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();
    winmgr.getWindow("Root/Help")->setVisible(true);
    return true;
}

bool CFishScene::HandleBtnControlBullet(const CEGUI::EventArgs& args)
{
    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();
    winmgr.getWindow("Root/BulletBuy")->setVisible(true);
    return true;
}

bool CFishScene::HandleCloseWindHelp(const CEGUI::EventArgs& args)
{
    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();
    winmgr.getWindow("Root/Help")->setVisible(false);
    return true;
}

bool CFishScene::HandleCloseWinBulletBuy(const CEGUI::EventArgs& args)
{
    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();
    winmgr.getWindow("Root/BulletBuy")->setVisible(false);
    return true;
}

bool CFishScene::HandleCheckBullet(const CEGUI::EventArgs& args)
{
    std::string text = static_cast<const CEGUI::WindowEventArgs&> (args).window->getText ().c_str();

    if (text == "Root/BulletBuy/Check100")
    {
        
    }
    else if (text == "Root/BulletBuy/Check600")
    {
       
    }
    else
    {
    }

    return true;
}
#include "FishApplication.h"
#include "FishScene.h"

#include "path.h"

#include <fstream>
#include <cmath>
#include <algorithm>

#include "tinyxml.h"

CEGUI::String CharArrayToString(const char* pszText)
{
    //      ASCII -> Unicode
    int nLen = strlen(pszText);
    std::wstring unicode(nLen, 0);
    unicode.resize(
        MultiByteToWideChar(CP_ACP, 0, pszText, static_cast<int>(nLen),
        const_cast<wchar_t*>(unicode.data()), unicode.size())
        );

    //      Unicode -> UTF-8
    char buff[128] = "";
    WideCharToMultiByte(CP_UTF8, 0, unicode.data(), static_cast<int>(unicode.size()),
        buff, sizeof(buff), NULL, NULL);

    return CEGUI::String((CEGUI::utf8*)(buff));
}

/////////////////////////////////////////////////////////////
CFishScene::CFishScene()
{
    m_bMouseRaw = false;

    //logic
    m_ClientKernel = NULL;

    m_nScene = 0;   
    m_nLastFire = 0;
    m_nUITime = 0;
    m_Timer.reset();
    m_UITimer.reset();

    for (int i=0; i<GAME_PLAYER; i++)
    {
        m_RoleObject[i].nID = INVALID_CHAIR;
        m_RoleObject[i].nBonus = 0;
        m_RoleObject[i].nCannonStyle = 0;
        m_RoleObject[i].nBulletStyle = 0;
        m_RoleObject[i].nBulletCount = 0;
    }

    for (int i=0; i<11; i++)
    {
        m_nFishCount[i] = 0;
    }

    char szPath[20];
    for (int i=1; i<111; i++)
    {
        sprintf(szPath, "path\\path%02d.pth", i);
        PathManager::instance()->add_path(szPath);
    }

    TiXmlDocument xml;
    xml.LoadFile("fish\\config.xml");

    m_bSound = 1;
    m_bMusic = 1;

    m_fSound = 1;
    m_fMusic = 1;

    TiXmlElement *elemet = xml.FirstChildElement("sound");
    elemet->Attribute("enable",&m_bSound);
    elemet->Attribute("value",&m_fSound);

    elemet = xml.FirstChildElement("music");
    elemet->Attribute("enable",&m_bMusic);
    elemet->Attribute("value",&m_fMusic);

    //frame
    Window_Event_Utilities::add_event_listener(this);
    INPUT_MANAGER()->add_input_listener(this);

    //create scene
    CreateScene(); 
    CreateUI();

    int sound_id = RESOURCE_MANAGER()->sound("Scene1");
    sound_scene_[0] = SOUND_MANAGER()->sound_instance(sound_id);
    sound_id = RESOURCE_MANAGER()->sound("Scene2");
    sound_scene_[1] = SOUND_MANAGER()->sound_instance(sound_id);

    sound_id = RESOURCE_MANAGER()->sound("WaveEnter");
    m_TideWater = SOUND_MANAGER()->sound_instance(sound_id);
    

    Root::instance()->add_frame_listener(this);
}

CFishScene::~CFishScene()
{
    sound_scene_[0]->release();
    sound_scene_[1]->release();
    m_TideWater->release();
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
    if (INPUT_MANAGER()->keyboard()->isKeyDown(OIS::KC_UP)||
        INPUT_MANAGER()->keyboard()->isKeyDown(OIS::KC_SPACE)||
        (INPUT_MANAGER()->mouse()->getMouseState().buttonDown(OIS::MB_Left)&&m_bMouseRaw))
    {
        InputFire();
    }


    //if (INPUT_MANAGER()->keyboard()->isKeyDown(OIS::KC_DOWN))
    //{
    //}

    if (INPUT_MANAGER()->keyboard()->isKeyDown(OIS::KC_LEFT))
    {
        if (m_ClientKernel==NULL)
            return ;

        int nChairID = m_ClientKernel->GetMeChairID();
        if (INVALID_CHAIR == nChairID)
            return ;

        float fRorate = m_spCannon[nChairID]->rotation();
        if (nChairID < 2)
        {
            fRorate += 0.05;

            if (fRorate > M_PI_2+M_PI)
                fRorate =  M_PI_2+M_PI;
        }
        else
        {
            fRorate -= 0.05;

            if (fRorate < -M_PI_2 )
                fRorate =  -M_PI_2;
        }

        m_spCannon[nChairID]->set_rotation(fRorate);

    }

    if (INPUT_MANAGER()->keyboard()->isKeyDown(OIS::KC_RIGHT))
    {
        if (m_ClientKernel==NULL)
            return ;

        int nChairID = m_ClientKernel->GetMeChairID();
        if (INVALID_CHAIR == nChairID)
            return ;

        float fRorate = m_spCannon[nChairID]->rotation();

        if (nChairID <2)
        {
            fRorate -= 0.05;

            if (fRorate < M_PI_2)
                fRorate =  M_PI_2;
        }
        else
        {
            fRorate += 0.05;

            if (fRorate > M_PI_2)
                fRorate =  M_PI_2;
        }

        m_spCannon[nChairID]->set_rotation(fRorate);
    }

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
    Point ptBullet;
    Rect rcScreen(0,0,1024,570);
    Scene_Nodes::iterator i=m_layBullet->child_begin();
    for (; i!=m_layBullet->child_end(); ++i)
    {
        ptBullet = (*i)->position();

        Scene_Nodes::iterator j=m_layFish->child_begin();
        for (; j!=m_layFish->child_end(); ++j)
        {
            if (!rcScreen.pt_in_rect((*j)->position()))
            {
                continue;
            }

            if ((*j)->tag()!=-1)
            {
                int nStyle;
                Animation *ani = ((Sprite*)(*j))->animation_by_tag(0);
                std::string szAni = ani->name();
                std::sscanf(szAni.c_str(), "Fish%d", &nStyle);

                if (nStyle == 10)
                {
                    if (m_GameLogic.CollisionShark((*j)->position(), ptBullet, 0,0))
                    {
                        bFire = true;
                    }
                }
                else
                {
                    if (m_GameLogic.CollisionFish((*j)->position(), ptBullet, m_GameLogic.GetFishRadius(nStyle),0))
                    {
                        bFire = true;     
                    }
                }
            }

            if (!rcScreen.pt_in_rect(ptBullet))
            {
                bFire = true;
            }

        }

        if (bFire)
        {
            bFire = false;
            FireFishnet((*i)->tag(), ptBullet);
            m_NodeDelete.push_back((*i));
        }
    }

    std::vector<Scene_Node*>::iterator m=m_NodeDelete.begin();

    for (; m!=m_NodeDelete.end(); ++m)
    {
        Scene_Node *parent = (*m)->parent();
        parent->remove_child((*m));
        (*m)->stop_all_action();
        delete (*m);
    }

    m_NodeDelete.clear();

    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();
    CEGUI::Window *pFishValueImage=(CEGUI::WindowManager::getSingleton().getWindow("Root/WindowMenu/FishValue"));   
    unsigned long time = m_UITimer.get_milli_seconds();
    if (time < 2000)
    {
        pFishValueImage->setAlpha(time/2000.0);
    }
    else if (time < 10000 && time > 8000)
    {

        pFishValueImage->setAlpha((10000-time)/2000.0);
    }
    else if(time >= 10000)
    {
        m_nUITime++;
        m_nUITime%=6;

        char szImage[32];
        sprintf(szImage, "set:UI image:FishValue%d", m_nUITime);
       
        pFishValueImage->setProperty("Image", szImage);
        pFishValueImage->setAlpha(0);
        
        m_UITimer.reset();
    }

    __super::visit(dt);
}

void CFishScene::window_moved(Render_Window* rw) 
{
}

void CFishScene::window_focus_change(Render_Window* rw) 
{
    m_bMouseRaw = false;
}

void CFishScene::window_resized(Render_Window* rw)
{
}

void CFishScene::window_closed(Render_Window* rw)
{
    //Window_Event_Utilities::remove_event_listener(this);
    //INPUT_MANAGER()->remove_input_listener(this);
    TiXmlDocument xml;
    xml.LoadFile("fish\\config.xml");

    TiXmlElement *elemet = xml.FirstChildElement("sound");
    elemet->SetAttribute("enable",m_bSound);
    elemet->SetDoubleAttribute("value",m_fSound);

    elemet = xml.FirstChildElement("music");
    elemet->SetAttribute("enable",m_bMusic);
    elemet->SetDoubleAttribute("value",m_fMusic);

    xml.SaveFile();

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
    if (m_ClientKernel==NULL)
        return true;

    int nChairID = m_ClientKernel->GetMeChairID();
    if (INVALID_CHAIR == nChairID)
        return true;

    switch (arg.key)
    {        
       case OIS::KC_1:
       case OIS::KC_2:
       case OIS::KC_3:
       case OIS::KC_4:
       case OIS::KC_5:
           {
               ChangeCannon(nChairID, arg.key-OIS::KC_1);
               SendChangeCannon(arg.key-OIS::KC_1);
               break;
           }
       case OIS::KC_SPACE:
       case OIS::KC_UP:
           {

               InputFire();
               break;
           }
       case OIS::KC_DOWN:
           {
               int nCannonStyle = (m_RoleObject[nChairID].nCannonStyle+1)%5;

               ChangeCannon(nChairID, nCannonStyle);
               SendChangeCannon(nCannonStyle); 
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

    if (arg.state.Y.abs < 30)
        return true;

    bool bHandle = CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MouseButton(id));
    if (bHandle)
    {
        m_bMouseRaw = false;
        return true;
    }

    m_bMouseRaw = true;

    if (id == 0)
    {
        unsigned long time = m_Timer.get_milli_seconds();
        if (time < m_GameLogic.GetFireInterval(m_RoleObject[wChairId].nBulletStyle))
            return true;
        m_Timer.reset();

        CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();

        if (!winmgr.getWindow("Root/BulletBuy")->isVisible())
        {   

            if (m_RoleObject[wChairId].nBulletCount <= m_RoleObject[wChairId].nCannonStyle)
            {
                PlaySample("NoBullet");
                winmgr.getWindow("Root/BulletBuy/Buy")->enable();
                winmgr.getWindow("Root/BulletBuy/Effect")->setVisible(false);
                winmgr.getWindow("Root/BulletBuy/Effect1")->setVisible(false);
                winmgr.getWindow("Root/BulletBuy")->setVisible(true);
            }
            else if (m_RoleObject[wChairId].nBulletCount > m_RoleObject[wChairId].nCannonStyle)
            {
                FireBullet(wChairId, m_RoleObject[wChairId].nCannonStyle, m_spCannon[wChairId]->rotation());
                SendFireBullet();
            }
        }
    }

    if (id == 1)
    {
        if (m_ClientKernel==NULL)
            return true;

        int nChairID = m_ClientKernel->GetMeChairID();
        if (INVALID_CHAIR == nChairID)
            return true;

        
        int nCannonStyle = (m_RoleObject[nChairID].nCannonStyle+1)%5;

        ChangeCannon(nChairID, nCannonStyle);
        SendChangeCannon(nCannonStyle);       
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
    else if (Command.wSubCmdID == SUB_S_SCENE_ADD_FISH)
    {
        return OnSubGameSceneAddFish(pBuffer, wDataSize);
    }
    else if (Command.wSubCmdID == SUB_S_ACCOUNT)
    {
        return OnSubGameAccount(pBuffer, wDataSize);
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
    m_layRole->remove_child(m_spCannontag[wChairID]); 
    m_layRole->remove_child(m_spCannonnum[wChairID]);

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
    m_layRole->add_child(m_spCannontag[wChairID]); 
    m_layRole->add_child(m_spCannonnum[wChairID]);

    char szPrex[32];
    char szDetial[32];

    sprintf(szPrex, "Root/Role%d/", wChairID);

    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();

    if (m_ClientKernel==NULL)
        return true;

    if (wChairID == m_ClientKernel->GetMeChairID())
    {
        sprintf(szDetial, "%sNickname", szPrex);
        winmgr.getWindow(szDetial)->setVisible(false);
        sprintf(szDetial, "%sNickname/Text", szPrex);
        winmgr.getWindow(szDetial)->setText(CharArrayToString(pUserData->szName));

        sprintf(szDetial, "%sGold", szPrex);
        winmgr.getWindow(szDetial)->setVisible(true);
        sprintf(szDetial, "%sTicket", szPrex);
        winmgr.getWindow(szDetial)->setVisible(true);
    }
    else
    {
        sprintf(szDetial, "%sNickname", szPrex);
        winmgr.getWindow(szDetial)->setVisible(true);
        sprintf(szDetial, "%sNickname/Text", szPrex);
        winmgr.getWindow(szDetial)->setText(CharArrayToString(pUserData->szName));

        sprintf(szDetial, "%sGold", szPrex);
        winmgr.getWindow(szDetial)->setVisible(false);
        sprintf(szDetial, "%sTicket", szPrex);
        winmgr.getWindow(szDetial)->setVisible(false);
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
    FireBullet(pGameFire->nChairID, pGameFire->nStyle, pGameFire->fRote);

    return true;
}

bool CFishScene::OnSubGameBuyBullet(const void * pBuffer, WORD wDataSize)
{
    if (wDataSize!=sizeof(CMD_S_BUY_BULLET)) return false;

    //消息处理
    CMD_S_BUY_BULLET * pGameBuyBullet=(CMD_S_BUY_BULLET *)pBuffer;

    if (pGameBuyBullet->bSuccess == false && pGameBuyBullet->nChairID == m_ClientKernel->GetMeChairID())
    {
        CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();
        winmgr.getWindow("Root/BulletBuy/Buy")->enable();
        winmgr.getWindow("Root/BulletBuy/Effect")->setVisible(false);
        winmgr.getWindow("Root/BulletBuy/Effect1")->setVisible(true);

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

    if (pGameBuyBullet->nChairID == m_ClientKernel->GetMeChairID())
    {
        winmgr.getWindow("Root/BulletBuy")->setVisible(false);
        winmgr.getWindow("Root/BulletBuy/Effect")->setVisible(false);
        winmgr.getWindow("Root/BulletBuy/Effect1")->setVisible(false);
    }

    m_spWarn[pGameBuyBullet->nChairID]->set_visible(false);
    

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


    for (int i=0; i<20; i++)
    {
        AddFish(pGameAddFish->Fish[i].nID, pGameAddFish->Fish[i].nStyle, 
                pGameAddFish->Fish[i].nPath, pGameAddFish->Fish[i].wFollowX, pGameAddFish->Fish[i].wFollowY);
    }

    return true;
}

bool CFishScene::OnSubGameSceneAddFish(const void * pBuffer, WORD wDataSize)
{
    if (m_ClientKernel==NULL) return true;

    if (wDataSize!=sizeof(CMD_S_SCENE_ADD_FISH)) return false;

    //消息处理
    CMD_S_SCENE_ADD_FISH * pGameAddFish=(CMD_S_SCENE_ADD_FISH *)pBuffer;

    int nID;
    int nStyle; 
    int nPath;  
    float fDelay=0.7f;
    Point start, end;

    for (int i=0; i<10; i++)
    {
        for (int j=0; j<5; j++)
        {
            nID = pGameAddFish->Fish[j+i*5].nID;
            nStyle = pGameAddFish->Fish[j+i*5].nStyle;
            nPath = pGameAddFish->Fish[j+i*5].nPath;
            start.set_point(1224, 180+nPath*50);
            end.set_point(-200, 180+nPath*50);
            
            if (nID != -1)
               AddSceneFish(nID, nStyle, start, end, fDelay);
            
        }

        fDelay +=  0.7f;
    }

    

    return true;
}

bool CFishScene::OnSubGameCapture(const void * pBuffer, WORD wDataSize)
{
    //效验数据
    //if (wDataSize!=sizeof(CMD_S_CAPTURE)) return false;

    //消息处理
    CMD_S_GROUP_CAPTURE *pCapture = (CMD_S_GROUP_CAPTURE *)pBuffer;

    for (int i=0; i<pCapture->cbCount; i++)
    {
        if (pCapture->Capture[i].bSuccess == false)
            continue;

        Sprite *Fish = (Sprite *)m_layFish->child_by_tag(pCapture->Capture[i].nFishID);
        if (Fish==0)
            continue;

        if (Fish->tag() == -1)
            continue;

        Fish->stop_all_action();

        Action_Finit_Time *act1 = new Action_Animation(0.05, Fish->animation_by_tag(1), false);
        Action_Finit_Time *act2 = new Action_Func(rak::mem_fn(this, &CFishScene::FishCaptureEnd));
        act2->set_tag(pCapture->Capture[i].nChairID);
        Action *action = new Action_Sequence(act1,act2, NULL);

        Fish->run_action(action);  
        Fish->set_tag(-1);
        ACTION_MANAGER()->resume_targer(Fish);

        Animation *ani = Fish->animation_by_tag(0);
        std::string szAni = ani->name();

        int nStyle;
        std::sscanf(szAni.c_str(), "Fish%d", &nStyle);
        char szSamply[32];
        sprintf(szSamply, "FishDying%d", nStyle);
        PlaySample(szSamply);

        if (nStyle > 6 )
        {
            char szHit[32];
            sprintf(szHit, "HitFish%d", nStyle);
            PlaySample(szHit);
        }

        m_nFishCount[nStyle] ++;
    }

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

        

        m_spTideWater->set_position(Point(1024+262/2, 576/2));
        Action *act = new Action_Sequence(new Action_Delay(3.0), new Action_Move_To(3, Point(-262/2, 576/2)), new Action_Func(rak::mem_fn(this, &CFishScene::TidewaterEnd)), NULL);
        m_spTideWater->run_action(act);
        ACTION_MANAGER()->resume_targer(m_spTideWater);

        Rect rcScreen(0,0,1024,570);
        Scene_Nodes::iterator j=m_layFish->child_begin();
        for (; j!=m_layFish->child_end(); ++j)
        {
            //if (!rcScreen.pt_in_rect((*j)->position()))
            //{
            //    continue;
            //}

            if ((*j)->tag()!=-1)
            {
                Action_Interval *actFish = (Action_Interval *)(*j)->action_by_tag(1);
                Action *actSpeed = new Action_Speed(actFish, 8.0);
                (*j)->run_action(actSpeed);
                ACTION_MANAGER()->resume_targer((*j));
            }
        }

        if (m_bSound != 0)
          m_TideWater->play(true, false);
    }

    return true;
}

bool CFishScene::OnSubGameAccount(const void * pBuffer, WORD wDataSize)
{
    //效验数据
    if (wDataSize!=sizeof(CMD_S_ACCOUNT)) return false;

    //消息处理
    CMD_S_ACCOUNT *pAccount = (CMD_S_ACCOUNT *)pBuffer;

    if (pAccount->nChairID == m_ClientKernel->GetMeChairID())
    {
        CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();

        if (m_RoleObject[pAccount->nChairID].nBulletCount <= m_RoleObject[pAccount->nChairID].nCannonStyle)
        winmgr.getWindow("Root/BulletBuy/Buy")->enable();
        winmgr.getWindow("Root/BulletBuy/Exit")->enable();
        winmgr.getWindow("Root/BulletBuy/Effect")->setVisible(false);
    }

    m_RoleObject[pAccount->nChairID].nBonus = 0;

    char szPrex[32];
    char szDetial[32];

    sprintf(szPrex, "Root/Role%d/", pAccount->nChairID);
    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();

    sprintf(szDetial, "%sBonusText", szPrex);
    winmgr.getWindow(szDetial)->setText("0");

    for (int i=0; i<11; i++)
    {
          m_nFishCount[i] = 0;
    }

    return true;
}

bool CFishScene::SendBuyBullet(int nBulletStyle)
{
    if (m_ClientKernel==NULL)
        return true;

    int nChairID = m_ClientKernel->GetMeChairID();
    if (m_RoleObject[nChairID].nBulletCount > m_RoleObject[nChairID].nCannonStyle)
        return true;

    CMD_C_BUY_BULLET BuyBullet;

    BuyBullet.nOldStyle = m_RoleObject[nChairID].nBulletStyle;
    BuyBullet.nOldCount = m_RoleObject[nChairID].nBulletCount;

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
    if (m_ClientKernel==NULL)
        return true;

    WORD wChairId = m_ClientKernel->GetMeChairID();
    if (wChairId == INVALID_CHAIR)
        return true;

    CMD_C_FIRE Fire;
    Fire.nStyle = m_RoleObject[wChairId].nCannonStyle;
    Fire.fRote = m_spCannon[wChairId]->rotation();
    return m_ClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_FIRE, &Fire, sizeof(CMD_C_FIRE));
}

bool CFishScene::SendCaptureFish(std::vector<int> &vecFish)
{    
    if (m_ClientKernel==NULL)
        return true;

    CMD_C_GROUP_CAPTURE gCapture;
    gCapture.cbCount = vecFish.size();

    for (int i=0; i<gCapture.cbCount; i++)
    {
        Sprite *fish = (Sprite *)m_layFish->child_by_tag(vecFish[i]);

        Animation *ani = fish->animation_by_tag(0);
        std::string szAni = ani->name();

        int nStyle;
        std::sscanf(szAni.c_str(), "Fish%d", &nStyle);

        gCapture.Capture[i].nFishID = vecFish[i];
        gCapture.Capture[i].nFishStyle = nStyle; 
    }


    return m_ClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_CAPTURE, &gCapture, sizeof(gCapture)-sizeof(gCapture.Capture)+gCapture.cbCount*sizeof(CMD_C_CAPTURE));
}

bool CFishScene::BulletEnd(Scene_Node *node, int tag)
{
    FireFishnet(node->tag(),node->position());

    m_NodeDelete.push_back(node);

    return true;
}

bool CFishScene::FishNetEnd(Scene_Node *node, int tag)
{
    if (m_ClientKernel==NULL)
        return true;

    Rect rcScreen(0,0,1024,570);
    WORD wChairID = m_ClientKernel->GetMeChairID();

    std::vector<int> vecFish;

    if (node->tag() == wChairID)
    {
        Scene_Nodes::iterator i=m_layFish->child_begin();
        for (; i!=m_layFish->child_end(); ++i)
        {
            if (!rcScreen.pt_in_rect((*i)->position()))
            {
                continue;
            }

            if ((*i)->tag()!=-1)
            {
                int nStyle;
                Animation *ani = ((Sprite*)(*i))->animation_by_tag(0);
                std::string szAni = ani->name();
                std::sscanf(szAni.c_str(), "Fish%d", &nStyle);

                if (nStyle == 10)
                {
                    if (m_GameLogic.CollisionShark((*i)->position(), node->position(), 0,m_GameLogic.GetFishNetScale(m_RoleObject[wChairID].nCannonStyle)*90))
                    {
                        vecFish.push_back((*i)->tag());
                        
                    }
                }
                else
                {
					if (m_GameLogic.Collision((*i)->position(), node->position(), 
                        m_GameLogic.GetFishRadius(nStyle),m_GameLogic.GetFishNetScale(m_RoleObject[wChairID].nCannonStyle)*90))
                    {

                        vecFish.push_back((*i)->tag());
                    }
                }

            }

        }
    }

    SendCaptureFish(vecFish);

    m_NodeDelete.push_back(node);

    return true;
}

bool CFishScene::FishCaptureEnd(Scene_Node *node, int tag)
{ 
    Animation *ani = ((Sprite*)node)->animation_by_tag(0);
    std::string szAni = ani->name();

    int nStyle;
    std::sscanf(szAni.c_str(), "Fish%d", &nStyle);

    m_RoleObject[tag].nBonus += m_GameLogic.GetFishBonus(nStyle);
   
    for (int i=0; i<nStyle+1; i++)
    {
        if (i>3)
            break;

        Point pt = node->position();
        pt.x_ += i*50;

        Sprite *spCoin = new Sprite(RESOURCE_MANAGER()->animation("Coin"));
        spCoin->set_tag(tag);
        spCoin->set_position(pt);
        spCoin->set_scalex(0.7);
        spCoin->set_scaley(0.7);

        m_layCoin->add_child(spCoin);

        Action *actionCoin1 = new Action_Sequence(new Action_Move_To(1, m_spCannon[tag]->position()),
                                                  new Action_Func(rak::mem_fn(this, &CFishScene::CoinEnd)), NULL);
        Action *actionCoin2 = new Action_Repeat_Forever(new Action_Animation(0.05, spCoin->animation_by_name("Coin"), false));
        Action *actionCoin3 = new Action_Scale_To(1, 1.5, 1.5);
        spCoin->run_action(actionCoin1);   
        spCoin->run_action(actionCoin2);
        spCoin->run_action(actionCoin3);
        ACTION_MANAGER()->resume_targer(spCoin);
    }

    int nValue = m_GameLogic.GetFishBonus(nStyle);
    int nBit1,nBit2,nBit3;
    char szValue[16];
    Sprite *spValue[4]={0};
    spValue[0] = new Sprite(RESOURCE_MANAGER()->sprite_frame("ValueCountX"));

    nBit1 = (nValue/100)%10;
    nBit2 = (nValue/10)%10;
    nBit3 = (nValue)%10;

    if (nBit1 == 0 && nBit2 == 0)
    {
        sprintf(szValue, "ValueCount%d", nBit3);
        spValue[1] = new Sprite(RESOURCE_MANAGER()->sprite_frame(szValue));
    }
    else if (nBit1 == 0 && nBit2 != 0 )
    {
        sprintf(szValue, "ValueCount%d", nBit2);
        spValue[1] = new Sprite(RESOURCE_MANAGER()->sprite_frame(szValue));
        sprintf(szValue, "ValueCount%d", nBit3);
        spValue[2] = new Sprite(RESOURCE_MANAGER()->sprite_frame(szValue));
    }
    else 
    {
        sprintf(szValue, "ValueCount%d", nBit1);
        spValue[1] = new Sprite(RESOURCE_MANAGER()->sprite_frame(szValue));
        sprintf(szValue, "ValueCount%d", nBit2);
        spValue[2] = new Sprite(RESOURCE_MANAGER()->sprite_frame(szValue));
        sprintf(szValue, "ValueCount%d", nBit3);
        spValue[3] = new Sprite(RESOURCE_MANAGER()->sprite_frame(szValue));
    }

    for (int i=0; i<4; i++)
    {
        if (spValue[i] != 0)
        {
            Point pt = node->position();
            pt.x_ += i*30-20;
            spValue[i]->set_position(pt);

            m_layCoin->add_child(spValue[i]);

            Action *act = new Action_Sequence(new Action_Fade_Out(1.5), new Action_Func(rak::mem_fn(this, &CFishScene::ValueEnd)), NULL);
            spValue[i]->run_action(act);   
            ACTION_MANAGER()->resume_targer(spValue[i]);
        }

    }


    PlaySample("CoinFly");

    m_NodeDelete.push_back(node);
    return true;
}

bool CFishScene::CoinEnd(Scene_Node *node, int tag)
{
    char szPrex[32];
    char szDetial[32];
    char szValue[32];

    sprintf(szPrex, "Root/Role%d/", node->tag());
    sprintf(szValue, "%d", m_RoleObject[node->tag()].nBonus);
    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();

    sprintf(szDetial, "%sBonusText", szPrex);
    winmgr.getWindow(szDetial)->setText(szValue);
  
    m_NodeDelete.push_back(node);

    return true;
}

bool CFishScene::ValueEnd(Scene_Node *node, int tag)
{
    m_NodeDelete.push_back(node);

    return true;
}

bool CFishScene::TidewaterEnd(Scene_Node *node, int tag)
{
    m_TideWater->stop();
    ChangeScene();

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
    sprite->set_position(Point(140,100));
    sprite->run_action(action);
    m_layBackground[1]->add_child(sprite);
    sprite->set_rotation(3.0);

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


    for (int i=0; i<4; i++)
    {
        frame = RESOURCE_MANAGER()->sprite_frame("Cannontag5");
        m_spCannontag[i] = new Sprite(frame);
    }

    m_spCannontag[0]->set_position(Point(310,38));
    m_spCannontag[1]->set_position(Point(710,38));
    m_spCannontag[2]->set_position(Point(710,548));
    m_spCannontag[3]->set_position(Point(310,538));

    for (int i=0; i<4; i++)
    {
        frame = RESOURCE_MANAGER()->sprite_frame("Cannontag0");
        m_spCannonnum[i] = new Sprite(frame);
    }

    m_spCannonnum[0]->set_position(Point(310,38));
    m_spCannonnum[1]->set_position(Point(710,38));
    m_spCannonnum[2]->set_position(Point(710,548));
    m_spCannonnum[3]->set_position(Point(310,538));

    frame = RESOURCE_MANAGER()->sprite_frame("Warn");
    for (int i=0; i<4; i++)
    {
        m_spWarn[i] = new Sprite(frame);
        m_spWarn[i]->set_visible(false);
        m_layRole->add_child(m_spWarn[i]);
    }

    m_spWarn[0]->set_position(Point(140,126));
    m_spWarn[1]->set_position(Point(900,126));
    m_spWarn[2]->set_position(Point(900,450));
    m_spWarn[3]->set_position(Point(140,450));

}

void CFishScene::CreateUI()
{
    m_ceguiRenderer =new CEGUI::DirectX9Renderer( Root::instance()->render_system()->device(), 0 ); 
    m_ceguiSystem =new CEGUI::System( m_ceguiRenderer );
    CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>   
        (CEGUI::System::getSingleton().getResourceProvider());    

    rp->setResourceGroupDirectory("schemes", "fish/datafiles/schemes/");    
    rp->setResourceGroupDirectory("imagesets", "fish/datafiles/imagesets/");    
    rp->setResourceGroupDirectory("fonts", "fish/datafiles/fonts/");    
    rp->setResourceGroupDirectory("layouts", "fish/datafiles/layouts/");    
    rp->setResourceGroupDirectory("looknfeels", "fish/datafiles/looknfeel/");    
    rp->setResourceGroupDirectory("lua_scripts", "fish/datafiles/lua_scripts/"); 

    CEGUI::Imageset::setDefaultResourceGroup("imagesets");    
    CEGUI::Font::setDefaultResourceGroup("fonts");    
    CEGUI::Scheme::setDefaultResourceGroup("schemes");    
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");    
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");    
    CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");  

    CEGUI::SchemeManager::getSingleton().loadScheme("WindowsLook.scheme");

    //if (!CEGUI::FontManager::getSingleton().isFontPresent("Commonwealth-10"))
    //    CEGUI::FontManager::getSingleton().createFont("Commonwealth-10.font");

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
    btnControlHelp->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CFishScene::HandleBtnControl, this));

    CEGUI::PushButton *btnControlBullet = static_cast<CEGUI::PushButton *>(winmgr.getWindow("Root/WindowMenu/BtnBullet"));
    btnControlBullet->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CFishScene::HandleBtnControl, this));

    CEGUI::PushButton *btnControlSetting = static_cast<CEGUI::PushButton *>(winmgr.getWindow("Root/WindowMenu/BtnSettting"));
    btnControlSetting->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CFishScene::HandleBtnControl, this));

    CEGUI::PushButton *btnControlChat = static_cast<CEGUI::PushButton *>(winmgr.getWindow("Root/WindowMenu/BtnChat"));
    btnControlChat->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CFishScene::HandleBtnControl, this));

    CEGUI::PushButton *btnControlExit = static_cast<CEGUI::PushButton *>(winmgr.getWindow("Root/WindowMenu/BtnAccountExitHover"));
    btnControlExit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CFishScene::HandleBtnControl, this));

    CEGUI::PushButton *btnContinueGame = static_cast<CEGUI::PushButton *>(winmgr.getWindow("Root/Balance/ContinueGame"));
    btnContinueGame->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CFishScene::HandleBtnContinueGame, this));

    CEGUI::PushButton *btnExitGame = static_cast<CEGUI::PushButton *>(winmgr.getWindow("Root/Balance/ExitGame"));
    btnExitGame->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CFishScene::HandleBtnExitGame, this));

    CEGUI::Window*  windHelp = winmgr.getWindow("Root/Help");
    windHelp->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&CFishScene::HandleCloseWin, this));

    CEGUI::Window*  winBulletBuy = winmgr.getWindow("Root/BulletBuy");
    winBulletBuy->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&CFishScene::HandleCloseWin, this));

    CEGUI::Window*  winSetting = winmgr.getWindow("Root/Setting");
    winSetting->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&CFishScene::HandleSettingCannel, this));

    CEGUI::Window*  winBalance= winmgr.getWindow("Root/Balance");
    winBalance->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&CFishScene::HandleCloseWin, this));

    CEGUI::Window*  winChat = winmgr.getWindow("Root/Chat");
    winChat->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&CFishScene::HandleCloseWin, this));

    CEGUI::RadioButton *rb100 = static_cast<CEGUI::RadioButton *>(winmgr.getWindow("Root/BulletBuy/Check100"));
    rb100->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&CFishScene::HandleCheckBullet, this));
    rb100->setSelected(true);

    CEGUI::RadioButton *rb600 = static_cast<CEGUI::RadioButton *>(winmgr.getWindow("Root/BulletBuy/Check600"));
    rb100->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&CFishScene::HandleCheckBullet, this));

    CEGUI::RadioButton *rb800 = static_cast<CEGUI::RadioButton *>(winmgr.getWindow("Root/BulletBuy/Check800"));
    rb800->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&CFishScene::HandleCheckBullet, this));

    winmgr.getWindow("Root/Help")->setVisible(false);

    CEGUI::Checkbox *checkboxSound = static_cast<CEGUI::Checkbox *>(winmgr.getWindow("Root/Setting/Tab/Tab 1/CheckSound"));
    CEGUI::Slider *sliderSound = static_cast<CEGUI::Slider *>(winmgr.getWindow("Root/Setting/Tab/Tab 1/SliderSound"));
    CEGUI::Checkbox *checkboxMusic = static_cast<CEGUI::Checkbox *>(winmgr.getWindow("Root/Setting/Tab/Tab 1/CheckMusic"));
    CEGUI::Slider *sliderMusic = static_cast<CEGUI::Slider *>(winmgr.getWindow("Root/Setting/Tab/Tab 1/SliderMusic"));
    CEGUI::PushButton *btnSettingOK = static_cast<CEGUI::PushButton *>(winmgr.getWindow("Root/Setting/Tab/Tab 1/BtnOk"));
    CEGUI::PushButton *btnSettingCannel = static_cast<CEGUI::PushButton *>(winmgr.getWindow("Root/Setting/Tab/Tab 1/BtnCannel"));
    btnSettingOK->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CFishScene::HandleSettingOk, this));
    btnSettingCannel->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CFishScene::HandleSettingCannel, this));

    checkboxSound->setSelected(m_bSound);
    sliderSound->setCurrentValue(m_fSound);
    checkboxMusic->setSelected(m_bMusic);
    sliderMusic->setCurrentValue(m_fMusic);


    //cursor
    //CEGUI::Image *pImage = 

    winmgr.getWindow("Root")->setMouseCursor(&CEGUI::ImagesetManager::getSingleton().getImageset("WindowsLook")->getImage("MouseMoveCursor"));
   

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

        if (m_bMusic != 0)
        {
            sound_scene_[0]->play(true,false);
            sound_scene_[1]->stop();
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
            /*ACTION_MANAGER()->resume_targer(*i);*/
        }

        add_child(m_layBackground[1],1);

        if (m_bMusic != 0)
        {
            sound_scene_[1]->play(true,false);
            sound_scene_[0]->stop();
        }
    }
}

void CFishScene::ChangeCannon(int nChairID, int nCannonStyle)
{
    Animation *ani;
    ani = m_spCannon[nChairID]->animation_by_tag(nCannonStyle);

    m_spCannon[nChairID]->set_display_frame(ani->at(0));

    char szCannontag[16];
    sprintf(szCannontag, "Cannontag%d", nCannonStyle);
    Sprite_Frame *frame = RESOURCE_MANAGER()->sprite_frame(szCannontag);

    m_spCannonnum[nChairID]->set_display_frame(frame);

    Action *act = new Action_Sequence(new Action_Scale_To(0.3, 1.8), new Action_Scale_To(0.3, 1.0), NULL);
    m_spCannonnum[nChairID]->run_action(act);
    ACTION_MANAGER()->resume_targer(m_spCannonnum[nChairID]);

    Action *act1 = new Action_Sequence(new Action_Scale_To(0.3, 1.8), new Action_Scale_To(0.3, 1.0), NULL);
    m_spCannontag[nChairID]->run_action(act1);
    ACTION_MANAGER()->resume_targer(m_spCannontag[nChairID]);

    PlaySample("SwitchSilo");
}

void CFishScene::RotateCannon(int nChairID, const Point &ptMouse)
{
    Point pt(ptMouse);
    float fRorate;

    if (nChairID<2)
    {
        pt = pt-m_spCannon[nChairID]->position();
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
        pt = m_spCannon[nChairID]->position() - pt;
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

void CFishScene::FireBullet(int nChairID,  int nStyle, float fRote)
{
    if (m_RoleObject[nChairID].nBulletCount <= m_RoleObject[nChairID].nCannonStyle)
    {
        return ;
    }

    Point pt = m_spCannon[nChairID]->position();
    float fRadius = m_GameLogic.GetBulletRadius(m_RoleObject[nChairID].nCannonStyle);
    Point ptStart (pt.x_+56*std::cos(fRote-M_PI_2), pt.y_+56*std::sin(fRote-M_PI_2));
    Point ptEnd (pt.x_+fRadius*std::cos(fRote-M_PI_2), pt.y_+fRadius*std::sin(fRote-M_PI_2));

    Sprite *spBullet = new Sprite(RESOURCE_MANAGER()->sprite_frame("Bullet"));
    spBullet->set_tag(nChairID);
    spBullet->set_position(ptStart);
    spBullet->set_rotation(fRote-M_PI_2);
    m_layBullet->add_child(spBullet);

    Action *act = new Action_Sequence(new Action_Move_To(fRadius/200.0-m_RoleObject[nChairID].nBulletStyle*0.25, ptEnd), 
                                      new Action_Func(rak::mem_fn(this, &CFishScene::BulletEnd)), NULL); 
    spBullet->run_action(act);
    ACTION_MANAGER()->resume_targer(spBullet);

    m_RoleObject[nChairID].nBulletCount-=nStyle+1;


    Animation *aniCannon = m_spCannon[nChairID]->animation_by_tag(m_RoleObject[nChairID].nCannonStyle);
    Action *actCannon = new Action_Animation(0.06,aniCannon,false);
    m_spCannon[nChairID]->run_action(actCannon);
    ACTION_MANAGER()->resume_targer(m_spCannon[nChairID]);

    Animation *aniBoat = m_spBoat[nChairID]->animation_by_name("Boat");
    Action *actBoat = new Action_Animation(0.06,aniBoat,false);
    m_spBoat[nChairID]->run_action(actBoat);
    ACTION_MANAGER()->resume_targer(m_spBoat[nChairID]);

    char szPrex[32];
    char szDetial[32];
    char szBulletCount[32];

    sprintf(szPrex, "Root/Role%d/", nChairID);
    sprintf(szBulletCount, "%d", m_RoleObject[nChairID].nBulletCount);

    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();

    sprintf(szDetial, "%sBulletText", szPrex);
    winmgr.getWindow(szDetial)->setText(szBulletCount);

    char szStyle[32];
    sprintf(szStyle, "Silo%d", nStyle);
    PlaySample(szStyle);

    if (m_RoleObject[nChairID].nBulletCount < 20)
    {
        m_spWarn[nChairID]->set_visible(true);
    }
    else
    {
        m_spWarn[nChairID]->set_visible(false);
    }
}

void CFishScene::FireFishnet(int nChairID, Point ptPosition)
{
    Animation *animation = RESOURCE_MANAGER()->animation("Fishnet");
    Sprite *spFishNet = new Sprite(animation);
    spFishNet->set_tag(nChairID);
    spFishNet->set_position(ptPosition);
    spFishNet->set_scalex(m_GameLogic.GetFishNetScale(m_RoleObject[nChairID].nCannonStyle));
    spFishNet->set_scaley(m_GameLogic.GetFishNetScale(m_RoleObject[nChairID].nCannonStyle));
    spFishNet->set_color(m_GameLogic.GetFishNetColor(m_RoleObject[nChairID].nCannonStyle));
    m_layFishNet->add_child(spFishNet);

    Action *action = new Action_Sequence(new Action_Animation(0.05, spFishNet->animation_by_name("Fishnet"), false), 
                                         new Action_Func(rak::mem_fn(this, &CFishScene::FishNetEnd)), NULL);
    spFishNet->run_action(action);
    ACTION_MANAGER()->resume_targer(spFishNet);

    PlaySample("NetOpen");
}

void CFishScene::AddFish(int nID, int nStyle, int nPath, int nFollowX, int nFollowY)
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

    Action_Finit_Time *actPath[14]={0};
    int nActCount = 0;
    Point Bezier[3];

    for (int i=1; i<path->size(); i++)
    {
        int nEnd = (i-1)%3;      
        Bezier[nEnd] = path->at(i);

        if (nFollowX>0)
        {
            Bezier[nEnd].x_ += nFollowX;
            Bezier[nEnd].y_ += nFollowY;
        }

        if (nEnd == 2)
        {
            actPath[nActCount] = new Action_Bezier_By(3, Bezier[0],Bezier[1], Bezier[2]);
            nActCount++;

            if (nActCount>12)
            {
                break;
            }
        }
    }

    actPath[nActCount] = new Action_Func(rak::mem_fn(this, &CFishScene::FishEnd));

    Action *actPaths = new Action_Sequence(actPath[0], actPath[1], actPath[2], actPath[3], actPath[4], 
        actPath[5], actPath[6], actPath[7], actPath[8], actPath[9],
        actPath[10], actPath[11], actPath[12], actPath[13], NULL);
    actPaths->set_tag(1);
    spFish->run_action(actPaths);
    ACTION_MANAGER()->resume_targer(spFish);

}

void CFishScene::AddSceneFish(int nID, int nStyle, const Point &start, const Point &end, float fDelay)
{
    char szAnimation[20];
    char szAnimationCapture[20];
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
    spFish->set_position(start);
    spFish->set_rotation(M_PI);
    m_layFish->add_child(spFish);

    Action *actPaths = new Action_Sequence(new Action_Delay(fDelay), new Action_Move_To(12.0, end), new Action_Func(rak::mem_fn(this, &CFishScene::FishEnd)), NULL);
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

    if (m_RoleObject[nChairID].nBulletCount > m_RoleObject[nChairID].nCannonStyle)
        return true;

    int nBulletStyle = -1;

    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();

    CEGUI::RadioButton *rb100 = static_cast<CEGUI::RadioButton *>(winmgr.getWindow("Root/BulletBuy/Check100"));
    CEGUI::RadioButton *rb600 = static_cast<CEGUI::RadioButton *>(winmgr.getWindow("Root/BulletBuy/Check600"));
    CEGUI::RadioButton *rb800 = static_cast<CEGUI::RadioButton *>(winmgr.getWindow("Root/BulletBuy/Check800"));
   

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
    winmgr.getWindow("Root/BulletBuy/Effect")->setVisible(true);

    return true;
}

bool CFishScene::HandleBtnBulletBuyExit(const CEGUI::EventArgs& args) // 结算消息
{
    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();
    winmgr.getWindow("Root/BulletBuy/Buy")->disable();
    winmgr.getWindow("Root/BulletBuy/Exit")->disable();
    winmgr.getWindow("Root/BulletBuy/Effect")->setVisible(true);
    winmgr.getWindow("Root/BulletBuy/Effect1")->setVisible(false);

    CMD_C_ACCOUNT Account;
    Account.bSuccess = true;

    return m_ClientKernel->SendSocketData(MDM_GF_GAME, SUB_C_ACCOUNT, &Account, sizeof(CMD_C_ACCOUNT));
}

bool CFishScene::HandleBtnControl(const CEGUI::EventArgs& args)
{
    CEGUI::Window *win = static_cast<CEGUI::Window *> (
        static_cast<const CEGUI::WindowEventArgs&> (args).window);

    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();
    CEGUI::String szWin = win->getName();
    if (szWin == "Root/WindowMenu/BtnHelp")
    {
        CEGUI::Window *wins = winmgr.getWindow("Root/Help");
        wins->setVisible(true);
    }
    else if (szWin == "Root/WindowMenu/BtnSettting")
    {
        CEGUI::Window *wins = winmgr.getWindow("Root/Setting");
        wins->setVisible(true);
    }
    else if (szWin == "Root/WindowMenu/BtnChat")
    {
        //CEGUI::Window *wins = winmgr.getWindow("Root/Chat");
        //wins->setVisible(true);
    }
    else if (szWin == "Root/WindowMenu/BtnBullet")
    {
        CEGUI::Window *wins = winmgr.getWindow("Root/BulletBuy");
        wins->setVisible(true);
    }
    else if (szWin == "Root/WindowMenu/BtnAccountExitHover")
    {
        int nChairID = m_ClientKernel->GetMeChairID();
        if (INVALID_CHAIR == nChairID)
            return true;

        CEGUI::Window *wins = winmgr.getWindow("Root/Balance");
        wins->setVisible(true);

        char szFish[32];
        char szValue[32];

        for (int i=0; i<11; i++)
        {
            sprintf(szFish, "Root/Balance/Fish%d", i);
            sprintf(szValue, "X %d", m_nFishCount[i]);
            winmgr.getWindow(szFish)->setText(szValue);
        }

        sprintf(szValue, "%d", m_RoleObject[nChairID].nBulletCount*(m_RoleObject[nChairID].nBulletStyle+1));
        winmgr.getWindow("Root/Balance/BulletValue")->setText(szValue);

        sprintf(szValue, "%d", m_RoleObject[nChairID].nBonus);
        winmgr.getWindow("Root/Balance/FishValue")->setText(szValue);

        sprintf(szValue, "%d", m_RoleObject[nChairID].nBulletCount*(m_RoleObject[nChairID].nBulletStyle+1)+m_RoleObject[nChairID].nBonus);
        winmgr.getWindow("Root/Balance/TolteValue")->setText(szValue);


    }

    return true;
}

bool CFishScene::HandleCloseWin(const CEGUI::EventArgs& args)
{
    CEGUI::Window *win = static_cast<CEGUI::Window *> (
            static_cast<const CEGUI::WindowEventArgs&> (args).window);

    win->setVisible(false);

    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();
    CEGUI::String szWin = win->getName();
    if (szWin == "Root/BulletBuy")
    {
         winmgr.getWindow("Root/BulletBuy/Effect1")->setVisible(false);
    }
   

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

bool CFishScene::HandleSettingOk(const CEGUI::EventArgs& args)
{
    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();

    CEGUI::Window *wins = winmgr.getWindow("Root/Setting");
    wins->setVisible(false);

    CEGUI::Checkbox *checkboxSound = static_cast<CEGUI::Checkbox *>(winmgr.getWindow("Root/Setting/Tab/Tab 1/CheckSound"));
    CEGUI::Slider *sliderSound = static_cast<CEGUI::Slider *>(winmgr.getWindow("Root/Setting/Tab/Tab 1/SliderSound"));
    CEGUI::Checkbox *checkboxMusic = static_cast<CEGUI::Checkbox *>(winmgr.getWindow("Root/Setting/Tab/Tab 1/CheckMusic"));
    CEGUI::Slider *sliderMusic = static_cast<CEGUI::Slider *>(winmgr.getWindow("Root/Setting/Tab/Tab 1/SliderMusic"));
  
    m_bSound = checkboxSound->isSelected();
    m_fSound = sliderSound->getCurrentValue();
    m_bMusic = checkboxMusic->isSelected();
    m_fMusic = sliderMusic->getCurrentValue();

    SOUND_MANAGER()->set_volume(m_fSound);
    sound_scene_[0]->set_volume(m_fMusic);
    sound_scene_[1]->set_volume(m_fMusic);

    if (m_bMusic == 1)
    {
        sound_scene_[m_nScene]->play(true,false);
    }
    else
    {
        sound_scene_[m_nScene]->stop();
    }

    return true;
}

bool CFishScene::HandleSettingCannel(const CEGUI::EventArgs& args)
{
    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();

    CEGUI::Window *wins = winmgr.getWindow("Root/Setting");
    wins->setVisible(false);

    CEGUI::Checkbox *checkboxSound = static_cast<CEGUI::Checkbox *>(winmgr.getWindow("Root/Setting/Tab/Tab 1/CheckSound"));
    CEGUI::Slider *sliderSound = static_cast<CEGUI::Slider *>(winmgr.getWindow("Root/Setting/Tab/Tab 1/SliderSound"));
    CEGUI::Checkbox *checkboxMusic = static_cast<CEGUI::Checkbox *>(winmgr.getWindow("Root/Setting/Tab/Tab 1/CheckMusic"));
    CEGUI::Slider *sliderMusic = static_cast<CEGUI::Slider *>(winmgr.getWindow("Root/Setting/Tab/Tab 1/SliderMusic"));
  
    checkboxSound->setSelected(m_bSound);
    sliderSound->setCurrentValue(m_fSound);
    checkboxMusic->setSelected(m_bMusic);
    sliderMusic->setCurrentValue(m_fMusic);

    return true;
}

bool CFishScene::HandleBtnContinueGame(const CEGUI::EventArgs& args)
{
    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();
    CEGUI::Window *wins = winmgr.getWindow("Root/Balance");
    wins->setVisible(false);
    return true;
}

bool CFishScene::HandleBtnExitGame(const CEGUI::EventArgs& args)
{
    PostMessage(RENDER_WINDOW()->window(), WM_CLOSE, 0, 0);

    return true;
}

bool CFishScene::InputFire()
{
    if (m_ClientKernel==NULL)
        return true;

    int nChairID = m_ClientKernel->GetMeChairID();
    if (INVALID_CHAIR == nChairID)
        return true;

    unsigned long time = m_Timer.get_milli_seconds();
    if (time < m_GameLogic.GetFireInterval(m_RoleObject[nChairID].nBulletStyle))
        return true;
    m_Timer.reset();

    CEGUI::WindowManager& winmgr = CEGUI::WindowManager::getSingleton ();

    if (!winmgr.getWindow("Root/BulletBuy")->isVisible())
    {   

        if (m_RoleObject[nChairID].nBulletCount <= m_RoleObject[nChairID].nCannonStyle)
        {
            PlaySample("NoBullet");
            winmgr.getWindow("Root/BulletBuy/Buy")->enable();
            winmgr.getWindow("Root/BulletBuy/Effect")->setVisible(false);
            winmgr.getWindow("Root/BulletBuy")->setVisible(true);
            winmgr.getWindow("Root/BulletBuy/Effect1")->setVisible(false);
        }
        else if (m_RoleObject[nChairID].nBulletCount > m_RoleObject[nChairID].nCannonStyle)
        {
            FireBullet(nChairID, m_RoleObject[nChairID].nCannonStyle, m_spCannon[nChairID]->rotation());
            SendFireBullet();
        }
    }
}

void CFishScene::PlaySample(const std::string &id)
{
    if (m_bSound == 0)
        return;

    int sound_id = RESOURCE_MANAGER()->sound(id);

    Sound_Instance* sound_instance = SOUND_MANAGER()->sound_instance(sound_id);
    sound_instance->play(false, true);
}


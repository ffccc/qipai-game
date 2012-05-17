#include "AccountLayer.h"
#include "ClientKernel.h"
#include "FishApplication.h"
#include "GameScene.h"
#include "RoleLayer.h"
#include "BuyBulletLayer.h"

#include <sstream>
////////////////////////////////////////////////////////////
CAccountLayer::CAccountLayer()
:m_CaptureFishs(12)
{	
    m_Font = Root::instance()->font_manager()->create_font_ttf("simsun", "fish\\simsun.ttc");
    m_Font->set_size(Size(14,15));

    m_btnAccountAndExit = new Button_Widget(Root::instance()->imageset_manager()->imageset("account")->image("btn_account_and_exit_image"),
        Root::instance()->imageset_manager()->imageset("account")->image("btn_account_and_exit_over"),
        Root::instance()->imageset_manager()->imageset("account")->image("btn_account_and_exit_down"),
        Root::instance()->imageset_manager()->imageset("account")->image("btn_account_and_exit_disable"));
    m_btnAccountAndExit->set_tag(10200);
    m_btnAccountAndExit->set_position(Point(514,560));
    m_btnAccountAndExit->set_content_size(Size(158,57));
    m_btnAccountAndExit->add_listener(this);

    m_btnAccount = new Button_Widget(Root::instance()->imageset_manager()->imageset("account")->image("btn_account_image"),
        Root::instance()->imageset_manager()->imageset("account")->image("btn_account_over"),
        Root::instance()->imageset_manager()->imageset("account")->image("btn_account_down"),
        Root::instance()->imageset_manager()->imageset("account")->image("btn_account_disable"));
    m_btnAccount->set_tag(10201);
    m_btnAccount->set_position(Point(514,560));
    m_btnAccount->set_content_size(Size(158,57));
    m_btnAccount->add_listener(this);

    m_btnClose = new Button_Widget(Root::instance()->imageset_manager()->imageset("account")->image("btn_close_image"),
        Root::instance()->imageset_manager()->imageset("account")->image("btn_close_over"),
        Root::instance()->imageset_manager()->imageset("account")->image("btn_close_down"),
        Root::instance()->imageset_manager()->imageset("account")->image("btn_close_disable"));
    m_btnClose->set_tag(10202);
    m_btnClose->set_position(Point(514,560));
    m_btnClose->set_content_size(Size(158,57));
    m_btnClose->add_listener(this);

 
    m_sprBackground = new Sprite(Root::instance()->imageset_manager()->imageset("account")->image("bg"));
    m_sprBackground->set_hot(Point(0,0));
    add_child(m_sprBackground);

    for (int i=0; i<m_CaptureFishs.size(); i++)
    {
        m_CaptureFishs[i] = 0;
    }

    std::ostringstream ostr;
    for (int i=0; i<10; i++)
    {
        ostr.str("");
        ostr << "number_" << i ;
        m_imgNumber[i] = Root::instance()->imageset_manager()->imageset("role")->image(ostr.str());
    }

    for (int i=0; i<10; i++)
    {
        ostr.str("");
        ostr << "time_" << i ;
        m_imgTimer[i] = Root::instance()->imageset_manager()->imageset("account")->image(ostr.str());
    }

    
}

CAccountLayer::~CAccountLayer()
{
}

void CAccountLayer::update(float dt)
{
    __super::update(dt);

}

void CAccountLayer::draw()
{
    if (!visible_)
    {
        return;
    }

    __super::draw();

    CGameScene *pGameScene = (CGameScene *)parent();
    CClientKernel *pClientKernel = pGameScene->GetClientKernel();
    if ( !pClientKernel)
        return ;

    Point pt(position_absolute());
    Point ptDraw;
    std::ostringstream ostr;

    for (int i=0; i<4; i++)
    {
        ostr.str("");
        ostr << m_CaptureFishs[i];

        ptDraw.x_ = pt.x_ + 136+i*180;
        ptDraw.y_ = pt.y_ +104;

        DrawNumber(ostr.str(), ptDraw);
    }

    for (int i=0; i<4; i++)
    {
        ostr.str("");
        ostr << m_CaptureFishs[i+4];

        ptDraw.x_ = pt.x_ + 136+i*180;
        ptDraw.y_ = pt.y_ +104+74;

        DrawNumber(ostr.str(), ptDraw);
    }

    for (int i=0; i<4; i++)
    {
        ostr.str("");
        ostr << m_CaptureFishs[i+8];

        ptDraw.x_ = pt.x_ + 136+i*180;
        ptDraw.y_ = pt.y_ +104+148;

        DrawNumber(ostr.str(), ptDraw);
    }


     WORD wMeChairID = pGameScene->GetMeChairID();

     if (wMeChairID != INVALID_CHAIR)
     {
         int nFishGold = pGameScene->m_layRoles[wMeChairID]->GetFishGold();
         ostr.str("");
         ostr << nFishGold; 
         ptDraw.x_ = pt.x_ + 296;
         ptDraw.y_ = pt.y_ + 310;
         DrawNumber(ostr.str(), ptDraw);

         LONG lCellScore = pGameScene->m_layBuyBulletLayer->GetCellScore();

         ostr.str("");
         ostr << nFishGold*lCellScore; 
         ptDraw.x_ = pt.x_ + 500;
         ptDraw.y_ = pt.y_ + 310;
         DrawNumber(ostr.str(), ptDraw);



         const tagUserData *pUserData = pClientKernel->GetMeUserInfo();

         if (pUserData)
         {
             ostr.str("");
             ostr << 0; 
             ptDraw.x_ = pt.x_ + 240;
             ptDraw.y_ = pt.y_ + 352;
             DrawNumber(ostr.str(), ptDraw);

             ostr.str("");
             ostr << pUserData->lScore; 
             ptDraw.x_ = pt.x_ + 580;
             ptDraw.y_ = pt.y_ + 352;
             DrawNumber(ostr.str(), ptDraw);
         }



     }

     DWORD dwTime = (DWORD)time(NULL) - m_dwStartTime;

     if (dwTime >= 20)
     {
         ShowWidnow(false);
     }
     else
     {
         ostr.str("");
         ostr << 20-dwTime; 

         ptDraw.x_ = pt.x_ + 696;
         ptDraw.y_ = pt.y_ + 38;

         DrawTimer(ostr.str(), ptDraw);
     }
}

void CAccountLayer::add_widget(Node *node)
{
    node->add_child(this);
    node->add_child(m_btnAccountAndExit);
    node->add_child(m_btnAccount);
    node->add_child(m_btnClose);
}

void CAccountLayer::DrawNumber(std::string &number, const Point &pt)
{
    std::ostringstream ostr;
    Point ptDraw;
    ptDraw = pt - Point(number.size()/2.0*24.0, 0);

    for (int i=0; i<number.size(); i++)
    {
        ostr.str("");
        ostr << number[i];
        int nIndex = std::atoi(ostr.str().c_str());
        m_imgNumber[nIndex]->draw(ptDraw);

        ptDraw.x_ += 24;
    }

}

void CAccountLayer::DrawTimer(std::string &number, const Point &pt)
{
    std::ostringstream ostr;
    Point ptDraw;
    ptDraw = pt - Point(number.size()/2.0*24.0, 0);

    for (int i=0; i<number.size(); i++)
    {
        ostr.str("");
        ostr << number[i];
        int nIndex = std::atoi(ostr.str().c_str());
        m_imgTimer[nIndex]->draw(ptDraw);

        ptDraw.x_ += 24;
    }

}


void CAccountLayer::ShowWidnow(bool bShow)
{                          
    set_visible(bShow);    
    m_btnAccountAndExit->set_visible(bShow);
    m_btnAccount->set_visible(bShow);
    m_btnClose->set_visible(bShow);

    if (bShow == true)
    {
        m_dwStartTime = (DWORD)time(NULL);
    }
}     

void CAccountLayer::DisableWindow(bool bDisable)
{
    set_disable(bDisable);    
    m_btnAccountAndExit->set_disable(bDisable);
    m_btnAccount->set_disable(bDisable);
    m_btnClose->set_disable(bDisable);
}
                            
void CAccountLayer::button_press(int tag)
{      
    CGameScene *pGameScene = (CGameScene *)parent();

    if (tag == 10200)     
    {                      
          pGameScene->window_closed(0);
           Root::instance()->queue_end_rendering();		
    }
    else if (tag == 10201)
    {
         CMD_C_Account Account;
         Account.wChairID = pGameScene->GetMeChairID();

         pGameScene->GetClientKernel()->SendSocketData(MDM_GF_GAME, SUB_C_ACCOUNT, &Account, sizeof(CMD_C_Account));

         DisableWindow(true);
    }
    else if (tag == 10202)
    {
        ShowWidnow(false);
    }
}

void CAccountLayer::resize(const Point &pt, const Size &size)
{
    __super::resize(pt, size);

    set_position(pt);
    m_btnAccountAndExit->set_position(Point(pt.x_+80, pt.y_+390));
    m_btnAccount->set_position(Point(pt.x_+310, pt.y_+390));
    m_btnClose->set_position(Point(pt.x_+540, pt.y_+390));
}

////////////////////////////////////////////////////////////
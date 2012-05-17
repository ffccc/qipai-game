#ifndef __FISH_COIN_LAYER_H__
#define __FISH_COIN_LAYER_H__

//////////////////////////////////////////////////////////////////
#include "xlbe.h"

#include "..\消息定义\CMD_Fish.h"
#include "GameCore.h"

#include "widget/button_widget.h"
#include "widget/slider_widget.h"

/////////////////////////////////////////////////////////////////
class CCoinLayer : public Layer
{
public:
    CCoinLayer();
    virtual ~CCoinLayer ();

    virtual void update(float dt);
    virtual void draw();

    bool CoinSilverEnd(Node *node, int tag);
    bool CoinGoldEnd(Node *node, int tag);
    bool CoinValueEnd(Node *node, int tag);

private:
   
};

//////////////////////////////////////////////////////////////////////////
#endif
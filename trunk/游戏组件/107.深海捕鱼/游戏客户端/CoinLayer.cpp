#include "CoinLayer.h"
#include "ClientKernel.h"
#include "FishApplication.h"
#include "GameScene.h"

#include <sstream>
////////////////////////////////////////////////////////////
CCoinLayer::CCoinLayer()
{	

}

CCoinLayer::~CCoinLayer()
{
}

void CCoinLayer::update(float dt)
{
    __super::update(dt);

}

void CCoinLayer::draw()
{
    __super::draw();
}

bool CCoinLayer::CoinSilverEnd(Node *node, int tag)
{
    remove_child(node);
    delete node;

    return true;
}

bool CCoinLayer::CoinGoldEnd(Node *node, int tag)
{
    remove_child(node);
    delete node;

    return true;
}

bool CCoinLayer::CoinValueEnd(Node *node, int tag)
{

    remove_child(node);
    delete node;

    return true;
}
////////////////////////////////////////////////////////////
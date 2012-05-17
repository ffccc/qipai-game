#ifndef __FISH_BACKGROUND_LAYER_H__
#define __FISH_BACKGROUND_LAYER_H__

//////////////////////////////////////////////////////////////////
#include "xlbe.h"

#include "GameCore.h"

/////////////////////////////////////////////////////////////////
class CBackgroundLayer : public Layer
{
public:
    CBackgroundLayer();
    virtual ~CBackgroundLayer ();

public:
    virtual void update(float dt);
    virtual void draw();

    CGameCore::enSceneType GetSceneType() const { return m_SceneType; }
    void SetSceneType(CGameCore::enSceneType SceneType);

    void ChangeSceneType(CGameCore::enSceneType SceneType);

    bool ChangeSceneEnd(Node *node, int tag);
    
private:
    CGameCore::enSceneType m_SceneType;

    Sprite *m_sprTideWater;

    Image *m_imgBackground0;
    Image *m_imgBackground1;

    Sound_Instance*	m_BackSound[4];
};

//////////////////////////////////////////////////////////////////////////
#endif
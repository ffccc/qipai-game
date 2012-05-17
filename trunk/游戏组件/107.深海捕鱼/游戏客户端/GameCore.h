#ifndef __FISH_GAME_CORE_H__
#define __FISH_GAME_CORE_H__

#include "xlbe.h"

/////////////////////////////////////////////////////////////////////////

class CGameCore
{
public:
    enum enSceneType
    {
        SceneType_0 = 0,
        SceneType_1,
        SceneType_2,
        SceneType_3,
        SceneTypeCount
    };

    enum enFishType
    {
        FishType_0 = 0,
        FishType_1,
        FishType_2,
        FishType_3,
        FishType_4,
        FishType_5,
        FishType_6,
        FishType_7,
        FishType_8,
        FishType_9,
        FishType_10,
        FishType_11,
        FishType_12,
        FishTypeCount
    };

    enum enCannonType
    {
        CannonType_0 = 0,
        CannonType_1,
        CannonType_2,
        CannonType_3,
        CannonType_4,
        CannonType_5,
        CannonType_6,
        CannonTypeCount          
    };

public:
    static enSceneType ByteToSceneType(BYTE cbScene)
    {
        switch(cbScene)
        {
        case 0: return SceneType_0;
        case 1: return SceneType_1;
        case 2: return SceneType_2;
        case 3: return SceneType_3;
        default: return SceneTypeCount;
        }
    }

    static enFishType WordToFishType(WORD wFishType)
    {
        switch(wFishType)
        {
        case 0: return FishType_0;
        case 1: return FishType_1;
        case 2: return FishType_2;
        case 3: return FishType_3;
        case 4: return FishType_4;
        case 5: return FishType_5;
        case 6: return FishType_6;
        case 7: return FishType_7;
        case 8: return FishType_8;
        case 9: return FishType_9;
        case 10: return FishType_10;
        case 11: return FishType_11;
        case 12: return FishType_12;
        default: return FishTypeCount;
        }
    }

    static enCannonType WordToCannonType(WORD wCannonType)
    {
        switch(wCannonType)
        {
        case 0: return CannonType_0;
        case 1: return CannonType_1;
        case 2: return CannonType_2;
        case 3: return CannonType_3;
        case 4: return CannonType_4;
        case 5: return CannonType_5;
        case 6: return CannonType_6;

        default: return CannonTypeCount;
        }
    }

    static int GetFishPathByFishType(enFishType FishType)
    {
        switch(FishType)
        {
        case FishType_0: return 1;
        case FishType_1: return 0;
        case FishType_2: return 0;
        case FishType_3: return 0;
        case FishType_4: return 0;
        case FishType_5: return 0;
        case FishType_6: return 0;
        case FishType_7: return 0;
        case FishType_8: return 0;
        case FishType_9: return 0;
        case FishType_10: return 0;
        case FishType_11: return 0;
        case FishType_12: return 0;
        default:  return 0;
        }
    }
   
};

/////////////////////////////////////////////////////////////////////////
#endif
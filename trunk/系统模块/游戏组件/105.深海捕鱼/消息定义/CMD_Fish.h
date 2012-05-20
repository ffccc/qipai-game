#ifndef CMD_FISH_HEAD_FILE
#define CMD_FISH_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						2009									      //游戏 I D
#define GAME_PLAYER					4									        //游戏人数
#define GAME_NAME					TEXT("深海捕鱼")						//游戏名字

#define IDI_ADD_FISH           1
#define IDI_CHANGE_SCENE       2
#define TIME_ADD_FISH          6000

#define SUB_S_BUY_BULLET        100
#define SUB_S_FIRE              101
#define SUB_S_CAPTURE           102
#define SUB_S_ADD_FISH          103
#define SUB_S_CHANGE_CANNON     104
#define SUB_S_CHANGE_SCENE      105

#define SUB_C_BUY_BULLET        120
#define SUB_C_FIRE              121
#define SUB_C_CAPTURE           122
#define SUB_C_CHANGE_CANNON     123

//////////////////////////////////////////////////////////////////////////
struct FishNetObject
{
    int nID;
    int nStyle; // 0 1 ... 11
    int nPath;  // 0 - 10 
};

struct RoleNetObject
{
    int nID;
    int nBonus;
    int nCannonStyle;  // 0 1 2 3 4 5 
    int nBulletStyle;  // 0 1 2
    int nBulletCount;
};

struct CMD_S_StatusFree
{
    int nScene;
    RoleNetObject Role[GAME_PLAYER];
};

struct CMD_C_BUY_BULLET
{
   int nStyle; 
   int nCount;
};

struct CMD_S_BUY_BULLET
{
   bool bSuccess;
   int nChairID;
   int nStyle; 
   int nCount;
};

struct CMD_C_FIRE
{
    float fRote;
};

struct CMD_S_FIRE
{
    int nChairID;
    float fRote;
};

struct CMD_S_ADD_FISH
{
   FishNetObject Fish[10];
};

struct CMD_C_CHANGE_CANNON
{
    int nStyle;
};

struct CMD_S_CHANGE_CANNON
{
    int nChairID;
    int nStyle;
};

struct CMD_C_CAPTURE
{
    int nFishID;
    int nFishStyle;
};

struct CMD_S_CAPTURE
{
    bool bSuccess;
    int nChairID;
    int nBonus;
    int nFishID;
};

struct CMD_S_CHANGE_SCENE
{
    int nScene;
};

//////////////////////////////////////////////////////////////////////////

#endif
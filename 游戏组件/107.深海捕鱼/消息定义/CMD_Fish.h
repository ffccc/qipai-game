#ifndef CMD_FISH_HEAD_FILE
#define CMD_FISH_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						520									    //游戏 I D
#define GAME_PLAYER					4									    //游戏人数
#define GAME_NAME					TEXT("深海捕鱼")						//游戏名字

#define GAME_PROBABILITY			800//950										// 几率系数
#define BULLET_MAX_COUNT			5										// 购买子弹最大数目

//////////////////////////////////////////////////////////////////////////
#define SUB_S_CHANGE_SCENE            100
#define SUB_S_BUY_BULLET_SUCCESS      101
#define SUB_S_BUY_BULLET_FAILED       102
#define SUB_S_SEND_LINE_PATH_FISH     103
#define SUB_S_SEND_POINT_PATH_FISH    104
#define SUB_S_FIRE_SUCCESS            105
#define SUB_S_FIRE_FAILED             106
#define SUB_S_CAST_NET_SUCCESS        107
#define SUB_S_CAST_NET_FAILED         108
#define SUB_S_CHANGE_CANNON           109
#define SUB_S_ACCOUNT                 110
#define SUB_S_SEND_GROUP_POINT_PATH_FISH    111
#define SUB_S_SEND_SPECIAL_POINT_PATH  112
#define SUB_S_TASK                     114
#define SUB_S_LASER_BEAN_SUCCESS                115
#define SUB_S_BOMB_SUCCESS                     116
#define SUB_S_LASER_BEAN_FAILED                117
#define SUB_S_BOMB_SUCCESS_FAILED      118
#define SUB_S_BONUS                    119
#define SUB_S_COOK                    120

#define SUB_C_BUY_BULLET              200
#define SUB_C_FIRE                    201
#define SUB_C_CAST_NET                202
#define SUB_C_CHANGE_CANNON           203
#define SUB_C_ACCOUNT                 204
#define SUB_C_LASER_BEAN              205
#define SUB_C_BOMB                    206
#define SUB_C_BONUS                   207
#define SUB_C_COOK                    208
#define SUB_C_TASK_PREPARED           209

//time define 
//////////////////////////////////////////////////////////////////////////
#define IDI_SCENE_START        1
#define IDI_SCENE_END          2

#define IDI_NORMAL_ADD_FISH_START    3
#define IDI_NORMAL_ADD_FISH_END      4

#define IDI_SCENE_ADD_FISH_START     5
#define IDI_SCENE_ADD_FISH_END       6

#define IDI_CHECK_FISH_DESTORY_START  7
#define IDI_CHECK_FISH_DESTORY_END    8

#define TIME_SCENE_START       1000
#define TIME_SCENE_END         1000

#define TIME_NORMAL_ADD_FISH_START   1000
//#define TIME_NORMAL_ADD_FISH_END     500

#define TIME_SCENE_ADD_FISH_START   1000
#define TIME_SCENE_ADD_FISH_END     1000

#define TIME_CHECK_FISH_DESTORY_START  1000
#define TIME_CHECK_FISH_DESTORY_END    1000

#define CHANGE_SCENE_TIME_COUNT    600
#define SEND_SCENE_TIME_COUNT      10
#define FISH_DESTROY_TIME          30

//////////////////////////////////////////////////////////////////////////
#define MAX_FISH_OBJECT         2048
#define MAX_FISH_IN_NET         100
#define MAX_FISH_SEND           50

#define MAX_FISH_PATH           14
#define MAX_FISH_STYLE          12

#define MAX_CANNON_STYLE        7 

#define MAX_SCENE              4

#define MAX_SMALL_POINT_PATH   208
#define MAX_BIG_POINT_PATH     130
#define MAX_HUGE_POINT_PATH    62

#define MAX_LEFT_LINE_SPATH     3
#define MAX_RIGHT_LINE_SPATH     3

/////////////////////////////////////////////////////////////////////////
struct SPoint
{
    short x;
    short y;
};

struct Line_Path
{
    SPoint spStart;
    SPoint spEnd;
    DWORD dwTime;
};

/////////////////////////////////////////////////////////////////////////
struct Fish_Net_Object
{
    WORD wID;
    WORD wRoundID;
    WORD wType; 
    DWORD dwTime;
};

struct Role_Net_Object
{
    WORD wID;
    WORD wFishGold;
    WORD wCannonType;  
    WORD wFireCount;
};

struct Fish_With_Line_Path
{
    Fish_Net_Object FishNetObject;
    Line_Path LinePath;
};

struct Fish_With_Point_Path
{
    Fish_Net_Object FishNetObject;
    WORD wPath;
    DWORD dwTime;
};

struct Fish_With_Special_Point_Path
{
    WORD wPath;
    float fDelay;
    DWORD dwTime;
    Fish_Net_Object FishNetObject;
};

/////////////////////////////////////////////////////////////////////////
struct CMD_S_StatusFree
{
    BYTE cbScene;
    LONGLONG lCellScore;
    Role_Net_Object RoleObjects[GAME_PLAYER];
};

struct CMD_S_Change_Scene 
{
    BYTE cbScene;
};

struct CMD_C_Buy_Bullet
{
    WORD wCount;
};

struct CMD_S_Buy_Bullet_Success
{
    WORD wChairID;
    WORD wCount;
};

struct CMD_S_Buy_Bullet_Failed
{
    WORD wChairID;
    char szReason[32];
};


struct CMD_S_Send_Line_Path_Fish
{
    BYTE cbCount;
    Fish_With_Line_Path FishPaths[MAX_FISH_SEND];
};

struct CMD_S_Send_Point_Path_Fish
{
    BYTE cbCount;
    Fish_With_Point_Path FishPaths[MAX_FISH_SEND];
};

struct CMD_S_Send_Group_Point_Path_Fish
{
    WORD wPath;
    DWORD dwTime;
    BYTE cbPahtType;
    BYTE cbCount;
    Fish_Net_Object FishNetObject[MAX_FISH_SEND];
};

struct CMD_S_Send_Special_Point_Path
{
    BYTE cbCount;
    Fish_With_Special_Point_Path SpecialPointPath[MAX_FISH_SEND];
};

struct CMD_C_Fire
{
    float fRote;
};

struct CMD_S_Fire_Success
{
    WORD  wChairID;	
	WORD  wAndroidLogicChairID;				//机器人碰撞逻辑检测用户椅子ID
	int	  nFireCount;
    float fRote;
};

struct CMD_S_Fire_Failed
{
    WORD wChairID;
    char szReason[32];
};

struct CMD_C_Cast_Net
{
    WORD wChairID;
    BYTE cbCount;
    Fish_Net_Object FishNetObjects[MAX_FISH_IN_NET];
};

struct CMD_C_Bomb
{
    BYTE cbCount;
    Fish_Net_Object FishNetObjects[MAX_FISH_IN_NET];
};

struct CMD_C_Laser_Bean
{
    BYTE cbCount;
	float fRote;
    Fish_Net_Object FishNetObjects[MAX_FISH_IN_NET];
};

struct CMD_S_Cast_Net_Success
{
    BYTE cbCount;
    WORD wChairID;
    Fish_Net_Object FishNetObjects[MAX_FISH_IN_NET];
};

struct CMD_S_Cast_Net_Failed
{
    WORD wChairID;
    char szReason[32];
};

struct CMD_S_Bomb_Success
{
    BYTE cbCount;
    WORD wChairID;
    Fish_Net_Object FishNetObjects[MAX_FISH_IN_NET];
};

struct CMD_S_Bomb_Failed
{
    WORD wChairID;
    char szReason[32];
};

struct CMD_S_Laser_Bean_Success
{
    BYTE cbCount;
    WORD wChairID;
	float fRote;
    Fish_Net_Object FishNetObjects[MAX_FISH_IN_NET];
};

struct CMD_S_Laser_Bean_Failed
{
    WORD wChairID;
    char szReason[32];
};

struct CMD_C_Change_Cannon
{
    WORD wStyle;
};

struct CMD_S_Change_Cannon
{
    WORD wChairID;
    BYTE wStyle;
};

struct CMD_C_Account
{
    WORD wChairID;
};

struct CMD_S_Account
{
    WORD wChairID;
};

struct KillFishTask
{
	int nDuration;		/// 时间
	int nFishType;		/// 鱼类型
	int nFishCount;		/// 条数
	int m_nBonus;		/// 奖励金币
};

struct CMD_S_Task
{
    WORD wChairID;
    BYTE cbLevel;
    int nTask;
	int nBonus;
    int nDuration;
    BYTE nStartWheel[3];
	BYTE nEndWheel[3];
    BYTE cbFishType;
    BYTE cbFishCount;
};

struct CMD_C_Bonus
{
    int nBonus;
};

struct CMD_S_Bonus
{
    WORD wChairID;
    int nBonus;
};

struct CMD_C_Cook
{
    int nBonus;
};

struct CMD_S_Cook
{
    WORD wChairID;
    int nBonus;
    BYTE cbSucess;
};

struct CMD_C_Task_Prepared
{
   int nTask;
};

//////////////////////////////////////////////////////////////////////////

#endif
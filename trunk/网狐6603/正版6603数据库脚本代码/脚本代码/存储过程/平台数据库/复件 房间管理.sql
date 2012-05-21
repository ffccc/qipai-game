
----------------------------------------------------------------------------------------------------

USE QPPlatformDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GS_InsertGameRoom]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GS_InsertGameRoom]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GS_ModifyGameRoom]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GS_ModifyGameRoom]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GS_DeleteGameRoom]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GS_DeleteGameRoom]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GS_LoadGameRoomItem]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GS_LoadGameRoomItem]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- 插入房间
CREATE  PROCEDURE dbo.GSP_GS_InsertGameRoom

	-- 索引变量
	@wGameID INT,								-- 模块标识
	@wKindID INT,								-- 类型标识
	@wNodeID INT,								-- 挂接标识
	@wSortID INT,								-- 排序标识

	-- 积分配置
	@lCellScore INT,							-- 单元积分
	@cbRevenueRatio INT,						-- 税收比例

	-- 限制配置
	@lRestrictScore	BIGINT,						-- 限制积分
	@lMinTableScore	BIGINT,						-- 最低积分
	@lMinEnterScore	BIGINT,						-- 最低积分
	@lMaxEnterScore	BIGINT,						-- 最高积分

	-- 会员限制
	@cbMinEnterMember INT,						-- 最低会员
	@cbMaxEnterMember INT,						-- 最高会员

	-- 房间配置
	@dwServerRule INT,							-- 房间规则
	@dwAttachUserRight INT,						-- 附加权限

	-- 房间属性
	@wMaxPlayer INT,							-- 游戏人数
	@wTableCount INT,							-- 桌子数目
	@wServerPort INT,							-- 服务端口
	@wServerType INT,							-- 服务类型
	@strServerName NVARCHAR(32),				-- 房间名字

	-- 分组设置 6603源码中有,而数据库中没有 月影 2012.03.17
	@cbDistributeRule NVARCHAR(256),						--分组规则
	@wMinDistributeUser INT,							--最少人数
	@wMaxDistributeUser INT,							--最多人数
	@wDistributeTimeSpace INT,							--分组间隔
	@wDistributeDrawCount INT,							--分组局数
	@wDistributeStartDelay INT,							--开始延时

	-- 连接信息
	@strDataBaseName NVARCHAR(32),				-- 数据库名
	@strDataBaseAddr NVARCHAR(15),				-- 连接地址

	-- 数据设置
	@strCustomRule NVARCHAR(256),				-- 定制规则
	@strServiceMachine NVARCHAR(32),			-- 服务机器

	-- 输出信息
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息

WITH ENCRYPTION AS

-- 设置属性
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 查找模块
	DECLARE @GameID INT
	SELECT @GameID=GameID FROM GameGameItem(NOLOCK) WHERE GameID=@wGameID

	-- 存在判断
	IF @GameID IS NULL
	BEGIN
		SET @strErrorDescribe=N'相应的游戏服务组件信息不存在，房间创建失败！'
		RETURN 1
	END

	-- 获取端口
	IF @wServerPort<>0
	BEGIN
		
		-- 获取端口
		DECLARE @ServerPort INT
		SELECT @ServerPort=ServerPort FROM GameRoomInfo(NOLOCK) WHERE ServerPort=@wServerPort AND ServiceMachine=@strServiceMachine

		-- 端口判断
		IF @ServerPort=@wServerPort
		BEGIN
			SET @strErrorDescribe=N'存在服务端口相同的游戏房间，房间创建失败！'
			RETURN 1
		END

	END

	-- 创建房间 月影 2012.03.17
	INSERT INTO GameRoomInfo (ServerName, KindID, NodeID, SortID, GameID, TableCount, ServerPort, ServerType, CellScore,
		RevenueRatio, RestrictScore, MinTableScore, MinEnterScore, MaxEnterScore, MinEnterMember, MaxEnterMember, MaxPlayer,
		ServerRule, AttachUserRight, DataBaseName, DataBaseAddr, ServiceMachine, CustomRule, DistributeRule, 
MinDistributeUser, MaxDistributeUser, DistributeTimeSpace, DistributeDrawCount, DistributeStartDelay)
	VALUES (@strServerName, @wKindID, @wNodeID, @wSortID, @wGameID, @wTableCount, @wServerPort, @wServerType, @lCellScore,
		@cbRevenueRatio, @lRestrictScore, @lMinTableScore, @lMinEnterScore, @lMaxEnterScore, @cbMinEnterMember, @cbMaxEnterMember,
		@wMaxPlayer, @dwServerRule, @dwAttachUserRight, @strDataBaseName, @strDataBaseAddr, @strServiceMachine, @strCustomRule, @cbDistributeRule, @wMinDistributeUser, @wMaxDistributeUser, @wDistributeTimeSpace, @wDistributeDrawCount,
@wDistributeStartDelay)

	-- 加载房间
	SELECT GameRoomInfo.*, GameGameItem.GameName, GameGameItem.ServerVersion, GameGameItem.ClientVersion, GameGameItem.ServerDLLName,
		GameGameItem.ClientExeName FROM GameRoomInfo(NOLOCK), GameGameItem(NOLOCK)
	WHERE GameRoomInfo.GameID=GameGameItem.GameID AND ServerID=SCOPE_IDENTITY()

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- 修改房间
CREATE  PROCEDURE dbo.GSP_GS_ModifyGameRoom

	-- 索引变量
	@wServerID INT,								-- 房间标识

	-- 挂接属性
	@wKindID INT,								-- 类型标识
	@wNodeID INT,								-- 挂接标识
	@wSortID INT,								-- 排序标识

	-- 税收配置
	@lCellScore INT,							-- 单元积分
	@cbRevenueRatio INT,						-- 税收比例

	-- 限制配置
	@lRestrictScore	BIGINT,						-- 限制积分
	@lMinTableScore	BIGINT,						-- 最低积分
	@lMinEnterScore	BIGINT,						-- 最低积分
	@lMaxEnterScore	BIGINT,						-- 最高积分

	-- 会员限制
	@cbMinEnterMember INT,						-- 最低会员
	@cbMaxEnterMember INT,						-- 最高会员

	-- 房间配置
	@dwServerRule INT,							-- 房间规则
	@dwAttachUserRight INT,						-- 附加权限

	-- 房间属性
	@wMaxPlayer INT,							-- 游戏人数
	@wTableCount INT,							-- 桌子数目
	@wServerPort INT,							-- 服务端口
	@wServerType INT,							-- 服务类型
	@strServerName NVARCHAR(32),				-- 房间名字

	-- 连接信息
	@strDataBaseName NVARCHAR(32),				-- 数据库名
	@strDataBaseAddr NVARCHAR(15),				-- 连接地址

	-- 数据设置
	@strCustomRule NVARCHAR(256),				-- 定制规则
	@strServiceMachine NVARCHAR(32),			-- 服务机器

	-- 输出信息
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息

WITH ENCRYPTION AS

-- 设置属性
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 查找房间
	DECLARE @ServerID INT
	SELECT @ServerID=ServerID FROM GameRoomInfo(NOLOCK) WHERE ServerID=@wServerID

	-- 结果判断
	IF @ServerID IS NULL
	BEGIN
		SET @strErrorDescribe=N'游戏房间不存在或者已经被删除了，房间修改失败！'
		RETURN 2
	END

	-- 获取端口
	IF @wServerPort<>0
	BEGIN
		
		-- 获取端口
		DECLARE @ServerPort INT
		SELECT @ServerPort=ServerPort FROM GameRoomInfo(NOLOCK) WHERE ServerPort=@wServerPort AND ServiceMachine=@strServiceMachine

		-- 端口判断
		IF @ServerPort=@wServerPort
		BEGIN
			SET @strErrorDescribe=N'存在服务端口相同的游戏房间，房间修改失败！'
			RETURN 1
		END

	END

	-- 更新房间
	UPDATE GameRoomInfo SET ServerName=@strServerName, KindID=@wKindID, NodeID=@wNodeID, SortID=@wSortID,
		TableCount=@wTableCount, ServerPort=@wServerPort, ServerType=@wServerType, CellScore=@lCellScore,
		RevenueRatio=@cbRevenueRatio, RestrictScore=@lRestrictScore, MinTableScore=@lMinTableScore, MinEnterScore=@lMaxEnterScore, 
		MaxEnterScore=@cbMinEnterMember, MinEnterMember=@cbMinEnterMember, MaxEnterMember=@cbMaxEnterMember, MaxPlayer=@wMaxPlayer, 
		ServerRule=@dwServerRule, AttachUserRight=@dwAttachUserRight, DataBaseName=@strDataBaseName, DataBaseAddr=@strDataBaseAddr,
		ServiceMachine=@strServiceMachine, CustomRule=@strCustomRule
	WHERE ServerID=@ServerID

	-- 加载房间
	SELECT GameRoomInfo.*, GameGameItem.GameName, GameGameItem.ServerVersion, GameGameItem.ClientVersion, GameGameItem.ServerDLLName,
		GameGameItem.ClientExeName FROM GameRoomInfo(NOLOCK), GameGameItem(NOLOCK)
	WHERE GameRoomInfo.GameID=GameGameItem.GameID AND ServerID=@wServerID

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- 删除房间
CREATE  PROCEDURE dbo.GSP_GS_DeleteGameRoom
	@wServerID INT,								-- 房间标识
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
WITH ENCRYPTION AS

-- 设置属性
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 查找房间
	DECLARE @ServerID INT
	SELECT @ServerID=ServerID FROM GameRoomInfo(NOLOCK) WHERE ServerID=@wServerID

	-- 结果判断
	IF @ServerID IS NULL
	BEGIN
		SET @strErrorDescribe=N'游戏房间不存在或者已经被删除了，房间修改失败！'
		RETURN 1
	END

	-- 删除房间
	DELETE GameRoomInfo WHERE ServerID=@wServerID

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- 加载房间
CREATE  PROCEDURE dbo.GSP_GS_LoadGameRoomItem
	@wServerID INT,								-- 房间标识
	@strMachineID NCHAR(32),					-- 服务机器
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
WITH ENCRYPTION AS

-- 设置属性
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 加载房间
	SELECT GameRoomInfo.*, GameGameItem.GameName, GameGameItem.ServerVersion, GameGameItem.ClientVersion,
		GameGameItem.ServerDLLName, GameGameItem.ClientExeName FROM GameRoomInfo(NOLOCK), GameGameItem(NOLOCK)
	WHERE GameRoomInfo.GameID=GameGameItem.GameID AND ((@wServerID=ServerID) OR (@wServerID=0))
		AND ((ServiceMachine=@strMachineID) OR (@strMachineID=''))
	ORDER BY GameName,ServerName

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

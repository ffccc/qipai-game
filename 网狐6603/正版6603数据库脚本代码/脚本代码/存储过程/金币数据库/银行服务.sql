
----------------------------------------------------------------------------------------------------

USE QPTreasureDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_UserSaveScore]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_UserSaveScore]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_UserTakeScore]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_UserTakeScore]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_QueryUserInsureInfo]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_QueryUserInsureInfo]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON
GO

----------------------------------------------------------------------------------------------------

-- 存入金币
CREATE PROC GSP_GR_UserSaveScore
	@dwUserID INT,								-- 用户 I D
	@lSaveScore BIGINT,							-- 金币数目
	@strClientIP NVARCHAR(15),					-- 连接地址
	@strMachineID NVARCHAR(32),					-- 机器标识
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 金币变量
DECLARE @SourceScore BIGINT
DECLARE @SourceInsure BIGINT
DECLARE @InsureRevenue BIGINT
DECLARE @VariationScore BIGINT
DECLARE @VariationInsure BIGINT

-- 执行逻辑
BEGIN

	-- 辅助变量
	DECLARE @EnjoinLogon INT
	DECLARE @EnjoinInsure INT

	-- 系统暂停
	SELECT @EnjoinInsure=StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinInsure'
	IF @EnjoinInsure IS NOT NULL AND @EnjoinInsure<>0
	BEGIN
		SELECT @strErrorDescribe=StatusString FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinInsure'
		RETURN 2
	END

	-- 效验地址
	SELECT @EnjoinLogon=EnjoinLogon FROM ConfineAddress(NOLOCK) WHERE AddrString=@strClientIP AND GETDATE()<EnjoinOverDate
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SET @strErrorDescribe=N'抱歉地通知您，系统禁止了您所在的 IP 地址的游戏服务权限，请联系客户服务中心了解详细情况！'
		RETURN 4
	END
	
	-- 效验机器
	SELECT @EnjoinLogon=EnjoinLogon FROM ConfineMachine(NOLOCK) WHERE MachineSerial=@strMachineID AND GETDATE()<EnjoinOverDate
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SET @strErrorDescribe=N'抱歉地通知您，系统禁止了您的机器的游戏服务权限，请联系客户服务中心了解详细情况！'
		RETURN 7
	END
 
	-- 查询用户
	DECLARE @UserID INT
	DECLARE @Nullity BIT
	DECLARE @StunDown BIT
	DECLARE	@MachineID NVARCHAR(32)
	DECLARE @MoorMachine AS TINYINT
	SELECT @UserID=UserID, @Nullity=Nullity, @StunDown=StunDown, @MoorMachine=MoorMachine, @MachineID=LastLogonMachine
	FROM QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo WHERE UserID=@dwUserID

	-- 查询用户
	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'您的帐号不存在或者密码输入有误，请查证后再次尝试登录！'
		RETURN 1
	END	

	-- 帐号禁止
	IF @Nullity<>0
	BEGIN
		SET @strErrorDescribe=N'您的帐号暂时处于冻结状态，请联系客户服务中心了解详细情况！'
		RETURN 2
	END	

	-- 帐号关闭
	IF @StunDown<>0
	BEGIN
		SET @strErrorDescribe=N'您的帐号使用了安全关闭功能，必须重新开通后才能继续使用！'
		RETURN 2
	END	
	
	-- 固定机器
	IF @MoorMachine=1
	BEGIN
		IF @MachineID<>@strMachineID
		BEGIN
			SET @strErrorDescribe=N'您的帐号使用固定机器登录功能，您现所使用的机器不是所指定的机器！'
			RETURN 1
		END
	END

	-- 金币判断
	IF @lSaveScore<1000
	BEGIN
		SET @strErrorDescribe=N'存入保险柜的游戏币数目不能少于 1000，游戏币存入失败！'
		RETURN 4
	END

	-- 开始事务
	SET TRANSACTION ISOLATION LEVEL REPEATABLE READ
	BEGIN TRAN

	-- 游戏信息
	SELECT @SourceScore=Score, @SourceInsure=InsureScore FROM GameScoreInfo WHERE UserID=@dwUserID

	-- 金币判断
	IF @SourceScore IS NULL OR @SourceScore<@lSaveScore
	BEGIN
		-- 结束事务
		ROLLBACK TRAN
		SET TRANSACTION ISOLATION LEVEL READ COMMITTED

		-- 错误信息
		SET @strErrorDescribe=N'您当前游戏币的可用额度不够，游戏币存入失败！'
		RETURN 4
	END

	-- 查询锁定
	DECLARE @LockKindID INT
	DECLARE @LockServerID INT
	SELECT @LockKindID=KindID, @LockServerID=ServerID FROM GameScoreLocker WHERE UserID=@dwUserID

	-- 锁定判断
	IF @LockKindID IS NOT NULL AND @LockServerID IS NOT NULL
	BEGIN

		-- 结束事务
		ROLLBACK TRAN
		SET TRANSACTION ISOLATION LEVEL READ COMMITTED

		-- 查询类型
		IF @LockKindID<>0
		BEGIN
			-- 查询信息
			DECLARE @KindName NVARCHAR(31)
			DECLARE @ServerName NVARCHAR(31)
			SELECT @KindName=KindName FROM QPPlatformDBLink.QPPlatformDB.dbo.GameKindItem WHERE KindID=@LockKindID
			SELECT @ServerName=ServerName FROM QPPlatformDBLink.QPPlatformDB.dbo.GameRoomInfo WHERE ServerID=@LockServerID

			-- 错误信息
			IF @KindName IS NULL SET @KindName=N'未知游戏'
			IF @ServerName IS NULL SET @ServerName=N'未知房间'
			SET @strErrorDescribe=N'您正在 [ '+@KindName+N' ] 的 [ '+@ServerName+N' ] 游戏房间中，不能进行当前的保险柜操作！'
			RETURN 4

		END
		ELSE
		BEGIN
			-- 提示消息
			SELECT [ErrorDescribe]=N'您正在使用网站页面进行保险柜处理过程中，不能进行当前的保险柜操作！'
			RETURN 4
		END
	END

	-- 计算变量
	SET @InsureRevenue=0
	SET @VariationScore=-@lSaveScore
	SET @VariationInsure=@lSaveScore

	-- 设置信息
	SET @strErrorDescribe=N'游戏币存入保险柜操作成功，请查验您的账户信息！'

	-- 更新数据
	UPDATE GameScoreInfo SET Score=Score+@VariationScore, InsureScore=InsureScore+@VariationInsure, Revenue=Revenue+@InsureRevenue
	WHERE UserID=@dwUserID

	-- 记录日志

	-- 结束事务
	COMMIT TRAN
	SET TRANSACTION ISOLATION LEVEL READ COMMITTED

	-- 输出结果
	SELECT @dwUserID AS UserID, @SourceScore AS SourceScore, @SourceInsure AS SourceInsure, @VariationScore AS VariationScore,
		@VariationInsure AS VariationInsure, @InsureRevenue AS InsureRevenue

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- 提取金币
CREATE PROC GSP_GR_UserTakeScore
	@dwUserID INT,								-- 用户 I D
	@lTakeScore BIGINT,							-- 金币数目
	@strPassword NCHAR(32),						-- 用户密码
	@strClientIP NVARCHAR(15),					-- 连接地址
	@strMachineID NVARCHAR(32),					-- 机器标识
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 金币变量
DECLARE @SourceScore BIGINT
DECLARE @SourceInsure BIGINT
DECLARE @InsureRevenue BIGINT
DECLARE @VariationScore BIGINT
DECLARE @VariationInsure BIGINT

-- 执行逻辑
BEGIN

	-- 辅助变量
	DECLARE @EnjoinLogon INT
	DECLARE @EnjoinInsure INT

	-- 系统暂停
	SELECT @EnjoinInsure=StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinInsure'
	IF @EnjoinInsure IS NOT NULL AND @EnjoinInsure<>0
	BEGIN
		SELECT @strErrorDescribe=StatusString FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinInsure'
		RETURN 2
	END

	-- 效验地址
	SELECT @EnjoinLogon=EnjoinLogon FROM ConfineAddress(NOLOCK) WHERE AddrString=@strClientIP AND GETDATE()<EnjoinOverDate
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SET @strErrorDescribe=N'抱歉地通知您，系统禁止了您所在的 IP 地址的游戏服务权限，请联系客户服务中心了解详细情况！'
		RETURN 4
	END
	
	-- 效验机器
	SELECT @EnjoinLogon=EnjoinLogon FROM ConfineMachine(NOLOCK) WHERE MachineSerial=@strMachineID AND GETDATE()<EnjoinOverDate
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SET @strErrorDescribe=N'抱歉地通知您，系统禁止了您的机器的游戏服务权限，请联系客户服务中心了解详细情况！'
		RETURN 7
	END
 
	-- 查询用户
	DECLARE @UserID INT
	DECLARE @Nullity BIT
	DECLARE @StunDown BIT
	DECLARE @InsurePass AS NCHAR(32)
	DECLARE	@MachineID NVARCHAR(32)
	DECLARE @MoorMachine AS TINYINT
	SELECT @UserID=UserID, @InsurePass=InsurePass, @Nullity=Nullity, @StunDown=StunDown, @MoorMachine=MoorMachine, @MachineID=LastLogonMachine
	FROM QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo WHERE UserID=@dwUserID

	-- 查询用户
	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'您的帐号不存在或者密码输入有误，请查证后再次尝试登录！'
		RETURN 1
	END	

	-- 帐号禁止
	IF @Nullity<>0
	BEGIN
		SET @strErrorDescribe=N'您的帐号暂时处于冻结状态，请联系客户服务中心了解详细情况！'
		RETURN 2
	END	

	-- 帐号关闭
	IF @StunDown<>0
	BEGIN
		SET @strErrorDescribe=N'您的帐号使用了安全关闭功能，必须重新开通后才能继续使用！'
		RETURN 2
	END	
	
	-- 固定机器
	IF @MoorMachine=1
	BEGIN
		IF @MachineID<>@strMachineID
		BEGIN
			SET @strErrorDescribe=N'您的帐号使用固定机器登录功能，您现所使用的机器不是所指定的机器！'
			RETURN 1
		END
	END

	-- 密码判断
	IF @InsurePass<>@strPassword
	BEGIN
		SET @strErrorDescribe=N'您的保险柜密码不正确或者输入有误，请查证后再次尝试！'
		RETURN 3
	END

	-- 金币判断
	IF @lTakeScore<1000
	BEGIN
		SET @strErrorDescribe=N'从保险柜取出的游戏币数目不能少于 1000，游戏币提取失败！'
		RETURN 4
	END

	-- 开始事务
	SET TRANSACTION ISOLATION LEVEL REPEATABLE READ
	BEGIN TRAN

	-- 游戏信息
	SELECT @SourceScore=Score, @SourceInsure=InsureScore FROM GameScoreInfo WHERE UserID=@dwUserID

	-- 金币判断
	IF @SourceInsure IS NULL OR @SourceInsure<@lTakeScore
	BEGIN
		-- 结束事务
		ROLLBACK TRAN
		SET TRANSACTION ISOLATION LEVEL READ COMMITTED

		-- 错误信息
		SET @strErrorDescribe=N'您当前保险柜的游戏币余额不足，游戏币提取失败！'
		RETURN 4
	END

	-- 查询锁定
	DECLARE @LockKindID INT
	DECLARE @LockServerID INT
	SELECT @LockKindID=KindID, @LockServerID=ServerID FROM GameScoreLocker WHERE UserID=@dwUserID

	-- 锁定判断
	IF @LockKindID IS NOT NULL AND @LockServerID IS NOT NULL
	BEGIN

		-- 结束事务
		ROLLBACK TRAN
		SET TRANSACTION ISOLATION LEVEL READ COMMITTED

		-- 查询类型
		IF @LockKindID<>0
		BEGIN
			-- 查询信息
			DECLARE @KindName NVARCHAR(31)
			DECLARE @ServerName NVARCHAR(31)
			SELECT @KindName=KindName FROM QPPlatformDBLink.QPPlatformDB.dbo.GameKindItem WHERE KindID=@LockKindID
			SELECT @ServerName=ServerName FROM QPPlatformDBLink.QPPlatformDB.dbo.GameRoomInfo WHERE ServerID=@LockServerID

			-- 错误信息
			IF @KindName IS NULL SET @KindName=N'未知游戏'
			IF @ServerName IS NULL SET @ServerName=N'未知房间'
			SET @strErrorDescribe=N'您正在 [ '+@KindName+N' ] 的 [ '+@ServerName+N' ] 游戏房间中，不能进行当前的保险柜操作！'
			RETURN 4

		END
		ELSE
		BEGIN
			-- 提示消息
			SELECT [ErrorDescribe]=N'您正在使用网站页面进行保险柜处理过程中，不能进行当前的保险柜操作！'
			RETURN 4
		END
	END

	-- 计算变量
	SET @InsureRevenue=@lTakeScore*1/100
	SET @VariationScore=@lTakeScore-@InsureRevenue
	SET @VariationInsure=-@lTakeScore

	-- 设置信息
	SET @strErrorDescribe=N'保险柜提取游戏币操作成功，请查验您的账户信息！'

	-- 更新数据
	UPDATE GameScoreInfo SET Score=Score+@VariationScore, InsureScore=InsureScore+@VariationInsure, Revenue=Revenue+@InsureRevenue
	WHERE UserID=@dwUserID

	-- 记录日志

	-- 结束事务
	COMMIT TRAN
	SET TRANSACTION ISOLATION LEVEL READ COMMITTED

	-- 输出结果
	SELECT @dwUserID AS UserID, @SourceScore AS SourceScore, @SourceInsure AS SourceInsure, @VariationScore AS VariationScore,
		@VariationInsure AS VariationInsure, @InsureRevenue AS InsureRevenue

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- 查询银行
CREATE PROC GSP_GR_QueryUserInsureInfo
	@dwUserID INT,								-- 用户 I D
	@strClientIP NVARCHAR(15),					-- 连接地址
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 基本信息
DECLARE @Score BIGINT
DECLARE @Insure BIGINT
DECLARE @ServerID SMALLINT
DECLARE @InsurePass NVARCHAR(32)

-- 执行逻辑
BEGIN

	-- 查询房间
	SELECT @ServerID=ServerID FROM GameScoreLocker(NOLOCK) WHERE UserID=@dwUserID

	-- 查询积分
	SELECT @Score=Score, @Insure=InsureScore FROM GameScoreInfo(NOLOCK) WHERE UserID=@dwUserID

	-- 密码查询
	SELECT @InsurePass=InsurePass FROM QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo WHERE UserID=@dwUserID

	-- 数据调整
	IF @Score IS NULL SET @Score=0
	IF @Insure IS NULL SET @Insure=0
	IF @ServerID IS NULL SET @ServerID=0
	IF @InsurePass IS NULL SET @InsurePass=N''

	-- 输出结果
	SELECT @dwUserID AS UserID, @Score AS Score, @Insure AS Insure, @ServerID AS ServerID, @InsurePass AS InsurePass

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

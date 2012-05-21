
----------------------------------------------------------------------------------------------------

USE QPAccountsDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_QueryUserIndividual]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_QueryUserIndividual]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_ModifyUserIndividual]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_ModifyUserIndividual]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- 查询资料
CREATE PROC GSP_GP_QueryUserIndividual
	@dwUserID INT,								-- 用户 I D
	@strClientIP NVARCHAR(15)					-- 连接地址
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 变量定义
	DECLARE @UserID INT
	DECLARE @QQ NVARCHAR(16)
	DECLARE @EMail NVARCHAR(33)
	DECLARE @UserNote NVARCHAR(256)
	DECLARE @SeatPhone NVARCHAR(32)
	DECLARE @MobilePhone NVARCHAR(16)
	DECLARE @Compellation NVARCHAR(16)
	DECLARE @DwellingPlace NVARCHAR(128)

	-- 查询用户
	SELECT @UserID=UserID, @QQ=QQ, @EMail=EMail, @UserNote=UserNote, @SeatPhone=SeatPhone,
		@MobilePhone=MobilePhone, @Compellation=Compellation, @DwellingPlace=DwellingPlace
	FROM IndividualDatum(NOLOCK) WHERE UserID=@dwUserID

	-- 查询用户
	IF @UserID IS NULL
	BEGIN
		SET @QQ=N''	
		SET @EMail=N''	
		SET @UserNote=N''	
		SET @SeatPhone=N''	
		SET @MobilePhone=N''	
		SET @Compellation=N''	
		SET @DwellingPlace=N''	
	END

	-- 输出信息
	SELECT @dwUserID AS UserID, @Compellation AS Compellation, @QQ AS QQ, @EMail AS EMail, @SeatPhone AS SeatPhone,
		@MobilePhone AS MobilePhone, @DwellingPlace AS DwellingPlace, @UserNote AS UserNote

	RETURN 0

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- 更新资料
CREATE PROC GSP_GP_ModifyUserIndividual
	@dwUserID INT,								-- 用户 I D
	@strPassword NCHAR(32),						-- 用户密码
	@cbGender TINYINT,							-- 用户性别
	@strNickName NVARCHAR(32),					-- 用户昵称
	@strUnderWrite NVARCHAR(63),				-- 个性签名
	@strCompellation NVARCHAR(16),				-- 真实名字
	@strQQ NVARCHAR(16),						-- Q Q 号码
	@strEMail NVARCHAR(33),						-- 电子邮电
	@strSeatPhone NVARCHAR(32),					-- 固定电话
	@strMobilePhone NVARCHAR(16),				-- 移动电话
	@strDwellingPlace NVARCHAR(128),			-- 详细地址
	@strUserNote NVARCHAR(256),					-- 用户备注				
	@strClientIP NVARCHAR(15),					-- 连接地址
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- 输出信息
WITH ENCRYPTION AS

-- 属性设置
SET NOCOUNT ON

-- 执行逻辑
BEGIN

	-- 变量定义
	DECLARE @UserID INT
	DECLARE @Nullity BIT
	DECLARE @StunDown BIT
	DECLARE @LogonPass AS NCHAR(32)

	-- 查询用户
	SELECT @UserID=UserID, @LogonPass=LogonPass, @Nullity=Nullity, @StunDown=StunDown
	FROM AccountsInfo(NOLOCK) WHERE UserID=@dwUserID

	-- 查询用户
	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'您的帐号不存在或者密码输入有误，请查证后再次尝试！'
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
	
	-- 密码判断
	IF @LogonPass<>@strPassword
	BEGIN
		SET @strErrorDescribe=N'您的帐号不存在或者密码输入有误，请查证后再次尝试登录！'
		RETURN 3
	END

	-- 效验昵称
	IF (SELECT COUNT(*) FROM ConfineContent(NOLOCK) WHERE CHARINDEX(String,@strNickName)>0)>0
	BEGIN
		SET @strErrorDescribe=N'您所输入的游戏昵称名含有限制字符串，请更换昵称名后再次修改！'
		RETURN 4
	END

	-- 存在判断
	IF EXISTS (SELECT UserID FROM AccountsInfo(NOLOCK) WHERE NickName=@strNickName AND UserID<>@dwUserID)
	BEGIN
		SET @strErrorDescribe=N'此昵称已被其他玩家使用了，请更换昵称名后再次修改！'
		RETURN 4
	END

	-- 修改资料
	UPDATE AccountsInfo SET NickName=@strNickName, UnderWrite=@strUnderWrite, Gender=@cbGender WHERE UserID=@dwUserID

	-- 修改资料
	UPDATE IndividualDatum SET Compellation=@strCompellation, QQ=@strQQ, EMail=@strEMail, SeatPhone=@strSeatPhone,
		MobilePhone=@strMobilePhone, DwellingPlace=@strDwellingPlace, UserNote=@strUserNote WHERE UserID=@dwUserID
		
	-- 修改资料
	IF @@ROWCOUNT=0
	INSERT IndividualDatum (UserID, Compellation, QQ, EMail, SeatPhone, MobilePhone, DwellingPlace, UserNote)
	VALUES (@dwUserID, @strCompellation, @strQQ, @strEMail, @strSeatPhone, @strMobilePhone, @strDwellingPlace, @strUserNote)

	-- 设置信息
	IF @@ERROR=0 SET @strErrorDescribe=N'服务器已经接受了您的新资料！'

	RETURN 0

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------


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

-- ������
CREATE PROC GSP_GR_UserSaveScore
	@dwUserID INT,								-- �û� I D
	@lSaveScore BIGINT,							-- �����Ŀ
	@strClientIP NVARCHAR(15),					-- ���ӵ�ַ
	@strMachineID NVARCHAR(32),					-- ������ʶ
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ��ұ���
DECLARE @SourceScore BIGINT
DECLARE @SourceInsure BIGINT
DECLARE @InsureRevenue BIGINT
DECLARE @VariationScore BIGINT
DECLARE @VariationInsure BIGINT

-- ִ���߼�
BEGIN

	-- ��������
	DECLARE @EnjoinLogon INT
	DECLARE @EnjoinInsure INT

	-- ϵͳ��ͣ
	SELECT @EnjoinInsure=StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinInsure'
	IF @EnjoinInsure IS NOT NULL AND @EnjoinInsure<>0
	BEGIN
		SELECT @strErrorDescribe=StatusString FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinInsure'
		RETURN 2
	END

	-- Ч���ַ
	SELECT @EnjoinLogon=EnjoinLogon FROM ConfineAddress(NOLOCK) WHERE AddrString=@strClientIP AND GETDATE()<EnjoinOverDate
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SET @strErrorDescribe=N'��Ǹ��֪ͨ����ϵͳ��ֹ�������ڵ� IP ��ַ����Ϸ����Ȩ�ޣ�����ϵ�ͻ����������˽���ϸ�����'
		RETURN 4
	END
	
	-- Ч�����
	SELECT @EnjoinLogon=EnjoinLogon FROM ConfineMachine(NOLOCK) WHERE MachineSerial=@strMachineID AND GETDATE()<EnjoinOverDate
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SET @strErrorDescribe=N'��Ǹ��֪ͨ����ϵͳ��ֹ�����Ļ�������Ϸ����Ȩ�ޣ�����ϵ�ͻ����������˽���ϸ�����'
		RETURN 7
	END
 
	-- ��ѯ�û�
	DECLARE @UserID INT
	DECLARE @Nullity BIT
	DECLARE @StunDown BIT
	DECLARE	@MachineID NVARCHAR(32)
	DECLARE @MoorMachine AS TINYINT
	SELECT @UserID=UserID, @Nullity=Nullity, @StunDown=StunDown, @MoorMachine=MoorMachine, @MachineID=LastLogonMachine
	FROM QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo WHERE UserID=@dwUserID

	-- ��ѯ�û�
	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'�����ʺŲ����ڻ������������������֤���ٴγ��Ե�¼��'
		RETURN 1
	END	

	-- �ʺŽ�ֹ
	IF @Nullity<>0
	BEGIN
		SET @strErrorDescribe=N'�����ʺ���ʱ���ڶ���״̬������ϵ�ͻ����������˽���ϸ�����'
		RETURN 2
	END	

	-- �ʺŹر�
	IF @StunDown<>0
	BEGIN
		SET @strErrorDescribe=N'�����ʺ�ʹ���˰�ȫ�رչ��ܣ��������¿�ͨ����ܼ���ʹ�ã�'
		RETURN 2
	END	
	
	-- �̶�����
	IF @MoorMachine=1
	BEGIN
		IF @MachineID<>@strMachineID
		BEGIN
			SET @strErrorDescribe=N'�����ʺ�ʹ�ù̶�������¼���ܣ�������ʹ�õĻ���������ָ���Ļ�����'
			RETURN 1
		END
	END

	-- ����ж�
	IF @lSaveScore<1000
	BEGIN
		SET @strErrorDescribe=N'���뱣�չ����Ϸ����Ŀ�������� 1000����Ϸ�Ҵ���ʧ�ܣ�'
		RETURN 4
	END

	-- ��ʼ����
	SET TRANSACTION ISOLATION LEVEL REPEATABLE READ
	BEGIN TRAN

	-- ��Ϸ��Ϣ
	SELECT @SourceScore=Score, @SourceInsure=InsureScore FROM GameScoreInfo WHERE UserID=@dwUserID

	-- ����ж�
	IF @SourceScore IS NULL OR @SourceScore<@lSaveScore
	BEGIN
		-- ��������
		ROLLBACK TRAN
		SET TRANSACTION ISOLATION LEVEL READ COMMITTED

		-- ������Ϣ
		SET @strErrorDescribe=N'����ǰ��Ϸ�ҵĿ��ö�Ȳ�������Ϸ�Ҵ���ʧ�ܣ�'
		RETURN 4
	END

	-- ��ѯ����
	DECLARE @LockKindID INT
	DECLARE @LockServerID INT
	SELECT @LockKindID=KindID, @LockServerID=ServerID FROM GameScoreLocker WHERE UserID=@dwUserID

	-- �����ж�
	IF @LockKindID IS NOT NULL AND @LockServerID IS NOT NULL
	BEGIN

		-- ��������
		ROLLBACK TRAN
		SET TRANSACTION ISOLATION LEVEL READ COMMITTED

		-- ��ѯ����
		IF @LockKindID<>0
		BEGIN
			-- ��ѯ��Ϣ
			DECLARE @KindName NVARCHAR(31)
			DECLARE @ServerName NVARCHAR(31)
			SELECT @KindName=KindName FROM QPPlatformDBLink.QPPlatformDB.dbo.GameKindItem WHERE KindID=@LockKindID
			SELECT @ServerName=ServerName FROM QPPlatformDBLink.QPPlatformDB.dbo.GameRoomInfo WHERE ServerID=@LockServerID

			-- ������Ϣ
			IF @KindName IS NULL SET @KindName=N'δ֪��Ϸ'
			IF @ServerName IS NULL SET @ServerName=N'δ֪����'
			SET @strErrorDescribe=N'������ [ '+@KindName+N' ] �� [ '+@ServerName+N' ] ��Ϸ�����У����ܽ��е�ǰ�ı��չ������'
			RETURN 4

		END
		ELSE
		BEGIN
			-- ��ʾ��Ϣ
			SELECT [ErrorDescribe]=N'������ʹ����վҳ����б��չ�������У����ܽ��е�ǰ�ı��չ������'
			RETURN 4
		END
	END

	-- �������
	SET @InsureRevenue=0
	SET @VariationScore=-@lSaveScore
	SET @VariationInsure=@lSaveScore

	-- ������Ϣ
	SET @strErrorDescribe=N'��Ϸ�Ҵ��뱣�չ�����ɹ�������������˻���Ϣ��'

	-- ��������
	UPDATE GameScoreInfo SET Score=Score+@VariationScore, InsureScore=InsureScore+@VariationInsure, Revenue=Revenue+@InsureRevenue
	WHERE UserID=@dwUserID

	-- ��¼��־

	-- ��������
	COMMIT TRAN
	SET TRANSACTION ISOLATION LEVEL READ COMMITTED

	-- ������
	SELECT @dwUserID AS UserID, @SourceScore AS SourceScore, @SourceInsure AS SourceInsure, @VariationScore AS VariationScore,
		@VariationInsure AS VariationInsure, @InsureRevenue AS InsureRevenue

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- ��ȡ���
CREATE PROC GSP_GR_UserTakeScore
	@dwUserID INT,								-- �û� I D
	@lTakeScore BIGINT,							-- �����Ŀ
	@strPassword NCHAR(32),						-- �û�����
	@strClientIP NVARCHAR(15),					-- ���ӵ�ַ
	@strMachineID NVARCHAR(32),					-- ������ʶ
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ��ұ���
DECLARE @SourceScore BIGINT
DECLARE @SourceInsure BIGINT
DECLARE @InsureRevenue BIGINT
DECLARE @VariationScore BIGINT
DECLARE @VariationInsure BIGINT

-- ִ���߼�
BEGIN

	-- ��������
	DECLARE @EnjoinLogon INT
	DECLARE @EnjoinInsure INT

	-- ϵͳ��ͣ
	SELECT @EnjoinInsure=StatusValue FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinInsure'
	IF @EnjoinInsure IS NOT NULL AND @EnjoinInsure<>0
	BEGIN
		SELECT @strErrorDescribe=StatusString FROM SystemStatusInfo(NOLOCK) WHERE StatusName=N'EnjoinInsure'
		RETURN 2
	END

	-- Ч���ַ
	SELECT @EnjoinLogon=EnjoinLogon FROM ConfineAddress(NOLOCK) WHERE AddrString=@strClientIP AND GETDATE()<EnjoinOverDate
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SET @strErrorDescribe=N'��Ǹ��֪ͨ����ϵͳ��ֹ�������ڵ� IP ��ַ����Ϸ����Ȩ�ޣ�����ϵ�ͻ����������˽���ϸ�����'
		RETURN 4
	END
	
	-- Ч�����
	SELECT @EnjoinLogon=EnjoinLogon FROM ConfineMachine(NOLOCK) WHERE MachineSerial=@strMachineID AND GETDATE()<EnjoinOverDate
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SET @strErrorDescribe=N'��Ǹ��֪ͨ����ϵͳ��ֹ�����Ļ�������Ϸ����Ȩ�ޣ�����ϵ�ͻ����������˽���ϸ�����'
		RETURN 7
	END
 
	-- ��ѯ�û�
	DECLARE @UserID INT
	DECLARE @Nullity BIT
	DECLARE @StunDown BIT
	DECLARE @InsurePass AS NCHAR(32)
	DECLARE	@MachineID NVARCHAR(32)
	DECLARE @MoorMachine AS TINYINT
	SELECT @UserID=UserID, @InsurePass=InsurePass, @Nullity=Nullity, @StunDown=StunDown, @MoorMachine=MoorMachine, @MachineID=LastLogonMachine
	FROM QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo WHERE UserID=@dwUserID

	-- ��ѯ�û�
	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'�����ʺŲ����ڻ������������������֤���ٴγ��Ե�¼��'
		RETURN 1
	END	

	-- �ʺŽ�ֹ
	IF @Nullity<>0
	BEGIN
		SET @strErrorDescribe=N'�����ʺ���ʱ���ڶ���״̬������ϵ�ͻ����������˽���ϸ�����'
		RETURN 2
	END	

	-- �ʺŹر�
	IF @StunDown<>0
	BEGIN
		SET @strErrorDescribe=N'�����ʺ�ʹ���˰�ȫ�رչ��ܣ��������¿�ͨ����ܼ���ʹ�ã�'
		RETURN 2
	END	
	
	-- �̶�����
	IF @MoorMachine=1
	BEGIN
		IF @MachineID<>@strMachineID
		BEGIN
			SET @strErrorDescribe=N'�����ʺ�ʹ�ù̶�������¼���ܣ�������ʹ�õĻ���������ָ���Ļ�����'
			RETURN 1
		END
	END

	-- �����ж�
	IF @InsurePass<>@strPassword
	BEGIN
		SET @strErrorDescribe=N'���ı��չ����벻��ȷ���������������֤���ٴγ��ԣ�'
		RETURN 3
	END

	-- ����ж�
	IF @lTakeScore<1000
	BEGIN
		SET @strErrorDescribe=N'�ӱ��չ�ȡ������Ϸ����Ŀ�������� 1000����Ϸ����ȡʧ�ܣ�'
		RETURN 4
	END

	-- ��ʼ����
	SET TRANSACTION ISOLATION LEVEL REPEATABLE READ
	BEGIN TRAN

	-- ��Ϸ��Ϣ
	SELECT @SourceScore=Score, @SourceInsure=InsureScore FROM GameScoreInfo WHERE UserID=@dwUserID

	-- ����ж�
	IF @SourceInsure IS NULL OR @SourceInsure<@lTakeScore
	BEGIN
		-- ��������
		ROLLBACK TRAN
		SET TRANSACTION ISOLATION LEVEL READ COMMITTED

		-- ������Ϣ
		SET @strErrorDescribe=N'����ǰ���չ����Ϸ�����㣬��Ϸ����ȡʧ�ܣ�'
		RETURN 4
	END

	-- ��ѯ����
	DECLARE @LockKindID INT
	DECLARE @LockServerID INT
	SELECT @LockKindID=KindID, @LockServerID=ServerID FROM GameScoreLocker WHERE UserID=@dwUserID

	-- �����ж�
	IF @LockKindID IS NOT NULL AND @LockServerID IS NOT NULL
	BEGIN

		-- ��������
		ROLLBACK TRAN
		SET TRANSACTION ISOLATION LEVEL READ COMMITTED

		-- ��ѯ����
		IF @LockKindID<>0
		BEGIN
			-- ��ѯ��Ϣ
			DECLARE @KindName NVARCHAR(31)
			DECLARE @ServerName NVARCHAR(31)
			SELECT @KindName=KindName FROM QPPlatformDBLink.QPPlatformDB.dbo.GameKindItem WHERE KindID=@LockKindID
			SELECT @ServerName=ServerName FROM QPPlatformDBLink.QPPlatformDB.dbo.GameRoomInfo WHERE ServerID=@LockServerID

			-- ������Ϣ
			IF @KindName IS NULL SET @KindName=N'δ֪��Ϸ'
			IF @ServerName IS NULL SET @ServerName=N'δ֪����'
			SET @strErrorDescribe=N'������ [ '+@KindName+N' ] �� [ '+@ServerName+N' ] ��Ϸ�����У����ܽ��е�ǰ�ı��չ������'
			RETURN 4

		END
		ELSE
		BEGIN
			-- ��ʾ��Ϣ
			SELECT [ErrorDescribe]=N'������ʹ����վҳ����б��չ�������У����ܽ��е�ǰ�ı��չ������'
			RETURN 4
		END
	END

	-- �������
	SET @InsureRevenue=@lTakeScore*1/100
	SET @VariationScore=@lTakeScore-@InsureRevenue
	SET @VariationInsure=-@lTakeScore

	-- ������Ϣ
	SET @strErrorDescribe=N'���չ���ȡ��Ϸ�Ҳ����ɹ�������������˻���Ϣ��'

	-- ��������
	UPDATE GameScoreInfo SET Score=Score+@VariationScore, InsureScore=InsureScore+@VariationInsure, Revenue=Revenue+@InsureRevenue
	WHERE UserID=@dwUserID

	-- ��¼��־

	-- ��������
	COMMIT TRAN
	SET TRANSACTION ISOLATION LEVEL READ COMMITTED

	-- ������
	SELECT @dwUserID AS UserID, @SourceScore AS SourceScore, @SourceInsure AS SourceInsure, @VariationScore AS VariationScore,
		@VariationInsure AS VariationInsure, @InsureRevenue AS InsureRevenue

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- ��ѯ����
CREATE PROC GSP_GR_QueryUserInsureInfo
	@dwUserID INT,								-- �û� I D
	@strClientIP NVARCHAR(15),					-- ���ӵ�ַ
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ������Ϣ
DECLARE @Score BIGINT
DECLARE @Insure BIGINT
DECLARE @ServerID SMALLINT
DECLARE @InsurePass NVARCHAR(32)

-- ִ���߼�
BEGIN

	-- ��ѯ����
	SELECT @ServerID=ServerID FROM GameScoreLocker(NOLOCK) WHERE UserID=@dwUserID

	-- ��ѯ����
	SELECT @Score=Score, @Insure=InsureScore FROM GameScoreInfo(NOLOCK) WHERE UserID=@dwUserID

	-- �����ѯ
	SELECT @InsurePass=InsurePass FROM QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo WHERE UserID=@dwUserID

	-- ���ݵ���
	IF @Score IS NULL SET @Score=0
	IF @Insure IS NULL SET @Insure=0
	IF @ServerID IS NULL SET @ServerID=0
	IF @InsurePass IS NULL SET @InsurePass=N''

	-- ������
	SELECT @dwUserID AS UserID, @Score AS Score, @Insure AS Insure, @ServerID AS ServerID, @InsurePass AS InsurePass

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------


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

-- ��ѯ����
CREATE PROC GSP_GP_QueryUserIndividual
	@dwUserID INT,								-- �û� I D
	@strClientIP NVARCHAR(15)					-- ���ӵ�ַ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- ��������
	DECLARE @UserID INT
	DECLARE @QQ NVARCHAR(16)
	DECLARE @EMail NVARCHAR(33)
	DECLARE @UserNote NVARCHAR(256)
	DECLARE @SeatPhone NVARCHAR(32)
	DECLARE @MobilePhone NVARCHAR(16)
	DECLARE @Compellation NVARCHAR(16)
	DECLARE @DwellingPlace NVARCHAR(128)

	-- ��ѯ�û�
	SELECT @UserID=UserID, @QQ=QQ, @EMail=EMail, @UserNote=UserNote, @SeatPhone=SeatPhone,
		@MobilePhone=MobilePhone, @Compellation=Compellation, @DwellingPlace=DwellingPlace
	FROM IndividualDatum(NOLOCK) WHERE UserID=@dwUserID

	-- ��ѯ�û�
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

	-- �����Ϣ
	SELECT @dwUserID AS UserID, @Compellation AS Compellation, @QQ AS QQ, @EMail AS EMail, @SeatPhone AS SeatPhone,
		@MobilePhone AS MobilePhone, @DwellingPlace AS DwellingPlace, @UserNote AS UserNote

	RETURN 0

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- ��������
CREATE PROC GSP_GP_ModifyUserIndividual
	@dwUserID INT,								-- �û� I D
	@strPassword NCHAR(32),						-- �û�����
	@cbGender TINYINT,							-- �û��Ա�
	@strNickName NVARCHAR(32),					-- �û��ǳ�
	@strUnderWrite NVARCHAR(63),				-- ����ǩ��
	@strCompellation NVARCHAR(16),				-- ��ʵ����
	@strQQ NVARCHAR(16),						-- Q Q ����
	@strEMail NVARCHAR(33),						-- �����ʵ�
	@strSeatPhone NVARCHAR(32),					-- �̶��绰
	@strMobilePhone NVARCHAR(16),				-- �ƶ��绰
	@strDwellingPlace NVARCHAR(128),			-- ��ϸ��ַ
	@strUserNote NVARCHAR(256),					-- �û���ע				
	@strClientIP NVARCHAR(15),					-- ���ӵ�ַ
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- ��������
	DECLARE @UserID INT
	DECLARE @Nullity BIT
	DECLARE @StunDown BIT
	DECLARE @LogonPass AS NCHAR(32)

	-- ��ѯ�û�
	SELECT @UserID=UserID, @LogonPass=LogonPass, @Nullity=Nullity, @StunDown=StunDown
	FROM AccountsInfo(NOLOCK) WHERE UserID=@dwUserID

	-- ��ѯ�û�
	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'�����ʺŲ����ڻ������������������֤���ٴγ��ԣ�'
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
	
	-- �����ж�
	IF @LogonPass<>@strPassword
	BEGIN
		SET @strErrorDescribe=N'�����ʺŲ����ڻ������������������֤���ٴγ��Ե�¼��'
		RETURN 3
	END

	-- Ч���ǳ�
	IF (SELECT COUNT(*) FROM ConfineContent(NOLOCK) WHERE CHARINDEX(String,@strNickName)>0)>0
	BEGIN
		SET @strErrorDescribe=N'�����������Ϸ�ǳ������������ַ�����������ǳ������ٴ��޸ģ�'
		RETURN 4
	END

	-- �����ж�
	IF EXISTS (SELECT UserID FROM AccountsInfo(NOLOCK) WHERE NickName=@strNickName AND UserID<>@dwUserID)
	BEGIN
		SET @strErrorDescribe=N'���ǳ��ѱ��������ʹ���ˣ�������ǳ������ٴ��޸ģ�'
		RETURN 4
	END

	-- �޸�����
	UPDATE AccountsInfo SET NickName=@strNickName, UnderWrite=@strUnderWrite, Gender=@cbGender WHERE UserID=@dwUserID

	-- �޸�����
	UPDATE IndividualDatum SET Compellation=@strCompellation, QQ=@strQQ, EMail=@strEMail, SeatPhone=@strSeatPhone,
		MobilePhone=@strMobilePhone, DwellingPlace=@strDwellingPlace, UserNote=@strUserNote WHERE UserID=@dwUserID
		
	-- �޸�����
	IF @@ROWCOUNT=0
	INSERT IndividualDatum (UserID, Compellation, QQ, EMail, SeatPhone, MobilePhone, DwellingPlace, UserNote)
	VALUES (@dwUserID, @strCompellation, @strQQ, @strEMail, @strSeatPhone, @strMobilePhone, @strDwellingPlace, @strUserNote)

	-- ������Ϣ
	IF @@ERROR=0 SET @strErrorDescribe=N'�������Ѿ����������������ϣ�'

	RETURN 0

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

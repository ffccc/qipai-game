

----------------------------------------------------------------------------------------------------

USE QPTreasureDB
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_PurchaseProp]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_PurchaseProp]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_LoadUserProp]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_LoadUserProp]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_WriteUserProp]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_WriteUserProp]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_LoadFlowerCate]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_LoadFlowerCate]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_LoadGameShopCate]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_LoadGameShopCate]
GO

----------------------------------------------------------------------------------------------------
-- �������
CREATE PROC GSP_GR_PurchaseProp
	@dwSendUserID INT,				--������ID
	@dwRcvUserID INT,				--������ID		
	@dwKindID INT,					--���ͱ�ʶ
	@dwServerID INT,				--�����ʶ
	@dwCateID INT,					--�������
	@dwCurCount BIGINT,				--��ǰ��Ŀ
	@dwOnceCount BIGINT,			--������Ŀ
	@dwPachurseCount BIGINT,		--������Ŀ
	@bwSpendScore BIGINT=0,			--����۸�
	@strClientIP	NVARCHAR(15) 	--�����ַ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ��Ա����
DECLARE @CurMemberOrder INT
DECLARE @MaxMemberOrder INT
DECLARE @MemberOverDate DATETIME
DECLARE @MemberCount INT
DECLARE @UpMemberOverDate DATETIME

-- ��������
DECLARE @EffectRows INT

-- ִ���߼�
BEGIN
	INSERT RecordPurchaseProp(SendUserID,RcvUserID,KindID,ServerID,CateID,CurCount,OnceCount,PachurseCount,SpendScore,ClientIP)
    VALUES(@dwSendUserID,@dwRcvUserID,	@dwKindID,@dwServerID,@dwCateID,@dwCurCount,@dwOnceCount,@dwPachurseCount,@bwSpendScore,@strClientIP)

	-- ���ͻ�Ա
	IF @dwCateID>=7
	BEGIN
		---------------------------== Ϊ�û��󶨻�Ա��Ϣ begin ==-----------------------------
		SET @CurMemberOrder=@dwCateID-7+1

		--------------------------------------------------------------
		-- ��������
		SELECT @MemberCount=COUNT(UserID)
		FROM QPGameUserDBLink.QPGameUserDB.dbo.MemberInfo WHERE UserID=@dwRcvUserID
		
		-- �״ι���
		IF @MemberCount IS NULL OR @MemberCount<=0
		BEGIN
			INSERT QPGameUserDBLink.QPGameUserDB.dbo.MemberInfo (UserID,MemberOrder,MemberOverDate)
			VALUES (@dwRcvUserID,@CurMemberOrder,DATEADD(ss, @dwOnceCount*@dwPachurseCount, GETDATE()))
		END
		ELSE 
		BEGIN	
			-- �ϼ���Ա
			SELECT @UpMemberOverDate=MAX(MemberOverDate)
			FROM QPGameUserDBLink.QPGameUserDB.dbo.MemberInfo WHERE UserID=@dwRcvUserID AND MemberOrder>=@CurMemberOrder		

			-- ���ϼ���Ա
			IF @UpMemberOverDate IS NULL SET @UpMemberOverDate=GETDATE()	 
			IF @MemberOverDate IS NULL SET @MemberOverDate=GETDATE()
			
			-- �ϼ���Ա����ʱ��С
			IF @UpMemberOverDate<@MemberOverDate SET @UpMemberOverDate=@MemberOverDate
			
			-- ����ͬ�����Ա
			UPDATE QPGameUserDBLink.QPGameUserDB.dbo.MemberInfo SET MemberOverDate=DATEADD(ss, @dwOnceCount*@dwPachurseCount, @UpMemberOverDate)
			WHERE UserID=@dwRcvUserID AND MemberOrder=@CurMemberOrder

			SELECT @EffectRows=@@ROWCOUNT
			IF @EffectRows=0
			BEGIN
				INSERT QPGameUserDBLink.QPGameUserDB.dbo.MemberInfo (UserID,MemberOrder,MemberOverDate)
				VALUES (@dwRcvUserID,@CurMemberOrder,DATEADD(ss, @dwOnceCount*@dwPachurseCount, @UpMemberOverDate))
			END		

			-- �����¼���Ա
			UPDATE QPGameUserDBLink.QPGameUserDB.dbo.MemberInfo SET MemberOverDate=DATEADD(ss, @dwOnceCount*@dwPachurseCount, MemberOverDate)
			WHERE UserID=@dwRcvUserID AND MemberOrder<@CurMemberOrder
		END		
		--------------------------------------------------------------

		-- �󶨻�Ա,(��Ա�������л�ʱ��)
		SELECT @MaxMemberOrder=MAX(MemberOrder),@MemberOverDate=MAX(MemberOverDate)
		FROM QPGameUserDBLink.QPGameUserDB.dbo.MemberInfo WHERE UserID=@dwRcvUserID	

		-- ���ӻ�Ա��Ϣ
		UPDATE QPGameUserDBLink.QPGameUserDB.dbo.AccountsInfo
		SET MemberOrder=@MaxMemberOrder,MemberOverDate=@MemberOverDate
		WHERE UserID=@dwRcvUserID
		---------------------------== Ϊ�û��󶨻�Ա��Ϣ end ==-------------------------------
	END
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- �û�����
CREATE PROC GSP_GR_LoadUserProp
	@dwUserID INT,				-- ��ұ�ʶ	
	@dwKindID INT				-- ���ͱ�ʶ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ��Ա����
DECLARE @MemberOrder INT
DECLARE @MemberOverDate DATETIME
DECLARE @MemberOverSec INT
DECLARE @PropCount BIGINT

DECLARE @MemberCount INT
DECLARE @MaxMemberOrder INT
DECLARE @MaxMemberOverDate DATETIME
DECLARE @CurMemberOverDate DATETIME

-- ִ���߼�
BEGIN
	SELECT @MemberOrder=MemberOrder, @MemberOverDate=MemberOverDate
	FROM QPGameUserDBLink.QPGameUserDB.dbo.AccountsInfo WHERE UserID=@dwUserID

	-- ��Ա�ȼ�
	IF GETDATE()<@MemberOverDate
	BEGIN		
		DECLARE @tbMember TABLE (UserID INT, CateID INT, KINDID INT, PropCount BIGINT)

		SELECT @MemberCount=COUNT(UserID),@MaxMemberOrder=MAX(MemberOrder) 
		FROM QPGameUserDBLink.QPGameUserDB.dbo.MemberInfo WHERE UserID=@dwUserID	
		IF @MemberCount IS NULL SET @MemberCount=0
		SET @MaxMemberOverDate=GETDATE()

		WHILE (@MemberCount>0)
		BEGIN
			SELECT @CurMemberOverDate=MemberOverDate
			FROM QPGameUserDBLink.QPGameUserDB.dbo.MemberInfo
			WHERE UserID=@dwUserID AND MemberOrder=@MaxMemberOrder

			-- ��Ա��ʶ��7 ��ʼ @MemberOrder+6
			INSERT @tbMember 
			SELECT @dwUserID,@MaxMemberOrder+6,0,DATEDIFF(ss,@MaxMemberOverDate,@CurMemberOverDate)			

			SELECT @MaxMemberOrder=MAX(MemberOrder) 
			FROM QPGameUserDBLink.QPGameUserDB.dbo.MemberInfo WHERE UserID=@dwUserID AND MemberOrder<@MaxMemberOrder
			SET @MaxMemberOverDate=@CurMemberOverDate

			SET @MemberCount=@MemberCount-1
		END

		DELETE FROM @tbMember WHERE PropCount<=0
	END
	
	-- ȫ������
	IF @dwKindID=0
		-- ��Ա�ȼ�
		IF GETDATE()<@MemberOverDate
		BEGIN
			SELECT UserID,CateID,KindID,PropCount FROM GameUserShelf WHERE UserID=@dwUserID AND UsedFlag=0
			UNION
			SELECT UserID,CateID,KindID,PropCount FROM @tbMember
		END
		ELSE
		BEGIN
			SELECT UserID,CateID,KindID,PropCount FROM GameUserShelf WHERE UserID=@dwUserID AND UsedFlag=0
		END
	ELSE
		-- ��Ա�ȼ�
		IF GETDATE()<@MemberOverDate
		BEGIN
			SELECT UserID,CateID,KindID,PropCount FROM GameUserShelf WHERE UserID=@dwUserID AND UsedFlag=0 AND (KindID=@dwKindID OR KindID=0)
			UNION
			SELECT UserID,CateID,KindID,PropCount FROM @tbMember
		END
		ELSE
		BEGIN
			SELECT UserID,CateID,KindID,PropCount FROM GameUserShelf WHERE UserID=@dwUserID AND UsedFlag=0 AND (KindID=@dwKindID OR KindID=0)
		END
END

RETURN 0
GO

----------------------------------------------------------------------------------------------------

-- д�����
CREATE PROC GSP_GR_WriteUserProp	
	@dwUserID INT,					-- ��ұ�ʶ	
	@dwKindID INT,					-- ���ͱ�ʶ
	@dwCateID INT,					-- �������
	@dwPropCount BIGINT,			-- ʹ����Ŀ
	@strClientIP NVARCHAR(15) 		-- ��ҵ�ַ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN
	--
	IF @dwPropCount<=0
	BEGIN
		DELETE FROM GameUserShelf WHERE UserID=@dwUserID AND KindID=@dwKindID AND CateID=@dwCateID
	END
	ELSE
	BEGIN
		-- ������Ŀ����
		UPDATE GameUserShelf SET PropCount=@dwPropCount,UsedFlag=0,ClientIP=@strClientIP WHERE UserID=@dwUserID AND KindID=@dwKindID AND CateID=@dwCateID

		IF @@ROWCOUNT=0
		BEGIN
			INSERT GameUserShelf(UserID,CateID,KindID,PropCount,ClientIP) 
			VALUES(@dwUserID,@dwCateID,@dwKindID,@dwPropCount,@strClientIP)
		END	
	END

	-- д��¼
	INSERT RecordUserProp (UserID,KindID,CateID,PropCount,ClientIP)
	VALUES (@dwUserID,@dwKindID,@dwCateID,@dwPropCount,@strClientIP)
END

RETURN 0
GO

----------------------------------------------------------------------------------------------------
-- �����ʻ�
CREATE PROC GSP_GR_LoadFlowerCate
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

DECLARE @FlowerCount INT

-- ִ���߼�
BEGIN
	SELECT [ID],[Name],Price,SendUserCharm,RcvUserCharm,Discount FROM FlowerCate WHERE Nullity=0

	SELECT @FlowerCount=@@ROWCOUNT
END

RETURN @FlowerCount
GO

----------------------------------------------------------------------------------------------------
-- ���ص���
CREATE PROC GSP_GR_LoadGameShopCate	
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

DECLARE @PropCount INT

-- ִ���߼�
BEGIN
	SELECT [CateID],[CateName],[PropCount1],[PropCount2],[PropCount3],[PropCount4],[PropCount5],[PropCount6],[PropCount7],[PropCount8],[PropCount9],[PropCount10]
      ,[Price1],[Price2],[Price3],[Price4],[Price5],[Price6],[Price7],[Price8],[Price9],[Price10],[Discount],[CateNote]
  FROM [GameShopCate] WHERE  Nullity=0

	SELECT @PropCount=@@ROWCOUNT
END

RETURN @PropCount
GO
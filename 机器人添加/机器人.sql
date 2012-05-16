use QPTreasureDB
go

--delete from AndroidUserInfo
--go

declare @KindID int
declare @ServerID int
declare @charts int
declare @UserID int
declare @SpreaderID int

set @KindID=102
set @ServerID=6102
set @charts=42

select @SpreaderID=UserID
from QPGameUserDBLink.QPGameUserDB.dbo.AccountsInfo 
where Accounts=N'zhou2848'

while @charts>0
begin

set @charts=@charts-1

select @UserID=UserID
from QPGameUserDBLink.QPGameUserDB.dbo.AccountsInfo
where RegAccounts<>N'0' and SpreaderID=@SpreaderID

insert into AndroidUserInfo(UserID,KindID,ServerID) VALUES (@UserID,@KindID,@ServerID)

update QPGameUserDBLink.QPGameUserDB.dbo.AccountsInfo set RegAccounts=N'0' WHERE UserID=@UserID

end

--insert into AndroidUserInfo(UserID,KindID,ServerID)
--select top(@charts) UserID,@KindID,@ServerID
--from QPGameUserDBLink.QPGameUserDB.dbo.AccountsInfo 
--where userid not in (
--select UserID
--from AndroidUserInfo
--)
--order by UserID desc


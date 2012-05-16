USE QPTreasureDB
GO

-- �ʻ�
TRUNCATE TABLE [FlowerCate]
GO

INSERT [FlowerCate] ([ID],[Name],[Price],[SendUserCharm],[RcvUserCharm],[Discount],[Nullity]) VALUES ( 0,'����',100,0,1,90,0)
INSERT [FlowerCate] ([ID],[Name],[Price],[SendUserCharm],[RcvUserCharm],[Discount],[Nullity]) VALUES ( 1,'����',100,0,1,90,0)
INSERT [FlowerCate] ([ID],[Name],[Price],[SendUserCharm],[RcvUserCharm],[Discount],[Nullity]) VALUES ( 2,'�ʻ�',100,0,1,90,0)
INSERT [FlowerCate] ([ID],[Name],[Price],[SendUserCharm],[RcvUserCharm],[Discount],[Nullity]) VALUES ( 3,'ơ��',100,0,1,90,0)
INSERT [FlowerCate] ([ID],[Name],[Price],[SendUserCharm],[RcvUserCharm],[Discount],[Nullity]) VALUES ( 4,'����',100,0,1,90,0)
INSERT [FlowerCate] ([ID],[Name],[Price],[SendUserCharm],[RcvUserCharm],[Discount],[Nullity]) VALUES ( 5,'���',100,0,1,90,0)
INSERT [FlowerCate] ([ID],[Name],[Price],[SendUserCharm],[RcvUserCharm],[Discount],[Nullity]) VALUES ( 6,'�γ�',100,0,1,90,0)
INSERT [FlowerCate] ([ID],[Name],[Price],[SendUserCharm],[RcvUserCharm],[Discount],[Nullity]) VALUES ( 7,'����',100,0,1,90,0)
INSERT [FlowerCate] ([ID],[Name],[Price],[SendUserCharm],[RcvUserCharm],[Discount],[Nullity]) VALUES ( 8,'����',100,0,-1,90,0)
INSERT [FlowerCate] ([ID],[Name],[Price],[SendUserCharm],[RcvUserCharm],[Discount],[Nullity]) VALUES ( 9,'שͷ',100,0,-1,90,0)
INSERT [FlowerCate] ([ID],[Name],[Price],[SendUserCharm],[RcvUserCharm],[Discount],[Nullity]) VALUES ( 10,'ը��',100,0,-1,90,0)

GO

-- ����

Truncate TABLE GameShopCate
GO

INSERT [GameShopCate] ([CateID],[CateName],[PropCount1],[PropCount2],[PropCount3],[PropCount4],[PropCount5],[PropCount6],[PropCount7],[PropCount8],[PropCount9],[PropCount10],[Price1],[Price2],[Price3],[Price4],[Price5],[Price6],[Price7],[Price8],[Price9],[Price10],[Discount],[Nullity]) VALUES ( 0,'˫�����ֿ�',60,120,0,0,0,0,0,0,0,0,100,200,0,0,0,0,0,0,0,0,90,0)
INSERT [GameShopCate] ([CateID],[CateName],[PropCount1],[PropCount2],[PropCount3],[PropCount4],[PropCount5],[PropCount6],[PropCount7],[PropCount8],[PropCount9],[PropCount10],[Price1],[Price2],[Price3],[Price4],[Price5],[Price6],[Price7],[Price8],[Price9],[Price10],[Discount],[Nullity]) VALUES ( 1,'�ı����ֿ�',60,120,0,0,0,0,0,0,0,0,100,200,0,0,0,0,0,0,0,0,90,0)
INSERT [GameShopCate] ([CateID],[CateName],[PropCount1],[PropCount2],[PropCount3],[PropCount4],[PropCount5],[PropCount6],[PropCount7],[PropCount8],[PropCount9],[PropCount10],[Price1],[Price2],[Price3],[Price4],[Price5],[Price6],[Price7],[Price8],[Price9],[Price10],[Discount],[Nullity]) VALUES ( 2,'��������',1,0,0,0,0,0,0,0,0,0,100,200,0,0,0,0,0,0,0,0,90,0)
INSERT [GameShopCate] ([CateID],[CateName],[PropCount1],[PropCount2],[PropCount3],[PropCount4],[PropCount5],[PropCount6],[PropCount7],[PropCount8],[PropCount9],[PropCount10],[Price1],[Price2],[Price3],[Price4],[Price5],[Price6],[Price7],[Price8],[Price9],[Price10],[Discount],[Nullity]) VALUES ( 3,'��������',1,0,0,0,0,0,0,0,0,0,100,200,0,0,0,0,0,0,0,0,90,0)
INSERT [GameShopCate] ([CateID],[CateName],[PropCount1],[PropCount2],[PropCount3],[PropCount4],[PropCount5],[PropCount6],[PropCount7],[PropCount8],[PropCount9],[PropCount10],[Price1],[Price2],[Price3],[Price4],[Price5],[Price6],[Price7],[Price8],[Price9],[Price10],[Discount],[Nullity]) VALUES ( 4,'С����',60,120,0,0,0,0,0,0,0,0,100,200,0,0,0,0,0,0,0,0,90,0)
INSERT [GameShopCate] ([CateID],[CateName],[PropCount1],[PropCount2],[PropCount3],[PropCount4],[PropCount5],[PropCount6],[PropCount7],[PropCount8],[PropCount9],[PropCount10],[Price1],[Price2],[Price3],[Price4],[Price5],[Price6],[Price7],[Price8],[Price9],[Price10],[Discount],[Nullity]) VALUES ( 5,'���߿�',60,120,0,0,0,0,0,0,0,0,100,200,0,0,0,0,0,0,0,0,90,0)
INSERT [GameShopCate] ([CateID],[CateName],[PropCount1],[PropCount2],[PropCount3],[PropCount4],[PropCount5],[PropCount6],[PropCount7],[PropCount8],[PropCount9],[PropCount10],[Price1],[Price2],[Price3],[Price4],[Price5],[Price6],[Price7],[Price8],[Price9],[Price10],[Discount],[Nullity]) VALUES ( 6,'�����',60,120,0,0,0,0,0,0,0,0,100,200,0,0,0,0,0,0,0,0,90,0)
INSERT [GameShopCate] ([CateID],[CateName],[PropCount1],[PropCount2],[PropCount3],[PropCount4],[PropCount5],[PropCount6],[PropCount7],[PropCount8],[PropCount9],[PropCount10],[Price1],[Price2],[Price3],[Price4],[Price5],[Price6],[Price7],[Price8],[Price9],[Price10],[Discount],[Nullity]) VALUES ( 7,'�����Ա��',2592000,5184000,0,0,0,0,0,0,0,0,1000,2000,0,0,0,0,0,0,0,0,90,0)
INSERT [GameShopCate] ([CateID],[CateName],[PropCount1],[PropCount2],[PropCount3],[PropCount4],[PropCount5],[PropCount6],[PropCount7],[PropCount8],[PropCount9],[PropCount10],[Price1],[Price2],[Price3],[Price4],[Price5],[Price6],[Price7],[Price8],[Price9],[Price10],[Discount],[Nullity]) VALUES ( 8,'�����Ա��',60,120,0,0,0,0,0,0,0,0,100,200,0,0,0,0,0,0,0,0,90,0)
INSERT [GameShopCate] ([CateID],[CateName],[PropCount1],[PropCount2],[PropCount3],[PropCount4],[PropCount5],[PropCount6],[PropCount7],[PropCount8],[PropCount9],[PropCount10],[Price1],[Price2],[Price3],[Price4],[Price5],[Price6],[Price7],[Price8],[Price9],[Price10],[Discount],[Nullity]) VALUES ( 9,'�����Ա��',60,120,0,0,0,0,0,0,0,0,100,200,0,0,0,0,0,0,0,0,90,0)
INSERT [GameShopCate] ([CateID],[CateName],[PropCount1],[PropCount2],[PropCount3],[PropCount4],[PropCount5],[PropCount6],[PropCount7],[PropCount8],[PropCount9],[PropCount10],[Price1],[Price2],[Price3],[Price4],[Price5],[Price6],[Price7],[Price8],[Price9],[Price10],[Discount],[Nullity]) VALUES ( 10,'�����Ա��',60,120,0,0,0,0,0,0,0,0,100,200,0,0,0,0,0,0,0,0,90,0)

GO
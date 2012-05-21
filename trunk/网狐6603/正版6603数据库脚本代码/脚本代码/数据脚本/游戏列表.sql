


----------------------------------------------------------------------------------------------------

USE QPPlatformDB
GO

----------------------------------------------------------------------------------------------------

-- ɾ������
DELETE GameTypeItem
DELETE GameKindItem
DELETE GameNodeItem
GO

----------------------------------------------------------------------------------------------------

-- ��������
INSERT [GameTypeItem] ([TypeID],[JoinID],[SortID],[TypeName],[Nullity]) VALUES ( 1,0,100,'�Ƹ���Ϸ',0)
INSERT [GameTypeItem] ([TypeID],[JoinID],[SortID],[TypeName],[Nullity]) VALUES ( 2,0,200,'������Ϸ',0)
INSERT [GameTypeItem] ([TypeID],[JoinID],[SortID],[TypeName],[Nullity]) VALUES ( 3,0,300,'�˿���Ϸ',0)
INSERT [GameTypeItem] ([TypeID],[JoinID],[SortID],[TypeName],[Nullity]) VALUES ( 4,0,400,'�齫��Ϸ',0)
INSERT [GameTypeItem] ([TypeID],[JoinID],[SortID],[TypeName],[Nullity]) VALUES ( 5,0,500,'������Ϸ',0)
INSERT [GameTypeItem] ([TypeID],[JoinID],[SortID],[TypeName],[Nullity]) VALUES ( 6,0,600,'������Ϸ',0)
INSERT [GameTypeItem] ([TypeID],[JoinID],[SortID],[TypeName],[Nullity]) VALUES ( 7,0,700,'�ط���Ϸ',0)


----------------------------------------------------------------------------------------------------

INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 1,1,1,0,100,'��ť���','ShowHandAN.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 2,2,1,0,100,'��ť����','HKFiveCardAN.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 3,3,1,0,100,'�����˿�','DZShowHand.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 4,4,1,0,100,'����ţţ','OxEx.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 5,5,1,0,100,'ţţ','Ox.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 6,6,1,0,100,'թ��','ZaJinHua.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 7,7,1,0,100,'ʮ����','Thirteen.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 8,8,1,0,100,'���˸�','28Gang.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 9,9,1,0,100,'��Ͳ��','TuiTongZi.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 10,10,1,0,100,'��񶷵���','LandCrazy.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 11,11,1,0,100,'����','CheXuan.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 12,12,1,0,100,'���','RedNine.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 13,13,1,0,100,'ҡ����','Liarsdice.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 14,14,1,0,100,'�ƾ�','PaiJiu.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 15,15,1,0,100,'ʮ���','TenHalfPoint.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 16,16,1,0,100,'21 ��','BlackJack.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 17,17,1,0,100,'����','TianDaKeng.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 18,18,1,0,100,'����','FourCard.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 19,19,1,0,100,'�������','ShowHandANEx.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 22,22,1,0,100,'������������','HKFiveCardSZEX.EXE',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 100,100,2,0,100,'�ټ���','Baccarat.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 101,101,2,0,100,'���˵�˫','DanShuangBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 102,102,2,0,100,'�������','ShowHandBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 103,103,2,0,100,'ʮ����Ф','ZodiacBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 104,104,2,0,100,'����ţţ','OxBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 105,105,2,0,100,'����','RedNineBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 106,106,2,0,100,'�����ƾ�','PaiJiuBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 107,107,2,0,100,'����������','LongHuDouBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 108,108,2,0,100,'������','BumperCarBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 109,109,2,0,100,'����С��','NineXiaoBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 110,110,2,0,100,'���˱�ʮ','BieShiBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 111,111,2,0,100,'��������','DiceBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 200,200,3,0,100,'������','Land.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 201,201,3,0,100,'����������','Land2.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 202,202,3,0,100,'���˶�����','LandEx.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 203,203,3,0,100,'��������','UpGrade2.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 204,204,3,0,100,'��������','UpGrade3.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 205,205,3,0,100,'�ķ�����','UpGrade4.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 206,206,3,0,100,'��������','UpGradeGX.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 207,207,3,0,100,'�������','BiaoFen.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 208,208,3,0,100,'����˫��','ShuangKouEx.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 209,209,3,0,100,'˫��','ShuangKou.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 210,210,3,0,100,'ǧ��˫��','ShuangKouQB.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 211,211,3,0,100,'�ٱ�˫��','ShuangKouBB.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 212,212,3,0,100,'����','GuanPaiBG.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 213,213,3,0,100,'����','GuanPai.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 214,214,3,0,100,'�����','ChuDaDi.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 215,215,3,0,100,'����','GongZhu.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 216,216,3,0,100,'�ڿ�','WaKeng.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 217,217,3,0,100,'510K','510K.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 218,218,3,0,100,'����','HongWu.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 219,219,3,0,100,'��������','HongWu2.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 220,220,3,0,100,'��������','HongWu3.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 221,221,3,0,100,'����һ','SanDaYi.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 222,222,3,0,100,'��Ǩ�走','GuanDanSQ.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 223,223,3,0,100,'�����走','GuanDanHA.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 224,224,3,0,100,'���������','SanDaHaHN.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 225,225,3,0,100,'��ʮ','RedTen.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 300,300,4,0,100,'�����齫','SparrowDZ.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 301,301,4,0,100,'�����齫','SparrowGB.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 302,302,4,0,100,'Ѫս�齫','SparrowXZ.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 303,303,4,0,100,'��ɳ�齫','SparrowCS.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 304,304,4,0,100,'�㽭�齫','SparrowZJ.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 305,305,4,0,100,'�����齫','SparrowCC.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 306,306,4,0,100,'�����齫','SparrowER.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 307,307,4,0,100,'�����齫','SparrowWZ.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 308,308,4,0,100,'���ݶ����齫','SparrowWZEX.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 309,309,4,0,100,'�����齫','SparrowNH.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 400,400,5,0,100,'�й�����','ChinaChessVD.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 401,401,5,0,100,'������','GoBang.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 402,402,5,0,100,'�Ĺ�����','FourEnsign.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 403,403,5,0,100,'Χ��','Rock.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 404,404,5,0,100,'����','Round.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 500,500,6,0,100,'������','LLShow.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 501,501,6,0,100,'������','Plane.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 600,600,7,0,100,'����ţ��','OxYW.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 601,601,7,0,100,'�����ܵÿ�','RunFastHN.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 602,602,7,0,100,'�ܺ���','PaoHuZi.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 603,603,7,0,100,'������ʮ','RedTenDB.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 604,604,7,0,100,'��Դ��ʮ','RedTenLY.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 605,605,7,0,100,'������ʮ','RedTenDD.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 606,606,7,0,100,'�˫��','ShuangKouFH.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 607,607,7,0,100,'����������','LandLB.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 608,608,7,0,100,'����������','WuLangTui.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 609,609,7,0,100,'��Դ����','PaoYao.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 610,610,7,0,100,'����ʮ','SanQiShi.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 611,611,7,0,100,'����','DaZong.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 612,612,7,0,100,'�Ͼ��ܵÿ�','RunFastNJ.exe',0)

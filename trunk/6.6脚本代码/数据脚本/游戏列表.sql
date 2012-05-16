
----------------------------------------------------------------------------------------------------

USE QPServerInfoDB
GO

----------------------------------------------------------------------------------------------------

-- 删除数据
DELETE GameTypeItem
DELETE GameKindItem
DELETE GameNodeItem
GO

----------------------------------------------------------------------------------------------------

-- 类型数据
INSERT GameTypeItem (TypeID, TypeName, SortID, Nullity) VALUES ( 1, '财富游戏',100, 0)
INSERT GameTypeItem (TypeID, TypeName, SortID, Nullity) VALUES ( 2, '牌类游戏',200, 0)
INSERT GameTypeItem (TypeID, TypeName, SortID, Nullity) VALUES ( 3, '棋类游戏',300, 0)
INSERT GameTypeItem (TypeID, TypeName, SortID, Nullity) VALUES ( 4, '麻将游戏',400, 0)
INSERT GameTypeItem (TypeID, TypeName, SortID, Nullity) VALUES ( 5, '休闲游戏',500, 0)
INSERT GameTypeItem (TypeID, TypeName, SortID, Nullity) VALUES ( 6, '视频游戏',600, 0)

----------------------------------------------------------------------------------------------------

-- 财富游戏
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 4, 1, '百家乐', 'Baccarat.exe', 67078, 100, 0, 'QPTreasureDB')
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 10, 1, '21 点', 'BlackJack.exe', 67078, 100, 0, 'QPTreasureDB')
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 25, 1, '摇骰子', 'Liarsdice.exe', 67078, 100, 0, 'QPTreasureDB')
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 26, 1, '两张', 'RedNine100.exe', 67078, 100, 0, 'QPTreasureDB')
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 28, 1, '百人牛牛', 'Ox100.exe', 67078, 100, 0, 'QPTreasureDB')
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 29, 1, '百变梭哈', 'ShowHand100.exe', 67078, 100, 0, 'QPTreasureDB')

-- 牌类游戏
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 111, 2, '千变双扣', 'ShuangKouQB.exe', 67078, 100, 0, 'QPShuangKouQBDB')

-- 棋类游戏
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 202, 3, '四国军棋', 'FourEnsign.exe', 67078, 100, 0, 'QPFourEnsignDB')

-- 麻将游戏
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 305, 4, '浙江麻将', 'SparrowZJ.exe', 67078, 100, 0, 'QPSparrowZJDB')

-- 休闲游戏
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 400, 5, '连连看', 'LLShow.exe', 67078, 100, 0, 'QPLLShowDB')

----视频类
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 522, 6, '疯狂斗地主', 'LandCrazyVD.exe', 67078, 100, 0, 'QPLandCrazyDB')

INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 2, 1, '同花顺（港式五张）', 'HKFiveCard.exe', 67078, 0, 0, 'QPTreasureDB')

----------------------------------------------------------------------------------------------------

GO
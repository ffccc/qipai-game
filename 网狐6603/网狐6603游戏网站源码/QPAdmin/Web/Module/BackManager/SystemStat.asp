<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../conn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title><%=GetQPAdminSiteName() %></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <style type="text/css">
        .gamelist{width: 650px;}
        .gamelist span{float:left;width:200px;height:23px;line-height:19px;	text-align:left;margin-top: 2px;margin-right: 0;margin-bottom: 2px;	margin-left: 0;}
    </style>
</head>
<%
    If CxGame.GetRoleValue("502",trim(session("AdminName")))<"1" Then
    response.redirect "/Index.asp"   
    End If
%>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：后台系统 - 系统统计</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <% 
        Dim rs
    %>
    <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
        <tr>
            <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">用户统计</div></td>
        </tr>
        <tr>
            <td class="listTdLeft">在线人数：</td>
            <td>
                <% 
                    Call ConnectGame(QPPlatformDB)
                        set rs = GameConn.Execute("select Top 1 OnLineCountSum from OnLineStreamInfo(nolock) order by InsertDateTime desc")
                        If rs.BOF and rs.EOF Then
                            response.Write "0 人"
                        Else
                            response.Write rs(0) & " 人"
                        End If                        
                        rs.close
                        set rs=nothing
                    Call CloseGame()
                %>      
            </td>
        </tr>
        <tr>
            <td class="listTdLeft">停权用户：</td>
            <td>
                <% 
                    Call ConnectGame(QPAccountsDB)
                        Set rs = GameConn.Execute("select count(UserID) from AccountsInfo(nolock) where Nullity = 1")
                        response.Write rs(0) & " 人"
                        rs.close
                        set rs=nothing
                    Call CloseGame()
                %>      
            </td>
        </tr>
        <tr>
            <td class="listTdLeft">注册总人数：</td>
            <td>
                <% 
                    Call ConnectGame(QPAccountsDB)
                        Set rs = GameConn.Execute("select count(UserID) from AccountsInfo(nolock)")
                        response.Write rs(0) & " 人"
                        rs.close
                        set rs=nothing                        
                    Call CloseGame()
                %>            
            </td>
        </tr>
        <tr>
            <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">金币统计</div></td>
        </tr>
        <% 
            Call ConnectGame(QPTreasureDB)
                Dim goldSum,insureSum,allSum
                Set rs = GameConn.Execute("select sum(Score),sum(InsureScore),sum(Score+InsureScore) from GameScoreInfo(nolock)")
                IF IsNull(rs(0)) Then
                    goldSum = 0
                Else
                    goldSum=rs(0)
                End If
                IF IsNull(rs(1)) Then
                    insureSum = 0
                Else
                    insureSum=rs(1)
                End If
                IF IsNull(rs(2)) Then
                    allSum = 0
                Else
                    allSum=rs(2)
                End If              
                
                rs.close
                set rs=nothing  
            Call CloseGame()
        %>
        <tr>
            <td class="listTdLeft">金币总量：</td>
            <td>
                <%=FormatNumber(goldSum,0,-1) %> 
            </td>
        </tr>
        <tr>
            <td class="listTdLeft">银行总量：</td>
            <td>
                <%=FormatNumber(insureSum,0,-1) %>     
            </td>
        </tr>
        <tr>
            <td class="listTdLeft">银行+金币总量：</td>
            <td>
                <%=FormatNumber(allSum,0,-1) %>     
            </td>
        </tr>        
        <tr>
            <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">魅力统计</div></td>
        </tr>
        <% 
            Call ConnectGame(QPAccountsDB)
                Dim LoveLiness,Present,RemainLove
                Set rs = GameConn.Execute("select sum(LoveLiness),sum(Present) from AccountsInfo(nolock)")
                If IsNull(rs(0)) Then
                    LoveLiness=0
                    Present=0
                    RemainLove=0
                Else
                    LoveLiness = rs(0)
                    Present = rs(1)
                    RemainLove = LoveLiness-Present
                End If 
                rs.close
                set rs=nothing                        
            Call CloseGame()
        %>        
        <tr>
            <td class="listTdLeft">魅力总量：</td>
            <td>
                 <%=LoveLiness %>
            </td>
        </tr>
        <tr>
            <td class="listTdLeft">已兑换魅力总量：</td>
            <td>
                 <%=Present %>
            </td>
        </tr>
        <tr>
            <td class="listTdLeft">未兑换魅力总量：</td>
            <td>
                 <%=RemainLove %>
            </td>
        </tr>
        <tr>
            <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">税收统计</div></td>
        </tr>
        <tr>
            <td class="listTdLeft">税收总量：</td>
            <td>
                <% 
                    Call ConnectGame(QPTreasureDB)
                        Set rs = GameConn.Execute("select sum(Revenue) from GameScoreInfo(nolock)")
                        If IsNull(rs(0)) Then
                            response.Write FormatNumber(0,0,-1)
                        Else
                            response.Write FormatNumber(rs(0),0,-1)
                        End If 
                        rs.close
                        set rs=nothing                        
                    Call CloseGame()
                %>                
            </td>
        </tr>
        <tr>
            <td class="listTdLeft">转账税收：</td>
            <td>
                <% 
                    Call ConnectGame(QPRecordDB)
                        Set rs = GameConn.Execute("select Sum(Revenue) from RecordInsure(nolock)")
                        If IsNull(rs(0)) Then
                            response.Write FormatNumber(0,0,-1)
                        Else
                            response.Write FormatNumber(rs(0),0,-1)
                        End If 
                        rs.close
                        set rs=nothing                        
                    Call CloseGame()
                %>                
            </td>
        </tr>
        <tr>
            <td class="listTdLeft">游戏税收：</td>
            <td>
                <div class="gamelist">                    
                    <% 
                        Call ConnectGame(QPTreasureDB)
                            Set rs = GameConn.Execute("select KindID,Sum(Revenue) from RecordUserInout(nolock) group by KindID")
                            If Not IsNull(rs) Then                                 
                            do until rs.eof
                            response.Write  "<span>"&CxGame.GetKindNameByKindID(rs(0))&"("&FormatNumber(rs(1),0,-1)&")</span>"    
                            rs.movenext
                            loop
                            End If
                        Call CloseGame()
                    %>
                </div>               
            </td>
        </tr>
        <tr style="padding-top:20px">
            <td class="listTdLeft">房间税收：</td>
            <td>
                <div class="gamelist">                    
                    <% 
                        Call ConnectGame(QPTreasureDB)
                            Set rs = GameConn.Execute("select ServerID,Sum(Revenue) from RecordUserInout(nolock) group by ServerID")
                            If Not IsNull(rs) Then                                
                            do until rs.eof
                            response.Write  "<span>"&CxGame.GetRoomNameByRoomID(rs(0))&"("&FormatNumber(rs(1),0,-1)&")</span>"    
                            rs.movenext
                            loop
                            End If
                        Call CloseGame()
                    %>
                </div>      
            </td>
        </tr>
        <tr>
            <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">点卡统计</div></td>
        </tr>
        <% 
            Call ConnectGame(QPTreasureDB)
                Dim cardAllCount,cardAllGold,cardAllPrice
                Set rs = GameConn.Execute("select Count(CardID),sum(CardGold),sum(CardPrice) from LivcardAssociator(nolock)")
                If IsNull(rs(0)) Then
                    cardAllCount=0
                Else
                    cardAllCount = rs(0)
                End If 
                If IsNull(rs(1)) Then
                    cardAllGold=0
                Else
                    cardAllGold = rs(1)
                End If 
                If IsNull(rs(2)) Then
                    cardAllPrice=0
                Else
                    cardAllPrice = rs(2)
                End If 
                rs.close
                set rs=nothing  
            Call CloseGame()
        %>
        <tr>
            <td class="listTdLeft">生成张数：</td>
            <td>
                <%=cardAllCount %> 张 
            </td>
        </tr>
        <tr>
            <td class="listTdLeft">金币总量：</td>
            <td>
                <%=FormatNumber(cardAllGold,0,-1) %>       
            </td>
        </tr>
        <tr>
            <td class="listTdLeft">面额总量：</td>
            <td>
                <%=FormatNumber(cardAllPrice,0,-1) %>       
            </td>
        </tr>
        <% 
            Call ConnectGame(QPTreasureDB)
                Dim cardUsedCount,cardUsedGold,cardUsedPrice
                Set rs = GameConn.Execute("select Count(CardID),sum(CardGold),sum(CardPrice) from LivcardAssociator(nolock) where ApplyDate is not null")
                If IsNull(rs(0)) Then
                    cardUsedCount=0
                Else
                    cardUsedCount = rs(0)
                End If 
                If IsNull(rs(1)) Then
                    cardUsedGold=0
                Else
                    cardUsedGold = rs(1)
                End If 
                If IsNull(rs(2)) Then
                    cardUsedPrice=0
                Else
                    cardUsedPrice = rs(2)
                End If 
                rs.close
                set rs=nothing  
            Call CloseGame()
        %>
        <tr>
            <td class="listTdLeft">充值张数：</td>
            <td>
                <%=cardUsedCount %> 张      
            </td>
        </tr>
        <tr>
            <td class="listTdLeft">充值金币：</td>
            <td>
                <%=FormatNumber(cardUsedGold,0,-1) %>
            </td>
        </tr>
        <tr>
            <td class="listTdLeft">充值人民币总数：</td>
            <td>
                <%=FormatNumber(cardUsedPrice,0,-1) %>             
            </td>
        </tr>
        <tr>
            <td class="listTdLeft">会员卡张数：</td>
            <td>
                <% 
                    Call ConnectGame(QPTreasureDB)
                        Set rs = GameConn.Execute("select Count(CardID) from LivcardAssociator(nolock) where MemberOrder<>0")
                        response.Write rs(0) &" 张"
                        rs.close
                        set rs=nothing  
                    Call CloseGame()
                %>   
            </td>
        </tr>        
    </table>
</body>
</html>

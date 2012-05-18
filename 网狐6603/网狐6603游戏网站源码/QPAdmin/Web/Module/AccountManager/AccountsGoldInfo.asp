<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../function.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../conn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>“<%=CxGame.GetUserNameByUserID(Request("id")) %>”财富信息</title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">目前操作功能：用户信息</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7">
        <tr>
            <td height="28">
                <ul>
                    <li class="tab2" onclick="openWindowSelf('AccountsInfo.asp?id=<%=Request("id") %>')">基本信息</li>
	                <li class="tab2" onclick="openWindowSelf('AccountsDetailInfo.asp?id=<%=Request("id") %>')">详细信息</li>
	                <li class="tab1">财富信息</li>
	                <li class="tab2" onclick="openWindowSelf('AccountsScoreInfo.asp?id=<%=Request("id") %>')">积分信息</li>
	                <li class="tab2" onclick="openWindowSelf('AccountsRecordInfo.asp?id=<%=Request("id") %>')">记录信息</li>
                </ul>
            </td>
        </tr>
    </table>   
    <% 
        Call ConnectGame(QPTreasureDB)
        Select case lcase(Request("action"))
            case "save"
            Call DeleteLocker()
            case else
            Call Main()
        End Select
        Call CloseGame()
        
        '清理卡线
        Sub DeleteLocker()
            GameConn.execute "delete GameScoreLocker where  UserID='"&Request("id")&"'"
            Call CxGame.MessageBoxReload("清理成功！","?id="&Request("id")) 
        End Sub
        
        Sub Main()
            Dim rs,sql
            Dim score,revenue,insureScore,winCount,lostCount,drawCount,fleeCount,allLogonTimes,playTimeCount,onLineTimeCount
            Dim lastLogonIP,lastLogonDate,lastLogonMachine,registerIP,registerDate,registerMachine
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from GameScoreInfo(nolock) where UserID='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3  
            If Not rs.Bof And Not rs.Eof Then
                score = rs("Score")
                revenue = rs("Revenue")
                insureScore = rs("InsureScore")
                winCount = rs("WinCount")
                lostCount = rs("LostCount")
                drawCount = rs("DrawCount")
                fleeCount = rs("FleeCount")
                allLogonTimes = rs("AllLogonTimes")
                playTimeCount = rs("PlayTimeCount")
                onLineTimeCount = rs("OnLineTimeCount")
                lastLogonIP = rs("LastLogonIP")
                lastLogonDate = rs("LastLogonDate")
                lastLogonMachine = rs("LastLogonMachine")
                registerIP = rs("RegisterIP")
                registerDate = rs("RegisterDate")
                registerMachine = rs("RegisterMachine")
            End If  
            
            Dim rs_1,sql_1
            Set rs_1=Server.CreateObject("Adodb.RecordSet")
            sql_1 = "select * from GameScoreLocker where UserID='"&Request("id")&"'"
            rs_1.Open sql_1,GameConn,1,3  
    %>
    <form name="form1" action='?action=save&id=<%=Request("id") %>' method="post" onsubmit="return confirm('确定要清理卡线吗？')">
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="关闭" class="btn wd1" onclick="window.close()" />                
                </td>
            </tr>
        </table>
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">金币信息</div></td>
            </tr>
            <tr>
	            <td class="listTdLeft">用户名：</td>
	            <td>
	                <span style="color:blue"><%=CxGame.GetUserNameByUserID(Request("id")) %></span>&nbsp;&nbsp;&nbsp;              
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">金币：</td>
	            <td>
	                <span><%=FormatNumber(score,2,-1) %>&nbsp;</span>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">银行金币：</td>
	            <td>
	                <span><%=FormatNumber(insureScore,2,-1) %>&nbsp;</span>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">赢局：</td>
	            <td>
	                <span><%=winCount %>&nbsp;</span>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">输局：</td>
	            <td>
	                <span><%=lostCount %>&nbsp;</span>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">和局：</td>
	            <td>
	                <span><%=drawCount %>&nbsp;</span>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">逃局：</td>
	            <td>
	                <span><%=fleeCount %>&nbsp;</span>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">游戏税收：</td>
	            <td>
	                <span><%=FormatNumber(revenue,2,-1) %>&nbsp;</span>
	            </td>
	        </tr>
	        <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">登录信息</div></td>
            </tr>
            <tr>
	            <td class="listTdLeft">登录次数：</td>
	            <td>
	                <span><%=allLogonTimes %>&nbsp;</span>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">游戏时长(秒)：</td>
	            <td>
	                <span><%=playTimeCount %>&nbsp;</span>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">在线时长(秒)：</td>
	            <td>
	                <span><%=onLineTimeCount %>&nbsp;</span>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">登录时间：</td>
	            <td>
	                <% If lastLogonDate<>"" Then %>
			            <%=lastLogonDate %>&nbsp;&nbsp;<%=Common.FormatDateSpan(lastLogonDate) %>   
			        <% Else%>
			            &nbsp;				                
			        <% End If %>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">登录地址：</td>
	            <td>
	                <% If lastLogonIP<>"" Then %>
			            <%=lastLogonIP %>&nbsp;&nbsp;<%=GetCityFromIP(lastLogonIP) %>        
			        <% Else%>
			            &nbsp;
			        <% End If %>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">登录机器：</td>
	            <td>
	                <span><%=lastLogonMachine %>&nbsp;</span>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">注册时间：</td>
	            <td>
	                <% If registerDate<>"" Then %>
			            <%=registerDate %>&nbsp;&nbsp;<%=Common.FormatDateSpan(registerDate) %>   
			        <% Else%>
			            &nbsp;				                
			        <% End If %>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">注册地址：</td>
	            <td>
	                <% If registerIP<>"" Then %>
			            <%=registerIP %>&nbsp;&nbsp;<%=GetCityFromIP(registerIP) %>        
			        <% Else%>
			            &nbsp;
			        <% End If %>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">注册机器：</td>
	            <td>
	                <span><%=registerMachine %>&nbsp;</span>
	            </td>
	        </tr>	        
        </table>
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">卡线情况</div></td>
            </tr>
            <tr>
                <td colspan="2">
                    <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class=" box Tmg7 sortable">
                        <tr class="bold">
                            <th class="listTitle1">时间</th>      
                            <th class="listTitle1">游戏</th>                
                            <th class="listTitle2">房间</th>             
                            <th class="listTitle2">进入地址</th>             
                            <th class="listTitle2">进入机器</th>                                
                        </tr>
                        <% 
                            If rs_1.EOF or rs_1.BOF Then
                                Response.Write("<tr><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
                            Else                                                        
                        %>
                        <tr align="center" class="list">
                            <td>
                                <a class="l" href="javascript:void(0)" onclick="javascript:openWindowOwn('GoldUserInoutInfo.asp?id=<%=rs_1("EnterID") %>',<%=rs_1("EnterID") %>,850,400)"><%=rs_1("CollectDate") %></a>
                            </td>
                            <td><%=CxGame.GetKindNameByKindID(rs_1("KindID")) %></td>
                            <td><%=CxGame.GetRoomNameByRoomID(rs_1("ServerID")) %></td>
                            <td><%=rs_1("EnterIP") %></td>
                            <td><%=rs_1("EnterMachine") %></td>
                        </tr>
                        <% 
                            End If
                        %>
                    </table>
                </td>
            </tr>         
        </table>
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="关闭" class="btn wd1" onclick="window.close()" />      
                    <% 
                        If not rs_1.EOF and not rs_1.BOF Then
                    %>
                    <input type="submit" value="清理卡线" class="btn wd2"/>             
                    <% End If %>
                </td>
            </tr>
        </table>
    </form>
    <% 
        rs.Close()
        Set rs=nothing
        End Sub
    %>
</body>
</html>

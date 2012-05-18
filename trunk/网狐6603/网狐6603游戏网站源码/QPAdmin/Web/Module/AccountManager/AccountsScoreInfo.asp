<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../function.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../conn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>“<%=CxGame.GetUserNameByUserID(Request("id")) %>”积分信息</title>
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
	                <li class="tab2" onclick="openWindowSelf('AccountsGoldInfo.asp?id=<%=Request("id") %>')">财富信息</li>
	                <li class="tab1">积分信息</li>
	                <li class="tab2" onclick="openWindowSelf('AccountsRecordInfo.asp?id=<%=Request("id") %>')">记录信息</li>
                </ul>
            </td>
        </tr>
    </table>   
    <% 
        Dim kindID,dname
        kindID = CxGame.GetInfo(1,"KindID")
        dname="QPGameScoreDB"
        
        IF kindID<=0 Then
            dname="QPGameScoreDB"
        Else
            dname = CxGame.GetKindDBNameByKindID(kindID)
            If IsNull(dname) Or dname="" Then
                dname="QPGameScoreDB"
            End If
        End If  
        
        Call ConnectGame(dname)
            Call Main()
        Call CloseGame()
        
        Sub Main()
            Dim rs,sql
            Dim score,revenue,insureScore,winCount,lostCount,drawCount,fleeCount,allLogonTimes,playTimeCount,onLineTimeCount
            Dim lastLogonIP,lastLogonDate,lastLogonMachine,registerIP,registerDate,registerMachine
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from GameScoreInfo(nolock) where UserID='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3  
            If Not rs.Bof And Not rs.Eof Then
                score = rs("Score")
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
    %>
    <script type="text/javascript">
        function ChangeKind(kindID)
        {
            document.form1.action = "?id=<%=Request("id") %>&KindID="+kindID;
            document.form1.submit();
        }
    </script>
    <form name="form1" action='' method="post">
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
	            <td class="listTdLeft">游戏：</td>
	            <td>
	                <select name="kindID" style="width:100px" onchange="ChangeKind(this.value)">
			        <option value="0">积分</option>
                    <% 
                        Dim ArrayList,i
                        ArrayList = CxGame.GetScoreKindList
                        For i=0 To Ubound(ArrayList) 
                        If Not IsEmpty(ArrayList(i,0)) Then
                    %>
                    <option value="<%=ArrayList(i,0) %>"<%If CxGame.GetInfo(1,"kindID")= ArrayList(i,0) Then %> selected="selected" <%End If %>><%=ArrayList(i,1) %></option>           
                    <% 
                        End If
                        Next  
                        Set ArrayList = nothing
                    %>         
                    </select>                    
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">用户名：</td>
	            <td>
	                <span style="color:blue"><%=CxGame.GetUserNameByUserID(Request("id")) %></span>&nbsp;&nbsp;&nbsp;
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">积分：</td>
	            <td>
	                <span><%=FormatNumber(score,2,-1) %>&nbsp;</span>
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
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="关闭" class="btn wd1" onclick="window.close()" />                
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

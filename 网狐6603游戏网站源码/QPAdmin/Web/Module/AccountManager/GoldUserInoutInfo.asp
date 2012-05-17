<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../function.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../Cls_Page.asp"-->
<!--#include file="../../conn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>“<%=CxGame.GetUserNameByUserID(Request("id")) %>”进出记录</title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script> 
    <script type="text/javascript" src="../../Js/common.js"></script> 
    <script type="text/javascript" src="../../Js/Calendar.js"></script>  
     <script type="text/javascript" src="../../Js/Sort.js"></script>
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">前操作功能：进出记录</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <% 
        Call ConnectGame(QPTreasureDB)
        Call Main()
        Call CloseGame()
        
        Sub Main()
            Dim rs,sql
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from RecordUserInout(nolock) where ID='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3  
    %>
    <form name="myformList" method="post" action="">
        <div id="content">
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box">
                <tr align="center" class="bold">
                    <th class="listTitle1">进入时间</th>
                    <th class="listTitle2">进入地址</th>
                    <th class="listTitle2">进入机器</th>
                    <th class="listTitle2">游戏</th>
                    <th class="listTitle2">房间</th>
                    <th class="listTitle2">积分</th>
                    <th class="listTitle2">奖牌</th>
                    <th class="listTitle2">魅力</th>
                    <th class="listTitle2">离开时间</th>
                    <th class="listTitle2">离开地址</th>
                    <th class="listTitle2">离开原因</th>
                    <th class="listTitle2">输赢积分</th>
                    <th class="listTitle2">魅力变更</th>
                    <th class="listTitle2">经验变更</th>
                    <th class="listTitle2">税收</th>
                    <th class="listTitle2">游戏时长(秒)</th>
                    <th class="listTitle2">在线时长(秒)</th>
                    <th class="listTitle2">总局</th>
                    <th class="listTitle2">赢局</th>
                    <th class="listTitle2">输局</th>
                    <th class="listTitle2">和局</th>
                    <th class="listTitle">逃局</th>
                </tr>
                <% 
                    If rs.EOF or rs.BOF Then
                        Response.Write("<tr class='tdbg'><td colspan='100' align='left'><br>没有任何信息!<br><br></td></tr>")
                    Else
                %>
                <tr align="center" class="list">
                    <td><%=rs("EnterTime") %></td>
                    <td><%=rs("EnterClientIP") %></td>
                    <td><%=rs("EnterMachine") %></td>
                    <td><%=CxGame.GetKindNameByKindID(rs("KindID")) %></td>
                    <td><%=CxGame.GetRoomNameByRoomID(rs("ServerID")) %></td>
                    <td><%=rs("EnterScore") %></td>
                    <td><%=rs("EnterUserMedal") %></td>
                    <td><%=rs("EnterLoveliness") %></td>
                    <% If IsNull(rs("LeaveTime")) Then %>
                    <td colspan="100" align="left">正在游戏中</td>   
                    <% Else %>
                    <td><%=rs("LeaveTime") %></td>     
                    <td><%=rs("LeaveClientIP") %></td>     
                    <td>
                        <% If rs("LeaveReason")=0 Then %>
                        常规离开
                        <% Elseif rs("LeaveReason")=1 Then %>
                        系统原因
                        <% Elseif rs("LeaveReason")=2 Then %>
                        用户冲突
                        <% Elseif rs("LeaveReason")=3 Then %>
                        网络原因
                        <% Else %>
                        人满为患
                        <% End If %>
                    </td>  
                    <td><%=rs("Score") %></td> 
                    <td><%=rs("LoveLiness") %></td> 
                    <td><%=rs("Experience") %></td> 
                    <td><%=rs("Revenue") %></td> 
                    <td><%=rs("PlayTimeCount") %></td>
                    <td><%=rs("OnLineTimeCount") %></td>   
                    <td><%=rs("WinCount")+rs("LostCount")+rs("DrawCount")+rs("FleeCount") %></td>  
                    <td><%=rs("WinCount") %></td>
                    <td><%=rs("LostCount") %></td>
                    <td><%=rs("DrawCount") %></td>
                    <td><%=rs("FleeCount") %></td>
                    <% End If %>          
                </tr>
                <% 
                    End If
                %>
            </table>
        </div> 
    </form>
    <% 
        rs.Close()
        Set rs=nothing
        End Sub
    %>
</body>
</html>

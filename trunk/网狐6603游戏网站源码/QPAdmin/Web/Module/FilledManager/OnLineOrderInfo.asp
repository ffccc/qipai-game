<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../conn.asp"-->
<!--#include file="../../function.asp"-->
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">目前操作功能：订单信息</td>
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
            sql = "select * from OnLineOrder(nolock) where OrderID='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3   
    %>
    <form name="form1" method="post" action=''>
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="关闭" class="btn wd1" onclick="window.close()" />                
                </td>
            </tr>
        </table>
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
                <td class="listTdLeft">订单号码：</td>
                <td>
                    <%=rs("OrderID") %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">订单日期：</td>
                <td>
                    <%=rs("ApplyDate") %>&nbsp;&nbsp;<%=Common.FormatDateSpan(rs("ApplyDate")) %>             
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">用户名(游戏ID)：</td>
                <td>
                    <%=rs("Accounts") %>(<%=rs("GameID") %>)      
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">卡名称：</td>
                <td>
                    <%=CxGame.GetCardNameByCardID(rs("CardTypeID")) %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">卡价格：</td>
                <td>
                    <%=FormatNumber(rs("CardPrice")) %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">订单数量：</td>
                <td>
                    <%=rs("CardTotal") %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">订单金额：</td>
                <td>
                    <%=FormatNumber(rs("OrderAmount")) %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">实付金额：</td>
                <td>
                    <%=FormatNumber(rs("PayAmount")) %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">充值地址：</td>
                <td>
                    <%=rs("IPAddress") %>&nbsp;&nbsp;<%=GetCityFromIP(rs("IPAddress")) %>               
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">操作用户：</td>
                <td>
                    <%=CxGame.GetUserNameByUserID(rs("OperUserID")) %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">订单状态：</td>
                <td>
                    <% If rs("OrderStatus") = 0 Then %>
                    <span style="color:red">未付款</span>
                    <% Elseif rs("OrderStatus") = 1 Then %>
                    <span style="color:Blue">已付款待处理</span>
                    <% Else %>
                    <span>订单完成</span>
                    <% End If %>
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
        Set rs= Nothing
        End Sub
    %>
</body>
</html>

<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="function.asp"-->
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
            <td width="1232" height="25" valign="top" align="left">目前操作功能：快钱返回信息</td>
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
            sql = "select * from ReturnKQDetailInfo(nolock) where DetailID='"&Request("id")&"'"
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
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">基本信息</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">订单号码：</td>
                <td>
                    <a class="l" href="javascript:void(0)" onclick="javascript:openWindowOwn('OnLineOrderInfo.asp?id=<%=rs("OrderID") %>','online_<%=rs("OrderID") %>',600,530)"><%=rs("OrderID") %></a>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">订单日期：</td>
                <td>
                    <%=rs("OrderTime") %>&nbsp;&nbsp;<%=Common.FormatDateSpan(rs("OrderTime")) %>             
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">订单金额：</td>
                <td>
                    <%=FormatNumber(rs("OrderAmount"),2,-1) %>         
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">实付金额：</td>
                <td>
                    <%=FormatNumber(rs("PayAmount"),2,-1) %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">手续费：</td>
                <td>
                    <%=FormatNumber(rs("Fee"),2,-1) %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">实收金额：</td>
                <td>
                    <span style="color:red"><%=FormatNumber(CCur(rs("PayAmount"))-CCur(rs("Fee"))) %></span>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">订单状态：</td>
                <td>
                    <% If rs("PayResult") = "10" Then %>
				    <span style="color:Blue">成功</span>
				    <% Else %>
				    <span style="color:red">失败</span>
				    <% End If %>
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">交易信息</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">快钱交易号：</td>
                <td>
                    <%=rs("DealID") %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">交易日期：</td>
                <td>
                    <%=rs("DealTime") %>&nbsp;&nbsp;<%=Common.FormatDateSpan(rs("DealTime")) %>             
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">银行交易号：</td>
                <td>
                    <%=rs("BankDealID") %>           
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">支付方式：</td>
                <td>
                    <%=GetKQPayType(Trim(rs("PayType"))) %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">支付银行：</td>
                <td>
                    <%=GetKQBankID(Trim(rs("BankID"))) %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">银行账户：</td>
                <td>
                    <%=rs("MerchantAcctID") %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">支付错误描述：</td>
                <td>
                    <%=GetKQErrCode(Trim(rs("ErrCode"))) %>
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">其它信息</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">快钱网关版本：</td>
                <td>
                    <%=rs("Version") %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">网页语言：</td>
                <td>
                    中文
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">扩展字段1：</td>
                <td>
                    <%=rs("Ext1") %>&nbsp;&nbsp;<%=GetCityFromIP(rs("Ext1")) %>   
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">扩展字段2：</td>
                <td>
                    <%=rs("Ext2") %>&nbsp;
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">签名信息</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">签名类型：</td>
                <td>
                    与提交订单时的签名类型保持一致
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">MD5 签名后的字符串：</td>
                <td>
                    <%=rs("SignMsg") %>
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

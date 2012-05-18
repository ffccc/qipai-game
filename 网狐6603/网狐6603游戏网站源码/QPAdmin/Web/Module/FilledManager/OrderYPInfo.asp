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
            <td width="1232" height="25" valign="top" align="left">目前操作功能：易宝返回信息</td>
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
            sql = "select * from ReturnYPDetailInfo(nolock) where DetailID="&Request("id")
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
                    <a class="l" href="javascript:void(0)" onclick="javascript:openWindowOwn('OnLineOrderInfo.asp?id=<%=rs("r6_Order") %>','online_<%=rs("r6_Order") %>',600,500)"><%=rs("r6_Order") %></a>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">订单日期：</td>
                <td>
                    <%=rs("CollectDate") %>&nbsp;&nbsp;<%=Common.FormatDateSpan(rs("CollectDate")) %>             
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">业务类型：</td>
                <td>
                    <%=rs("r0_Cmd") %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">支付金额：</td>
                <td>
                    <%=FormatNumber(rs("r3_Amt")) %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">交易币种：</td>
                <td>
                    <%=rs("r4_Cur") %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">支付结果：</td>
                <td>
                    <% If rs("r1_Code") = "1" Then %>
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
                <td class="listTdLeft">易宝钱交易号：</td>
                <td>
                    <%=rs("r2_TrxId") %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">商品名称：</td>
                <td>
                    <%=rs("r5_Pid") %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">易宝支付会员ID：</td>
                <td>
                    <%=rs("r7_Uid") %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">商户扩展信息：</td>
                <td>
                    <%=rs("r8_MP") %>&nbsp;
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">交易结果返回类型：</td>
                <td>
                    <%If rs("r9_BType") = 1 Then %>
				    浏览器重定向
				    <%Else %>
				    服务器点对点通讯
				    <%End If %>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">银行编码：</td>
                <td>
                    <%=rs("rb_BankId") %>&nbsp;
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">银行订单号：</td>
                <td>
                    <%=rs("ro_BankOrderId") %>&nbsp;
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">支付成功时间：</td>
                <td>
                    <%=rs("rp_PayDate") %>&nbsp;
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">神州行充值卡序列号：</td>
                <td>
                    <%=rs("rq_CardNo") %>&nbsp;
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">交易结果通知时间：</td>
                <td>
                    <%=rs("ru_Trxtime") %>&nbsp;
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">交易结果通知时间：</td>
                <td>
                    <%=rs("ru_Trxtime") %>&nbsp;
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">签名信息</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">签名数据：</td>
                <td>
                    <%=rs("hmac") %>
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

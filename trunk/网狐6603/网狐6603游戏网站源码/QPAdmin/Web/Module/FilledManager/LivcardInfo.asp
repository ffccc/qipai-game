<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../conn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>   
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript">
        function Operate(opType,SerialID)
        {
            var opVal = document.form1.in_optype
            if(!confirm("确定要执行选定的操作吗？"))
            {
                return;
            }
            else
            {
                var cid="";
                var cids = GelTags("input");                
                for(var i=0;i<cids.length;i++)
                {
                    if(cids[i].checked)
                        cid+=cids[i].value+",";
                }
                if(cid=="")
                {
                    alert("未选中任何数据");
                    return;
                }
                //操作处理
                if(opType=="jinyong")
                    opVal.value = "jinyong";
                if(opType=="huanyuan")
                    opVal.value = "huanyuan";
            }
            document.form1.action = "?SerialID="+SerialID+"&action="+opVal.value;
            document.form1.submit();
        }
    </script>
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">目前操作功能：实卡信息</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <% 
        Call ConnectGame(QPTreasureDB)
            Call Main()
        Call CloseGame()
        
         '禁用还原
        Sub Nullity(typeValue)
            Dim ID
            ID = lID
            GameConn.execute "Update LivcardAssociator Set Nullity="&typeValue&" Where SerialID='"&CStr(Request("SerialID"))&"'"
        End Sub
        
        Sub Main()
            Dim rs,sql,SerialID,acType
            acType = Request("action")
            Select case acType
                case "jinyong"
                    Call Nullity(1)
                case "huanyuan"
                    Call Nullity(0)
            End Select  
            SerialID=Request("SerialID")
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from LivcardAssociator(nolock) where SerialID='"&Request("SerialID")&"'"
            rs.Open sql,GameConn,1,3   
    %>
    <form name="form1" method="post" action=''>
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="关闭" class="btn wd1" onclick="window.close()" /> 
                    <input class="btnLine" type="button" />   
                    <input class="btn wd1" onclick="Operate('jinyong','<%=SerialID %>')" type="button" value="禁用" />
                    <input type="button" value="还原" class="btn wd1" onclick="Operate('huanyuan','<%=SerialID %>')"/>  
                    <input name="in_optype" type="hidden" />   
                    <input name="in_SerialID" type="hidden" value="<%=SerialID %>" />                                   
                </td>
            </tr>
        </table>
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
        <% If rs.EOF Then %>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">会员卡信息不存在</div></td>
            </tr>
        <% Else %>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">基本信息</div></td>
            </tr>
            <tr>
	            <td class="listTdLeft">实卡卡号：</td>
	            <td>
	                <%=rs("SerialID") %>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">实卡名称：</td>
	            <td>
	                <%=CxGame.GetCardNameByCardID(rs("CardTypeID")) %>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">生产批次：</td>
	            <td>
	                <%=rs("BuildID") %>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">实卡价格：</td>
	            <td>
	                <%=FormatNumber(rs("CardPrice")) %>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">实卡金币：</td>
	            <td>
	                <%=FormatNumber(rs("CardGold")) %>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft" style="height: 19px">会员等级：</td>
	            <td style="height: 19px">
	                <%=CxGame.GetMemberOrderName(rs("MemberOrder")) %>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">会员天数：</td>
	            <td>
	                <%=rs("MemberDays") %>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">使用范围：</td>
	            <td>
	                <%=CxGame.GetCardUseRange(rs("UseRange")) %>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">销售商：</td>
	            <td>
	                <%=rs("SalesPerson") %>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">禁用状态：</td>
	            <td>
	                <%=CxGame.GetNullityInfo(rs("Nullity")) %>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">生成日期：</td>
	            <td>
	                <%=rs("BuildDate") %>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">有效日期：</td>
	            <td>
	                <%=rs("ValidDate") %>
	            </td>
	        </tr>
	        <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">权限信息</div></td>
            </tr>
            <tr>
	            <td class="listTdLeft">用户权限：</td>
	            <td>
	                <table border="0" style="padding:5px 5px 5px 0;" cellpadding="0" cellspacing="0">
                    <tr>
                        <td><input name="in_UserRight" id="in_UserRight1" disabled="disabled" type="checkbox" value="1"<% If rs("UserRight") And 1 Then %> checked='checked'<% End If %> /><label for="in_UserRight1">限制游戏</label></td>           
                        <td><input name="in_UserRight" id="in_UserRight2" disabled="disabled" type="checkbox" value="2"<% If rs("UserRight") And 2 Then %> checked='checked'<% End If %>/><label for="in_UserRight2">限制旁观</label></td>
                        <td><input name="in_UserRight" id="in_UserRight3" disabled="disabled" type="checkbox" value="4"<% If rs("UserRight") And 4 Then %> checked='checked'<% End If %>/><label for="in_UserRight3">限制私聊</label></td>
                        <td><input name="in_UserRight" id="in_UserRight4" disabled="disabled" type="checkbox" value="8"<% If rs("UserRight") And 8 Then %> checked='checked'<% End If %>/><label for="in_UserRight4">限制大厅聊天</label></td>
                        <td><input name="in_UserRight" id="in_UserRight5" disabled="disabled" type="checkbox" value="16"<% If rs("UserRight") And 16 Then %> checked='checked'<% End If %>/><label for="in_UserRight5">限制房间聊天</label></td>
                        <td><input name="in_UserRight" id="in_UserRight6" disabled="disabled" type="checkbox" value="268435456"<% If rs("UserRight") And 268435456 Then %> checked='checked'<% End If %>/><label for="in_UserRight6">限制比赛</label></td>
                    </tr> 
                    </table>   
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">服务权限：</td>
	            <td>
	                <table border="0" style="padding:5px 5px 5px 0;" cellpadding="0" cellspacing="0">
                    <tr>
                        <td><input name="in_ServiceRight" id="in_ServiceRight1" disabled="disabled" type="checkbox" value="1"<% If rs("ServiceRight") And 1 Then %> checked='checked'<% End If %> /><label for="in_ServiceRight1">限制游戏</label></td>           
                        <td><input name="in_ServiceRight" id="in_ServiceRight2" disabled="disabled" type="checkbox" value="2"<% If rs("ServiceRight") And 2 Then %> checked='checked'<% End If %>/><label for="in_ServiceRight2">限制旁观</label></td>
                        <td><input name="in_ServiceRight" id="in_ServiceRight3" disabled="disabled" type="checkbox" value="4"<% If rs("ServiceRight") And 4 Then %> checked='checked'<% End If %>/><label for="in_ServiceRight3">限制私聊</label></td>
                        <td><input name="in_ServiceRight" id="in_ServiceRight4" disabled="disabled" type="checkbox" value="8"<% If rs("ServiceRight") And 8 Then %> checked='checked'<% End If %>/><label for="in_ServiceRight4">限制大厅聊天</label></td>
                        <td><input name="in_ServiceRight" id="in_ServiceRight5" disabled="disabled" type="checkbox" value="16"<% If rs("ServiceRight") And 16 Then %> checked='checked'<% End If %>/><label for="in_ServiceRight5">限制房间聊天</label></td>
                        <td><input name="in_ServiceRight" id="in_ServiceRight6" disabled="disabled" type="checkbox" value="268435456"<% If rs("ServiceRight") And 268435456 Then %> checked='checked'<% End If %>/><label for="in_ServiceRight6">限制比赛</label></td>
                    </tr> 
                    </table>   
	            </td>
	        </tr>
	        <% 
                Dim rs_1,sql_1
                Set rs_1=Server.CreateObject("Adodb.RecordSet")
                sql_1 = "select * from ShareDetialInfo(nolock) where SerialID='"&Request("SerialID")&"'"
                rs_1.Open sql_1,GameConn,1,3 
                If rs_1.EOF Then
            %>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">充值信息不存在</div></td>
            </tr> 
            <% Else%>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">充值信息</div></td>
            </tr> 
            <tr>
	            <td class="listTdLeft">充值日期：</td>
	            <td>
	                <%=rs_1("ApplyDate") %>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">用户名(游戏ID)：</td>
	            <td>
	                <%=rs_1("Accounts") %>(<%=rs_1("GameID") %>)
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">操作用户：</td>
	            <td>
	                <%=CxGame.GetUserNameByUserID(rs_1("OperUserID")) %>&nbsp;
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft" style="height: 19px">充值前金币：</td>
	            <td style="height: 19px">
	                <%=rs_1("BeforeGold") %>
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">充值地址：</td>
	            <td>
	                <%=rs_1("IPAddress") %>&nbsp;&nbsp;<%=GetCityFromIP(rs_1("IPAddress")) %>                         
	            </td>
	        </tr>
	       
	        <% 
                End If
                Set rs_1 = Nothing
            %>
        </table> 
        <table width="99%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="关闭" class="btn wd1" onclick="window.close()" />      
                     <input class="btnLine" type="button" />   
                    <input type="button" value="禁用" class="btn wd1" onclick="Operate('jinyong','<%=SerialID %>')"/>
                    <input type="button" value="还原" class="btn wd1" onclick="Operate('huanyuan','<%=SerialID %>')"/>                           
                </td>
            </tr>
        </table>
    </form>
    <% 
        End If
        Set rs= Nothing
        End Sub
    %>
</body>
</html>

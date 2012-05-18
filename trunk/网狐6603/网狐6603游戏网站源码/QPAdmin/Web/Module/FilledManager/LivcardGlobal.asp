<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../Cls_Page.asp"-->
<!--#include file="../../conn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>   
    <script type="text/javascript" src="../../Js/comm.js"></script> 
    <script type="text/javascript" src="../../Js/Check.js"></script>
     <script type="text/javascript" src="../../Js/Sort.js"></script>   
</head>
<%
    If CxGame.GetRoleValue("200",trim(session("AdminName")))<"1" Then
    response.redirect "/Index.asp"   
    End If
%>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：充值系统 - 实卡管理</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7">
        <tr>
            <td height="28">
                <ul>
                    <li class="tab2" onclick="openWindowSelf('LivcardList.asp')">会员卡管理</li>
	                <li class="tab2" onclick="openWindowSelf('LivcardCreate.asp')">会员卡生成</li>
	                <li class="tab2" onclick="openWindowSelf('LivcardStat.asp')">库存统计</li>
	                <li class="tab1">类型管理</li>
                </ul>
            </td>
        </tr>
    </table> 
    <% 
        Call ConnectGame(QPTreasureDB)
        Select case Request("action") 
            case "cardinfo"
                Call GetCardInfo()  
            case "savecard"
                Call Save()         
            case else
                Call Main()
        End Select        
        Call CloseGame()
        
        Sub Save()
            Dim rs,sql
            Dim UserRightArr,UserRight,ServiceRightArr,ServiceRight,RL_i
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from GlobalLivcard where CardTypeID='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3  
            If rs.eof Then
                rs.addnew
            End If
            rs("CardName") = CxGame.GetInfo(0,"in_CardName")
            rs("CardPrice") = CxGame.GetInfo(0,"in_CardPrice")
            rs("CardGold") = CxGame.GetInfo(1,"in_CardGold")
            rs("MemberOrder") = CxGame.GetInfo(1,"in_MemberOrder")
            rs("MemberDays") = CxGame.GetInfo(1,"in_MemberDays")
            UserRightArr = Split(Request("in_UserRight"),",")
            UserRight =0
            For RL_i=0 To UBound(UserRightArr)
                UserRight = UserRight Or UserRightArr(RL_i)
            Next
            rs("UserRight") = UserRight
            ServiceRightArr = Split(Request("in_ServiceRight"),",")
            ServiceRight =0
            For RL_i=0 To UBound(ServiceRightArr)
                ServiceRight = ServiceRight Or ServiceRightArr(RL_i)
            Next
            rs("ServiceRight") = ServiceRight
            rs.update
            If Request("id")<>"" Then
                Call CxGame.ShowInfo2("修改成功！","LivcardGlobal.asp?page="&Request("page")&"",1200)
            Else
                Call CxGame.ShowInfo2("新增成功！","LivcardGlobal.asp",1200)
            End If
            rs.close()
            Set rs = Nothing  
        End Sub
        
        '删除操作
        Sub Delete(lID)
            Dim ID
            ID = lID
            GameConn.execute "delete GlobalLivcard where CardTypeID='"&ID&"'"            
        End Sub
        
        Sub Main()
            Dim cidArray, lLoop
            cidArray = Split(Request("cid"),",")
            For lLoop=0 To UBound(cidArray)
                Call Delete(cidArray(lLoop))
            Next
            
            Dim rs,nav,Page,i
            Dim lCount, queryCondition, OrderStr
            Dim startDate,endDate          
            If Request.QueryString("orders")<>""And Request.QueryString("OrderType")<>"" Then
                If Request.QueryString("OrderType")<>"-102" Then
                    OrderStr=Replace(Request.QueryString("orders")," ","+")&" Asc "
                End If
                If Request.QueryString("OrderType")<>"-101" Then
                    OrderStr=Replace(Request.QueryString("orders")," ","+")&" Desc "
                End If
            Else 
                OrderStr = "CardTypeID ASC"
            End If
            Dim field
            field = "CardTypeID,CardName,CardPrice,CardGold,MemberOrder,MemberDays"
            
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = GetPageSize					'每页记录条数
                .PageParm = "page"				'页参数
                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	            .Pkey="CardTypeID"					'主键
	            .Field=field	'字段
	            .Table="GlobalLivcard"			'表名
	            .Condition=queryCondition		'条件,不需要where
	            .OrderBy=OrderStr				'排序,不需要order by,需要asc或者desc
	            .RecordCount = 0				'总记录数，可以外部赋值，0不保存（适合搜索），-1存为session，-2存为cookies，-3存为applacation

	            .NumericJump = 5                '数字上下页个数，可选参数，默认为3，负数为跳转个数，0为显示所有
	            .Template = "总记录数：{$RecordCount} 总页数：{$PageCount} 每页记录数：{$PageSize} 当前页数：{$PageIndex} {$FirstPage} {$PreviousPage} {$NumericPage} {$NextPage} {$LastPage} {$InputPage} {$SelectPage}" '整体模板，可选参数，有默认值
	            .FirstPage = "首页"             '可选参数，有默认值
	            .PreviousPage = "上一页"        '可选参数，有默认值
	            .NextPage = "下一页"            '可选参数，有默认值
	            .LastPage = "尾页"              '可选参数，有默认值
	            .NumericPage = " {$PageNum} "   '数字分页部分模板，可选参数，有默认值
            End With
            
            rs = Page.ResultSet()       '记录集
            lCount = Page.RowCount()    '可选，输出总记录数
            nav = Page.Nav()            '分页样式
            
            Page = Null
            Set Page = Nothing
            '==============================================================================================================
    %>
    <script type="text/javascript">
        function CheckFormList()
        {
            if(!confirm("确定要执行选定的操作吗？"))
            {
                return false;
            }
            else
            {
                var cid="";
                var cids = GelTags("input");                
                for(var i=0;i<cids.length;i++)
                {
                    if(cids[i].checked)
                    {
                        if(cids[i].name == "cid")
                            cid+=cids[i].value+",";
                    }
                }       
                if(cid=="")
                {
                    showError("未选中任何数据");
                    return false;
                }
            }
        }
    </script>
    <form name="myformList" method="post" action='' onsubmit="return CheckFormList();">
        <% If Request("Action") = "DeleteAll" Then %>
        <script type="text/javascript">
            showInfo("删除成功")
        </script>
        <% End If %>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td height="39" class="titleOpBg">
	                <input type="button" value="新建" class="btn wd1" onclick="Redirect('?action=cardinfo')" />
                    <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1"/>      
                    <input type="hidden" name="Action" value="DeleteAll" />                 
                </td>
            </tr>
        </table> 
        <div>
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box">
                <tr align="center" class="bold">
                    <th class="listTitle1" width='38' align='center'><input type="checkbox" name="chkAll" onclick="SelectAll(this.checked);" /></th>
                    <th class="listTitle1"><a class="l1"  href="" onclick="GetOrder('CardName',this)">实卡名称</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CardPrice',this)">实卡价格</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CardGold',this)">赠送金币</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('MemberOrder',this)">会员等级</a></th>
                    <th class="listTitle"><a class="l1"  href="" onclick="GetOrder('MemberDays',this)">会员天数</a></th>                    
                </tr>
                <% 
                    Dim className
                    If IsNull(rs) Then
                        Response.Write("<tr class='tdbg'><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
                    Else
                    
                    For i=0 To Ubound(rs,2)
                    If i Mod 2 = 0 Then
                        className="list"        
                    Else
                        className="listBg"      
                    End If 
                %>
                <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='pointer';"
                    onmouseout="this.style.backgroundColor=currentcolor">
                    <td>
                        <%If rs(0,i)>3 Then %>
                        <input name='cid' type='checkbox' value='<%=rs(0,i)%>'/>
                        <%End If %>
                    </td>      
                    <td onclick="Redirect('?action=cardinfo&id=<%=rs(0,i) %>&page=<%=Request("page") %>')"><%=rs(1,i) %></td>  
                    <td onclick="Redirect('?action=cardinfo&id=<%=rs(0,i) %>&page=<%=Request("page") %>')"><%=FormatNumber(rs(2,i),2,-1) %></td>  
                    <td onclick="Redirect('?action=cardinfo&id=<%=rs(0,i) %>&page=<%=Request("page") %>')"><%=FormatNumber(rs(3,i),2,-1) %></td>  
                    <td onclick="Redirect('?action=cardinfo&id=<%=rs(0,i) %>&page=<%=Request("page") %>')"><%=CxGame.GetMemberOrderName(rs(4,i)) %></td>  
                    <td onclick="Redirect('?action=cardinfo&id=<%=rs(0,i) %>&page=<%=Request("page") %>')"><%=rs(5,i) %></td>      
                </tr>
                <% 
                    Next
                    End If
                %>
            </table>
        </div>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="page" align="right"><%Response.Write nav%></td>
            </tr>
        </table> 
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td height="39" class="titleOpBg">
	                <input type="button" value="新建" class="btn wd1" onclick="Redirect('?action=cardinfo')" />
                    <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1"/>              
                </td>
            </tr>
        </table> 
    </form>
    <% 
        End Sub
        
        Sub GetCardInfo()
            Dim rs,sql,titleInfo
            Dim cardTypeID,cardName,cardPrice,cardGold,memberOrder,memberDays,userRight,serviceRight
            Set rs=Server.CreateObject("Adodb.RecordSet")            
            sql = "select * from GlobalLivcard(nolock) where CardTypeID='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3          
            If rs.Bof And rs.Eof Then
                titleInfo = "新增会员卡信息"
            Else
                titleInfo = "修改会员卡信息"
                cardTypeID = rs("CardTypeID")
                cardName = rs("CardName")
                cardPrice = rs("CardPrice")
                cardGold = rs("CardGold")
                memberOrder = rs("MemberOrder")
                memberDays = rs("MemberDays")
                userRight = rs("UserRight")
                serviceRight = rs("ServiceRight")
            End If  
            
    %>
    <script>
        function CheckFormInfo()
        {
            var cardName = document.myFormInfo.in_CardName.value;
            var cardPrice = document.myFormInfo.in_CardPrice.value;
            var cardGold = document.myFormInfo.in_CardGold.value;
            var memberDays= document.myFormInfo.in_MemberDays.value;
            if(len(cardName)>16)
            {
                alert("实卡名称长度不能超过16字符！")
                return false;
            }
            if(IsPositiveInt(cardPrice)==false)
            {
                alert("实卡价格非数字字符或者超过允许大小！");
                return false;
            }
            if(IsPositiveInt(cardGold)==false)
            {
                alert("赠送金币非数字字符或者超过允许大小！");
                return false;
            }
            if(IsPositiveInt(memberDays)==false)
            {
                alert("会员天数非数字字符或者超过允许大小！");
                return false;
            }
        }
    </script>
    <form name="myFormInfo" method="post" action='?action=savecard&id=<%=Request("id") %>&page=<%=Request("page") %>' onsubmit="return CheckFormInfo()" >
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td height="39" class="titleOpBg">
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('LivcardGlobal.asp')" />                           
                    <input class="btnLine" type="button" />  
                    <input type="submit" value="保存" class="btn wd1" />  
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7"><%=titleInfo %></div></td>
            </tr>
            <tr>
	            <td class="listTdLeft">实卡名称：</td>
	            <td>
	                <input name="in_CardName" type="text" class="text" value="<%=cardName %>" />     
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">实卡价格：</td>
	            <td>
	                <input name="in_CardPrice" type="text" class="text" value="<%=cardPrice %>" />                        
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">赠送金币：</td>
	            <td>
	                <input name="in_CardGold" type="text" class="text" value="<%=cardGold %>" />                                          
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">会员等级：</td>
	            <td>
	                <input name="in_MemberOrder" id="in_MemberOrder0" type="radio" value="0"<% If memberOrder=0 Then %> checked="checked"<% End If %> /><label for="in_MemberOrder0">普通会员</label>
                    <input name="in_MemberOrder" id="in_MemberOrder1" type="radio" value="1"<% If memberOrder=1 Then %> checked="checked"<% End If %> /><label for="in_MemberOrder1">蓝钻</label>                             
                    <input name="in_MemberOrder" id="in_MemberOrder2" type="radio" value="2"<% If memberOrder=2 Then %> checked="checked"<% End If %> /><label for="in_MemberOrder2">黄钻</label>
                    <input name="in_MemberOrder" id="in_MemberOrder3" type="radio" value="3"<% If memberOrder=3 Then %> checked="checked"<% End If %> /><label for="in_MemberOrder3">白钻</label>                             
                    <input name="in_MemberOrder" id="in_MemberOrder4" type="radio" value="4"<% If memberOrder=4 Then %> checked="checked"<% End If %> /><label for="in_MemberOrder4">红钻</label>                             
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">会员天数：</td>
	            <td>
	                <input name="in_MemberDays" type="text" class="text" value="<%=memberDays %>" />        
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">用户权限：</td>
	            <td>
	                <table border="0" style="padding:5px 5px 5px 0;" cellpadding="0" cellspacing="0">
                    <tr>
                        <td><input name="in_UserRight" id="in_UserRight1" type="checkbox" value="1"<% If userRight And 1 Then %> checked='checked'<% End If %> /><label for="in_UserRight1">限制游戏</label></td>           
                        <td><input name="in_UserRight" id="in_UserRight2" type="checkbox" value="2"<% If userRight And 2 Then %> checked='checked'<% End If %>/><label for="in_UserRight2">限制旁观</label></td>
                        <td><input name="in_UserRight" id="in_UserRight3" type="checkbox" value="4"<% If userRight And 4 Then %> checked='checked'<% End If %>/><label for="in_UserRight3">限制私聊</label></td>
                        <td><input name="in_UserRight" id="in_UserRight4" type="checkbox" value="8"<% If userRight And 8 Then %> checked='checked'<% End If %>/><label for="in_UserRight4">限制大厅聊天</label></td>
                        <td><input name="in_UserRight" id="in_UserRight5" type="checkbox" value="16"<% If userRight And 16 Then %> checked='checked'<% End If %>/><label for="in_UserRight5">限制房间聊天</label></td>
                        <td><input name="in_UserRight" id="in_UserRight6" type="checkbox" value="268435456"<% If userRight And 268435456 Then %> checked='checked'<% End If %>/><label for="in_UserRight6">限制比赛</label></td>
                    </tr> 
                    </table>   
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">服务权限：</td>
	            <td>
	                <table border="0" style="padding:5px 5px 5px 0;" cellpadding="0" cellspacing="0">
                    <tr>
                        <td><input name="in_ServiceRight" id="in_ServiceRight1" type="checkbox" value="1"<% If serviceRight And 1 Then %> checked='checked'<% End If %> /><label for="in_ServiceRight1">限制游戏</label></td>           
                        <td><input name="in_ServiceRight" id="in_ServiceRight2" type="checkbox" value="2"<% If serviceRight And 2 Then %> checked='checked'<% End If %>/><label for="in_ServiceRight2">限制旁观</label></td>
                        <td><input name="in_ServiceRight" id="in_ServiceRight3" type="checkbox" value="4"<% If serviceRight And 4 Then %> checked='checked'<% End If %>/><label for="in_ServiceRight3">限制私聊</label></td>
                        <td><input name="in_ServiceRight" id="in_ServiceRight4" type="checkbox" value="8"<% If serviceRight And 8 Then %> checked='checked'<% End If %>/><label for="in_ServiceRight4">限制大厅聊天</label></td>
                        <td><input name="in_ServiceRight" id="in_ServiceRight5" type="checkbox" value="16"<% If serviceRight And 16 Then %> checked='checked'<% End If %>/><label for="in_ServiceRight5">限制房间聊天</label></td>
                        <td><input name="in_ServiceRight" id="in_ServiceRight6" type="checkbox" value="268435456"<% If serviceRight And 268435456 Then %> checked='checked'<% End If %>/><label for="in_ServiceRight6">限制比赛</label></td>
                    </tr> 
                    </table>   
	            </td>
	        </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td height="39" class="titleOpBg">
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('LivcardGlobal.asp')" />                           
                    <input class="btnLine" type="button" />  
                    <input type="submit" value="保存" class="btn wd1" />  
                </td>
            </tr>
        </table>        
    </form>
    <% 
        End Sub
    %>
</body>
</html>

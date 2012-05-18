<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../conn.asp"-->
<!--#include file="../../Cls_Page.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script> 
    <script type="text/javascript" src="../../Js/comm.js"></script>   
    <script type="text/javascript" src="../../Js/Calendar.js"></script>  
    <script type="text/javascript" src="../../Js/Check.js"></script>  
    <script type="text/javascript" src="../../Js/Sort.js"></script>
</head>
<%
    If CxGame.GetRoleValue("106",trim(session("AdminName")))<"1" Then
    Response.Redirect "/Index.asp"   
    End If
%>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：游戏用户 - 限制机器</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <form id="form1" action="">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7 titleQueBg">
            <tr>
                <td class="listTdLeft" style="width: 80px">
                    限制机器：
                </td>
                <td>
                    <input name="search" type="text" class="text" value="<%=CxGame.GetInfo(0,"search") %>" />
                    <input type="submit" value="查询" class="btn wd1" />
                </td>
            </tr>
        </table>
    </form>
    <% 
        Call ConnectGame(QPAccountsDB)
        Select case lcase(Request("action"))
            case "confineinfo"
            Call GetConfineInfo()
            case "saveconfineinfo"
            Call SaveConfineInfo()
            case else
            Call Main()
        End Select
        Call CloseGame()
        
        '保存
        Sub SaveConfineInfo()
            Dim rs,sql
            Dim overDate
            overDate = CxGame.GetInfo(0,"in_EnjoinOverDate")
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from ConfineMachine where MachineSerial='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3
            If rs.eof Then
                rs.addnew
                rs("MachineSerial") = CxGame.GetInfo(0,"in_MachineSerial")
                If CxGame.IsExistConfineMachine(Trim(CxGame.GetInfo(0,"in_MachineSerial"))) Then
                    Call CxGame.ShowInfo2(CxGame.GetInfo(0,"in_MachineSerial")&"已经存在！","ConfineMachine.asp?action=confineInfo",1200)
                    Exit Sub
                End If
            End If
            rs("EnjoinLogon") = CxGame.GetInfo(0,"in_EnjoinLogon")
            rs("EnjoinRegister") = CxGame.GetInfo(0,"in_EnjoinRegister")
            If overDate<>"" Then
                rs("EnjoinOverDate") = CDate(overDate)
            Else
                rs("EnjoinOverDate") = null
            End If
            rs("CollectNote") = CxGame.GetInfo(0,"in_CollectNote")
            rs.update
            If Request("id")<>"" Then
                Call CxGame.ShowInfo2("修改成功！","ConfineMachine.asp?page="&Request("page")&"",1200)
            Else
                Call CxGame.ShowInfo2("新增成功！","ConfineMachine.asp",1200)
            End If
            rs.close()
            Set rs = Nothing       
        End Sub 
        
        '删除操作
        Sub Delete(lID)
            Dim ID
            ID = Trim(lID)
            GameConn.execute "delete ConfineMachine where MachineSerial='"&ID&"'"                
        End Sub
        
        Sub Main()
            Dim cidArray, lLoop
            cidArray = Split(Request("cid"),",")
            For lLoop=0 To UBound(cidArray)
                Call Delete(Trim(cidArray(lLoop)))
            Next
            
            Dim rs,nav,Page,i
            Dim lCount, queryCondition, OrderStr
            If Request.QueryString("orders")<>""And Request.QueryString("OrderType")<>"" Then
                If Request.QueryString("OrderType")<>"-102" Then
                    OrderStr=Replace(Request.QueryString("orders")," ","+")&" Asc "
                End If
                If Request.QueryString("OrderType")<>"-101" Then
                    OrderStr=Replace(Request.QueryString("orders")," ","+")&" Desc "
                End If
            Else 
                OrderStr = "CollectDate DESC"
            End If
            '查询条件
            If Request("search")<>"" Then
                queryCondition = "MachineSerial like '%"&CxGame.GetInfo(0,"search")&"%'"
            End If
            
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = GetPageSize					'每页记录条数
                .PageParm = "page"				'页参数
                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	            .Pkey="MachineSerial"					'主键
	            .Field="*"	'字段
	            .Table="ConfineMachine"			'表名
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
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7">
            <tr>
                <td height="39" class="titleOpBg">
                    <%
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("106",trim(session("AdminName")))="2" or CxGame.GetRoleValue("106",trim(session("AdminName")))="3" Then
                    %>
                    <input type="button" value="新建" class="btn wd1" onclick="Redirect('?action=confineInfo')" />
                    <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1" />
                    <%
                        End If
                     %>
                    <input type="hidden" name="Action" value="DeleteAll" />         
                </td>
            </tr>
        </table>
        <div id="content">        
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box">
                <tr class="bold">
                    <th class="listTitle1" width='38' align='center'><input type="checkbox" name="chkAll" onclick="SelectAll(this.checked);" /></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('MachineSerial',this)">限制机器</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('EnjoinLogon',this)">限制登录</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('EnjoinRegister',this)">限制注册</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('EnjoinOverDate',this)">失效时间</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CollectDate',this)">收集时间</a></th>
                    <th class="listTitle"><a class="l1"  href="" onclick="GetOrder('CollectNote',this)">备注</a></th>
                </tr>
                <% 
                    Dim MachineSerial,className
                    If IsNull(rs) Then
                        Response.Write("<tr class='tdbg'><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
                    Else
                    
                    For i=0 To Ubound(rs,2)
                    MachineSerial = rs(0,i) 
                    If i Mod 2 = 0 Then
                        className="list"        
                    Else
                        className="listBg"      
                    End If                
                %>
                <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='pointer';"
                    onmouseout="this.style.backgroundColor=currentcolor">
                    <td><input name='cid' type='checkbox' value='<%=MachineSerial%>'/></td>                  
                    <td onclick="Redirect('ConfineMachine.asp?action=confineInfo&id=<%=MachineSerial %>&page=<%=Request("page") %>')"><%=rs(0,i) %></td>    
                    <td onclick="Redirect('ConfineMachine.asp?action=confineInfo&id=<%=MachineSerial %>&page=<%=Request("page") %>')">
                        <% If rs(1,i) = 0 Then%>
                        正常
                        <% Else %>
                        禁止
                        <% End If %>
                    </td>  
                    <td onclick="Redirect('ConfineMachine.asp?action=confineInfo&id=<%=MachineSerial %>&page=<%=Request("page") %>')">
                        <% If rs(2,i) = 0 Then%>
                        正常
                        <% Else %>
                        禁止
                        <% End If %>
                    </td>  
                    <td onclick="Redirect('ConfineMachine.asp?action=confineInfo&id=<%=MachineSerial %>&page=<%=Request("page") %>')">
                        <% If IsNull(rs(3,i)) Then%>
                        永久限制
                        <% Else %>
                        <%=rs(3,i)%>
                        <% End If %>
                    </td>  
                    <td onclick="Redirect('ConfineMachine.asp?action=confineInfo&id=<%=MachineSerial %>&page=<%=Request("page") %>')"><%=rs(4,i) %></td>    
                    <td onclick="Redirect('ConfineMachine.asp?action=confineInfo&id=<%=MachineSerial %>&page=<%=Request("page") %>')"><%=rs(5,i) %></td>                
                </tr>
                <% 
                    Next
                    End If
                %>
            </table>
        </div>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="listTitleBg"><span>选择：</span>&nbsp;<a class="l1" href="javascript:SelectAll(true);">全部</a>&nbsp;-&nbsp;<a class="l1" href="javascript:SelectAll(false);">无</a></td>
                <td class="page" align="right"><%Response.Write nav%></td>
            </tr>
        </table>   
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" id="OpList">
            <tr>
                <td height="39" class="titleOpBg">
                    <%
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("106",trim(session("AdminName")))="2" or CxGame.GetRoleValue("106",trim(session("AdminName")))="3" Then
                    %>
                    <input type="button" value="新建" class="btn wd1" onclick="Redirect('?action=confineInfo')" />
                    <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1" />
                    <%
                        End If
                     %>
                </td>
            </tr>
        </table>
    </form>
    <% 
        End Sub
        
        Sub GetConfineInfo()
            Dim rs,sql,titleInfo
            Dim machineSerial,enjoinLogon,enjoinRegister,enjoinOverDate,collectDate,collectNote
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from ConfineMachine where MachineSerial='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3
            If rs.Bof And rs.Eof Then
                titleInfo = "新增限制机器码信息"
            Else
                titleInfo = "修改限制机器码信息"
                machineSerial = rs("MachineSerial")
                enjoinLogon = rs("EnjoinLogon")
                enjoinRegister = rs("EnjoinRegister")
                enjoinOverDate = rs("EnjoinOverDate")
                collectDate = rs("CollectDate")
                collectNote = rs("CollectNote")
            End If
    %>
    <br />
    <script type="text/javascript">
        function CheckFormInfo()
        {
            var machineSerial = document.myFormInfo.in_MachineSerial.value;
            var enjoinOverDate = document.myFormInfo.in_EnjoinOverDate.value;
           
            if(machineSerial.trim()=="")
            {
                alert("机器不能为空！");
                return false;
            } 
            if(len(machineSerial)>33)
            {
                alert("机器字符长度不能超过31个字符！");
                return false;
            }           
            if(enjoinOverDate.trim()!=""&&!isDate(enjoinOverDate.trim()))
            {
                alert("日期格式不正确！");
                return false;
            }  
        }
    </script>
    <form name="myFormInfo" method="post" action='?action=saveconfineinfo&id=<%=Request("id") %>&page=<%=Request("page") %>' onsubmit="return CheckFormInfo()">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td height="39" class="titleOpBg">
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('ConfineMachine.asp')" />                               
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
	            <td class="listTdLeft">机器：</td>
	            <td>
	                <% If machineSerial<>"" Then %>               
                    <span><%=machineSerial %></span>
                    <input name="in_MachineSerial" type="text" value="<%=machineSerial %>" style="display:none;" /> 
                    <% Else%>
                    <input name="in_MachineSerial" type="text" class="text" value="<%=machineSerial %>" />                    
                    <% End If%>  
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">用户登录：</td>
	            <td>
	                <input name="in_EnjoinLogon" id="in_EnjoinLogon0" type="radio" value="False"<% If enjoinLogon=False Then %> checked="checked"<% End If %> /><label for="in_EnjoinLogon0">正常</label>
                    <input name="in_EnjoinLogon" id="in_EnjoinLogon1" type="radio" value="True"<% If enjoinLogon=True Then %> checked="checked"<% End If %> /><label for="in_EnjoinLogon1">冻结</label>                                                 
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">用户注册：</td>
	            <td>
	                <input name="in_EnjoinRegister" id="in_EnjoinRegister1" type="radio" value="False"<% If enjoinRegister=False Then %> checked="checked"<% End If %> /><label for="in_EnjoinRegister1">正常</label>
                    <input name="in_EnjoinRegister" id="in_EnjoinRegister2" type="radio" value="True"<% If enjoinRegister=True Then %> checked="checked"<% End If %> /><label for="in_EnjoinRegister2">冻结</label>       
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">失效时间：</td>
	            <td>
	                <input name="in_EnjoinOverDate" type="text" class="text" value="<%=enjoinOverDate %>" /><img src="../../Images/btn_calendar.gif" onclick="calendar(in_EnjoinOverDate)" style="cursor:pointer;vertical-align:middle"/>                    
	            </td>
	        </tr>
	          <tr>
	            <td class="listTdLeft">
	                警告：
	            </td >
	            <td>
	            不填写默认永久限制
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">备注：</td>
	            <td>
	                <input name="in_CollectNote" type="text" class="text" maxlength="32" style="width:500px;" value="<%=collectNote %>" />
	            </td>
	        </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td height="39" class="titleOpBg">
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('ConfineMachine.asp')" />                               
                    <input class="btnLine" type="button" />  
                    <input type="submit" value="保存" class="btn wd1" /> 
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

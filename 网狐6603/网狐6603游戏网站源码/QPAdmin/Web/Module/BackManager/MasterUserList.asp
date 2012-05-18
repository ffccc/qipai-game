<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../Cls_Page.asp"-->
<!--#include file="../../Conn.asp"-->
<!--#include file="../../md5.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
     <script type="text/javascript" src="../../Js/Sort.js"></script>
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
</head>
<%
    If CxGame.GetRoleValue("500",trim(session("AdminName")))<"1" Then
    response.redirect "/Index.asp"   
    End If
%>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：后台系统 - 用户管理</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->   
    <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7">
        <tr>
            <td height="28">
                <ul>
                    <li class="tab2" onclick="openWindowSelf('UserOrderList.asp')">角色管理</li>
	                <li class="tab1">用户管理</li>
                </ul>
            </td>
        </tr>
    </table>    
    <% 
        Call ConnectGame(QPPlatformManagerDB)
        Select case lcase(Request("action"))
            case "userinfo"
            Call GetUserInfo()
            case "saveuserinfo"
            Call SaveUserInfo()
            case "del"
            Call DeleteUserInfo()
            case "savesiteinfo"
            Call SaveSiteInfo()
            case else
            Call Main()
        End Select
        
        '保存
        Sub SaveUserInfo()
            Dim rs,sql,id,aa
            Set rs=Server.CreateObject("Adodb.RecordSet")
            If Request("id")="" Then
                id = 0
                rs=GameConn.execute(" BaseUser_OP  '"&CxGame.GetInfo(0,"in_UserName")&"','"&md5(CxGame.GetInfo(0,"in_Pwd"),32)&"',"&Request("in_Role")&",'Insert'")
                id=rs(0)
            Else
                id = Request("id")
                sql="select *  from  Base_Users where  userid="&id
                Response.Write id
                rs.Open sql,GameConn,1,3
                rs("Username") = CxGame.GetInfo(0,"in_UserName")
                rs("Password") = md5(CxGame.GetInfo(0,"in_Pwd"),32)
                rs("RoleID") = Request("in_Role")
                rs.update
            End If           
            If id<>"" Then
                CxGame.InsertSystemSecurity("修改密码")
                Call CxGame.ShowInfo2("更新成功！","MasterUserList.asp",1200)
            Else
                Call CxGame.ShowInfo2("新增成功！","MasterUserList.asp",1200)
            End If
            Set rs = Nothing     
        End Sub
        
        '删除
        Sub DeleteUserInfo(lID)
            Dim rs,sql,ID
            ID = Trim(lID)
            GameConn.execute "delete from Base_Users where UserID="&ID    
            Call CxGame.ShowInfo2("删除成功！","MasterUserList.asp",1200)         
        End Sub
        
        Sub Main()
            Dim cidArray, lLoop
            cidArray = Split(Request("cid"),",")
            For lLoop=0 To UBound(cidArray)
                Call DeleteUserInfo(cidArray(lLoop))
            Next 
            Dim nav,Page,i
            Dim lCount, queryCondition
            Dim rs,sql,id,className,rowNum,OrderStr
            If Request.QueryString("orders")<>""And Request.QueryString("OrderType")<>"" Then
                If Request.QueryString("OrderType")<>"-102" Then
                    OrderStr=Replace(Request.QueryString("orders")," ","+")&" Asc "
                End If
                If Request.QueryString("OrderType")<>"-101" Then
                    OrderStr=Replace(Request.QueryString("orders")," ","+")&" Desc "
                End If
            Else 
                OrderStr = "username Asc"
            End If
            Dim fields        
            fields = "UserID,username,PreLogintime,PreLoginIP,LastLogintime,LastLoginIP,LoginTimes,RoleID"     
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = 20				'每页记录条数
                .PageParm = "page"				'页参数
                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	            .Pkey="UserID"					'主键
	            .Field=fields	'字段
	            .Table="Base_Users"			'表名
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
    <form name="myformList" method="post" action=''>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td height="39" class="titleOpBg">
                    <%
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("500",trim(session("AdminName")))="2" or CxGame.GetRoleValue("500",trim(session("AdminName")))="3" Then
                    %>
                    <input type="button" value="新增" class="btn wd1" onclick="Redirect('?action=userInfo')" />
                    <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1" />     
                    <input type="hidden" name="Action" value="DeleteAll" />      
                    <%
                        End If
                     %> 
                </td>
            </tr>
        </table> 
        <div id="content">        
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box">
                <tr align="center" class="bold">
                    <td class="listTitle1" style="width:38px;"><input type="checkbox" name="chkAll" onclick="SelectAll(this.checked);" /></td>
                    <th class="listTitle1" style="width:100px;"><a class="l1"  href="" onclick="GetOrder('username',this)">用户帐号</a></th>
                    <th class="listTitle1" style="width:100px;"><a class="l1"  href="" onclick="GetOrder('RoleID',this)">权限组别</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('PreLogintime',this)">上次登录时间</a></th>
                    <th class="listTitle2" style="width:200px;"><a class="l1"  href="" onclick="GetOrder('PreLoginIP',this)">上次登录地址</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('LastLogintime',this)">最后登录时间</a></th>
                    <th class="listTitle2" style="width:200px;"><a class="l1"  href="" onclick="GetOrder('LastLoginIP',this)">最后登录地址</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('LoginTimes',this)">登录次数</a></th>
                </tr>
                <% 
                    If IsNull(rs) Then
                        Response.Write("<tr class='tdbg'><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
                    Else  
                    For i=0 To Ubound(rs,2)               
                    rowNum = rowNum+1
                    If rowNum Mod 2 = 0 Then
                        className="list"        
                    Else
                        className="listBg"      
                    End If 
                %>
                <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='pointer';"
                    onmouseout="this.style.backgroundColor=currentcolor"> 
                    <%
                        If rs(0,i)="1" Then
                     %>  
                    <td>&nbsp;</td>   
                    <%
                        Else
                     %>       
                    <td><input name='cid' type='checkbox' value='<%=rs(0,i)%>'/></td>         
                    <%
                        End If
                     %>     
                    <%
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("500",trim(session("AdminName")))="2" or CxGame.GetRoleValue("500",trim(session("AdminName")))="3" Then
                    %>            
                    <td onclick="Redirect('MasterUserList.asp?action=userinfo&id=<%=rs(0,i) %>')"><%=rs(1,i) %></td>    
                    <td onclick="Redirect('MasterUserList.asp?action=userinfo&id=<%=rs(0,i) %>')"><%=CxGame.GetRoleNameByRoleID(rs(7,i)) %></td>    
                    <td onclick="Redirect('MasterUserList.asp?action=userinfo&id=<%=rs(0,i) %>')"><%=rs(2,i) %></td>    
                    <td onclick="Redirect('MasterUserList.asp?action=userinfo&id=<%=rs(0,i) %>')"><%=rs(3,i) %></td>   
                    <td onclick="Redirect('MasterUserList.asp?action=userinfo&id=<%=rs(0,i) %>')"><%=rs(4,i) %></td>    
                    <td onclick="Redirect('MasterUserList.asp?action=userinfo&id=<%=rs(0,i) %>')"><%=rs(5,i) %></td>   
                    <td onclick="Redirect('MasterUserList.asp?action=userinfo&id=<%=rs(0,i) %>')"><%=rs(6,i) %></td>  
                    <%
                        Else
                    %>
                    <td><%=rs(1,i) %></td> 
                    <td><%=CxGame.GetRoleNameByRoleID(rs(7,i)) %></td>   
                    <td><%=rs(2,i) %></td>    
                    <td><%=rs(3,i) %></td>   
                    <td><%=rs(4,i) %></td>    
                    <td><%=rs(5,i) %></td>   
                    <td><%=rs(6,i) %></td>  
                    <%
                            End If
                            Next
                        End If
                    %>       
                </tr>
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
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("500",trim(session("AdminName")))="2" or CxGame.GetRoleValue("500",trim(session("AdminName")))="3" Then
                    %>
                    <input type="button" value="新增" class="btn wd1" onclick="Redirect('?action=userInfo')" /> 
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
        
        Sub GetUserInfo()
            Dim rs,sql,titleInfo
            Dim id,userName,pwd,roleID
            if Request("id")="" Then
                id = 0
            Else
                id = Request("id")
            End If
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select *  from  Base_Users where  UserID="&id
            
            rs.Open sql,GameConn,1,3
          
            If rs.Bof And rs.Eof Then
                titleInfo = "新增用户信息"
            Else
                titleInfo = "修改用户信息"
                userName = rs("Username")
                pwd = rs("Password")   
                roleID = rs("RoleID")             
            End If
    %>
    
    <script type="text/javascript">
        function CheckFormInfo()
        {
            var userName = document.myFormInfo.in_UserName.value;
            var pwd1 = document.myFormInfo.in_Pwd.value;
            var pwd2 = document.myFormInfo.in_Pwd2.value;
            if(userName.trim()=="")
            {
                alert("用户名不能为空！");
                return false;
            }
            if(pwd1.trim()=="")
            {
                alert("密码不能为空！");
                return false;
            }
            if(pwd1.trim().length<4)
            {
                alert("密码至少为4位！");
                return false;
            }
            if(pwd2.trim()!=pwd1.trim())
            {
                alert("二次输入的密码不相同，请重新输入！");
                return false;
            }
        }
    </script>
    <form name="myFormInfo" method="post" action='?action=saveuserinfo&id=<%=Request("id") %>' onsubmit="return CheckFormInfo()">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" >
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('MasterUserList.asp')" />          
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
                <td class="listTdLeft">用户名：</td>
                <td>
                    <% If id=0 Then %>
                    <input name="in_UserName" type="text" class="text" value="<%=userName %>" />                    
                    <% Else%>
                    <span><%=userName %></span>
                    <input name="in_UserName" type="text" value="<%=userName %>" style="display:none" />  
                    <% End If %>                  
                </td>
            </tr>
             <tr>
                <td class="listTdLeft">
                    权限组别：
                </td>
                <td>
                    <select name="in_Role"  style="width:157px;">
                    <% 
                        Dim ArrayKind,i
                        ArrayKind = CxGame.GetRoleInfo()
                        For i=0 To Ubound(ArrayKind)     
                        If roleID=ArrayKind(i,0) Then                                                        
                    %>
                    <option value="<%=ArrayKind(i,0) %>" selected="selected"><%=ArrayKind(i,1)%></option>
                     <% Else%>
                    <option value="<%=ArrayKind(i,0) %>"><%=ArrayKind(i,1) %></option>
                    <%  
                        End If                         
                        Next  
                        Set ArrayKind = nothing
                    %>
                    </select>   
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">密码：</td>
                <td>
                    <input name="in_Pwd" type="password" class="text" />                                                
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">确认密码：</td>
                <td>
                    <input name="in_Pwd2" type="password" class="text"/>                               
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('MasterUserList.asp')" />          
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

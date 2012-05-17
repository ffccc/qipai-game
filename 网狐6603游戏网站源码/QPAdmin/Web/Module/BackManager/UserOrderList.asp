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
     <script type="text/javascript" language="javascript" >
     function Operate()
     {
        if(!confirm("确定要执行选定的操作吗？"))
        {
            return;
        }
        else
        {
            var RoleName = document.form2.RoleName.value.trim()
		   
		    if(RoleName=="")
		    {
		        alert("组别名称不能为空！");
		        return;
		    }
            var cid="";
            var cvalue="";
            var ivalue=0;
            var cids = GelTags("input");  
            var n=0;              
            for(var i=0;i<cids.length;i++)
            {
                if(cids[i].name == "cid")
                {
                    if(cids[i].checked)
                    {
                        if(cid=="")
                        {
                            cid=cids[i].value.split(',')[0];
                            ivalue=cids[i].value.split(',')[1];
                        }
                        else if(cids[i].value.split(',')[0]!=cids[i-1-n].value.split(',')[0])
                        {
                            if(cvalue!="")
                            {
                                cvalue=cvalue+","+ivalue;
                            }
                            else
                            {
                                cvalue=ivalue;
                            }
                            cid=cid+","+cids[i].value.split(',')[0];
                            ivalue=cids[i].value.split(',')[1];
                        }
                        else
                        {
                            ivalue=parseInt(ivalue)+parseInt(cids[i].value.split(',')[1]);
                        }
                        n=0;
                       
                    }
                    else
                    {
                       n=n+1;
                    }
                }
               
                
                if(i==(cids.length-1))
                {
                    if(cvalue!="")
                    {
                        cvalue=cvalue+","+ivalue;
                    }
                    else
                    {
                        cvalue=ivalue;
                    }
                }
            } 
            if(cid=="")
            {
                alert("未选中任何数据");
                return;
            }
               
         }
         var roleid= document.getElementById("in_Role").value;
         document.form2.action = "?action=saveinfo&ModuleID="+cid+"&PValue="+cvalue+"&in_Role="+roleid+"&in_rolename="+RoleName;
         document.form2.submit();
     }
     </script>
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：后台系统 - 角色管理</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->  
     <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7">
        <tr>
            <td height="28">
                <ul>
                    <li class="tab1" >角色管理</li>
	                <li class="tab2" onclick="openWindowSelf('MasterUserList.asp')">用户管理</li>
	             
                </ul>
            </td>
        </tr>
    </table>  
    <% 
        Call ConnectGame(QPPlatformManagerDB)
        Select case lcase(Request("action"))
            case "orderinfo"
            Call GetOrdernIfo()
            case "saveinfo"
            Call SaveOrdernIfo()
            case "roleadd"
            Call NewOrderInfo()
            case "rolesave"
            Call SaveOrder()
            case else
            Call Main()
        End Select
        '删除
        Sub DeleteUserInfo(lID)
            Dim rs,sql,ID
            ID = Trim(lID)
            GameConn.execute "delete from Base_Roles where RoleID="&ID    
            Call CxGame.ShowInfo2("删除成功！","UserOrderList.asp",1200)         
        End Sub
        
        Sub SaveOrdernIfo()
            Dim rs,sql,rs1,sql1,ID,cidArray,cvalueArray
            ID = Trim(Request("in_Role"))
            GameConn.execute "delete from Base_RolePermission where RoleID="&ID   
            Set rs=Server.CreateObject("Adodb.RecordSet")
            Set rs1=Server.CreateObject("Adodb.RecordSet")
            sql1="update Base_Roles set rolename='"&Request("in_rolename")&"' where roleid="&Request("in_Role")
            rs1.Open sql1,GameConn,1,3
            cidArray = Split(Request("ModuleID"),",")
            cvalueArray = Split(Request("PValue"),",")
            Dim n
            For lLoop=0 To UBound(cvalueArray)
                sql="insert into Base_RolePermission (RoleID,ModuleID,OperationPermission) values ('"&Request("in_Role")&"','"&cidArray(lLoop)&"','"&cvalueArray(lLoop)&"')"
                rs.Open sql,GameConn,1,3
            Next    
            Call CxGame.ShowInfo2("更新成功！","UserOrderList.asp",1200)         
            Call Main()  
        End Sub
        
        Sub SaveOrder()
            roleName = Trim(Request("RoleName"))
            Description = Trim(Request("Description"))
            Dim rs,sql,roleName,Description
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select * from  Base_Roles where RoleName='"&roleName&"'"
	        rs.Open sql,GameConn,1,3
            If rs.BOF and rs.EOF Then
                GameConn.execute " Insert into Base_Roles (rolename,description) values ('"&roleName&"','"&Description&"')"
                Call CxGame.ShowInfo2("新增成功！","UserOrderList.asp",1200)
            Else
                Call CxGame.ShowInfo2("组别名称重复！","UserOrderList.asp",1200)
            End If
            Call Main()  
        End Sub
        
        Sub Main()
            Dim cidArray, lLoop
            If Request("action")="delete" Then
                cidArray = Split(Request("cid"),",")
                For lLoop=0 To UBound(cidArray)
                    Call DeleteUserInfo(cidArray(lLoop))
                Next 
            End If
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
                OrderStr = "RoleID Asc"
            End If
            Dim fields        
            fields = "*"     
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
	            .Table="Base_Roles"			'表名
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
                var cids = form2.GelTags("input");                
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
    <form name="form1" id="form1" method="post" action="?action=delete">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" >
            <tr>
                <td height="39" class="titleOpBg">
                    <input type="button" value="新增" class="btn wd1" onclick="Redirect('UserOrderList.asp?action=roleadd')" />
                    <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1" />     
                    <input type="hidden" name="Action" value="DeleteAll" />     
                </td>
            </tr>
        </table> 
        <div id="content">        
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box">
                <tr align="center" class="bold">
                    <td class="listTitle1" style="width:38px;"><input type="checkbox" name="chkAll" onclick="SelectAll(this.checked);" /></td>
                    <th class="listTitle1" style="width:100px;"><a class="l1"  href="" onclick="GetOrder('RoleID',this)">角色标识</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('RoleName',this)">角色名称</a></th>
                    <th class="listTitle2" style="width:200px;"><a class="l1"  href="" onclick="GetOrder('Description',this)">备注</a></th>
                    <th class="listTitle2">操作</th>
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
                    <td onclick="Redirect('UserOrderList.asp?action=orderinfo&id=<%=rs(0,i) %>&in_Role=<%=rs(0,i)%>')"><%=rs(0,i) %></td>    
                    <td onclick="Redirect('UserOrderList.asp?action=orderinfo&id=<%=rs(0,i) %>&in_Role=<%=rs(0,i)%>')"><%=rs(1,i) %></td>    
                    <td onclick="Redirect('UserOrderList.asp?action=orderinfo&id=<%=rs(0,i) %>&in_Role=<%=rs(0,i)%>')"><%=rs(2,i) %></td>   
                    <td>
                            <a href="?action=orderinfo&id=<%=rs(0,i) %>&in_Role=<%=rs(0,i)%>
                            " class="l" >权限配置</a>        
                    </td>                           
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
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0"  id="OpList">
            <tr>
                <td height="39" class="titleOpBg">
                   <input type="button" value="新增" class="btn wd1" onclick="Redirect('UserOrderList.asp?action=roleadd')" />
                   <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1" />     
                </td>
            </tr>
        </table>        
    </form>
    <% 
        Set rs=nothing
        End Sub
        
        Sub GetOrdernIfo()
        lcase(Request("action"))
    %>
   
    <form id="form2" method="post" action="?action=orderinfo" name="form2" onsubmit="return CheckFormInfo();">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" >
            <tr>
                <td height="39" class="titleOpBg">
                    <input type="button" value="保存" class="btn wd1" onclick="Operate()" />
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('?')" />
                    <input name="in_Role" id="in_Role" type="hidden" value="<%=CxGame.GetInfo(0,"in_Role") %>" />    
                </td>
            </tr>
        </table> 
        <div>
            <%
                Dim rs,sql,n
                Call ConnectGame(QPPlatformManagerDB)
                set rs=server.createobject("adodb.recordset")
                sql="select ModuleID,Title,(select rolename from Base_Roles where roleid='"&Request("id")&"' ) as rolename from Base_Module where ParentID='0' order by orderno"
                rs.open sql,GameConn,1,3
            %>
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
                <tr>
                    <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">组别名称</div></td>
                </tr>
                 <tr>
                    <td class="listTdLeft">组别名称：</td>
                    <td><input name="RoleName" type=text class="text wd4" value="<%=rs("rolename") %>" /></td>
                </tr>
            </table>
            <%
                Dim j
                j=0  
                do until rs.eof 
                        
            %>        
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2" id="list_<%=j %>">
                <tr>
                    <td height="35" colspan="2" class="Lpd10 Rpd10"><div class="hg3  pd7"><span class="f14 bold "><%=rs("Title") %></span><span class="f12">（全选：<input type="checkbox" name="chkAll" onclick="SelectAllTable(this.checked,'list_<%=j %>');" />）</span></div></td>
                </tr>
                <% 
                    Dim rs1,sql1,id
                    Call ConnectGame(QPPlatformManagerDB)
                    set rs1=server.createobject("adodb.recordset")
                    sql1="select * from Base_Module where parentid="&rs("moduleid")
                    rs1.open sql1,GameConn,1,3
                    If rs.RecordCount=0 Then
                        Response.Write("<tr class='tdbg'><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
                    Else                    
                    do until rs1.eof 
                %>
                <tr >                    
                    <td class="listTdLeft"><%=rs1("title") %>：</td>
                    <td>
                           <% 
                                Dim rolers,rolesql
                                Call ConnectGame(QPPlatformManagerDB)
                                set rolers=server.createobject("adodb.recordset")
                                rolesql="select bm.moduleid as moduleid,bm.title as title , bmp.PermissionTitle as PermissionTitle,(select Operationpermission from Base_RolePermission brp where brp.moduleid=bmp.moduleid and brp.RoleID="&Request("in_Role")&" ) as Operationpermission,bmp.Permissionvalue  from Base_Module bm,Base_ModulePermission bmp  where bm.moduleid = bmp.moduleid and bm.moduleid="&rs1("moduleid")&"  order by bmp.moduleid"
                                rolers.open rolesql,GameConn,1,3
                                           
                                do until rolers.eof    
                                Select case rolers("Operationpermission")
                                    case "7"
                                        Select case rolers("Permissionvalue")
                                            case "1"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>      
                           <%   
                                            case "2"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>
                           <%
                                            case "4"
                           %>     
                           <input name='cid' type='checkbox'checked="checked"  value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>
                           <%
                                            case else
                           %>
                           <input name='cid' type='checkbox' value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>
                           <%
                                        End Select
                                    case "3"
                                        Select case rolers("Permissionvalue")
                                            case "1"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>      
                           <%   
                                            case "2"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%
                                            case else
                           %>
                           <input name='cid' type='checkbox' value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>
                           <%
                                        End Select
                                    case "6"
                                        Select case rolers("Permissionvalue")
                                            case "2"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%
                                            case "4"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%
                                            case else
                           %>
                            <input name='cid' type='checkbox' value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>
                           <%
                                        End Select
                                    case "1"
                                        Select case rolers("Permissionvalue")
                                            case "1"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>   
                           <%
                                            case else
                           %>
                           <input name='cid' type='checkbox' value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>
                           <%
                                        End Select
                                    case "2"
                                        Select case rolers("Permissionvalue")
                                            case "2"
                           %> 
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>   
                           <%
                                            case else
                           %>
                           <input name='cid' type='checkbox' value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>
                           <%
                                         End Select
                                     case "5"
                                        Select case rolers("Permissionvalue")
                                            case "1"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>  
                           <%
                                            case "4"
                           %> 
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>  
                           <%
                                            case else
                           %>
                           <input name='cid' type='checkbox' value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>
                           <%
                                        End Select 
                                    case "4"
                                        Select case rolers("Permissionvalue")
                                            case "4"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>   
                           <%
                                            case else
                           %>
                           <input name='cid' type='checkbox' value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>
                           <%   
                                        End Select 
                                    case "8"
                                         Select case rolers("Permissionvalue")
                                            case "8"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>   
                           <%
                                            case else
                           %>
                           <input name='cid' type='checkbox' value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>
                           <%
                                         End Select
                                    case "9"
                                         Select case rolers("Permissionvalue")
                                            case "1"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>  
                           <% 
                                            case "8"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%
                                            case else
                           %>
                           <input name='cid' type='checkbox' value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>
                           <%             
                                         End Select
                                    case "10"
                                         Select case rolers("Permissionvalue")
                                            case "2"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%     
                                            case "8"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%
                                            case else
                           %>
                           <input name='cid' type='checkbox' value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>
                           <%
                                         End Select
                                    case "11"
                                         Select case rolers("Permissionvalue")
                                            case "1"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%
                                            case "2"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%
                                            case "8"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%
                                            case else
                           %>
                           <input name='cid' type='checkbox' value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>
                           <%
                                         End Select
                                     case "12"
                                         Select case rolers("Permissionvalue")
                                            case "4"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%
                                            case "8"           
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%
                                            case else
                           %>
                           <input name='cid' type='checkbox' value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>
                           <%
                                         End Select  
                                     case "13"
                                         Select case rolers("Permissionvalue")
                                            case "1" 
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%
                                            case "4" 
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>    
                           <%           
                                            case "8"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>   
                           <%
                                            case else 
                           %>
                           <input name='cid' type='checkbox' value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>
                           <%
                                         End Select 
                                     case "14"
                                         Select case rolers("Permissionvalue")
                                            case "4" 
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%
                                            case "8" 
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%
                                            case "2"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%       
                                            case else
                           %>  
                           <input name='cid' type='checkbox' value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>
                           <%
                                         End Select   
                                     case "15"
                                         Select case rolers("Permissionvalue")
                                            case "1"    
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%
                                            case "2"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%
                                            case "4"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%
                                            case "8"
                           %>
                           <input name='cid' type='checkbox'checked="checked" value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/> 
                           <%
                                            case else 
                                         End Select                         
                                    case else
                           %>
                           <input name='cid' type='checkbox' value='<%=rolers("ModuleID")%>,<%=rolers("Permissionvalue")%>'/>

                           <% 
                                End Select
                           %>        
                     
                           <%=rolers("PermissionTitle") %>
                           <%
                                 rolers.movenext
                                 loop
                           %>
                    </td>          
                </tr>
                <% 
                    rs1.movenext
                    loop
                    End If
                %>
            </table>
            <% 
             j=j+1
             rs.movenext
             loop
             
        %>
        </div>  
          <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" >
            <tr>
                <td height="39" class="titleOpBg">
                    <input type="button" value="保存" class="btn wd1" onclick="Operate()" />
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('?')" />   
                </td>
            </tr>
        </table>  
        
    </form>
    

    <%
    End Sub
    
    Sub NewOrderInfo()
    %>
    <form id="form3" method="post" action="?action=rolesave" name="form3">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" >
            <tr>
                <td height="39" class="titleOpBg">
                    <input type="submit" value="保存" class="btn wd1" />
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('?')" />   
                </td>
            </tr>
        </table> 
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
                <td class="listTdLeft">
                    组别名：
                </td>
                <td>
                     <input name="RoleName" type="text" class="text wd4" />   
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">
                    组别描述：
                </td>
                <td>
                    <input name="Description" type="text" class="text wd4" />   
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td height="39" class="titleOpBg">
                    <input type="submit" value="保存" class="btn wd1" />
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('?')" />   
                </td>
            </tr>
        </table>  
    </form>
    <%
    End Sub
    %>
</body>
</html>
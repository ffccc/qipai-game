<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../Cls_Page.asp"-->
<!--#include file="../../Conn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>   
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript" src="../../Js/Sort.js"></script>  
</head>
<%
    If CxGame.GetRoleValue("300",trim(session("AdminName")))<"1" Then
    response.redirect "/Index.asp"   
    End If
%>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：系统维护 - 机器管理</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <form name="form1" action="">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="titleQueBg Tmg7">
            <tr>
                <td class="listTdLeft" style="width: 80px">
                    机器名称：
                </td>
                <td>
                    <input name="search" type="text" class="text"  value="<%=CxGame.GetInfo(0,"search") %>"/>
                    <input type="submit" value="查询" class="btn wd1" />
                </td>
            </tr>
        </table>
    </form>
    <% 
        Call ConnectGame(QPPlatformDB)
        Select case lcase(Request("action"))
            case "dbinfo"
            Call GetDBInfo()
            case "savedbinfo"
            Call SaveDBInfo()
            case else
            Call Main()
        End Select
        Call CloseGame()
        
        '保存
        Sub SaveDBInfo()
            Dim rs,sql
            Dim xorObj
            Set rs=Server.CreateObject("Adodb.RecordSet")
            Set xorObj = Server.CreateObject("Game.CWHEncryptNet")
            sql = "select * from DataBaseInfo where DBAddr='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3
            If rs.eof Then
                rs.addnew
            End If
            rs("DBAddr") = CxGame.GetInfo(0,"DBAddr")
            rs("DBPort") = CxGame.GetInfo(1,"DBPort")
            rs("DBUser") = xorObj.XorEncrypt(CxGame.GetInfo(0,"DBUser"))            
            If CxGame.GetInfo(0,"DBPassword2")="********" Then
                rs("DBPassword") = xorObj.XorEncrypt(CxGame.GetInfo(0,"DBPassword"))
            Else
                rs("DBPassword") = CxGame.GetInfo(0,"DBPassword2")
            End If
            rs("MachineID") = CxGame.GetInfo(0,"MachineID")
            rs("Information") = CxGame.GetInfo(0,"Information")
            rs.update
            If Request("id")<>"" Then
                Call CxGame.ShowInfo2("修改成功！","DataBaseInfo.asp?page="&Request("page")&"",1200)
            Else
                Call CxGame.ShowInfo2("新增成功！","DataBaseInfo.asp",1200)
            End If
            rs.close()
            Set xorObj = Nothing
            Set rs = Nothing           
        End Sub
        
        '删除操作
        Sub Delete(lID)
            Dim ID
            ID = Trim(lID)
            GameConn.execute "delete DataBaseInfo where DBAddr='"&ID&"'"                
        End Sub
        
        Sub Main()
            Dim cidArray, lLoop
            cidArray = Split(Request("cid"),",")
            For lLoop=0 To UBound(cidArray)
                Call Delete(cidArray(lLoop))
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
                OrderStr = "Information Asc"
            End If
            '查询条件
            If Request("search")<>"" Then
                queryCondition = "Information like '%"&CxGame.GetInfo(0,"search")&"%'"
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
	            .Pkey="DBAddr"					'主键
	            .Field="DBAddr,DBPort,DBUser,DBPassword,MachineID,Information"	'字段
	            .Table="DataBaseInfo"			'表名
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
	                <input type="button" value="新建" class="btn wd1" onclick="Redirect('?action=dbInfo')" />
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
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('Information',this)">服务器名</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('DBAddr',this)">地址</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('DBPort',this)">端口</a></th>
                    <th class="listTitle"><a class="l1"  href="" onclick="GetOrder('MachineID',this)">机器码</a></th>
                </tr>
                <% 
                    Dim DBAddr,className
                    If IsNull(rs) Then
                        Response.Write("<tr class='tdbg'><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
                    Else
                    
                    For i=0 To Ubound(rs,2)
                    DBAddr = rs(0,i)                
                    If i Mod 2 = 0 Then
                        className="list"        
                    Else
                        className="listBg"      
                    End If               
                %>
                <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='pointer';"
                    onmouseout="this.style.backgroundColor=currentcolor">
                    <td><input name='cid' type='checkbox' value='<%=DBAddr%>'/></td>                  
                    <td onclick="Redirect('DataBaseInfo.asp?action=dbInfo&id=<%=DBAddr %>&page=<%=Request("page") %>')"><%=rs(5,i) %></td>    
                    <td onclick="Redirect('DataBaseInfo.asp?action=dbInfo&id=<%=DBAddr %>&page=<%=Request("page") %>')"><%=rs(0,i) %></td>    
                    <td onclick="Redirect('DataBaseInfo.asp?action=dbInfo&id=<%=DBAddr %>&page=<%=Request("page") %>')"><%=rs(1,i) %></td>    
                    <td onclick="Redirect('DataBaseInfo.asp?action=dbInfo&id=<%=DBAddr %>&page=<%=Request("page") %>')"><%=rs(4,i) %></td>   
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
	                <input type="button" value="新建" class="btn wd1" onclick="Redirect('?action=dbInfo')" />
                    <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1"/>              
                </td>
            </tr>
        </table> 
    </form>
    <% 
        End Sub
        
        Rem 机器信息
        Sub GetDBInfo  
            Dim rs,sql,titleInfo
            Dim dbAddr,dbPort,dbUser,dbPwd,machineID,info
            Dim xorObj            
            Set rs=Server.CreateObject("Adodb.RecordSet")
            Set xorObj = Server.CreateObject("Game.CWHEncryptNet")
            sql = "select * from DataBaseInfo(nolock) where DBAddr='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3
            If rs.Bof And rs.Eof Then
                titleInfo = "新增机器信息"
            Else
                titleInfo = "修改机器信息"
                dbAddr = rs("DBAddr")
                dbPort = rs("DBPort")
                dbUser = xorObj.XorCrevasse(rs("DBUser"))
                dbPwd = rs("DBPassword")
                machineID = rs("MachineID")
                info = rs("Information")
            End If
            Set xorObj = nothing
    %>
    <script type="text/javascript">
        function ChangePwd()
        {
            var pwd2 = document.myFormInfo.DBPassword2;
            pwd2.value = "********";
        }
        
        function CheckFormInfo()
        {
            var information = document.myFormInfo.Information.value;
            var dBAddr = document.myFormInfo.DBAddr.value;
            var dBPort = document.myFormInfo.DBPort.value;
            var dBUser = document.myFormInfo.DBUser.value;
            var dBPassword = document.myFormInfo.DBPassword.value;
            var re = /^(-?[1-9][0-9]*|0)$/; 
            if(information.trim()=="")
            {
                alert("机器名称不能为空！");
                return false;
            }
            if(dBAddr.trim()=="")
            {
                alert("地址不能为空！");
                return false;
            }
            else
            {
                if(!IsIp(dBAddr))
                {
                    alert("地址输入非法！");
                    return false;
                }
            }
            if(dBPort.trim()=="")
            {
                alert("端口不能为空！");
                return false;
            }
            else
            {
                if (!re.test(dBPort.trim()))
                {
                    alert("端口必须为正整数！");
                    return false;
                }
                else
                {
                    if(dBPort.trim()<0)
                    {
                        alert("端口必须为正整数！");
                        return false;
                    }
                }
            }
            if(dBUser.trim()=="")
            {
                alert("用户名不能为空！");
                return false;
            }
            if(dBPassword.trim()=="")
            {
                alert("密码不能为空！");
                return false;
            }
        }
    </script>
    <form name="myFormInfo" method="post" action='?action=savedbinfo&id=<%=Request("id") %>&page=<%=Request("page") %>' onsubmit="return CheckFormInfo()">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('DataBaseInfo.asp')" />                           
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
                <td class="listTdLeft">机器名称：</td>
                <td>
                    <input name="Information" type="text" class="text" value="<%=info %>" />         
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">地址：</td>
                <td>
                    <input name="DBAddr" type="text" class="text" value="<%=dbAddr %>"/>    
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">端口：</td>
                <td>
                    <input name="DBPort" type="text" class="text" value="<%=dbPort %>"/>           
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">用户名：</td>
                <td>
                    <input name="DBUser" type="text" class="text" value="<%=dbUser %>"/>              
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">密码：</td>
                <td>
                    <input name="DBPassword" type="password" class="text"<% If dbAddr="" Then%> value="" <% Else %> value="********" <% End If%> onchange="ChangePwd()"/>    
                    <input name="DBPassword2" type="hidden" value="<%=dbPwd %>"/>                   
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">机器码：</td>
                <td>
                    <input name="MachineID" type="text" class="text" value="<%=machineID %>"/>                               
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('DataBaseInfo.asp')" />                           
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

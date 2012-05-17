<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../Cls_Page.asp"-->
<!--#include file="../../function.asp"-->
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
    If CxGame.GetRoleValue("301",trim(session("AdminName")))<"1" Then
    response.redirect "/Index.asp"   
    End If
%>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：系统维护 - 游戏管理</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7">
        <tr>
            <td height="28">
                <ul>
                    <li class="tab1">模块</li>
	                <li class="tab2" onclick="openWindowSelf('GameTypeItem.asp')">类型</li>
	                <li class="tab2" onclick="openWindowSelf('GameKindItem.asp')">游戏</li>
	                <li class="tab2" onclick="openWindowSelf('GameNodeItem.asp')">节点</li>
	                <li class="tab2" onclick="openWindowSelf('GamePageItem.asp')">自定义页</li>
                </ul>
            </td>
        </tr>
    </table>    
    <% 
        Call ConnectGame(QPPlatformDB)
        Select case lcase(Request("action"))
            case "gameinfo"
            Call GetGameInfo()
            case "save"
            Call SaveGameInfo()
            case else
            Call Main()
        End Select
        Call CloseGame()
        
        '保存
        Sub SaveGameInfo()
            Dim rs,sql,SupporTypeArr,SupporType,RL_i
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from GameGameItem where GameID='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3
            If rs.eof Then
                rs.addnew
            End If
            rs("GameID") = CxGame.GetInfo(1,"in_GameID")
            rs("GameName") = CxGame.GetInfo(0,"in_GameName")
            rs("DataBaseAddr") = CxGame.GetInfo(0,"in_DBAddr")
            rs("DataBaseName") = CxGame.GetInfo(0,"in_DBName")
            '支出类型
            SupporTypeArr = Split(Request("in_SupporType"),",")
            SupporType=0
            For RL_i=0 To UBound(SupporTypeArr)
                SupporType = SupporType Or SupporTypeArr(RL_i)
            Next
            rs("SuporType") = SupporType
            rs("ServerVersion") = Common.CalVersion2(CxGame.GetInfo(0,"in_ServerVersion"))
            rs("ClientVersion") = Common.CalVersion2(CxGame.GetInfo(0,"in_ClientVersion"))
            rs("ServerDLLName") = CxGame.GetInfo(0,"in_ServerDLLName")
            rs("ClientExeName") = CxGame.GetInfo(0,"in_ClientExeName")
            rs.update
            If Request("id")<>"" Then
                Call CxGame.ShowInfo2("修改成功！","GameGameItem.asp?page="&Request("page")&"",1200)
            Else
                Call CxGame.ShowInfo2("新增成功！","GameGameItem.asp",1200)
            End If
            rs.close()
            Set rs = Nothing  
        End Sub
        
        '删除
        Sub Delete(lID)
            Dim ID
            ID = lID
            GameConn.execute "delete GameGameItem where GameID='"&ID&"'"            
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
                OrderStr = "GameID Asc"
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
	            .Pkey="GameID"					'主键
	            .Field="*"	'字段
	            .Table="GameGameItem"			'表名
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
	                <input type="button" value="新建" class="btn wd1" onclick="Redirect('?action=gameinfo')" />
                    <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1" />            
                </td>
            </tr>
        </table> 
        <div id="content">
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box">
                <tr align="center" class="bold">
                    <th class="listTitle1" width='38' align='center'><input type="checkbox" name="chkAll" onclick="SelectAll(this.checked);" /></th>
                    <th class="listTitle2" width="80"><a href="" onclick="GetOrder('GameID',this)">模块标识</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('GameName',this)">模块名称</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('ServerVersion',this)">服务端版本</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('ClientVersion',this)">客户端版本</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('DataBaseName',this)">数据库名</a></th>
                    <th class="listTitle"><a class="l1" href="" onclick="GetOrder('DataBaseAddr',this)">数据库地址</a></th>
                </tr>
                <% 
                    Dim GameID,className
                    If IsNull(rs) Then
                        Response.Write("<tr class='tdbg'><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
                    Else
                    For i=0 To Ubound(rs,2)
                    GameID = rs(0,i)    
                    If i Mod 2 = 0 Then
                        className="list"        
                    Else
                        className="listBg"      
                    End If             
                %>
                <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='pointer';"
                    onmouseout="this.style.backgroundColor=currentcolor">
                    <td><input name='cid' type='checkbox' value='<%=GameID%>'/></td>                  
                    <td onclick="Redirect('?action=gameinfo&id=<%=GameID %>&page=<%=Request("page") %>')"><%=rs(0,i) %></td>    
                    <td onclick="Redirect('?action=gameinfo&id=<%=GameID %>&page=<%=Request("page") %>')"><%=rs(1,i) %></td>        
                    <td onclick="Redirect('?action=gameinfo&id=<%=GameID %>&page=<%=Request("page") %>')"><%=Common.CalVersion(rs(5,i)) %></td>   
                    <td onclick="Redirect('?action=gameinfo&id=<%=GameID %>&page=<%=Request("page") %>')"><%=Common.CalVersion(rs(6,i)) %></td>   
                    <td onclick="Redirect('?action=gameinfo&id=<%=GameID %>&page=<%=Request("page") %>')"><%=rs(4,i) %></td>   
                    <td onclick="Redirect('?action=gameinfo&id=<%=GameID %>&page=<%=Request("page") %>')"><%=rs(3,i) %></td>  
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
	                <input type="button" value="新建" class="btn wd1" onclick="Redirect('?action=gameinfo')" />
                    <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1" /><input type="hidden" name="Action" value="DeleteAll" />         
                </td>
            </tr>
        </table> 
    </form>
    <% 
        End Sub
        
        Sub GetGameInfo()
            Dim rs,sql,titleInfo
            Dim gameID,gameName,dBAddr,dBName,supporType,serverVersion,clientVersion,serverDLLName,clientExeName
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from GameGameItem(nolock) where GameID='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3
            If rs.Bof And rs.Eof Then
                titleInfo = "新增模块"
                gameID=0
            Else
                titleInfo = "修改模块"
                gameID = rs("GameID")
                gameName = rs("GameName")
                dBAddr = rs("DataBaseAddr")
                dBName = rs("DataBaseName")
                supporType = rs("SuporType")
                serverVersion = rs("ServerVersion")
                clientVersion = rs("ClientVersion")
                serverDLLName = rs("ServerDLLName")
                clientExeName = rs("ClientExeName")
            End If   
    %>
    <script type="text/javascript">
        function CheckFormInfo()
        {
            var gameID = document.myFormInfo.in_GameID.value;
            var gameName = document.myFormInfo.in_GameName.value;
            var serverVersion = document.myFormInfo.in_ServerVersion.value;
            var clientVersion = document.myFormInfo.in_ClientVersion.value;
            var serverDLLName = document.myFormInfo.in_ServerDLLName.value;
            var clientExeName = document.myFormInfo.in_ClientExeName.value;
            if(gameID.trim()=="")
            {
                alert("模块标识不能为空！");
                return false;
            }
            else if(IsPositiveInt(gameID)==false)
            {
                alert("模块标识非数字字符或者超过允许大小！")
                return false;
            }
            if(gameName.trim()=="")
            {
                alert("模块名称不能为空！");
                return false;
            }
            else if(len(gameName.trim())>31)
            {
                alert("模块名称字符长度不能超过31个字符！")
                return false;
            }
            if(!IsIp(serverVersion))
            {
                alert("服务端版本输入非法！");
                return false;
            }
            if(!IsIp(clientVersion))
            {
                alert("客户端版本输入非法！");
                return false;
            }
            if(serverDLLName.trim()=="")
            {
                alert("服务端名称不能为空！");
                return false;
            }
            if(clientExeName.trim()=="")
            {
                alert("客户端名称不能为空！");
                return false;
            }
        }
    </script>
    <form name="myFormInfo" method="post" action='?action=save&id=<%=Request("id") %>&page=<%=Request("page") %>' onsubmit="return CheckFormInfo();">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('GameGameItem.asp')" />                               
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
                <td class="listTdLeft">模块标识：</td>
                <td>
                    <% If gameID=0 Then%>
                    <input name="in_GameID" type="text" class="text" value="<%=CxGame.GetMaxGameID()+1 %>" />  
                    <% Else%>      
                    <input name="in_GameID" type="text" class="text" readonly="readonly" value="<%=GameID %>" />  
                    <% End If%>         
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">模块名称：</td>
                <td>
                    <input name="in_GameName" type="text" class="text" value="<%=gameName %>" />               
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">数据库名：</td>
                <td>
                    <input name="in_DBName" type="text" class="text" value="<%=dBName %>" />                
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">数据库地址：</td>
                <td>
                    <select name="in_DBAddr" style="width:159px;">
			            <% 
			                Dim ArrayDB,i
                            ArrayDB = CxGame.GetDataBaseList()
                            For i=0 To Ubound(ArrayDB)    
                            If dBAddr=ArrayDB(i,0) Then   
			            %>
			            <option value="<%=ArrayDB(i,0) %>" selected="selected"><%=ArrayDB(i,1) %></option>
			            <% Else %>
			            <option value="<%=ArrayDB(i,0) %>"><%=ArrayDB(i,1) %></option>
			            <% 
                            End If                        
                            Next  
                            Set ArrayDB = nothing
                        %>
			        </select>             
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">服务端版本：</td>
                <td>
                    <input name="in_ServerVersion" type="text" class="text" value="<%=Common.CalVersion(serverVersion) %>" />              
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">客户端版本：</td>
                <td>
                    <input name="in_ClientVersion" type="text" class="text" value="<%=Common.CalVersion(clientVersion) %>" />                 
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">服务端名称：</td>
                <td>
                    <input name="in_ServerDLLName" type="text" class="text" value="<%=serverDLLName %>" />             
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">客户端名称：</td>
                <td>
                    <input name="in_ClientExeName" type="text" class="text" value="<%=clientExeName %>" />     
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">支持类型：</td>
                <td>
                    <input name="in_SupporType" id="Checkbox1" type="checkbox" value="1"<% If supporType And 1 Then %> checked="checked"<% End If  %> /><label for="in_SupporType1">金币类型</label> 
                    <input name="in_SupporType" id="Checkbox2" type="checkbox" value="2"<% If supporType And 2 Then %> checked="checked"<% End If  %> /><label for="in_SupporType2">点值类型</label> 
                    <input name="in_SupporType" id="Checkbox3" type="checkbox" value="4"<% If supporType And 4 Then %> checked="checked"<% End If  %> /><label for="in_SupporType3">比赛类型</label>  
                    <input name="in_SupporType" id="Checkbox4" type="checkbox" value="8"<% If supporType And 8 Then %> checked="checked"<% End If  %> /><label for="in_SupporType4">训练类型</label>                                       
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('GameGameItem.asp')" />                               
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

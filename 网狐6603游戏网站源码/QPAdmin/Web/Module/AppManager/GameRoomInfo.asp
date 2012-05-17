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
    If CxGame.GetRoleValue("302",trim(session("AdminName")))<"1" Then
    response.redirect "/Index.asp"   
    End If
%>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：系统维护 - 房间管理</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <form id="form1" action="">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7 titleQueBg">
            <tr>
                <td class="listTdLeft" style="width: 80px">
                    房间名称：
                </td>
                <td>
                    <input name="search" type="text" class="text" value="<%=CxGame.GetInfo(0,"search") %>" />
                    <input type="submit" value="查询" class="btn wd1" />
                </td>
            </tr>
        </table>
    </form>
    <% 
        Call ConnectGame(QPPlatformDB)
        Select case lcase(Request("action"))
            case "roominfo"
            Call GetRoomInfo()
            case "saveroominfo"
            Call SaveRoomInfo()
            case else
            Call Main()
        End Select
        Call CloseGame()
        
        Function GetServerTypeName(serverType)
            Dim rValue
            rValue=""
            Select case serverType
                case 1
                rValue="点值类型"
                case 2
                rValue="金币类型"
                case 4
                rValue="比赛类型"
                case 8
                rValue="训练类型"
            End Select 
            GetServerTypeName = rValue
        End Function 
        
        '保存
        Sub SaveRoomInfo()
            Dim rs,sql
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from GameRoomInfo where ServerID='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3
            If rs.eof Then
                rs.addnew
            End If
            rs("ServerName") = CxGame.GetInfo(0,"in_ServerName")
            rs("KindID") = CxGame.GetInfo(1,"in_KindID")
            rs("NodeID") = CxGame.GetInfo(1,"in_NodeID")
            rs("SortID") = CxGame.GetInfo(1,"in_SortID")
            rs("GameID") = CxGame.GetInfo(1,"in_GameID")
            rs("TableCount") = CxGame.GetInfo(1,"in_TableCount")
            rs("ServerType") = CxGame.GetInfo(1,"in_ServerType")
            rs("ServerPort") = CxGame.GetInfo(1,"in_ServerPort")
            rs("DataBaseName") = CxGame.GetInfo(0,"in_GameDataBaseName")
            rs("DataBaseAddr") = CxGame.GetInfo(0,"in_GameDataBaseAddr")
            rs("CellScore") = CxGame.GetInfo(1,"in_CellScore")
            rs("RevenueRatio") = CxGame.GetInfo(1,"in_RevenueRatio")
            rs("RestrictScore") = CxGame.GetInfo(1,"in_RestrictScore")
            rs("MinTableScore") = CxGame.GetInfo(1,"in_MinTableScore")
            rs("MinEnterScore") = CxGame.GetInfo(1,"in_MinEnterScore")
            rs("MaxEnterScore") = CxGame.GetInfo(1,"in_MaxEnterScore")
            rs("MinEnterMember") = CxGame.GetInfo(1,"in_MinEnterMember")
            rs("MaxEnterMember") = CxGame.GetInfo(1,"in_MaxEnterMember")
            rs("MaxPlayer") = CxGame.GetInfo(1,"in_MaxPlayer")
            rs("ServerRule") = CxGame.GetInfo(1,"in_ServerRule")
            rs("AttachUserRight") = CxGame.GetInfo(1,"in_AttachUserRight")
            rs("ServiceMachine") = CxGame.GetInfo(1,"in_ServiceMachine")
            rs("CustomRule") = CxGame.GetInfo(1,"in_CustomRule")
            rs("Nullity") = CxGame.GetInfo(1,"in_Nullity")
            rs("ServerNote") = CxGame.GetInfo(1,"in_ServerNote")
            rs("ModifyDateTime") = Now()
            rs.update
            If Request("id")<>"" Then
                Call CxGame.ShowInfo2("修改成功！","GameRoomInfo.asp?page="&Request("page")&"",1200)
            Else
                Call CxGame.ShowInfo2("新增成功！","GameRoomInfo.asp",1200)
            End If
            rs.close()
            Set rs = Nothing    
        End Sub
        
        '删除操作
        Sub Delete(lID)
            Dim ID
            ID = lID
            GameConn.execute "delete GameRoomInfo where ServerID='"&ID&"'"            
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
                OrderStr = "SortID Asc"
            End If
            '查询条件
            If Request("search")<>"" Then
                queryCondition = "ServerName like '%"&CxGame.GetInfo(0,"search")&"%'"
            End If
            
            Dim fields        
            fields = "ServerID,ServerName,KindID,NodeID,SortID,GameID,TableCount,ServerType,ServerPort,DataBaseName,DataBaseAddr,CellScore,RevenueRatio,RestrictScore,"
            fields = fields&"MinTableScore,MinEnterScore,MaxEnterScore,MinEnterMember,MaxEnterMember,MaxPlayer,ServerRule,ServiceMachine,Nullity,ServerNote,"         
            fields = fields&"CreateDateTime,ModifyDateTime"
            
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = GetPageSize					'每页记录条数
                .PageParm = "page"				'页参数
                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	            .Pkey="ServerID"					'主键
	            .Field=fields	'字段
	            .Table="GameRoomInfo"			'表名
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
	                <input type="button" value="新建" class="btn wd1" onclick="Redirect('?action=roomInfo')" />
                    <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1" />        
                    <input type="hidden" name="Action" value="DeleteAll" />             
                </td>
            </tr>
        </table> 
        <div id="content">
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box">
                <tr align="center" class="bold">
                    <th class="listTitle1" width='38' align='center'><input type="checkbox" name="chkAll" onclick="SelectAll(this.checked);" /></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('ServerID',this)">房间标识</a></th>  
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('ServerName',this)">房间名称</a></th>  
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('KindID',this)">游戏名称</a></th> 
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('NodeID',this)">节点名称</a></th>  
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('SortID',this)">排序</a></th> 
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('GameID',this)">模块名称</a></th>      
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('TableCount',this)">桌子数量</a></th>  
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('ServerType',this)">房间类型</a></th>   
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('ServerPort',this)">服务端口</a></th>   
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('DataBaseName',this)">数据库名称</a></th>  
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('DataBaseAddr',this)">数据库地址</a></th>   
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CellScore',this)">单位积分</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('RevenueRatio',this)">税收比例</a></th>                     
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('RestrictScore',this)">限制积分</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('MinTableScore',this)">最低积分</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('MinEnterScore',this)">最小进入积分</a></th>    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('MaxEnterScore',this)">最大进入积分</a></th>   
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('MinEnterMember',this)">最小进入等级</a></th>  
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('MaxEnterMember',this)">最大进入等级</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('MaxPlayer',this)">最大人数</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('ServerRule',this)">房间规则</a></th>   
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('ServiceMachine',this)">运行机器</a></th>                
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('Nullity',this)">冻结状态</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('ServerNote',this)">备注</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CreateDateTime',this)">创建时间</a></th>
                    <th class="listTitle"><a class="l1"  href="" onclick="GetOrder('ModifyDateTime',this)">修改时间</a></th>                    
                </tr>
                <% 
                    Dim ServerID,className
                    If IsNull(rs) Then
                        Response.Write("<tr class='tdbg'><td colspan='100' align='left'><br>没有任何信息!<br><br></td></tr>")
                    Else
                    
                    For i=0 To Ubound(rs,2)
                    ServerID = rs(0,i) 
                    If i Mod 2 = 0 Then
                        className="list"        
                    Else
                        className="listBg"      
                    End If       
                %>
                <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='pointer';"
                    onmouseout="this.style.backgroundColor=currentcolor">
                    <td><input name='cid' type='checkbox' value='<%=ServerID%>'/></td>    
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(0,i) %></td>   
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(1,i) %></td>   
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=CxGame.GetKindNameByKindID(rs(2,i)) %></td>  
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=CxGame.GetNodeNameByNodeID(rs(3,i)) %></td>               
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(4,i) %></td>    
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=CxGame.GetGameNameByGameID(rs(5,i)) %></td>   
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(6,i) %></td>              
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=GetServerTypeName(rs(7,i)) %></td>   
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(8,i) %></td>              
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(9,i) %></td>              
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(10,i) %></td>  
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(11,i) %></td>
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(12,i) %></td>   
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(13,i) %></td>  
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(14,i) %></td>  
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(15,i) %></td>  
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(16,i) %></td>  
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(17,i) %></td>  
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(18,i) %></td>  
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(19,i) %></td>  
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(20,i) %></td>
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(21,i) %></td>
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=CxGame.GetNullityInfo(rs(22,i)) %></td>
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(23,i) %></td>
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(24,i) %></td>
                    <td onclick="Redirect('GameRoomInfo.asp?action=roomInfo&id=<%=ServerID %>&page=<%=Request("page") %>')"><%=rs(25,i) %></td>
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
	                <input type="button" value="新建" class="btn wd1" onclick="Redirect('?action=roomInfo')" />
                    <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1" />            
                </td>
            </tr>
        </table> 
    </form>
    <% 
        End Sub
        
        Sub GetRoomInfo()
            Dim rs,sql,titleInfo
            Dim serverName,kindID,nodeID,sortID,gameID,tableCount,serverType,serverPort,dataBaseName,dataBaseAddr,cellScore,revenueRatio,restrictScore
            Dim minTableScore,minEnterScore,maxEnterScore,minEnterMember,maxEnterMember,maxPlayer,serverRule,attachUserRight,serviceMachine,customRule
            Dim nullity,serverNote,createDateTime,modifyDateTime
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from GameRoomInfo where ServerID='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3
            If rs.Bof And rs.Eof Then
                titleInfo = "新增房间信息"
            Else
                titleInfo = "修改房间信息"   
                serverName = rs("ServerName")
                kindID = rs("KindID")
                nodeID = rs("NodeID")
                sortID = rs("SortID")
                gameID = rs("GameID")
                tableCount = rs("TableCount")
                serverType = rs("ServerType")
                serverPort = rs("ServerPort")
                dataBaseName = rs("DataBaseName")
                dataBaseAddr = rs("DataBaseAddr")
                cellScore = rs("CellScore")
                revenueRatio = rs("RevenueRatio")
                restrictScore = rs("RestrictScore")
                minTableScore = rs("MinTableScore")
                minEnterScore = rs("MinEnterScore")
                maxEnterScore = rs("MaxEnterScore")
                minEnterMember = rs("MinEnterMember")
                maxEnterMember = rs("MaxEnterMember")
                maxPlayer = rs("MaxPlayer")
                serverRule = rs("ServerRule")
                attachUserRight = rs("AttachUserRight")
                serviceMachine = rs("ServiceMachine")
                customRule = rs("CustomRule")
                nullity = rs("Nullity")
                serverNote = rs("ServerNote")
                createDateTime = rs("CreateDateTime")
                modifyDateTime = rs("ModifyDateTime")
            End If
    %>
    <script type="text/javascript">
        function CheckFormInfo()
        {
            var serverName = document.myFormInfo.in_ServerName.value;
            var sortID = document.myFormInfo.in_SortID.value;
            var gameID = document.myFormInfo.in_GameID.value;
            var tableCount = document.myFormInfo.in_TableCount.value;
            var serverPort = document.myFormInfo.in_ServerPort.value;
            var gameDataBaseName = document.myFormInfo.in_GameDataBaseName.value;
            var cellScore = document.myFormInfo.in_CellScore.value;
            var revenueRatio = document.myFormInfo.in_RevenueRatio.value;
            var restrictScore = document.myFormInfo.in_RestrictScore.value;
            var minTableScore = document.myFormInfo.in_MinTableScore.value;
            var minEnterScore = document.myFormInfo.in_MinEnterScore.value;
            var maxEnterScore = document.myFormInfo.in_MaxEnterScore.value;
            var minEnterMember = document.myFormInfo.in_MinEnterMember.value;
            var maxEnterMember = document.myFormInfo.in_MaxEnterMember.value;
            var maxPlayer = document.myFormInfo.in_MaxPlayer.value;
            var serverRule = document.myFormInfo.in_ServerRule.value;
            var serverNote = document.myFormInfo.in_ServerNote.value;
            if(serverName.trim()=="")
            {
                alert("房间名称不能为空！");
                return false;
            }
            if(len(serverName.trim())>31)
            {
                alert("房间名称字符长度不可超过31个字符！")
                return false;
            }
            if(IsPositiveInt(sortID)==false)
            {
                alert("排序非数字字符或者超过允许大小！");
                return false;
            }
            if(IsPositiveInt(gameID)==false)
            {
                alert("模块名称非数字字符或者超过允许大小！");
                return false;
            }
            if(IsPositiveInt(gameID)==false)
            {
                alert("桌子数量非数字字符或者超过允许大小！");
                return false;
            }
            if(IsPositiveInt(serverPort)==false)
            {
                alert("服务端口非数字字符或者超过允许大小！");
                return false;
            }
            if(len(gameDataBaseName)>31)
            {
                alert("游戏数据库名称字符长度不可超过31个字符！")
                return false;
            }
            if(IsPositiveInt(cellScore)==false)
            {
                alert("单位积分非数字字符或者超过允许大小！")
                return false;
            }
            if(IsPositiveInt(revenueRatio)==false)
            {
                alert("税收比率非数字字符或者超过允许大小！")
                return false;
            }
            if(IsPositiveInt(restrictScore)==false)
            {
                alert("限制积分非数字字符或者超过允许大小！")
                return false;
            }
            if(IsPositiveInt(minTableScore)==false)
            {
                alert("最低积分非数字字符或者超过允许大小！")
                return false;
            }
            if(IsPositiveInt1(minEnterScore)==false)
            {
                alert("最小进入积分非数字字符或者超过允许大小！")
                return false;
            }
            if(IsPositiveInt1(maxEnterScore)==false)
            {
                alert("最大进入积分非数字字符或者超过允许大小！")
                return false;
            }
            if(IsPositiveInt(minEnterMember)==false)
            {
                alert("最小进入等级非数字字符或者超过允许大小！")
                return false;
            }
            if(IsPositiveInt(maxEnterMember)==false)
            {
                alert("最大进入等级非数字字符或者超过允许大小！")
                return false;
            }
            if(IsPositiveInt(maxPlayer)==false)
            {
                alert("最大玩家数目非数字字符或者超过允许大小！")
                return false;
            }
            if(len(serverRule)>512)
            {
                alert("游戏数据库名称字符长度不可超过512个字符！")
                return false;
            }
             if(len(serverNote)>64)
            {
                alert("备注字符长度不可超过64个字符！")
                return false;
            }
        }
    </script>
    <form name="myFormInfo" method="post" action='?action=saveroomInfo&id=<%=Request("id") %>&page=<%=Request("page") %>' onsubmit="return CheckFormInfo()">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('GameRoomInfo.asp')" />                     
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
                <td class="listTdLeft">房间名称：</td>
                <td>
                    <input name="in_ServerName" type="text" class="text" value="<%=serverName %>" />                   
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">游戏名称：</td>
                <td>
                    <select name="in_KindID" style="width:157px;">
                    <% 
                        Dim ArrayKind,i
                        ArrayKind = CxGame.GetKindList()
                        For i=0 To Ubound(ArrayKind)    
                        If kindID=ArrayKind(i,0) Then                                                      
                    %>
                    <option value="<%=ArrayKind(i,0) %>" selected="selected"><%=ArrayKind(i,1) %></option>
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
                <td class="listTdLeft">节点名称：</td>
                <td>
                    <select name="in_NodeID" style="width:157px;">
                    <option value="0">无挂接</option>
                    <% 
                        Dim ArrayNode,j
                        ArrayNode = CxGame.GetNodeList()
                        For j=0 To Ubound(ArrayNode)    
                        If nodeID=ArrayNode(j,0) Then    
                    %>
                    <option value="<%=ArrayNode(j,0) %>" selected="selected"><%=ArrayNode(j,1) %></option>
                    <% Else%>
                    <option value="<%=ArrayNode(j,0) %>"><%=ArrayNode(j,1) %></option>
                    <% 
                        End If
                        Next 
                        Set ArrayNode = nothing
                    %>
                    </select>               
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">排序：</td>
                <td>
                    <input name="in_SortID" type="text" class="text" value="<%=sortID %>" />           
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">模块名称：</td>
                <td>
                    <select name="in_GameID" style="width:157px;">
			            <% 
			                Dim ArrayGameID,GameID_i
                            ArrayGameID = CxGame.GetGameList()
                            For GameID_i=0 To Ubound(ArrayGameID)    
                            If gameID=ArrayGameID(GameID_i,0) Then   
			            %>
			            <option value="<%=ArrayGameID(GameID_i,0) %>" selected="selected"><%=ArrayGameID(GameID_i,1) %></option>
			            <% Else %>
			            <option value="<%=ArrayGameID(GameID_i,0) %>"><%=ArrayGameID(GameID_i,1) %></option>
			            <% 
                            End If                        
                            Next  
                            Set ArrayGameID = nothing
                        %>
			        </select>                 
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">桌子数量：</td>
                <td>
                    <input name="in_TableCount" type="text" class="text" value="<%=tableCount %>" />         
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">房间类型：</td>
                <td>
                    <select name="in_ServerType" style="width:157px;">
                    <option value="1"<% If serverType=1 Then%> selected="selected" <% End If %>>点值类型</option>
                    <option value="2"<% If serverType=2 Then%> selected="selected" <% End If %>>金币类型</option>
                    <option value="4"<% If serverType=4 Then%> selected="selected" <% End If %>>比赛类型</option>
                    <option value="8"<% If serverType=8 Then%> selected="selected" <% End If %>>训练类型</option>
                    </select>       
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">服务端口：</td>
                <td>
                    <input name="in_ServerPort" type="text" class="text" value="<%=serverPort %>" />
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">游戏数据库名称：</td>
                <td>
                    <input name="in_GameDataBaseName" type="text" class="text" value="<%=dataBaseName %>" />
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">游戏数据库地址：</td>
                <td>
                    <select name="in_GameDataBaseAddr" style="width:157px;">
			            <% 
			                Dim ArrayGameDB,Game_i
                            ArrayGameDB = CxGame.GetDataBaseList()
                            For Game_i=0 To Ubound(ArrayGameDB)    
                            If dataBaseAddr=ArrayGameDB(Game_i,0) Then   
			            %>
			            <option value="<%=ArrayGameDB(Game_i,0) %>" selected="selected"><%=ArrayGameDB(Game_i,1) %></option>
			            <% Else %>
			            <option value="<%=ArrayGameDB(Game_i,0) %>"><%=ArrayGameDB(Game_i,1) %></option>
			            <% 
                            End If                        
                            Next  
                            Set ArrayGameDB = nothing
                        %>
			        </select>       
                </td>
            </tr>           
            <tr>
                <td class="listTdLeft">单位积分：</td>
                <td>
                    <input name="in_CellScore" type="text" class="text" value="<%=cellScore %>" />
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">税收比例：</td>
                <td>
                    <input name="in_RevenueRatio" type="text" class="text" value="<%=revenueRatio %>" />
                </td>
            </tr>            
            <tr>
                <td class="listTdLeft">限制积分：</td>
                <td>
                    <input name="in_RestrictScore" type="text" class="text" value="<%=restrictScore %>" />
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">最低积分：</td>
                <td>
                    <input name="in_MinTableScore" type="text" class="text" value="<%=minTableScore %>" />
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">最小进入积分：</td>
                <td>
                    <input name="in_MinEnterScore" type="text" class="text" value="<%=minEnterScore %>" />
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">最大进入积分：</td>
                <td>
                    <input name="in_MaxEnterScore" type="text" class="text" value="<%=maxEnterScore %>" />
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">最小进入等级：</td>
                <td>
                    <input name="in_MinEnterMember" type="text" class="text" value="<%=minEnterMember %>" />
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">最大进入等级：</td>
                <td>
                    <input name="in_MaxEnterMember" type="text" class="text" value="<%=maxEnterMember %>" />
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">最大玩家数目：</td>
                <td>
                    <input name="in_MaxPlayer" type="text" class="text" value="<%=maxPlayer %>" />
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">房间规则：</td>
                <td>
                    <input name="in_ServerRule" type="text" class="text" value="<%=serverRule %>" />
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">附加属性：</td>
                <td>
                    <input name="in_AttachUserRight" type="text" class="text" value="<%=attachUserRight %>" />
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">服务机器：</td>
                <td>
                    <input name="in_ServiceMachine" type="text" class="text" value="<%=serviceMachine %>" />
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">自定规则：</td>
                <td>
                    <input name="in_CustomRule" type="text" class="text" style="width:500px;" value="<%=customRule %>" />
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">冻结状态：</td>
                <td>
                    <input name="in_Nullity" id="in_Nullity0" type="radio" value="0"<% If nullity=0 Then %> checked="checked"<% End If %> /><label for="in_Nullity0">正常</label>
                    <input name="in_Nullity" id="in_Nullity1" type="radio" value="1"<% If nullity=1 Then %> checked="checked"<% End If %> /><label for="in_Nullity1">冻结</label>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">备注：</td>
                <td>
                    <input name="in_ServerNote" type="text" class="text" style="width:500px;" value="<%=serverNote %>" />
                </td>
            </tr>
            <% If Request("id")>0 Then%>
            <tr>
                <td class="listTdLeft">创建时间：</td>
                <td>
                    <label name="in_CreateDateTime"><%=createDateTime %></label>
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">修改时间：</td>
                <td>
                    <label name="in_ModifyDateTime"><%=modifyDateTime %></label>
                </td>
            </tr>
            <% End If %>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('GameRoomInfo.asp')" />                     
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

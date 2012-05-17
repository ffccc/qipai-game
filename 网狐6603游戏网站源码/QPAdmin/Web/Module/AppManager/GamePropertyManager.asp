<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../function.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../conn.asp"-->
<!--#include file="../../md5.asp"-->
<!--#include file="../../Cls_Page.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript" src="../../Js/Calendar.js"></script>
    <script type="text/javascript" src="../../Js/Sort.js"></script>
</head>
<%
    If CxGame.GetRoleValue("305",trim(session("AdminName")))<"1" Then
    response.redirect "/Index.asp"   
    End If
%>

<body>

    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">目前操作功能：道具管理</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
<%
     Select case Request("action")
            case "flowercateinfo"
                Call GetFlowerCateInfo()
            case "save"
                Call SaveFlowerCateInfo()
            case else
                Call Main()
     End Select 
     
       '删除
        Sub Delete(lID)
            Dim ID,KindID,dname
            ID = lID
            GameConn.execute "Delete From GameProperty Where ID="&lID
        End Sub
        
        '禁用还原
        Sub Nullity(lID,typeValue)
            Dim ID,KindID,dname
            ID = lID
            GameConn.execute "Update GameProperty Set Nullity="&typeValue&" Where ID="&lID
        End Sub
       '更新
        Sub SaveFlowerCateInfo()
            Dim ID,sql,rs
            ID=Request("id")
            Call ConnectGame(QPTreasureDB)
            Set rs=Server.CreateObject("Adodb.RecordSet")
            '发行范围
            IssueAreaArr = Split(Request("in_IssueArea"),",")
            IssueArea=0
            For RL_i=0 To UBound(IssueAreaArr)
                IssueArea = IssueArea Or IssueAreaArr(RL_i)
            Next
            '使用范围范围
            ServiceAreaArr = Split(Request("in_ServiceArea"),",")
            ServiceArea=0
            For SL_i=0 To UBound(ServiceAreaArr)
                ServiceArea = ServiceArea Or ServiceAreaArr(SL_i)
            Next
            dim re
            re=""
            If ID<>""  Then
                re = GameConn.execute("WSP_PM_GameProperty_OP '"&Request("in_ID")&"','"&ID&"','"&Request("Name")&"','"&Request("Cash")&"','"&Request("Gold")&"','"&Request("Discount")&"','"&IssueArea&"','"&ServiceArea&"','"&Request("SendUserCharm")&"','"&Request("RcvUserCharm")&"','"&Request("RegulationsInfo")&"','Update'")(0)
                
                Call CxGame.ShowInfo2(re,"GamePropertyManager.asp",1200)
            Else
                re = GameConn.execute("WSP_PM_GameProperty_OP '"&Request("in_ID")&"','"&Request("in_ID")&"','"&Request("Name")&"','"&Request("Cash")&"','"&Request("Gold")&"','"&Request("Discount")&"','"&IssueArea&"','"&ServiceArea&"','"&Request("SendUserCharm")&"','"&Request("RcvUserCharm")&"','"&Request("RegulationsInfo")&"','Insert'")(0)
               
                Call CxGame.ShowInfo2(re,"GamePropertyManager.asp",1200)
            End If
            Set rs = Nothing  
            Call Main()
        End Sub
       
     Sub Main()
            '连接数据库
            Dim dname
            dname="QPTreasureDB"
            Call ConnectGame(dname)
            '操作
            Dim cidArray, lLoop,acType
            cidArray = Split(Request("cid"),",")
            acType = Request("action")
            For lLoop=0 To UBound(cidArray)
                Select case acType
                    case "del"
                        Call Delete(cidArray(lLoop))
                    case "jinyong"
                        Call Nullity(cidArray(lLoop),1)
                    case "huanyuan"
                        Call Nullity(cidArray(lLoop),0)
                End Select
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
                OrderStr = "ID DESC"
            End If
            Dim field
            field = "ID,Name,Cash,Gold,Discount,IssueArea,ServiceArea,SendLoveLiness,RecvLoveLiness,Nullity"
          
            Dim startDate,endDate
            
           
            Dim sql,userID
           
           
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = GetPageSize					'每页记录条数
                .PageParm = "page"				'页参数
                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	            .Pkey=""					'主键
	            .Field=field	'字段
	            .Table="GameProperty"			'表名
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
        function Operate(opType)
        {
            var opVal = document.myformList.in_optype
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
                    {
                        if(cids[i].name == "cid")
                            cid+=cids[i].value+",";
                    }
                }       
                if(cid=="")
                {
                    showError("未选中任何数据");
                    return;
                }
                //操作处理
                if(opType=="del")
                    opVal.value = "del";
                if(opType=="jinyong")
                    opVal.value = "jinyong";
                if(opType=="huanyuan")
                    opVal.value = "huanyuan";
            }
           
            document.myformList.action = "?action="+opVal.value;
            document.myformList.submit();
        }
          
      
    </script>
    <form name="myformList" method="post" action=''>
        <% If Request("action") = "del" Then %>
        <script type="text/javascript">
            showInfo("删除成功")
        </script>
        <% End If %>
        <% If Request("action") = "jinyong" Then %>
        <script type="text/javascript">
            showInfo("禁用成功")
        </script>
        <% End If %>
        <% If Request("action") = "huanyuan" Then %>
        <script type="text/javascript">
            showInfo("还原成功")
        </script>
        <% End If %>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" >
            <tr>
                <td height="39" class="titleOpBg">
                    <%
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("305",trim(session("AdminName")))="3" or CxGame.GetRoleValue("305",trim(session("AdminName")))="1" or CxGame.GetRoleValue("305",trim(session("AdminName")))="7" Then
                    %>
                    <!--<input type="button" class="btn wd1" onclick="Redirect('GamePropertyManager.asp?action=flowercateinfo')" value="新增"     />  -->
	                <input type="button" value="删除" class="btn wd1" onclick="Operate('del')" />
                    <input type="button" value="禁用" class="btn wd1" onclick="Operate('jinyong')"/>
                    <input type="button" value="还原" class="btn wd1" onclick="Operate('huanyuan')"/>
                     <input name="in_optype" type="hidden" />  
                    <%
                        End If
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("305",trim(session("AdminName")))>"3" Then
                    %>       
                    <%
                        End If
                    %>    
                </td>
            </tr>
        </table>  
        <div id="content">
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box">
                <tr align="center" class="bold">
                    <th class="listTitle1" width='38' align='center'><input type="checkbox" name="chkAll" onclick="SelectAll(this.checked);" /></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('Name',this)">道具名称</a></th>           
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('Cash',this)">现金价格</a></th>              
                    <th class="listTitle2"><a  class="l1" href="" onclick="GetOrder('Gold',this)">金币价格</a></th>
                    <th class="listTitle2"><a  class="l1" href="" onclick="GetOrder('Discount',this)">折扣价</a></th>   
                    <th class="listTitle2"><a  class="l1" href="" onclick="GetOrder('IssueArea',this)">发行范围</a></th>  
                    <th class="listTitle2"><a  class="l1" href="" onclick="GetOrder('ServiceArea',this)">使用范围</a></th>  
                    <th class="listTitle2"><a  class="l1" href="" onclick="GetOrder('SendLoveLiness',this)">赠送魅力</a></th>  
                    <th class="listTitle2"><a  class="l1" href="" onclick="GetOrder('RecvLoveLiness',this)">接收魅力</a></th>    
                    <th class="listTitle"><a  class="l1" href="" onclick="GetOrder('Nullity',this)">禁止标识</a></th>               
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
                    <td><input name='cid' type='checkbox' value='<%=rs(0,i)%>'/></td>                  
                    <td onclick="Redirect('GamePropertyManager.asp?action=flowercateinfo&id=<%=rs(0,i) %>')"><%=rs(1,i) %></td>   
                    <td onclick="Redirect('GamePropertyManager.asp?action=flowercateinfo&id=<%=rs(0,i) %>')"><%=rs(2,i) %></td>
                    <td onclick="Redirect('GamePropertyManager.asp?action=flowercateinfo&id=<%=rs(0,i) %>')"><%=rs(3,i) %></td>  
                    <td onclick="Redirect('GamePropertyManager.asp?action=flowercateinfo&id=<%=rs(0,i) %>')"><%=rs(4,i) %></td> 
                    <td onclick="Redirect('GamePropertyManager.asp?action=flowercateinfo&id=<%=rs(0,i) %>')"><%=CxGame.GetIssueAreae(rs(5,i)) %></td>
                    <td onclick="Redirect('GamePropertyManager.asp?action=flowercateinfo&id=<%=rs(0,i) %>')"><%=CxGame.GetServiceArea(rs(6,i)) %></td>  
                    <td onclick="Redirect('GamePropertyManager.asp?action=flowercateinfo&id=<%=rs(0,i) %>')"><%=rs(7,i) %></td>  
                    <td onclick="Redirect('GamePropertyManager.asp?action=flowercateinfo&id=<%=rs(0,i) %>')"><%=rs(8,i) %></td>    
                    <td onclick="Redirect('GamePropertyManager.asp?action=flowercateinfo&id=<%=rs(0,i) %>')"><%=CxGame.GetNullityInfo(rs(9,i)) %></td>                      
                </tr>
                <% 
                    Next                    
                    End If
                %>
            </table>           
            </div> 
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
                <tr>
                    <td class="listTitleBg" style="height: 19px"><span>选择：</span>&nbsp;<a class="l1" href="javascript:SelectAll(true);">全部</a>&nbsp;-&nbsp;<a class="l1" href="javascript:SelectAll(false);">无</a></td>
                    <td class="page" align="right"><%Response.Write nav%></td>
                </tr>
            </table>  
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" id="OpList">
            <tr>
                <td height="39" class="titleOpBg">
	                <%
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("305",trim(session("AdminName")))="3" or CxGame.GetRoleValue("305",trim(session("AdminName")))="1" or CxGame.GetRoleValue("305",trim(session("AdminName")))="7" Then
                    %>
                    <!--<input type="button" class="btn wd1" onclick="Redirect('GamePropertyManager.asp?action=flowercateinfo')" value="新增"     />-->  
                     
	                <input type="button" value="删除" class="btn wd1" onclick="Operate('del')" />
                    <input type="button" value="禁用" class="btn wd1" onclick="Operate('jinyong')"/>
                    <input type="button" value="还原" class="btn wd1" onclick="Operate('huanyuan')"/>
                    <%
                        End If
                        If trim(session("AdminName"))="admin" or CxGame.GetRoleValue("305",trim(session("AdminName")))>"3" Then
                    %>       
                    <%
                        End If
                    %>  
                </td>
            </tr>
        </table>  
    </form>
    <% 
        Call CloseGame()
        End Sub
        
        Sub GetFlowerCateInfo()
            Dim rs,sql,titleInfo
            Dim id,Name,Cash,SendUserCharm,RcvUserCharm,DisCount,IssueArea,ServiceArea,RegulationsInfo,Gold
            If Request("id")<>"" Then
                Set rs=Server.CreateObject("Adodb.RecordSet")
                Call ConnectGame(QPTreasureDB)
                sql="select *  from  GameProperty where  ID="&Request("id")
                rs.Open sql,GameConn,1,3
                id=rs("ID")
                Name=rs("Name")
                Cash=rs("Cash")
                Gold=rs("Gold")
                SendUserCharm=rs("SendLoveLiness")
                RcvUserCharm=rs("RecvLoveLiness")
                DisCount=rs("DisCount")
                IssueArea=rs("IssueArea")
                ServiceArea=rs("ServiceArea")
                RegulationsInfo=rs("RegulationsInfo")
                titleInfo = "新增道具信息"
            Else
                id=""
                Name=""
                Cash=""
                Gold=""
                SendUserCharm=""
                RcvUserCharm=""
                DisCount=""
                IssueArea=""
                ServiceArea=""
                RegulationsInfo=""
                titleInfo = "修改道具信息"   
            End If
    %>
    <script>
        function CheckFormInfo()
        {
            var name = document.form1.Name.value;
            var cash = document.form1.Cash.value;
            var ID = document.form1.in_ID.value;
            var gold = document.form1.Gold.value;
            var sendUserCharm = document.form1.SendUserCharm.value;
            var rcvUserCharm = document.form1.RcvUserCharm.value;
            var disCount = document.form1.DisCount.value;
            var regulationsInf = document.form1.RegulationsInfo.value;
            
            if(Trim(name)=="")
            {
                alert("道具名称不能为空！");
                return false;
            }
            else if(len(name)>32)
            {
                alert("道具名称字符长度不可超过32个字符！");
                return false;
            }
            if(IsPositiveInt(ID)==false)
            {
                alert("道具ID非数值型字符！");
                return false;
            }
            if(IsPositiveInt(cash)==false)
            {
                alert("道具价格非数值型字符！");
                return false;
            }
            if(IsPositiveInt(gold)==false)
            {
                alert("道具金币非数值型字符！");
                return false;
            }
            if(IsPositiveInt(disCount)==false)
            {
                alert("折扣价非数值型字符！");
                return false;
            }
            if(IsPositiveInt(sendUserCharm)==false)
            {
                alert("赠送魅力值非数值型字符！");
                return false;
            }
            if(IsPositiveInt(rcvUserCharm)==false)
            {
                alert("接收魅力值非数值型字符！");
                return false;
            }
            if(Trim(regulationsInf)=="")
            {
                alert("道具说明不能为空！");
                return false;
            }
            else if(len(regulationsInf)>255)
            {
                alert("道具说明字符长度不可超过255个字符！")
                return false;
            }
        }
    </script>
     <form name="form1" method="post" action='?action=save&id=<%=id %>' onsubmit="return CheckFormInfo()">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">          
                    <input type="submit" value="保存" class="btn wd1" />   
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('GamePropertyManager.asp')" />      
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">基本信息</div></td>
            </tr>
            <tr>
	            <td class="listTdLeft"> 道具名称：</td>
	            <td>
	                <input name="Name" type="text" class="text wd4" value="<%=Name %>"/>       
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft"> 道具ID：</td>
	            <td>
	                <input name="in_ID" type="text" class="text wd4" value="<%=id %>"/>       
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">道具价格：</td>
	            <td>
	                <input name="Cash" type="text" class="text wd4" value="<%=Cash %>"/>                  
	            </td>
	        </tr> 
	        <tr>
	            <td class="listTdLeft">道具金币：</td>
	            <td>
	                <input name="Gold" type="text" class="text wd4" value="<%=Gold %>"/>                  
	            </td>
	        </tr> 
	        <tr>
	            <td class="listTdLeft">折扣：</td>
	            <td>
	                <input name="DisCount" type="text" class="text wd4" value="<%=DisCount %>" />%              
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">赠送魅力值：</td>
	            <td>
	                <input name="SendUserCharm" type="text" class="text wd4" value="<%=SendUserCharm %>" />
	            </td>
	        </tr>
	        <tr>
	            <td class="listTdLeft">接收魅力值：</td>
	            <td>
	                <input name="RcvUserCharm" type="text" class="text wd4" value="<%=RcvUserCharm %>" />
	            </td>
	        </tr>
	        <%
	            If IssueArea<>"" Then
	        %>
	        <tr>
	            <td class="listTdLeft">发行范围：</td>
	            
	            <td>
	                <input name="in_IssueArea" id="in_IssueArea1" type="checkbox" value="1"<% If IssueArea And 1 Then %> checked='checked'<% End If %>/><label for="in_IssueArea1">商城道具</label>        
                    <input name="in_IssueArea" id="in_IssueArea2" type="checkbox" value="2"<% If IssueArea And 2 Then %> checked='checked'<% End If %>/><label for="in_IssueArea2">游戏道具</label>
                    <input name="in_IssueArea" id="in_IssueArea3" type="checkbox" value="4"<% If IssueArea And 4 Then %> checked='checked'<% End If %>/><label for="in_IssueArea3">房间道具</label>
	            </td>
	        </tr>
	         <tr>
	            <td class="listTdLeft">使用范围：</td>
	            <td>
	                <input name="in_ServiceArea" id="in_ServiceArea1" type="checkbox" value="1"<% If ServiceArea And 1 Then %> checked='checked'<% End If %>/><label for="in_ServiceArea1">自己范围</label>        
                    <input name="in_ServiceArea" id="in_ServiceArea2" type="checkbox" value="2"<% If ServiceArea And 2 Then %> checked='checked'<% End If %>/><label for="in_ServiceArea2">玩家范围</label>
                    <input name="in_ServiceArea" id="in_ServiceArea3" type="checkbox" value="4"<% If ServiceArea And 4 Then %> checked='checked'<% End If %>/><label for="in_ServiceArea3">旁观范围</label>             
	            </td>
	        </tr>
	        <%
	            Else
	        %>
	        <tr>
	            <td class="listTdLeft">发行范围：</td>
	            
	            <td>
	                <input name="in_IssueArea" id="Checkbox1" type="checkbox" value="1"/><label for="in_IssueArea1">商城道具</label>        
                    <input name="in_IssueArea" id="Checkbox2" type="checkbox" value="2"/><label for="in_IssueArea2">游戏道具</label>
                    <input name="in_IssueArea" id="Checkbox3" type="checkbox" value="4"/><label for="in_IssueArea3">房间道具</label>
	            </td>
	        </tr>
	         <tr>
	            <td class="listTdLeft">使用范围：</td>
	            <td>
	                <input name="in_ServiceArea" id="Checkbox4" type="checkbox" value="1"/><label for="in_ServiceArea1">自己范围</label>        
                    <input name="in_ServiceArea" id="Checkbox5" type="checkbox" value="2"/><label for="in_ServiceArea2">玩家范围</label>
                    <input name="in_ServiceArea" id="Checkbox6" type="checkbox" value="4"/><label for="in_ServiceArea3">旁观范围</label>             
	            </td>
	        </tr>
	        <%
	            End If
	        %>
	        <tr>
                <td class="listTdLeft">使用说明：</td>
                <td>
                    <input name="RegulationsInfo" type="text" class="text wd4" style="width:500px;" value="<%=RegulationsInfo %>"/>&nbsp;使用说明字符长度不可超过256个字符                                                           
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">            
                    <input type="submit" value="保存" class="btn wd1" />   
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('GamePropertyManager.asp')" />    
                </td>
            </tr>
        </table>    
    </form>
    <%
        End Sub 
     %>
</body>
</html>

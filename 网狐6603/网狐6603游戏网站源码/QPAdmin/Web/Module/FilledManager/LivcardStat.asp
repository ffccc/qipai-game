<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>   
    <script type="text/javascript" src="../../Js/comm.js"></script>   
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
	                <li class="tab1">库存统计</li>
	                <li class="tab2" onclick="openWindowSelf('LivcardGlobal.asp')">类型管理</li>
                </ul>
            </td>
        </tr>
    </table> 
    <% 
        Call ConnectGame(QPTreasureDB)
            Call Main()
        Call CloseGame()
        
        Sub Main()
            Dim rsStat,Array
            Array = Null
            Set rsStat = GameConn.execute("WSP_PM_LivcardStat")
            If Not rsStat.eof Then
            Array = rsStat.GetRows()
            Set rsStat= Nothing
            End If
    %>
    <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box Tmg7">
        <tr align="center" class="bold">
            <th class="listTitle1">会员卡</th>
            <th class="listTitle2">库存</th>
            <th class="listTitle2">已充值</th>
            <th class="listTitle2">未充值</th>
            <th class="listTitle2">已过期</th>
            <th class="listTitle2">冻结</th>
            <th class="listTitle2">总金额</th>
            <th class="listTitle">总金币</th>
        </tr>
        <% 
            Dim rs,sql,index,i,ret,className,rowNum
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from GlobalLivcard(nolock)"
            rs.Open sql,GameConn,1,3 
            If rs.EOF Then
                Response.Write("<tr class='tdbg'><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
            Else      
            do until rs.eof 
            index=0
            '行样式
            rowNum=rowNum+1
            If rowNum Mod 2 = 0 Then
                className="list"        
            Else
                className="listBg"      
            End If 
            
            If IsNull(Array) Then
                ret = false
            Else
                For i=0 To UBound(Array,2)
                    If rs("CardTypeID") = Array(0,i) Then
                        ret = true  
                        Exit For
                    Else
                        ret = false
                    End If
                index=index+1
                Next   
            End If
            If ret = true Then
        %>
            <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='default';"
                    onmouseout="this.style.backgroundColor=currentcolor">
                <td><%=CxGame.GetCardNameByCardID(rs("CardTypeID")) %></td>    
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=<%=rs("CardTypeID") %>&TypeID=1','cardList',900,550)"><%=Array(1,index) %> 张</a></td>   
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=<%=rs("CardTypeID") %>&TypeID=2','cardList',900,550)"><%=Array(2,index) %> 张</a></td>   
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=<%=rs("CardTypeID") %>&TypeID=3','cardList',900,550)"><%=Array(3,index) %> 张</a></td>   
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=<%=rs("CardTypeID") %>&TypeID=4','cardList',900,550)"><%=Array(4,index) %> 张</a></td>   
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=<%=rs("CardTypeID") %>&TypeID=5','cardList',900,550)"><%=Array(5,index) %> 张</a></td>   
                <td><%=FormatNumber(Array(6,index)) %></td>
                <td><%=FormatNumber(Array(7,index)) %></td>
            </tr>
            <% Else %>
            <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='default';"
                    onmouseout="this.style.backgroundColor=currentcolor">
                <td><%=CxGame.GetCardNameByCardID(rs("CardTypeID")) %></td>    
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=<%=rs("CardTypeID") %>&TypeID=1','cardList',900,550)">0 张</a></td> 
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=<%=rs("CardTypeID") %>&TypeID=2','cardList',900,550)">0 张</a></td> 
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=<%=rs("CardTypeID") %>&TypeID=3','cardList',900,550)">0 张</a></td> 
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=<%=rs("CardTypeID") %>&TypeID=4','cardList',900,550)">0 张</a></td> 
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=<%=rs("CardTypeID") %>&TypeID=5','cardList',900,550)">0 张</a></td> 
                <td>0</td>
                <td>0</td>
            </tr>     
        <% 
            End If
            rs.movenext
            loop            
            End If
            
            '汇总行处理
            If IsNull(Array) Then
        %>
            <tr class="list" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='default';"
                    onmouseout="this.style.backgroundColor=currentcolor">
                <td>合计：</td>    
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=0&TypeID=1','cardList',900,550)">0 张</a></td> 
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=0&TypeID=2','cardList',900,550)">0 张</a></td> 
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=0&TypeID=3','cardList',900,550)">0 张</a></td> 
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=0&TypeID=4','cardList',900,550)">0 张</a></td> 
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=0&TypeID=5','cardList',900,550)">0 张</a></td> 
                <td>0</td>
                <td>0</td>
            </tr> 
        <% Else %>
             <tr class="list" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='default';"
                    onmouseout="this.style.backgroundColor=currentcolor">
                <td>合计：</td>    
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=0&TypeID=1','cardList',900,550)"><%=Array(1,UBound(Array,2)) %> 张</a></td>
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=0&TypeID=2','cardList',900,550)"><%=Array(2,UBound(Array,2)) %> 张</a></td>
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=0&TypeID=3','cardList',900,550)"><%=Array(3,UBound(Array,2)) %> 张</a></td>
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=0&TypeID=4','cardList',900,550)"><%=Array(4,UBound(Array,2)) %> 张</a></td>
                <td><a href="javascript:void(0)" class="l" onclick="javascript:openWindowOwn('LivcardListByStat.asp?CardTypeID=0&TypeID=5','cardList',900,550)"><%=Array(5,UBound(Array,2)) %> 张</a></td>
                <td><%=FormatNumber(Array(6,UBound(Array,2))) %></td>
                <td><%=FormatNumber(Array(7,UBound(Array,2))) %></td>
            </tr>    
        <% End If %>
    </table>
    <% 
        End Sub
    %>
</body>
</html>

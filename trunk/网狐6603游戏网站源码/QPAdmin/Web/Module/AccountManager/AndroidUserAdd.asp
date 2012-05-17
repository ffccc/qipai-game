<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../function.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../Cls_Page.asp"-->
<!--#include file="../../conn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript" src="../../Js/Calendar.js"></script>
    <script type="text/javascript" src="../../Js/Sort.js"></script>
    <script type="text/javascript" src="../../Js/My97DatePicker/WdatePicker.js"></script>
    <script type="text/javascript">
    function setAction(type)
    {
        if(type==1)
        {
            document.getElementById("form2").action="?action=save";
            document.getElementById("oneAdd").style.display="";
            document.getElementById("moreAdd").style.display="none";
        }
        if(type==2)
        {
            document.getElementById("form2").action="?action=savemore";
            document.getElementById("oneAdd").style.display="none";
            document.getElementById("moreAdd").style.display="";
        }
    }
    </script>
</head>
<body>
   <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">目前操作功能：增加机器人</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <% 
        Select case lcase(Request("action"))
            case "save"
            Call Save()
             case "savemore"
            Call SaveMore()
            
        End Select
        
        Sub Save()
            Dim dbBase,rs,sql,userName,userID
            Dim MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,StartServiceTime,CloseServiceTime,ServiceGender,AndroidNote,Nullity,CreateDate 
            MinPlayDraw = CxGame.GetInfo(1,"MinPlayDraw")
            MaxPlayDraw = CxGame.GetInfo(1,"MaxPlayDraw")
            MinTakeScore = CxGame.GetInfo(1,"MinTakeScore")
            MaxTakeScore = CxGame.GetInfo(1,"MaxTakeScore")
            MinReposeTime = CxGame.GetInfo(1,"MinReposeTime")
            MaxReposeTime = CxGame.GetInfo(1,"MaxReposeTime")
            StartServiceTime = CxGame.GetInfo(0,"StartServiceTime")
            CloseServiceTime = CxGame.GetInfo(0,"CloseServiceTime")
            ServiceGender = CxGame.GetInfo(1,"ServiceGender")
            AndroidNote = CxGame.GetInfo(0,"AndroidNote")
            Nullity = CxGame.GetInfo(1,"Nullity")                               
            dbBase = Split(Request("RoomID"),",")
            userName=Request("Accounts")
            Call ConnectGame(dbBase(2))
            Set rs=Server.CreateObject("Adodb.RecordSet")
            If userName<>"" Then
                sql = "select userid from QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo where Accounts='"&userName&"' "'and IsAndroid=1"
                rs.Open sql,GameConn,1,3
                If rs.RecordCount=0 Then
                    CxGame.MessageBox("该机器人不存在！")
                Else
                    userID=rs("UserID")
                    sql = "select * from AndroidManager where userid =(select userid from QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo where Accounts='"&userName&"')"
                    Set rs=Server.CreateObject("Adodb.RecordSet")
                    rs.Open sql,GameConn,1,3
                    If rs.RecordCount>0 Then
                        CxGame.MessageBox("该机器人已经被添加！")
                    Else
                    Call ConnectGame(dbBase(2))
                        sql = "insert into AndroidManager(UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,StartServiceTime,CloseServiceTime,ServiceGender,AndroidNote,Nullity)" &_
			                "values("&userID &"," & dbBase(1) & ","&MinPlayDraw&","&MaxPlayDraw&","&MinTakeScore&","&MaxTakeScore&","&MinReposeTime&","&MaxReposeTime&",'"&StartServiceTime&"','"&CloseServiceTime&"',"&ServiceGender&",'"&AndroidNote&"',"&Nullity&")"
			        GameConn.execute sql
			        Call CxGame.MessageBoxReload("添加机器人"&userName&"成功！","AndroidUserInfo.asp")
                    End If
                End If
            Else
                CxGame.MessageBox("请输入机器名！")
            End If
            Set rs=Nothing
            Call CloseGame()
        End Sub
        
         Sub SaveMore()
            Dim dbBase,rs,sql,counts,userID,dbBaseLink,i,j
            j=0
            Dim MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,StartServiceTime,CloseServiceTime,ServiceGender,AndroidNote,Nullity,CreateDate 
            MinPlayDraw = CxGame.GetInfo(1,"MinPlayDraw")
            MaxPlayDraw = CxGame.GetInfo(1,"MaxPlayDraw")
            MinTakeScore = CxGame.GetInfo(1,"MinTakeScore")
            MaxTakeScore = CxGame.GetInfo(1,"MaxTakeScore")
            MinReposeTime = CxGame.GetInfo(1,"MinReposeTime")
            MaxReposeTime = CxGame.GetInfo(1,"MaxReposeTime")
            StartServiceTime = CxGame.GetInfo(0,"StartServiceTime")
            CloseServiceTime = CxGame.GetInfo(0,"CloseServiceTime")
            ServiceGender = CxGame.GetInfo(1,"ServiceGender")
            AndroidNote = CxGame.GetInfo(0,"AndroidNote")
            Nullity = CxGame.GetInfo(1,"Nullity")        
            
            dbBase = Split(Request("RoomID2"),",")
            counts= Request("counts")
            If counts<>"" Then
                Call ConnectGame(dbBase(2))
                Set rs=Server.CreateObject("Adodb.RecordSet")
                sql = "select userid from AndroidManager "
                rs.Open sql,GameConn,1,3
                i=0
                do until rs.eof 
                If i=0 Then
                    userID=rs("UserID")
                Else 
                    userID=userID&","&rs("UserID")
                End If
                i=i+1
		        rs.movenext
                loop
                If userID<>"" Then
                    Set rs=Nothing
                    Call CloseGame()
                    Call ConnectGame("QPAccountsDB")
                    Set rs=Server.CreateObject("Adodb.RecordSet")
                    sql = "select top "&counts&" IsAndroid,UserID from AccountsInfo where UserID not in ("&userID&") and IsAndroid=1"
                    rs.Open sql,GameConn,1,3
                    Call CloseGame()
                    If rs.RecordCount=0 Then                     
                        CxGame.MessageBox("无可用机器人！")
                    Else
                        Call ConnectGame(dbBase(2))
                        do until rs.eof 
                        Set userID=rs("UserID")
                        sql = "insert into AndroidManager(UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,StartServiceTime,CloseServiceTime,ServiceGender,AndroidNote,Nullity)" &_
			                "values("&userID &"," & dbBase(1) & ","&MinPlayDraw&","&MaxPlayDraw&","&MinTakeScore&","&MaxTakeScore&","&MinReposeTime&","&MaxReposeTime&",'"&StartServiceTime&"','"&CloseServiceTime&"',"&ServiceGender&",'"&AndroidNote&"',"&Nullity&")"
		                GameConn.execute sql
        			   
                        rs.movenext
                        j=j+1
                        loop
                        Call CxGame.MessageBoxReload("成功增加"&j&"个机器人！","AndroidUserInfo.asp")
                        Set rs=Nothing
                        Call CloseGame()
                    End If
                Else
                    Set rs=Nothing
                    Call CloseGame()
                    Call ConnectGame("QPAccountsDB")
                    Set rs=Server.CreateObject("Adodb.RecordSet")
                    sql = "select top "&counts&" IsAndroid,UserID from AccountsInfo where IsAndroid=1"
                    rs.Open sql,GameConn,1,3
                    Call CloseGame()
                    Call ConnectGame(dbBase(2))
                    do until rs.eof 
                    Set userID=rs("UserID")
                    sql = "insert into AndroidManager(UserID,ServerID,MinPlayDraw,MaxPlayDraw,MinTakeScore,MaxTakeScore,MinReposeTime,MaxReposeTime,StartServiceTime,CloseServiceTime,ServiceGender,AndroidNote,Nullity)" &_
			                "values("&userID &"," & dbBase(1) & ","&MinPlayDraw&","&MaxPlayDraw&","&MinTakeScore&","&MaxTakeScore&","&MinReposeTime&","&MaxReposeTime&",'"&StartServiceTime&"','"&CloseServiceTime&"',"&ServiceGender&",'"&AndroidNote&"',"&Nullity&")"
		            GameConn.execute sql
        			rs.movenext
        			j=j+1
                    loop
                    Call CxGame.MessageBoxReload("成功增加"&j&"个机器人！","AndroidUserInfo.asp")
                    Set rs=Nothing
                    Call CloseGame()
                End If
             End If
         End Sub
    %>
    <form name="form2" id="form2" method="post" action="?action=save">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" class="btn wd1" onclick="Redirect('AndroidUserInfo.asp')" value="返回" />   
                    <input type="submit" value="保存" class="btn wd1" onclick="return CheckCounts();"/>   
                   
                </td>
            </tr>
        </table>
        <table  width="100%" class="listBg2" cellpadding="0" cellspacing="0">
            <tr>
                 <td class="listTdLeft">
                    新增方式：
                </td>
                <td>
                    <input type="radio" name="rb" id="rb1" value="单笔新增" checked="checked" onclick="setAction('1')" /><label for="rb1">单笔新增</label>
                    <input type="radio" name="rb" id="rb2" value="批量新增" onclick="setAction('2')"/><label for="rb2">批量新增</label>
                </td>
            </tr>
        </table>
        <table id="oneAdd" width="100%" class="listBg2" cellpadding="0" cellspacing="0" >
            <tr>
                <td class="listTdLeft">
                    请填入机器人名：
                </td>
                <td>
                    <input type="text" name="Accounts" id="Accounts" class="text" style="width: 150px" />
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">
                    请选择游戏房间：
                </td>
                <td>
                     <select name="RoomID" id="Select1" style="width:157px;">
                     <%
                        Call ConnectGame("QPPlatformDB")
                          Dim rs,sql,GameName
                                Set rs=Server.CreateObject("Adodb.RecordSet")
                                sql = "select * from GameRoomInfo "
                                rs.Open sql,GameConn,1,3
            			 
		  	            IF Not rs.Eof Then
				            Do While Not rs.Eof
					            GameName=GetGameNameByID(rs("GameID"))
            				
		            %>
                    <option value="<%= rs("GameID") %>,<%= rs("ServerID")%>,<%= rs("DataBaseName")%>"><%=rs("ServerName") %></option>
                    <%
					    rs.MoveNext
				            Loop
			            Else
			
		            %>
                    <option value="0">没有任何房间</option>
                    <%
                        Set rs = Nothing
                        Call CloseGame()
			        End IF
		
		            %>
                  </select>
                   
                </td>
            </tr>
        </table>
         <table id="moreAdd"  width="100%" style="display:none;" class="listBg2" cellpadding="0" cellspacing="0">
             <tr>
                <td class="listTdLeft">
                    请填入新增机器人个数：
                </td>
                <td style="height: 24px;">
                    <input type="text" id="counts" name="counts" class="text"  style="width: 150px" />
                    
                    </td>
                    </tr>
                    <tr>
                    <td class="listTdLeft">
                    请选择游戏房间：
                    </td>
                    <td>
                   <select name="RoomID2" id="Select2" >
                   <% 
                   Call ConnectGame("QPPlatformDB")
                        Set rs=Server.CreateObject("Adodb.RecordSet")
                        sql = "select * from GameRoomInfo "
                        rs.Open sql,GameConn,1,3
			 
		  	        IF Not rs.Eof Then
				        Do While Not rs.Eof
					        GameName=GetGameNameByID(rs("GameID"))
				  %>
                  <option value="<%= rs("GameID") %>,<%= rs("ServerID")%>,<%= rs("DataBaseName")%>"><%= GameName&rs("ServerName") %></option>
                  <%
					rs.MoveNext
				        Loop
			        Else
		          %>
                  <option value="0">没有任何房间</option>
                  <%
                    Set rs = Nothing
                    Call CloseGame()
			        End IF
		
		          %>
		          </select>
                      
                </td>
            </tr>
        </table>
        <table id="Table1"  width="100%" class="listBg2" cellpadding="0" cellspacing="0">
            <tr>
                <td class="listTdLeft">最少局数：</td>
                <td>
                    <input name="MinPlayDraw" type="text" class="text" />    
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">最大局数：</td>
                <td>
                    <input name="MaxPlayDraw" type="text" class="text" />           
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">最少分数：</td>
                <td>
                    <input name="MinTakeScore" type="text" class="text" />              
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">最高分数：</td>
                <td>
                    <input name="MaxTakeScore" type="text" class="text" />              
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">最少休息：</td>
                <td>
                    <input name="MinReposeTime" type="text" class="text" />              
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">最大休息：</td>
                <td>
                    <input name="MaxReposeTime" type="text" class="text" />              
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">开始时间：</td>
                <td>
                    <input name="StartServiceTime" id="StartServiceTime" type="text" class="text"/><img src="../../Images/btn_calendar.gif" onclick="WdatePicker({el:'StartServiceTime',skin:'whyGreen',dateFmt:'yyyy-MM-dd HH:mm:ss'})" style="cursor:pointer;vertical-align:middle"/>                   
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">结束时间：</td>
                <td>
                    <input name="CloseServiceTime" id="CloseServiceTime" type="text" class="text" /><img src="../../Images/btn_calendar.gif" onclick="WdatePicker({el:'CloseServiceTime',skin:'whyGreen',dateFmt:'yyyy-MM-dd HH:mm:ss'})" style="cursor:pointer;vertical-align:middle"/>                  
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">服务类型：</td>
                <td>
                    <input name="ServiceGender" type="text" class="text" />              
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">备注信息：</td>
                <td>
                    <input name="AndroidNote" type="text" class="text" />              
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">禁用状态：</td>
                <td>
                    <input name="in_Nullity" id="Checkbox1" type="radio" value="0" checked="checked" /><label for="Checkbox1">启用</label> 
                    <input name="in_Nullity" id="Checkbox2" type="radio" value="1" /><label for="Checkbox2">冻结</label> 
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" class="btn wd1" onclick="Redirect('AndroidUserInfo.asp')" value="返回" />   
                    <input type="submit" value="保存" class="btn wd1" onclick="return CheckCounts();" />  
                 
                </td>
            </tr>
        </table>
    </form>
   <%
    Function GetGameNameByID( KindID)
     Call ConnectGame("QPPlatformDB")
    Dim lRs, lSql, GameName
    Set lRs =Server.CreateObject("Adodb.RecordSet")
    lSql="select KindName from GameKindItem where KindID = " & KindID
    lRs.Open lSql,GameConn,1,3
    If Not lRs.Eof Then
	    IF isnull(lRs(0)) Then
		    GetGameNameByID = ""
	    Else
		    GetGameNameByID = lRs(0)
	    End IF
    Else
	    GetGameNameByID = ""
    End IF

    lRs.Close
    Set lRs = Nothing
    End Function
   
    %>
</body>
</html>
<script type="text/javascript">
    function CheckCounts()
    {
        var isType=IsPositiveInt( document.getElementById("counts").value);
        var StartServiceTime = document.getElementById("StartServiceTime").value
        var CloseServiceTime = document.getElementById("CloseServiceTime").value
        if(document.getElementById("rb2").checked)
        {
            if(!isType)
            {
                alert("请输入整数！");
                return false
            }
            return true;
        }
        
        if(!isDateTime(StartServiceTime))
        {
            alert("开始时间格式不对！")
            return false;
        }
        if(!isDateTime(CloseServiceTime))
        {
            alert("结束时间格式不对！")
            return false;
        }
    }
</script>
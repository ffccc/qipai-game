
<!--#include file="Inc/Conn.asp" -->

<%
        Dim Query_Badword,Form_Badword,i,Err_Message,name,Chk_badword

        Err_Message = 1 

        Query_Badword="'‖and‖select‖update‖chr‖delete‖%20from‖;‖insert‖mid‖master.‖set‖chr(37)‖="     

        Form_Badword="'‖%‖&‖*‖#"    
      
        On Error Resume Next

        If request.QueryString<>"" then
            Chk_badword=split(Query_Badword,"‖")
            FOR EACH name IN Request.QueryString
                For i=0 to 9
                    If Instr(LCase(request.QueryString("CustomID")),Chk_badword(i))<>0 Then
                        
                        Select Case Err_Message
                          Case "1"
                       Response.Write "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'><Script charset='utf-8' type='text/javascript'>alert('传参错误！参数 "&name&"的值中包含非法字符串！\n\n请不要在参数中出现：and update delete ; insert mid master 等非法字符！');window.close();</Script>"
                          Case "2"
                        Response.Write "<Script>location.href='"&Err_Web&"'</Script>"
                        End Select
                    End If
                NEXT
            NEXT
        End If
        
        Dim rs,sql,fso
        Dim CustomFace
        Call ConnectGame(QPAccountsDB)
        Set rs=server.createobject("adodb.recordset")    
        If QueryString("CustomID")<>"" Then
            sql="select  * from AccountsFace where ID="&Request("CustomID")&" and UserID="&Request("UserID")
        End If
        rs.open sql,GameConn,1,3
        If  rs.eof  then  
            CustomFace=""
        Else

            Response.AddHeader "Content-Disposition", "attachment;filename=" &rs(1)&".txt"
            Response.BinaryWrite(rs(2))
            Response.End()
        End If  
   
        rs.Close       
        Set rs = nothing 

 %> 
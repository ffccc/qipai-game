<%
Dim sql_Injdata,sql_inj,sql_post,sql_data,sql_get
sql_Injdata = Lcase("'|and|exec|insert|update|count|chr|<%")
sql_inj = split(sql_Injdata,"|")

If Request.Form<>"" Then
    For Each sql_post In Request.Form
        For sql_data=0 To Ubound(sql_inj)
            IF Instr(Lcase(Request.Form(sql_post)),sql_inj(sql_data))>0 Then
                Response.Write "<Script Language=JavaScript>alert('SQL通用防注入系统提示\n\n请不要在参数中包含非法字符尝试注入！\n\n  ');history.back(-1)</Script>"
                Response.end
            END IF
        Next
    Next
END IF

If Request.QueryString<>"" Then
    For Each sql_get In Request.QueryString
        For sql_data=0 To Ubound(sql_inj)
            IF Instr(Lcase(Request.QueryString(sql_get)),sql_inj(sql_data))>0 Then
                Response.Write "<Script Language=JavaScript>alert('SQL通用防注入系统提示\n\n请不要在参数中包含非法字符尝试注入！\n\n ');history.back(-1)</Script>"
                Response.end
            END IF
        Next
    Next
End If
%>
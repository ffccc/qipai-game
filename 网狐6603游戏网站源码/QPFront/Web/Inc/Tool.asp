<% 
Class ToolObj

    Private Sub Class_Terminate()
    
	End Sub 
	
	Private Sub Class_Initialize  
	
	End Sub
	
	Rem 可自由选择不同的日期显示样式：
    '============================================
    ' 格式化时间(显示)
    ' 参数：n_Flag
    ' 1:"yyyy-mm-dd hh:mm:ss"
    ' 2:"yyyy-mm-dd"
    ' 3:"hh:mm:ss"
    ' 4:"yyyy年mm月dd日"
    ' 5:"yyyymmdd"
    ' ============================================
    Function Format_Time(s_Time, n_Flag)
        Dim y, m, d, h, mi, s
        Format_Time = ""
        If IsDate(s_Time) = False Then Exit Function
        y = cstr(year(s_Time))
        m = cstr(month(s_Time))
        If len(m) = 1 Then m = "0" & m
        d = cstr(day(s_Time))
        If len(d) = 1 Then d = "0" & d
        h = cstr(hour(s_Time))
        If len(h) = 1 Then h = "0" & h
        mi = cstr(minute(s_Time))
        If len(mi) = 1 Then mi = "0" & mi
        s = cstr(second(s_Time))
        If len(s) = 1 Then s = "0" & s
        Select Case n_Flag
            Case 1
              ' yyyy-mm-dd hh:mm:ss
              Format_Time = y & "-" & m & "-" & d & " " & h & ":" & mi & ":" & s
            Case 2
              ' yyyy-mm-dd
              Format_Time = y & "-" & m & "-" & d
            Case 3
              ' hh:mm:ss
              Format_Time = h & ":" & mi & ":" & s
            Case 4
              ' yyyy年mm月dd日
              Format_Time = y & "年" & m & "月" & d & "日"
            Case 5
              ' yyyymmdd
              Format_Time = y & m & d
        End Select
    End Function
    
    Rem 生成指定长度的数字随机数
    Function GetRandomNum(cardLength)
        Dim i,rValue
        for i=1 To cardLength
            rValue = rValue&GetRand(1,9)
        Next
        GetRandomNum = rValue
    End Function
    
    '生成一个随机数
    Function GetRand(min,max)
        Dim rValue
        Randomize
        rValue = Int((max - min + 1) * Rnd + min) 
        GetRand = rValue
    End Function

    Rem 生成指定长度日期格式前缀的订单号
    Function CreateOrderNum(preStr,orderLen)
        Dim datePrefix,realPrefix,cardRndNum,serialNo
        datePrefix = Year(Date())&Month(Date())&Day(Date())&Hour(Now())&Minute(Now())&Second(Now())
        If preStr="" Then
            realPrefix = datePrefix
        Else
            realPrefix = preStr&datePrefix
        End If
        cardRndNum = orderLen-Len(realPrefix)
        serialNo = realPrefix&GetRandomNum(cardRndNum)
        CreateOrderNum = serialNo
    End Function
End Class

Rem 建立类对象
Dim Tool
Set Tool=New ToolObj
%>
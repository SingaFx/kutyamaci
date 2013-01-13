#include <Debug.au3>

_DebugSetup("Log window client", True)

Global $g_IP = "127.0.0.1"
Global $g_PORT = 3001

Global $listening_socket
Global $listening_ip = IniRead("keyboard.ini", "connections", "serverip", "-1")
Global $listening_port = IniRead("keyboard.ini", "connections", "serverport", "-1")
Global $max_connections = 100
Global $cmd_max_length = 256

dbgOut("Connected to the server")

TCPStartup()
$listening_socket = TCPListen($listening_ip, $listening_port, $max_connections)
If $listening_socket = -1 Then
	MsgBox(0, "Error TCPListen", "Error: Cannot open port for the client - scheduler!")
	Exit
 EndIf
 
 while 1
   $connected_socket = -1
   while $connected_socket = -1
	  $connected_socket = TCPAccept($listening_socket)
   WEnd

   dbgOut("Client connected")

   Local $recv_encrypted
   Local $temp

   Local $parseB = 0
   Local $sendstr = ""
   
   While 1
		$recv_encrypted = ""
		While ($recv_encrypted == "") And Not @error
			$recv_encrypted = TCPRecv($connected_socket, $cmd_max_length)
		WEnd
		if @error then
		   ExitLoop
	    endif
		dbgOut("Message: " & $recv_encrypted)
		dbgOut("sendstr: " & $sendstr)
		
		if $parseB = 1 Then
		    $temp = StringSplit($recv_encrypted,"")
			if ($temp[1] >= '0' && $temp[1] <= '9') || $temp[1] == '.' Then
			   $sendstr = $sendstr & $recv_encrypted
			EndIf
			dbgOut("sendstr2: " & $sendstr)
			if (StringLen($sendstr) - StringInStr($sendstr, ".")) = 2 Then
			   Send($sendstr)
			   $sendstr = ""
			   $parseB = 0
			EndIf
	    EndIf
		 
		if $recv_encrypted == "{DEL}" Then
		   $parseB = 1
		EndIf
		
;~ 		$temp = StringSplit($recv_encrypted,"")
;~ 		for $i=1 to $temp[0]
;~ 		   Send($temp[$i])
;~ 		   dbgOut($temp[$i])
;~ 		next
   WEnd
WEnd

Func dbgOut($str)
   $curHwnd = WinGetHandle("")
   _DebugOut($str)
   WinActivate($curHwnd)
 EndFunc
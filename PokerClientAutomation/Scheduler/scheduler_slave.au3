#include <Crypt.au3>
#include <Constants.au3>
#include <Debug.au3>

;~  _DebugSetup("Log window", True) ; start displaying debug environment

 Global $max_connections = 10
 Global $cmd_max_length = 256
 Global $listening_socket, $recv

 Global $serverip = IniRead("slave.ini", "connection", "listenip", "-1")
 Global $serverport = IniRead("slave.ini", "connection", "listenport", "-1")
 Global $bwinscript = IniRead("slave.ini", "other", "bwinscript", "-1")
 Global $partyscript = IniRead("slave.ini", "other", "partyscript", "-1")
 Global $wptcript = IniRead("slave.ini", "other", "wptscript", "-1")
 Global $client = IniRead("slave.ini", "other", "client", "-1")

 TCPStartup()

 $listening_socket = TCPListen($serverip, $serverport, $max_connections)
 If $listening_socket = -1 Then
	 MsgBox(0, "Error TCPListen", "Error: Cannot open port for the slave !")
	 Exit
 EndIf
 
 ; Wait for and Accept a connection
 $connected_socket = -1
 Do
	 $connected_socket = TCPAccept($listening_socket)
 Until $connected_socket <> -1
 dbgOut("Master connected...")

 While 1
	$recv = ""
	While ($recv == "") And Not @error
		 $recv = TCPRecv($connected_socket, $cmd_max_length)
    WEnd
	dbgOut("Received : " & $recv)
	
	If @error Then
		MsgBox(0, "Error TCPRecv", "Received not a command !")
    Else
		 If ($recv == "START_BWIN") Then
			Run("C:\kutya\client_bwin\client.exe", "C:\kutya\client_bwin\")
		 EndIf
		 If ($recv == "START_PARTY") Then
			Run("C:\kutya\client_party\client.exe", "C:\kutya\client_party\")
		 EndIf
		 If ($recv == "START_WPT") Then
			Run("C:\kutya\client_wpt\client.exe", "C:\kutya\client_wpt\")
		 EndIf
		 If ($recv == "CLOSE_BWIN") Then
			closeSession("bwin")
		 EndIf
		 If ($recv == "CLOSE_PARTY") Then
			closeSession("Party")
		 EndIf
		 If ($recv == "CLOSE_WPT") Then
			closeSession("WPT")
		 EndIf
    EndIf
 WEnd
 
Func closeSession($lobby)
   Local $name = "Hold'em"
   AutoItSetOption("WinTitleMatchMode", 2)
   $array = WinList($name)
   Local $k
   for $k = 1 to $array[0][0]
	  Local $table = WinGetPos($array[$k][1])
	  MouseClick("left", $table[0] + 13, $table[1] + 330, 1, 1)
	  Sleep(500)
   Next
   
   Sleep(80000)

   for $k = 1 to $array[0][0]
	  WinClose($array[$k][1])
	  Local $table = WinGetPos($array[$k][1])
	  MouseClick("left", $table[0] + 200, $table[1] + 213, 1, 1)
   Next
   
   Sleep(1000)
   
   AutoItSetOption("WinTitleMatchMode", 2)
   $array = WinList($lobby)
   WinClose($array[1][1])
   Local $lobbyWindow = WinGetPos($array[1][1])
   MouseClick("left", $lobbyWindow[0] + 465, $lobbyWindow[1] + 400, 1)
   
   Local $updater_name  = "updater"
   AutoItSetOption("WinTitleMatchMode", 2)
   $array = WinList($updater_name)
   WinClose($array[1][1])
   
   ProcessClose("client.exe")
EndFunc

Func dbgOut($str)
;~ 	$curHwnd = WinGetHandle("")
;~ 	_DebugOut($str)
;~ 	WinActivate($curHwnd)
;~ 	Sleep(50)
EndFunc   ;==>dbgOut
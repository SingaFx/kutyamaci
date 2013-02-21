#include <Crypt.au3>
#include <Constants.au3>
#include <Debug.au3>

;~  _DebugSetup("Log window", True) ; start displaying debug environment

 Opt("TrayIconHide", 1)

 Global $max_connections = 10
 Global $cmd_max_length = 256
 Global $listening_socket, $recv

 Global $serverip = IniRead("slave.ini", "connection", "listenip", "-1")
 Global $serverport = IniRead("slave.ini", "connection", "listenport", "-1")
 Global $bwinscript = IniRead("slave.ini", "other", "bwinscript", "-1")
 Global $partyscript = IniRead("slave.ini", "other", "partyscript", "-1")
 Global $wptcript = IniRead("slave.ini", "other", "wptscript", "-1")
 Global $client = IniRead("slave.ini", "other", "client", "-1")

;Run("C:\kutya\kutya.exe -s 3000 tancoskurva 100", "C:\kutya\")

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
			Run("bwintime.bat")
			Sleep(1000)
			Run("C:\kutya\client_bwin\client.exe", "C:\kutya\client_bwin\")
		 EndIf
		 If ($recv == "START_PARTY") Then
			Run("Partyime.bat")
			Sleep(1000)
			Run("C:\kutya\client_party\client.exe", "C:\kutya\client_party\")
		 EndIf
		 If ($recv == "START_WPT") Then
			Run("WPTtime.bat")
			Sleep(1000)
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
		 If ($recv == "CHECK_BWIN") Then
			check("bwin")
		 EndIf
		 If ($recv == "SEND_MAIL") Then
			RunWait("php sendmail.php")
			DirRemove("c:\handhistory\", 1)
			FileDelete("c:\handhistory.zip")
		 EndIf
    EndIf
 WEnd
 
Func check($lobby)
   Local $name = "Hold'em"
   Local $tablemap = $lobby & 'client.tm'
   AutoItSetOption("WinTitleMatchMode", 2)
   Local $array = WinList($name)
   for $k = 1 to $array[0][0]
	  Local $text = _DllScrape_scrapeRegion($tablemap, $array[$k][1], "imback", "OpenScrapeDLL.dll",20)
	  if $text = 'imback' Then
		 _DllScrape_clickRegion("bwinclient.tm", $array[$k][1], "imback", "OpenScrapeDLL.dll",1)
	  EndIf
	  Sleep(100)
   Next
EndFunc   
 
Func closeSession($lobby)
   dbgOut("Closing the session")
   Local $name = "Hold'em"
   Local $tablemap = $lobby & 'client.tm'
   AutoItSetOption("WinTitleMatchMode", 2)
   Local $array = WinList($name)
   Local $k
   Local $text
   
   for $k = 1 to $array[0][0]
	  $text = ''
	  While $text <> 'iabk'
		 MouseMove(1, 1, 1)
		 $text = _DllScrape_scrapeRegion($tablemap, $array[$k][1], "imback", "OpenScrapeDLL.dll",20)
		 Sleep(5000)
	  WEnd
   Next

   for $k = 1 to $array[0][0]
	  WinActivate($array[$k][1])
	  Local $table = WinGetPos($array[$k][1])
	  WinClose($array[$k][1])
	  If IsArray($table) Then
		 Sleep(1000)
		 MouseClick("left", $table[0] + 200, $table[1] + 213, 1, 10)
		 MouseClick("left", $table[0] + 204, $table[1] + 213, 1, 10)
		 MouseClick("left", $table[0] + 200, $table[1] + 216, 1, 10)
		 MouseClick("left", $table[0] + 196, $table[1] + 213, 1, 10)
		 MouseClick("left", $table[0] + 196, $table[1] + 216, 1, 10)
	  Else
		 WinKill($array[$k][1])
;~ 		 MsgBox(0, "Error", "Couldn't get the position of the window")
	  EndIf
   Next
   
   Sleep(10000)
   
   WinActivate($lobby)
   AutoItSetOption("WinTitleMatchMode", 2)
   $lobby_hwnd = WinGetHandle($lobby)
   WinKill($lobby_hwnd)
   
   Local $lobbyWindow = WinGetPos($lobby_hwnd)
   if IsArray($lobbyWindow) Then
	  Sleep(1000)
	  MouseClick("left", $lobbyWindow[0] + 465, $lobbyWindow[1] + 400, 1, 10)
	  MouseClick("left", $lobbyWindow[0] + 460, $lobbyWindow[1] + 400, 1, 10)
	  MouseClick("left", $lobbyWindow[0] + 470, $lobbyWindow[1] + 400, 1, 10)
	  MouseClick("left", $lobbyWindow[0] + 465, $lobbyWindow[1] + 395, 1, 10)
	  MouseClick("left", $lobbyWindow[0] + 465, $lobbyWindow[1] + 405, 1, 10)
   Endif
   
   Sleep(5000)
   
   ProcessClose("bwin.exe")
   ProcessClose("bwincom.exe")
   ProcessClose("WPT.exe")
   ProcessClose("PartyGaming.exe")
   ProcessClose("updater.exe")
   ProcessClose("client.exe")
   
   While ProcessExists("chrome.exe")
	  ProcessClose("chrome.exe")
   WEnd
   
EndFunc

Func dbgOut($str)
;~ 	$curHwnd = WinGetHandle("")
;~ 	_DebugOut($str)
;~ 	WinActivate($curHwnd)
;~ 	Sleep(50)
EndFunc   ;==>dbgOut

;------- Functions -------

Func _DllScrape_scrapeRegion($nameTableMap, $hWnd, $regionName, $dllName, $offset)

	;Opening Dll
	Local $dll = DllOpen($dllName)

	;Loading Table Map
	_DllScrape_LoadTablemap($nameTableMap, $dll)

	;Let's read another region! (Color region)
	Local $text = _DllScrape_ReadRegionWithOffset($hWnd, $regionName, $offset,$dll)

	;Don't forge to close Dll
	DllClose($dll)

	Return $text

EndFunc

Func _DllScrape_clickRegion($nameTableMap, $hWnd, $regionName, $dllName, $numberClicks)

	;Opening Dll
	Local $dll = DllOpen($dllName)

	;Loading Table Map
	_DllScrape_LoadTablemap($nameTableMap,$dll)

	;Lets Get coord from the region
	Local $coord = _DllScrape_GetCoordRegion($regionName,$dll)

	Local $pos = WinGetPos ( $hWnd )

	Local $extraX = 4      ;Classic XP
	Local $extraY = 23     ;Classic XP

;~ 	$x = Random(0, $coord[4] - $coord[2], 1) + Int($coord[2]) + $pos[0] + $extraX
;~  $y = Random(0, $coord[5] - $coord[3], 1) + Int($coord[3]) + $pos[1] + $extraY
	$x = Int((($coord[4] - $coord[2]) / 2) + $coord[2] + $pos[0] + $extraX )
	$y = Int((($coord[5] - $coord[3]) / 2) + $coord[3] + $pos[1] + $extraY ) + 5

	MouseClick ( "left" , $x  , $y, $numberClicks, 1)

	;Don't forge to close Dll
	DllClose($dll)

EndFunc

;Function to load Table Map in Dll
Func _DllScrape_LoadTablemap($name, $dll)

	Local $res = DllCall($dll, "int:cdecl", "OpenTablemap", "str", $name)

	If (@error <> 0) Then
		ConsoleWrite("ERROR in dllcall(OpenTablemap) " & @error & @CRLF)
		Exit
	EndIf

	If ($res[0] = 0) Then
		ConsoleWrite("ERROR in OpenTablemap with map " & $name & @CRLF)
		Exit
	EndIf
EndFunc   ;==>LoadTablemap

;Function to read a region using a window ($hWnd) and name of region
Func _DllScrape_ReadRegion($hWnd, $name,$dll)
	Return _DllScrape_ReadRegionWithOffset($hWnd, $name, 0,$dll)
EndFunc   ;==>ReadRegion

;Function to read a region using a window ($hWnd) and name of region
Func _DllScrape_ReadRegionWithOffset($hWnd, $name, $offset, $dll)
	Local $res = DllCall($dll, "int:cdecl", "ReadRegion", "hwnd", $hWnd, "str", $name, "str*", "", "int", $offset)

	If (@error <> 0) Then
		ConsoleWrite("ERROR in dllcall(ReadRegion) " & @error & @CRLF)
		Exit
	EndIf

	Return $res[3]

EndFunc   ;==>ReadRegionWithOffset

;Function to get Coord from a region ($name)
Func _DllScrape_GetCoordRegion($name,$dll)
	Local $posl
	Local $posr
	Local $post
	Local $posb

	Local $res = DllCall($dll, "none:cdecl", "GetRegionPos", "str", $name, "int*", $posl, "int*", $post, "int*", $posr, "int*", $posb)

	if (@error <> 0) Then
		ConsoleWrite("ERROR in dllcall(ReadRegion) " & @error & @CRLF)
		Exit
	EndIf

	Return $res

EndFunc   ;==>GetCoordRegion
#include <Crypt.au3>
#include <Constants.au3>
#include <Debug.au3>

;~ _DebugSetup("Log window", True) ; start displaying debug environment

Global $slaveip = IniRead("master.ini", "connection", "slaveip", "-1")
Global $slaveport = IniRead("master.ini", "connection", "slaveport", "-1")
Global $OH_Mutex = IniRead("master.ini", "other", "ohmutex", "-1")

Global $script[4], $script_dir[4], $start_command[4], $end_command[4], $lobby[4], $oh_dir[4]

Global $Global_Mutex_Handle
Global $Mutex_Locked = 0

$script[0] = "serverOH_1_03_rel.exe"
$script_dir[0] = "c:\botting\stuff\BwinServerScript\"
$start_command[0] = "START_BWIN"
$end_command[0] = "CLOSE_BWIN"
$lobby[0] = "bwin"
$oh_dir[0] = "c:\botting\OH\ScriptBwin\"

$script[1] = "serverOH_1_03_rel.exe"
$script_dir[1] = "c:\botting\stuff\PartyServerScript\"
$start_command[1] = "START_PARTY"
$end_command[1] = "CLOSE_PARTY"
$lobby[1] = "Party"
$oh_dir[1] = "c:\botting\OH\ScriptParty\"

$script[2] = "serverOH_1_03_rel.exe"
$script_dir[2] = "c:\botting\stuff\WPTServerScript\"
$start_command[2] = "START_WPT"
$end_command[2] = "CLOSE_WPT"
$lobby[2] = "WPT"
$oh_dir[2] = "c:\botting\OH\ScriptWPT\"

;MutexLock()
;MutexUnlock()

TCPStartup()
Local $socket = TCPConnect($slaveip, $slaveport)

If $socket = -1 Then
   MsgBox(4096, "", "Cannot connect to server fool!")
   Exit
EndIf
SRandom(@MSEC)

Local $play
$play = Random(0, 2, 1)

While 1
   Local $sData = InetRead("ftp://scrazy:tancoskurva@scrazy.exavault.com/dllinterface.dll")
   
   Local $path = $oh_dir[0] & "dllinterface.dll"
   Local $file = FileOpen($path, 2)
   FileWrite($file, $sData)
   FileClose($file)
   $path = $oh_dir[1] & "dllinterface.dll" 
   Local $file = FileOpen($path, 2)
   FileWrite($file, $sData)
   FileClose($file)
   $path = $oh_dir[2] & "dllinterface.dll" 
   Local $file = FileOpen($path, 2)
   FileWrite($file, $sData)
   FileClose($file)
   
   $sData = InetRead("ftp://scrazy:tancoskurva@scrazy.exavault.com/session.ini")
   $path = "session.ini"
   Local $file = FileOpen($path, 2)
   FileWrite($file, $sData)
   FileClose($file)
   Local $sessionTime1 = IniRead("session.ini", "other", "time1", "3600")
   Local $sessionTime2 = IniRead("session.ini", "other", "time2", "3700")
   
   playSession($play, Random($sessionTime1, $sessionTime2, 1))
   
   Local $akt = $play
   While $akt = $play
	  $akt = Random(0, 2, 1)
   WEnd
   $play = $akt
WEnd

Func playSession($id, $time)
   DbgOut("Starting session: " & $id)
   Run($script_dir[$id] & $script[$id], $script_dir[$id])
   TCPSend($socket, $start_command[$id])
   
   Sleep(60000)
   
   check($id)
   
   Local $i
   
   For $i = 1 to $time
	  Sleep(1000)
	  if ProcessExists("OpenHoldem.exe") Then
		 check($id)
	  EndIf
   Next
   ;CLICK I'M BACK
   
   DbgOut("Sitting out session: " & $id)
   Local $k
   for $k = 1 to 15
	  ClickSitout($id)
	  Sleep(100)
   Next
   
   TCPSend($socket, $end_command[$id])
   
   Sleep(200000)

   While ProcessExists("OpenHoldem.exe")
	  ProcessClose("OpenHoldem.exe")
   WEnd
   
   ProcessClose($script[$id])
   DbgOut("Finished session: " & $id)
   RunWait("php sendMail.php")
   TCPSend($socket, "SEND_MAIL")
   DirRemove("c:\botting\log\", 1)
   FileDelete("c:\botting\log.zip")
   Sleep(1000)   
EndFunc

Func ClickSitout($id)
   Local $name = "^[^+].*Hold'em"
   Local $tablemap = $lobby[$id] & 'client.tm'
   AutoItSetOption("WinTitleMatchMode", 2)
   Local $array = WinList("[REGEXPTITLE:" & $name & "]")
   Local $k
   
   Local $text
   for $k = 1 to $array[0][0]
	  $text = 'false'
	  $text = _DllScrape_scrapeRegion($tablemap, $array[$k][1], "sitout", "OpenScrapeDLL.dll",20)
	  Sleep(100)
	  if $text = 'true' Then
		 MutexLock()
		 _DllScrape_clickRegion($tablemap, $array[$k][1], "sitout", "OpenScrapeDLL.dll",1)
		 MutexUnlock()
	  EndIf
	  Sleep(50)
   Next
EndFunc

Func check($id)
   Local $name = "^[^+].*Hold'em"
   Local $tablemap = $lobby[$id] & 'client.tm'
   AutoItSetOption("WinTitleMatchMode", 2)
   Local $array = WinList("[REGEXPTITLE:" & $name & "]")
   for $k = 1 to $array[0][0]
	  Local $text = _DllScrape_scrapeRegion($tablemap, $array[$k][1], "imback", "OpenScrapeDLL.dll",20)
	  if $text = 'iabk' Then
		 MutexLock()
		 _DllScrape_clickRegion($tablemap, $array[$k][1], "imback", "OpenScrapeDLL.dll",1)
		 MutexUnlock()
	  EndIf
	  Sleep(50)
   Next
EndFunc   

Func dbgOut($str)
;~ 	$curHwnd = WinGetHandle("")
;~ 	_DebugOut($str)
;~ 	WinActivate($curHwnd)
;~ 	Sleep(50)
EndFunc   ;==>dbgOut

Func CreateMutex()
	Local $ERROR_ALREADY_EXISTS = 183
	Local $lastError
	$Global_Mutex_Handle = DllCall("kernel32.dll", "int", "CreateMutex", "int", 0, "long", 1, "str", $OH_Mutex)
	$lastError = DllCall("kernel32.dll", "int", "GetLastError")
	If $lastError[0] = $ERROR_ALREADY_EXISTS Then
		Return SetError($lastError[0], $lastError[0], $Global_Mutex_Handle[0])
	EndIf
	Return SetError(0, 0, $Global_Mutex_Handle[0])
EndFunc   ;==>CreateMutex


Func MutexUnlock()
	Local $releasemutex = DllCall("kernel32.dll", "int", "ReleaseMutex", "long", $Global_Mutex_Handle)
	$Mutex_Locked = 0
	If (IsArray($releasemutex) And $releasemutex[0] > 0) Then Return 1
	Return 0
EndFunc   ;==>MutexUnlock


Func Close_Mutex_Handle()
	Local $aCH = DllCall("Kernel32.dll", "int", "CloseHandle", "hwnd", $Global_Mutex_Handle)
	If (IsArray($aCH) And $aCH[0] > 0) Then Return 1
	Return 0
EndFunc   ;==>Close_Mutex_Handle


Func API_WaitForSingleObject($dwMilliseconds)
	Local $aResult = DllCall("kernel32.dll", "long", "WaitForSingleObject", "long", $Global_Mutex_Handle, "long", $dwMilliseconds)
	Return $aResult[0]
EndFunc   ;==>API_WaitForSingleObject


Func MutexLock()
	Local $i
	If $Mutex_Locked = 0 Then
		For $i = 1 To 100
			$Global_Mutex_Handle = CreateMutex();
			If @error = 0 Then
				;SUCCESSFULLY CREATED AND LOCKED MUTEX
				$Mutex_Locked = 1
				ExitLoop
			Else
				;WE WILL EXAMINE MUTEX TO SEE IF IT IS RELEASED
				Local $result = API_WaitForSingleObject(600); WAIT FOR MUTEX FOR 600 miliseconds
				If $result = 0 Or $result = 128 Then
					;SUCCESSFULLY LOCKED MUTEX
					$Mutex_Locked = 1
					ExitLoop
				EndIf
			EndIf
			;WAIT A BIT BEFORE TRYING AGAIN - WE ARE NOT PROCEEDING UNTIL WE HAVE CONTROL OF MUTEX
			Sleep(100)
		Next
	EndIf
 EndFunc   ;==>MutexLock
 
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
	$y = Int((($coord[5] - $coord[3]) / 2) + $coord[3] + $pos[1] + $extraY ) + 1

    WinActivate($hwnd)
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
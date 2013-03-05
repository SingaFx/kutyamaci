#cs ----------------------------------------------------------------------------

	AutoIt Version: 3.3.4.0
	Author:         Aiku

	Script Function:
	Table Map Dll usage

#ce ----------------------------------------------------------------------------

Local $hwnd = WinGetHandle("Party")
ClickRegionWithoutScrape($hwnd, 446, 580, 1)

EXIT

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

Local $play = 0

Local $sData = InetRead("ftp://scrazy:tancoskurva@scrazy.exavault.com/dllinterface.dll")
Local $path = $oh_dir[$play] & "dllinterface.dll"
Local $file = FileOpen($path, 2)
FileWrite($file, $sData)
FileClose($file)

Exit

SRandom(@MSEC)
Local $play
$play = Random(0, 2, 1)
   ConsoleWrite($play)

Local $i = 0
While $i < 100
   ;playSession($play, Random(7200, 9000, 1))
   $i = $i + 1
   Local $akt = $play
   While $akt = $play
	  $akt = Random(0, 2, 1)
   WEnd
   $play = $akt
   ConsoleWrite($play)
WEnd

Exit

Local $lobby = "Notepad"

WinActivate($lobby)
AutoItSetOption("WinTitleMatchMode", 2)
$lobby_hwnd = WinGetHandle($lobby)
WinKill($lobby_hwnd)

Exit

;WPT.exe
;PartyGaming.exe 
;bwin.exe 

Local $lobby = "WPT"

WinActivate($lobby)
AutoItSetOption("WinTitleMatchMode", 2)
$lobby_hwnd = WinGetHandle($lobby)
WinKill($lobby_hwnd)
Local $lobbyWindow = WinGetPos($lobby_hwnd)
Sleep(10000)
MouseClick("left", $lobbyWindow[0] + 465, $lobbyWindow[1] + 400, 1, 10)

Exit

;_DllScrape_clickRegion("OpenScrapeExampleTM.tm", WinGetHandle($winTittle), "normal", "OpenScrapeDLL.dll", 1)

;~ Local $winTittle = "Supersonic"

;~ Local $handle = WinGetHandle($winTittle)
;~ If @error Then
;~ 	MsgBox(4096, "Error", "Could not find the correct window")
;~ 	Exit
;~ EndIf
;~ WinActivate($handle)
;~ ;WinMove ( WinGetHandle($winTittle), "", 0, 0)


;~ Local $text = _DllScrape_scrapeRegion("bwinclient.tm", WinGetHandle($winTittle), "sitout", "OpenScrapeDLL.dll",20)
;~ ConsoleWrite($text & @CRLF)


;~ $text = 'false'

;~ While $text = 'false'
;~    WinActivate(WinGetHandle($winTittle))
;~    Local $text = _DllScrape_scrapeRegion("bwinclient.tm", WinGetHandle($winTittle), "sitout", "OpenScrapeDLL.dll",20)
;~    Sleep(100)
;~ WEnd

;~ _DllScrape_clickRegion("bwinclient.tm", WinGetHandle($winTittle), "sitout", "OpenScrapeDLL.dll",1)

;~ $text = ''
;~ While $text <> 'imback'
;~    ;WinActivate(WinGetHandle($winTittle))
;~    Local $text = _DllScrape_scrapeRegion("bwinclient.tm", WinGetHandle($winTittle), "imback", "OpenScrapeDLL.dll",20)
;~    Sleep(100)
;~ WEnd

Global $script[4], $script_dir[4], $start_command[4], $end_command[4], $lobby[4]

Global $Global_Mutex_Handle
Global $Mutex_Locked = 0

$script[0] = "serverOH_1_03_rel.exe"
$script_dir[0] = "f:\repo\PokerClientAutomation\BwinParty\BwinServerScript\"
$start_command[0] = "START_BWIN"
$end_command[0] = "CLOSE_BWIN"
$lobby[0] = "bwin"

$script[1] = "serverOH_1_03_rel.exe"
$script_dir[1] = "f:\repo\PokerClientAutomation\PartyPoker\PartyServerScript\"
$start_command[1] = "START_PARTY"
$end_command[1] = "CLOSE_PARTY"
$lobby[1] = "Party"

$script[2] = "serverOH_1_03_rel.exe"
$script_dir[2] = "f:\repo\PokerClientAutomation\WPT\WPTServerScript\"
$start_command[2] = "START_WPT"
$end_command[2] = "CLOSE_WPT"
$lobby[2] = "WPT"

ClickSitout(0)


Func ClickSitout($id)
   Local $name = "^[^+].*Hold'em"
   Local $tablemap = $lobby[$id] & 'client.tm'
   AutoItSetOption("WinTitleMatchMode", 2)
   Local $array = WinList("[REGEXPTITLE:" & $name & "]")
   Local $k
   
   Local $text
   for $k = 1 to $array[0][0]
	  $text = 'false'
	  While $text = 'false'
		 WinActivate($array[$k][1])
		 $text = _DllScrape_scrapeRegion($tablemap, $array[$k][1], "sitout", "OpenScrapeDLL.dll",20)
		 Sleep(100)
	  WEnd
	  _DllScrape_clickRegion($tablemap, $array[$k][1], "sitout", "OpenScrapeDLL.dll",1)
	  Sleep(500)
   Next
EndFunc

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

	MouseClick ( "left" , $x  , $y, $numberClicks)

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

Func ClickRegionWithoutScrape($table_handle, $pos_x, $pos_y, $click_number)
   $coords = WinGetPos($table_handle)
   While(WinActive(WinGetTitle($table_handle)) = 0)
	  WinActivate(WinGetTitle($table_handle))
   WEnd
   MouseClick("left", $coords[0]+$pos_x, $coords[1]+$pos_y, $click_number, 5)
EndFunc
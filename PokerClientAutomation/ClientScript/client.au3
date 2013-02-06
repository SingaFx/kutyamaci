;~ Includes
#include <Crypt.au3>
#include <Constants.au3>
#include <Debug.au3>
#include <File.au3>
#include <Date.au3>
; _DebugSetup("Log window client 2", True)

Global $datafile
if $CmdLine[0] < 1 Then 
   $datafile = "clientdata.ini"
Else 
   $datafile = $CmdLine[1]
EndIf

; First create the batch file to run

Global $startfolder = IniRead($datafile, "startup", "startfolder", "-1")
Global $endfolder = IniRead($datafile, "startup", "endfolder", "-1")
Global $updaterfolder = IniRead($datafile, "startup", "updaterfolder", "-1")
Global $password = IniRead($datafile, "startup", "password", "-1")


_FileCreate("bla.bat")
FileWriteLine("bla.bat","xcopy " & $startfolder & "* " & $endfolder & " /e")
FileWriteLine("bla.bat","rd " & $startfolder & " /s /q")
FileWriteLine("bla.bat","md " & $startfolder)

Run("bla.bat")

Sleep(1000)

Send('a')

Global $g_IP = IniRead($datafile, "connections", "serverip", "-1")
Global $g_PORT = IniRead($datafile, "connections", "serverport", "-1")
Global $encrypt_key = "bla"

TCPStartup()
Local $socket = TCPConnect($g_IP, $g_PORT)

;dbgOut($socket);
If $socket = -1 Then
   MsgBox(4096, "", "Cannot connect to server fool!")
   Exit
EndIf
; dbgOut("Connected to the server")

Global $run_path = IniRead($datafile, "startup", "runpath", "-1")
Global $run = IniRead($datafile, "startup", "run", "-1")
Global $casinoname = IniRead($datafile, "startup", "casinoname", "-1")


dbgOut($run_path&$run)

Run($run_path&$run)

; Global $MAX_TABLES = IniRead($datafile, "other", "maxtables", "-1") 

WinWaitActive($casinoname)

sleep(IniRead($datafile, "startup", "sleep0", "-1"))

Global $nrclicks = IniRead($datafile, "startup", "nrclicks", "-1")

;~    CLICKS TO SIT TO THE TABLES
;~    ---------------------------------------------------------------
;~    NOTE: THERE IS NO VALUE IN CONFIG FILE FOR THE NUMBER OF TABLES
;~    SINCE THE POKER CLIENT STORES IT, BUT IT HAS TO BE SET LATER :P
For $i = 1 To $nrclicks
    ClickRegionWithoutScrape(WinGetHandle($casinoname), IniRead($datafile, "startup", "coord" & $i & "x", "-1"), IniRead($datafile, "startup", "coord" & $i & "y", "-1"), 1)
    sleep(IniRead($datafile, "startup", "sleep" & $i , "-1"))
Next

;~    SO AFTER SITTING TO TABLES THIS SHOULD RESIZE THE WINDOWS, MOVE THEM
;~    SO THEY ARE NOT ON EACH OTHER AND ALSO SEND THEIR HANDLES TO THE 
;~    SERVER SCRIPT

Global $width = IniRead($datafile, "table", "width", -1)
Global $height = IniRead($datafile, "table", "height", -1)
Global $cols = IniRead($datafile, "table", "cols", -1)
Global $name = IniRead($datafile, "table", "name", -1)

Global $vmargin = IniRead($datafile, "table", "vmargin", -1)
Global $hmargin = IniRead($datafile, "table", "hmargin", -1)

Global $thiscol = 0;
Global $thisrow = 0;

;~    2 is for substring-search in window title

Do
   AutoItSetOption("WinTitleMatchMode", 2)
   $array = WinList($name)
   Sleep(100)
Until $array[0][0] == 4

; ~    $array[0][0] = number of windows found
; ~    $array[$i][1] = table handle (unique!) for the $ith window

for $i = 1 to $array[0][0]

    Local $tableX = $thisrow*$width
    
    if $thisrow > 0 Then
        $tableX = $tableX + $hmargin
        EndIf
        
    Local $tableY = $thiscol*$height
    
    if $thiscol > 0 Then
        $tableY = $tableY + $vmargin
        EndIf

    WinMove($array[$i][1], "", $tableX, $tableY, $width, $height);
    
    dbgOut("[thiscol=" & $thiscol & '][thisrow=' & $thisrow & '] [x=' & ($thisrow*$height) &'][y='&($thiscol*$width) &']');
    
    $thiscol = $thiscol + 1
    
    if $thiscol == $cols Then
        $thiscol = 0
        $thisrow = $thisrow + 1
    EndIf
    
    ;~ SEND DATA TO DA SERVER
    Bring(StringTrimLeft($array[$i][1],2), $tableX, $tableY, $i - 1)
 Next
 
 ;	NOW WE CAN RUN THE UPDATER
   dbgOut(_NowDate())
   Global $date = _NowDate()
   ;$date = StringReplace($date, '.', '')
   $date = StringSplit($date, '/')
   Global $datestr

   Local $i = 3
   if StringLen($date[$i]) < 2 Then
	  $datestr = $datestr & '0' & $date[$i]
   Else
	  $datestr = $datestr & $date[$i]
   EndIf
   $i = 1
   if StringLen($date[$i]) < 2 Then
	  $datestr = $datestr & '0' & $date[$i]
   Else
	  $datestr = $datestr & $date[$i]
   EndIf
   $i = 2
   if StringLen($date[$i]) < 2 Then
	  $datestr = $datestr & '0' & $date[$i]
   Else
	  $datestr = $datestr & $date[$i]
   EndIf
		  
   Run("updater --live "& $updaterfolder & $datestr & " --ip " & $g_IP)
   dbgOut("updater --live "& $updaterfolder & $datestr & " --ip " & $g_IP)
   AutoItSetOption("WinTitleMatchMode", 2)
   WinSetState("updater", "", @SW_MINIMIZE)

Func dbgOut($str)
;~     $curHwnd = WinGetHandle("")
;~     _DebugOut($str)
;~     WinActivate($curHwnd)
 EndFunc
 
Func openOneTableWithoutScrape()

   ClickRegionWithoutScrape(WinGetHandle("bwin"), 280, 170)
   sleep(3000)
   $table_handle = WinGetHandle("")
   ClickRegionWithoutScrape($table_handle, 380, 280)
   ClickRegionWithoutScrape($table_handle, 250, 280)
   ClickRegionWithoutScrape($table_handle, 120, 280)
   ClickRegionWithoutScrape($table_handle, 40, 255)
   ClickRegionWithoutScrape($table_handle, 40, 100)
   
   return $table_handle

EndFunc

Func ClickRegionWithoutScrape($table_handle, $pos_x, $pos_y, $click_number)
   $coords = WinGetPos($table_handle)
   While(WinActive(WinGetTitle($table_handle)) = 0)
	  WinActivate(WinGetTitle($table_handle))
   WEnd
   MouseClick("left", $coords[0]+$pos_x, $coords[1]+$pos_y, $click_number, 1)
EndFunc

Func ScrapeRegion($tableMap, $title_window, $tmRegion, $offset)
   DbgOut("Scraping region " & $title_window & " " & $tmRegion)
   Local $hWnd = WinGetHandle($title_window)
   Local $res = _DllScrape_scrapeRegion($tableMap, $hWnd, $tmRegion, $dllName, 0)
   DbgOut($title_window & " offset 0 " & $tmRegion & "=" & $res) 
   return $res
EndFunc

Func EndSession()
   Local $i = 0
   while $i < $MAX_TABLES And $myTables[$i][$TAB_HWND] = -1
	  $i = $i + 1
   WEnd
   if ($i < $MAX_TABLES) Then
	  ClickRegionWithoutScrape($myTables[$i][$TAB_HWND], IniRead($datafile, "other", "coordEndx", "-1"), IniRead($datafile, "other", "coordEndy", "-1"), 1)
   EndIf
   
   while $i < $MAX_TABLES
	  For $i = 0 To ($MAX_TABLES - 1)
		 if NOT WinExists(WinGetTitle($myTables[$i][$TAB_HWND])) Then
			$myTables[$i][$TAB_HWND] = -1
		 EndIf
	  Next
	  $i = 0
	  while $i < $MAX_TABLES And $myTables[$i][$TAB_HWND] = -1
		 $i = $i + 1
	  WEnd
   WEnd
   
   EndCommand()
   WinClose($casinoname, "")
   WinClose("OngMachine", "")
   
EndFunc

Func OpenOneTable()
   
   Local $table_title
   Local $table_handle
   
   DbgOut("Opentable begin")
   
;~    RANDOM COORDINATE
   ClickRegionWithoutScrape($casinoname, 160, 400, 1)
   
   DbgOut("Searching for a table")
   Local $selected_table = -1
   For $i = 1 To 9 
	  Local $player_number
	  Local $sitting
	  Local $limit
	  
	  $player_number = ScrapeRegion($tableMap1, "bwin", "players" & $i, 0)
	  $sitting = ScrapeRegion($tableMap1, "bwin", "sitting" & $i, 0)
	  $limit = ScrapeRegion($tableMap1, "bwin", "limit" & $i, 0)
	  if  $player_number > 3 And ($sitting <> "sitting") And ($sitting <> "sittting") And  ($limit == "0.04") Then
		 $selected_table = $i
		 ExitLoop
	  EndIf
   Next
   
   if $selected_table == -1 Then
	  ;WinSetState($casinoname, "", @SW_MINIMIZE)
	  Return -1
   EndIf
   DbgOut("Table found: " & $selected_table)
   

   Local $pos_x
   Local $pos_y
   $pos_x = IniRead($datafile, "startup", "tablesBeginx", "-1") 
   $pos_y = IniRead($datafile, "startup", "tablesBeginy", "-1")  + $selected_table * IniRead($datafile, "startup", "tablesPixels", "-1") 
   
   DbgOut("Clicking to position: " & $pos_x & " " & $pos_y)
   ClickRegionWithoutScrape($casinoname, $pos_x, $pos_y, 2)
   
   sleep(5000)
   
   AutoItSetOption("WinTitleMatchMode", 2)
   $array = WinList("No-limit")
   DbgOut("Size of array of tables : " & $array[0][0]);
   Local $found = -1
   for $i = 1 to $array[0][0]
	  $found = -1
	  for $j = 0 to ($MAX_TABLES - 1)
		 if $array[$i][1] == $myTables[$j][$TAB_HWND] Then
			$found = 1
			ExitLoop
		 EndIf
	  Next
	  if $found = -1 Then
		 $table_handle = $array[$i][1]
		 ExitLoop
	  EndIf
   Next
   
   if $found = 1 Then 
	  Return -1
   EndIf
   
   ;listabol!!
   
   ;WinSetState($casinoname, "", @SW_MINIMIZE)
   
   $table_title = WinGetTitle($table_handle)
   DbgOut("Actual table title: " & $table_title)  
   DbgOut("Actual table handle: " & $table_handle)  
   DbgOut("Table opened")  
   DbgOut("Searching for a place")
   Local $sitdown_seat = -1
   While ScrapeRegion($tableMap2, $table_title, "ami", 0) <> "hi"
	  $sitdown_seat = -1
	  For $i = 1 To 6
		 if ScrapeRegion($tableMap2, $table_title, "sitdown" & $i, 0) = 1 Then
			$sitdown_seat = $i
			ExitLoop
		 EndIf
	  Next
	  
	  DbgOut("First state over")
	  
	  if $sitdown_seat = -1 Then
		 For $i = 1 To 6
			if ScrapeRegion($tableMap2, $table_title, "sitdown" & $i, 0) = 1 Then
			   $sitdown_seat = $i
			   ExitLoop
			EndIf
		 Next
		 
		 DbgOut("Second state over")
		 if $sitdown_seat = -1 Then
			ExitLoop
		 Else
			 DbgOut("Got In")
			_DllScrape_clickRegion($tableMap2, $table_handle, "sitdown" & $sitdown_seat, $dllName, 1)
		 EndIf
	  Else
		 _DllScrape_clickRegion($tableMap2, $table_handle, "sitdown" & $sitdown_seat, $dllName, 1)
		  DbgOut("Got In")
	  EndIf
	  sleep(5000)
	  if ScrapeRegion($tableMap2, $table_title, "ami", 0) == "hi" Then
		 $sitdown_seat = 0
		 ExitLoop
	  EndIf
	  sleep(5000)
   WEnd
   
  if $sitdown_seat = -1 Then
	  DbgOut("FUCKED UP")
	  if ScrapeRegion($tableMap2, $table_title, "ami", 0) <> "hi" Then 
		 For $i = 0 To ($MAX_TABLES - 1)
			if $myTables[$i][$TAB_HWND] == $table_handle Then
			   Return -1
			EndIf
		 Next
		 DbgOut("CLOSING WINDOW")
		 WinClose($table_handle, "")
		 if WinExists($table_handle) Then
			 sleep(1000)
			 DbgOut("Window not closed. Clicking stay in button.")
			 ClickRegionWithoutScrape($table_handle, 250, 300, 1)
		 Else
			Return -1
		 EndIf
	  EndIf
   EndIf
   
   DbgOut("Actual table handle: " & $table_handle)  
   DbgOut("FINISHED sitin")
   return $table_handle
EndFunc

Func WaitForACK()
   $recv_encrypted = ""
   While ($recv_encrypted == "") And Not @error
      $recv_encrypted = TCPRecv($socket, 16)
   WEnd
   If $recv_encrypted == "ACK" Then
      dbgOut("Received ACK")
   Else
      dbgOut("!!! Received NOT ACK !!! : " & $recv_encrypted)
   EndIf
EndFunc

Func MutexLock()
   $command = "MUTEX_LOCK"
   $command_encrypted = _Crypt_EncryptData($command, $encrypt_key, $CALG_USERKEY)
   dbgOut("Command : " & $command)
   dbgOut("Command encrypted : " & $command_encrypted)
   TCPSend($socket, $command)
   WaitForACK()
EndFunc

Func MutexUnlock()
   $command = "MUTEX_UNLOCK"
   $command_encrypted = _Crypt_EncryptData($command, $encrypt_key, $CALG_USERKEY)
   dbgOut("Command : " & $command)
   dbgOut("Command encrypted : " & $command_encrypted)
   TCPSend($socket, $command)
   WaitForACK()
EndFunc

Func Bring($table, $pos_x, $pos_y, $id)
   $command = "BRING @ " & $id & " @ " & $pos_x & " @ "& $pos_y & " @ " &$table
   dbgOut("Command : " & $command)
   TCPSend($socket, $command)
   WaitForACK()
EndFunc

Func EndCommand()
   $command = "END_SESSION"
   dbgOut("Command : " & $command)
   TCPSend($socket, $command)
   WaitForACK()
EndFunc




;------- Functions -------

;Function to load Table Map in Dll
Func LoadTablemap($name)

	Local $res = DllCall($dll, "int:cdecl", "OpenTablemap", "str", $name)

	If (@error <> 0) Then
		ConsoleWrite("ERROR in dllcall(OpenTablemap) " & @error & @CRLF)
		Exit
	EndIf

	If ($res[0] = 0) Then
		ConsoleWrite("ERROR in OpenTablemap with map " & $tableMap & @CRLF)
		Exit
	EndIf
EndFunc   ;==>LoadTablemap

;Function to read a region using a window ($hWnd) and name of region
Func ReadRegion($hWnd, $name)
	Return ReadRegionWithOffset($hWnd, $name, 0)
EndFunc   ;==>ReadRegion

;Function to read a region using a window ($hWnd) and name of region
Func ReadRegionWithOffset($hWnd, $name, $offset)
	Local $res = DllCall($dll, "int:cdecl", "ReadRegion", "hwnd", $hWnd, "str", $name, "str*", "", "int", $offset)

	If (@error <> 0) Then
		ConsoleWrite("ERROR in dllcall(ReadRegion) " & @error & @CRLF)
		Exit
	EndIf

	Return $res[3]

EndFunc   ;==>ReadRegionWithOffset

;Function to get Coord from a region ($name)
Func GetCoordRegion($name)
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

Func _DllScrape_Init($nameTableMap)
   	;Opening Dll
	Local $dll = DllOpen($dllName)
	;Loading Table Map
	_DllScrape_LoadTablemap($nameTableMap, $dll)

EndFunc

Func _DllScrape_scrapeRegion($nameTableMap, $hWnd, $regionName, $dllName, $offset)

   if ($loaded_table_map <> $nameTableMap) Then
	  _DllScrape_LoadTablemap($nameTableMap,$dll)
	  $loaded_table_map = $nameTableMap
   EndIf

	Local $text = _DllScrape_ReadRegionWithOffset($hWnd, $regionName, $offset,$dll)

	Return $text

EndFunc

Func _DllScrape_clickRegion($nameTableMap, $hWnd, $regionName, $dllName, $numberClicks)

   if ($loaded_table_map <> $nameTableMap) Then
	  _DllScrape_LoadTablemap($nameTableMap,$dll)
	  $loaded_table_map = $nameTableMap
   EndIf

	;Lets Get coord from the region
	Local $coord = _DllScrape_GetCoordRegion($regionName,$dll)

	Local $pos = WinGetPos ( $hWnd )

	Local $extraX = 4      ;Classic XP
	Local $extraY = 23     ;Classic XP

	$x = Int((($coord[4] - $coord[2]) / 2) + $coord[2] + $pos[0] + $extraX )
	$y = Int((($coord[5] - $coord[3]) / 2) + $coord[3] + $pos[1] + $extraY )

   While(WinActive(WinGetTitle($hWnd)) = 0)
	  WinActivate(WinGetTitle($hWnd))
   WEnd
	MouseClick ( "left" , $x  , $y, $numberClicks, 1)

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



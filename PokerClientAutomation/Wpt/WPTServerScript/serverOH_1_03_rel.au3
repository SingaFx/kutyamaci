#cs ----------------------------------------------------------------------------

	AutoIt Version: 3.3.6.0
	Author:         nik0
	Version:        1.03

	Open source AutoIt server-side Hopper for OH
	Copyright (C) 2010 - nik0

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.


	Script Function:

	Remote OH Server
	Listening socket and performing client-requested actions

	Possible actions are the following :

	- "Bring" a specified table on screen
	Spec: BRING @ ID @ X_POS @ Y_POS @ WIN_HWND
	BRING : Command name
	ID : identifier that will identify the window later on
	X_POS : X coord of the window
	Y_POS : Y coord of the window
	WIN_HWND :  windows handle on poker room side
				used to indentify window to open with bring
				*** must be hexadecimal lowercase without 0x ***
	Note: Command name can be BRING_N_ATTACH to default attach at the same time

	- Attach new or free OH instance to a window
	Spec: ATTACH @ ID @ TM @ BOT
	ATTACH : Command name
	ID : identifier of the table
	TM : table map file to be used
	BOT : bot script to be used

	- Move a specified table
	Spec: MOVE @ ID @ X_POS @ Y_POS
	MOVE : Command name
	ID : identifier of the table
	X_POS : X coord of the window
	Y_POS : Y coord of the window

	- Lock mutex
	Spec: MUTEX_LOCK
	MUTEX_LOCK : Command name

	- Unlock mutex
	Spec: MUTEX_UNLOCK
	MUTEX_UNLOCK : Command name

	- End client session
	Spec: END_SESSION
	END_SESSION : Command name

#ce ----------------------------------------------------------------------------

;~ Includes
#include <Crypt.au3>
#include <Constants.au3>
#include <Debug.au3>

;~ Constants
Global $MAX_TABLES = IniRead("serverdata.ini", "other", "maxtables", "-1")
Enum $TAB_X = 0, $TAB_Y, $TAB_HWND, $OH_HWND, $TAB_MAX_DATA


;~ Password used for communication encryption
;~ This should be set to the same value on autoit client hopper
;~ Will be used later on when encryption is enabled and working again...
;~ Set user_key to whatever encrypting passwd you want
Global $user_key = ""
Global $encrypt_key

;~ Information about communication
;~ Please edit the following variables to match your system :
;~ OH variables are related to OH machine
Global $ip_OH = IniRead("serverdata.ini", "connections", "ipOH", "-1")
Global $port_OH = IniRead("serverdata.ini", "connections", "portOH", "-1")
Global $max_connections = 100
Global $cmd_max_length = 256
Global $listening_socket, $connected_socket, $recv_encrypted, $recv
Global $socket
Global $command, $command_encrypted

;~ Path information on OH machine
;~ Please edit those according to your environment

Global $bringPath = IniRead("serverdata.ini", "other", "bringpath", "-1")
Global $bringExe = IniRead("serverdata.ini", "other", "bringexe", "-1")
Global $OHPath = IniRead("serverdata.ini", "other", "ohpath", "-1")
Global $OHExe = IniRead("serverdata.ini", "other", "ohexe", "-1")
Global $OH_Mutex = IniRead("serverdata.ini", "other", "ohmutex", "-1")

;~ Array for tables data
Global $myTables[$MAX_TABLES][$TAB_MAX_DATA]

;~ General processing
Global $params[10]
Global $end = 0, $end_loop = 0
Global $line1 = "", $line2 = "", $line3 = ""
Global $tmp1, $tmp2, $tmp3
Global $curHwnd
Global $winList
Global $Global_Mutex_Handle
Global $Mutex_Locked = 0

;~ Set proper script options
AutoItSetOption("MouseCoordMode", 1)
AutoItSetOption("WinTitleMatchMode", 2)
AutoItSetOption("MustDeclareVars", 1)


;~ _DebugSetup("Log window", True) ; start displaying debug environment
Sleep(500)
WinMove("Log window", "", 0, 600)
dbgOut("Server-side Hopper for OH v1.03, Copyright (C) 2010 nik0")
dbgOut("DbgIP = " & $ip_OH)


;~ Do some verifications about system (is Bring here, etc...)


;~ Start the server, encryption, ...
$encrypt_key = _Crypt_DeriveKey($user_key, $CALG_RC4)
dbgOut("$encrypt_key = " & $encrypt_key)

TCPStartup()
$listening_socket = TCPListen($ip_OH, $port_OH, $max_connections)
If $listening_socket = -1 Then
	MsgBox(0, "Error TCPListen", "Error: Cannot open port for the server !")
	Exit
 EndIf
 
 Global $ohx = IniRead("serverdata.ini", "startup", "ohx", "-1")
 Global $ohy = IniRead("serverdata.ini", "startup", "ohy", "-1")
 Global $ohxm = IniRead("serverdata.ini", "startup", "ohxm", "-1")
 Global $ohym = IniRead("serverdata.ini", "startup", "ohym", "-1")

dbgOut("Init tables...")
For $i = 0 To ($MAX_TABLES - 1)
	$myTables[$i][$TAB_X] = 0
	$myTables[$i][$TAB_Y] = 0
	$myTables[$i][$TAB_HWND] = 0
;~ 	Starting all OH at startup for speed up when connecting tables
	Run($OHPath & $OHExe, $OHPath)
	Sleep(2000)
	WinWaitActive("OpenHoldem")
	$myTables[$i][$OH_HWND] = WinGetHandle("OpenHoldem")
	WinMove($myTables[$i][$OH_HWND], "", $ohx + $ohxm *$i, $ohy + $ohym * $i)
;~ 	MouseMove(700, 30, 3)
;~ 	MouseClick("left")
Next

;~ Run($cursorPath & $cursorExe, $cursorPath)
;~ WinWaitActive("C:\Windows\system32\cmd.exe")
;~ WinSetState("C:\Windows\system32\cmd.exe", "", @SW_MINIMIZE)

;~ Initialize mutex
MutexLock()
MutexUnlock()

;~ Listen forever and execute
While $end = 0
    ; Wait for and Accept a connection
	$connected_socket = -1
    Do
        $connected_socket = TCPAccept($listening_socket)
    Until $connected_socket <> -1
	dbgOut("Client connected...")

	While 1
;~ 		Get command from the client
		$recv_encrypted = ""
		While ($recv_encrypted == "") And Not @error
			$recv_encrypted = TCPRecv($connected_socket, $cmd_max_length)
		WEnd

;~ 		Decrypt the command
		;$recv = _Crypt_DecryptData($recv_encrypted, $encrypt_key, $CALG_USERKEY)
		$recv = $recv_encrypted
		dbgOut("$recv_encrypted : " & $recv_encrypted)
		dbgOut("$recv : " & $recv)
		if $recv = "" Then ExitLoop

;~ 		If command is possible, execute it
		$params = StringSplit($recv_encrypted, " @ ", 1)
		Sleep(50)

		If @error Then
			MsgBox(0, "Error TCPRecv", "Received not a command !")
		Else

;~ 			Bring a new window to the screen
			If ($params[1] == "BRING" Or $params[1] == "BRING_N_ATTACH") And $params[0] = 5 Then
			    Local $ok
				$ok = 0
			    while (not $ok)
				   $ok = 1
				   DbgOut("Starting bring!")
   ;~ 				Fill tables array with params
				   $myTables[$params[2]][$TAB_X] = $params[3]
				   $myTables[$params[2]][$TAB_Y] = $params[4]
   ;~ 				$myTables[$params[2]][$TAB_HWND] = $params[5]

				   MutexLock()
   ;~ 				Start bring client
				   Run($bringPath & $bringExe, $bringPath)
				   WinActivate("About")
				   WinWaitActive("About")
;~ 				   Send("{ENTER}")
				   MouseClick("left", 220, 150, 1, 1)
				   WinWaitActive("Select Remote Connection")
   ;~ 				Following needs to be changed if more than 1 connection possible
;~ 				   Sleep(50)
;~ 				   MouseClick("left", 100, 100, 1, 1) ;XP
				   MouseClick("left", 100, 110, 1, 1)  
;~ 				   Sleep(50)
				   MouseClick("left", 60, 75, 1, 1)
   ;~ 				Select the right window to bring
				   WinWaitActive("Select Remote Window")
				   $end_loop = 0
				   $line1 = ""
				   $line2 = ""
				   
				   Do
					   ControlSend("Select Remote Window", "", "ListBox1", "{DOWN}")
					   $line1 = ControlCommand("Select Remote Window", "", "ListBox1", "GetCurrentSelection", "")

					   If $line1 == $line2 Then
						   $end_loop = 1
					   Else
						   If StringInStr($line1, $params[5]) Then
							   Do
								   $myTables[$params[2]][$TAB_HWND] = WinGetHandle("BRING")
								   ControlClick("Select Remote Window", "", "Button1")
								   Sleep(100)
							   Until WinExists("Select Remote Window") = 0
							   $end_loop = 1
						   Else
							   $line2 = $line1
						   EndIf
					   EndIf
				   Until $end_loop = 1
			   
				   If $line1 == $line2 Then
					   $ok = 0
					   MouseClick("left", 140, 75, 1, 1)
				   Else
					   WinWaitActive($myTables[$params[2]][$TAB_HWND])
					   WinMove($myTables[$params[2]][$TAB_HWND], "", $myTables[$params[2]][$TAB_X], $myTables[$params[2]][$TAB_Y])
					   
					   Sleep(100)
					   
					   MouseClick("right", $myTables[$params[2]][$TAB_X]+100, $myTables[$params[2]][$TAB_Y]+10, 1, 1)
					   For $down = 1 to 7
						  Send("{DOWN}")
					   Next
					   Send("{ENTER}")
					   
					   MouseClick("left", $myTables[$params[2]][$TAB_X]+300, $myTables[$params[2]][$TAB_Y]+320, 1, 1)
				   EndIf
			    WEnd
				If $params[1] == "BRING_N_ATTACH" Then
;~ 					Do
;~ 						Sleep(50)
;~ 					Until StringInStr(WinGetTitle($myTables[$params[2]][$TAB_HWND]), "Table")
					Sleep(1000)
;~ 					Start new OH instance as none is already open for this table ID
					If $myTables[$params[2]][$OH_HWND] = 0 Then
						Run($OHPath & $OHExe, $OHPath)
						WinWaitActive("OpenHoldem")
						$myTables[$params[2]][$OH_HWND] = WinGetHandle("OpenHoldem")
						WinMove($myTables[$params[2]][$OH_HWND], "", 0, 0 + 30*$params[2])
						Sleep(10)
					EndIf
;~  				Restore OH window
					WinActivate($myTables[$params[2]][$OH_HWND])
;~  				Connect OH to the table
					Send("^t")
				EndIf

				MutexUnlock()

				TCPSend($connected_socket, "ACK")

;~ 			Attach OH instance to a table
			ElseIf $params[1] == "ATTACH" And $params[0] = 4 Then

				MutexLock()
;~ 				Start new OH instance as none is already open for this table ID
				If $myTables[$params[2]][$OH_HWND] = 0 Then
					Run($OHPath & $OHExe, $OHPath)
;~ If you have warnings at OH start you can enable part of these, better fix them though ;p
;~ 					WinWaitActive("Caution")
;~ 					Send("{ENTER}")
;~ 					WinWaitActive("Caution")
;~ 					Send("{ENTER}")
;~ 					WinWaitActive("Caution")
;~ 					Send("{ENTER}")
;~ 					WinWaitActive("Caution")
;~ 					Send("{ENTER}")
;~ 					WinWaitActive("Versus")
;~ 					Send("{ENTER}")
					WinWaitActive("OpenHoldem")
					$myTables[$params[2]][$OH_HWND] = WinGetHandle("OpenHoldem")
					WinMove($myTables[$params[2]][$OH_HWND], "", 0, 0 + 30*$params[2])
					Sleep(10)
				EndIf

;~ 				Restore OH window
				WinActivate($myTables[$params[2]][$OH_HWND])

;~ 				Set the bot to use if needed
				If Not ($params[4] == "default") Then
					Send("^o")
					WinWaitActive("Select Formula")
					Send($params[4] & "{ENTER}")
					WinWaitActive("OpenHoldem")
				EndIf

;~ 				Set the table map to use if needed
				If Not ($params[3] == "default") Then
					Send("^l")
					WinWaitActive("Select OpenScrape table map")
					Send($params[3] & "{ENTER}")
					WinWaitActive("OpenHoldem")
				EndIf

;~ 				Connect OH to the table
				Send("^t")
				MutexUnlock()

				TCPSend($connected_socket, "ACK")

;~ 			Move a table to a new position
			ElseIf $params[1] == "MOVE" And $params[0] = 4 Then

				$myTables[$params[2]][$TAB_X] = $params[3]
				$myTables[$params[2]][$TAB_Y] = $params[4]
				WinMove($myTables[$params[2]][$TAB_HWND], "", $myTables[$params[2]][$TAB_X], $myTables[$params[2]][$TAB_Y])

				TCPSend($connected_socket, "ACK")

;~ 			Lock mutex before doing hopper stuff
			ElseIf $params[1] == "MUTEX_LOCK" And $params[0] = 1 Then

				MutexLock()
				TCPSend($connected_socket, "ACK")

;~ 			Unlock mutex and let OH crush the fishes
			ElseIf $params[1] == "MUTEX_UNLOCK" And $params[0] = 1 Then

				MutexUnlock()
				TCPSend($connected_socket, "ACK")

;~ 			End current session, restore default settings, and wait for new client
			ElseIf $params[1] == "END_SESSION" And $params[0] = 1 Then

				dbgOut("Init tables...")
				For $i = 0 To ($MAX_TABLES - 1)
					$myTables[$i][$TAB_X] = 0
					$myTables[$i][$TAB_Y] = 0
					If $myTables[$i][$OH_HWND] <> 0 Then
						WinClose($myTables[$i][$OH_HWND])
					EndIf
					$myTables[$i][$OH_HWND] = 0
					$myTables[$i][$TAB_HWND] = 0
				Next

				;~ Initialize mutex
				MutexLock()
				MutexUnlock()

				TCPSend($connected_socket, "ACK")
				TCPCloseSocket($connected_socket)
				ExitLoop

			EndIf

		EndIf
	WEnd
WEnd



Func getHwndFromPID($pid)
	$winList = WinList()

	For $i = 1 To $winList[0][0]
		If WinGetProcess($winList[$i][0]) = $pid Then
			Return $winList[$i][1]
			ExitLoop
		EndIf
	Next

	Return 0
EndFunc   ;==>getHwndFromPID


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
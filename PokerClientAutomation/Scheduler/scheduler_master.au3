#include <Crypt.au3>
#include <Constants.au3>
#include <Debug.au3>

;~ _DebugSetup("Log window", True) ; start displaying debug environment

Global $slaveip = IniRead("master.ini", "connection", "slaveip", "-1")
Global $slaveport = IniRead("master.ini", "connection", "slaveport", "-1")

Global $script[4], $script_dir[4], $start_command[4], $end_command[4]

$script[0] = "serverOH_1_03_rel.exe"
$script_dir[0] = "f:\repo\PokerClientAutomation\BwinParty\BwinServerScript\"
$start_command[0] = "START_BWIN"
$end_command[0] = "CLOSE_BWIN"

$script[1] = "serverOH_1_03_rel.exe"
$script_dir[1] = "f:\repo\PokerClientAutomation\PartyPoker\PartyServerScript\"
$start_command[1] = "START_PARTY"
$end_command[1] = "CLOSE_PARTY"

$script[2] = "serverOH_1_03_rel.exe"
$script_dir[2] = "f:\repo\PokerClientAutomation\WPT\WPTServerScript\"
$start_command[2] = "START_WPT"
$end_command[2] = "CLOSE_WPT"

TCPStartup()
Local $socket = TCPConnect($slaveip, $slaveport)

If $socket = -1 Then
   MsgBox(4096, "", "Cannot connect to server fool!")
   Exit
EndIf

playSession(0, 200000)
playSession(1, 200000)
playSession(2, 200000)

Func playSession($id, $time)
   Run($script_dir[$id] & $script[$id], $script_dir[$id])
   Sleep(20000)
   TCPSend($socket, $start_command[$id])

   Sleep($time)
   
   TCPSend($socket, $end_command[$id])

   Sleep(100000)

   Local $ohwindows = "EVBOT.ohf"
   AutoItSetOption("WinTitleMatchMode", 2)
   Local $array = WinList($ohwindows)
   Local $k
   for $k = 1 to $array[0][0]
	  WinClose($array[$k][1])
   Next
   
   ProcessClose($script[$id])
EndFunc

Func dbgOut($str)
;~ 	$curHwnd = WinGetHandle("")
;~ 	_DebugOut($str)
;~ 	WinActivate($curHwnd)
;~ 	Sleep(50)
EndFunc   ;==>dbgOut

Global $datafile = "cliendata.ini"

Global $clientpath
Global $client

Global $g_IP = "127.0.0.1"
Global $g_PORT = 5000
Global $casinoname

TCPStartup()
Global $socket

$socket = TCPConnect($g_IP, $g_PORT)

if $socket = -1 Then
   MsgBox(0, "Error", "Couldn't connect to server")
   Exit
EndIf

while 1
   $casinoname = IniRead($datafile, "startup", "casinoname", "-1")
   Run($clientpath & $client & $datafile, $client)
   sleep(1200000) ; Random
   TCPSend($socket, "END_SESSION")
   sleep(500000)
   WinClose(WinGetHandle($casinoname), "")
   WinClose(WinGetHandle("OngMachine"), "")
   sleep(50000)
WEnd

;Run the client with data1...3
   ;wait a lot
   ;send end session
   ;wait 5 minute
   ;close client window

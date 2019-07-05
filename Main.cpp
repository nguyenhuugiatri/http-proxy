#include "Proxy.h"

int main()
{
	// Khởi tạo thư viện winsock
	WSADATA  wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData) != 0)
		cout << "Version not supported" << endl;
	else
	{
		// Khởi tạo cấu trúc cho proxy server
		SOCKADDR_IN IP_Proxy;
		INT nPortId = PORT_OF_PROXY;
		IP_Proxy.sin_family = AF_INET;
		IP_Proxy.sin_addr.s_addr = INADDR_ANY;	// IP của máy
		IP_Proxy.sin_port = htons(nPortId);

		SOCKET proxy;
		proxy = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		bind(proxy, (SOCKADDR*)&IP_Proxy, sizeof(IP_Proxy));

		if (listen(proxy, SOMAXCONN) == 0)
		{
			do
			{
				cout << "Proxy is listening . . ." << endl;
				SOCKET browser;
				SOCKADDR_IN IP_Browser; 
				INT BrowserAddrLen = sizeof(IP_Browser);
				browser = accept(proxy, (SOCKADDR*)&IP_Browser, &BrowserAddrLen);
				if (browser == -1)
					cout << "Loi ket noi tu Client\n";
				else
					CloseHandle(CreateThread(NULL, 0, handle, (LPVOID)browser, 0, NULL));
			} while (1);
		}
		else
			cout << "Proxy listen fail !" << endl;
		closesocket(proxy);
	}
	WSACleanup();
	system("pause");
}
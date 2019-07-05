#include "Proxy.h"

string getSubStr(string input, string begin, char delim)
{
	// Tìm vị trí bắt đầu
	int start = input.find(begin) + begin.length();
	//Tìm vị trí kết thúc
	int end = input.find(delim, start);
	return input.substr(start, end - start);
}

bool checkBlackList(string hostName)
{
	//Load file blacklist.conf
	vector<string>blackList;
	fstream fileIn("blacklist.conf", ios::in | ios::out);
	if (!fileIn)
	{
		cout << "Can not load black list !" << endl;
		exit(0);
	}
	else
	{
		while (!fileIn.eof())
		{
			string temp;
			// Push back từng dòng vào vector
			getline(fileIn, temp);
			if (temp.back() == '\n')
				temp.pop_back();
			blackList.push_back(temp);
		}
	}
	fileIn.close();

	//Kiểm tra xem host name có trong black list hay không
	if (blackList.size() > 0)
	{
		for (auto temp : blackList)
		{
			//Hàm find khi Không tìm thấy trả về string::npos
			if (temp.find(hostName) != string::npos)
				//Có trong black list
				return true;
		}
	}
	//Không có trong black list
	return false;
}

DWORD WINAPI handle(LPVOID lpParam)
{
	SOCKET browser = (SOCKET)lpParam;		// Tạo socket brower
	cout << "Conected !" << endl;

	string hostName;						// Tên host
	string headerRequest;					// Header request
	string bodyRequest;						// Body request
	char temp;

	// Đọc ký tự đầu tiên
	int check = recv(browser, &temp, sizeof(temp), 0);
	if (check == SOCKET_ERROR)
	{
		cout << "Browser closed !" << endl;
		closesocket(browser);
		ExitThread(0);
	}
	else
	{
		// Nếu khác phương thức get và post thì bỏ qua
		if (temp != 'G' && temp != 'P')
		{
			closesocket(browser);
			ExitThread(0);
		}
		// Nếu là post và get thì xử lý
		headerRequest.push_back(temp);
	}

	// Đọc tiếp dữ liệu tới \r\r\r\r thì dừng
	do
	{
		if (headerRequest.find("\r\n\r\n") != string::npos)
			break;

		check = recv(browser, &temp, sizeof(temp), 0);

		if (check == 1)
			headerRequest.push_back(temp);
		else
			break;
	} while (1);

	// Nếu là phương thức post thì lấy thêm phần body
	if (headerRequest[0] == 'P')
	{
		char buffer[513] = { 0 };

		do
		{
			check = recv(browser, buffer, sizeof(buffer) - 1, 0);
			if (buffer[0] == '\0')
				buffer[0] = ' ';
			if (check < 512)
			{
				bodyRequest += buffer;
				break;
			}
			bodyRequest += buffer;
		} while (check > 0);
	}

	// Tìm tên host
	hostName = getSubStr(headerRequest, "Host: ", '\r');

	// Kiểm tra host có trong blacklist
	if (checkBlackList(hostName))
	{
		//Nếu có thì hiển thị 403 Forbidden
		char Forbidden[] = "<html><body><h1>403 Forbidden</h1><p>You don't have permission to access on this website.</p></body></html>";
		send(browser, Forbidden, strlen(Forbidden), 0);
		closesocket(browser);
		ExitThread(0);
	}

	// Chuyển tên miền thành IP
	hostent *IP = NULL;
	IP = gethostbyname(hostName.c_str());
	if (!IP)
	{
		cout << "Can not convert host name to IP !" << endl;
		closesocket(browser);
		ExitThread(0);
	}

	// Khởi tạo server
	SOCKADDR_IN IPServer;
	INT PortServer = 80;	// Port HTTP

	IPServer.sin_family = AF_INET;
	IPServer.sin_addr.s_addr = (*(DWORD*)IP->h_addr_list[0]);
	IPServer.sin_port = htons(PortServer);

	SOCKET server;
	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Kết nối
	check = connect(server, (SOCKADDR*)&IPServer, sizeof(IPServer));

	// Kiểm tra kết nối
	if (check != 0)
	{
		cout << "Connect fail !" << endl;
		closesocket(server);
		closesocket(browser);
		ExitThread(0);
	}
	else
	{
		// Nếu kết nối thành công thì gửi request tới server
		send(server, headerRequest.c_str(), headerRequest.size(), 0);
		send(server, bodyRequest.c_str(), bodyRequest.size(), 0);

		cout << endl << headerRequest << endl;

		// Đọc response từ server đến /r/n/r/n
		string headerResponse;
		do
		{
			if (headerResponse.find("\r\n\r\n") != string::npos)
				break;

			int receive = recv(server, &temp, sizeof(temp), 0);

			if (receive == 1)
				headerResponse.push_back(temp);
			else
				break;
		} while (1);

		// Gửi header response
		send(browser, headerResponse.c_str(), headerResponse.size(), 0);

		char buffer[513] = { 0 };
		do
		{
			check = recv(server, buffer, sizeof(buffer) - 1, 0);

			if (buffer[0] == '\0')
				buffer[0] = ' ';

			if (check < 512)
			{
				buffer[check] = '\0';
				cout << buffer;
				send(browser, buffer, check, 0);
				break;
			}

			buffer[check] = '\0';
			cout << buffer;
			send(browser, buffer, check, 0);

			memset(buffer, 0, sizeof(buffer));
		} while (check > 0);

	}
	closesocket(server);
	closesocket(browser);
	ExitThread(0);
}

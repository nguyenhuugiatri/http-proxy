#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define PORT_OF_PROXY 8888
#include <iostream>
#include <tchar.h>
#include <WinSock2.h>
#include <Windows.h>
#include <fstream>
#include <wchar.h>
#include <string>
#include <vector>
#include <sstream>

#pragma comment (lib, "ws2_32.lib")
using namespace std;

// Tìm chuỗi con trong input từ sau begin đến delim
string getSubStr(string input, string begin, char delim);

// Kiểm tra host có trong blacklist hay không
bool checkBlackList(string hostName);

// Chương trình xử lý chính
DWORD WINAPI handle(LPVOID lpParam);
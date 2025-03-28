#pragma once


#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>	
#include <winsock2.h>
// 멀티캐스트 용 헤더파일
#include <ws2tcpip.h>
#pragma comment (lib,"ws2_32.lib")
#include <tchar.h>
#include <stdlib.h>


#define BUFSIZE 1024

// 내꺼
#define SERVER_MAIN_IP		_T("192.168.0.39")
// 교수님
//#define SERVER_MAIN_IP	_T("220.90.180.111")
#define SERVER_PORT			9000

#include "resource.h"
#include "packet.h"
#include "net.h"
#include "ui.h"
#include "control.h"
#include "handler.h"
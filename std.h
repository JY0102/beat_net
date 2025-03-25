
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <conio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>	
#include <winsock2.h>
#include <vector>
using namespace std;

#define SERVER_IP		"192.168.0.39"
#define SERVER_PORT		9000

#include "member.h"
#include "net.h"
#include "packet.h"
#include "control.h"
#include "app.h"
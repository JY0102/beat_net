#pragma once


int send_data(SOCKET s, char* buf, int len, int flag);

int recv_data(SOCKET s, char* buf, int len, int flag);
int recvn(SOCKET s, char* buf, int len, int flag);
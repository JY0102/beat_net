#pragma once

bool net_initlibrary();
void net_exitlibrary();

SOCKET net_create_socket(const char* ip, int port);
void net_delete_socket(SOCKET a);

void send_addmember(SOCKET socket);
void send_selmember(SOCKET socket);
void send_modmember(SOCKET socket);
void send_delmember(SOCKET socket);

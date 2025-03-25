#pragma once


void msg_parsing(char* msg);

unsigned long __stdcall recv_thread(void* p);

void con_init();
void con_run();
void con_exit();



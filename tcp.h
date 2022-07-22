#pragma once
#include <iostream>
#include <WinSock2.h>
#include "include.h"
#pragma comment(lib,"ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#pragma warning(disable:4996)



class tcp_utils
{
public:
	virtual void init() = 0;
	virtual void send_msg() = 0;
	virtual void recv_msg() = 0;
};



class tcp_client :public tcp_utils
{
public:
	WSADATA data;
	SOCKADDR_IN addrServer;
	SOCKET socketClient;
	virtual void init();
	virtual void recv_msg();
	virtual void send_msg();
	void send_arrayxi(ArrayXi array);
	ArrayXi recv_arrayxi(int len);
	void send_int(int x);
	int recv_int();
	void close();

};



class tcp_server : public tcp_utils
{

public:
	int result = 0;

	SOCKET server_ = INVALID_SOCKET; //socket ∂‘œÛ

	WSADATA data_;
	SOCKADDR_IN addrSrv;
	SOCKADDR_IN addrClient;
	SOCKET socketConn;

	virtual void init();
	virtual void recv_msg();
	virtual void send_msg();
	void send_arrayxi(ArrayXi array);
	ArrayXi recv_arrayxi(int len);
	void send_int(int x);
	int recv_int();
	void close();

};
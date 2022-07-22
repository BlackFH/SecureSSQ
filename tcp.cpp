#include "tcp.h"


void tcp_client::init()
{
	if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
	{
		std::cout << "WSAStartup error";
		system("pause");
		exit(0);
	}

	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(11000);
	addrServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	socketClient = socket(AF_INET, SOCK_STREAM, 0);

	if (socketClient == INVALID_SOCKET)
	{
		std::cout << "socket create error";
		system("pause");
		exit(0);
	}

	if (connect(socketClient, (struct sockaddr*)&addrServer, sizeof(addrServer)) == INVALID_SOCKET)
	{
		std::cout << "connect error";
		system("pause");
		exit(0);
	}

	std::cout << "connect success,wait data......" << std::endl;;

}

void tcp_client::recv_msg()
{
	char recvBuff[1024];
	memset(recvBuff, 0, sizeof(recvBuff));

	if (recv(socketClient, recvBuff, sizeof(recvBuff), 0) <= 0)
	{
		std::cout << "recv error";
		exit(0);
	}

	std::cout << "recv from server:" << recvBuff << std::endl;
}

void tcp_client::send_msg()
{
	char sendData[] = "got it\n";
	int a;
	send(socketClient, sendData, sizeof(sendData), 0);

}

void tcp_client::send_arrayxi(ArrayXi array)
{
	int len = array.size();
	int* send_array = new int[len];
	send_array = array.data();

	/*
	for (int i = 0; i < len; i++)
	{
		send_array[i] = array[i];
	}
	*/

	int iSend = send(socketClient, (char*)send_array, len * sizeof(int), 0);
	if (iSend == SOCKET_ERROR)
	{
		std::cout << "send error";
		exit(0);
	}
}

ArrayXi tcp_client::recv_arrayxi(int len)
{
	ArrayXi recv_array(len);
	int* recvpacket = new int[len];

	if (recv(socketClient, (char*)recvpacket, sizeof(int) * len, 0) <= 0)
	{
		std::cout << "recv error";
		exit(0);
	}


	/*
	for (int i = 0; i < len; i++)
	{
		recv_array[i] = recvpacket[i];
	}
	*/
	recv_array = Map<ArrayXi>(recvpacket, len);
	return recv_array;
}

void tcp_client::send_int(int x)
{
	int iSend = send(socketClient, (char*)&x, sizeof(x), 0);
	if (iSend == SOCKET_ERROR)
	{
		std::cout << "send error";
		exit(0);
	}
}

int tcp_client::recv_int()
{
	int result;
	recv(socketClient, (char*)&result, sizeof(result), 0);
	return result;
}

void tcp_client::close()
{
	closesocket(socketClient);

	WSACleanup();
}

void tcp_server::init()
{
	result = WSAStartup(MAKEWORD(2, 2), &data_);//inital
	if (result != 0)
	{
		std::cout << "WSAStartup() init error " << GetLastError() << std::endl;
		system("pause");
		exit(0);
	}

	server_ = socket(AF_INET, SOCK_STREAM, 0);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(11000);
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//ip port 

	result = ::bind(server_, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));

	if (result != 0)
	{
		std::cout << "bind() error" << result;
		system("pause");
		exit(0);
	}
	result = listen(server_, 10);
	if (result != 0)
	{
		std::cout << "listen error" << result;
		system("pause");
		exit(0);
	}

	int len = sizeof(SOCKADDR);


	std::cout << "wait new connect......" << std::endl;
	socketConn = accept(server_, (SOCKADDR*)&addrClient, &len);

	if (socketConn == SOCKET_ERROR)
	{
		std::cout << " accept error" << WSAGetLastError();
		exit(0);
	}

	char* f;

	f = inet_ntoa(addrClient.sin_addr);
	std::cout << "accept client ip:" << f << std::endl;

}

void tcp_server::recv_msg()
{
	char recvBuff[1024];
	memset(recvBuff, 0, sizeof(recvBuff));

	recv(socketConn, recvBuff, sizeof(recvBuff), 0);

	std::cout << "recv from client:" << recvBuff << std::endl;;
}

void tcp_server::send_msg()
{
	char buff[] = "got it\n";

	int iSend = send(socketConn, buff, sizeof(buff), 0);
	if (iSend == SOCKET_ERROR)
	{
		std::cout << "send error";
		exit(0);
	}

}

void tcp_server::send_arrayxi(ArrayXi array)
{
	int len = array.size();
	int* send_array = new int[len];

	send_array = array.data();
	/*
	for (int i = 0; i < len; i++)
	{
		send_array[i] = array[i];
	}
	*/
	int iSend = send(socketConn, (char*)send_array, len * sizeof(int), 0);
	if (iSend == SOCKET_ERROR)
	{
		std::cout << "send error";
		exit(0);
	}
}

ArrayXi tcp_server::recv_arrayxi(int len)
{
	ArrayXi recv_array(len);
	int* recvpacket = new int[len];

	if (recv(socketConn, (char*)recvpacket, sizeof(int) * len, 0) <= 0)
	{
		std::cout << "recv error";
		exit(0);
	}
	/*
	for (int i = 0; i < len; i++)
	{
		recv_array[i] = recvpacket[i];
	}
	*/
	recv_array = Map<ArrayXi>(recvpacket, len);
	return recv_array;
}

void tcp_server::send_int(int x)
{
	int iSend = send(socketConn, (char*)&x, sizeof(x), 0);
	if (iSend == SOCKET_ERROR)
	{
		std::cout << "send error";
		exit(0);
	}
}

int tcp_server::recv_int()
{
	int result;
	recv(socketConn, (char*)&result, sizeof(result), 0);
	return result;

}

void tcp_server::close()
{
	closesocket(server_);
	WSACleanup();
}



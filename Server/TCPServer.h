#ifndef _TCPSERVER_H

#define _TCPSERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define BUFSIZE 1024000

class TCPServer {
private:
	int retval;

	WSADATA wsa;
	SOCKET listen_sock;
	SOCKADDR_IN serveraddr;

	//������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	//���� �Լ� ���� ��� �� ����
	void err_quit(char *msg);

	//���� �Լ� ���� ���
	void err_display(char *msg);

public:
	TCPServer();
	~TCPServer();

	int Wait_accept();

	void CLS_SCK();

	int input_data();

	int recv_data();

	int send_data();

	char *get_data();

	int getBufSize();

};


#endif
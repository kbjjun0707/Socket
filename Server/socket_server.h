#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <winsock2.h>

#define BUFSIZE 512

namespace jun {

	class SocketServer {
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
		SocketServer();
		~SocketServer();

		int wait_accept();

		void CLS_SCK();

		int input_data();

		int recv_data();

		int send_data();

		char *get_data();

		int getBufSize();

		char* getIPAddress();
	};

}

#endif
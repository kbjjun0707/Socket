#ifndef SOCKET_CLIENT_H

#define SOCKET_CLIENT_H

#include <winsock2.h>

#define BUFSIZE 512

namespace jun {

	class SocketClient {
		int retval;

		WSADATA wsa;
		SOCKET sock;
		SOCKADDR_IN serveraddr;

		// ������ ��ſ� ����� ����
		char buf[BUFSIZE + 1];
		int len;

		// ���� �Լ� ���� ��� �� ����
		void err_quit(char *msg);

		// ���� �Լ� ���� ���
		void err_display(char *msg);

		// ����� ���� ������ ���� �Լ�
		int recvn(SOCKET s, char *buf, int len, int flags);
	public:
		SocketClient();
		~SocketClient();

		int input_data();

		int send_data();

		int recv_data();

		char *get_data();
	};

}

#endif
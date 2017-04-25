#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <winsock2.h>

#define BUFSIZE 1024000

namespace jun {

	class SocketServer {
	private:
		int retval;

		WSADATA wsa;
		SOCKET listen_sock;
		SOCKADDR_IN serveraddr;

		//데이터 통신에 사용할 변수
		SOCKET client_sock;
		SOCKADDR_IN clientaddr;
		int addrlen;
		char buf[BUFSIZE + 1];

		//소켓 함수 오류 출력 후 종료
		void err_quit(char *msg);

		//소켓 함수 오류 출력
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
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

		// 데이터 통신에 사용할 변수
		char buf[BUFSIZE + 1];
		int len;

		// 소켓 함수 오류 출력 후 종료
		void err_quit(char *msg);

		// 소켓 함수 오류 출력
		void err_display(char *msg);

		// 사용자 정의 데이터 수신 함수
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
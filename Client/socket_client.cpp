#include "socket_client.h"

#include <stdlib.h>
#include <stdio.h>

#pragma warning(disable:4996)
#pragma comment (lib, "ws2_32.lib")

#define SOCKET_PORT 80

void jun::SocketClient::err_quit(char * msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 9, NULL);
	wchar_t wtext[100];
	mbstowcs(wtext, msg, strlen(msg) + 1);
	LPWSTR wmsg = wtext;
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, wmsg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

void jun::SocketClient::err_display(char * msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int jun::SocketClient::recvn(SOCKET s, char * buf, int len, int flags) {
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

jun::SocketClient::SocketClient() {
	// 윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	serveraddr;
	char SVADDR[200];
	printf("서버 주소 입력: ");
	fgets(SVADDR, 199, stdin);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SOCKET_PORT);
	serveraddr.sin_addr.s_addr = inet_addr(SVADDR);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");
}

jun::SocketClient::~SocketClient() {
	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
}

int jun::SocketClient::input_data() {
	// 데이터 입력
	ZeroMemory(buf, sizeof(buf));
	printf("\n[보낼 데이터]");
	if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
		return -1;

	// '\n' 문자 제거
	len = strlen(buf);
	if (buf[len - 1] == '\n')
		buf[len - 1] = '\0';
	if (strlen(buf) == 0)
		return -1;

	return 1;
}

int jun::SocketClient::send_data() {
	// 데이터 보내기
	retval = send(sock, buf, strlen(buf), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return -1;
	}

	printf("[클라이언트] %d바이트를 보냈습니다.\n", retval);
	return 1;
}

int jun::SocketClient::recv_data() {
	// 데이터 받기
	retval = recvn(sock, buf, retval, 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return -1;
	} else if (retval == 0)
		return -1;

	// 받은 데이터 출력
	buf[retval] = '\0';
	printf("[클라이언트] %d바이트를 받았습니다.\n", retval);
	printf("[받은 데이터] %s\n", buf);
	return 1;
}

char * jun::SocketClient::get_data() {
	return buf;
}

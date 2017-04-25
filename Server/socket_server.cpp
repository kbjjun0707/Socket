#include "socket_server.h"

#include <stdio.h>
#include <stdlib.h>
#include <atlconv.h>

#pragma warning(disable:4996)
#pragma comment (lib, "ws2_32.lib")

#define SOCKET_PORT 80

//소켓 함수 오류 출력 후 종료
void jun::SocketServer::err_quit(char *msg) {
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

//소켓 함수 오류 출력
void jun::SocketServer::err_display(char *msg) {
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

jun::SocketServer::SocketServer() {
	//윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	//socket()
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	//bind()
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SOCKET_PORT);						// 포트
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	//linsten()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
}

jun::SocketServer::~SocketServer() {
	//clossocket()
	closesocket(listen_sock);

	//윈속 종료
	WSACleanup();
}

int jun::SocketServer::wait_accept() {
	printf("[서버] 서버 IP주소 = %s, 포트 번호 = %d\n", 
		getIPAddress(), ntohs(serveraddr.sin_port));

	//accept()
	addrlen = sizeof(clientaddr);
	client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
	if (client_sock == INVALID_SOCKET) {
		err_display("accept()");
		return -1;
	}
	printf("\n[서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", 
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 1;
}

void jun::SocketServer::CLS_SCK() {
	//closesocket()
	closesocket(client_sock);
	printf("[서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", 
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
}

int jun::SocketServer::input_data() {
	// 데이터 입력
	ZeroMemory(buf, sizeof(buf));
	printf("\n[보낼 데이터]");
	if (fgets(buf, BUFSIZE, stdin) == NULL)
		return -1;

	// '\n' 문자 제거
	int len = strlen(buf);
	if (buf[len - 1] != '\0')
		buf[len - 1] = '\0';
	if (strlen(buf) == 0)
		return -1;

	return 1;
}

int jun::SocketServer::recv_data() {
	//데이터 받기
	retval = recv(client_sock, buf, BUFSIZE, 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return -1;
	}
	else if (retval == 0)
		return -1;

	//받은 데이터 출력
	buf[retval] = '\0';
	printf("[receive / %s:%d]\n%s\n", 
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

	return 1;

}

int jun::SocketServer::send_data() {
	//데이터 보내기
	retval = send(client_sock, buf, retval, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return -1;
	}
	return 1;
}

char * jun::SocketServer::get_data() { return buf; }

int jun::SocketServer::getBufSize()
{
	return BUFSIZE;
}

// IP 얻기
char* jun::SocketServer::getIPAddress() {
	char* strIPAddress = NULL;
	char name[_MAX_PATH + 1] = { 0 };		// 컴퓨터 이름
	hostent* hostinfo;

	if (gethostname(name, sizeof(name)) == 0) {
		if ((hostinfo = gethostbyname(name)) != NULL) {
			for (int i = 0; hostinfo->h_addr_list[i] != NULL; i++) {
				strIPAddress = inet_ntoa(*(in_addr *)hostinfo->h_addr_list[i]);
				break;
			}
		}
	}
	return strIPAddress;
}
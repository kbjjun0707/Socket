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
	// ���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	serveraddr;
	char SVADDR[200];
	printf("���� �ּ� �Է�: ");
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

	// ���� ����
	WSACleanup();
}

int jun::SocketClient::input_data() {
	// ������ �Է�
	ZeroMemory(buf, sizeof(buf));
	printf("\n[���� ������]");
	if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
		return -1;

	// '\n' ���� ����
	len = strlen(buf);
	if (buf[len - 1] == '\n')
		buf[len - 1] = '\0';
	if (strlen(buf) == 0)
		return -1;

	return 1;
}

int jun::SocketClient::send_data() {
	// ������ ������
	retval = send(sock, buf, strlen(buf), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return -1;
	}

	printf("[Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);
	return 1;
}

int jun::SocketClient::recv_data() {
	// ������ �ޱ�
	retval = recvn(sock, buf, retval, 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return -1;
	} else if (retval == 0)
		return -1;

	// ���� ������ ���
	buf[retval] = '\0';
	printf("[Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
	printf("[���� ������] %s\n", buf);
	return 1;
}

char * jun::SocketClient::get_data() {
	return buf;
}

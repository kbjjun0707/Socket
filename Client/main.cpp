#include "socket_client.h"


int main() {
	jun::SocketClient m_Client;

	// ���� ������ ���� ��
	while (1) {
		m_Client.input_data();
		if (m_Client.send_data() != 1) break;
		if (m_Client.recv_data() != 1) break;
	}


	return 0;
}
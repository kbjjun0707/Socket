#include "socket_client.h"


int main() {
	jun::SocketClient m_Client;

	// 에코 서버에 대한 예
	while (1) {
		m_Client.input_data();
		if (m_Client.send_data() != 1) break;
		if (m_Client.recv_data() != 1) break;
	}


	return 0;
}
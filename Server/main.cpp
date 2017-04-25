#include "socket_server.h"

#include <iostream>
using namespace std;

int main() {
	jun::SocketServer m_Server;

	// 에코 서버로 사용
	while (m_Server.wait_accept() == 1) {
		while (1) {
			if (m_Server.recv_data() != 1) {
				m_Server.CLS_SCK();
				break;
			}			
			if (m_Server.send_data() != 1) {
				m_Server.CLS_SCK();
				break;
			}
		}
	}


	return 0;
}
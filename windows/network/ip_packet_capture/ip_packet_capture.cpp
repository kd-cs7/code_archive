#include <winsock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

void print_packet(const char* buffer, int length) {
	std::cout << "Captured Packet Length: " << length << " bytes" << std::endl;
	for (int i = 0; i < length; i++) {
		printf("%02X ", (unsigned char)buffer[i]);
		if ((i + 1) % 16 == 0) std::cout << std::endl;
	}
	std::cout << "\n-----------------------------------\n";
}

int main() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "WSAStartup failed." << std::endl;
		return 1;
	}

	SOCKET raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	if (raw_socket == INVALID_SOCKET) {
		std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
		std::cerr << "Get hostname failed: " << WSAGetLastError() << std::endl;
		closesocket(raw_socket);
		WSACleanup();
		return 1;
	}

	struct hostent* host = gethostbyname(hostname);
	if (host == nullptr) {
		std::cerr << "Get host info failed." << std::endl;
		closesocket(raw_socket);
		WSACleanup();
		return 1;
	}

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ((struct in_addr*)host->h_addr_list[0])->s_addr;
	addr.sin_port = 0;

	if (bind(raw_socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
		closesocket(raw_socket);
		WSACleanup();
		return 1;
	}

	DWORD mode = 1;
	if (ioctlsocket(raw_socket, SIO_RCVALL, &mode) == SOCKET_ERROR) {
		std::cerr << "Failed to set promiscuous mode: " << WSAGetLastError() << std::endl;
		closesocket(raw_socket);
		WSACleanup();
		return 1;
	}

	std::cout << "Listening for packets..." << std::endl;
	char buffer[65536];
	while (true) {
		int packet_size = recv(raw_socket, buffer, sizeof(buffer), 0);
		if (packet_size > 0) {
			print_packet(buffer, packet_size);
		}
	}

	closesocket(raw_socket);
	WSACleanup();
	return 0;
}


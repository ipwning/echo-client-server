#include "echo.h"

using namespace std;

extern struct in_addr IP;
extern uint16_t PORT;

void recvThread(int sd) {
	cout << "connected\n";
	static const int BUFSIZE = 65536;
	char buf[BUFSIZE];
	while (true) {
		ssize_t res = recv(sd, buf, BUFSIZE - 1, 0);
		if (res == 0 || res == -1) {
			cerr << "recv return " << res;
			perror(" ");
			break;
		}
		buf[res] = '\0';
		cout << buf;
		cout.flush();
	}
	cout << "disconnected\n";
	close(sd);
	exit(0);
}

int main (int argc, const char *argv[]) {
    argParse(argc, argv, 1);
    
	int sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd == -1) {
		perror("socket");
		return -1;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr = IP;
	memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

	int res = connect(sd, (struct sockaddr *)&addr, sizeof(addr));
	if (res == -1) {
		perror("connect");
		return -1;
	}

	thread t(recvThread, sd);
	t.detach();

	while (true) {
		string s;
		getline(cin, s);
		s += "\r\n";
		ssize_t res = send(sd, s.c_str(), s.size(), 0);
		if (res == 0 || res == -1) {
			cerr << "send return " << res;
			perror(" ");
			break;
		}
	}
	close(sd);
}
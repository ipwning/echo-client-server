#include "echo.h"

using namespace std;

extern uint16_t PORT;
extern bool IS_ECHO;
extern bool IS_BROADCAST;

list<int> SOCKET_LIST;

void recvThread(int sd) {
	static const int BUFSIZE = 65536;
	char buf[BUFSIZE];
    cout << "connected" << endl;
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
		if (IS_ECHO) {
            if(IS_BROADCAST) {
                for (int _sd : SOCKET_LIST) {
                    res = send(_sd, buf, res, 0);
                    if (res == 0 || res == -1) {
                        cerr << "send return " << res;
                        perror(" ");
                        break;
                    }
                }
            } else {
                res = send(sd, buf, res, 0);
                if (res == 0 || res == -1) {
                    cerr << "send return " << res;
                    perror(" ");
                    break;
                }
            }
		}
	}
	cout << "disconnected\n";
	close(sd);
}

int main (int argc, const char *argv[]) {
    argParse(argc, argv, 0);
    
	int sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd == -1) {
		perror("socket");
		return -1;
	}

	int res;
	int optval = 1;
	res = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (res == -1) {
		perror("setsockopt");
		return -1;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);
	ssize_t res2 = ::bind(sd, (struct sockaddr *)&addr, sizeof(addr));
	if (res2 == -1) {
		perror("bind");
		return -1;
	}

	res = listen(sd, 5);
	if (res == -1) {
		perror("listen");
		return -1;
	}

	while (true) {
		struct sockaddr_in cli_addr;
		socklen_t len = sizeof(cli_addr);
		int cli_sd = accept(sd, (struct sockaddr *)&cli_addr, &len);
		if (cli_sd == -1) {
			perror("accept");
			break;
		}
        SOCKET_LIST.push_back(cli_sd);
		thread* t = new thread(recvThread, cli_sd);
		t->detach();
	}
	close(sd);
}

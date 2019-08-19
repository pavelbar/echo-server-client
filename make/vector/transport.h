#ifndef __TRANSPORT_H__
#define __TRANSPORT_H__

#include <sys/select.h> // fd_set

/*Message read buffer size and clients arr size*/
static const int BUF_SIZE = 100;

/*The protocol family which will be used for communicatio*/
static const int PROTOCOL = 0;

/*The backlog argument defines the maximum length to which the queue ofpending
 * connections for sockfd may grow*/
static const int BACKLOG = 10;

/*Setting the Timeout Value*/
static const int TIMEOUT = 10;

static int _listener = 0;  // fd
static int* _pVector;  // arr fd
static fd_set _readset;

void createServer(int port);
static void createListener(int port);
int waitingForConnection();
int newConnection();
int parseDateFromClient();
void deleteServer();
int _max(int a, int b);

#endif

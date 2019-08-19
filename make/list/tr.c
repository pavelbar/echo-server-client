#include <netdb.h>   // AF_INET, SOCK_STREAM, INADDR_ANY, sockaddr_in
#include <stdlib.h>  // exit() malloc()
#include <stdio.h>   // perror(), printf
#include <fcntl.h>   // fcntl(), F_SETFL, O_NONBLOCK
#include "transport.h"

void createServer(int port) {
  CreateList(&_pFirst, &_pCurr);
  createListener(port);
}

int _max(int a, int b) {
  return a > b ? a : b;
}

void createListener(int port) {
  printf("\n  Current settings: \n");
  printf("-> Input port: %d \n", port);

  int listener = socket(AF_INET, SOCK_STREAM, PROTOCOL);
  if (listener < 0) {
    perror("Could not create socket \n");
    exit(1);
  } else {
    printf("-> domain: %d \n", AF_INET);
    printf("-> type: %d \n", SOCK_STREAM);
    printf("-> protocol: %d \n", PROTOCOL);
    printf("-> listener: %d \n", listener);
  }

  int err = fcntl(listener, F_SETFL, O_NONBLOCK);
  if (err == -1) {
    perror("Error in fcntl() \n");
    close(listener);
    exit(2345);
  } else {
    printf("-> + Successful fcntl \n");
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  err = bind(listener, (struct sockaddr*)&addr, sizeof(addr));
  if (err < 0) {
    printf("-> - Port %d is bussy \n", port);
    int port = 0;
    int portFound = 0;

    for (port = 50000; port <= 55000; port++) {
      addr.sin_port = htons(port);
      err = bind(listener, (struct sockaddr*)&addr, sizeof(addr));
      if (err >= 0) {
        portFound = 1;
        break;
      }
    }

    if (portFound == 1) {
      printf("-> Port: %d \n", port);
    } else {
      perror("Not fount empty port \n");
      close(listener);
      exit(2345);
    }
  } else {
    printf("-> + Successful binding \n");
  }

  err = listen(listener, BACKLOG);
  if (err < 0) {
    perror("Could not listen on socket\n");
    close(listener);
    exit(345);
  } else {
    printf("-> + Successful listening \n");
    printf("-> backlog: %i \n", BACKLOG);
  }
  _listener = listener;
}

int waitingForConnection() {
  FD_ZERO(&_readset);
  FD_SET(_listener, &_readset);

  int ind = 0;
  int sizeVec = GetSize();

  for (ind = 0; ind < sizeVec; ind++) {
    FD_SET(GetElem(ind, &_pFirst, &_pCurr), &_readset);
  }

  //
  struct timeval timeout;
  timeout.tv_sec = TIMEOUT;
  timeout.tv_usec = 0;

  //
  int mx = _max(_listener, getMaxElem(&_pFirst, &_pCurr));

  /*=========== there is a wait for connection*/

  puts("-> ? Waiting for connection");
  int count = 0;
  count = select(mx + 1, &_readset, NULL, NULL, &timeout);

  if (count == 0) {
    printf("-> - Time out %d sec \n", TIMEOUT);
    return 1;
  }
  if (count < 0) {
    perror("Error in select() \n");
    close(_listener);
    exit(3);
  }

  return 0;
}

int newConnection() {
  if (FD_ISSET(_listener, &_readset)) {
    /*=========== if connection request */

    printf("-> + Listener in set\n");
    //
    int sock = accept(_listener, NULL, NULL);
    if (sock >= FD_SETSIZE) {
      perror("fd big size. Err in select()\n");
      close(sock);
      return 1;
    }
    if (sock < 0) {
      perror("Could not establish new connection \n");
      close(_listener);
      exit(3);
    } else {
      printf("\n-> + Connected client (listener: %d) (client: %d)\n", _listener,
             sock);
    }

    int err = fcntl(sock, F_SETFL, O_NONBLOCK);
    if (err == -1) {
      perror("Error in fcntl() \n");
      close(_listener);
      close(sock);
      exit(2345);
    } else {
      printf("-> + Successful fcntl \n");
    }
    
    printf("sock %d \n", sock);
    printf("GetSize() undo  %d \n", GetSize());
    InSertFirst(sock, &_pFirst, &_pCurr);
    printf("GetSize() after  %d \n", GetSize());
    printf("GetElem(int pos)  %d \n", GetElem(0, &_pFirst, &_pCurr));
  }

  return 0;
}

int parseDateFromClient() {
  int sizeVec = GetSize();
  int ind = 0;
  char buf[BUF_SIZE];
  int bytes_read = 0;
  int err = 0;

  for (ind = 0; ind < sizeVec; ind++) {
    if (FD_ISSET(GetElem(ind, &_pFirst, &_pCurr), &_readset)) {
      /*=========== if have data from the client */
      bytes_read = recv(GetElem(ind, &_pFirst, &_pCurr), buf, BUF_SIZE, 0);
      if (bytes_read <= 0) {
        /*=========== if disconect */
        close(GetElem(ind, &_pFirst, &_pCurr));
	Delete(ind, &_pFirst, &_pCurr);
        return 1;
      }
      printf("-> (Source: %d / Bytes: %d) :%s \n",
             GetElem(ind, &_pFirst, &_pCurr), bytes_read, buf);

      /*=========== send answer */
      err = send(GetElem(ind, &_pFirst, &_pCurr), buf, bytes_read, 0);
      if (err == -1) {
        perror("Error in send() \n");
        close(GetElem(ind, &_pFirst, &_pCurr));
        close(_listener);
        exit(2345);
      }
    }
  }
  return 0;
}

void deleteServer() {
  close(_listener);
  DeleteAll(&_pFirst, &_pCurr);
}



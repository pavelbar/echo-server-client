#include "lifecycle.h"
#include "transport.h"

void lifeCycle(int port){
  createServer(port);  		   //"transport.h"
  int err = 0;
  
  while (1) {
    err = waitingForConnection();  //"transport.h"
    if (err == 1) {
      // timeout
      continue;
    }

    err = newConnection(); 	   //"transport.h"
    if (err == 1) {
      // fd big size
      continue;
    }

    err = parseDateFromClient();   //"transport.h"
    if (err == 1) {
      // disconect
      continue;
    }
  }

  deleteServer();  		   //"transport.h"
  } 

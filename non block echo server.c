// server

#include <argp.h>
#include <netdb.h> // AF_INET, SOCK_STREAM, INADDR_ANY, sockaddr_in
#include <string.h> // bzero(), strlen()
#include <stdio.h> // perror(), printf
#include <stdlib.h> // exit()
#include <fcntl.h> // fcntl()

///////////////////////
///////////////////////
// server
///////////////////////
///////////////////////

/*Message read buffer size and clients arr size*/
static const int BUF_SIZE = 100;

/*The protocol family which will be used for communicatio*/
static const int PROTOCOL = 0;

/*The backlog argument defines the maximum length to which the queue ofpending
 * connections for sockfd may grow*/
static const int BACKLOG = 10;

/*Setting the Timeout Value*/
static const int TIMEOUT = 10;

///////////////////////
///////////////////////
// argp
///////////////////////
///////////////////////

/* Program version */
const char* argp_program_version = "Echoserver 0.1";

/* Program bug address */
const char* argp_program_bug_address = "<.com>";

/* Program description. */
static const char argp_program_desc[] = "An Echoserver accepts connection requests from clients on some port. "
                                        "After the connection has been established, the server simply echoes back "
                                        "all the data arriving from the client, until the client disconnects.";

/* The options we understand. */
const static struct argp_option options[] = {
    // name, key, argname, flags, doc, group
    { "port", 'p', "PORT", 0, "connection port" },
    { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments {
    int port;
};

/* Parse a single option. */
static error_t parse_option(int key, char* arg, struct argp_state* state)
{
    if (state->argc == 1) {
        argp_usage(state);
    }

    struct arguments* arguments = state->input;

    switch (key) {
    case 'p':
        arguments->port = atoi(arg);
        break;

    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_option, 0, argp_program_desc };

///////////////////////
///////////////////////
// vector
///////////////////////
///////////////////////

static const int MAX_VECTOR_SIZE = 10000;
static int size = 0; // secret
static int* pVector; // secret

static void createVector()
{
    pVector = (int*)malloc(MAX_VECTOR_SIZE * sizeof(int));
}

static int getSizeVector()
{
    return size;
}

static int getElemAtVector(int pos)
{
    if ((pos < 0) || (pos >= size)) {
        perror("Error in getElemVector() \n");
    }
    return pVector[pos];
}

static void addElemAtVector(int val)
{
    if (size == MAX_VECTOR_SIZE) {
        perror("Error in addElemAtVector() \n");
    }

    pVector[size] = val;
    size++;
}

static void setElemAtVector(int pos, int val)
{
    if ((pos < 0) || (pos >= size)) {
        perror("Error in setElemAtVector() \n");
    }
    pVector[pos] = val;
}

static void delElemAtVector(int pos)
{
    if ((pos < 0) || (pos >= size)) {
        perror("Error in delElemAtVector() \n");
    }
    int oldInd = 0;
    int newInd = 0;
    for (oldInd = 0; oldInd < size; oldInd++) {
        if (oldInd != pos) {
            pVector[newInd] = pVector[oldInd];
            newInd++;
        }
    }
    size--;  
}

static int getMaxElemAtVector()
{
    int max = 0;
    if (size != 0) {
        max = pVector[0];
    }
    else {            
        return 0;
        //perror("Error in getMaxElemAtVector() \n");
    }

    int i = 0;
    for (i = 0; i < size; ++i) {
        if (pVector[i] > max) {
            max = pVector[i];
        }
    }
    return max;
}

static int _max(int a, int b) { return a > b ? a : b; }

///////////////////////
///////////////////////

int main(int argc, char* argv[])
{
    struct arguments arguments;
    int err = 0;
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    if (err != 0) {
        perror("Error in argp_parse() \n");
        exit(012);
    }

    printf("\n  Current settings: \n");
    printf("-> Input port: %d \n", arguments.port);

    int listener = socket(AF_INET, SOCK_STREAM, PROTOCOL);
    if (listener < 0) {
        perror("Could not create socket \n");
        exit(1);
    }
    else {
        printf("-> domain: %d \n", AF_INET);
        printf("-> type: %d \n", SOCK_STREAM);
        printf("-> protocol: %d \n", PROTOCOL);
        printf("-> listener: %d \n", listener);
    }

    err = fcntl(listener, F_SETFL, O_NONBLOCK);
    if (err == -1) {
        perror("Error in fcntl() \n");
        close(listener);
        exit(2345);
    }
    else {
        printf("-> + Successful fcntl \n");
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(arguments.port);
    addr.sin_addr.s_addr = INADDR_ANY;
    err = bind(listener, (struct sockaddr*)&addr, sizeof(addr));
    if (err < 0) {
    	printf("-> - Port %d is bussy \n", arguments.port);
	int port = 0;
	int portFound = 0;
	
    	for(port = 50000; port <= 55000; port++){
	 addr.sin_port = htons(port);
	 err = bind(listener, (struct sockaddr*)&addr, sizeof(addr));
	  if (err >= 0) {
	    portFound = 1;
	    break;
	  }
	}
	
	if(portFound == 1){
	    arguments.port = port;
	    printf("-> Port: %d \n", arguments.port);
	} 
	else {
	        perror("Not fount empty port \n");
        	close(listener);
        	exit(2345);
	}
    }
    else {
        printf("-> + Successful binding \n");
    }

    err = listen(listener, BACKLOG);
    if (err < 0) {
        perror("Could not listen on socket\n");
        close(listener);
        exit(345);
    }
    else {
        printf("-> + Successful listening \n");
        printf("-> backlog: %i \n", BACKLOG);
    }

    char buf[BUF_SIZE];
    int bytes_read = 0;
    createVector(MAX_VECTOR_SIZE);

    while (1) {
        //
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(listener, &readset);

        int ind = 0;
        int sizeVec = getSizeVector();
        for (ind = 0; ind < sizeVec; ind++) {
            FD_SET(getElemAtVector(ind), &readset);
        }

        //
        struct timeval timeout;
        timeout.tv_sec = TIMEOUT;
        timeout.tv_usec = 0;

        //
        int mx = _max(listener, getMaxElemAtVector());

        /*=========== there is a wait for connection*/

        puts("-> ? Waiting for connection");
        int count = 0;
        count = select(mx + 1, &readset, NULL, NULL, &timeout);

        if (count == 0) {
	    printf("-> - Time out %d sec \n", TIMEOUT);
            continue;
        }
        if (count < 0) {
            perror("Error in select() \n");
            close(listener);
            exit(3);
        }
        /*=========== there is a detect type of event */
	
	int sock = 0;
        if (FD_ISSET(listener, &readset)) {
            /*=========== if connection request */

            printf("-> + Listener in set\n");
            //
            sock = accept(listener, NULL, NULL);            
	    if (sock >= FD_SETSIZE) {
                perror("fd big size. Err in select()\n");
                close(sock);
                continue;
            }
		
            if (sock < 0) {
                perror("Could not establish new connection \n");
                close(listener);
                exit(3);
            }
            else {
                printf("\n-> + Connected client (listener: %d) (client: %d)\n", listener, sock);
            }

            err = fcntl(sock, F_SETFL, O_NONBLOCK);
            if (err == -1) {
                perror("Error in fcntl() \n");
                close(listener);
                close(sock);
                exit(2345);
            }
            else {
                printf("-> + Successful fcntl \n");
            }
            addElemAtVector(sock);
        }

        sizeVec = getSizeVector();
        for (ind = 0; ind < sizeVec; ind++) {
            if (FD_ISSET(getElemAtVector(ind), &readset)) {

                /*=========== if have data from the client */
                bytes_read = recv(getElemAtVector(ind), buf, BUF_SIZE, 0);
                if (bytes_read <= 0) {
                    /*=========== if disconect */
                    close(getElemAtVector(ind));
                    delElemAtVector(ind);
                    continue;
                }
		printf("-> (Source: %d / Bytes: %d) :%s \n", sock, bytes_read, buf);

                /*=========== send answer */
                err = send(getElemAtVector(ind), buf, bytes_read, 0);
		if (err == -1){
		   perror("Error in send() \n");
                   close(getElemAtVector(ind));
   		   close(listener);
                   exit(2345);
		}
            }
        }
    }
close(listener);
free(pVector);
return 0;
}

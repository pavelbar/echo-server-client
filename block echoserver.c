#include <argp.h>
#include <netdb.h>	// AF_INET, SOCK_STREAM, INADDR_ANY, sockaddr_in
#include <string.h>	// bzero(), strlen()
#include <stdio.h> 	// perror()
#include <stdlib.h>	// exit()

/*Message read buffer size*/
const int BUF_SIZE = 100;

/*The protocol family which will be used for communicatio*/
const int PROTOCOL = 0;

/*The backlog argument defines the maximum length to which the queue ofpending connections for sockfd may grow*/
const int BACKLOG = 10;

/* Program version */
const char* argp_program_version = "Echoserver 0.1";

/* Program bug address */
const char* argp_program_bug_address = "<....com>";

/* Program description. */
const static char argp_program_desc[] = "An Echoserver ...";

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

int main(int argc, char* argv[])
{
    struct arguments arguments;
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    
    printf("\n  Current settings: \n");
    printf("-> port: %i \n", arguments.port);
     
    int listener_fd = socket(AF_INET, SOCK_STREAM, PROTOCOL);	//return: fd for socket (positive)
    if (listener_fd < 0){
       perror("Could not create socket \n");
       exit(123);
    }
    else{
	printf("-> domain: %i \n", AF_INET);
	printf("-> type: %i \n", SOCK_STREAM);
	printf("-> protocol: %i \n", PROTOCOL);
	printf("-> listener_fd: %i \n", listener_fd);
    }
 
    struct sockaddr_in servaddr;
    bzero( &servaddr, sizeof(servaddr)); /* set everything to zero */
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY); //host to network short
    servaddr.sin_port = htons(arguments.port);  //host to network short
    
    int err = 0;
    err = bind(listener_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if (err < 0){
    perror("Could not bind socket \n");
    //here foo() for brut force rorts
    exit(234);
    }
    else{
    	printf("-> + Successful binding \n");
    }
 
    err = listen(listener_fd, BACKLOG);
    if (err < 0){
    perror("Could not listen on socket\n");
    exit(345);
    }
    else{
        printf("-> + Successful listening \n");
	printf("-> backlog: %i \n", BACKLOG);
    }
    
    int comm_fd = 0;
    comm_fd = accept(listener_fd, (struct sockaddr*) NULL, NULL);
    if (comm_fd < 0){
    perror("Could not establish new connection \n");
    exit(456);
    }
    else{
        printf("\n-> + Successful accepting (comm_fd: %i)\n", comm_fd);
    }
    
    char str[BUF_SIZE];
    
    while(1)
    {

        bzero( str, BUF_SIZE); /* set everything to zero */
 
        read(comm_fd, str, BUF_SIZE);
 
        printf("(%d) Echoing back - %s",comm_fd, str);
 
        write(comm_fd, str, strlen(str)+1);
 
    }
    
     close(comm_fd);
    
    return 0;
}

//https://makework.blog/
//https://devpreview.livejournal.com/12567.html
//https://gist.github.com/ictlyh/b9be0b020ae3d044dc75ef0caac01fbb
//https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/
//https://rsdn.org/article/unix/sockets.xml 

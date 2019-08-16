// client

#include <argp.h>
#include <netdb.h>   // AF_INET, SOCK_STREAM
#include <stdio.h>   // stdin()
#include <string.h>  // bzero(), strlen()
#include <stdlib.h>  // exit()

/*Message read buffer size*/
const int BUF_SIZE = 100;

/*The protocol family which will be used for communicatio*/
const int PROTOCOL = 0;

/* Program version */
const char* argp_program_version = "Echoclient 0.1";

/* Program bug address */
const char* argp_program_bug_address = "<.com>";

/* Program description. */
const static char argp_program_desc[] = "An Echoclient sent msg to echoserver.";

/* The options we understand. */
const static struct argp_option options[] = {
    // name, key, argname, flags, doc, group
    {"port", 'p', "PORT", 0, "connection port"},
    {0}};

/* Used by main to communicate with parse_opt. */
struct arguments {
  int port;
};

/* Parse a single option. */
static error_t parse_option(int key, char* arg, struct argp_state* state) {
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
static struct argp argp = {options, parse_option, 0, argp_program_desc};

int main(int argc, char** argv) {
  struct arguments arguments;
  int err = 0;
  err = argp_parse(&argp, argc, argv, 0, 0, &arguments);
  if (err != 0) {
    perror("Error in argp_parse() \n");
    exit(012);
  }

  printf("\n  Current settings: \n");
  printf("-> port: %i \n", arguments.port);

  int sockfd = socket(AF_INET, SOCK_STREAM, PROTOCOL);  // return: fd for socket (positive)
  if (sockfd < 0) {
    perror("Could not create socket \n");
    exit(123);
  } else {
    printf("-> domain: %i \n", AF_INET);
    printf("-> type: %i \n", SOCK_STREAM);
    printf("-> protocol: %i \n", PROTOCOL);
    printf("-> listener_fd: %i \n", sockfd);
  }

  struct sockaddr_in servaddr;
  bzero(&servaddr, sizeof servaddr);
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(arguments.port);

  err = inet_pton(AF_INET, "127.0.0.1", &(servaddr.sin_addr));
  if (err < 0) {
    perror("Error in inet_pton() \n");
    exit(234);
  } else {
    printf(" + Successful inet_pton() \n");
  }

  err = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
  if (err < 0) {
    perror("Error in connect() \n");
    exit(345);
  } else {
    printf("\n-> + Successful connecting (sockfd: %i)\n", sockfd);
  }

  char sendline[BUF_SIZE];
  char recvline[BUF_SIZE];
  while (1) {
    bzero(sendline, BUF_SIZE);
    fgets(sendline, BUF_SIZE, stdin); /*stdin = 0 , for standard input */
    err = write(sockfd, sendline, strlen(sendline) + 1);
    if (err == -1) {
      perror("Error in write() \n");
      exit(456);
    }

    bzero(recvline, BUF_SIZE);
    err = read(sockfd, recvline, BUF_SIZE);
    if (err == -1) {
      perror("Error in read() \n");
      exit(567);
    }
    printf("-> %s", recvline);
  }
    return 0;
}

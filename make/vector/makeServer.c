1#include <argp.h>
#include <stdlib.h>  // exit() malloc()
#include "lifecycle.h"

/* Program version */
const char* argp_program_version = "Echoserver 0.1";

/* Program bug address */
const char* argp_program_bug_address = "<.com>";

/* Program description. */
static const char argp_program_desc[] =
    "An Echoserver accepts connection requests from clients on some port. "
    "After the connection has been established, the server simply echoes back "
    "all the data arriving from the client, until the client disconnects.";

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

int main(int argc, char* argv[]) {
  struct arguments arguments;  
  int err = argp_parse(&argp, argc, argv, 0, 0, &arguments);
  if (err != 0) {
    perror("Error in argp_parse() \n");
    exit(101);
  }

  lifeCycle(arguments.port);  //"lifecycle.h

  return 0;
}

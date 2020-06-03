#ifndef SERVER_H_
#define SERVER_H_

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>

#include <iostream>

#include "parseHttp.h"
#include "controller.h"

#endif // !SERVER_H_
void startServer();

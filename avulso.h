#pragma once
/*----------- Bliblioteca sockets POSIX -----------*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
//-------------------------------------------------//
#include <stdio.h>
#include <unistd.h>
#include <string> /* memset() */
#include <string.h>
#include <iostream>   // std::cout
#include <stdexcept>
#include <vector>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <utility>      // std::pair
#include "Adress.h"
#include <map>
#ifndef EVENT_HPP
#define EVENT_HPP

#include "EpollModule.hpp"
#include "Webserv.hpp"

extern Epoll ev;

void proceeEventsAndTimers(Webserv& ws);

#endif

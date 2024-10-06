
ngx_int_t ngx_open_listening_socket(ngx_cycle_t* cycle)
{
	ngx_listening_t* ls = cycle->listening.elts;
	for (i = 0; i < cycle->listening.nelts; i++)
	{
		ngx_socket_t s = socket(ls[i].sockaddr->sa_family, ls[i].type, 0);
		setsockopt(s, SOL_SOCKET, SO_REUSEADDR, ...);
		setsockopt(s, SOL_SOCKET, SO_REUSEPORT, ...);
		/* some additional setsockopt() */
		if (/* some condition */)
		{
			ngx_nonblocking(s);
			/* using ioctl() inside this function */
		}
		bind(s, ls[i].sockaddr, ls[i].socklen);
		listen(s, ls[i].backlog);
		ls[i].listen = 1;
		ls[i].fd = s;
	}
}


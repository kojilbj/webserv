void ngx_event_accept(ngx_event_t* ev)
{
	ngx_enable_accept_event(cycle);
	ngx_connection_t* lc = ev->data;
	ngx_listening_t* ls = lc->listening;
	do
	{
		socklen_t socklen = sizeof(ngx_sockaddr_t);
		ngx_soket_t s = accept(lc->fd, &sa.sockaddr, &socklen);
		if (s == -1)
		{
			/* error handling */
		}
		c = ngx_get_connection(s, ev->log);
		/* create part of c */
		ngx_nonblocking(s);
		c->recv = ngx_recv;
		c->send = ngx_send;
		/* some of c initialization */
		ls->handler(c);
		/* this call ngx_http_init_connection() */
	}
}

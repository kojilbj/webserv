
void ngx_http_init_connection(ngx_connection_t* c)
/* a handler to pass a new connection object */
{
	ngx_http_connection_t* hc = ngx_palloc(...);
	c->data = hc;
	/* some of c initialization */
	ngx_event_t* rev = c->read;
	rev->handler = ngx_http_wait_request_handler;
	c->write->handler = ngx_http_empty_handler;
	if (rev->ready)
		rev->handler(rev);
}

static void ngx_http_wait_request_handler(ngx_event_t* rev)
{
	ngx_connection_t* c = rev->data;
	ssize_t n = c->recv(c, ...);
	rev->handler = ngx_http_process_request_line;
	ngx_http_process_request_line(rev);
}

static void ngx_http_process_request_line(ngx_event_t* rev)
{
	ngx_int_t rc = NGX_AGAIN;
	ssize_t n;
	ngx_http_request_t* r;

	for (;;)
	{
		if (rc == NGX_AGAIN)
			n = ngx_http_read_request_header(r);
		rc = ngx_http_parse_request_line(r, r->header_in);
		if (rc == NGX_OK)
		{
			ngx_http_process_request_uri(r);
			rev->handler = ngx_http_process_request_headers;
			ngx_http_process_request_headers(rev);
			break;
		}
		else if (rc != NGX_AGAIN)
		{
			/* error handling */
			/* ex. BAD_REQUEST */
		}
	}
}

static void ngx_http_process_request_headers(ngx_event_t* rev)
{
	ngx_int_t rc = NGX_AGAIN;

	for (;;)
	{
		if (rc == NGX_AGAIN)
		{
			/* some code */
		}
		rc = ngx_http_parse_header_line();
		if (rc == NGX_OK)
		{
			/* some code */
			continue;
		}
		if (rc == NGX_PARSE_HEADER_DONE)
		{
			rc = ngx_http_process_request_header(r);
			ngx_http_process_request(r);
		}
	}
}

void ngx_http_process_request(ngx_event_t* r)
{
	ngx_connection_t* c = r->connection;
	c->read->handler = ngx_http_request_handler;
	c->write->handler = ngx_http_request_handler;
	r->read_event_handler = ngx_http_block_reading;
	ngx_http_handler(r);
}

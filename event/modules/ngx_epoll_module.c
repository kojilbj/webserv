
ngx_module_t ngx_epoll_module = {NGX_MODULE_V1,
								 &ngx_epoll_module_ctx, /* module context */
								 ngx_epoll_commands, /* module directives */
								 NGX_EVENT_MODULE, /* module type */
								 NULL, /* init master */
								 NULL, /* init module */
								 NULL, /* init process */
								 NULL, /* init thread */
								 NULL, /* exit thread */
								 NULL, /* exit process */
								 NULL, /* exit master */
								 NGX_MODULE_V1_PADDING};

static ngx_event_module_t ngx_epoll_module_ctx = {
	&epoll_name,
	ngx_epoll_create_conf, /* create configuration */
	ngx_epoll_init_conf, /* init configuration */
	{
		ngx_epoll_add_event, /* add an event */
		ngx_epoll_del_event, /* delete an event */
		ngx_epoll_add_event, /* enable an event */
		ngx_epoll_del_event, /* disable an event */
		ngx_epoll_add_connection, /* add an connection */
		ngx_epoll_del_connection, /* delete an connection */
#if (NGX_HAVE_EVENTFD)
		ngx_epoll_notify, /* trigger a notify */
#else
		NULL, /* trigger a notify */
#endif
		ngx_epoll_process_events, /* process the events */
		ngx_epoll_init, /* init the events */
		ngx_epoll_done, /* done the events */
	}};

typedef struct
{
	ngx_int_t (*add)(ngx_event_t* ev, ngx_int_t event, ngx_uint_t flags);
	ngx_int_t (*del)(ngx_event_t* ev, ngx_int_t event, ngx_uint_t flags);
	ngx_int_t (*enable)(ngx_event_t* ev, ngx_int_t event, ngx_uint_t flags);
	ngx_int_t (*disable)(ngx_event_t* ev, ngx_int_t event, ngx_uint_t flags);
	ngx_int_t (*add_conn)(ngx_connection_t* c);
	ngx_int_t (*del_conn)(ngx_connection_t* c, ngx_uint_t flags);
	ngx_int_t (*notify)(ngx_event_handler_pt handler);
	ngx_int_t (*process_events)(ngx_cycle_t* cycle, ngx_msec_t timer, ngx_uint_t flags);
	/* --> this indicate ngx_epoll_process_events in this case */
	ngx_int_t (*init)(ngx_cycle_t* cycle, ngx_msec_t timer);
	void (*done)(ngx_cycle_t* cycle);
} ngx_event_actions_t;

static int ep = -1;
static struct epoll_event* event_list;

static ngx_int_t ngx_epoll_init(ngx_cycle_t* cycle, ngx_msec_t timer)
{
	if (ep == -1)
	{
		ep = epoll_create(cycle->connection_n / 2);
		ngx_event_actions = ngx_epoll_module_ctx.actions;
	}
}

static ngx_int_t ngx_epoll_add_event(ngx_event_t* ev, ngx_int_t event, ngx_int_t flag)
{
	ngx_connection_t* c = ev->data;
	epoll_ctl(ep, op, c->fd, ...);
}

static ngx_int_t ngx_epoll_process_events(ngx_cycle_t* cycle, ngx_msec_t timer, ngx_uint_t flag)
{
	ngx_connection_t* c;
	ngx_event_t *rev, wev;
	uint32_t revents;

	events = epoll_wait(ep, event_list, nevents, timer);
	ngx_err_t err = (events == -1) ? ngx_errno : 0;
	if (err)
	{
		/* error handling */
	}
	if (events == 0)
	{
		/* error handling */
	}
	for (i = 0; i < events; i++)
	{
		c = event_list[i].data.ptr;
		rev = c->read;
		revents = event_list[i].events;
		if (revents & (EPOLLERR | EPOLLHUP))
		{
			/* error handling */
		}
		if ((revents & EPOLLIN) && rev->active)
		{
			rev->ready = 1;
			rev->handler(rev);
			/* this call ngx_event_accept() */
		}
		wev = c->write;
		if ((revents & EPOLLOUT) && rev->active)
		{
			wev->ready = 1;
			wev->handler(wev);
		}
	}
}


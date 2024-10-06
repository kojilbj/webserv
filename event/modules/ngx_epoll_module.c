
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

static int ep = -1;

static ngx_int_t ngx_epoll_init(ngx_cycle_t* cycle, ngx_msec_t timer)
{
	ep = epoll_create(cycle->connection_n / 2);
	ngx_epoll_notifiy_init(cycle->log);
	ngx_event_actions = ngx_epoll_module_ctx.actions;
}

static ngx_epoll_notify_init(ngx_log_t* log)
{
	epoll_ctl(ep, EPOLL_CTL_ADD, ...);
}

static ngx_int_t ngx_epoll_process_events(ngx_cycle_t* cycle, ngx_msec_t timer, ngx_uint_t flag)
{
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
	}
}


static ngx_event_module_t ngx_event_core_module_ctx = {
	&event_core_name,
	ngx_event_core_create_conf, /* create configuration */
	ngx_event_core_init_conf, /* init configuration */

	{NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}};

ngx_module_t ngx_event_core_module = {NGX_MODULE_V1,
									  &ngx_event_core_module_ctx, /* module context */
									  ngx_event_core_commands, /* module directives */
									  NGX_EVENT_MODULE, /* module type */
									  NULL, /* init master */
									  ngx_event_module_init, /* init module */
									  ngx_event_process_init, /* init process */
									  NULL, /* init thread */
									  NULL, /* exit thread */
									  NULL, /* exit process */
									  NULL, /* exit master */
									  NGX_MODULE_V1_PADDING};

extern ngx_event_actions_t ngx_event_actions;
/* --> this variable is defined in ngx_epoll_module.c */

#define ngx_process_events ngx_event_actions.process_event

void ngx_process_events_and_timers(ngx_cycle_t* cycle)
{
	(void)ngx_process_events(cycle, timer, flag);
}

static ngx_init ngx_event_module_init() { }
/* (init_module() in ngx_init_module call this function) */

static ngx_int_t ngx_event_process_init(ngx_cycle_t* cycle)
/* (init_process() in ngx_single_process_cycle call this function) */
{
	for (i = 0; cycle->modules[i]; i++)
	{
		if (cycle->modules[i]->type != NGX_EVENT_MODULE)
			continue;
		ngx_event_module_t* module = cycle->modules[i]->ctx;
		module->actions.init(cycle, ngx_timer_resolution)
		/* this fuction call ngx_epoll_init() */
	}

	ngx_connection_t c;
	ngx_event_t rev, wev;

	cycle->connections = ngx_alloc(...);
	c = cycle->connections;
	cycle->read_events = ngx_alloc(...);
	rev = cycle->read_events;
	for (i = 0; i < cycle->connection_n; i++)
	{
		rev[i].close = 1;
	}
	cycle->write_events = ngx_alloc(...);
	wev = cycle->write_events;
	for (i = 0; i < cycle->connection_n; i++)
	{
		wev[i].close = 1;
	}
	int i = cycle->connection_n;
	next = NULL;
	do
	{
		i--;
		c[i].data = next;
		c[i].read = &cycle->read_events[i];
		c[i].write = &cycle->write_events[i];
		c[i].fd = -1;
	} while (1);
	cycle->free_connections = next;
	cycle->free_connection_n = cycle->connection_n;

	for (i = 0; i < cycle->listening.nelts; i++)
	{
		c = ngx_get_connection(listening.elts[i].fd, cycle->log);
		rev = c->read;
		/* some initialization of c and rev */
		if (c->type == SOCK_STREAM)
			rev->handler = ngx_event_accept;
		ngx_add_event(rev, NGX_READ_EVENT, 0);
		/* this fuction call ngx_epoll_add_event() */
	}
}

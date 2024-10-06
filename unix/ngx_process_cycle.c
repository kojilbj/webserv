
struct ngx_module_s
{
	ngx_uint_t ctx_index;
	ngx_uint_t index;
	char* name;
	ngx_uint_t version;
	const char* signature;
	void* ctx;
	ngx_command_t* commands;
	ngx_uint_t type;
	ngx_int_t (*init_master)(ngx_log_t* log);
	ngx_int_t (*init_module)(ngx_cycle_t* cycle);
	ngx_int_t (*init_process)(ngx_cycle_t* cycle);
	ngx_int_t (*init_thread)(ngx_cycle_t* cycle);
	void (*exit_thread)(ngx_cycle_t* cycle);
	void (*exit_process)(ngx_cycle_t* cycle);
	void (*exit_master)(ngx_cycle_t* cycle);
	/* some varialble */
};

void ngx_single_process_cycle(ngx_cycle_t* cycle)
{
	for (i = 0; cycle->modules[i]; i++)
	{
		if (cycle->modules[i]->init_process)
		{
			cycle->modules[i]->init_process(cycle);
			/* ngx_core_module == DONTHAVE */
			/* ngx_events_module == DONTHAVE */
			/* ngx_epoll_module == DONTHAVE */
			/* ngx_event_core_module == HAVE */
		}
	}
	for (;;)
	{
		ngx_process_events_and_timer(cycle);
		if (ngx_terminate || ngx_quit)
		{
			for (i = 0; cycle->modules[i]; i++)
			{
				if (cycle->modules[i]->exit_process)
				{
					cycle->modules[i]->exit_process(cycle);
					/* ngx_core_module == DONTHAVE */
					/* ngx_events_module == DONTHAVE */
					/* ngx_epoll_module == DONTHAVE */
					/* ngx_event_core_module == DONTHAVE */
				}
			}
			ngx_master_process_exit(cycle);
		}
	}
}


ngx_cycle_t* ngx_init_cycle(ngx_cycle_t* old_cycle)
{
	ngx_cycle_t* cycle;

	/* some initialization */
	ngx_uint_t n = old_cycle->listening.nelts ? old_cycle->listening.nelts : 10;
	ngx_array_init(&cycle->listening, pool, n, sizeof(ngx_listening_t));
	ngx_memzero(cycle->listening.elts, n * sizeof(ngx_listening_t));

	cycle->conf_ctx = ngx_palloc(pool, ngx_max_module * sizeof(void*));
	/* some initialization */
	ngx_cycle_modules(cycle);
	/* only this initialization is separated from others. */
	/* ##variation points## reference C++ software design p .38 */
	for (i = 0; cycle->modules[i]; i++)
	{
		if (cycle->modules[i]->type != NGX_CORE_MODULE)
			continue;
		module = cycle->modules[i]->ctx; // --> get foo_core_module_t
		rv = module->create_conf(cycle);
		/* --> allocate memory for the configuration and set default value */
		/* ngx_core_module == HAVE */
		/* ngx_events_module == DONTHAVE */
		/* ngx_epoll_module == HAVE */
		/* ngx_event_core_module == HAVE */
		cycle->conf_ctx[cycle->modules[i]->index] = rv;
	}
	ngx_conf_t conf;
	conf.ctx = cycle->conf_ctx;
	/* some conf initialization */
	ngx_conf_parse(&conf, &cycle->conf_file);
	for (i = 0; cycle->modules[i]; i++)
	{
		if (cycle->modules[i]->type != NGX_CORE_MODULE)
			continue;
		module = cycle->modules[i]->ctx;
		if (module->init_conf)
		{
			module->init_conf(cycle, cycle->conf_ctx[cycle->modules[i]->index]);
			/* ngx_core_module == HAVE */
			/* ngx_events_module == HAVE */
			/* ngx_epoll_module == HAVE */
			/* ngx_event_core_module == HAVE */
		}
	}
	ngx_listening_t* ls = cycle->listening.elts;
	ngx_open_listening_socket(cycle);
	/* --> In Webserv, just one listening socket is required ? */
	/* ( one listening socket for thousand of connections ) */
	ngx_init_module(cycle);
}

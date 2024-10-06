static char* ngx_http_block(ngx_conf_t* cf, ngx_command_t* cmd, void* conf)
{
	ngx_http_conf_ctx_t* ctx = ngx_palloc(...);
	ngx_http_max_module = ngx_count_module(...);
	for (m = 0; cf->cycle->modules[m]; m++)
	{
		if (cf->cycle->modules[m]->type != NGX_HTTP_MODULE)
			continue;
		ngx_http_module_t module = cf->cyle->modules[m]->ctx;
		module->preconfiguration(...);
	}
	ngx_conf_parse(cf, NULL);
	/* some initialization */
	/* complicated ... */
	ngx_http_optimize_servers(...);
}

static ngx_int_t
ngx_http_optimize_servers(ngx_conf_t* cf, ngx_http_core_main_conf_t* cmcf, ngx_array_t* ports)
{
	for (p = 0; p < ports->nelts; p++)
	{
		ngx_sort(...);
		ngx_http_init_listening(...);
	}
}

static ngx_int_t ngx_http_init_listening(ngx_conf_t* cf, ngx_http_conf_port_t* port)
{
	ngx_http_add_listening(...);
}

static ngx_listening_t* ngx_http_add_listening(...)
{
	ls->handler = ngx_http_init_connection;
}

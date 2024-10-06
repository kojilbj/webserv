void ngx_http_handler(ngx_http_request_t* r)
{
	r->write_event_handler = ngx_http_core_run_phase;
	ngx_http_core_run_phase(r);
}

void ngx_http_core_run_phase(ngx_http_request_t* r)
{
	ngx_http_phase_handler_t ph = cmcf->phase_engine.handlers;
	/* reference ngx_http_init_phase_handler() */
	while (ph[r->phase_handler].checker)
	{
		rc = ph[r->phase_handler].checker(...);
	}
}

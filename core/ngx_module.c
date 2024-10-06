extern ngx_module_t ngx_modules[];
/* --> this is defined in objs/ngx_modules.c created while build. */
/* reference nginx/auto/make line.527 */

/* example in my environment. */

/* ngx_module_t* ngx_modules[] = { */
/* &ngx_core_module,  -> NGX_CORE_MODULE */
/* &ngx_errlog_module,  -> NGX_CORE_MODULE */
/* &ngx_conf_module, */
/* &ngx_regex_module,  -> NGX_CORE_MODULE */
/* &ngx_events_module, */
/* &ngx_event_core_module, */
/* &ngx_epoll_module, */
/* &ngx_http_module, */
/* &ngx_http_core_module, */
/* &ngx_http_log_module, */
/* &ngx_http_upstream_module, */
/* &ngx_http_static_module, */
/* &ngx_http_autoindex_module, */
/* &ngx_http_index_module, */
/* &ngx_http_mirror_module, */
/* &ngx_http_try_files_module, */
/* &ngx_http_auth_basic_module, */
/* &ngx_http_access_module, */
/* &ngx_http_limit_conn_module, */
/* &ngx_http_limit_req_module, */
/* &ngx_http_geo_module, */
/* &ngx_http_map_module, */
/* &ngx_http_split_clients_module, */
/* &ngx_http_referer_module, */
/* &ngx_http_rewrite_module, */
/* &ngx_http_proxy_module, */
/* &ngx_http_fastcgi_module, */
/* &ngx_http_uwsgi_module, */
/* &ngx_http_scgi_module, */
/* &ngx_http_memcached_module, */
/* &ngx_http_empty_gif_module, */
/* &ngx_http_browser_module, */
/* &ngx_http_upstream_hash_module, */
/* &ngx_http_upstream_ip_hash_module, */
/* &ngx_http_upstream_least_conn_module, */
/* &ngx_http_upstream_random_module, */
/* &ngx_http_upstream_keepalive_module, */
/* &ngx_http_upstream_zone_module, */
/* &ngx_http_write_filter_module, */
/* &ngx_http_header_filter_module, */
/* &ngx_http_chunked_filter_module, */
/* &ngx_http_range_header_filter_module, */
/* &ngx_http_gzip_filter_module, */
/* &ngx_http_postpone_filter_module, */
/* &ngx_http_ssi_filter_module, */
/* &ngx_http_charset_filter_module, */
/* &ngx_http_userid_filter_module, */
/* &ngx_http_headers_filter_module, */
/* &ngx_http_copy_filter_module, */
/* &ngx_http_range_body_filter_module, */
/* &ngx_http_not_modified_filter_module, */
/* NULL */
/* }; */

ngx_int_t ngx_cycle_modules(ngx_cycle_t* cycle)
{
	cycle->modules = ngx_palloc(...);
	ngx_memcpy(cycle->modules, ngx_modules, ngx_modules_n * sizeof(ngx_module_t*));
	cycle->modules_n = ngx_modules_n;
}

ngx_int_t ngx_init_module(ngx_cycle_t* cycle)
{
	for (i = 0; cycle->modules[i]; i++)
	{
		if (cycle->modules[i]->init_module)
			cycle->modules[i]->init_module(cycle);
	}
}

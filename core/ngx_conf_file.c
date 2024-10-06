char* ngx_conf_parse(ngx_conf_t* cf, ngx_str_t* filename)
{
	int fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0);
	cf->conf_file->file.fd = fd;
	/* some initialization */
	for (;;)
	{
		ngx_int_t rc = ngx_conf_read_token(cf);
		/* ngx_conf_read_token() may return */
		/* NGX_ERROR             there is error */
		/* NGX_OK                the token terminated by ";" was found */
		/* NGX_CONF_BLOCK_START  the token terminated by "{" was found */
		/* NGX_CONF_BLOCK_DONE   the "}" was found */
		/* GX_CONF_FILE_DONE    the configuration file is done */

		if (rc == NGX_OK || rc == NGX_CONF_BLOCK_START)
		{
			if (cf->handler)
			{
				rv = (*cf->handler)(cd, NULL, cd->handler_conf);
				continue;
			}
			rc = ngx_conf_handler(cf, rc);
		}
	}
}

static ngx_int_t ngx_conf_handler(ngx_conf_t* cf, ngx_int_t last)
{
	ngx_str_t name = cf->args->elts;

	for (i = 0; cf->cycle->modules[i]; i++)
	{
		ngx_command_t* cmd = cd->cycle->modules[i]->commands;
		if (!cmd)
			continue;
		for (; cmd->name.len; cmd++)
		{
			if (ngx_strcmp(name->data, cmd->name.data) != 0)
				continue;
			/* some parse code */
			rv = cf->set(cf, cmd, conf);
			/* this call ngx_http_block() in case of http module */
		}
	}
}

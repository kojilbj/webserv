int main()
{
	ngx_cycle_t cycle, init_cycle;

	ngx_memzero(&init_cycle, sizeof(ngx_cycle_t));
	// some initialization of init_cycle

	cycle = ngx_init_cycle(&init_cycle);
	ngx_init_signals(cycle->log);
	ngx_single_process_cycle(cycle);
}

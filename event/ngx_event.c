
extern ngx_event_actions_t ngx_event_actions;
/* --> this variable is defined in ngx_epoll_module.c */

#define ngx_process_events ngx_event_actions.process_event

void ngx_process_events_and_timers(ngx_cycle_t* cycle)
{
	(void)ngx_process_events(cycle, timer, flag);
}

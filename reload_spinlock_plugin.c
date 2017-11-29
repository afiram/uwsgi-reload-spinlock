#include <uwsgi.h>

extern struct uwsgi_server uwsgi;

void wait_thread_busy_loop(){
	int is_busy;
	for(;;){
		is_busy = uwsgi_worker_is_busy(uwsgi.mywid);
		uwsgi_log("wait_busy, worker:%d (pid: %d) busy:%d ..\n",
				   uwsgi.mywid, uwsgi.mypid,is_busy);
		if (! is_busy){
			break;
		}
		sleep(1);
	}
	return;
}

void reload_spinlock_hook(){
	// uwsgi_log("== gbcw_hook() ==\n");
	uwsgi.workers[uwsgi.mywid].manage_next_request = 0;
	if (uwsgi.threads > 1 && !uwsgi_instance_is_dying) {
		// wait_for_threads();
		wait_thread_busy_loop();
	}
	uwsgi_log("...The work of process %d is done. Seeya!\n", getpid());
	exit(0);
}	

static void on_load_reload_spinlock(){
	uwsgi_log("reload-spainlock plugin initialization \n");
	uwsgi.gbcw_hook = reload_spinlock_hook;
}

struct uwsgi_plugin reload_spinlock_plugin = {
        .name = "reload-spinlock",
		.on_load = on_load_reload_spinlock,
};


/*
void goodbye_cruel_world() {
	uwsgi_curse(uwsgi.mywid, 0);
	if (!uwsgi.gbcw_hook) {
		simple_goodbye_cruel_world();
	}
	else {
		uwsgi.gbcw_hook();
	}
}
void simple_goodbye_cruel_world() {
	if (uwsgi.threads > 1 && !uwsgi_instance_is_dying) {
		wait_for_threads();
	}
	uwsgi.workers[uwsgi.mywid].manage_next_request = 0;
	uwsgi_log("...The work of process %d is done. Seeya!\n", getpid());
	exit(0);
}
*/

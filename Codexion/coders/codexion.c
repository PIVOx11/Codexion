#include "codexion.h"


void    ft_error(char *err)
{
	fprintf(stderr, "ERROR: %s\n", err);
	exit(2);
}


int     main(int ac, char **av)
{
    long time = ft_gettime();
    t_data      data;

    if (ac != 9)
        ft_error("All argument are mandatory");
    
    parse_arg(&data, av);

    
    fprintf(stdout, "number of coder: %d\n", data.coder_count);
    fprintf(stdout, "bornout_time: %d\n", data.bornout_time);
    fprintf(stdout, "compile_time: %d\n", data.compile_time);
    fprintf(stdout, "debug_time : %d\n", data.debug_time);
    fprintf(stdout, "refactor_time: %d\n", data.refactor_time);
    fprintf(stdout, "compile_req: %d\n", data.compile_req);
    fprintf(stdout, "cold_down_time: %d\n", data.cold_down_time);
    fprintf(stdout, "scheduler: %s\n", data.scheduler);
    printf("%ld", task_time(time));
}

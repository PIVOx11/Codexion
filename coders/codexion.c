/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:55:58 by blidriss          #+#    #+#             */
/*   Updated: 2026/05/15 20:25:57 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void    ft_error(char *err)
{
	fprintf(stderr, "ERROR: %s\n", err);
	exit(2);
}

void    change(t_data *data)
{
    int i = -1;

    while (++i < data->coder_count)
        data->coders[i].id = 5;
}
int     main(int ac, char **av)
{
    t_data      data;

    if (ac != 9)
        ft_error("All argument are mandatory");
    parse_arg(&data, av);
    full_init(&data);
    // init_threads(&data);
    // free_all(&data); // if you want to free befor you leave :)

    int i = -1;

    change(&data);
    while(++i < data.coder_count)
    {
        printf("codeer id: %d, coder compile: %d\n", 
        data.coders[i].id,
        data.coders[i].compile_count);
    }
}





// printing the data was parssed :)

//     fprintf(stdout, "number of coder: %d\n", data.coder_count);
//     fprintf(stdout, "bornout_time: %d\n", data.bornout_time);
//     fprintf(stdout, "compile_time: %d\n", data.compile_time);
//     fprintf(stdout, "debug_time : %d\n", data.debug_time);
//     fprintf(stdout, "refactor_time: %d\n", data.refactor_time);
//     fprintf(stdout, "compile_req: %d\n", data.compile_req);
//     fprintf(stdout, "cold_down_time: %d\n", data.cold_down_time);
//     fprintf(stdout, "scheduler: %s\n", data.scheduler);
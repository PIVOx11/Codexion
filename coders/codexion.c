/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:55:58 by blidriss          #+#    #+#             */
/*   Updated: 2026/05/19 15:46:13 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int     main(int ac, char **av)
{
    t_data      data;
    
    if (ac != 9)
        return 1;
    if (!parse_arg(&data, av))
        return 1;
    if (!full_init(&data))
        return(free_all(&data), 1);
    start_semulation(&data);
    free_all(&data);

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
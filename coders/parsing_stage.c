/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_stage.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 16:21:30 by blidriss          #+#    #+#             */
/*   Updated: 2026/05/18 09:20:04 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void set_arg(char **arg, t_data *data)
{
    data->coder_count = atoi(arg[1]);
    data->bornout_time = atoi(arg[2]);
    data->compile_time = atoi(arg[3]);
    data->debug_time = atoi(arg[4]);
    data->refactor_time = atoi(arg[5]);
    data->compile_req = atoi(arg[6]);
    data->cold_down_time = atoi(arg[7]);
}

void parse_arg(t_data *data, char **arg)
{
    int     i;
    int     j;

    i = 1;
    while(i < 8)
    {
        j = 0;
        if (arg[i][j] == '\0')
            ft_error("argument most valid not emtpy");
        while(arg[i][j])
        {
            if (arg[i][j] < '0' || arg[i][j] > '9')
                if (arg[i][j] != '+')
                    ft_error("Argument number invalid");
            j++;
        }
        if (atoi(arg[i]) == 0)
            ft_error("Argument number most be greet then 0");
        i++;
    }
    if(strcmp(arg[i], "fifo") != 0 && strcmp(arg[i], "edf") != 0)
        ft_error("scheduler argument most be 'fifo' or 'edf'");
    data -> scheduler = arg[i];
    set_arg(arg, data);
}


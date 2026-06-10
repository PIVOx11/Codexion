/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_stage.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 16:21:30 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/10 22:58:36 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_arg(char **arg, t_data *data)
{
	data->coder_count = atoi(arg[1]);
	data->bornout_time = atoi(arg[2]);
	data->compile_time = atoi(arg[3]);
	data->debug_time = atoi(arg[4]);
	data->refactor_time = atoi(arg[5]);
	data->compile_req = atoi(arg[6]);
	data->cold_down_time = atoi(arg[7]);
}

static int	costum_atoi(char *s)
{
	int		i;
	long	nb;

	if (!s || !*s)
		return (0);
	nb = 0;
	i = -1;
	while (s[++i])
	{
		if (s[i] < '0' || s[i] > '9')
		{
			if (i == 0 && s[i] == '+')
				continue ;
			else
				return (0);
		}
		nb = nb * 10 + s[i] - 48;
		if (nb > INT_MAX)
			return (0);
	}
	return ((int )nb);
}

int	parse_arg(t_data *data, char **arg)
{
	int		i;

	i = 0;
	while (++i < 8)
	{
		if (!costum_atoi(arg[i]))
			return (FALSE);
	}
	if (strcmp(arg[i], "fifo") == 0)
		data->scheduler = FIFO;
	else if (strcmp(arg[i], "edf") == 0)
		data->scheduler = EDF;
	else
		return (FALSE);
	set_arg(arg, data);
	return (TRUE);
}

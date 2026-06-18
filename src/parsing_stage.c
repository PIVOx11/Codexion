/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_stage.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 16:21:30 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/18 10:18:26 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	set_arg(char **arg, t_data *data)
{
	data->coder_c = atoi(arg[1]);
	if (data->coder_c == 0)
		return (FALSE);
	data->bornout_t = atoi(arg[2]);
	data->compile_t = atoi(arg[3]);
	data->debug_t = atoi(arg[4]);
	data->refactor_t = atoi(arg[5]);
	data->compile_r = atoi(arg[6]);
	if (data->compile_r == 0)
		return (FALSE);
	data->cold_down_t = atoi(arg[7]);
	return (TRUE);
}

static int	is_valid_number(char *s)
{
	int		i;
	long	nb;

	if (!s || !*s)
		return (FALSE);
	nb = 0;
	i = 0;
	if (s[i] == '+' && s[i + 1])
		i++;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (FALSE);
		nb = nb * 10 + s[i] - 48;
		if (nb > INT_MAX)
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	parse_arg(t_data *data, char **arg)
{
	int		i;
	int		status;

	i = 0;
	while (++i < 8)
	{
		if (!is_valid_number(arg[i]))
			return (FALSE);
	}
	if (strcmp(arg[i], "fifo") == 0)
		data->scheduler = FIFO;
	else if (strcmp(arg[i], "edf") == 0)
		data->scheduler = EDF;
	else
		return (FALSE);
	status = set_arg(arg, data);
	return (status);
}

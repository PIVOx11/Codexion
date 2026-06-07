/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 09:36:55 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/07 14:43:09 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 9)
		return (1);
	if (!parse_arg(&data, av))
		return (1);
	if (!full_init(&data))
		return 1;
	start_semulation(&data);
	clean_resources(&data);
}
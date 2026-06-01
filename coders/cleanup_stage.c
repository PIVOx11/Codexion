/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_stage.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 09:37:21 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/01 09:37:22 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

void	free_all(t_data *data)
{
	if (data->coders)
		free(data->coders);
	if (data->dongles)
		free(data->dongles);
}

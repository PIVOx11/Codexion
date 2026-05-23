/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_stage.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:56:00 by blidriss          #+#    #+#             */
/*   Updated: 2026/05/23 10:48:56 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void free_all(t_data *data)
{
    if (data->coders)
        free(data->coders);
    if (data->dongles)
        free(data->dongles);
}


// TO DO
// void    mutex_cond_deestroy(t_coder *coder)
// {
//     // TO_DO
// }
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blidriss <blidriss@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 17:56:07 by blidriss          #+#    #+#             */
/*   Updated: 2026/06/13 14:46:26 by blidriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <string.h>
# include <stdio.h>
# include <pthread.h>
# include <time.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>

# define FALSE 0
# define TRUE 1
# define EDF 2
# define FIFO 3

typedef pthread_mutex_t	t_mutex;
typedef pthread_cond_t	t_cond;
typedef struct s_data	t_data;
typedef struct s_coder	t_coder;

/*
_t = time
_c = count
_s = size
_r = required
*/
typedef struct s_request
{
	t_coder	*coder;
	long	request_t;
	long	dead_line;

}	t_request;

typedef struct s_dongle
{
	t_request	heap[2];
	t_mutex		d_data;
	t_cond		dongle_cond;
	long		relais_t;
	int			is_taken;
	int			id;
	t_data		*data;
	int			heap_s;

}	t_dongle;

typedef struct s_coder
{
	int			id;
	int			compile_c;
	long		compile_start_t;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	pthread_t	thread_id;
	t_mutex		coder_mutex;
	int			finish;
	t_data		*data;

}	t_coder;

struct s_data
{
	int			coder_c;
	int			bornout_t;
	int			compile_t;
	int			debug_t;
	int			refactor_t;
	int			compile_r;
	int			cold_down_t;
	int			scheduler;
	int			semulation_over;
	long		semulation_start;
	int			compiles;
	pthread_t	monitor;
	t_mutex		stop;
	t_mutex		data_mutex;
	t_mutex		print_mutex;
	t_dongle	*dongles;
	t_coder		*coders;
};

// parsing stage
int		parse_arg(t_data *data, char **arg);
void	set_arg(char **arg, t_data *data);

// time functions
long	task_time(long time);
long	ft_gettime(void);

// init stage
int		full_init(t_data *data);

/*clean up functons*/
void	malloc_clean(t_data *data);
void	coder_mutex_destroy(t_coder *coder, int counter);
void	dongle_destroy(t_dongle *dongles, int counter);
void	clean_data(t_data *data, int counter);
void	clean_resources(t_data *data);

// simulation functions
int		start_semulation(t_data *data);
void	*semulation(void *co);

// Thread Actions
void	compile(t_coder *coder, t_dongle *first, t_dongle *second);
void	debug(t_coder *coder);
void	refactol(t_coder *coder);

// monitor
void	*monitor(void *d);

// seters and geters
int		get_bool(t_mutex *mtx, int *value);
int		set_bool(t_mutex *mtx, int *dest, int value);
long	get_time(t_mutex *mtx, long *time);
int		set_time(t_mutex *mtx, long *dest, long value);

// safety functions
void	safe_print(char *str, t_coder *coder);
void	safe_sleep(t_coder *coder, int time);
void	print_burnout(t_coder *coder);

// scheduler
void	mutex_lock(t_coder *coder);
void	remove_request(t_coder *coder, t_dongle *dongle);
void	add_request(t_coder *coder, t_dongle *f, t_dongle *s);
void	fill_request(t_coder *coder, t_dongle *first, t_dongle *second);
int		try_take_dongle(t_dongle *dongle, t_coder *coder);
void	wait_dongle(t_coder *coder, t_dongle *dongle);
void	dongle_order(t_coder *coder, t_dongle **f, t_dongle **s);
void	preorety(t_dongle *dongle);

#endif
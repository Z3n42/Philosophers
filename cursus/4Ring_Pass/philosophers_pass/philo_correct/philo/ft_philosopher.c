/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosopher.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ingonzal <ingonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/16 14:08:07 by ingonzal          #+#    #+#             */
/*   Updated: 2022/02/01 19:57:11 by ingonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ft_philo.h"

static int	ft_integer(const char *str)
{
	int	integer;

	integer = 0;
	while (*str > 47 && *str < 58)
	{
		integer = (integer * 10) + (*str - '0');
		str++;
	}
	return (integer);
}

int	ft_atoi(const char *str)
{
	int	minus;
	int	plus;
	int	integer;

	while (*str == ' ' || (*str > 8 && *str < 14))
		str++;
	minus = 0;
	plus = 0;
	while (*str == '-' || *str == '+')
	{
		if (*str == '-')
			minus++;
		plus++;
		str++;
	}
	if (minus > 1 || plus > 1)
		return (0);
	integer = ft_integer(str);
	if ((minus % 2) != 0)
		integer = integer * (-1);
	return (integer);
}

void	ft_create(t_ph *ph)
{
	int				i;
	pthread_t		*thread;

	i = -1;
	while (++i < ph->num)
		pthread_mutex_init(&ph->mutex[i], NULL);
	thread = malloc(ph->num * sizeof(pthread_t));
	i = -1;
	while (++i < ph->num)
	{
		pthread_mutex_lock(&ph->mutex[0]);
		ph->id = i + 1;
		pthread_create(&thread[i], NULL, ft_routine, ph);
	}
	i = -1;
	while (++i < ph->num)
		pthread_join(thread[i], NULL);
	i = -1;
	while (++i < ph->num)
		pthread_mutex_destroy(&ph->mutex[i]);
	free(ph->mutex);
	free(thread);
	free(ph->stat);
}

void	ft_init(int argc, char **argv)
{
	t_ph	ph;

	ph.num = ft_atoi(argv[1]);
	ph.mutex = (pthread_mutex_t *)malloc(ph.num * sizeof(pthread_mutex_t));
	ph.stat = malloc(1 * sizeof(int));
	memset(ph.stat, 0, 4);
	ph.max = -1;
	if (argc == 6)
		ph.max = ft_atoi(argv[5]);
	ph.life = ft_atoi(argv[2]);
	ph.blood = ph.life;
	ph.eat = ft_atoi(argv[3]);
	ph.sleep = ft_atoi(argv[4]);
	ph.full = 0;
	ph.kill = 0;
	ph.wait = 0;
	ph.print = 0;
	ph.times = 0;
	if (argc < 5 || ph.num < 1 || ph.life < 1 || ph.eat < 1 || ph.sleep < 1)
		return ;
	if (!ph.num || !ph.life || !ph.eat || !ph.sleep)
		return ;
	ft_create(&ph);
}

int	main(int argc, char **argv )
{
	if (argc >= 5 && argc <= 7)
		ft_init(argc, argv);
	else
		return (0);
}

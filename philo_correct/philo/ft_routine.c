/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ingonzal <ingonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/16 14:08:07 by ingonzal          #+#    #+#             */
/*   Updated: 2022/02/01 20:44:21 by ingonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "ft_philo.h"

void	ft_maxtimes(t_ph *ph)
{
	static int	max;

	if (ph->full == 0)
	{
		max += 1;
		ph->full = 1;
		if (max == ph->num)
			ph->stat[0] = 1;
	}
}

void	ft_wait(t_ph *ph)
{
	static int	door;

	ph->left = ph->id - 2;
	ph->right = ph->id - 1;
	if (ph->id == 1)
		ph->left = ph->num - 1;
	door += 1;
	while (door != ph->num)
	{
		if (door != ph->num)
			usleep(50);
	}
}

void	*ft_routine(void *tid)
{
	t_ph			ph;
	struct timeval	live;

	ph = *(t_ph *)tid;
	pthread_mutex_unlock(&ph.mutex[0]);
	ft_wait(&ph);
	gettimeofday(&live, NULL);
	ph.life = (live.tv_sec * 1000) + (live.tv_usec / 1000);
	ph.die = (live.tv_sec * 1000) + (live.tv_usec / 1000) + ph.blood;
	ph.born = ph.life;
	while (ph.stat[0] == 0)
	{
		if ((ph.id % 2) == 0 && ph.wait == 0)
		{
			ph.wait = 1;
			usleep(1500);
		}
		ft_fk1(&ph);
		if (ph.stat[0] == 1)
			break ;
	}
	return (0);
}

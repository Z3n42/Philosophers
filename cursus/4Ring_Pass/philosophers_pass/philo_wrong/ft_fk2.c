/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fk2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ingonzal <ingonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/16 14:08:07 by ingonzal          #+#    #+#             */
/*   Updated: 2022/02/01 14:12:57 by ingonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "ft_philo.h"

void	ft_fkl2a(t_ph *ph, struct timeval take2)
{	
	ph->fk[ph->id - 2] = 0;
	gettimeofday(&take2, NULL);
	ph->life = (take2.tv_sec * 1000) + (take2.tv_usec / 1000);
	if ((ph->die - ph->life) < 0)
		ft_die(ph);
	if (ph->stat[0] == 0)
		printf("%ld %d has taken a L1 - %dfork\n", (ph->life - ph->born), ph->id, ph->id - 2);
	ft_eat(ph);
}

void	ft_fkl2b(t_ph *ph, struct timeval take2)
{
	usleep(1000 + take2.tv_usec / 10000);
	gettimeofday(&take2, NULL);
	ph->life = (take2.tv_sec * 1000) + (take2.tv_usec / 1000);
	if ((ph->die - ph->life) < 0)
		ft_die(ph);
	if (ph->fk[ph->id - 2] == -1)
	{
		ph->fk[ph->id - 2] = 0;
		pthread_mutex_unlock(&ph->mutex[ph->id - 2]);
		gettimeofday(&take2, NULL);
		ph->life = (take2.tv_sec * 1000) + (take2.tv_usec / 1000);
		if (ph->stat[0] == 0)
			printf("%ld %d has taken a L2 - %dfork\n", (ph->life - ph->born), ph->id, ph->id - 2);
		ft_eat(ph);
	}
	else
	{
		ph->fk[ph->id - 1] = -1;
		/* printf("Free R - %d fork\n", ph->id - 1); */
		pthread_mutex_unlock(&ph->mutex[ph->id - 2]);
		pthread_mutex_unlock(&ph->mutex[ph->id - 1]);
		/* if (ph->num % 2 != 0) */
			usleep(140);
		ft_fk2(ph);
	}
}

void	ft_fk2(t_ph *ph)
{
	struct timeval	take2;

	pthread_mutex_lock(&ph->mutex[ph->id - 1]);
	if (ph->fk[ph->id - 1] == -1 && ph->stat[0] == 0)
	{
		ph->fk[ph->id - 1] = 0;
		gettimeofday(&take2, NULL);
		ph->life = (take2.tv_sec * 1000) + (take2.tv_usec / 1000);
		if ((ph->die - ph->life) < 0)
			ft_die(ph);
		if (ph->print == 0 && ph->stat[0] == 0)
		{
			printf("%ld %d has taken a R - %dfork\n", (ph->life - ph->born), ph->id, ph->id - 1);
			ph->print = 1;
		}
		pthread_mutex_lock(&ph->mutex[ph->id - 2]);
		if (ph->fk[ph->id - 2] == -1)
			ft_fkl2a(ph, take2);
		else
			ft_fkl2b(ph, take2);
	}
}

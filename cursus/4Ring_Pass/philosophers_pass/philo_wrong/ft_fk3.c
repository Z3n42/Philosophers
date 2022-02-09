/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fk3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ingonzal <ingonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 17:24:55 by ingonzal          #+#    #+#             */
/*   Updated: 2022/01/20 16:17:02 by ingonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "ft_philo.h"

void	ft_fkl2a(t_ph *ph, struct timeval take2)
{	
	pthread_mutex_lock(&ph->mutex[ph->id - 2]);
	/* ph->fk[ph->id - 2] = 0; */
	/* pthread_mutex_unlock(&ph->mutex[ph->id - 2]); */
	gettimeofday(&take2, NULL);
	ph->life = (take2.tv_sec * 1000) + (take2.tv_usec / 1000);
	if ((ph->die - ph->life) < 0)
		ft_die(ph);
	if (ph->stat[0] == 0)
		printf("%ld %d has taken a fork\n", (ph->die - ph->life), ph->id);
	ft_eat(ph);
}



void	ft_fk2(t_ph *ph)
{
	struct timeval	take2;

	pthread_mutex_lock(&ph->mutex[ph->id - 1]);
	/* ph->fk[ph->id - 1] = 0; */
	/* pthread_mutex_unlock(&ph->mutex[ph->id - 1]); */
	gettimeofday(&take2, NULL);
	ph->life = (take2.tv_sec * 1000) + (take2.tv_usec / 1000);
	if ((ph->die - ph->life) < 0)
		ft_die(ph);
	if (ph->print == 0 && ph->stat[0] == 0)
	{
		printf("%ld %d has taken a fork\n", (ph->die - ph->life), ph->id);
		ph->print = 1;
	}

	/* if (ph->fk[ph->id - 2] == -1) */
		ft_fkl2a(ph, take2);
	/* else */
		/* ft_fkl2b(ph, take2); */
	
}

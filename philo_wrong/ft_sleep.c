/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sleep.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ingonzal <ingonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/16 14:08:07 by ingonzal          #+#    #+#             */
/*   Updated: 2022/02/01 13:44:12 by ingonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "ft_philo.h"

void	ft_die(t_ph *ph)
{
	struct timeval	die;
	pthread_mutex_t mutex;

	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_lock(&mutex);
	if (ph->kill == 0)
	{
		gettimeofday(&die, NULL);
		ph->life = (die.tv_sec * 1000) + (die.tv_usec / 1000);
		if (ph->stat[0] == 0)
			printf("%ld %d died\n", (ph->life - ph->born), ph->id);
		ph->stat[0] = 1;
		ph->kill = 1;
	}
	pthread_mutex_unlock(&mutex);
	pthread_mutex_destroy(&mutex);
}

void	ft_sleeptime(t_ph *ph)
{
	long			sleeptime;
	struct timeval	sleept;

	/* sleeptime = 0; */
	gettimeofday(&sleept, NULL);
	ph->life = (sleept.tv_sec * 1000) + (sleept.tv_usec / 1000);
	sleeptime = (sleept.tv_sec * 1000) + (sleept.tv_usec / 1000) + ph->sleep;
	while (ph->life < sleeptime)
	{
		gettimeofday(&sleept, NULL);
		ph->life = (sleept.tv_sec * 1000) + (sleept.tv_usec / 1000);
		if ((ph->die - ph->life) < 0)
			ft_die(ph);
		usleep(250);
	}
}

void	ft_sleep(t_ph *ph)
{
	struct timeval	sleep;

	if (ph->kill == 0 && ph->stat[0] == 0)
	{
		if (ph->stat[0] == 0)
			printf("%ld %d is sleeping\n", (ph->life - ph->born), ph->id);
		if (ph->num % 2 != 0)
			usleep(600);
		ft_sleeptime(ph);
		gettimeofday(&sleep, NULL);
		ph->life = (sleep.tv_sec * 1000) + (sleep.tv_usec / 1000);
		if ((ph->die - ph->life) < 0)
			ft_die(ph);
		if (ph->stat[0] == 0)
			printf("%ld %d is thinking\n", (ph->life - ph->born), ph->id);
		if (ph->num % 2 != 0)
			usleep(600);
	}
	gettimeofday(&sleep, NULL);
	ph->life = (sleep.tv_sec * 1000) + (sleep.tv_usec / 1000);
	if ((ph->die - ph->life) < 0)
		ft_die(ph);
}

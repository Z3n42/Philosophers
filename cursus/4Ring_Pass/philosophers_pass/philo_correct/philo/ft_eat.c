/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_eat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ingonzal <ingonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/16 14:08:07 by ingonzal          #+#    #+#             */
/*   Updated: 2022/02/06 14:01:51 by ingonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "ft_philo.h"

void	ft_die(t_ph *ph)
{
	struct timeval	die;
	pthread_mutex_t	mutex;

	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_lock(&mutex);
	if (ph->kill == 0 && ph->stat[0] == 0)
	{
		ph->stat[0] = 1;
		ph->kill = 1;
		gettimeofday(&die, NULL);
		ph->life = (die.tv_sec * 1000) + (die.tv_usec / 1000);
		printf("%ld %d died\n", (ph->life - ph->born), ph->id);
		pthread_mutex_unlock(&mutex);
	}
	pthread_mutex_destroy(&mutex);
}

void	ft_sleep(t_ph *ph)
{
	struct timeval	sleep;
	long			sleeptime;

	if (ph->kill == 0 && ph->stat[0] == 0)
	{
		printf("%ld %d is sleeping\n", (ph->life - ph->born), ph->id);
		sleeptime = 0;
		gettimeofday(&sleep, NULL);
		ph->life = (sleep.tv_sec * 1000) + (sleep.tv_usec / 1000);
		sleeptime = (sleep.tv_sec * 1000) + (sleep.tv_usec / 1000) + ph->sleep;
		while (ph->life < sleeptime)
		{
			gettimeofday(&sleep, NULL);
			ph->life = (sleep.tv_sec * 1000) + (sleep.tv_usec / 1000);
			if ((ph->die - ph->life) < 0)
				ft_die(ph);
			usleep(100);
		}
		gettimeofday(&sleep, NULL);
		ph->life = (sleep.tv_sec * 1000) + (sleep.tv_usec / 1000);
		if ((ph->die - ph->life) < 0)
			ft_die(ph);
		if (ph->stat[0] == 0)
			printf("%ld %d is thinking\n", (ph->life - ph->born), ph->id);
	}
}

void	ft_eatime(t_ph *ph)
{
	long			eatime;
	struct timeval	eatt;

	eatime = 0;
	gettimeofday(&eatt, NULL);
	eatime = (eatt.tv_sec * 1000) + (eatt.tv_usec / 1000) + ph->eat;
	while (ph->life < eatime)
	{
		gettimeofday(&eatt, NULL);
		ph->life = (eatt.tv_sec * 1000) + (eatt.tv_usec / 1000);
		if ((ph->die - ph->life) < 0)
			ft_die(ph);
		usleep(100);
	}
}

void	ft_eat(t_ph *ph)
{
	struct timeval	eat;

	if (ph->kill == 0 && ph->stat[0] == 0)
	{
		gettimeofday(&eat, NULL);
		ph->life = (eat.tv_sec * 1000) + (eat.tv_usec / 1000);
		if ((ph->die - ph->life) < 0)
			ft_die(ph);
		if (ph->kill == 0 && ph->stat[0] == 0)
		{
			printf("%ld %d is eating\n", (ph->life - ph->born), ph->id);
			if (ph->max != -1)
				ph->times += 1;
			if (ph->max != -1 && ph->times == ph->max)
				ft_maxtimes(ph);
		}
		ft_eatime(ph);
		gettimeofday(&eat, NULL);
		ph->life = (eat.tv_sec * 1000) + (eat.tv_usec / 1000);
		ph->die = (eat.tv_sec * 1000) + (eat.tv_usec / 1000) + ph->blood;
		ph->print = 0;
	}
	pthread_mutex_unlock(&ph->mutex[ph->right]);
	pthread_mutex_unlock(&ph->mutex[ph->left]);
	ft_sleep(ph);
}

void	ft_fk1(t_ph *ph)
{
	struct timeval	take;

	pthread_mutex_lock(&ph->mutex[ph->right]);
	gettimeofday(&take, NULL);
	ph->life = (take.tv_sec * 1000) + (take.tv_usec / 1000);
	if (ph->print == 0 && ph->stat[0] == 0)
	{
		printf("%ld %d has taken a fork\n", (ph->life - ph->born), ph->id);
		ph->print = 1;
	}
	if (ph->num == 1)
	{
		usleep(ph->blood * 1003);
		pthread_mutex_unlock(&ph->mutex[ph->right]);
	}
	gettimeofday(&take, NULL);
	ph->life = (take.tv_sec * 1000) + (take.tv_usec / 1000);
	if ((ph->die - ph->life) < 0)
		ft_die(ph);
	pthread_mutex_lock(&ph->mutex[ph->left]);
	gettimeofday(&take, NULL);
	ph->life = (take.tv_sec * 1000) + (take.tv_usec / 1000);
	if (ph->stat[0] == 0)
		printf("%ld %d has taken a fork\n", (ph->life - ph->born), ph->id);
	ft_eat(ph);
}

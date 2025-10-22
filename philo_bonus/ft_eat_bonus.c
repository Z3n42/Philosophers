/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_eat_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ingonzal <ingonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/16 14:08:07 by ingonzal          #+#    #+#             */
/*   Updated: 2022/02/06 13:39:06 by ingonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/time.h>
#include "ft_philo_bonus.h"

void	ft_die(t_ph *ph)
{
	struct timeval	die;

	sem_wait(ph->semd);
	gettimeofday(&die, NULL);
	ph->life = (die.tv_sec * 1000) + (die.tv_usec / 1000);
	printf("%ld %d died\n", (ph->life - ph->born), ph->id);
	exit(1);
}

void	ft_sleep(t_ph *ph)
{
	struct timeval	sleep;
	long			sleeptime;

	sem_wait(ph->semd);
	printf("%ld %d is sleeping\n", (ph->life - ph->born), ph->id);
	sem_post(ph->semd);
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
	sem_wait(ph->semd);
	printf("%ld %d is thinking\n", (ph->life - ph->born), ph->id);
	sem_post(ph->semd);
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

	gettimeofday(&eat, NULL);
	ph->life = (eat.tv_sec * 1000) + (eat.tv_usec / 1000);
	if ((ph->die - ph->life) < 0)
		ft_die(ph);
	sem_wait(ph->semd);
	printf("%ld %d is eating\n", (ph->life - ph->born), ph->id);
	sem_post(ph->semd);
	if (ph->max != -1)
		ph->times += 1;
	if (ph->max != -1 && ph->times == ph->max)
		ft_maxtimes(ph);
	ft_eatime(ph);
	gettimeofday(&eat, NULL);
	ph->life = (eat.tv_sec * 1000) + (eat.tv_usec / 1000);
	ph->die = (eat.tv_sec * 1000) + (eat.tv_usec / 1000) + ph->blood;
	if ((ph->die - ph->life) < 0)
		ft_die(ph);
	sem_post(ph->sem);
	sem_post(ph->sem);
	ft_sleep(ph);
}

void	ft_fk1(t_ph *ph)
{
	struct timeval	take;

	sem_wait(ph->sem);
	gettimeofday(&take, NULL);
	ph->life = (take.tv_sec * 1000) + (take.tv_usec / 1000);
	sem_wait(ph->semd);
	printf("%ld %d has taken a fork\n", (ph->life - ph->born), ph->id);
	sem_post(ph->semd);
	if (ph->num == 1)
	{
		usleep(ph->blood * 1000);
		ft_die(ph);
	}
	gettimeofday(&take, NULL);
	ph->life = (take.tv_sec * 1000) + (take.tv_usec / 1000);
	if ((ph->die - ph->life) < 0)
		ft_die(ph);
	sem_wait(ph->sem);
	gettimeofday(&take, NULL);
	ph->life = (take.tv_sec * 1000) + (take.tv_usec / 1000);
	sem_wait(ph->semd);
	printf("%ld %d has taken a fork\n", (ph->life - ph->born), ph->id);
	sem_post(ph->semd);
	ft_eat(ph);
}

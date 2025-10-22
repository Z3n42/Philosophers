/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philosopher_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ingonzal <ingonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/16 14:08:07 by ingonzal          #+#    #+#             */
/*   Updated: 2022/02/05 14:29:25 by ingonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include "ft_philo_bonus.h"

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
	sem_unlink("forks");
	sem_unlink("wait");
	sem_unlink("die");
	ph->semd = sem_open("die", O_CREAT, 0644, 1);
	ph->semw = sem_open("wait", O_CREAT, 0644, 0);
	ph->sem = sem_open("forks", O_CREAT, 0644, ph->num);
	ft_alive(ph);
	sem_close(ph->sem);
	sem_close(ph->semw);
	sem_close(ph->semd);
	sem_unlink("die");
	sem_unlink("wait");
	sem_unlink("forks");
	free(ph->child);
}

void	ft_init(int argc, char **argv)
{
	t_ph	ph;

	ph.num = ft_atoi(argv[1]);
	ph.child = malloc(ph.num * sizeof(int));
	ph.max = -1;
	ph.status = 0;
	if (argc == 6)
		ph.max = ft_atoi(argv[5]);
	ph.life = ft_atoi(argv[2]);
	ph.blood = ph.life;
	ph.eat = ft_atoi(argv[3]);
	ph.sleep = ft_atoi(argv[4]);
	ph.wait = 0;
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

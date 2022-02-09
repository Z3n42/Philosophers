/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo_bonus.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ingonzal <ingonzal@student.42urduli>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/13 13:50:41 by ingonzal          #+#    #+#             */
/*   Updated: 2022/02/05 14:34:11 by ingonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PHILO_BONUS_H
# define FT_PHILO_BONUS_H

typedef struct s_ph{
	int				id;
	int				num;
	long			die;
	long			life;
	long			born;
	int				blood;
	int				eat;
	int				sleep;
	int				wait;
	int				status;
	int				times;
	int				full;
	int				max;
	int				*child;
	sem_t			*sem;
	sem_t			*semw;
	sem_t			*semd;
}	t_ph;

void	ft_alive(t_ph *ph);
void	ft_kill(t_ph *ph);
int		ft_atoi(const char *str);
void	ft_maxtimes(t_ph *ph);
void	ft_die(t_ph *ph);
void	ft_sleep(t_ph *ph);
void	ft_eat(t_ph *ph);
void	ft_eatime(t_ph *ph);
void	ft_fk1(t_ph *ph);
void	*ft_routine(t_ph *ph);
void	ft_create(t_ph *ph);
void	ft_init(int argc, char **argv);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palaca <palaca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 18:17:20 by palaca            #+#    #+#             */
/*   Updated: 2025/08/02 20:28:53 by palaca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher_lifecycle(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->num_philo == 1)
		return (try_to_live_alone(philo), NULL);
	// Staggered start - her filozof farklı zamanda başlasın
	if (philo->id % 2 == 0)
		smart_sleep(philo->data->time_to_eat / 2, philo->data);
	while (1)
	{
		if (is_simulation_over(philo->data))
			break ;
		eat(philo);
		if (is_simulation_over(philo->data))
			break ;
		sleep_and_think(philo);
	}
	return (NULL);
}

void	eat(t_philo *philo)
{
	if (is_simulation_over(philo->data))
		return ;
	take_forks(philo);
	if (is_simulation_over(philo->data))
	{
		put_down_forks(philo);
		return ;
	}
	perform_eating(philo);
	if (is_simulation_over(philo->data))
	{
		put_down_forks(philo);
		return ;
	}
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	put_down_forks(philo);
}

void	sleep_and_think(t_philo *philo)
{
	if (is_simulation_over(philo->data))
		return ;
	print_action(philo, "is sleeping");
	smart_sleep(philo->data->time_to_sleep, philo->data);
	if (is_simulation_over(philo->data))
		return ;
	print_action(philo, "is thinking");
	// Thinking süresi - kritik iyileştirme
	// Her filozof biraz düşünsün ki diğerleri şans bulsun
	if (philo->data->num_philo % 2 != 0) // Tek sayıda filozof varsa
		usleep(1000); // 1ms thinking time
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	// Fork order strategy - deadlock prevention ve fairness için
	// ID'ye göre fork sırası belirleme (daha etkili strateji)
	if (philo->id % 2 == 1)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	pthread_mutex_lock(first_fork);
	print_action(philo, "has taken first a fork");
	// Eğer tek çatal kaldıysa ve tek filozof varsa
	if (philo->data->num_philo == 1)
		return ;
	pthread_mutex_lock(second_fork);
	print_action(philo, "has taken a second fork");
}

void	put_down_forks(t_philo *philo)
{
	// Fork bırakma sırası - deadlock prevention
	if (philo->id % 2 == 1)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}

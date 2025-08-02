/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palaca <palaca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 18:07:05 by palaca            #+#    #+#             */
/*   Updated: 2025/08/02 20:17:18 by palaca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

void	print_action(t_philo *philo, const char *msg)
{
	long	timestamp;

	pthread_mutex_lock(&philo->data->print_mutex);
	// Simulation durumunu print sırasında kontrol et
	if (!is_simulation_over(philo->data))
	{
		timestamp = get_time_in_ms() - philo->data->start_time;
		printf("%ld %d %s\n", timestamp, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void	print_died(t_philo *philo, const char *msg)
{
	long	timestamp;

	timestamp = get_time_in_ms() - philo->data->start_time;
	pthread_mutex_lock(&philo->data->print_mutex);
	pthread_mutex_lock(&philo->data->death_mutex);
	if (!philo->data->all_ate) // Sadece all_ate durumu değilse print et
	{
		printf("%ld %d %s\n", timestamp, philo->id, msg);
		philo->data->someone_died = 1;
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

int	philo_has_died(t_philo *philo)
{
	long	current_time;
	long	time_since_last_meal;
	int		is_dead;

	is_dead = 0;
	pthread_mutex_lock(&philo->meal_mutex);
	current_time = get_time_in_ms();
	time_since_last_meal = current_time - philo->last_meal_time;
	pthread_mutex_unlock(&philo->meal_mutex);
	// Death check - daha kesin kontrol
	if (time_since_last_meal >= philo->data->time_to_die)
	{
		pthread_mutex_lock(&philo->data->death_mutex);
		if (!philo->data->someone_died && !philo->data->all_ate)
		{
			is_dead = 1;
		}
		pthread_mutex_unlock(&philo->data->death_mutex);
	}
	return (is_dead);
}

void	smart_sleep(long duration_ms, t_data *data)
{
	long	start_time;

	start_time = get_time_in_ms();
	// Büyük sleep'leri parçalara böl ve simulation durumunu kontrol et
	while ((get_time_in_ms() - start_time) < duration_ms)
	{
		// Her 5ms'de simulation durumunu kontrol et
		if (is_simulation_over(data))
			return ;	
		// if (duration_ms - elapsed > 10)
		// 	usleep(5000); // 5ms chunks
		// else
			usleep(500);  // Son kısımda daha hassas
	}
}

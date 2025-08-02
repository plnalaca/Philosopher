/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: palaca <palaca@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 17:14:58 by palaca            #+#    #+#             */
/*   Updated: 2025/08/02 20:16:53 by palaca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_struct_vars(int argc, char **argv, t_data *data)
{
	data->num_philo = ft_atol(argv[1], 1);
	data->time_to_die = ft_atol(argv[2], 1);
	data->time_to_eat = ft_atol(argv[3], 1);
	data->time_to_sleep = ft_atol(argv[4], 1);
	if (argc == 6)
		data->must_eat_count = ft_atol(argv[5], 1);
	else
		data->must_eat_count = -1;
	data->someone_died = 0;
	data->all_ate = 0;
	data->start_time = get_time_in_ms();
}

int	init_data(int argc, char **argv, t_data *data)
{
	int	i;

	i = 0;
	init_struct_vars(argc, argv, data);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philo);
	if (!data->forks)
	{
		printf("Malloc error.\n");
		return (1);
	}
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->death_mutex, NULL);
	while (i < data->num_philo)
		pthread_mutex_init(&data->forks[i++], NULL);
	data->philos = init_philosophers(data);
	return (0);
}

t_philo	*init_philosophers(t_data *data)
{
	t_philo			*philos;
	int				i;

	i = 0;
	philos = malloc(sizeof(t_philo) * data->num_philo);
	if (!philos)
		exit(1);
	while (i < data->num_philo)
	{
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].last_meal_time = get_time_in_ms();
		philos[i].data = data;
		philos[i].left_fork = &data->forks[i];
		philos[i].right_fork = &data->forks[(i + 1) % data->num_philo];
		philos[i].num = data->num_philo;
		pthread_mutex_init(&philos[i].meal_mutex, NULL);
		i++;
	}
	return (philos);
}

void	cleanup_data(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->num_philo)
			pthread_mutex_destroy(&data->forks[i++]);
		free(data->forks);
		data->forks = NULL;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	pthread_mutex_destroy(&data->philos->meal_mutex);
	if (data->philos)
	{
		free(data->philos);
		data->philos = NULL;
	}
}

void	perform_eating(t_philo *philo)
{
	long	eat_chunks;
	long	remaining;
	int		i;
	
	i = 0;
	// Critical section - meal time update
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_time_in_ms();
	pthread_mutex_unlock(&philo->meal_mutex);
	print_action(philo, "is eating");
	// Eating süresini bölerek kontrol et
	eat_chunks = philo->data->time_to_eat / 50;
	remaining = philo->data->time_to_eat % 50;
	while (i < eat_chunks && !is_simulation_over(philo->data))
	{
		smart_sleep(50, philo->data);
		i++;
	}
	if (!is_simulation_over(philo->data) && remaining > 0)
		smart_sleep(remaining, philo->data);
}

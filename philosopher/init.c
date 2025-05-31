#include "philo.h"

int init_data (int argc, char **argv, t_data *data)
{
  int i = 0;
  data->num_philo = ft_atol(argv[1]);
  data->time_to_die = ft_atol(argv[2]);
  data->time_to_eat = ft_atol(argv[3]);
  data->time_to_sleep = ft_atol(argv[4]);
  if (argc == 6)
	  data->must_eat_count = ft_atol(argv[5]);
	else
		data->must_eat_count = -1;
	data->someone_died = 0;
	data->all_ate = 0;
	data->start_time = get_time_in_ms(); 
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
	return(0);
}

t_philo	*init_philosophers(t_data *data)
{
	t_philo			*philos;
	int				i;

	philos = malloc(sizeof(t_philo) * data->num_philo);
	if (!philos)
		exit(1);
	for (i = 0; i < data->num_philo; i++)
	{
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].last_meal_time = get_time_in_ms();
		philos[i].data = data;
		philos[i].left_fork = &data->forks[i];
		philos[i].right_fork = &data->forks[(i + 1) % data->num_philo];
    philos[i].num = data->num_philo;
    pthread_mutex_init(&philos[i].meal_mutex, NULL);
	}
	return (philos);
}

void	cleanup_data(t_data *data)
{
	int	i;

	// 1. Fork mutex’lerini yok et
	if (data->forks)
	{
		i = 0;
		while (i < data->num_philo)
			pthread_mutex_destroy(&data->forks[i++]);
		// Belleği serbest bırak
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


#include "philo.h"

void	loop(t_data *data, int *full_count, int i)
{
	pthread_mutex_lock(&data->philos[i].meal_mutex);  // Doğru philosopher'ın mutex'ini kullan
	if (data->must_eat_count != -1
		&& data->philos[i].meals_eaten >= data->must_eat_count)
		(*full_count)++;
	pthread_mutex_unlock(&data->philos[i].meal_mutex);
}

void	*monitor_thread(void *arg)
{
	t_data	*data;
	int		i;
	int		full_count;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		full_count = 0;
		while (i < data->num_philo)
		{
			if (philo_has_died(&data->philos[i]))
			{
				print_died(&data->philos[i], "died");
				return (NULL);
			}
			loop(data, &full_count, i);
			i++;
		}
		if (all_eat(data, full_count))
			return (NULL);
		usleep(100);  // Daha sık kontrol et (200'den 100'e düşür)
	}
	return (NULL);
}

int	all_eat(t_data *data, int full_count)
{
	if (data->must_eat_count != -1 && full_count == data->num_philo)
	{
		pthread_mutex_lock(&data->death_mutex);
		data->all_ate = 1;
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	return (0);
}

int	is_simulation_over(t_data *data)
{
	int	result;

	pthread_mutex_lock(&data->death_mutex);
	result = (data->someone_died || data->all_ate);
	pthread_mutex_unlock(&data->death_mutex);
	return (result);
}
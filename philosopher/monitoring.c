#include "philo.h"

void	*monitor_thread(void *arg)
{
	t_data	*data = (t_data *)arg;
	int		i;
	int		full_count;
	while (1)
	{
		i = 0;
		full_count = 0;
		while (i < data->num_philo)
		{
			if (philo_has_died(&data->philos[i]))
				return (NULL);
			pthread_mutex_lock(&data->philos->meal_mutex);
			if (data->must_eat_count != -1 &&
				data->philos[i].meals_eaten >= data->must_eat_count)
				full_count++;
			pthread_mutex_unlock(&data->philos->meal_mutex);
			i++;
		}
		if (data->must_eat_count != -1 && full_count == data->num_philo)
		{
			data->all_ate = 1;
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}



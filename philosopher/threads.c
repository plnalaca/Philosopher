#include "philo.h"

int	start_threads(t_data *data)
{
	int	i;

	data->start_time = get_time_in_ms();
	i = 0;
	while (i < data->num_philo)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				philosopher_lifecycle, &data->philos[i]) != 0)
			return (1);
		i++;
	}
	i = 0;
	while (i < data->num_philo)
	{
		if (pthread_join(data->philos[i].thread, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}
void	*philosopher_lifecycle(void *arg)
{
	t_philo	*philo = (t_philo *)arg;
	if (philo->data->num_philo == 1)
	{
		print_action(philo, "has taken a left fork");
		usleep(philo->data->time_to_die * 1000);
		print_action(philo, "died");
		philo->data->someone_died = 1;
		return (NULL);
	}
	while (1)
	{
		if (philo->data->someone_died || philo->data->all_ate)
			break;
		if (philo->id % 2 == 0)
		{
			eat(philo);
			sleep_and_think(philo);
		}
		else
		{
			sleep_and_think(philo);
			eat(philo);
		}
	}
	return (NULL);
}
void	eat(t_philo *philo)
{
	if (philo->data->someone_died || philo->data->all_ate)
		return;
    take_forks(philo);
    if (!philo->left_fork || !philo->right_fork)
        return;
	if (philo->data->someone_died || philo->data->all_ate)
	{
		put_down_forks(philo);
		return;
	}
	pthread_mutex_lock(&philo->meal_mutex);
    philo->last_meal_time = get_time_in_ms();
    pthread_mutex_unlock(&philo->meal_mutex);
	print_action(philo, "is eating");
	smart_sleep(philo->data->time_to_eat);
	philo->meals_eaten++;
    put_down_forks(philo);
}

void	sleep_and_think(t_philo *philo)
{
	if (philo->data->someone_died || philo->data->all_ate)
		return;
	print_action(philo, "is sleeping");
	smart_sleep(philo->data->time_to_sleep);
	if (philo->data->someone_died || philo->data->all_ate)
		return;
	print_action(philo, "is thinking");
}

void	take_forks(t_philo *philo)
{
    if (philo->id % 2 == 0)
	{
        pthread_mutex_lock(philo->left_fork);
	    print_action(philo, "has taken a left fork");
	    pthread_mutex_lock(philo->right_fork);
	    print_action(philo, "has taken a right fork");
    }
    else if (philo->id % 2 != 0)
	{
        pthread_mutex_lock(philo->left_fork);
	    print_action(philo, "has taken a left fork");
	    pthread_mutex_lock(philo->right_fork);
	    print_action(philo, "has taken a right fork");
    }
}
void	put_down_forks(t_philo *philo)
{
	   if (philo->id % 2 == 0)
	{
	    pthread_mutex_unlock(philo->right_fork);
	    print_action(philo, "has put a right fork");
        pthread_mutex_unlock(philo->left_fork);
	    print_action(philo, "has put a left fork");
    }
    else if (philo->id % 2 != 0)
	{
	    pthread_mutex_unlock(philo->right_fork);
	    print_action(philo, "has put a right fork");
        pthread_mutex_unlock(philo->left_fork);
	    print_action(philo, "has put a left fork");
    }
}

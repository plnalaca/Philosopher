#include "philo.h"

long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

void    print_action(t_philo *philo, const char *msg)
{
	long    timestamp;

	timestamp = get_time_in_ms() - philo->data->start_time;

	pthread_mutex_lock(&philo->data->print_mutex);
	pthread_mutex_lock(&philo->data->death_mutex);
	if (!(philo->data->someone_died) && !(philo->data->all_ate))
		printf("%ld %d %s\n", timestamp, philo->id, msg);
	pthread_mutex_unlock(&philo->data->death_mutex);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void    print_died(t_philo *philo, const char *msg)
{
	long    timestamp;

	timestamp = get_time_in_ms() - philo->data->start_time;

	pthread_mutex_lock(&philo->data->print_mutex);
	if(philo->data->someone_died)
		printf("%ld %d %s\n", timestamp, philo->id, msg);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

int	philo_has_died(t_philo *philo)
{
	long	current_time;
	long	time_since_last_meal;

	pthread_mutex_lock(&philo->meal_mutex);
	current_time = get_time_in_ms();
	time_since_last_meal = current_time - philo->last_meal_time;
	pthread_mutex_unlock(&philo->meal_mutex);
	pthread_mutex_lock(&philo->data->death_mutex);
	if (time_since_last_meal >= philo->data->time_to_die && !philo->data->someone_died)
	{
		philo->data->someone_died = 1;
		pthread_mutex_unlock(&philo->data->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	return (0);
}


void	smart_sleep(long duration_ms)
{
	long	start_time = get_time_in_ms();

	while ((get_time_in_ms() - start_time) < duration_ms)
		usleep(100);

}

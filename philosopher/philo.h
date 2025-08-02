#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>

typedef struct s_data
{
	int				num_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	int				someone_died;
	long			start_time;
	int				all_ate;

	pthread_mutex_t	death_mutex;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;

	struct s_philo 	*philos;
}	t_data;


typedef struct	s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	int				num;

	pthread_t		thread;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;

	t_data			*data;
}   t_philo;

int		init_data (int argc, char **argv, t_data *data);
int		check_args(int argc, char **argv);
t_philo	*init_philosophers(t_data *data);
void	cleanup_data(t_data *data);
int		start_threads(t_data *data);
long	ft_atol(char *str);
long	get_time_in_ms(void);
void	take_forks(t_philo *philo);
void	*philosopher_lifecycle(void *arg);
void		print_action(t_philo *philo, const char *msg);
void	eat(t_philo *philo);
void	sleep_and_think(t_philo *philo);
void	put_down_forks(t_philo *philo);
int		philo_has_died(t_philo *philo);
void	smart_sleep(long duration_ms, t_data *data);
void	*monitor_thread(void *arg);
int		is_simulation_over(t_data *data);
void	print_died(t_philo *philo, const char *msg);
int		all_eat(t_data *data, int full_count);
void	perform_eating(t_philo *philo);
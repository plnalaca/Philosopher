#include "philo.h"

long	ft_atol(char *str)
{
	long	res = 0;
	int		i = 0;
	int		sign = 1;

	while (str[i] >= 9 && str[i] <= 13)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = sign * (-1);
		i++;
	}
	if (str[i] == '\0')
		return (-1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		res = res * 10 + (str[i] - '0');
		if (res > INT_MAX)
			return (-1);
		i++;
	}
	return (res * sign);
}

int	check_args(int argc, char **argv)
{
	int	val;
	int	i;

	i = 1;
	if (argc != 5 && argc != 6)
	{
		printf("Error: Expected 4 or 5 arguments.\n");
		return (1);
	}
	while (i < argc)
	{
		val = ft_atol(argv[i]);
		if (val <= 0)
		{
			printf("Error: All arguments must be positive integers.\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	monitor;

	if (check_args(argc, argv))
		return (1);
	if (init_data(argc, argv, &data))
		return (1);
	if (pthread_create(&monitor, NULL, monitor_thread, &data) != 0)
	{
		printf("Monitor başlatılamadı\n");
		return (1);
	}
	if (start_threads(&data) != 0)
	{
		printf("Thread başlatılamadı.\n");
		cleanup_data(&data);
		return (1);
	}
	pthread_join(monitor, NULL);
	cleanup_data(&data);
	return (0);
}

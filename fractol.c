#include "mlx.h"
#include <unistd.h>
#include <stdio.h>
//#include <math.h>
#define WIDTH 1000
#define HEIGHT 1000
#define MAX_ITERATION 1000

typedef struct	s_mlx_info
{
	void	*mlx;
	void	*window;
	void	*image;
	int		*img;
	int		line_size;
}				t_mlx_info;

int power(int nb, int pow)
{
	if (pow > 1)
		return (power(nb, pow - 1) * nb);
	else if (pow)
		return (nb);
	else
		return (1);
}

int get_amazing_color(int iteration)
{
	int true_number;

	true_number = ((double)iteration / MAX_ITERATION) * 768;
	return (power(256,true_number / 256) * (true_number % 256));
}

int tab[1000] = {0};

int get_real_color(int iteration)
{
	int true_number;

	true_number = ((double)iteration / MAX_ITERATION) * 256;
	if (true_number < 128)
		return (256 * true_number + (256 - true_number));
	else
		return (256 * (128 - (true_number - 128)) + 256 * 256 * true_number);
}

int get_grey(int iteration)
{
	int true_number;

	true_number = ((double)iteration / MAX_ITERATION) * 256;
	return (256 * 256 * true_number + 256 * true_number + true_number);
}

void init_tab()
{
	int iteration = 0;

	while (iteration != MAX_ITERATION)
	{
		tab[iteration] = get_grey(iteration);
		iteration++;
	}
}

double size = 1000;
double inc = 100;

int	loop_hook(t_mlx_info *mlx_info)
{
	double	zx;
	double	zy;
	double	temp;
	double	R = 2;
	double	cx = -0.7269;
	double	cy = 0.1889;
	double	size_by_2;
	double	remember_zy;
	int		x;
	int		y = 0;
	int		iteration;
	long int check = 0;
	
	cy *= -1;
	size_by_2 = 500 - size / 2;
	while (y < HEIGHT)
	{
		x = 0;
		remember_zy = (((((double)y / WIDTH) * size + size_by_2) / 1000) * R * 2) - R;
		while (x < WIDTH)
		{
			zx = (((((double)x / WIDTH) * size + size_by_2) / 1000) * R * 2) - R;
			zy = remember_zy;
			iteration = 0;
			while (zx * zx + zy * zy < R * R && iteration < MAX_ITERATION)
			{
				temp = zx * zx - zy * zy;
				zy = 2 * zx * zy + cy;
				zx = temp + cx;
				iteration++;
				check++;
			}
			if (iteration == MAX_ITERATION)
				mlx_info->img[(mlx_info->line_size / 4) * y + x] = 0;
			else
			{
				//mlx_info->img[(mlx_info->line_size / 4) * y + x] = (int)(20 * 255 * ((double)iteration / MAX_ITERATION)) * 256 * 256;
				//mlx_info->img[(mlx_info->line_size / 4) * y + x] = get_amazing_color(iteration);
				//mlx_info->img[(mlx_info->line_size / 4) * y + x] = get_real_color(iteration);
				mlx_info->img[(mlx_info->line_size / 4) * y + x] = tab[iteration];	
			}
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(mlx_info->mlx, mlx_info->window, mlx_info->image, 0, 0);
	if (size <= inc * 2)
		inc /= 10;
	size -= inc;
	fprintf(stderr, "new image generated : %.30f %.30f\n", size, inc);
	return (0);
}

int main()
{
	t_mlx_info mlx_info;
	int trash;

	mlx_info.mlx = mlx_init();
	mlx_info.window = mlx_new_window(mlx_info.mlx, 1000, 1000, "fractoly fractola");
	mlx_info.image = mlx_new_image(mlx_info.mlx, 1000, 1000);
	mlx_info.img = (int*)mlx_get_data_addr(mlx_info.image, &trash, &mlx_info.line_size, &trash);
	mlx_loop_hook(mlx_info.mlx, *loop_hook, &mlx_info);
	init_tab();
	mlx_loop(mlx_info.mlx);
}

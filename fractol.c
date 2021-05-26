#include "mlx.h"
#include <unistd.h>
#include <stdio.h>

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

float R = 2;

int	loop_hook(t_mlx_info *mlx_info)
{
	float	zx;
	float	zy;
	float	cx = -0.70176;
	float	cy = -0.3842;
	float	temp;
	int		x;
	int		y = 0;
	int		iteration;
	long int check = 0;
	
	cy *= -1;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			zx = (((float)x / WIDTH) * R * 2) - R;
			zy = (((float)y / WIDTH) * R * 2) - R;
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
				mlx_info->img[(mlx_info->line_size / 4) * y + x] = (int)(20 * 255 * ((float)iteration / MAX_ITERATION)) * 256 * 256;
			}
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(mlx_info->mlx, mlx_info->window, mlx_info->image, 0, 0);
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
	mlx_loop(mlx_info.mlx);
}

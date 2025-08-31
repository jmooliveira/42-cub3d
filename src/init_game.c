//init_game.c

#include "cub3d.h"

int	init_game(t_game *game, t_config *cfg)
{
	game->cfg = cfg;
	game->mlx = mlx_init(WIDTH, HEIGHT, "Cub3D", true);
	if (!game->mlx)
	{
		fprintf(stderr, "Error: Failed to initialize MLX\n");
		return (0);
	}

	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (!game->img)
	{
		fprintf(stderr, "Error: Failed to create image\n");
		mlx_terminate(game->mlx);
		return (0);
	}

	if (mlx_image_to_window(game->mlx, game->img, 0, 0) == -1)
	{
		fprintf(stderr, "Error: Failed to display image\n");
		mlx_terminate(game->mlx);
		return (0);
	}

	// Inicializar posição do jogador
	game->player_x = cfg->player_x + 0.5;
	game->player_y = cfg->player_y + 0.5;
	game->fov = M_PI / 3; // 60 graus

	// Definir ângulo inicial baseado na direção do jogador
	if (cfg->player_dir == 'N')
		game->player_angle = 3 * M_PI / 2;
	else if (cfg->player_dir == 'S')
		game->player_angle = M_PI / 2;
	else if (cfg->player_dir == 'E')
		game->player_angle = 0;
	else if (cfg->player_dir == 'W')
		game->player_angle = M_PI;

	return (1);
}

void	render_frame(t_game *game)
{
	int	x;

	// Limpar a tela
	ft_memset(game->img->pixels, 0, WIDTH * HEIGHT * sizeof(int32_t));

	// Renderizar cada coluna da tela
	for (x = 0; x < WIDTH; x++)
	{
		render_column(game, x);
	}
}

void	render_column(t_game *game, int x)
{
	double	ray_angle;
	double	distance;
	int		wall_height;
	int		y;
	int		wall_start;
	int		wall_end;
	uint32_t	wall_color;
	uint32_t	floor_color;
	uint32_t	ceiling_color;

	// Calcular ângulo do raio
	ray_angle = game->player_angle - game->fov / 2 + (game->fov * x) / WIDTH;

	// Lançar raio e calcular distância
	distance = cast_ray(game, ray_angle);

	// Corrigir distorção fishbowl
	distance *= cos(ray_angle - game->player_angle);

	// Calcular altura da parede na tela
	wall_height = (int)(HEIGHT / distance);
	wall_start = (HEIGHT - wall_height) / 2;
	wall_end = wall_start + wall_height;

	// Converter cores RGB para formato MLX
	ceiling_color = rgb_to_color(game->cfg->ceil[0], game->cfg->ceil[1], game->cfg->ceil[2]);
	floor_color = rgb_to_color(game->cfg->floor[0], game->cfg->floor[1], game->cfg->floor[2]);
	wall_color = 0xFF808080; // Cinza para agora

	// Renderizar coluna
	for (y = 0; y < HEIGHT; y++)
	{
		if (y < wall_start)
			mlx_put_pixel(game->img, x, y, ceiling_color);
		else if (y >= wall_start && y < wall_end)
			mlx_put_pixel(game->img, x, y, wall_color);
		else
			mlx_put_pixel(game->img, x, y, floor_color);
	}
}

double	cast_ray(t_game *game, double angle)
{
	double	x, y;
	double	dx, dy;
	double	step_size = 0.01;

	x = game->player_x;
	y = game->player_y;
	dx = cos(angle) * step_size;
	dy = sin(angle) * step_size;

	while (1)
	{
		x += dx;
		y += dy;
		
		// Verificar se saiu do mapa
		if (x < 0 || y < 0 || y >= get_map_height(game->cfg) || 
			x >= (int)ft_strlen(game->cfg->map[(int)y]))
			break;
		
		// Verificar se bateu numa parede
		if (game->cfg->map[(int)y][(int)x] == '1')
			break;
	}

	// Calcular distância
	return (sqrt(pow(x - game->player_x, 2) + pow(y - game->player_y, 2)));
}

uint32_t	rgb_to_color(int r, int g, int b)
{
    return (0xFF000000 | (r << 16) | (g << 8) | b);
}

void	handle_input(void *param)
{
	t_game	*game = (t_game *)param;
	double	move_speed = 0.05;
	double	rot_speed = 0.05;
	double	new_x, new_y;

	// Movimento para frente/trás
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
	{
		new_x = game->player_x + cos(game->player_angle) * move_speed;
		new_y = game->player_y + sin(game->player_angle) * move_speed;
		if (game->cfg->map[(int)new_y][(int)new_x] != '1')
		{
			game->player_x = new_x;
			game->player_y = new_y;
		}
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
	{
		new_x = game->player_x - cos(game->player_angle) * move_speed;
		new_y = game->player_y - sin(game->player_angle) * move_speed;
		if (game->cfg->map[(int)new_y][(int)new_x] != '1')
		{
			game->player_x = new_x;
			game->player_y = new_y;
		}
	}

	// Rotação
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		game->player_angle -= rot_speed;
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		game->player_angle += rot_speed;

	// ESC para sair
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
}

void	game_loop(void *param)
{
	t_game	*game = (t_game *)param;

	handle_input(game);
	render_frame(game);
}

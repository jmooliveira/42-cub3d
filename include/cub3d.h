// cub3d.h

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <limits.h>
# include <math.h>
# include "MLX42/MLX42.h"
# include "libft.h"
# include "get_next_line.h"
# include "ft_printf.h" 

# define WIDTH 800
# define HEIGHT 600

# define BUFFERS_SIZE 1024

typedef struct s_config
{
	char	*text_no;
	char	*text_so;
	char	*text_we;
	char	*text_ea;
	int		floor[3];
	int		ceil[3];
	char	**map;
	int		player_x;
	int		player_y;
	char	player_dir;
}	t_config;

typedef struct s_game
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_config	*cfg;
	double		player_x;
	double		player_y;
	double		player_angle;
	double		fov;
}	t_game;

void		parse_file(const char *filename, t_config *cfg);

int			parse_texture(const char *filename, t_config *cfg);

int			parse_color(char *line, t_config *cfg);
int			parse_rgb(char *str, int *rgb);

int			parse_map_line(char *line, t_config *cfg);
int			is_map_line(char *line);

int			validate_map(t_config *cfg);
int			find_player(t_config *cfg);
int			validate_map_characters(t_config *cfg);
int			check_map_closed(t_config *cfg);
int			get_map_height(t_config *cfg);
int			check_horizontal_walls(t_config *cfg, int row);
int			check_vertical_walls(t_config *cfg);
int			check_reachable_spaces(t_config *cfg);
int			is_surrounded_by_walls(t_config *cfg, int y, int x);

int			init_game(t_game *game, t_config *cfg);
void		render_frame(t_game *game);
void		render_column(t_game *game, int x);
double		cast_ray(t_game *game, double angle);
uint32_t	rgb_to_color(int r, int g, int b);
void		handle_input(void *param);
void		game_loop(void *param);

void		free_config(t_config *cfg);
void		free_split(char **split);

#endif

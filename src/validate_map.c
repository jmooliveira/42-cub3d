// validate_map.c

#include "cub3d.h"

int	validate_map(t_config *cfg)
{
	if (!cfg->map || !cfg->map[0])
	{
		fprintf(stderr, "Error: No map found\n");
		return (0);
	}
	if (!find_player(cfg))
	{
		fprintf(stderr, "Error: No player found or multiple players\n");
		return (0);
	}
	if (!check_map_closed(cfg))
	{
		fprintf(stderr, "Error: Map is not closed by walls\n");
		return (0);
	}
	if (!validate_map_characters(cfg))
	{
		fprintf(stderr, "Error: Invalid characters in map\n");
		return (0);
	}
	return (1);
}

int	find_player(t_config *cfg)
{
	int	i;
	int	j;
	int	player_count;

	i = 0;
	player_count = 0;
	while (cfg->map[i])
	{
		j = 0;
		while (cfg->map[i][j])
		{
			if (cfg->map[i][j] == 'N' || cfg->map[i][j] == 'S'
				|| cfg->map[i][j] == 'E' || cfg->map[i][j] == 'W')
			{
				cfg->player_x = j;
				cfg->player_y = i;
				cfg->player_dir = cfg->map[i][j];
				player_count++;
			}
			j++;
		}
		i++;
	}
	return (player_count == 1);
}

int	validate_map_characters(t_config *cfg)
{
	int	i;
	int	j;

	i = 0;
	while (cfg->map[i])
	{
		j = 0;
		while (cfg->map[i][j])
		{
			if (cfg->map[i][j] != '0' && cfg->map[i][j] != '1' &&
				cfg->map[i][j] != 'N' && cfg->map[i][j] != 'S' &&
				cfg->map[i][j] != 'E' && cfg->map[i][j] != 'W' &&
				cfg->map[i][j] != ' ' && cfg->map[i][j] != '\n')
			{
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	check_map_closed(t_config *cfg)
{
	int	map_height;

	map_height = get_map_height(cfg);
	// Verifica bordas superior e inferior
	if (!check_horizontal_walls(cfg, 0)
		|| !check_horizontal_walls(cfg, map_height - 1))
		return (0);
	// Verifica bordas laterais
	if (!check_vertical_walls(cfg))
		return (0);
	// Verifica se todos os espaços vazios (0) são cercados por paredes
	if (!check_reachable_spaces(cfg))
		return (0);
	return (1);
}

int	get_map_height(t_config *cfg)
{
	int	height;

	height = 0;
	while (cfg->map[height])
		height++;
	return (height);
}

int	check_horizontal_walls(t_config *cfg, int row)
{
	int	j;

	j = 0;
	while (cfg->map[row][j])
	{
		if (cfg->map[row][j] == '0' || cfg->map[row][j] == 'N'
			|| cfg->map[row][j] == 'S' || cfg->map[row][j] == 'E'
			|| cfg->map[row][j] == 'W')
			return (0);
		j++;
	}
	return (1);
}

int	check_vertical_walls(t_config *cfg)
{
	int	i;
	int	first_char;
	int	last_char;

	i = 0;
	while (cfg->map[i])
	{
		first_char = 0;
		while (cfg->map[i][first_char] && cfg->map[i][first_char] == ' ')
			first_char++;
		if (cfg->map[i][first_char] && cfg->map[i][first_char] != '1')
			return (0);
		last_char = ft_strlen(cfg->map[i]) - 1;
		while (last_char >= 0 && (cfg->map[i][last_char] == ' '
			|| cfg->map[i][last_char] == '\n'))
			last_char--;
		if (last_char >= 0 && cfg->map[i][last_char] != '1')
			return (0);
		i++;
	}
	return (1);
}

int	check_reachable_spaces(t_config *cfg)
{
	int	i;
	int	j;

	i = 0;
	while (cfg->map[i])
	{
		j = 0;
		while (cfg->map[i][j])
		{
			if (cfg->map[i][j] == '0' || cfg->map[i][j] == 'N'
				|| cfg->map[i][j] == 'S' || cfg->map[i][j] == 'E'
				|| cfg->map[i][j] == 'W')
			{
				if (!is_surrounded_by_walls(cfg, i, j))
					return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	is_surrounded_by_walls(t_config *cfg, int y, int x)
{
	int	map_height;
	int	line_len;

	map_height = get_map_height(cfg);
	// Verifica bordas
	if (y == 0 || y == map_height - 1)
		return (0);
	line_len = ft_strlen(cfg->map[y]);
	if (x == 0 || x == line_len - 1)
		return (0);
	// Verifica células adjacentes
	if (y > 0 && (cfg->map[y - 1][x] == ' ' || cfg->map[y - 1][x] == '\0'))
		return (0);
	if (y < map_height - 1 && (cfg->map[y + 1][x] == ' '
		|| cfg->map[y + 1][x] == '\0'))
		return (0);
	if (x > 0 && cfg->map[y][x - 1] == ' ')
		return (0);
	if (x < (int)ft_strlen(cfg->map[y]) - 1 && cfg->map[y][x + 1] == ' ')
		return (0);
	return (1);
}

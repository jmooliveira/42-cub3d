// parser.c

#include "cub3d.h"

void	parse_file(const char *filename, t_config *cfg)
{
	int		fd;
	char	*line;
	int		line_num;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("Error: file not open");
		exit(1);
	}
	line_num = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		printf("linha[%d]: %s", line_num, line);
		line_num++;
		if (line[0] == '\n' || line[0] == '\0')
		{
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		if (parse_texture(line, cfg))
			;
		else if (parse_color(line, cfg))
			;
		else if (parse_map_line(line, cfg))
			;
		else
			fprintf(stderr, "Error: invalid line -> %s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

//parser_texture.c
int	parse_texture(const char *line, t_config *cfg)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
	{
		cfg->text_no = ft_strtrim(line + 3, " \t\n");
		return (1);
	}
	if (ft_strncmp(line, "SO ", 3) == 0)
	{
		cfg->text_so = ft_strtrim(line + 3, " \t\n");
		return (1);
	}
	if (ft_strncmp(line, "WE ", 3) == 0)
	{
		cfg->text_we = ft_strtrim(line + 3, " \t\n");
		return (1);
	}
	if (ft_strncmp(line, "EA ", 3) == 0)
	{
		cfg->text_ea = ft_strtrim(line + 3, " \t\n");
		return (1);
	}
	return (0);
}

//parser_color.c
int	parse_color(char *line, t_config *cfg)
{
	if (ft_strncmp(line, "F ", 2) == 0)
	{
		if (parse_rgb(line + 2, cfg->floor) == 0)
			return (1);
		else
		{
			fprintf(stderr, "Error: invalid floor color format\n");
			return (0);
		}
	}
	if (ft_strncmp(line, "C ", 2) == 0)
	{
		if (parse_rgb(line + 2, cfg->ceil) == 0)
			return (1);
		else
		{
			fprintf(stderr, "Error: invalid ceiling color format\n");
			return (0);
		}
	}
	return (0);
}

int	parse_rgb(char *str, int *rgb)
{
	char	**values;
	int		i;
	int		value;

	str = ft_strtrim(str, " \t\n");
	if (!str)
		return (-1);
	values = ft_split(str, ',');
	free(str);
	if (!values)
		return (-1);
	i = 0;
	while (values[i])
		i++;
	if (i != 3)
	{
		free_split(values);
		return (-1);
	}
	i = 0;
	while (i < 3)
	{
		values[i] = ft_strtrim(values[i], " \t");
		value = ft_atoi(values[i]);
		if (value < 0 || value > 255)
		{
			free_split(values);
			return (-1);
		}
		rgb[i] = value;
		i++;
	}
	free_split(values);
	return (0);
}

int	parse_map_line(char *line, t_config *cfg)
{
	static int	map_started = 0;
	static int	map_size = 0;
	char		*trimmed_line;
	char		**new_map;
	int			i;

	trimmed_line = ft_strtrim(line, " \t\n");
	if (!trimmed_line)
		return (0);
	if (is_map_line(trimmed_line))
	{
		map_started = 1;
		new_map = malloc(sizeof(char *) * (map_size + 2));
		if (!new_map)
		{
			free(trimmed_line);
			return (0);
		}
		i = 0;
		while (i < map_size)
		{
			new_map[i] = cfg->map[i];
			i++;
		}
		new_map[map_size] = ft_strdup(line); // linha original com espaços
		new_map[map_size + 1] = NULL;
		if (cfg->map)
			free(cfg->map);
		cfg->map = new_map;
		map_size++;
		free(trimmed_line);
		return (1);
	}
	free(trimmed_line);
	if (map_started)
	{
		fprintf(stderr, "Error: invalid character in map\n");
		return (0);
	}
	return (0);
}

int	is_map_line(char *line)
{
	int	i;
	int	has_valid_char;

	if (!line || !*line)
		return (0);
	i = 0;
	has_valid_char = 0;
	while (line[i])
	{
		if (line[i] == '0' || line[i] == '1'
			|| line[i] == 'N' || line[i] == 'S'
			|| line[i] == 'E' || line[i] == 'W'
			|| line[i] == ' ')
		{
			if (line[i] != ' ')
				has_valid_char = 1;
			i++;
		}
		else
			return (0);
	}
	return (has_valid_char);
}

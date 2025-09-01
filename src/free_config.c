// free_config.c

#include "cub3d.h"

void	free_config(t_config *cfg)
{
	int	i;

	if (cfg->text_no)
		free(cfg->text_no);
	if (cfg->text_so)
		free(cfg->text_so);
	if (cfg->text_we)
		free(cfg->text_we);
	if (cfg->text_ea)
		free(cfg->text_ea);
	if (cfg->map)
	{
		i = 0;
		while (cfg->map[i])
		{
			free(cfg->map[i]);
			i++;
		}
		free(cfg->map);
	}
}

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

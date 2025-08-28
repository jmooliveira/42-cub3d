// main.c

#include "cub3d.h"

int	main(int argc, char **argv)
{
	t_config	cfg;
	t_game		game;

	if(argc != 2)
	{
		fprintf(stderr, "Error: use: %s <file.cub\n", argv[0]);
		return 1;
	}
	ft_memset(&cfg, 0, sizeof(t_config));
	parse_file(argv[1], &cfg);
		if (!validate_map(&cfg))
	{
		free_config(&cfg);
		return (1);
	}
	printf("\nMap validation successful!\n");
	printf("Player found at (%d, %d) facing %c\n", 
	cfg.player_x, cfg.player_y, cfg.player_dir);
	// *****
    if (!init_game(&game, &cfg))
	{
        free_config(&cfg);
        return (1);
    }
    printf("Game initialized! Use WASD to move, ESC to exit.\n");
    mlx_loop_hook(game.mlx, game_loop, &game);
    mlx_loop(game.mlx);
    mlx_terminate(game.mlx);
	// *****
	free_config(&cfg);
	return EXIT_SUCCESS;
}

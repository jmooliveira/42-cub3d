#include "cub3d.h"

int	main(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr, "Error: use: %s <file.cub\n", argv[0]);
		return 1;
	}
	parse_file(argv[1]);
	return (0);
}

void	parse_file(const char *filename)
{
	int	fd;
	char	buffer[BUFFERS_SIZE + 1];
	ssize_t	bytes_read;

	fd = open(filename, O_RDONLY);
	if(fd < 0)
	{
		perror("Error: file not open");
		exit(1);
	}
	while((bytes_read = read(fd, buffer, BUFFERS_SIZE)) > 0)
	{
		buffer[bytes_read] = '\0';
		printf("%s", buffer);
	}
	if(bytes_read < 0)
		perror("Error: filename read error");
	close(fd);
}
#include "shell.h"

/**
 * main - entry point
 * @custom_ac: arg count
 * @custom_av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int custom_ac, char **custom_av)
{
	custom_info_t info[] = { CUSTOM_INFO_INIT };
	int custom_fd = 2;

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (custom_fd)
		: "r" (custom_fd));

	if (custom_ac == 2)
	{
		custom_fd = open(custom_av[1], O_RDONLY);
		if (custom_fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_custom_eputs(custom_av[0]);
				_custom_eputs(": 0: Can't open ");
				_custom_eputs(custom_av[1]);
				_custom_eputchar('\n');
				_custom_eputchar(CUSTOM_BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		info->readFileDescriptor = custom_fd;
	}
	populate_custom_env_list(info);
	custom_read_history(info);
	custom_hsh(info, custom_av);
	return (EXIT_SUCCESS);
}

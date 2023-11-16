#include "shell.h"

/**
 * custom_clear_info - initializes info_t struct
 * @info: struct address
 */
void custom_clear_info(custom_info_t *info)
{
	info->custom_arg = NULL;
	info->custom_argv = NULL;
	info->custom_path = NULL;
	info->custom_argc = 0;
}
/**
 * custom_set_info - initializes info_t struct
 * @info: struct address
 * @custom_av: custom argument vector
 */
void custom_set_info(custom_info_t *info, char **custom_av)
{
	int i = 0;

	info->custom_fname = custom_av[0];
	if (info->custom_arg)
	{
		info->custom_argv = custom_strtow(info->custom_arg, " \t");
		if (!info->custom_argv)
		{
			info->custom_argv = malloc(sizeof(char *) * 2);
			if (info->custom_argv)
			{
				info->custom_argv[0] = custom_strdup(info->custom_arg);
				info->custom_argv[1] = NULL;
			}
		}
		for (i = 0; info->custom_argv && info->custom_argv[i]; i++)
			;
		info->custom_argc = i;
		custom_replace_alias(info);
		custom_replace_vars(info);
	}
}

/**
 * custom_free_info - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void custom_free_info(custom_info_t *info, int all)
{
	custom_ffree(info->custom_argv);
	info->custom_argv = NULL;
	info->custom_path = NULL;

	if (all)
	{
		if (!info->custom_cmd_buf)
			free(info->custom_arg);
		if (info->custom_env)
			custom_free_list(&(info->custom_env));
		if (info->custom_history)
			custom_free_list(&(info->custom_history));
		if (info->custom_alias)
			custom_free_list(&(info->custom_alias));
		custom_ffree(info->environ);
		info->environ = NULL;
		free_pointer((void **)info->custom_cmd_buf);
		if (info->readFileDescriptor > 2)
			close(info->readFileDescriptor);
		custom_putchar(CUSTOM_BUF_FLUSH);
	}
}

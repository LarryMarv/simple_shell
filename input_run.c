#include "shell.h"

/**
 * custom_hsh - main shell loop
 * @info: the parameter & return info struct
 * @custom_av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int custom_hsh(custom_info_t *info, char **custom_av)
{
	ssize_t custom_r = 0;
	int custom_builtin_ret = 0;

	while (custom_r != -1 && custom_builtin_ret != -2)
	{
		custom_clear_info(info);
		if (isInteractive(info))
			custom_puts("Jones&Hillary$: ");
		_custom_eputchar(CUSTOM_BUF_FLUSH);
		custom_r = custom_get_input(info);
		if (custom_r != -1)
		{
			custom_set_info(info, custom_av);
			custom_builtin_ret = custom_find_builtin(info);
			if (custom_builtin_ret == -1)
				custom_find_cmd(info);
		}
		else if (isInteractive(info))
			custom_putchar('\n');
		custom_free_info(info, 0);
	}
	custom_write_history(info);
	custom_free_info(info, 1);
	if (!isInteractive(info) && info->status)
		exit(info->status);
	if (custom_builtin_ret == -2)
	{
		if (info->errorNumber == -1)
			exit(info->status);
		exit(info->errorNumber);
	}
	return (custom_builtin_ret);
}

/**
 * custom_find_builtin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int custom_find_builtin(custom_info_t *info)
{
	int i, custom_built_in_ret = -1;
	custom_builtin_table custom_builtintbl[] = {
		{"exit", exitShell},
		{"env", _custom_env},
		{"help", showHelp},
		{"history",  _custom_history},
		{"setenv", _custom_setenv},
		{"unsetenv", _custom_unsetenv},
		{"cd", changeDirectory},
		{"alias",  _custom_alias},
		{NULL, NULL}
	};

	for (i = 0; custom_builtintbl[i].type; i++)
		if (custom_strcmp(info->custom_argv[0], custom_builtintbl[i].type) == 0)
		{
			info->line_count++;
			custom_built_in_ret = custom_builtintbl[i].func(info);
			break;
		}
	return (custom_built_in_ret);
}

/**
 * custom_find_cmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void custom_find_cmd(custom_info_t *info)
{
	char *path = NULL;
	int i, k;

	info->custom_path = info->custom_argv[0];
	if (info->custom_linecount_flag == 1)
	{
		info->line_count++;
		info->custom_linecount_flag = 0;
	}
	for (i = 0, k = 0; info->custom_arg[i]; i++)
		if (!isDelimiter(info->custom_arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = custom_find_path(info,
	getEnvironmentV(info, "PATH="), info->custom_argv[0]);
	if (path)
	{
		info->custom_path = path;
		custom_fork_cmd(info);
	}
	else
	{
		if ((isInteractive(info) ||
		getEnvironmentV(info, "PATH=")
			|| info->custom_argv[0][0] == '/') &&
			custom_is_cmd(info, info->custom_argv[0]))
			custom_fork_cmd(info);
		else if (*(info->custom_arg) != '\n')
		{
			info->status = 127;
			custom_print_error(info, "not found\n");
		}
	}
}

/**
 * custom_fork_cmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void custom_fork_cmd(custom_info_t *info)
{
	pid_t custom_child_pid;

	custom_child_pid = fork();
	if (custom_child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (custom_child_pid == 0)
	{
		if (execve(info->custom_path, info->custom_argv,
		custom_get_environ(info)) == -1)
		{
			custom_free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				custom_print_error(info, "Permission denied\n");
		}
	}
}


#include "shell.h"
/**
 * exitShell - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: exits with a given exit status
 * (0) if shellInfo.argv[0] != "exit"
 */
int exitShell(custom_info_t *info)
{
	int exitCheck;

	if (info->custom_argv[1]) /* If there is an exit argument */
	{
		exitCheck = _custom_erratoi(info->custom_argv[1]);
		if (exitCheck == -1)
		{
			info->status = 2;
			custom_print_error(info, "Illegal number: ");
			_custom_eputs(info->custom_argv[1]);
			_custom_eputchar('\n');
			return (1);
		}
		info->errorNumber = _custom_erratoi(info->custom_argv[1]);
		return (-2);
	}
	info->errorNumber = -1;
	return (-2);
}

/**
 * changeDirectory - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int changeDirectory(custom_info_t *info)
{
	char *currentDir, *newDir, buffer[1024];
	int chdirReturn;

	currentDir = getcwd(buffer, 1024);
	if (!currentDir)
		custom_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->custom_argv[1])
	{
		newDir = getEnvironmentV(info, "HOME=");
		if (!newDir)
			chdirReturn = /* TODO: what should this be? */
				chdir((newDir = getEnvironmentV(info, "PWD=")) ? newDir : "/");
		else
			chdirReturn = chdir(newDir);
	}
	else if (custom_strcmp(info->custom_argv[1], "-") == 0)
	{
		if (!getEnvironmentV(info, "OLDPWD="))
		{
			custom_puts(currentDir);
			custom_putchar('\n');
			return (1);
		}
		custom_puts(getEnvironmentV(info, "OLDPWD=")), custom_putchar('\n');
		chdirReturn = /* TODO: what should this be? */
			chdir((newDir = getEnvironmentV(info, "OLDPWD=")) ? newDir : "/");
	}
	else
		chdirReturn = chdir(info->custom_argv[1]);
	if (chdirReturn == -1)
	{
		custom_print_error(info, "can't cd to ");
		_custom_eputs(info->custom_argv[1]), _custom_eputchar('\n');
	}
	else
	{
		custom_setenv(info, "OLDPWD", getEnvironmentV(info, "PWD="));
		custom_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * showHelp - displays help information
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int showHelp(custom_info_t *info)
{
	char **argumentArray;

	argumentArray = info->custom_argv;
	custom_puts("help call works. Function not yet implemented \n");
	if (0)
		custom_puts(*argumentArray);
	return (0);
}

#include "shell.h"
/**
 * _custom_history - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _custom_history(custom_info_t *info)
{
	custom_print_list(info->custom_history);
	return (0);
}
/**
 * unset_custom_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_custom_alias(custom_info_t *info, char *str)
{
	char *p, c;
	int ret;

	p = _custom_strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = custom_delete_node_at_index(&(info->custom_alias),
		custom_get_node_index(info->custom_alias,
		custom_node_starts_with(info->custom_alias, str, -1)));
	*p = c;
	return (ret);
}
/**
 * set_custom_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_custom_alias(custom_info_t *info, char *str)
{
	char *p;

	p = _custom_strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_custom_alias(info, str));

	unset_custom_alias(info, str);
	return (custom_add_node_end(&(info->custom_alias), str, 0) == NULL);
}
/**
 * print_custom_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_custom_alias(custom_list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _custom_strchr(node->str, '=');
		for (a = node->str; a <= p; a++)
			custom_putchar(*a);
		custom_putchar('\'');
		custom_puts(p + 1);
		custom_puts("'\n");
		return (0);
	}
	return (1);
}
/**
 * _custom_alias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _custom_alias(custom_info_t *info)
{
	int i = 0;
	char *p = NULL;
	custom_list_t *node = NULL;

	if (info->custom_argc == 1)
	{
		node = info->custom_alias;
		while (node)
		{
			print_custom_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->custom_argv[i]; i++)
	{
		p = _custom_strchr(info->custom_argv[i], '=');
		if (p)
			set_custom_alias(info, info->custom_argv[i]);
		else
			print_custom_alias(custom_node_starts_with
			(info->custom_alias, info->custom_argv[i], '='));
	}

	return (0);
}


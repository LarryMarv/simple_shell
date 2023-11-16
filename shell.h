#ifndef MAIN_SHELL_HEADER_FILE
#define MAIN_SHELL_HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define CUSTOM_WRITE_BUF_SIZE 1024
#define CUSTOM_BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define CUSTOM_HIST_FILE	".simple_shell_history"
#define CUSTOM_HIST_MAX	4096

extern char **environ;

/**
 * struct custom_liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct custom_liststr
{
	int num;
	char *str;
	struct custom_liststr *next;
} custom_list_t;

/**
 * struct custom_passinfo - contains pseudo-arguments to pass into a function,
 * allowing a uniform prototype for the function pointer struct
 * @custom_arg: a string generated from getline containing arguments
 * @custom_argv: an array of strings generated from arg
 * @custom_path: a string path for the current command
 * @custom_argc: the argument count
 * @line_count: the error count
 * @errorNumber: the error code for exit()s
 * @custom_linecount_flag: if on count this line of input
 * @custom_fname: the program filename
 * @custom_env: linked list local copy of environ
 * @environ: custom modified copy of environ from LL env
 * @custom_history: the history node
 * @custom_alias: the alias node
 * @env_changed: on if environ was changed
 * @status: the return status of the last exec'd command
 * @custom_cmd_buf: address of pointer to cmd_buf, on if chaining
 * @custom_cmd_buf_type: CMD_type ||, &&, ;
 * @readFileDescriptor: the fd from which to read line input
 * @custom_histcount: the history line number count
 */
typedef struct custom_passinfo
{
	char *custom_arg;
	char **custom_argv;
	char *custom_path;
	int custom_argc;
	unsigned int line_count;
	int errorNumber;
	int custom_linecount_flag;
	char *custom_fname;
	custom_list_t *custom_env;
	custom_list_t *custom_history;
	custom_list_t *custom_alias;
	char **environ;
	int env_changed;
	int status;

	/* pointer to cmd ; chain buffer, for memory management */

	char **custom_cmd_buf;
	int custom_cmd_buf_type; /* CMD_type ||, &&, ; */
	int readFileDescriptor;
	int custom_histcount;
} custom_info_t;

#define CUSTOM_INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 * struct custom_builtin - contains a builtin string and related function
 * @type: the builtin command flag
 * @func: the function
 */
typedef struct custom_builtin
{
	char *type;
	int (*func)(custom_info_t *);
} custom_builtin_table;

/* input_run.c */
int custom_hsh(custom_info_t *, char **);
int custom_find_builtin(custom_info_t *);
void custom_find_cmd(custom_info_t *);
void custom_fork_cmd(custom_info_t *);

/* __parser.c */
int custom_is_cmd(custom_info_t *, char *);
char *custom_dup_chars(char *, int, int);
char *custom_find_path(custom_info_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* input_run.c */
int custom_hsh(custom_info_t *, char **);
int custom_find_builtin(custom_info_t *);
void custom_find_cmd(custom_info_t *);
void custom_fork_cmd(custom_info_t *);

/* errors.c */
void _custom_eputs(char *);
int _custom_eputchar(char);
int _custom_putfd(char c, int fd);
int _custom_putsfd(char *, int fd);

/* str_token.c */
int custom_strlen(char *);
int custom_strcmp(char *, char *);
char *custom_starts_with(const char *, const char *);
char *custom_strcat(char *, char *);

/* str_token1.c */
char *custom_strcpy(char *, char *);
char *custom_strdup(const char *);
void custom_puts(char *);
int custom_putchar(char);

/* exit.c */
char *_custom_strncpy(char *, char *, int);
char *_custom_strncat(char *, char *, int);
char *_custom_strchr(char *, char);

/* custom_tokenizer.c */
char **custom_strtow(char *, char *);
char **custom_strtow2(char *, char);

/* reallocate_mem.c */
char *custom_memset(char *, char, unsigned int);
void custom_ffree(char **);
void *custom_realloc(void *, unsigned int, unsigned int);

/* memory.c */
int free_pointer(void **);

/* atoi.c */
int isInteractive(custom_info_t *);
int isDelimiter(char, char *);
int isAlpha(int);
int stringToInteger(char *);

/* errors1.c */
int _custom_erratoi(char *);
void custom_print_error(custom_info_t *, char *);
int custom_print_d(int, int);
char *custom_convert_number(long int, int, int);
void custom_remove_comments(char *);

/* builtin.c */
int exitShell(custom_info_t *);
int changeDirectory(custom_info_t *);
int showHelp(custom_info_t *);

/* builtin1.c */
int _custom_history(custom_info_t *);
int unset_custom_alias(custom_info_t *, char *);
int set_custom_alias(custom_info_t *, char *);
int _custom_alias(custom_info_t *);
int print_custom_alias(custom_list_t *);

/* get_line.c */
ssize_t custom_input_buf(custom_info_t *, char **, size_t *);
ssize_t custom_get_input(custom_info_t *);
ssize_t custom_read_buf(custom_info_t *, char *, size_t *);
int custom_getline(custom_info_t *, char **, size_t *);
void custom_sigintHandler(__attribute__((unused))int sig_num);

/* getinformation.c */
void custom_clear_info(custom_info_t *);
void custom_set_info(custom_info_t *, char **);
void custom_free_info(custom_info_t *, int);

/* environment.c */
int _custom_env(custom_info_t *);
char *getEnvironmentV(custom_info_t *, const char *);
int _custom_setenv(custom_info_t *);
int _custom_unsetenv(custom_info_t *);
int populate_custom_env_list(custom_info_t *);


/* custom_get_environment.c */
char **custom_get_environ(custom_info_t *);
int custom_unsetenv(custom_info_t *, char *);
int custom_setenv(custom_info_t *, char *, char *);

/* history.c */
char *custom_get_history_file(custom_info_t *info);
int custom_write_history(custom_info_t *info);
int custom_read_history(custom_info_t *info);
int custom_build_history_list(custom_info_t *info,
char *custom_buf, int linecount);
int custom_renumber_history(custom_info_t *info);

/* list.c */
custom_list_t *custom_add_node(custom_list_t **, const char *, int);
custom_list_t *custom_add_node_end(custom_list_t **, const char *, int);
size_t custom_print_list_str(const custom_list_t *);
int custom_delete_node_at_index(custom_list_t **, unsigned int);
void custom_free_list(custom_list_t **);

/* list1.c */
size_t custom_list_len(const custom_list_t *);
char **custom_list_to_strings(custom_list_t *);
size_t custom_print_list(const custom_list_t *);
custom_list_t *custom_node_starts_with(custom_list_t *, char *, char);
ssize_t custom_get_node_index(custom_list_t *, custom_list_t *);

/* _variables.c */
int custom_is_chain(custom_info_t *, char *, size_t *);
void custom_check_chain(custom_info_t *, char *, size_t *, size_t, size_t);
int custom_replace_alias(custom_info_t *);
int custom_replace_vars(custom_info_t *);
int custom_replace_string(char **, char *);

#endif


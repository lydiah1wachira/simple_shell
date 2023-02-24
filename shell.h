#ifndef SHELL_H
#define SHELL_H

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
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

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

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *					allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


/* hsh.c */
int main_shell_loop(info_t *, char **);
int find_built_in_command(info_t *);
void find_command_in_path(info_t *);
void fork_command(info_t *);

/* path.c */
int is_cmd_executable(info_t *, char *);
char *duplicate_chars(char *, int, int);
char *find_path(info_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* err_string_functions.c */
void print_input_string(char *);
int write_c_to_stderr(char);
int write_c_to_fd(char c, int fd);
int print_input_string(char *str, int fd);

/* string_functions_1.c */
int string_lenght(char *);
int string_comparison(char *, char *);
char *if_starts_with_h_stack(const char *, const char *);
char *string_conc(char *, char *);

/* string_functions_2.c */
char *string_copy(char *, char *);
char *duplicate_string(const char *);
void print_input_string(char *);
int _putchar(char);

/* string_functions_3.c */
char *string_copy(char *, char *, int);
char *string_conc(char *, char *, int);
char *locate_string_character(char *, char);

/* string_functions_4.c */
char **strtow(char *, char *);
char **split_string_to_2_words(char *, char);

/* memory_functions_1.c */
char *_memset(char *, char, unsigned int);
void free_string_of_strings(char **);
void *relocate_memory_block(void *, unsigned int, unsigned int);

/* memory_functions_2.c */
int free_pointer(void **);

/* more_functions_1.c */
int check_if_interactive(info_t *);
int check_if_char_is_delimeter(char, char *);
int check_for_alpha_char(int);
int string_to_integer(char *);

/* more_functions_2.c */
int string_to_integer_2(char *);
void print_error_msg(info_t *, char *);
int print_decimal(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* builtin_emulators_1.c */
int exit_shell(info_t *);
int change_directory(info_t *);
int get_help(info_t *);

/* builtin_emulators_2.c */
int show_history(info_t *);
int unset_alias(info_t *);
int set_alias(info_t *info, char *str);
int print_alias(list_t *node);
int mimic_alias(info_t *info);

/* line_and_buffer_functions.c */
ssize_t input_buffer(info_t *);
int get_input(info_t *, char **, size_t *);
void block_copy(int);
ssize_t read_buf(info_t *info, char *buf, size_t *i);
int get_next_line(info_t *info, char **ptr, size_t *length);

/* info_t_functions.c module */
void initialize_info_t_struct(info_t *);
void set_info_t_struct(info_t *, char **);
void free_info_t_struct(info_t *, int);

/* environment_functions_1.c module */
char *print_current_environment(info_t *, const char *);
int get_environment_value(info_t *);
int set_environment_variable(info_t *);
int unset_environment_variable(info_t *);
int populate_env_list(info_t *);

/* environment_functions_2.c module */
char **get_environment_string(info_t *);
int remove_environment_variable(info_t *, char *);
int set_environment_variable(info_t *, char *, char *);

/* file_io_functions.c */
char *get_history_file(info_t *info);
int create_file(info_t *info);
int read_history_from_file(info_t *info);
int build_history_list(info_t *info, char *buf, int linecount);
int renumber_history(info_t *info);

/* list_and_node_functions_1.c.c module */
list_t *add_node_start(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_strings_in_list(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_all_nodes_from_list(list_t **);

/* list_and_node_functions.c module */
size_t list_lenght(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_starts_with_prefix(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* chain_functions.c */
int test_chain(info_t *, char *, size_t *);
void check_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_alias(info_t *);
int replace_variables(info_t *);
int replace_string(char **, char *);

#endif

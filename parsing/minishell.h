/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 21:59:27 by oel-mest          #+#    #+#             */
/*   Updated: 2025/03/11 15:30:36 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include "../libft/libft.h"
# include <signal.h>

typedef struct s_output
{
    char *file;
    struct s_output *next;
}   t_output;

typedef enum e_token_type {
    TOKEN_WORD,
	SINGLE_Q,
	DOUBLE_Q,
    TOKEN_PIPE,         // |
    TOKEN_REDIRECT_IN,  // <
    TOKEN_REDIRECT_OUT, // >
    TOKEN_APPEND,       // >>
    TOKEN_HEREDOC,      // <<
    TOKEN_AND,          // &&
    TOKEN_OR            // ||
} t_token_type;

typedef enum e_node_type {
    NODE_COMMAND,  // A single command with args (e.g., "echo hello")
    NODE_PIPE,     // A pipeline operator (|)
    NODE_AND,      // Logical AND (&&)
    NODE_OR        // Logical OR (||)
} t_node_type;

typedef struct s_token {
    char *value;
    t_token_type type;
    int has_space;
    struct s_token *next;
} t_token;

typedef struct s_cmd {
    char *args;    // Command arguments (string )
    char *input;    // File for input redirection (<)
    char *output;   // File for output redirection (>)
    t_output *output2;
    int append;     // 1 if >> (append), 0 if > (truncate)
    char *heredoc;  // Delimiter for heredoc (<<)
} t_cmd;

typedef struct s_ast {
    t_node_type type;     // COMMAND, PIPE, AND, OR
    t_cmd *cmd;           // Only for command nodes
    struct s_ast *left;   // Left child (for &&, ||, or pipes)
    struct s_ast *right;  // Right child (for &&, ||, or pipes)
} t_ast;

typedef struct EnvNode {
    char *var;            // Variable name
    char *value;          // Variable value
    struct EnvNode *next; // Pointer to next node
} EnvNode;

t_ast *create_ast_node(t_node_type type);
t_cmd *create_cmd_node();
void add_argument(t_cmd *cmd, const t_token *token);

t_ast *parse_command(t_token **tokens);
t_ast *parse_pipeline(t_token **tokens);
t_ast *parse_logical(t_token **tokens);
t_ast *parse(t_token *tokens);

void free_cmd(t_cmd *cmd);
void free_ast(t_ast *node);

void print_indentation(int depth, int is_last);
void print_cmd(t_cmd *cmd, int depth, int is_last);
void print_ast(t_ast *node, int depth, int is_last);
void print_ast_tree(t_ast *root);

t_output    *create_output_node(char *file);
void    add_output(t_output **head, t_output *node);
void    print_outputs(t_output *head);

// char	*ft_strjoin(const char *s1, const char *s2);
// char	*ft_strdup(char *src);
// size_t	ft_strlen(const char *str);

char    *ft_getenv(char *name, EnvNode *head);
void add_node(EnvNode **head, EnvNode *new_node);
EnvNode *create_node(const char *env_entry);
void free_list(EnvNode *head);

void    expand_tokens(t_token *tokens, EnvNode *head);

// mrabt functions
char	**ft_split(char const *s, char c);
char *search_command(char *cmd, EnvNode *envp);
int execute_ast(t_ast *node, EnvNode **envp);
int execute_command(t_cmd *cmd, EnvNode **envp);

// redirections
void redirect_output(char *file, int append);
void redirect_input(char *file);
void redirect_multiple_outputs(t_output *outputs, int append);
void handle_heredoc(char *delimiter);
void prepare_heredocs(t_ast *node);

// builtin
int is_builtin(char *cmd);
int run_builtin(char **args, EnvNode **envp, t_cmd *cmd);
int builtin_echo(char *args);
int builtin_cd(char **args);
int builtin_pwd();
int builtin_export(char **args, EnvNode **envp);
int builtin_unset(char **args, EnvNode **envp);
int builtin_env(EnvNode *envp);
int builtin_exit();

void    minishell(t_ast *root, EnvNode **envp);
void handle_sigint(int sig);
void handle_wildcards(t_cmd *cmd);

//oussama functions
char *expand_env_vars(char *args, EnvNode *env_list);
char    *handle_lstatus_var();
int	get_status(void);
#endif
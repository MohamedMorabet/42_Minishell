/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 01:39:52 by oel-mest          #+#    #+#             */
/*   Updated: 2025/04/12 17:13:58 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/execution.h"

char	*read_input(t_envnode *envp)
{
	char	*line;

	line = NULL;
	line = readline("\033[1;32mHHHH$ \033[0m");
	if (!line)
	{
		free(line);
		free_env_list(envp);
		return (NULL);
	}
	return (line);
}

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay(); 
	set_status(1);
}

void	ft_add_history(char *line)
{
	if (line && *line)
	{
		add_history(line);
	}
}

int	main(int argc __unused, char *argv[] __unused, char *envp[])
{
	t_envnode	*head;
	char		*line;
	t_token		*tokens;
	t_ast		*ast;

	head = parse_envp(envp);
	printf("\033[1;32mWelcome to Minishell\033[0m\n");
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	line = read_input(head);
	while (line != NULL)
	{
		ft_add_history(line);
		tokens = tokenize(line);
		ast = parse(tokens);
		free_tokens(tokens);
		free(line);
		minishell(ast, &head);
		free_ast(ast);
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		line = read_input(head);
	}
	return (EXIT_SUCCESS);
}

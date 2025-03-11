/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oussama.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 00:55:02 by mel-mora          #+#    #+#             */
/*   Updated: 2025/03/11 04:18:56 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../parsing/minishell.h"

char *append_char(char *str, char c)
{
    char *new_str = ft_strjoin(str, (char[]){c, '\0'});
    free(str);
    return new_str;
}

char    *handle_lstatus_var()
{
    return (ft_itoa(get_status()));
}
// Helper function to handle variable expansion
char *handle_var_expansion(char *args, int *i, EnvNode *env_list)
{
    char *result = NULL;
    if (args[*i] == '?')
    {
        (*i)++;
        result = handle_lstatus_var();
    }
    else if (isalnum(args[*i]) || args[*i] == '_')
    {
        int start = *i - 1;
        while (args[*i] && (isalnum(args[*i]) || args[*i] == '_'))
            (*i)++;
        char *var_name = strndup(args + start, *i - start);
        result = ft_getenv(var_name, env_list);
        free(var_name);
    }
    else
        result = ft_strdup("$");
    return result;
}

// Main function to expand environment variables
char *expand_env_vars(char *args, EnvNode *env_list)
{
    char *result = malloc(1);
    int i = 0;
    int in_single_quote = 0;
    int in_double_quote = 0;

    if (!args || !result)
        return (free(result), NULL);
    result[0] = '\0';

    while (args[i])
    {
        if (args[i] == '\'' && !in_double_quote)
            in_single_quote = !in_single_quote;
        if (args[i] == '"' && !in_single_quote)
            in_double_quote = !in_double_quote;
        if (args[i] == '$' && !in_single_quote)
        {
            i++;
            char *var_value = handle_var_expansion(args, &i, env_list);
            if (var_value)
            {
                char *new_result = ft_strjoin(result, var_value);
                free(result);
                result = new_result;
            }
        }
        else
            result = append_char(result, args[i++]);
    }
    free(args);
    return result;
}
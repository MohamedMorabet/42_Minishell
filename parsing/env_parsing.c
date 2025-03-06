#include "minishell.h"

EnvNode *create_node(const char *env_entry)
{
    EnvNode *node = malloc(sizeof(EnvNode));
    if (!node) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    char *eq = strchr(env_entry, '=');
    if (eq == NULL)
    {
        node->var = strdup(env_entry);
        node->value = strdup("");
    }
    else
    {
        size_t name_len = eq - env_entry;
        node->var = malloc(name_len + 1);
        if (!node->var) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strncpy(node->var, env_entry, name_len);
        node->var[name_len] = '\0';
        node->value = strdup(eq + 1);
    }
    node->next = NULL;
    return node;
}

void add_node(EnvNode **head, EnvNode *new_node)
{
    if (*head == NULL) {
        *head = new_node;
    } else {
        EnvNode *current = *head;
        while (current->next != NULL)
            current = current->next;
        current->next = new_node;
    }
}

void free_list(EnvNode *head)
{
    while (head != NULL) {
        EnvNode *temp = head;
        head = head->next;
        free(temp->var);
        free(temp->value);
        free(temp);
    }
}

int is_expandable(char *str)
{
    while (str && *str)
    {
        if (*str == '$')
            return 1;
        str++;
    }
    return 0;
}
char    *ft_getenv(char *name, EnvNode *head)
{
    char *value = NULL;
    while (head != NULL)
    {
        if (strcmp(name + 1, head->var) == 0)
        {
            value = strdup(head->value);
            break;
        }
        head = head->next;
    }
    if (value == NULL)
        return (strdup(""));
    else
        return value;
}

void ft_expand(t_token *token, EnvNode *head) {
    char *input = token->value;
    size_t out_size = 128;
    size_t out_index = 0;
    char *output = malloc(out_size);
    if (!output) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; input[i] != '\0'; )
    {
        if (input[i] == '$' && (isalpha(input[i + 1]) || input[i + 1] == '_'))
        {
            size_t name_start = i + 1;
            i++;
            while (isalnum(input[i]) || input[i] == '_')
                i++;
            size_t name_len = i - name_start;
            char *var_str = malloc(name_len + 2);
            if (!var_str) {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            var_str[0] = '$';
            strncpy(var_str + 1, &input[name_start], name_len);
            var_str[name_len + 1] = '\0';
            char *env_val = ft_getenv(var_str, head);
            free(var_str);
            size_t val_len = strlen(env_val);
            if (out_index + val_len + 1 > out_size) {
                out_size = (out_index + val_len + 1) * 2;
                output = realloc(output, out_size);
                if (!output) {
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }
            }
            memcpy(output + out_index, env_val, val_len);
            out_index += val_len;
        } else {
            if (out_index + 2 > out_size) {
                out_size *= 2;
                output = realloc(output, out_size);
                if (!output) {
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }
            }
            output[out_index++] = input[i++];
        }
    }
    output[out_index] = '\0';
    free(token->value);
    token->value = output;
}

void    expand_tokens(t_token *tokens, EnvNode *head)
{
    while (tokens != NULL)
    {
        if (tokens->type == TOKEN_WORD || tokens->type == DOUBLE_Q)
        {
            if (is_expandable(tokens->value))
                ft_expand(tokens, head);
        }
        tokens = tokens->next;
    }
}
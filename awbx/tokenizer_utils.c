/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhoudr <yakhoudr@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 10:05:12 by yakhoudr          #+#    #+#             */
/*   Updated: 2022/06/07 19:01:26 by yakhoudr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void drop_token(t_token *token)
{
	free(token->value);
	free(token);
}

t_token	*create_token(int type, char *value)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token)); //to_be_freed
	if (token == 0x0 || value == 0x0)
		return (0x0);
	token->type = type;
	token->value = value;
	token->next = 0x0;
	token->prev = 0x0;
	return (token);
}

t_token	*get_token(t_token *tokens)
{
	if (tokens && tokens->type == WSPACE)
		tokens = tokens->next;
	return (tokens);
}

int	is_space(char *line, t_token	**tokens)
{
	int	i;

	if (!*line)
		return (0);
	i = 0;
	while (line[i] == ' ')
		i += 1;
	push_back(tokens, create_token(WSPACE, ft_strdup(" "))); // to be freed
	return (i);
}
int	is_great(char *line, t_token **tokens)
{
	int	i;

	if (!*line)
		return (0);
	i = 0;
	if (line[i + 1] == '>')
	{
		push_back(tokens, create_token(APPEND, ft_strdup(">>"))); // to be freed
		return (2);
	}
	push_back(tokens, create_token(GREAT, ft_strdup(">"))); // to be freed
	return (1);
}

int	is_less(char *line, t_token **tokens)
{
	int	i;

	if (!*line)
		return (0);
	i = 0;
	if (line[i + 1] == '<')
	{
		push_back(tokens, create_token(HERDOC, ft_strdup("<<"))); // to be freed
		
		return (2);
	}
	push_back(tokens, create_token(LESS, ft_strdup("<"))); // to be freed
	return (1);
}

int is_quote(char *line, t_token **tokens)
{
	int	i;

	if (!*line)
		return (0);
	i = 1;
	if (line[i])
	{
		while (line[i] && line[i] != '\'')
			i += 1;
	}
	if (line[i] && line[i] == '\'')
		push_back(tokens, create_token(QUOTE, ft_strndup(line + 1, i - 1)));
	else
		return (-1);
	return (i + 1);
}

int is_dquote(char *line, t_token **tokens)
{
	int	i;

	if (!*line)
		return (0);
	i = 1;
	if (line[i])
	{
		while ( line[i] && line[i] != '"')
			i += 1;
	}
	if (line[i] && line[i] == '"')
		push_back(tokens, create_token(DQUOTE, ft_strndup(line + 1, i - 1)));
	else
		return (-1);
	return (i + 1);
}

int is_dollar(char *line, t_token **tokens)
{
	int	i;

	if (!*line)
		return (0);
	i = 1;
	while (line[i] != '\0' && line[i] != '(' && line[i] != ')' &&
			line[i] != '>' && line[i] != '<' && line[i] != '|' &&
			line[i] != '&' && line[i] != ' ')
		i += 1;
	if (i == 1)
	{
		push_back(tokens, create_token(DOLLAR, ft_strdup("$"))); // to be freed
		return (1);
	}
	else
	{
		push_back(tokens, create_token(DOLLAR, ft_strndup(line, i)));
		return (i);
	}
}

int is_pipe(char *line, t_token **tokens)
{
	int i;

	if (!*line)
		return (0);
	i = 0;
	if (line[i + 1] == '|')
	{
		push_back(tokens, create_token(OR, ft_strdup("||"))); // to be freed
		return (2);
	}
	push_back(tokens, create_token(PIPE, ft_strdup("|"))); // to be freed
	return (1);
}

int is_and(char *line, t_token **tokens)
{
	int i;

	if (!*line)
		return (0);
	i = 0;
	if (line[i + 1] == '&')
	{
		push_back(tokens, create_token(AND, ft_strdup("&&"))); // to be freed
		return (2);
	}
	push_back(tokens, create_token(WORD, ft_strdup("&"))); // to be freed
	return (1);
}

int is_wildcard(char *line, t_token **tokens)
{
	int i;

	if (!*line)
		return (0);
	i = 0;
	if (line[i] == '*')
		push_back(tokens, create_token(WILDCARD, ft_strdup("*"))); // to be freed
	return (1);
}

int is_lparen(char *line, t_token **tokens)
{
	if (!*line)
		return (0);
	if (*line == '(')
		push_back(tokens, create_token(LPAR, ft_strdup("("))); // to be freed
	return (1);
}

int is_rparen(char *line, t_token **tokens)
{
	if (!*line)
		return (0);
	if (*line == ')')
		push_back(tokens, create_token(RPAR, ft_strdup(")"))); // to be freed
	return (1);
}

int is_word(char *line, t_token **tokens)
{
	int i;

	if (!*line)
		return (0);
	i = 0;
	while (line[i] != '\0' && not_in(line[i], " ><'\"$|&*()"))
		i++;
	push_back(tokens, create_token(WORD, ft_strndup(line, i))); // to be freed
	return (i);
}

int has_redirections(t_token *tokens)
{
	int res;

	res = 0;
	while (tokens)
	{
		if (tokens->type == GREAT || tokens->type == LESS || tokens->type == HERDOC || tokens->type == APPEND)
			return (res);
		tokens = tokens->next;
		res += 1;
	}
	return (-1);
}

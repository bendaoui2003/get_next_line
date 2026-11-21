/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendaou <mbendaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/21 10:38:31 by mbendaou          #+#    #+#             */
/*   Updated: 2026/11/21 10:53:05 by mbendaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_list	*ft_lst_new(char *content)
{
	t_list	*node;
	int		i;

	i = 0;
	node = malloc(sizeof(t_list));
	if (node == NULL)
		return (NULL);
	while (content[i])
		i++;
	node->content = malloc(i + 1);
	if (node->content == NULL)
	{
		free(node);
		return (NULL);
	}
	i = 0;
	while (content[i])
	{
		node->content[i] = content[i];
		i++;
	}
	node->content[i] = '\0';
	node->next = NULL;
	return (node);
}

void	ft_add_back(t_list **lst, t_list *new)
{
	t_list	*help;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	help = *lst;
	while (help->next != NULL)
		help = help->next;
	help->next = new;
}

void	ft_clear(t_list **lst)
{
	t_list	*current;
	t_list	*help;

	if (lst == NULL)
		return ;
	current = *lst;
	while (current != NULL)
	{
		help = current->next;
		free(current->content);
		free(current);
		current = help;
	}
	*lst = NULL;
}

int	line_len(t_list *lst)
{
	int	i;
	int	len;

	len = 0;
	while (lst != NULL)
	{
		i = 0;
		while (lst->content[i] && lst->content[i] != '\n')
			i++;
		if (lst->content[i] == '\n')
		{
			len += i + 1;
			return (len);
		}
		len += i;
		lst = lst->next;
	}
	return (len);
}

int	find_newline(char *s)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (-1);
	while (s[i])
	{
		if (s[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

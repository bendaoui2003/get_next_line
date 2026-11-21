/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendaou <mbendaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/21 10:11:05 by mbendaou          #+#    #+#             */
/*   Updated: 2026/11/21 12:11:59 by mbendaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	fill_stash(int fd, t_list **stash1)
{
	char	*buf;
	t_list	*last;
	ssize_t	n;

	buf = malloc(BUFFER_SIZE + 1);
	if (buf == NULL)
		return (-1);
	n = 1;
	last = *stash1;
	while (n > 0 && (last == NULL || find_newline(last->content) == -1))
	{
		n = read(fd, buf, BUFFER_SIZE);
		if (n > 0)
		{
			buf[n] = '\0';
			last = ft_lst_new(buf);
			if (last == NULL)
				n = -1;
			ft_add_back(stash1, last);
		}
	}
	free(buf);
	if (n < 0)
		ft_clear(stash1);
	return (n);
}

char	*list_copy(t_list *lst)
{
	char	*line;
	int		len;
	int		i;
	int		j;

	len = line_len(lst);
	line = malloc((len + 1) * sizeof(char));
	if (line == NULL)
		return (NULL);
	j = 0;
	while (lst != NULL)
	{
		i = 0;
		while (lst->content[i] && j < len)
		{
			line[j] = lst->content[i];
			j++;
			i++;
		}
		lst = lst->next;
	}
	line[j] = '\0';
	return (line);
}

char	*after_newline(char *s)
{
	char	*str;
	int		i;
	int		j;
	int		r;

	r = find_newline(s);
	if (r == -1)
		return (NULL);
	i = r + 1;
	while (s[i])
		i++;
	str = malloc(i);
	if (str == NULL)
		return (NULL);
	j = 0;
	i = r + 1;
	while (s[i])
	{
		str[j] = s[i];
		j++;
		i++;
	}
	str[j] = '\0';
	return (str);
}

void	clean_node(t_list **stash1)
{
	char	*rest;
	t_list	*tmp;

	while (*stash1 != NULL && (find_newline((*stash1)->content)) == -1)
	{
		tmp = (*stash1)->next;
		free((*stash1)->content);
		free((*stash1));
		*stash1 = tmp;
	}
	if (*stash1 == NULL)
		return ;
	rest = after_newline((*stash1)->content);
	free((*stash1)->content);
	(*stash1)->content = rest;
	if (rest == NULL || rest[0] == '\0')
	{
		tmp = (*stash1)->next;
		free((*stash1)->content);
		free((*stash1));
		*stash1 = tmp;
	}
}

char	*get_next_line(int fd)
{
	static t_list	*stash;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (fill_stash(fd, &stash) == -1)
		return (NULL);
	if (stash == NULL)
		return (NULL);
	line = list_copy(stash);
	if (line == NULL)
	{
		ft_clear(&stash);
		return (NULL);
	}
	clean_node(&stash);
	return (line);
}

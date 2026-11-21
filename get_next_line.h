/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbendaou <mbendaou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/16 09:47:54 by mbendaou          #+#    #+#             */
/*   Updated: 2026/11/21 11:41:02 by mbendaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 2003
# endif

# include <stdlib.h>
# include <unistd.h>

typedef struct s_list
{
	char			*content;
	struct s_list	*next;
}					t_list;

char				*get_next_line(int fd);
int					fill_stash(int fd, t_list **stash1);
char				*list_copy(t_list *lst);
char				*after_newline(char *s);
void				clean_node(t_list **stash1);
t_list				*ft_lst_new(char *content);
void				ft_add_back(t_list **lst, t_list *new);
void				ft_clear(t_list **lst);
int					line_len(t_list *lst);
int					find_newline(char *s);
#endif

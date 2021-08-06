/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 14:03:26 by fcadet            #+#    #+#             */
/*   Updated: 2021/08/04 13:55:24 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

void	close_in_out(int in, int out)
{
	if (in != -1)
		close(in);
	if (out != -1)
		close(out);
}

void	error(char *title, char *str, int ret, t_bool fat)
{
	write(2, "pipex: ", 7);
	if (title)
	{
		write(2, title, str_len(title));
		write(2, ": ", 2);
	}
	write(2, str, str_len(str));
	write(2, "\n", 1);
	if (fat)
		exit(ret);
}

int	ass_ret(int *dst, int src)
{
	*dst = src;
	return (src);
}

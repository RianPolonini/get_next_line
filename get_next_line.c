/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfaria-p <rfaria-p@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 14:06:36 by rfaria-p          #+#    #+#             */
/*   Updated: 2025/02/06 06:49:25 by rfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*read_until_newline(int fd, char *stored_data)
{
	char	*buffer;
	char	*temp;
	ssize_t	bytes;

	if (!stored_data)
		stored_data = ft_strdup("");
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (free(stored_data), NULL);
	bytes = read(fd, buffer, BUFFER_SIZE);
	while (bytes > 0)
	{
		buffer[bytes] = '\0';
		temp = stored_data;
		stored_data = ft_strjoin(stored_data, buffer);
		free(temp);
		if (ft_strchr(stored_data, '\n'))
			break ;
		bytes = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	if (bytes == -1)
		return (free(stored_data), NULL);
	return (stored_data);
}

char	*extract_line(char **stored_data)
{
	int		i;
	char	*line;
	char	*remaining_data;

	i = 0;
	if (!(*stored_data)[i])
		return (NULL);
	while ((*stored_data)[i] && (*stored_data)[i] != '\n')
		i++;
	line = ft_substr(*stored_data, 0, i + ((*stored_data)[i] == '\n'));
	if ((*stored_data)[i] == '\n')
		remaining_data = ft_strdup(*stored_data + i + 1);
	else
		remaining_data = NULL;
	free(*stored_data);
	*stored_data = remaining_data;
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*stored_data;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stored_data = read_until_newline(fd, stored_data);
	if (!stored_data)
		return (NULL);
	return (extract_line(&stored_data));
}

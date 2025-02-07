/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfaria-p <rfaria-p@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 14:06:36 by rfaria-p          #+#    #+#             */
/*   Updated: 2025/02/07 09:57:17 by rfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*read_and_store(int fd, char *stored_data, char *buffer)
{
	ssize_t	bytes_read;
	char	*temp;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		if (!stored_data)
			stored_data = ft_strdup("");
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			free(stored_data);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		temp = stored_data;
		stored_data = ft_strjoin(stored_data, buffer);
		free(temp);
		if (ft_strchr(stored_data, '\n'))
			break ;
	}
	free(buffer);
	return (stored_data);
}

char	*extract_next_line(char **stored_data)
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
	char		*buffer;
	static char	*stored_data;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	stored_data = read_and_store(fd, stored_data, buffer);
	if (!stored_data)
		return (NULL);
	line = extract_next_line(&stored_data);
	if (!line && stored_data)
	{
		free(stored_data);
		stored_data = NULL;
	}
	return (line);
}
/*
#include <stdio.h>
int main(void)
{
    int fd = open("test.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("Erro ao abrir o arquivo");
        return (1);
    }

    char *line;
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }

    close(fd);
    return (0);
}
*/
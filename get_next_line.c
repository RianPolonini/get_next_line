/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfaria-p <rfaria-p@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 14:06:36 by rfaria-p          #+#    #+#             */
/*   Updated: 2025/02/07 09:41:39 by rfaria-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*fill_line(int fd, char *backup, char *buffer)
{
	ssize_t	bytes;
	char	*temp;

	bytes = 1;
	while (bytes > 0)
	{
		if (!backup)
			backup = ft_strdup("");
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1)
		{
			free(buffer);
			free(backup);
			return (NULL);
		}
		buffer[bytes] = '\0';
		temp = backup;
		backup = ft_strjoin(backup, buffer);
		free(temp);
		if (ft_strchr(backup, '\n'))
			break ;
	}
	free(buffer);
	return (backup);
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
	char		*buffer;
	static char	*backup;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	backup = fill_line(fd, backup, buffer);
	if (!backup)
		return (NULL);
	line = extract_line(&backup);
	if (!line && backup)
	{
		free(backup);
		backup = NULL;
	}
	return (line);
}

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
// gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c -o test_get_next_line
// valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./test_get_next_line
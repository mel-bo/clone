/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 15:13:31 by mel-bout          #+#    #+#             */
/*   Updated: 2025/05/23 17:53:40 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	check_char(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_isdigit(char	*arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!check_char(arg[i]))
			return (1);
		i++;
	}
	return (0);
}

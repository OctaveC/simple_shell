#include "shell-header.h"

/**
 * exit_blt - exits the program
 * @prm: our structure, containing various parameters
 */
void exit_blt(prm_t *prm)
{
	int ite2 = 0, ite3 = 0;
	int status = prm->status;

	if (prm == NULL)
	{
		return;
	}
	if (prm->token_array[1] != NULL)
	{
		while (prm->token_array[1][ite3] != '\0')
		{
			if (prm->token_array[1][ite3] >= '0' && prm->token_array[1][ite3] <= '9')
				;
			else
			{
				error_handler_cd(prm, "Illegal number: ");
				prm->status = 2;
				return;
			}
			ite3++;
		}
		status = _atoi(prm->token_array[1]);
		if (status < 0)
		{
			prm->status = 2;
			return;
		}
	}
	while (prm->token_array[ite2] != NULL)
	{
		free(prm->token_array[ite2]);
		ite2++;
	}
	free_list(prm->head);
	free(prm->token_array);
	free(prm);
	exit(status);
}

/**
 * env_blt - displays the environement
 * @prm: our structure, containing various parameters
 */
void env_blt(prm_t *prm __attribute__((unused)))
{
	print_list(prm->head);
}

/**
 * setenv_blt - adds an environemental variable to the environement
 * @prm: our structure, containing various parameters
 */
void setenv_blt(prm_t *prm)
{
	char *str;
	int ite = 0, index = 0;
	list_t *h = prm->head;
	char *name2;

	if (prm->token_array[1] == NULL || prm->token_array[2] == NULL)
	{	perror(prm->name);
		return;
	}
	while (prm->token_array[1][ite] != '\0')
	{
		if (prm->token_array[1][ite] == '=')
		{	perror(prm->name);
			return;
		}
		ite++;
	}
	name2 = _calloc(sizeof(char), (_strlen(prm->token_array[1]) +
				       _strlen(prm->token_array[2]) + 2));
	if (name2 == NULL)
	{	perror(prm->name);
		return;
	}
	_strcat(name2, prm->token_array[1]), _strcat(name2, "=");
	_strcat(name2, prm->token_array[2]);
	str = _getenvnode(prm, prm->token_array[1]);
	if (str == NULL)
		add_node_end(&prm->head, name2);
	else if (prm->token_array[2] != NULL)
	{
		while (h->str != str)
		{	h = h->next;
			index++;
		}
		delete_node_at_index(&prm->head, index);
		insert_node_at_index(&prm->head, index, name2);
		free(name2);
	}
}

/**
 * unsetenv_blt - deletes an environemental variable from the environement
 * @prm: our structure, containing various parameters
 */
void unsetenv_blt(prm_t *prm)
{
	char *str;
	int pos = 0, ite2 = 0;
	list_t *h = prm->head;

	if (prm->token_array[1] == NULL)
	{
		perror(prm->name);
		return;
	}

	while (prm->token_array[1][ite2] != '\0')
	{
		if (prm->token_array[1][ite2] == '=')
		{
			perror(prm->name);
			return;
		}
		ite2++;
	}

	str = _getenvnode(prm, prm->token_array[1]);
	if (str == NULL)
	{
		perror(prm->name);
		return;
	}

	while (h->str != str)
	{
		h = h->next;
		pos++;
	}

	delete_node_at_index(&prm->head, pos);
}

/**
 * cd_blt - adds an environemental variable to the environement
 * @prm: our structure, containing various parameters
 */
void cd_blt(prm_t *prm)
{
	int chdir_return = 0;
	char buffer_cwd[500], *tmp;

	if (prm->token_array[1] == NULL || prm->token_array[1][0] == '~')
	{
		if (_getenvvalue(prm, "HOME") != NULL)
		{
			tmp =  getcwd(buffer_cwd, 500);
			chdir_return = chdir(_getenvvalue(prm, "HOME"));
		}
		else if (prm->token_array[1] == NULL)
			return;
		else if (prm->token_array[1][0] == '~')
			chdir_return = -1;
	}
	else if (prm->token_array[1][0] == '-' && !prm->token_array[1][1])
	{
		if (_getenvvalue(prm, "OLDPWD") == NULL)
		{       tmp =  getcwd(buffer_cwd, 500);
			_puts(getcwd(buffer_cwd, 500));
			chdir_return = 0;
		}
		else
		{       tmp =  getcwd(buffer_cwd, 500);
			chdir_return = chdir(_getenvvalue(prm, "OLDPWD"));
			_puts(getcwd(buffer_cwd, 500));
		}
	}
	else
	{
		tmp =  getcwd(buffer_cwd, 500);
		chdir_return = chdir(prm->token_array[1]);
	}
	if (chdir_return == -1)
	{
		error_handler_cd(prm, "can't cd to ");
		return;
	}
	_setenv("OLDPWD", tmp, prm);
	_setenv("PWD", getcwd(buffer_cwd, 500), prm);
}

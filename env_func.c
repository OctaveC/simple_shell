#include "shell-header.h"

/**
 * _getenvvalue - Search in the environment for a given variable.
 * @prm: our structure, contaning various useful parameters
 * @name: name of the variable to search.
 *
 * Return: The value of the variable.
 */
char *_getenvvalue(prm_t *prm, char *name)
{
	char *envar, *tmp, *envar_return = NULL;
	int ite = 0;
	list_t *h;

	h = prm->head;
	envar = NULL;
	while (h != NULL)
	{
		tmp = h->str;
		for (ite = 0; tmp[ite] == name[ite];)
		{
			ite++;
			if (tmp[ite] == '=' && name[ite] == '\0')
				envar = tmp;
		}
		h = h->next;
	}

	envar_return = _strchr(envar, '=');
	if (envar_return == NULL || envar_return + 1 == NULL)
		return ("");

	return (envar_return + 1);
}

/**
 * _getenvnode - Search in the environment for a given variable.
 * @prm: our structure, contaning various useful parameters
 * @name: name of the variable to search.
 *
 * Return: The environemental variable itself.
 */
char *_getenvnode(prm_t *prm, char *name)
{
	char *envar, *tmp;
	int ite = 0;
	list_t *h;

	h = prm->head;
	envar = NULL;
	while (h != NULL)
	{
		tmp = h->str;
		for (ite = 0; tmp[ite] == name[ite];)
		{
			ite++;
			if (tmp[ite] == '=' && name[ite] == '\0')
				envar = tmp;
		}
		h = h->next;
	}

	return (envar);
}

/**
 * _setenv - Add or modify an environment variable.
 * @name: name of the variable.
 * @value: value of the variable.
 * @prm: the pointer of our structure.
 */
void _setenv(char *name, char *value, prm_t *prm)
{
	char *str;
	int ite = 0;
	char *name2 = _calloc(100, sizeof(char));

	if (name2 == NULL)
	{
		perror(prm->name);
		return;
	}

	if (name == NULL || value == NULL)
	{
		perror(prm->name);
		return;
	}
	while (name[ite] != '\0')
	{
		if (name[ite] == '=')
		{
			perror(prm->name);
			return;
		}
		ite++;
	}

	_strcat(name2, name);
	_strcat(name2, "=");
	_strcat(name2, value);

	str = _getenvnode(prm, name);

	if (str == NULL)
	{
		add_node_end(&prm->head, name2);
	}
	else if (value != NULL)
	{
		_unsetenv(name, prm);
		add_node_end(&prm->head, name2);
	}
}

/**
 * _unsetenv - Remove an environment variable.
 * @name: name of the variable.
 * @prm: the pointer of our structure.
 */
void _unsetenv(char *name, prm_t *prm)
{
	char *str;
	int pos = 0, ite2 = 0;
	list_t *h = prm->head;

	if (name == NULL)
	{
		perror(prm->name);
		return;
	}
	while (name[ite2] != '\0')
	{
		if (name[ite2] == '=')
		{
			perror(prm->name), exit(0);
			return;
		}
		ite2++;
	}

	str = _getenvnode(prm, name);

	while (h->str != str)
	{
		h = h->next;
		pos++;
	}

	delete_nodeint_at_index(&prm->head, pos);
}

/**
 * env_list - Turns the environ array into a linked list
 * @head: The head of the list
 *
 * Return: The address of the new element, or NULL if it failed
 */
list_t *env_list(list_t *head)
{
	int ite = 0;

	while (environ[ite] != NULL)
	{
		createNodeList(&head, environ[ite]);
		ite++;
	}

	return (head);
}

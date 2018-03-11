#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct dir{
	char *s;
	struct dir *next;
} PATHDIR;
/**
 * _strlength - finds length of string
 *@s: string to find length of
 *
 *Return: length. Otherwise 0.
 */
int _strlength(char *s)
{
	unsigned int i = 0;

	for (i = 0; s[i] != '\0'; i++)
		;

	return (i);
}
/**
 * _getenv - finds the environment that matches the name
 *@name: name to look for in environment
 *
 *Return: environ. Otherwise 0.
 */
char *_getenv(const char *name)
{
	extern char **environ;
	char *s, *finalpath;
	unsigned int i = 0, j = 0, x = 0;


	while (environ[i] != NULL)
	{
		if (name[j] == *environ[i])
		{
			while (name[j] != '\0')
			{
				if (name[j] != environ[i][x])
				{
					j = 0;
					break;
				}
				j++;
				x++;
			}
			x = 0;
			if (name[j] == '\0')
			{
				s = environ[i];
				i = 0;

				x = _strlength(s);

				for(;s[i] != '='; x--, i++)
					;

				i++;
				x--;

				finalpath = malloc(sizeof(char *) * x);

				if (finalpath == NULL)
					return (0);

				for(x = 0;s[i] != '\0'; x++, i++)
					finalpath[x] = s[i];

				return (finalpath);

			}
		}

		j = 0;
		i++;
	}
	return (0);
}
/**
 * findpath - prints each directory onto its own line
 *@fullpath - full path that needs to be split
 *
 *Return: linked list of path
 */
PATHDIR *findpath(char *path)
{
	PATHDIR *head, *temp, *temp2;
	char *token = NULL, *delim = ":";

	head = malloc(sizeof(PATHDIR));
	if (head == NULL)
		return (0);

	head->next = NULL;

	token = strtok(path, delim);

	head->s = token;

	temp = head;
	while (token != NULL)
	{
		token = strtok(NULL, delim);

		if (token != NULL)
		{
			temp2 = malloc(sizeof(PATHDIR));

			if (temp2 == NULL)
				return (0);

			temp2->s = token;
			temp->next = NULL;
			temp->next = temp2;
			temp = temp2;
		}
	}
	return (head);
}
/**
 * main - provides name to look for
 *
 *Return: 0
 */
int main(char *argv)
{
	const char *name = "PATH";
	char *pathdir, *prompt = "($)", *commandtoks, *delim = ":", **storetoken;
	unsigned int i = 0;
	PATHDIR *linklist;
	char *commandinput;
	size_t size = 0;
	pid_t childpid;
	const char *hold = "/usr/local/bin";

	while (1)
	{
		write(STDOUT_FILENO, prompt, 3);

		getline(&commandinput, &size, stdin);
		printf("%s", commandinput);
		commandtoks = strtok(commandinput, delim);
		storetoken[i] = commandtoks;
		i++;
		while (commandtoks != NULL)
		{

			commandtoks = strtok(NULL, delim);
			storetoken[i] = commandtoks;
			i++;
		}

		pathdir = _getenv(name);


/** create childPID so we are able to execute the command input while running this program*/
/**		childpid = fork();
		if (childpid = -1)
		return (0);
		if (childpid = 0)
		{

		printf("Inside childprocess\n");
		}
		else
		wait(NULL);*/


	}
	return (0);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

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
 * leaveshell - exits the shell program
 * @
 *
 *Return: void
 */
void leaveshell()
{
	exit(100);
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

				for(;s[i] != '='; x--, i++) /** see where the equal sign is past PATH*/
					;

				i++;
				x--;

				finalpath = malloc(sizeof(char *) * x); /** finalpath is the path of directories without the PATH= */

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
			temp2->next = NULL;
			temp->next = temp2;
			temp = temp2;
		}
	}
	return (head);
}

/**
 * searchcommand - finds the command in the path
 *@comm: first command inserted
 *@path: linked list of the path directories
 *
 *Return: the full path. Othwewise 0
 */
char *searchcommand(PATHDIR *path, char *comm)
{
	char *buff, *string;
	unsigned int commlength, i, commit, strlength = 0;
	struct stat st;

	commlength = _strlength(comm);

	while(path != NULL)
	{

		commit = 0;
		i = 0;
		string = path->s;

		strlength = _strlength(string);

		buff = malloc(sizeof(char *) * (strlength + commlength + 2));

		if (buff == NULL)
			return (0);
		while (string[i] != '\0')
		{
			buff[i] = string[i];
			i++;
		}
		buff[i++] = '/';

		while(comm[commit] != '\0')
		{
			buff[i] = comm[commit];
			i++;
			commit++;

		}
		buff[i] = '\0';

		if (stat(buff, &st) == 0) /** success */
		{
			return (buff);
		}

		free(buff);

		path = path->next;
	}
	return (comm);
}

/**
 * main - provides name to look for
 *
 *Return: 0
 */
int main(void)
{
	char *name = "PATH";
	char *pathdir, *prompt = "($)", *commandtoks, *delim = " \n", **storetoken = NULL;
	unsigned int i = 0, readline = 0, y = 0, spacecounter = 1;
	PATHDIR *head = NULL;
	char *commandinput = NULL, *commpath = NULL;
	size_t size = 0;
	pid_t childpid;

	while (1)
	{
		write(STDOUT_FILENO, prompt, 3);

		i = 0;


		readline = getline(&commandinput, &size, stdin);
		while(commandinput[y] != '\0')
		{

			if (commandinput[y] == ' ')
				spacecounter++;

			y++;
		}

		storetoken = malloc((1 + spacecounter) * sizeof(char *));

		if (storetoken == NULL)
			return (0);

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

		head = findpath(pathdir);


		commpath = searchcommand(head, storetoken[0]);

		childpid = fork();

		if (childpid == 0)
		{

			execve(storetoken[0], storetoken, NULL);

			execve(commpath, storetoken, NULL);
			free(storetoken);
			exit(98);
		}
		else
			wait(NULL);

	}
	return (0);
}

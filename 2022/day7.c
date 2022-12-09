#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define NAME_SIZE  256
#define COMMAND_SIZE 3

struct folder
{
	char name[NAME_SIZE];
	int size;

	struct file
	{
		char name[NAME_SIZE];
		int size;

		struct file *next;
	} *fileList;

	struct folder *folderList;

	struct folder *next;
	struct folder *parent;
};

void makeNextFolder(struct folder *node, char *folderName)
{
	node->next = malloc(sizeof(struct folder));
	node = node->next;

	strcpy(node->name, folderName);
	node->size = 0;
	node->fileList = NULL;
	node->folderList = NULL;
	node->next = NULL;
	node->parent = NULL;
}

struct folder *makeNewFolder(struct folder *folder, char *folderName)
{
	struct folder *node;

	node = folder->folderList;

	if (node == NULL)
	{
		folder->folderList = malloc(sizeof(struct folder));
		node = folder->folderList;

		strcpy(node->name, folderName);
		node->size = 0;
		node->fileList = NULL;
		node->folderList = NULL;
		node->next = NULL;
		node->parent = folder;

		return node;
	}

	for (;; node = node->next)
		if (node->next == NULL)
		{
			makeNextFolder(node, folderName);
			node->next->parent = folder;

			return node->next;
		}
}

void makeNewFile(struct folder *folder, char *fileName, int fileSize)
{
	struct file *node;

	node = folder->fileList;

	if (node == NULL)
	{
		folder->fileList = malloc(sizeof(struct file));

		strcpy(folder->fileList->name, fileName);
		folder->fileList->size = fileSize;
		folder->fileList->next = NULL;

		folder->size += fileSize;

		return;
	}

	for (;; node = node->next)
		if (node->next == NULL)
		{
			node->next = malloc(sizeof(struct file));
			node = node->next;

			strcpy(node->name, fileName);
			node->size = fileSize;
			node->next = NULL;

			folder->size += fileSize;

			return;
		}
}

void printFolder(struct folder *folder, int depth)
{
	struct folder *node;
	struct file *file;
	int i;

	for (i = 0; i < depth; ++i)
		printf(".\t");

	printf("/%s\tTotal size: %d\n", folder->name, folder->size);

	++depth;

	for (file = folder->fileList; file != NULL; file = file->next)
	{
		for (i = 0; i < depth; ++i)
			printf(".\t");

		printf("%d\t%s\n", file->size, file->name);
	}

	for (node = folder->folderList; node != NULL; node = node->next)
		printFolder(node, depth);
}

int folderSize(struct folder *folder, int *count)
{
	struct folder *node;
	int size;

	size = folder->size;

	for (node = folder->folderList; node != NULL; node = node->next)
		size += folderSize(node, count);

	folder->size = size;

	// this is part a
	/*
	if (size < 100000)
		*count += size;
	*/
	// this is part b
	if (size > 4274331 && size < *count)
		*count = size;

	return size;
}

int main(void)
{
	FILE *file;
	int c,
	    fileSize,
	    depth, i,
	    sum = 0;
	char command[COMMAND_SIZE],
	     dir[NAME_SIZE],
	     fileName[NAME_SIZE];
	struct folder root, *node;

	file = fopen("day7_input.txt", "r");

	node = &root;

	// initialize the root directory
	root.name[0] = '\0';
	root.size = 0;
	root.fileList = NULL;
	root.folderList = NULL;
	root.next = NULL;
	root.parent = NULL;

	command[2] = '\0';
	c = getc(file);

	for (; c != EOF; c = getc(file))
	{
		if (c == '$')
		{
			// skip space
			c = getc(file);

			command[0] = getc(file);
			command[1] = getc(file);

			// change directory command
			if (strcmp(command, "cd") == 0)
			{
				printf("cd'ing\n");

				// skip space
				c = getc(file);

				// get first character of directory
				c = getc(file);

				for (i = 0; c != '\n'; c = getc(file))
				{
					// indicate a new path
					dir[i++] = c;
				}

				// dir now holds the directory name
				dir[i] = '\0';

				if (strcmp(dir, "..") == 0 && node->parent != NULL)
				{
					// move up to parent folder
					node = node->parent;
				} else
					// create and move into this folder
					node = makeNewFolder(node, dir);
			}

			// list directory command
			if (strcmp(command, "ls") == 0)
			{
				printf("ls'ing\n");

				// skip new line
				c = getc(file);

				// get first character
				c = getc(file);

				// read lines so long as the first character is a number (a file listing)
				for (;; c = getc(file))
				{
					// file
					if (isdigit(c))
					{
						ungetc(c, file);

						fscanf(file, "%d %s\n", &fileSize, fileName);
						makeNewFile(node, fileName, fileSize);
					} else
					// folder; skip this. folder creation is handled with cd
					if (isalpha(c))
					{
						c = getc(file);

						for (; c != '\n'; c = getc(file))
							;
					} else
						break;
				}

				// put back the character that was not for a file listing
				ungetc(c, file);
			}
		} else {
			printf("This should never be printed. If it was, consider the program input's validity.\n");
		}
	}

	// part a
	// sum = 0;
	// part b
	sum = 70000000;

	folderSize(&root, &sum);
	printFolder(&root, 0);

	// part a
	// printf("The sum for part a is %d.\n", sum);
	// part b
	printf("The smallest directory that will free enough space is of size %d.\n", sum);

	return 0;
}

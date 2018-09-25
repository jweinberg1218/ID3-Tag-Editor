// Jason Weinberg

#include <stdio.h>
#include <string.h>

struct id3tag {
	char tagIdentifier[3];
	char title[30];
	char artist[30];
	char album[30];
	char year[4];
	char comment[28];
	char zeroByteSeparator;
	char track;
	char genre;
};

int main(int argc, char *argv[])
{
	char *filename;
	char *field;
	char *value;

	filename = argv[1];

	FILE *f = NULL;
	f = fopen(filename, "r+");

	if(f == NULL)
	{
		printf("Failed to open the file specified.");
		return 0;
	}

	struct id3tag tag;
	getTag(f, &tag);

	if(argc == 2)
	{
		printFields(&tag);
	}
	else if(argc >= 4 && argc % 2 == 0)
	{
		printf("hit");

		int i;
		for(i = 2; i < argc; i += 2)
		{
			field = argv[i];
			value = argv[i + 1];
			setFields(&tag, field, value);
		}

		int offset;
		if(strcmp(tag.tagIdentifier, "TAG") == 0)
		{
			offset = -128;
		}
		else
		{
			offset = 0;
		}

		writeTag(f, &tag, offset);
	}
	else
	{
		printf("Please enter a valid number of arguments.");
	}

	fclose(f);

	return 0;
}

int printFields(struct id3tag *tag)
{
	if(!strcmp(tag->tagIdentifier, "TAG"))
	{
		printf("File does not contain valid ID3 tag.");
		return 0;
	}

	printf("Title: %.*s\n", 30, tag->title);
	printf("Artist: %.*s\n", 30, tag->artist);
	printf("Album: %.*s\n", 30, tag->album);
	printf("Year: %.*s\n", 4, tag->year);
	printf("Comment: %.*s\n", 28, tag->comment);
	printf("Track: %d\n", tag->track);
	printf("Genre: %d\n", tag->genre);

	return 0;
}

int setFields(struct id3tag *tag, char *field, char *value)
{
	if(strcmp(field, "-title") == 0)
	{
		strncpy(tag->title, value, 30);
	}
	else if(strcmp(field, "-artist") == 0)
	{
		strncpy(tag->artist, value, 30);
	}
	else if(strcmp(field, "-album") == 0)
	{
		strncpy(tag->album, value, 30);
	}
	else if(strcmp(field, "-year") == 0)
	{
		strncpy(tag->year, value, 4);
	}
	else if(strcmp(field, "-comment") == 0)
	{
		strncpy(tag->comment, value, 28);
	}
	else if(strcmp(field, "-track") == 0)
	{
		strncpy(tag->track, value, 1);
	}
	else
	{
		printf("Invalid field argument.");
	}

	return 0;
}

int getTag(FILE *f, struct id3tag *tag)
{
	fseek(f, -128, SEEK_END);
	fread(tag, sizeof(*tag), 1, f);

	return 0;
}

int writeTag(FILE *f, struct id3tag *tag, int offset)
{
	fseek(f, offset, SEEK_END);
	fwrite(tag, sizeof(*tag), 1, f);

	return 0;
}

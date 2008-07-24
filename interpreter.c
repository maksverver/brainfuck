#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *fp;
	char code[4096], *ip = code, *eof;
	unsigned char data[4096] = { }, *dp = data;
	int n;
 
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <program>\n", argv[0]);
		return 1;
	}

	fp = fopen(argv[1], "rt");
	if (!fp)
	{
		fprintf(stderr, "Could not open \"%s\"!\n");
		return 1;
	}
	eof = code + fread(code, 1, sizeof(code), fp);
	fclose(fp);

	while (ip != eof)
	{
		switch (*ip++)
		{
		case '+': ++*dp; break;
		case '-': --*dp; break;
		case '>': ++dp; break;
		case '<': --dp; break;
		case ',':
			if ((n = getchar()) >= 0)
				*dp = n;
			break;
		case '.':
			putchar(*dp);
			fflush(stdout);
			break;
		case '[':
			if (!*dp)
				for (n = 1; n; ++ip) 
					n += (*ip == '[') - (*ip == ']');
			break;
		case ']': 
			if (*dp)
			{
				ip -= 2;
				for (n = 1; n; --ip) 
					n += (*ip == ']') - (*ip == '[');
				ip += 2;
			}
			break;
		case '!':
			printf("%04x  ", ip - code);
			for (n = 0; n < 32; ++n)
				printf("%c%02x", (dp == &data[n]) ? '>' : ' ' , data[n]);
			printf("\n");
			break;
		}
	}

	return 0;
}

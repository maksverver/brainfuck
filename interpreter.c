/* A trivial Brainfuck interpreter in C.
   Maks Verver <maksverver@geocities.com> May 2010 */

#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    char code[65536], *ip = code, *eof;
    unsigned char data[65536] = { }, *dp = data;
    int n;

    fp = argc > 1 ? fopen(argv[1], "rt") : stdin;
    if (!fp)
    {
        fprintf(stderr, "Could not open \"%s\"!\n", argv[0]);
        return 1;
    }
    eof = code + fread(code, 1, sizeof(code), fp);
    if (fp != stdin) fclose(fp);

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
            printf("%04x  ", (int)(ip - code));
            for (n = 0; n < 32; ++n)
                printf("%c%02x", (dp == &data[n]) ? '>' : ' ' , data[n]);
            printf("\n");
            break;
        }
    }

    return 0;
}

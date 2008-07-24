#include <stdio.h>
#include <string.h>
#include <stdarg.h>

int level = 0;

void print(char *format, ...)
{
	va_list ap;
	int n;

	for (n = 0; n < level; ++n)
		printf("\t");
	
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
	putchar('\n');
}

int main()
{
	int c, d, move = 0;
	print("#include <stdio.h>\n");
	print("unsigned char data[65536], *p = data;\n");
	print("void in() {");
	++level;
	print("int c = getchar();");
	print("if (c != EOF) *p = (unsigned char)c;");
	--level;
	print("}\n");
	print("void out() {");
	++level;
	print("putchar(*p);");
	print("fflush(stdout);");
	--level;
	print("}\n");
	print("int main() {");
	++level;
	while ((c = getchar()) != EOF)
	{
		int n = 1;
		for (;;) {
			d = getchar();
			if (c == d)
			{
				++n;
				continue;
			}
			if (d == EOF || strchr("<>+-.,[]", d))
			{
				ungetc(d, stdin);
				break;
			}
		}

		if (strchr("<>",c))
		{
			move += c == '<' ? -n : +n;
			if (!strchr("<>", d))
			{
				if (move > 0) print("p+=%d;", move);
				if (move < 0) print("p-=%d;", -move);
				move = 0;
			}
		}
		else
		if (c == '+' || c == '-')
			print("*p%c=%d;", c, n);
		else
		if (c == '.' || c == ',')
		{
			while (n--)
				print("%s();", c == '.' ? "out" : "in");
		}
		else
		if (c == '[')
		{
			while (n--)
				print("while(*p) {"), ++level;
		}
		else
		if (c == ']')
		{
			while (n--)
				--level, print("}");
		}
	}
	print("return 0;");
	--level;
	print("}");
	return 0;
}

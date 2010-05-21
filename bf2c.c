/* A simple C program to generate C code from a Brainfuck program.

  Uses a straightforward correspondence between Brainfuck operations and C
  language constructs, except that consecutive identical operations are
  collapsed to reduce the output size and required compilation time.

  Brainfuck implementation properties:
    - tape consists of 65536 characters (usually 8-bit bytes)
    - tape head starts at the left end of the tape
    - when reading fails (at end of file) the destination cell is unchanged

  Maks Verver <maksverver@geocities.com> revised May 2010 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static int indent = 0;

void print(char *format, ...)
{
	int i = indent;
	va_list ap;

	while (i--) putchar('\t');
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
	putchar('\n');
}

int main()
{
	int c, d, n;

	print("#include <stdio.h>\n");
	print("static char t[65536], *p = t;\n");
	print("static void in(void) {");
	++indent;
	print("int c = getchar();");
	print("if (c != EOF) *p = (char)c;");
	--indent;
	print("}\n");
	print("static void out(void) {");
	++indent;
	print("putchar(*p);");
	print("fflush(stdout);");
	--indent;
	print("}\n");
	print("int main() {");
	++indent;
	for (c = getchar(); c != EOF; c = d)
	{
		n = 1;
		while ((d = getchar()) == c) ++n;
		switch (c) {
		case '>': print("p+=%d;", n); break;
		case '<': print("p-=%d;", n); break;
		case '+': print("*p+=%d;", n); break;
		case '-': print("*p-=%d;", n); break;
		case ',': while (n--) print("in();"); break;
		case '.': while (n--) print("out();"); break;
		case '[': while (n--) print("while(*p) {"), ++indent; break;
		case ']': while (n--) --indent, print("}"); break;
		}
	}
	print("return 0;");
	--indent;
	print("}");
	return 0;
}

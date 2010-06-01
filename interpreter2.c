/* A Brainfuck bytecode interpreter in ~100 lines of efficient C code.
   Maks Verver <maksverver@geocities.com> May 2010 */

#define TAPE_SIZE   (1<<20)     /* number of cells on tape */
#define CODE_SIZE   (1<<16)     /* maximum number of opcodes */

#define CHECK(head) /* bounds checking -- remove line below to disable */ \
    if ((size_t)(head - tape) >= TAPE_SIZE) fatal("tape bounds exceeded")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char tape[TAPE_SIZE];
static int op[CODE_SIZE];
static int loop = -1, ops;   /* used during code generation */

enum OpCodes { RET, ADD, MOVE, JUMP, CALL };
enum FuncNos { READ, WRITE };

static void fatal(const char *msg) {
    fprintf(stderr, "Fatal error: %s!\n", msg);
    exit(1);
}

static void emit(int o, int a) {
    op[ops++] = o | (a << 4);
}

static void finish_move(int *move) {
    if (*move) emit(MOVE, *move), *move = 0;
}

static void finish_add(int *add) {
    if (*add) emit(ADD, *add), *add = 0;
}

static void finish(int *move, int *add) {
    finish_move(move);
    finish_add(add);
}

static void emit_loop_begin(void) {
    op[ops] = loop;
    loop = ops++;
}

static void emit_loop_end(void) {
    int target = loop;
    if (target == -1) fatal("unmatched closing brace in input");
    loop = op[target];
    op[target] = JUMP | ((ops - target) << 4);
    emit(JUMP, target - ops);
}

static void parse(FILE *fp) {
    int move = 0, add = 0;
    for (;;) {
        switch (getc(fp)) {
        case '+': finish_move(&move); add += 1; break;
        case '-': finish_move(&move); add -= 1; break;
        case '>': finish_add(&add); ++move; break;
        case '<': finish_add(&add); --move; break;
        case ',': finish(&move, &add); emit(CALL, READ); break;
        case '.': finish(&move, &add); emit(CALL, WRITE); break;
        case '[': finish(&move, &add); emit_loop_begin(); break;
        case ']': finish(&move, &add); emit_loop_end(); break;
        case EOF:
            finish(&move, &add);
            if (loop != -1) fatal("unmatched opening brace in input");
            return;
        }
        if (ops == CODE_SIZE) fatal("too much code");
    }
}

static void execute(void) {
    int *ip;
    char *head;
    for (ip = op, head = tape; *ip != RET; ++ip) {
        int a = *ip >> 4;
        switch (*ip & 15) {
        case ADD: *head += a; break;
        case MOVE: head += a; CHECK(head); break;
        case JUMP: if ((a > 0) ^ (*head != 0)) ip += a; break;
        case CALL: switch (a) {
            case READ:  if ((a = getc(stdin)) != EOF) *head = a; break;
            case WRITE: putc(*head, stdout); break;
            } break;
        }
    }
}

int main(int argc, char *argv[])
{
    FILE *fp;
    if (argc < 2) fatal("missing Brainfuck file argument");
    if (argc > 2) fatal("too many arguments");
    fp = fopen(argv[1], "r");
    if (fp == NULL) fatal("could not open input file");
    parse(fp);
    fclose(fp);
    execute();
    return 0;
}

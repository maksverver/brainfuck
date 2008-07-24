#include <elf.h>
#include <stdio.h>
#include <stdlib.h>

char lib[] = {
/* getchar: */
    0x50,                               /* push   %eax                    */
    0x89, 0xc1,                         /* mov    %eax,%ecx               */
    0xba, 0x01, 0x00, 0x00, 0x00,       /* mov    $0x1,%edx               */
    0xb8, 0x03, 0x00, 0x00, 0x00,       /* mov    $0x3,%eax               */
    0xcd, 0x80,                         /* int    $0x80                   */
    0x58,                               /* pop    %eax                    */
    0xc3,                               /* ret                            */
/* putchar: */
    0x50,                               /* push   %eax                    */
    0x43,                               /* inc    %ebx                    */
    0x89, 0xc1,                         /* mov    %eax,%ecx               */
    0x89, 0xda,                         /* mov    %ebx,%edx               */
    0xb8, 0x04, 0x00, 0x00, 0x00,       /* mov    $0x4,%eax               */
    0xcd, 0x80,                         /* int    $0x80                   */
    0x4b,                               /* dec    %ebx                    */
    0x58,                               /* pop    %eax                    */
    0xc3,                               /* ret                            */
};

char init[] = {
/* _start: */
    0xb9, 0x00, 0x00, 0x10, 0x00,       /* mov    $0x100000,%ecx          */
    0x29, 0xcc,                         /* sub    %ecx,%esp               */
    0x89, 0xe5,                         /* mov    %esp,%ebp               */
    0x89, 0xe0,                         /* mov    %esp,%eax               */
    0x31, 0xdb,                         /* xor    %ebx,%ebx               */
    0x89, 0xe7,                         /* mov    %esp,%edi               */
    0x30, 0xc0,                         /* xor    %al,%al                 */
    0xf3, 0xaa,                         /* rep stos %al,%es:(%edi)        */
    0x68, 0x54, 0x00, 0x00, 0x08,       /* push   $getchar                */
    0x68, 0x65, 0x00, 0x00, 0x08,       /* push   $putchar                */
};

char finish[]= {
/* bf_end: */
    0x8b, 0x18,                         /* mov    (%eax),%ebx             */
    0xb8, 0x01, 0x00, 0x00, 0x00,       /* mov    $0x1,%eax               */
    0xcd, 0x80,                         /* int    $0x80                   */
};

#define BASE_ADDR    0x08000000

Elf32_Ehdr eh = {
    { ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3,
      ELFCLASS32, ELFDATA2LSB, EV_CURRENT, ELFOSABI_LINUX },
    ET_EXEC, EM_386, EV_CURRENT, BASE_ADDR + sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr) + sizeof(lib),
    sizeof(Elf32_Ehdr), 0, 0, sizeof(Elf32_Ehdr),
    sizeof(Elf32_Phdr), 1, sizeof(Elf32_Shdr), 0,
    SHN_UNDEF };

Elf32_Phdr ph = {
    PT_LOAD, 0, BASE_ADDR, 0,
    sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr) + sizeof(lib) + sizeof(init) + sizeof(finish),
    sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr) + sizeof(lib) + sizeof(init) + sizeof(finish),
    PF_R | PF_X, 0x0100000 };

#define MAX_LEN 65536
char program[MAX_LEN];
int addr[MAX_LEN];

int main()
{
    int len = 0, size = 0, n, m, c;
    while((c = getchar()) != EOF) {
        switch(c) {
        case '>': program[len] = 1; size += 1; addr[len] = size; ++len; break;
        case '<': program[len] = 2; size += 1; addr[len] = size; ++len; break;
        case '+': program[len] = 3; size += 2; addr[len] = size; ++len; break;
        case '-': program[len] = 4; size += 2; addr[len] = size; ++len; break;
        case ',': program[len] = 5; size += 3; addr[len] = size; ++len; break;
        case '.': program[len] = 6; size += 3; addr[len] = size; ++len; break;
        case '[': program[len] = 7; size += 8; addr[len] = size; ++len; break;
        case ']': program[len] = 8; size += 8; addr[len] = size; ++len; break;
        }
        if(len == MAX_LEN)
            abort();
    }

    ph.p_filesz += size;
    ph.p_memsz  += size;

    fwrite(&eh,   sizeof(eh),   1, stdout);
    fwrite(&ph,   sizeof(ph),   1, stdout);
    fwrite(&lib,  sizeof(lib),  1, stdout);
    fwrite(&init, sizeof(init), 1, stdout);

    for(n = 0; n < len; ++n) {
        switch(program[n]) {
        default: abort();
        case 1: putchar(0x40); break;
        case 2: putchar(0x48); break;
        case 3: putchar(0xfe); putchar(0x00); break;
        case 4: putchar(0xfe); putchar(0x08); break;
        case 5: putchar(0xff); putchar(0x55); putchar(0xfc); break;
        case 6: putchar(0xff); putchar(0x55); putchar(0xf8); break;
        case 7:
            for(m = n + 1, c = 1; c; ++m)
            {
                if(m == len)
                    abort();
                if(program[m] == 7)
                    ++c;
                else
                if(program[m] == 8)
                    --c;
            }
            c = addr[m] - addr[n];
            putchar(0x3a); putchar(0x18); putchar(0x0f); putchar(0x84);
            fwrite(&c, 4, 1, stdout);
            break;
        case 8:
            for(m = n - 1, c = 1; c; --m)
            {
                if(m < 0)
                    abort();
                if(program[m] == 7)
                    --c;
                else
                if(program[m] == 8)
                    ++c;
            }
            c = addr[m] - addr[n];
            putchar(0x3a); putchar(0x18); putchar(0x0f); putchar(0x85);
            fwrite(&c, 4, 1, stdout);
            break;
        }
    }
    fwrite(&finish, sizeof(finish), 1, stdout);
}

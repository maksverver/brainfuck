/* Structure:
    elf header              52 bytes (0x34 bytes)
    program header          32 bytes (0x20 bytes)
    prolog
    <code>
    epilog
*/

/*    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00 */
char header[84] = {
    /*  ELF identifier (16 bytes) */
    0x7f, 0x45, 0x4c, 0x46,   0x01, 0x01, 0x01, 0x09,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x03, 0x00,   0x01, 0x00, 0x00, 0x00,   0xa7, 0x80, 0x04, 0x08,   0x34, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x34, 0x00, 0x20, 0x00,   0x01, 0x00, 0x00, 0x00,
                                                                                  /* Loaded address */
    0x00, 0x00, 0x00, 0x00,   0x01, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x80, 0x04, 0x08,
    /* Physical address */    /* Length in file */      /* Length in memory */    /* Flags */
    0x00, 0x80, 0x04, 0x08,   0x78, 0x56, 0x34, 0x12,   0x78, 0x56, 0x34, 0x12,   0x07, 0x00, 0x00, 0x00,
    /* Alignment */
    0x00, 0x01, 0x00, 0x00
};

char prologue[] = {
/* fixcall: */
    0x8b, 0x1c, 0x24,                   /* mov    (%esp),%ebx             */
    0x89, 0xdf,                         /* mov    %ebx,%edi               */
    0x83, 0xef, 0x06,                   /* sub    $0x6,%edi               */
    0xb9, 0x01, 0x00, 0x00, 0x00,       /* mov    $0x1,%ecx               */
    0x4b,                               /* dec    %ebx                    */
/* repeat: */
    0x43,                               /* inc    %ebx                    */
    0x80, 0x3b, 0x90,                   /* cmpb   $0x90,(%ebx)            */
    0x75, 0xfa,                         /* jne    8048082 <repeat>        */
    0x43,                               /* inc    %ebx                    */
    0x41,                               /* inc    %ecx                    */
    0x80, 0x3b, 0x90,                   /* cmpb   $0x90,(%ebx)            */
    0x75, 0xf3,                         /* jne    8048082 <repeat>        */
    0x83, 0xe9, 0x02,                   /* sub    $0x2,%ecx               */
    0x75, 0xee,                         /* jne    8048082 <repeat>        */
    0x43,                               /* inc    %ebx                    */
    0x89, 0x1f,                         /* mov    %ebx,(%edi)             */
    0x53,                               /* push   %ebx                    */
    0xc3,                               /* ret                            */
/* putchar: */
    0x6a, 0x01,                         /* push   $0x1                    */
    0x50,                               /* push   %eax                    */
    0x6a, 0x01,                         /* push   $0x1                    */
    0x31, 0xc0,                         /* xor    %eax,%eax               */
    0x50,                               /* push   %eax                    */
    0xb0, 0x04,                         /* mov    $0x4,%al                */
    0xcd, 0x80,                         /* int    $0x80                   */
    0x8b, 0x44, 0x24, 0x08,             /* mov    0x8(%esp),%eax          */
    0x83, 0xc4, 0x10,                   /* add    $0x10,%esp              */
    0xc3,                               /* ret                            */
/* getchar: */
    0x89, 0xc3,                         /* mov    %eax,%ebx               */
    0x6a, 0x01,                         /* push   $0x1                    */
    0x50,                               /* push   %eax                    */
    0x6a, 0x00,                         /* push   $0x0                    */
    0x31, 0xc0,                         /* xor    %eax,%eax               */
    0x50,                               /* push   %eax                    */
    0xb0, 0x03,                         /* mov    $0x3,%al                */
    0xcd, 0x80,                         /* int    $0x80                   */
    0x48,                               /* dec    %eax                    */
    0x74, 0x03,                         /* je     80480c1 <read_ok>       */
    0xc6, 0x03, 0x00,                   /* movb   $0x0,(%ebx)             */
/* read_ok: */
    0x83, 0xc4, 0x10,                   /* add    $0x10,%esp              */
    0x89, 0xd8,                         /* mov    %ebx,%eax               */
    0xc3,                               /* ret                            */
/* _start: */
    0x89, 0xe5,                         /* mov    %esp,%ebp               */
    0x68, 0x79, 0x80, 0x04, 0x08,       /* push   $0x8048079              */
    0x68, 0x8d, 0x80, 0x04, 0x08,       /* push   $0x804808d              */
    0xbb, 0x00, 0x00, 0x10, 0x00,       /* mov    $0x100000,%ebx          */
    0x31, 0xc0,                         /* xor    %eax,%eax               */
    0x50,                               /* push   %eax                    */
    0x50,                               /* push   %eax                    */
    0x6a, 0xff,                         /* push   $0xffffffff             */
    0x68, 0x00, 0x10, 0x00, 0x00,       /* push   $0x1000                 */
    0x6a, 0x03,                         /* push   $0x3                    */
    0x53,                               /* push   %ebx                    */
    0x50,                               /* push   %eax                    */
    0xb0, 0xc5,                         /* mov    $0xc5,%al               */
    0x50,                               /* push   %eax                    */
    0xcd, 0x80,                         /* int    $0x80                   */
    0x83, 0xc4, 0x20,                   /* add    $0x20,%esp              */
    0x83, 0xf8, 0xff,                   /* cmp    $0xffffffff,%eax        */
    0x75, 0x07,                         /* jne    80480fb <alloc_ok>      */
    0x31, 0xc0,                         /* xor    %eax,%eax               */
    0x40,                               /* inc    %eax                    */
    0x50,                               /* push   %eax                    */
    0x50,                               /* push   %eax                    */
    0xcd, 0x80,                         /* int    $0x80                   */
/* alloc_ok: */
    0x89, 0xc7,                         /* mov    %eax,%edi               */
    0x89, 0xd9,                         /* mov    %ebx,%ecx               */
    0x30, 0xc0,                         /* xor    %al,%al                 */
    0xf3, 0xaa,                         /* repz stos %al,%es:(%edi)       */
    0x89, 0xf8,                         /* mov    %edi,%eax               */
    0xd1, 0xeb,                         /* shr    %ebx                    */
    0x29, 0xd8                          /* sub    %ebx,%eax               */
};

char epilogue[] = {
    0x31, 0xc0,                         /* xor    %eax,%eax               */
    0x50,                               /* push   %eax                    */
    0x40,                               /* inc    %eax                    */
    0x50,                               /* push   %eax                    */
    0xcd, 0x80                          /* int    $0x80                   */
};

static char buffer[1<<20];

int main()
{
    int codelen, n, len;

    len = read(0, buffer, sizeof(buffer));
    if(len < 1 || len == sizeof(buffer))
        return 1;

    codelen = sizeof(header) + sizeof(prologue) + sizeof(epilogue);

    for(n = 0; n < len; ++n)
    {
        switch(buffer[n])
        {
        case '<':
        case '>':
            codelen += 1;
            break;

        case '+':
        case '-':
            codelen += 2;
            break;

        case ',':
        case '.':
            codelen += 3;
            break;

        case '[':
            codelen += 8;
            break;

        case ']':
            codelen += 14;
            break;
        }
    }

    write(1, header, 68);
    write(1, &codelen, 4);
    write(1, &codelen, 4);
    write(1, header + 76, 8);
    write(1, prologue, sizeof(prologue));

    for(n = 0; n < len; ++n)
    {
        switch(buffer[n])
        {
        case '>':
            putchar(0x40);
            break;

        case '<':
            putchar(0x48);
            break;

        case '+':
            putchar(0xfe);
            putchar(0x00);
            break;

        case '-':
            putchar(0xfe);
            putchar(0x08);
            break;

        case ',':
            putchar(0xff);
            putchar(0x55);
            putchar(0xf8);
            break;

        case '.':
            putchar(0xff);
            putchar(0x55);
            putchar(0xfc);
            break;

        case '[':
            putchar(0x90);
            putchar(0xbb);
            putchar(0x54);
            putchar(0x80);
            putchar(0x04);
            putchar(0x08);
            putchar(0xff);
            putchar(0xd3);
            break;

        case ']':
            putchar(0x90);
            putchar(0x90);
            putchar(0x8a);
            putchar(0x18);
            putchar(0x84);
            putchar(0xdb);
            putchar(0x74);
            putchar(0x03);
            putchar(0xff);
            putchar(0x24);
            putchar(0x24);
            putchar(0x83);
            putchar(0xc4);
            putchar(0x04);
            break;
        }
    }
    fflush(0);

    write(1, epilogue, sizeof(epilogue));

    return 0;
}

#include <iostream>
#include <string>
#include <sstream>

typedef unsigned char byte;

char header[84] = {
    0x7f, 0x45, 0x4c, 0x46,   0x01, 0x01, 0x01, 0x09,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x03, 0x00,   0x01, 0x00, 0x00, 0x00,   0xa7, 0x80, 0x04, 0x08,   0x34, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x34, 0x00, 0x20, 0x00,   0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,   0x01, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x80, 0x04, 0x08,
    0x00, 0x80, 0x04, 0x08,   0x78, 0x56, 0x34, 0x12,   0x78, 0x56, 0x34, 0x12,   0x07, 0x00, 0x00, 0x00,
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

std::string encode(byte b, char op)
{
    if(b == 0)
        return "";

    std::string res(b, op);
    for(int i = 1; i < 80; ++i)
        for(int j = i; j < 80; ++j)
            if(i*j < 256)
            {
                int k = 7 + i + j + abs(b - i*j);
                if(k < res.size())
                {
                    std::ostringstream oss;
                    oss << '<' << std::string(i, '+') << '[' << '-' << '>'
                        << std::string(j, op) << '<' << ']' << '>'
                        << std::string(abs(b - i*j), (b > i*j) ? op : (op == '-' ? '+' : '-'));
                    res = oss.str();
                }
            }
    return res;
}

std::string zero(byte b)
{
    return b ? (b > 128 ? "[+]" : "[-]") : "";
}

void encode(char *buf, int len)
{
    int n;
    byte next, cur = 0;

    std::string line, comment;
    for(n = 0; n < len; ++n)
    {
        next = ((byte*)buf)[n];
        std::string
            a = zero(cur) + encode(next, '+'),
            b = zero(cur) + encode(next, '-'),
            c = encode(abs(cur - next), next > cur  ? '+' : '-'),
            d = encode(256 - abs(cur - next), next > cur ? '-' : '+');

        std::string best = a;
        if(b.length() < best.length())
            best = b;
        if(c.length() < best.length())
            best = c;
        if(d.length() < best.length())
            best = d;

        if(line.size() + best.size() + 2 >= 60)
        {
            std::cout << line << std::string(60 - line.size(), ' ') << comment << '\n';
            line.clear();
            comment.clear();
        }

        line += best;
        line += ". ";

        comment += ' ';
        comment += "0123456789abcdef"[next/16];
        comment += "0123456789abcdef"[next%16];

        cur = next;
    }

    line += zero(cur);
    std::cout << line << std::string(60 - line.size(), ' ') << comment << '\n' << '\n';
}

int main()
{
    encode(header, 68);
    encode(header + 76, 8);
    encode(prologue, sizeof(prologue));
    encode(epilogue, sizeof(epilogue));

    return 0;
}

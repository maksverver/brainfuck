# Demonstration of Brainfuck-to-x86 mapping

.global _start

.text

# Writes the current character to stdout
putchar:
    pushl   %eax
    incl    %ebx            # fd = 1        (%ebx)
    movl    %eax, %ecx      # buf = %eax    (%ecx)
    movl    %ebx, %edx      # count = 1     (%edx)
    movl    $4, %eax        # write() syscall id
    int     $0x80           # syscall
    decl    %ebx
    popl    %eax
    ret

# Reads a character from stdin into the current character.
getchar:
    pushl   %eax
                            # fd = 0        (%ebx)
    movl    %eax, %ecx      # buf = %eax    (%ecx)
    movl    $1, %edx        # count = 1     (%edx)
    movl    $3, %eax        # read() syscall id
    int     $0x80           # syscall
    popl    %eax
    ret


# Execution starts here.

_start:
    movl    $0x100000, %ecx # %ecx = register storage size
    subl    %ecx, %esp      # reserve register storage on stack
    movl    %esp, %ebp      # set base pointer
    movl    %esp, %eax      # set %eax as register pointer
    xorl    %ebx, %ebx      # set %ebx = 0

    # zero registers
    movl    %esp, %edi
    xorb    %al, %al
    rep     stosb

    pushl   $getchar        # ebp[-4] == getchar
    pushl   $putchar        # ebp[-8] == putchar


# Brainfuck program starts here
#
# Sample program: +++[->++[->,..<]<]
# Reads six characters, and prints out each character twice.

bf_start:
    incb    (%eax)          #   +
    incb    (%eax)          #   +
    incb    (%eax)          #   +
    cmpb    (%eax), %bl     #   [
    je      d
a:
    decb    (%eax)          #   -
    incl    %eax            #   >
    incb    (%eax)          #   +
    incb    (%eax)          #   +
    cmpb    (%eax), %bl     #   [
    je      c
    .skip 12345

b:
    decb    (%eax)          #   -
    incl    %eax            #   >
    call    *-4(%ebp)       #   ,
    call    *-8(%ebp)       #   .
    call    *-8(%ebp)       #   .
    decl    %eax            #   <
    cmpb    (%eax), %bl     #   ]
    jne     b

c:
    decl    %eax            #   <
    cmpb    (%eax), %bl     #   ]
    jne     a
d:

bf_end:
# Brainfuck program ends here

    # exit() TODO
    movl    (%eax), %ebx    # code              => %ebx
    movl    $1, %eax        # exit() syscall id => %eax
    int     $0x80           # syscall

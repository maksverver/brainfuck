# Demonstration of Brainfuck-to-x86 mapping
#
#  Char     Code                Mnemonics
# -----    ------              -----------
#   >  (1)  40                  incl    %eax
#   <  (1)  48                  decl    %eax
#   +  (2)  fe 00               incb    (%eax)
#   -  (2)  fe 08               decb    (%eax)
#   ,  (3)  ff 55 f8            call    *-8(%ebp)
#   .  (3)  ff 55 fc            call    *-4(%ebp)
#   [  (8)  90                  nop
#           bb 74 80 04 08      mov     $fixcall, %ebx
#           ff d3               call    *%ebx
#   ] (14)  90                  nop
#           90                  nop
#           8a 18               mov     (%eax), %bl
#           84 db               test    %bl, %bl
#           74 03               je      +3
#           ff 24 24            jmp     *(%esp)
#           83 c4 04            add     $4, %esp

.global _start

.text

# This is where the magic happens!
# Replaces the target of the calling instruction, by the location
# corresponding the matching right brace.
fixcall:
    movl    (%esp), %ebx    # Store return address in %ebx
    movl    %ebx, %edi      # Preserve %ebx in %edi
    subl    $6, %edi        # Compute location of call operand

    # Search for location of matching right brace
    movl    $1, %ecx
    decl    %ebx
repeat:
    incl    %ebx
    cmpb    $0x90, (%ebx)
    jne     repeat
    incl    %ebx
    incl    %ecx
    cmpb    $0x90, (%ebx)
    jne     repeat
    subl    $2, %ecx
    jnz     repeat
    incl    %ebx

    # Target location is now in %ebx
    movl    %ebx, (%edi)    # Fix call instruction
    pushl   %ebx
    ret                     # Return to right brace location

# Writes the current character to stdout
putchar:
    pushl   $1              # bytes to write
    pushl   %eax            # buffer
    pushl   $1              # file descriptor 1 (stdout)
    xor     %eax, %eax
    pushl   %eax            # <dummy>
    movb    $4, %al         # write() syscall id
    int     $0x80           # syscall
    mov     8(%esp), %eax   # restore %eax
    addl    $16, %esp       # clean up stack
    ret

# Reads a character from stdin into the current character.
# In case of an error, the current character is set to 0.
getchar:
    movl    %eax, %ebx      # preserve %eax in %ebx
    pushl   $1              # bytes to read
    pushl   %eax            # buffer
    pushl   $0              # file descriptor 0 (stdin)
    xor     %eax, %eax
    pushl   %eax            # <dummy>
    movb    $3, %al         # read() syscall id
    int     $0x80           # syscall
    decl    %eax            # check for error
    jz      read_ok
    movb    $0, (%ebx)      # set EOF byte
read_ok:
    addl    $16, %esp       # clean up stack
    movl    %ebx, %eax
    ret

# Execution starts here.

_start:

    # Build stack frame
    movl    %esp, %ebp
    pushl   $putchar        # ebp[-4] == putchar
    pushl   $getchar        # ebp[-8] == getchar

    # Allocate tape memory (0x100000 == 1 MB)
    movl    $0x100000, %ebx
    xorl    %eax, %eax
    pushl   %eax            # offset: 0
    pushl   %eax            # padding: 0
    pushl   $-1             # fd: -1
    pushl   $0x1000         # flags MAP_ANON
    pushl   $3              # prot: PROT_READ | PROT_WRITE
    pushl   %ebx            # len
    pushl   %eax            # addr: 0
    movb    $197, %al       # mmap() syscall id
    pushl   %eax            # <dummy>
    int     $0x80           # syscall
    addl    $32, %esp

    cmpl    $-1, %eax       # test for allocation failure
    jne     alloc_ok

    # exit(1)
    xorl    %eax, %eax
    incl    %eax
    pushl   %eax            # exit code: 1
    pushl   %eax            # <dummy>
    int     $0x80           # syscall

alloc_ok:
    # Zero memory
    movl    %eax, %edi
    movl    %ebx, %ecx
    xorb    %al, %al
    rep     stosb

    # Set tape pointer halfway in tape memory
    movl    %edi, %eax
    shrl    $1, %ebx
    subl    %ebx, %eax

# Brainfuck program starts here
#
# Sample program: ++[->++[->,..<]<]
# Reads four characters, and prints out each character twice.

bf_start:
    incb    (%eax)          #   +
    incb    (%eax)          #   +
    nop                     #   1 nop marks a loop start
    movl    $fixcall, %ebx  #   [
    call    *%ebx
    decb    (%eax)          #   -
    incl    %eax            #   >
    incb    (%eax)          #   +
    incb    (%eax)          #   +
    nop                     #   1 nop marks a loop start
    movl    $fixcall, %ebx  #   [
    call    *%ebx
    decb    (%eax)          #   -
    incl    %eax            #   >
    call    *-8(%ebp)       #   ,
    call    *-4(%ebp)       #   .
    call    *-4(%ebp)       #   .
    decl    %eax            #   <
    nop                     #   2 nops mark a loop end
    nop
#    movb    (%eax), %bl     #   ]
#    test    %bl, %bl

    cmpb   (%eax), %bl

    jz      break1
    jmp     *(%esp)
break1:
    addl $4, %esp
    decl    %eax            #   <
    nop                     #   2 nops mark a loop end
    nop
    movb    (%eax), %bl     #   ]
    test    %bl, %bl
    jz      break2
    jmp     *(%esp)
break2:
    addl $4, %esp

bf_end:
# Brainfuck program ends here


    # exit(0)
    xorl    %eax, %eax
    pushl   %eax            # exit code: 0
    incl    %eax
    pushl   %eax            # <dummy>
    int     $0x80           # syscall

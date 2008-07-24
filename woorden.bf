Memory map:

     addr    value
    =======================
      0      main loop condition
      1      counter reset flag
      2      counter high digit
      3      counter low digit
      4      scratch: condition
      5      current character
      6      scratch: copy character
      7      scratch: copy character
      8plus  current word


init memory
+>+<

main loop
[>
base position inside loop is 4 (current character)

check counter reset flag
[-
    reset counter
    >[-]>[-]

    prompt for more input
    +<++++++++[->+++++++++<]>.
    +<++++++[->++++++<]>.
    ++.
    +++++.
    -.
    [-<+>]<[-->+<]>.
    -<+++++[->-----<]>.
    [-]
<<]

read next character
>>>>,


check if end of line
[->+>+<<]>>[-<<+>>]<<
<+> >----------[<<->>[-]]<
<[->
    set counter reset flag
    <<<<+>>>>

    change character to space
    ++++++++++++++++++++++
<]>

check if space
[->+>+<<]>>[-<<+>>]<<
<+>>>++++[-<-------->]<[<<->>[-]]<
<[->
    check if this is the first word
    <<[->>>+<<<]>>>[-<<+<+>>>]<

    check if first character is e
    >>>[-<+<+>>]<[->+<]< ->++++++++++[-<---------->]< [[-]<<+>>]<

    check if second character is n
    >>>>[-<<+<+>>>]<<[->>+<<]< >++++++++++[-<----------->]< [[-]<<+>>]<

    check if third character is d
    >>>>>[-<<<+<+>>>>]<<<[->>>+<<<]< >++++++++++[-<---------->]< [[-]<<+>>]<

    check if fourth character is \0
    >>>>>>[-<<<<+<+>>>>>]<<<<[->>>>+<<<<]< [[-]<<+>>]<

    terminate if all checks succeeded
    >+<<[[-]>>-<<]>>[-
        <<<<<<->>>>>>   toggle main loop condition
        >>[-]<<         prevent current word from being printed
    ]<

    check if word is empty
    >>>[-<+<+>>]<[->+<]<[[-]<

    increment low digit
    <<+

    [->+>>+<<<]>[-<+>]              copy
    +>>----------[[-]<<->>]<<       compare
    [[-]<----------<+>>]<           if =10 increment high digit

    print high digit
    <[->>+>>+<<<<]>>[-<<+>>]>>      copy to scratch
    [>++++++++[-<++++++>]<.[-]]     if <> 0 print ASCII digit
    <<<

    print low digit
    [->+>>+<<<]>[-<+>]>>
    >++++++++[-<++++++>]<.[-]       print ASCII digit
    <

    +<+++++[->+++++<]>   add 26
    .                    print ASCII colon
    -<+++++[->-----<]>   subtract 26
    .                    print space

    print out current word
    >>>[.>]<[[-]<]<<

    print newline
    >++++++++++.[-]<

    >]<
<]>

check for non space character
[->+>+<<]>>[-<<+>>]<<
>-->+++++[-<------>]<[[-]<<+>>]<
<[->
    copy character
    [->+>+<<]>>[-<<+>>]<<

    append to string
    > ->>[>]+[<]< [->>[>]<+[<]<] <
<]>

<<<<<]   repeat main loop

#!/usr/bin/env perl
#
# Converts objdump dissassembly output to C source code

while(<>) {
    chomp;
    print "/* $1: */\n" if /<(\w+)>:/;

    next unless /^ [0-9a-f]+:\t(([0-9a-f][0-9a-f] )+)\s*(.*)$/;
    $comment = $3;
    @bytes   = split / /, $1;

    $pos = 4;
    print '    ';
    foreach $byte (@bytes) {
        print "0x$byte, ";
        $pos += 6;
    }
    print ' 'x(40 - $pos), "/* $comment ", ' 'x(30 - length($comment)), "*/\n";
}

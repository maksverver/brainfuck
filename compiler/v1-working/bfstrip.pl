#!/usr/bin/env perl

while(<>) {
    foreach $c (split //) {
        print $c if $c =~ /[][+<>.,-]/;
    }
}

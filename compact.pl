#!/usr/bin/env perl

$_=join '', <>;
s/[^][.,<>+-]//g;
s/([^\n]{80}|[^\n]$)/\1\n/g;
print;

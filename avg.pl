#!/usr/bin/perl

$s = 0.0;
while(<STDIN>) {
  $s+= $_;
  $c+= 1;

}

print $s/$c,"\n";

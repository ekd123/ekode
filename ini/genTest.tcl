#!/usr/bin/tclsh
set f [open test.ini w]

for {set i 1} {$i <= 1000} {incr i} {
    puts $f "\[Section$i\]"
    for {set j 1} {$j <= 1000} {incr j} {
        puts $f "Key$j = [expr {rand()*$j}]"
    }
}

close $f

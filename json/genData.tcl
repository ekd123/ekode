#!/usr/bin/tclsh

set f [open test.json w]
puts $f "{"
for {set i 1} {$i < 10000} {incr i} {
    puts $f "\"$i\": $i, "
}
puts $f "\"10000\": 10000"
puts $f "}"

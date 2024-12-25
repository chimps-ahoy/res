#!/bin/sh

rm actual

gcc pp.c &&
./a.out > actual <<EOF
@foo bar
foo {{ ls }} bar
quux
{{ ls }} baz
@bar foo
EOF

printf '%s' 'foo ' > expected
ls >> expected
printf '%s\n' 'bar' >> expected
printf '%s\n' 'quux' >> expected
ls >> expected
printf '%s\n' 'baz' >> expected

cmp actual expected && echo 'pass' || echo 'fail'

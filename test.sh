#!/bin/sh

rm -f actual expected
touch actual expected

make res &&
./res > actual <<EOF
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

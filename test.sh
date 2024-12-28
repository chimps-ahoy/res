#!/bin/sh

rm -f actual expected
touch actual expected

make res &&
./res > actual <<EOF
@foo bar
foo {{ ls }} bar
quux
{{ foo }}
{{ ls }} baz
@bar foo
{{bar }}
EOF

printf '%s' 'foo ' > expected
ls >> expected
printf '%s\n' ' bar' >> expected
printf '%s\n' 'quux' >> expected
printf '%s\n' 'bar' >> expected
ls >> expected
printf '%s\n' ' baz' >> expected
printf '%s\n' 'foo' >> expected

cmp actual expected && echo 'pass' || echo 'fail'

#!/bin/bash

# Sometimes we have to program both the left and right modules as if they
# were the master. In particular, it seems necessary in order to get the
# LCD visualizations to be in sync.
#
# This shell script makes it easy.

echo "Do not press any keys until instructed!"
echo ""

make MASTER=left | grep -v "Nothing to be done"
read -p 'Connect LEFT module as master and press any key to continue ...' -n1 -s
make MASTER=left program | grep -v "Nothing to be done"

make MASTER=right| grep -v "Nothing to be done"
read -p 'Connect RIGHT module as master and press any key to continue ...' -n1 -s
make MASTER=right program | grep -v "Nothing to be done"

#!/bin/bash
#
# Clean-Junk Script: Javilonas, 24-04-2015
# Javilonas <admin@lonasdigital.com>
#

for i in `find . -type f \( -iname \*.rej \
				-o -iname \*.orig \
				-o -iname \*~ \
				-o -iname \*.bkp \
				-o -iname \*.ko \
				-o -iname \*.c.BACKUP.[0-9]*.c \
				-o -iname \*.c.BASE.[0-9]*.c \
				-o -iname \*.c.LOCAL.[0-9]*.c \
				-o -iname \*.c.REMOTE.[0-9]*.c \
				-o -iname \*.org \)`; do
	rm -vf $i;
done;

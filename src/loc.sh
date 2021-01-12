#!/bin/sh

# Counts the lines of code (without blank lines)
find . -iname '*.[cS]*' | grep -v '\.svn' | xargs cat | sed '/^\s*$/d' | wc

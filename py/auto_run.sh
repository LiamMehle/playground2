#!/bin/sh



find . | entr sh -c "clear ; python3 $1"

#!/bin/sh
ls -l | sed 's/\( \)\+/ /g' | cut -d' ' -f5,9

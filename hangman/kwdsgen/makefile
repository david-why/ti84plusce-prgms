SRCS=$(wildcard *.bin)
BINS=$(SRCS:.bin=.8xv)
SHELL=bash

%.8xv: %.bin
	export BASENAME="$(basename $<)"; convbin -j bin -i $< -o $@ -k 8xv -n $${BASENAME^^}

all: ${BINS}

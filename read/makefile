# ----------------------------
# Makefile Options
# ----------------------------

NAME := READ
DESCRIPTION := "Would you like a book?"
COMPRESSED := NO
ARCHIVED := YES

HAS_PRINTF := NO

CFLAGS := -Wall -Wextra -Oz
CXXFLAGS := -Wall -Wextra -Oz

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk

src/data.h: src/gen_ti_book/*
	cd src/gen_ti_book; python3 gen_ti_book.py; python3 bin_to_array.py > ../data.h

a: src/data.h all

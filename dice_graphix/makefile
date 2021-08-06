# ----------------------------
# Makefile Options
# ----------------------------

NAME := DICEG
ICON := icon.png
DESCRIPTION := "Roll a Dice! W/ Graphics"
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

g: gfx all

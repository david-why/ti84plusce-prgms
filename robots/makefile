# ----------------------------
# Makefile Options
# ----------------------------

NAME := ROBOTS
ICON := icon.png
DESCRIPTION := "Robots game"
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

$(BINDIR)/LUCIDAC.8xv: $(SRCDIR)/LucidaConsole.fnt
	convfont -o fontpack -N "LucidaConsole" -P "ASCII" -V "v2021.6.26" -A "Anonymous" \
	-D "The Lucida Console font." \
	-f $(SRCDIR)/LucidaConsole.fnt -w normal -s monospace

# This is a roundabout way to tell make that myfonts.c depends on the .inc files.
# It does it by saying the compiled object code depends on the .inc files.
$(OBJDIR)/myfonts.src: $(SRCDIR)/LucidaConsole.inc

# Convert a .fnt file into a .inc file
$(SRCDIR)/LucidaConsole.inc: $(SRCDIR)/LucidaConsole.fnt
	convfont -o carray -f $(SRCDIR)/LucidaConsole.fnt $(SRCDIR)/LucidaConsole.inc

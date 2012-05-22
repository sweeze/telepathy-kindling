## Process this file with automake to produce Makefile.in

## Created by Anjuta



AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(TELEPATHY_KINDLING_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = telepathy_kindling

telepathy_kindling_SOURCES = \
	main.c \
	kindling-connection-manager.h \
	kindling-connection-manager.c \
	kindling-protocol.c \
	kindling-protocol.h \
	kindling-connection.c \
	kindling-connection.h \
	kindling-muc-manager.c \
	kindling-muc-manager.h \
	kindling-roomlist-manager.c \
	kindling-roomlist-manager.h \
	kindling-roomlist-channel.c \
	kindling-roomlist-channel.h \
	kindling-muc-channel.c \
	kindling-muc-channel.h

telepathy_kindling_LDFLAGS = \
	-Wl,--export-dynamic

telepathy_kindling_LDADD = $(TELEPATHY_KINDLING_LIBS)



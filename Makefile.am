## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src 

telepathy_kindlingdocdir = ${prefix}/doc/telepathy_kindling
telepathy_kindlingdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = $(telepathy_kindlingdoc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(telepathy_kindlingdocdir)

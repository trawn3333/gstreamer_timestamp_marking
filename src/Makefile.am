# Note: plugindir is set in configure

##############################################################################
# TODO: change libgstplugin.la to something else, e.g. libmysomething.la     #
##############################################################################
plugin_LTLIBRARIES = libgstmarking.la

##############################################################################
# TODO: for the next set of variables, name the prefix if you named the .la, #
#  e.g. libmysomething.la => libmysomething_la_SOURCES                       #
#                            libmysomething_la_CFLAGS                        #
#                            libmysomething_la_LIBADD                        #
#                            libmysomething_la_LDFLAGS                       #
##############################################################################

# sources used to compile this plug-in
libgstmarking_la_SOURCES = gstmarking.c gstmarkin.c gstmark.c gstmarkout.c
libgstmarking_la_HEADERS = gstmarkin.h gstmarkout.h gstmark.h 
libgstmarking_ladir = 

# compiler and linker flags used to compile this plugin, set in configure.ac
libgstmarking_la_CFLAGS = $(GST_CFLAGS)
libgstmarking_la_LIBADD = $(GST_LIBS)
libgstmarking_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstmarking_la_LIBTOOLFLAGS = --tag=disable-static

# headers we need but don't want installed
noinst_HEADERS = gstmarkin.h gstmarkout.h

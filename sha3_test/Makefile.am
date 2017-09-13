AUTOMAKE_OPTIONS = foreign subdir-objects
ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = bootstrap

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(LDFLAGS)
AM_CXXFLAGS = $(CXXFLAGS)
AM_CPPFLAGS = $(CPPFLAGS)

SHA3_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BDB_CPPFLAGS) $(BOOST_CPPFLAGS)

sha3_CPPFLAGS = $(AM_CPPFLAGS) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(NODE_INCLUDES)
sha3_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
sha3_LDADD = $(BOOST_LIBS) $(BDB_LIBS)
sha3_LDFLAGS = $(AM_LDFLAGS)
sha3_SOURCES = \
	../common/ArgsManager.cpp \
	../common/logger.cpp \
	../common/utilstrencodings.cpp \
	src/sha3.cpp \
	sha3_test.cpp

bin_PROGRAMS = sha3


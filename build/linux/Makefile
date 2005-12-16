
include Makefile.sources
include Makefile.objects


###############################################################
#
# You should only need to edit these:
#

FREEIMAGE_INCLUDE = ../../../FreeImage/Dist
FREEIMAGE_LIBS    = ../../../FreeImage/

FREETYPE_INCLUDE  = /usr/include/freetype2
FREETYPE_LIBS     = /usr/lib

FTGL_INCLUDE      = /usr/include/FTGL
FTGL_LIBS         = /usr/lib

PYTHON_INCLUDE    = /usr/include/python2.4
PYTHON_LIBS       = /usr/lib

OPENAL_INCLUDE    = /usr/include/AL
OPENAL_LIBS       = /usr/lib

XERCESC_INCLUDE   = /usr/include
XERCESC_LIBS      = /usr/lib

H3D_HOME          = ../..

#
#
###############################################################

CXX        = g++
SHARED     = -shared -export-dynamic
INCLUDES   = -I$(H3D_HOME)/src
CXXFLAGS   = -g $(INCLUDES)
LDFLAGS    = 

#libcurl
CXXLIBS   += -lcurl

# CG
CXXLIBS   += -lCg -lCgGL

# libaudiofile
CXXLIBS   += -laudiofile

# Ogg vorbis
CXXLIBS   += -lvorbisfile

# FreeImage
CXXLIBS   += -L$(FREEIMAGE_LIBS) -lfreeimage
INCLUDES  += -I$(FREEIMAGE_INCLUDE)
 
# FreeType
CXXLIBS   += -L$(FREETYPE_LIBS) -lfreetype
INCLUDES  += -I$(FREETYPE_INCLUDE)

# FTGL
##FTGL_CPPFLAGS := $(shell pkg-config --cflags ftgl)
##FTGL_LDFLAGS  := $(shell pkg-config --libs-only-L ftgl)
##FTGL_LIBS     := $(shell pkg-config --libs-only-l ftgl)
CXXLIBS   += -L$(FTGL_LIBS) -lftgl
INCLUDES  += -I$(FTGL_INCLUDE)

# Xerces-C
CXXLIBS   += -L$(XERCESC_LIBS) -lxerces-c 
INCLUDES  += -I$(XERCESC_INCLUDE)

# OpenHaptics
CXXLIBS   += -lHL -lHLU -lHDU -lHD -lrt

# Python
CXXLIBS   += -L$(PYTHON_LIBS) -lpython2.4 -lutil
INCLUDES  += -I$(PYTHON_INCLUDE)

# OpenGL etc
CXXLIBS   += -lGLEW -lGL -lGLU -lglut

# OpenAL
CXXLIBS   += -L$(OPENAL_LIBS) -lopenal
INCLUDES  += -I$(OPENAL_INCLUDE)

# FontConfig
CXXLIBS   += -lfontconfig

###############################################################

all:	libH3DApi.so H3DLoad

libH3DApi.so:	$(OBJECTS)
	$(CXX) $(LDFLAGS) $(CXXLIBS) $(SHARED) -o $@ $(OBJECTS) $(VHTK_OBJECTS)

H3DLoad:	$(H3D_HOME)/src/H3DLoad.o libH3DApi.so
	$(CXX) $(LDFLAGS) -o $@ $^ $(CXXLIBS) 

clean:	
	$(RM) $(H3D_HOME)/src/*~ $(H3D_HOME)/src/*.o $(H3D_HOME)/src/Types/*~ $(H3D_HOME)/src/Types/*.o

install: libH3DApi.so
	cp libH3DApi.so /usr/lib
	cp H3DLoad /usr/local/bin

update:	
	$(MAKE) sources
	$(MAKE) objects
	$(MAKE) depend

sources:
	ls $(H3D_HOME)/src/*.cpp $(H3D_HOME)/src/Types/*.cpp > Makefile.sources
	echo -n "SOURCES=" > Makefile.sources2
	cat Makefile.sources \
	  | sed 's/.cpp/.cpp \\/g' \
	  >> Makefile.sources2
	mv Makefile.sources2 Makefile.sources

objects:
	ls $(H3D_HOME)/src/*.cpp $(H3D_HOME)/src/Types/*.cpp > Makefile.objects
	echo -n "OBJECTS=" > Makefile.objects2
	cat Makefile.objects \
	  | grep -v "H3DLoad.cpp" \
	  | sed 's/.cpp/.o \\/g' \
	  >> Makefile.objects2
	mv Makefile.objects2 Makefile.objects

depend:
	touch Makefile.depend
	makedepend -fMakefile.depend $(INCLUDES) $(SOURCES)


include Makefile.depend

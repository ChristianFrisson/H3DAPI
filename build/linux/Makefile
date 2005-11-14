
include Makefile.sources
include Makefile.objects


###############################################################
#
# You should only need to edit these:
#

FREEIMAGE_PATH=../../../../../External/FreeImage
FREETYPE_PATH=../../../../../External/freetype-2.1.9
FTGL_PATH=../../../../../External/FTGL
XERCESC_PATH=../../../../../External/xerces-c-src_2_6_0

#
#
###############################################################

CXX        = g++
SHARED     = -shared -export-dynamic
INCLUDES   = -I../../src
CXXFLAGS   = -g $(INCLUDES)

# FreeImage
CXXLIBS   += -L$(FREEIMAGE_PATH) -lfreeimage

# FreeType
CXXLIBS   += -lfreetype
INCLUDES  += -I$(FREETYPE_PATH)/include

# FTGL
##FTGL_CPPFLAGS := $(shell pkg-config --cflags ftgl)
##FTGL_LDFLAGS  := $(shell pkg-config --libs-only-L ftgl)
##FTGL_LIBS     := $(shell pkg-config --libs-only-l ftgl)
CXXLIBS   += -lftgl
INCLUDES  += -I$(FTGL_PATH)/include

# Xerces-C
CXXLIBS   += -L$(XERCESC_PATH)/lib -lxerces-c 
INCLUDES  += -I$(XERCESC_PATH)/src

# OpenHaptics
CXXLIBS   += -lHL -lHLU -lHDU -lHD -lrt
INCLUDES  += -DHAVE_OPENHAPTICS 

# Python
CXXLIBS   += -lpython2.3 -lutil
INCLUDES  += -I/usr/include/python2.3

# OpenGL etc
CXXLIBS   += -lGLEW -lGL -lGLU -lglut

# FontConfig
CXXLIBS   += -lfontconfig

###############################################################

libH3DApi.so:	$(OBJECTS)
	$(CXX) $(CXXLIBS) $(SHARED) -o $@ $(OBJECTS) $(VHTK_OBJECTS)

H3DLoad:	../../src/H3DLoad.o libH3DApi.so
	$(CXX) -o $@ $^ $(CXXLIBS) 

clean:	
	$(RM) ../../src/*~ ../../src/*.o ../../src/Types/*~ ../../src/Types/*.o

install: libH3DApi.so
	cp libH3DApi.so /usr/lib

update:	
	$(MAKE) sources
	$(MAKE) objects
	$(MAKE) depend

sources:
	ls ../../src/*.cpp ../../src/Types/*.cpp > Makefile.sources
	echo -n "SOURCES=" > Makefile.sources2
	cat Makefile.sources \
	  | sed 's/.cpp/.cpp \\/g' \
	  >> Makefile.sources2
	mv Makefile.sources2 Makefile.sources

objects:
	ls ../../src/*.cpp ../../src/Types/*.cpp > Makefile.objects
	echo -n "OBJECTS=" > Makefile.objects2
	cat Makefile.objects \
	  | grep -v "DeformableIFS.cpp" \
	  | grep -v "H3DLoad.cpp" \
	  | sed 's/.cpp/.o \\/g' \
	  >> Makefile.objects2
	mv Makefile.objects2 Makefile.objects

depend:
	touch Makefile.depend
	makedepend -fMakefile.depend -I../../src $(SOURCES)


include Makefile.depend

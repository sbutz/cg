HEADERS = cmdline.h scene.h raster.h
SOURCES = main.cpp cmdline.cpp scene.cpp raster.cpp

# zum Debuggen
# CXXFLAGS = -ggdb3 -O0
CXXFLAGS = -O4

renderpipeline: $(SOURCES) $(HEADERS)
	g++ $(CXXFLAGS) -std=c++14 $(SOURCES) -lpng -o $@

dist: renderpipeline2-ref.tar.bz2

clean:
	rm -f renderpipeline

renderpipeline2-ref.tar.bz2: $(SOURCES) $(HEADERS)
	mkdir renderpipeline2-ref
	cp $(SOURCES) $(HEADERS) bunny.data rot.py Makefile renderpipeline2-ref
	tar cfj $@ renderpipeline2-ref


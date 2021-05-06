HEADERS = cmdline.h scene.h raster.h
SOURCES = main.cpp cmdline.cpp scene.cpp raster.cpp

# zum Debuggen
# CXXFLAGS = -ggdb3 -O0
CXXFLAGS = -O4

renderpipeline: $(SOURCES) $(HEADERS)
	g++ $(CXXFLAGS) -std=c++14 $(SOURCES) -lpng -o $@

dist: renderpipeline-ref.tar.bz2

clean:
	rm -f renderpipeline

renderpipeline-ref.tar.bz2: $(SOURCES) $(HEADERS)
	mkdir renderpipeline-ref
	cp $(SOURCES) $(HEADERS) bunny.data rot.py Makefile renderpipeline-ref
	tar cfj $@ renderpipeline-ref


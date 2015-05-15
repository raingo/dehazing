OUT = dehaze
CXX = g++
IDIR = include/
#DEBUG=-ggdb
DEBUG=
CXXFALGS = -Wall ${DEBUG} -I/home/linzhi/software/include -I/usr/include/eigen3 -I/u/yli/.local/include/boost-numeric-bindings
OBJS = dehaze.o guidedfilter.o
LIBS = -lopencv_highgui -lopencv_core -lopencv_contrib -lopencv_imgproc -lopencv_legacy -llapack ${DEBUG}

dehaze:$(OBJS)
	$(CXX) -o $(OUT) $(OBJS) $(LIBS)

dehaze.o:dehaze.cpp $(IDIR)
	$(CXX) -c  dehaze.cpp $(CXXFALGS)


guidedfilter.o:
	$(CXX) -c  guided-filter/guidedfilter.cpp $(CXXFALGS)

.PHONY:clean

clean:
	rm -f $(OBJS)

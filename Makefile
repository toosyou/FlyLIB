CXX = g++-6
CXXFLAGS = -Lprogressbar/ -lprogressbar -lncurses

all: progressbar/libprogressbar.a Brain.o Cluster.o Neuron.o Neuropil.o Segment.o Vertices.o
	mkdir -p lib
	ar rcs libFly.a Brain.o Cluster.o Neuron.o Neuropil.o Segment.o Vertices.o
	mv *.a lib/
	rm -f *.o

progressbar/libprogressbar.a:
	make -C progressbar

Brain.o: Brain.cpp Brain.h
	$(CXX) $(CXXFLAGS) -c Brain.cpp -o $@

Cluster.o: Cluster.cpp Cluster.h
	$(CXX) $(CXXFLAGS) -c Cluster.cpp -o $@

Neuron.o: Neuron.cpp Neuron.h
	$(CXX) $(CXXFLAGS) -c Neuron.cpp -o $@

Neuropil.o: Neuropil.cpp Neuropil.h
	$(CXX) $(CXXFLAGS) -c Neuropil.cpp -o $@

Segment.o: Segment.cpp Segment.h
	$(CXX) $(CXXFLAGS) -c Segment.cpp -o $@

Vertices.o: Vertices.cpp Vertices.h
	$(CXX) $(CXXFLAGS) -c Vertices.cpp -o $@

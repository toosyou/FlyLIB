CXX = g++-5
CXXFLAGS = 

all: Brain.o Cluster.o Neuron.o Neuropil.o Segment.o Vertices.o
	mkdir -p lib
	#ar rcs libBrain.a Brain.o
	#ar rcs libCluster.a Cluster.o
	#ar rcs libNeuron.a Neuron.o
	#ar rcs libNeuropil.a Neuropil.o
	#ar rcs libSegment.a Segment.o
	#ar rcs libVertices.a Vertices.o
	ar rcs libFly.a Brain.o Cluster.o Neuron.o Neuropil.o Segment.o Vertices.o
	mv *.a lib/
	rm -f *.o

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

#similaritymatrix.o: similaritymatrix.cpp similaritymatrix.h
	#$(CXX) $(CXXFLAGS) -c similaritymatrix.cpp -o $@

Vertices.o: Vertices.cpp Vertices.h
	$(CXX) $(CXXFLAGS) -c Vertices.cpp -o $@
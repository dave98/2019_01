all: tracker peer

tracker: tracker.cpp
	g++ -std=c++17 -o tracker.exe tracker.cpp -pthread

peer: peer.cpp
	g++ -std=c++17 -o peer.exe peer.cpp -pthread

clean:
	rm -f *.out

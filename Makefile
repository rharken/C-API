all: callAPI tryCurl

callAPI: callAPI.cpp
	g++ -std=c++2a callAPI.cpp -L /usr/local/lib -l ssl -l crypto -o callAPI

tryCurl: tryCurl.cpp
	g++ -std=c++2a tryCurl.cpp -L /usr/local/lib -l curl -o tryCurl

all: callAPI

callAPI: callAPI.cpp
	g++ -std=c++2a callAPI.cpp -L /usr/local/lib -l ssl -l crypto -o callAPI

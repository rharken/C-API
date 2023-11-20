all: callAPI tryCurl tryCurlHttps

callAPI: callAPI.cpp
	g++ -std=c++2a callAPI.cpp -L /usr/local/lib -l ssl -l crypto -o callAPI

tryCurl: tryCurl.cpp
	g++ -std=c++2a tryCurl.cpp -L /usr/local/lib -l curl -o tryCurl

tryCurlHttps: tryCurlHttps.cpp
	g++ -std=c++2a tryCurlHttps.cpp -L /usr/local/lib -l curl -l jsoncpp -o tryCurlHttps

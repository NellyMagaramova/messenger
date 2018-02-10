all:
	g++ ./server_main.cpp -o echo_s 

clean:
	rm -f echo_s

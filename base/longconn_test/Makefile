all:
	g++ -g -o xserver_test xserver_test.cc -I. -I../../base -I../../thirdparty/gtest-1.6.0/include -I../../thirdparty/libevent-2.0.21-stable/include -L../../base/lib -L../../thirdparty/gtest-1.6.0/lib -L../../thirdparty/libevent-2.0.21-stable/lib -llongconn -levent -los -lgtest -lpthread
	g++ -g client.cc -o client -lpthread
clean:
	rm xserver_test 
	rm client

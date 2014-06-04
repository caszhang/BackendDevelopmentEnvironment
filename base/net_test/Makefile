all:
	g++ -g -o tcp_server_test tcp_server_test.cc -I. -I../../base -I../../thirdparty/gtest-1.6.0/include -L../../base/lib -L../../thirdparty/gtest-1.6.0/lib -lnet -los -lgtest -lpthread
	g++ -g -o simple_server_test simple_server_test.cc -I. -I../../base -I../../thirdparty/gtest-1.6.0/include -L../../base/lib -L../../thirdparty/gtest-1.6.0/lib -lnet -los -lgtest -lpthread
clean:
	rm tcp_server_test 

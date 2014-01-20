all:
	g++ -g -o thread_test thread_test.cc -I. -I../../base -I../../thirdparty/gtest-1.6.0/include -L../../base/lib -L../../thirdparty/gtest-1.6.0/lib -los -lgtest -lpthread
	g++ -g -o thread_base_test thread_base_test.cc -I. -I../../base -I../../thirdparty/gtest-1.6.0/include -L../../base/lib -L../../thirdparty/gtest-1.6.0/lib -los -lgtest -lpthread
clean:
	rm thread_test 
	rm thread_base_test

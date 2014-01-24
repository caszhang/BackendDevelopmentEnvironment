all:
	g++ -g -o thread_test thread_test.cc -I. -I../../base -I../../thirdparty/gtest-1.6.0/include -L../../base/lib -L../../thirdparty/gtest-1.6.0/lib -los -lgtest -lpthread
	g++ -g -o thread_base_test thread_base_test.cc -I. -I../../base -I../../thirdparty/gtest-1.6.0/include -L../../base/lib -L../../thirdparty/gtest-1.6.0/lib -los -lgtest -lpthread
	g++ -g -o singleton_test singleton_test.cc -I. -I../../base -I../../thirdparty/gtest-1.6.0/include -L../../base/lib -L../../thirdparty/gtest-1.6.0/lib -los -lgtest -lpthread
	g++ -g -o mutex_test mutex_test.cc -I. -I../../base -I../../thirdparty/gtest-1.6.0/include -L../../base/lib -L../../thirdparty/gtest-1.6.0/lib -los -lgtest -lpthread
	g++ -g -o lock_test lock_test.cc -I. -I../../base -I../../thirdparty/gtest-1.6.0/include -L../../base/lib -L../../thirdparty/gtest-1.6.0/lib -los -lgtest -lpthread
	g++ -g -o condition_test condition_test.cc -I. -I../../base -I../../thirdparty/gtest-1.6.0/include -L../../base/lib -L../../thirdparty/gtest-1.6.0/lib -los -lgtest -lpthread
	g++ -g -o md5_test md5_test.cc -I. -I../../base -I../../thirdparty/gtest-1.6.0/include -L../../base/lib -L../../thirdparty/gtest-1.6.0/lib -los -lgtest -lpthread
clean:
	rm thread_test 
	rm thread_base_test
	rm singleton_test
	rm mutex_test
	rm lock_test
	rm condition_test
	rm md5_test

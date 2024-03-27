driver: driver.o src/app/app_service.o src/app/app_controller.o src/core/utils.o src/core/service.o src/core/environment.o
	g++ driver.o src/app/app_service.o src/app/app_controller.o src/core/utils.o src/core/service.o src/core/environment.o -I/usr/include/mysql -o driver -lpthread -lssl -lcrypto  -lsoci_core -lsoci_mysql -lmysqlclient -DCROW_ENABLE_SSL

driver.o: driver.cpp
	g++ -c driver.cpp -o driver.o -DCROW_ENABLE_SSL

driver.cpp:
	$(error driver.cpp not found in /)

src/app/app_controller.o: src/app/app_controller.cpp
	g++ -c src/app/app_controller.cpp -o src/app/app_controller.o -DCROW_ENABLE_SSL

src/app/app_controller.cpp:
	$(error app_controller.cpp not found in /src/app)

src/app/app_service.o: src/app/app_service.cpp
	g++ -c src/app/app_service.cpp -o src/app/app_service.o

src/app/app_service.cpp:
	$(error app_service.cpp not found in /src/app)

src/core/utils.o: src/core/utils.cpp
	g++ -c src/core/utils.cpp -o src/core/utils.o

src/core/utils.cpp:
	$(error utils.cpp not found in src/core)

src/core/service.o: src/core/service.cpp
	g++ -c src/core/service.cpp -o src/core/service.o

src/core/service.cpp:
	$(error service.cpp not found in src/core)

src/core/environment.o: src/core/environment.cpp
	g++ -c src/core/environment.cpp -o src/core/environment.o 

src/core/environment.cpp:
	$(error environment.cpp not found in src/core)

clean:
	rm -f driver.o src/app/*.o src/core/*.o

run:
	sudo ./driver

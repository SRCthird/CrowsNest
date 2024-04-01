driver: src/core/driver.o src/user/user.controller.o src/core/utils.o src/core/service.o src/core/environment.o
	#g++ src/core/driver.o src/app/app.controller.o src/core/utils.o src/core/service.o src/core/environment.o -I/usr/include/mysql/ -o CrowsNest -lpthread -lssl -lcrypto  -lsoci_core -lsoci_mysql -lmysqlclient -DCROW_ENABLE_SSL
	g++ src/core/driver.o src/user/user.controller.o src/core/utils.o src/core/service.o src/core/environment.o -I/usr/include/mysql/ -o CrowsNest -lpthread -lssl -lcrypto -lsoci_core -lsoci_sqlite3 -lsqlite3 -DCROW_ENABLE_SSL

src/core/driver.o: src/core/driver.cpp
	g++ -c src/core/driver.cpp -o src/core/driver.o -DCROW_ENABLE_SSL -I/usr/include/mysql/

src/core/driver.cpp:
	$(error driver.cpp not found in /src/core)

src/user/user.controller.o: src/user/user.controller.cpp src/user/user.service.h
	g++ -c src/user/user.controller.cpp -o src/user/user.controller.o -DCROW_ENABLE_SSL -I/usr/include/mysql/

src/user/user.service.h:
	$(error user.service.h not found in /src/user)

src/user/user.controller.cpp:
	$(error user.controller.cpp not found in /src/user)

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

test:
	cd src/user && source ./test.sh

clean:
	rm -f src/user/*.o src/core/*.o

run:
	sudo ./CrowsNest

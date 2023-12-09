compile:
	g++ -std=c++17 -o ./obj/Graph_Node ./src/Graph_Node.hpp
	g++ -std=c++17 -o ./obj/network ./src/network.hpp
	g++ -std=c++17 -o ./obj/AlarmNetwork ./src/AlarmNetwork.hpp
	g++ -std=c++17 -o ./obj/EM ./src/EM.cpp
	g++ -std=c++17 -o ./obj/Format_Checker ./Format_Checker.cpp

clean:
	rm -f ./obj/Graph_Node
	rm -f ./obj/network
	rm -f ./obj/AlarmNetwork
	rm -f ./obj/EM
	clear

# givePermission:
# 	chmod +x ./obj/Graph_Node
# 	chmod +x ./obj/network
# 	chmod +x ./obj/AlarmNetwork
# 	chmod +x ./obj/EM

run:
	./obj/EM ./data/alarm.bif ./data/records_gen.dat

fm:
	./obj/Format_Checker
all:
	# make clean
	make compile
	# make givePermission
	make run
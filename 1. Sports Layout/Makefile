all: main.cpp SportsLayout.cpp
	g++ -o main main.cpp SportsLayout.cpp

all2: main.cpp SportsLayout.cpp
	g++ -o main2 main.cpp SportsLayout.cpp

clean:
	rm main

run: all
	./main ../test_cases/$(in) ../outputs/$(out)

test: all
	timeout 0.05m ./main ../test_cases/input1.txt ../outputs/output1.txt
	timeout 0.05m ./main ../test_cases/input2.txt ../outputs/output2.txt
	timeout 0.05m ./main ../test_cases/input3.txt ../outputs/output3.txt
	timeout 1m ./main ../test_cases/input4.txt ../outputs/output4.txt
	timeout 3m ./main ../test_cases/input5.txt ../outputs/output5.txt

rishit: all
	timeout 1m ./main ../Rishit\ TC/input0.txt ../Rishit\ TC/output0.txt
	timeout 3m ./main ../Rishit\ TC/input1.txt ../Rishit\ TC/output1.txt
	timeout 3m ./main ../Rishit\ TC/input2.txt ../Rishit\ TC/output2.txt
	timeout 3m ./main ../Rishit\ TC/input3.txt ../Rishit\ TC/output3.txt
	timeout 3m ./main ../Rishit\ TC/input4.txt ../Rishit\ TC/output4.txt
	timeout 3m ./main ../Rishit\ TC/input5.txt ../Rishit\ TC/output5.txt
	timeout 3m ./main ../Rishit\ TC/input6.txt ../Rishit\ TC/output6.txt
	timeout 3m ./main ../Rishit\ TC/input7.txt ../Rishit\ TC/output7.txt
	timeout 3m ./main ../Rishit\ TC/input10.txt ../Rishit\ TC/output10.txt

ghc: g++ -o ghc main.cpp SportsLayout.cpp
	timeout 1m ./ghc ../Rishit\ TC/input0.txt ../Rishit\ TC/output0.txt
	timeout 3m ./ghc ../Rishit\ TC/input1.txt ../Rishit\ TC/output1.txt
	timeout 3m ./ghc ../Rishit\ TC/input2.txt ../Rishit\ TC/output2.txt
	timeout 3m ./ghc ../Rishit\ TC/input3.txt ../Rishit\ TC/output3.txt
	timeout 3m ./ghc ../Rishit\ TC/input4.txt ../Rishit\ TC/output4.txt
	timeout 3m ./ghc ../Rishit\ TC/input5.txt ../Rishit\ TC/output5.txt
	timeout 3m ./ghc ../Rishit\ TC/input6.txt ../Rishit\ TC/output6.txt
	timeout 3m ./ghc ../Rishit\ TC/input7.txt ../Rishit\ TC/output7.txt
	timeout 3m ./ghc ../Rishit\ TC/input10.txt ../Rishit\ TC/output10.txt

bfs: 
	g++ -o bfs main.cpp SportsLayout.cpp
	timeout 1m ./bfs ../Rishit\ TC/input0.txt ../Rishit\ TC/output0.txt
	timeout 3m ./bfs ../Rishit\ TC/input1.txt ../Rishit\ TC/output1.txt
	timeout 3m ./bfs ../Rishit\ TC/input2.txt ../Rishit\ TC/output2.txt
	timeout 3m ./bfs ../Rishit\ TC/input3.txt ../Rishit\ TC/output3.txt
	timeout 3m ./bfs ../Rishit\ TC/input4.txt ../Rishit\ TC/output4.txt
	timeout 3m ./bfs ../Rishit\ TC/input5.txt ../Rishit\ TC/output5.txt
	timeout 3m ./bfs ../Rishit\ TC/input6.txt ../Rishit\ TC/output6.txt
	timeout 3m ./bfs ../Rishit\ TC/input7.txt ../Rishit\ TC/output7.txt
	timeout 3m ./bfs ../Rishit\ TC/input10.txt ../Rishit\ TC/output10.txt


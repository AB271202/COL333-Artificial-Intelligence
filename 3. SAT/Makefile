compile:
	./compile.sh

part_1a: compile
	./run1.sh ./part1_test/$(file)

minisat: #NOT WORKING
	minisat ./part1_test/$(file).satinput ./part1_test/$(file).satoutput

part_1: ./part1_test/$(file).satoutput
	./run2.sh ./part1_test/$(file)
	

part_2: compile
	./run3.sh ./part2_test/$(file)

clean1:
	rm -r obj
	rm -r *.satoutput *.satinput *.mapping

clean2:
	rm minisat.in
	rm minisat.out

clean: clean1 clean2
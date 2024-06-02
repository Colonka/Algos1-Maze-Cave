ARCHIVE = CPP_MAZE

all: install run

test: clean
	mkdir build
	cmake -S ./ -B build -DMAZE_TESTING=ON
	cmake --build build --target check

install: clean
	mkdir -p build
	cmake -S ./ -B build
	cmake --build build

run:
	open -Fn build/CPP_MAZE.app

uninstall: 
	rm -rf build
	rm -rf test_build
	rm -rf docs

dvi: clean
	mkdir -p docs
	doxygen Doxyfile
	mv html latex docs
	open docs/html/index.html

dvi_open:
	open docs/html/index.html

dist: clean install
	tar -cf $(ARCHIVE).tar build/*

gcovr_report: clean
	mkdir build
	mkdir report
	cmake -S ./ -B build -DMAZE_TESTING=ON -DMAZE_COVERAGE=ON
	cmake --build build --target coverage
	open report/report.html
	
git: clean
	git add -A
	git commit -m "$m"
	git push origin "$b"

test_no_run: clean
	mkdir build
	cmake -S ./ -B build -DMAZE_TESTING=ON
	cmake --build build

check: clean test_no_run
	cd build/test && leaks --atExit -- ./CPP_MAZE_test

clean:
	rm -rf CPP_MAZE/*.user build test_build CMakeLists.txt.user report docs .clang-format
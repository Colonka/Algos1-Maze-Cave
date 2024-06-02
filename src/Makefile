ARCHIVE = CPP_MAZE

all: install

test:
	mkdir build
	cmake -S ./ -B build -DMAZE_TESTING=ON
	cmake --build build --target check

install:
	mkdir -p build
	cmake -S ./ -B build -DMAZE_TESTING=OFF
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

dist: install
	tar -cf $(ARCHIVE).tar build/*

gcov_report:
	mkdir -p build
	mkdir -p report
	cmake -S ./ -B build -DMAZE_TESTING=ON -DMAZE_COVERAGE=ON
	cmake --build build --target coverage
	open report/report.html
	
git: clean
	git add -A
	git commit -m "$m"
	git push origin "$b"

test_no_run:
	mkdir -p build
	cmake -S ./ -B build -DMAZE_TESTING=ON
	cmake --build build

check: test_no_run
	cd build/test && leaks --atExit -- ./CPP_MAZE_test

clean:
	rm -rf CPP_MAZE/*.user build test_build CMakeLists.txt.user report docs .clang-format
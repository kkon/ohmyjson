ifeq ($(shell uname),Darwin)
BUILD_DIR := "build-clang-osx"
else ifeq ($(shell uname),Linux)
BUILD_DIR := "build-gcc-lunux"
else
BUILD_DIR := "build-windows"
endif


default_target: all

all:
	mkdir -p $(BUILD_DIR);
	cd $(BUILD_DIR) && cmake .. -DCMAKE_BUILD_TYPE=Release && make

test: all
	cd $(BUILD_DIR) && cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON && make
	cd $(BUILD_DIR) && ctest .. -V -VV

bench: all
	cd $(BUILD_DIR) && cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_BENCH=ON && make
	cd $(BUILD_DIR) && ./bench/ohmyjson-bench

dist: all
	tar -cvzf ohmyjson.tar.gz ./CMakeLists.txt ./cmake ./ohmy;

clean:
	rm -rf $(BUILD_DIR);


.PHONY : default_target test clean bench all

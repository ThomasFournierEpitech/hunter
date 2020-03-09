##
## EPITECH PROJECT, 2019
## Makefile
## File description:
## Makefile
##


all:
	mkdir -p build && cd build && conan install .. --build missing && cmake .. -G "Unix Makefiles" && cmake –build .
	make -C ./build

clean:
	rm -rf build
	rm -f CMakeCache.txt

fclean: clean

re: fclean all

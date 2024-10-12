all:
	cd ./stack && make
	cd ./spu && make
	cd ./assembler && make
	cd ./disassembler && make

clean:
	cd ./stack && make clean
	cd ./spu && make clean
	cd ./assembler && make clean
	cd ./disassembler && make clean
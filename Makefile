huffman:
	gcc -g \
	*.h array.c heap.c huffman.c io.c encode.c \
	-o encode

	gcc -g \
	*.h array.c heap.c huffman.c io.c decode.c \
	-o decode


# huffman-encoder #

## How To Use ##

Run 

```
make
``` 

to create the binaries. Then, you can do 

```
./encode file_to_be_encoded encoded_file
```

to write the encoded file. 

To decode the file, do 

```
./decode encoded_file decoded_file
```

If you're running Linux, you can run the following commands to test the program:

```
make && \
mkdir outputs && \ 
cd outputs && \
base64 /dev/urandom | head -c 10000000 > file.txt && \
../encode file.txt encoded_file && \
../decode encoded_file decoded_file.txt && \
ls -s
```

The size of `encoded_file` should always be smaller than `file.txt`, and `file.txt` should always be exactly equal to `decoded_file.txt`.

This program was written to practice C skills. Don't use it for real.


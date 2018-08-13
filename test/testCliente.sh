PWD=$(pwd)
for VARIABLE in $(seq 1 2 200)
do
	time $PWD/bin/client test/dump/hello.c
done
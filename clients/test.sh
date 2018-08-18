PWD=$(pwd)
for VARIABLE in $(seq 1 2 100)
do
	python $PWD/clients/client.py
done

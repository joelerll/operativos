PWD=$(pwd)
for VARIABLE in $(seq 1 2 2)
do
	python $PWD/clients/client.py
done

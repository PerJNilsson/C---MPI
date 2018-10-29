.PHONY : all

EXECS= mpi_djik run1
MPICC?=mpicc


all : ${EXECS}

mpi_djik :
	$(MPICC) -o djikstra mpi.c -lm

run1 : 
	time -p mpirun -n 1 ./djikstra 9 82 /home/hpc2018/a5_grading/test_data/graph_de1_ne3_we2 

run2 : 
	time -p mpirun -n 1 ./djikstra 17 40 /home/hpc2018/a5_grading/test_data/graph_de2_ne4_we2 

run3 : 
	time -p mpirun -n 4 ./djikstra 17 18 /home/hpc2018/a5_grading/test_data/graph_de2_ne4_we2 

run4 : 
	time -p mpirun -n 10 ./djikstra 107 0 /home/hpc2018/a5_grading/test_data/graph_de2_ne5_we2 

run5 : 
	time -p mpirun -n 20 ./djikstra 4 5 /home/hpc2018/a5_grading/test_data/graph_de3_ne5_we3 



clean:
	rm ${EXECS}

gcc_compile : gcc_compile
	gcc notmpi.c -o gcc_copy -O2

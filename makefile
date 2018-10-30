.PHONY : all

EXECS= dijkstra
MPICC?=mpicc


all: dijkstra


dijkstra:
	$(MPICC) -o dijkstra mpi.c -lm

dijkstra :
	$(MPICC) -o dijkstra mpi.c -lm

run1 : 
	time -p mpirun -n 1 ./dijkstra 9 82 /home/hpc2018/a5_grading/test_data/graph_de1_ne3_we2 

run2 : 
	time -p mpirun -n 1 ./dijkstra 17 40 /home/hpc2018/a5_grading/test_data/graph_de2_ne4_we2 

run3 : 
	time -p mpirun -n 4 ./dijkstra 17 18 /home/hpc2018/a5_grading/test_data/graph_de2_ne4_we2 

run4 : 
	time -p mpirun -n 10 ./dijkstra 107 0 /home/hpc2018/a5_grading/test_data/graph_de2_ne5_we2 

run5 : 
	time -p mpirun -n 20 ./dijkstra 4 5 /home/hpc2018/a5_grading/test_data/graph_de3_ne5_we3 

clean:
	rm -rf dijkstra; rm -rf dijkstra.tar.gz; rm -rf reports; rm -rf extracted;

compress:
	@mkdir dijkstra && cp mpi.c dijkstra/mpi.c && cp makefile dijkstra/makefile && cp report.md dijkstra/report.md;\
	cd dijkstra && tar czvf ../dijkstra.tar.gz . && cd -
submit:
	/home/hpc2018/a5_grading/check_submission.py /home/hpcuser071/assignment5/dijkstra.tar.gz

gcc_compile : gcc_compile
	gcc notmpi.c -o gcc_copy -O2

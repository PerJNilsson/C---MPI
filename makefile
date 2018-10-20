.PHONY : all

EXECS=mpi_exe run
MPICC?=mpicc


all : ${EXECS}

mpi_hello_world : mpi_hello_world
	$(MPICC) -o mpi_exe mpi.c

run : run
	mpirun -n 10 ./mpi_exe

clean:
	rm ${EXECS}

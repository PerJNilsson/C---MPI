.PHONY : all

EXECS= mpi_djik run
MPICC?=mpicc


all : ${EXECS}

mpi_djik : mpi_djik
	$(MPICC) -o mpi_djik_exe mpi.c

run : run
	@time mpirun -n 4 ./mpi_djik_exe

clean:
	rm ${EXECS}

gcc_compile : gcc_compile
	gcc notmpi.c -o gcc_copy -O2

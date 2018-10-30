#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int * unvisitedNodes;
int numberOfNodes;
int ** graph;
int * graphRows;
int * weights;
int * valueOfNodes;
int numberOfEdges;
int degrees;
int numberOfProcesses;
int processID;
MPI_Status status;


int dijkstra(int startNode, int endNode);

int main(int argc, char** argv) {
  
  int start_node = strtol(argv[1], NULL, 10);
  int end_node = strtol(argv[2], NULL, 10);
  char * filename = argv[3];

  
  int length = strlen(filename);
  int degree_exp = filename[length-9] - '0';
  int nodes_exp = filename[length-5] - '0';
  int weights_exp = filename[length-1] - '0';
  degrees = pow(10, degree_exp);
  numberOfNodes = pow(10, nodes_exp);

  
  // ***** Initializing the MPI ***** //
  MPI_Init(NULL, NULL);

  // Get the number of processes
  MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

  // Get the rank of the process
  MPI_Comm_rank(MPI_COMM_WORLD, &processID);
  
  numberOfEdges = numberOfNodes*degrees;
  unvisitedNodes =  calloc(numberOfNodes, sizeof(int));
  weights = malloc(sizeof(int)*numberOfEdges);
  valueOfNodes = malloc(sizeof(int)*numberOfNodes);
 
  //printf("degrees: %d \nnodes: %d\nedges: %d\n",degrees, numberOfNodes, numberOfEdges);

  graphRows = malloc(sizeof(int)*2*numberOfEdges);
  graph = malloc(sizeof(int*)*numberOfEdges);
  for (size_t i=0, j=0; i<numberOfEdges; i++, j+=2) {
    graph[i] = graphRows + j;
}


  FILE * fp;
  fp = fopen(filename, "r"); // ("/home/hpc2018/a5_grading/test_data/graph_de1_ne3_we2"
  for (int i=0; i<numberOfEdges; i++) {
    int a,b,c;
    fscanf(fp, "%d %d %d", &a, &b, &c);
    graph[i][0]=a;
    graph[i][1]=b;
    weights[i]=c;
  }
  fclose(fp);

  int shortestPath;
  shortestPath = dijkstra(start_node, end_node);

  if (processID == 0) {
    printf("The shortest path is: %d\n", shortestPath);
  }

  // Finalize the MPI environment.
  MPI_Finalize();

}

int dijkstra(int startNode, int endNode) {
  int largeTenativeInt = INT_MAX;
  int currentNode;
  int tmpNode;
  for (int i=0; i<numberOfNodes; i++) {
    valueOfNodes[i] = largeTenativeInt;
  }

  valueOfNodes[startNode] = 0;
  currentNode = startNode;
  int pair[2];
  int tmp_pair[2];

  for(int node=0; node<numberOfNodes; node++){
    int startIndex = currentNode*degrees;
    for (int i = startIndex; i<startIndex+degrees; i++) {
      if (unvisitedNodes[graph[i][1]] == 0 && graph[i][1]%numberOfProcesses==processID){ 
	int tmpNodeValue = valueOfNodes[currentNode] + weights[i]; // Put it in two if-cases to make
                                                                   // it clearer. Does not make any
                                                                   // runtime difference
	if (tmpNodeValue < valueOfNodes[graph[i][1]]) {
	  valueOfNodes[graph[i][1]] = valueOfNodes[currentNode] + weights[i];
	}
      }
    }
    
    int tmpLowest = INT_MAX;
    for (int ix=processID; ix< numberOfNodes; ix+=numberOfProcesses) {
      if ( unvisitedNodes[ix] == 0 && valueOfNodes[ix] < tmpLowest) {
	tmpLowest = valueOfNodes[ix]; // save the temporary lowest value
	tmpNode = ix; // saves the node with the temporary lowest value,
      }
    }
    pair[0] = tmpLowest; // save the lowest value form each iteration
    pair[1] = tmpNode; // saves the current node with the lowest value,

  
    if (processID != 0) {
      MPI_Send(pair, 2, MPI_INT, 0, 1, MPI_COMM_WORLD); /* Each MPI node has different tmpLowest and
                                                           tmpNode. Now need to send to master to
                                                           find which one we should pick of all the
                                                           MPI workers */
    }else {
      for (int k=1; k<numberOfProcesses; k++) {
	MPI_Recv(tmp_pair, 2, MPI_INT,k, 1, MPI_COMM_WORLD, &status);
	if (tmp_pair[0] < pair[0]) {
	  pair[0] = tmp_pair[0];
	  pair[1] = tmp_pair[1];
        }
      }
    }
    MPI_Bcast(pair, 2, MPI_INT, 0, MPI_COMM_WORLD);

    currentNode = pair[1];    
    unvisitedNodes[currentNode] = 1;
    valueOfNodes[currentNode]= pair[0];
    
    if (unvisitedNodes[endNode] == 1) {
      return valueOfNodes[endNode];
    }

    if (valueOfNodes[currentNode] == INT_MAX){
      return INT_MAX;
    }

  }

  return -1;
}   // End Djikstra funtion

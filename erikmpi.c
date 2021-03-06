//#include <mpi.h>
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
int dijkstra(int startNode, int endNode);

int main(int argc, char** argv) {

  FILE * fp;
  char[] filename = "/home/hpc2018/a5_grading/test_data/graph_de1_ne3_we2";

  int length = strlen(filename);
  
  int degree_exp = filename[length-9] - '0';
  int nodes_exp = filename[length-5] - '0';
  int weights_exp = filename[length-1] - '0';  
    
  fp = fopen(filename, "r");
  

  degrees = pow(10, degree_exp);
  numberOfNodes = pow(10, nodes_exp);
  numberOfEdges = numberOfNodes*degrees;
  unvisitedNodes =  calloc(numberOfNodes, sizeof(int));
  weights = malloc(sizeof(int)*numberOfEdges);
  valueOfNodes = malloc(sizeof(int)*numberOfNodes);

  graphRows = malloc(sizeof(int)*2*numberOfEdges);
  graph = malloc(sizeof(int*)*numberOfEdges);
  for (size_t i=0, j=0; i<numberOfEdges; i++, j+=2) {
    graph[i] = graphRows + j;
  }

  for (int i=0; i<numberOfEdges; i++) {
    int a,b,c;
    fscanf(fp, "%d %d %d", &a, &b, &c);
    graph[i][0]=a;
    graph[i][1]=b;
    weights[i]=c;
  }
  fclose(fp);

  int shortestPath;
  shortestPath = dijkstra(4, 5);

  printf("The shortest path is: %d\n", shortestPath);
  /*
  for (size_t jx=0; jx<numberOfNodes; jx++){
    printf("%d",unvisitedNodes[jx]);
  }
   printf("\n");
  */
   /*  
  // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",
           processor_name, world_rank, world_size);

    // Finalize the MPI environment.
    MPI_Finalize();
   */
}



int dijkstra(int startNode, int endNode) {
  int largeTenativeInt = INT_MAX;
  int currentNode;

  for (int i=0; i<numberOfNodes; i++) {
    valueOfNodes[i] = largeTenativeInt;
  }

  valueOfNodes[startNode] = 0;
  currentNode = startNode;
  
  while (1) {
    
    for (int i = currentNode*degrees; i<currentNode*degrees+degrees; i++) {
      if (unvisitedNodes[graph[i][1]] == 0 &&
          valueOfNodes[currentNode] + weights[i] < valueOfNodes[graph[i][1]]){


            valueOfNodes[graph[i][1]] = valueOfNodes[currentNode] + weights[i];
      }
    }       


    int tmpLowest = largeTenativeInt;
    for (int ix=0; ix< numberOfNodes; ix++) {
      if ( unvisitedNodes[ix] == 0 && valueOfNodes[ix] < tmpLowest) {	
        tmpLowest = valueOfNodes[ix];
        currentNode = ix;      
      }
    }

    unvisitedNodes[currentNode] = 1;
    
    if (currentNode == endNode) {
      return valueOfNodes[endNode];
    }

  } // End Djikstra funtion
}  // after iteratuion now remove current node from unvisited list.

  

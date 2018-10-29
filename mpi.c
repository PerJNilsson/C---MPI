#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
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
#define INFINITY_LENGTH 99999999


int dijkstra(int startNode, int endNode);

int main(int argc, char** argv) {

  // ***** Initializing the MPI ***** //
  MPI_Init(NULL, NULL);

  // Get the number of processes
  MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

  // Get the rank of the process
  MPI_Comm_rank(MPI_COMM_WORLD, &processID);

  for (int i=0; i<numberOfProcesses; i+=numberOfProcesses) {
    printf("ProcessId: %d\n", processID);
  }
  
  degrees = 10;
  numberOfNodes = 1000;
  numberOfEdges = numberOfNodes*degrees;
  unvisitedNodes =  calloc(numberOfNodes, sizeof(int));
  weights = malloc(sizeof(int)*numberOfEdges);
  valueOfNodes = malloc(sizeof(int)*numberOfNodes);
 
  
  graphRows = malloc(sizeof(int)*2*numberOfEdges);
  graph = malloc(sizeof(int*)*numberOfEdges);
  for (size_t i=0, j=0; i<numberOfEdges; i++, j+=2) {
    graph[i] = graphRows + j;
  }

  FILE * fp;
  fp = fopen("/home/hpc2018/a5_grading/test_data/graph_de1_ne3_we2", "r");
  for (int i=0; i<numberOfEdges; i++) {
    int a,b,c;
    fscanf(fp, "%d %d %d", &a, &b, &c);
    graph[i][0]=a;
    graph[i][1]=b;
    weights[i]=c;
  }
  fclose(fp);

  int shortestPath;
  shortestPath = dijkstra(9, 82);

  printf("The shortest path is: %d\n", shortestPath);
    // Finalize the MPI environment.
    MPI_Finalize();
}
// Function for binary search if not all nodes are evenly distributed
int binarySearchFun(int currentNode) {
    int binary_search = 0;
    int bs_check = 1;
    int left = 0; int right = numberOfEdges-1;
      
    
    while (left<=right) {
      //printf("BS: %d \nCN:%d\n", graph[binary_search][0], currentNode);
      binary_search = floor((right+left)/2);
      //printf("%d\n", binary_search);
      if (graph[binary_search][0] < currentNode) {
	left = binary_search+1;
      }
      else if (graph[binary_search][0] > currentNode) {
	right = binary_search -1;
      }
      else  {// (graph[binary_search][0] == currentNode) {
	if (binary_search - degrees < 0) {
	  return 0;
	}
	else {
	  return binary_search-degrees;
	}
      }
    }
    return 0;
}


int dijkstra(int startNode, int endNode) {
  int largeTenativeInt = INFINITY_LENGTH;
  int currentNode;
  for (int i=0; i<numberOfNodes; i++) {
    valueOfNodes[i] = largeTenativeInt;
  }

  valueOfNodes[startNode] = 0;
  currentNode = startNode;
  int pair[2];
  int tmp_pair[2] = {0, 0};
    
  while (1) {
    for (int i = currentNode*degrees; i<currentNode*degrees+degrees; i++) {
      // if (i%comm_size != rank) continue;    
      if (unvisitedNodes[graph[i][1]] == 0){
	int tmpNodeValue = valueOfNodes[currentNode] + weights[i]; // Put it in two if-cases to make it clearer. Does not make any runtime difference
	if (tmpNodeValue < valueOfNodes[graph[i][1]]) {
	  valueOfNodes[graph[i][1]] = valueOfNodes[currentNode] + weights[i];
	}
      }
   
    }
    unvisitedNodes[currentNode] = 1;

    int tmpLowest = INFINITY_LENGTH;
    for (int ix=processID; ix< numberOfNodes; ix+=numberOfProcesses) {
      if ( unvisitedNodes[ix] == 0 && valueOfNodes[ix] < tmpLowest) {
	tmpLowest = valueOfNodes[ix]; // save the temporary lowest value
	currentNode = ix; // saves the node with the temporary lowest value,
      }
    }
    pair[0] = tmpLowest; // save the lowest value form each iteration
    pair[1] = currentNode; // saves the current node with the lowest value,
    
    MPI_Status status;
    if (processID != 0) {
      MPI_Send(pair, 2, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }else {
      for (int k=1; k<numberOfProcesses; k++) {
	MPI_Recv(tmp_pair, 2, MPI_INT,k, 1, MPI_COMM_WORLD, &status);
	if (tmp_pair[0] < pair[1]) {
	  pair[0] = tmp_pair[0];
	  pair[1] = tmp_pair[1];
	}
      }
    }
    if (unvisitedNodes[endNode] == 1) {
      return valueOfNodes[endNode];
      }
    if ( tmpLowest == INFINITY_LENGTH) {
      return INFINITY_LENGTH;
    }
  }
}   // End Djikstra funtion

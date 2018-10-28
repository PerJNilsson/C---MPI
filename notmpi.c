//#include <mpi.h>
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
int dijkstra(int startNode, int endNode);

int main(int argc, char** argv) {
  degrees = 1000;
  numberOfNodes = 100000;
  numberOfEdges = numberOfNodes*degrees;
  unvisitedNodes =  calloc(numberOfNodes, sizeof(int));
  weights = malloc(sizeof(int)*numberOfEdges);
  valueOfNodes = malloc(sizeof(int)*numberOfNodes);
  //lastVisited = malloc(sizeof(int)*numberOfNodes);
 
  
  graphRows = malloc(sizeof(int)*2*numberOfEdges);
  graph = malloc(sizeof(int*)*numberOfEdges);
  for (size_t i=0, j=0; i<numberOfEdges; i++, j+=2) {
    graph[i] = graphRows + j;
  }

  FILE * fp;
  fp = fopen("/home/hpc2018/a5_grading/test_data/graph_de3_ne5_we3", "r");
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
// TODO: use modulo to find first element of the node



int dijkstra(int startNode, int endNode) {
  int largeTenativeInt = 10*numberOfEdges;
  int currentNode;
  for (int i=0; i<numberOfNodes; i++) {
    valueOfNodes[i] = largeTenativeInt;
  }

  valueOfNodes[startNode] = 0;
  currentNode = startNode;
  
  while (1) {
    int binary_search;
    binary_search = binarySearchFun(currentNode);
    
    for (int i = currentNode*degrees; i<currentNode*degrees+degrees; i++) {
      if (graph[i][0] == currentNode && unvisitedNodes[graph[i][1]] == 0){
	int tmpNodeValue = valueOfNodes[currentNode] + weights[i];
	if (tmpNodeValue < valueOfNodes[graph[i][1]]) {
	  valueOfNodes[graph[i][1]] = valueOfNodes[currentNode] + weights[i];
	}
      }
   
      if (graph[i][0] > currentNode) {
	unvisitedNodes[currentNode] = 1;
	i = numberOfEdges;
      }
    }
    unvisitedNodes[currentNode] = 1;

    int tmpLowest = largeTenativeInt;
    for (int ix=0; ix< numberOfNodes; ix++) {
      if ( unvisitedNodes[ix] == 0) {	
	if (valueOfNodes[ix] < largeTenativeInt && valueOfNodes[ix] < tmpLowest) {
	  tmpLowest = valueOfNodes[ix];
	  currentNode = ix;
	}
      }
    }
    //printf("Current node: %d\n Visited: %d \n", currentNode, unvisitedNodes[currentNode]);
    if (unvisitedNodes[endNode] == 1) {
      return valueOfNodes[endNode];
      }
    if ( tmpLowest == 100000) {
      return 999999;
    }
  } // End Djikstra funtion
}  // after iteratuion now remove current node from unvisited list.

  

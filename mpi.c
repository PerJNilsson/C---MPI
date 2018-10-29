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
#define INFINITY_LENGTH 99999999
int dijkstra(int startNode, int endNode);

int main(int argc, char** argv) {

  int start_node = strtol(argv[4], NULL, 10);
  int end_node = strtol(argv[5], NULL, 10);

  char * filename = argv[6];
     
  FILE * fp;
  fp = fopen(filename, "r");
  
  int string_length = strlen(filename);
  int degree_exp = filename[length-9] - '0';
  int nodes_exp = filename[length-5] - '0';
  int weights_exp = filename[length-1] - '0';


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
  shortestPath = dijkstra(9, 82);

  printf("The shortest path is: %d\n", shortestPath);
     
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
  
  while (1) {
    
    for (int i = currentNode*degrees; i<currentNode*degrees+degrees; i++) {
      if (unvisitedNodes[graph[i][1]] == 0){
	int tmpNodeValue = valueOfNodes[currentNode] + weights[i]; // Put it in two if-cases to make it clearer. Does not make any runtime difference
	if (tmpNodeValue < valueOfNodes[graph[i][1]]) {
	  valueOfNodes[graph[i][1]] = valueOfNodes[currentNode] + weights[i];
	}
      }
   
    }
    unvisitedNodes[currentNode] = 1;

    int tmpLowest = INFINITY_LENGTH;
    for (int ix=0; ix< numberOfNodes; ix++) {
      if ( unvisitedNodes[ix] == 0 && valueOfNodes[ix] < tmpLowest) {
	tmpLowest = valueOfNodes[ix];
	currentNode = ix;
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

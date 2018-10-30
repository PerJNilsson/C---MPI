<h1> Assignment 5 - hpcg031 </h2>

<h2> Program layout </h2>

We are starting out by declaring some variables that will be necessary later on in the program and
catching the file name where we extract information about the number of node, edges and
degrees. Next we initialize the MPI and get the number of processes and their ID. We will then read
the file specified in the arguments and save it into a 2-dimensional array, and the edge and weights
in a 1-dimentional array.

After this, we run the function dijkstra. In our dijkstra-funtion we have parallelized two different
parts. The first part is the one that checks if the new path is better than previous by checking the
current node with its neighbours. To be able to parallelize this we're assigning specific nodes to
each worker.

The other parallelizaiton is the one where we iterate through the whole list of, finding the next
current node by looking for the node with the lowest value and checking that it is in the unvisited
list. This is done by all workers sending their candidate to the master where the master iterates
through all candidates, choosing the lowest and then broadcasts the chosen node and its value to all
workers.


<h2> Results </h2>

| Different test | Test 1 | Test 2 | Test 3 | Test 4 | Test 5 |
|----|----|----|----|----|----|
| Degrees | 10 | 100 |	100 | 100 | 1000 |
| Number of vertices |	1000 | 10000 | 10000 | 100000 | 100000 |
| Maximal weight | 100 | 100 |	100 | 100 | 1000 |
| File name | graph_de1_ne3_we2 | graph_de2_ne4_we2 | graph_de2_ne4_we2 | graph_de2_ne5_we2 | graph_de3_ne5_we3 |
| Number of nodes  | 1 | 1 | 4 | 10 | 20 |
| Start point |	9 | 17 | 17 | 107 | 4 |
| End point | 82 | 40 |	18 | 0 | 5 |
| Required time in seconds | 1.2 | 6.8 | 3.1 | 98 | 256 |
| Our results in seconds | 0.60 | 0.87 | 0.79 | 6.10 | 25.60 |
| Path length | 112 | 12 | 8 | 18  | 21 |

<h2> Other comments </h2>

By parallelizing our code, we were able to reduce the running time by 40 %. With one node, our
script would take approximately 40 second for the graph_de3_ne5_we3 while, as one can see in the
result table that it only took 25 with parallelization. The largest difference was made by the last
part explained in the program layout. Where we iterate through the list in order to find a new
current node. By implementing this, we reduced the running time from 40 second to 27 seconds. This
meaning that the other parallelization part where we assigned each worker different nodes only
improved the speed with 2-3 seconds.

One thing that could have been improved is that we are using fscanf on all workers to save their
local copies of the nodes and weight array. By using fscanf on the master node and copy it to all
workers would decrease the load on each thread, but perhaps also a little slower.

Another improvement would be the fact that we are allocating space for
sizeof(int)\*numberOfWorkers\*NumberOfNodes\*degrees\*3 for each worker. Since we have dedicated
workers to each node, the allocation size for each worker could therefore be reduced to
sizeof(int)\*NumberOfNodes\*degrees\*3.

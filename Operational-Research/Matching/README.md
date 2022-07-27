# Hungarian algorithm for minimum cost perfect matching in bipartite graphs
The algorithm expects the following input:
- First line: values n (number of nodes from each class A and B) and m (number of edges in the bipartite graph)
- Next 2n lines, each with m columns, indicating the incidence matrix (Q) of the graph
- The last line contains m values indicating the edge weights

The following is an example of usage in the command line:
```
>>> python main.py
n m
Q_0,0 Q_0,1 ... Q_0,m
...
Q_2n,0 Q_2n,1 ... Q_2n,m
c1 c2 ... cm

```

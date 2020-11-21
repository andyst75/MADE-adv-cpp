#include <iostream>
#include "graph.h"
#include "dfs.h"



int main () {

  using graph =
  Graph<
  Vertex<0, Edge<0, 1>>,
  Vertex<1, Edge<1, 2>, Edge<1, 5>>,
  Vertex<2, Edge<2, 3>, Edge<2, 0>>,
  Vertex<3, Edge<3, 4>>,
  Vertex<4, Edge<4, 1>>,
  Vertex<5, Edge<5, 4>, Edge<5, 6>>,
  Vertex<6, Edge<6, 7>>,
  Vertex<7, Edge<7, 8>>,
  Vertex<8, Edge<8, 6>>
  >;

  std::cout << "Path exists (T): "
      << (PathExists<graph, 5, 0>::value ? "true" : "false") << std::endl;

  std::cout << "Path exists (F): "
      << (PathExists<graph, 7, 5>::value ? "true" : "false") << std::endl;

  std::cout << "Path exists (T): "
      << (PathExists<graph, 0, 0>::value ? "true" : "false") << std::endl;

    return 0;
}

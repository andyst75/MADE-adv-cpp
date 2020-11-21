#ifndef MEANWHILE_IN_THE_SEVENTH_GALAXY_THIRD_VARIANT_DFS_H
#define MEANWHILE_IN_THE_SEVENTH_GALAXY_THIRD_VARIANT_DFS_H

#include "graph.h"
#include "type_list.h"
#include "value_list.h"

template <typename Graph, long start, long end, typename Visit, bool visited>
struct rightDfs;

template <typename Graph, long from, long to, typename Visit, long edge>
struct leftDfs;

template <typename Graph, long start, long end>
struct PathExists {
  static constexpr bool value = rightDfs<Graph, start, end,
      typename Construct<false,
      Length<typename Graph::Vertices>::value>::type, false>::value;
};

template <typename Graph, long start, long end, typename Visit>
struct rightDfs<Graph, start, end, Visit, true> {
  static constexpr bool value = false;
};

template <typename Graph, long start, long end, typename Visit>
struct rightDfs<Graph, start, end, Visit, false> {
  static constexpr bool value = start == end ||
      leftDfs<Graph, start, end, typename Set<start, true, Visit>::type,
              Length<typename TypeAt<start,
                  typename Graph::Vertices>::type::Edges>::value>::value;
};

template <typename Graph, long start, long end, typename Visit>
struct leftDfs<Graph, start, end, Visit, 0> {
  static constexpr bool value = false;
};

template <typename Graph, long start, long end, typename Visit, long edge>
struct leftDfs {
  using next = typename TypeAt<edge - 1,
      typename TypeAt<start, typename Graph::Vertices>::type::Edges>::type;

  static constexpr bool value =
      rightDfs<Graph, next::to, end, Visit,
               Get<next::to, Visit>::value>::value ||
      leftDfs<Graph, start, end, Visit, edge - 1>::value;
};

#endif  /// MEANWHILE_IN_THE_SEVENTH_GALAXY_THIRD_VARIANT_DFS_H.

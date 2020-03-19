#include <algorithm>
#include <bitset>
#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>
#include "polygon.h"
#define BMASK_LIMIT 32
typedef long double ld;
using namespace std;

bitset<BMASK_LIMIT> getRangeMask(int beg, int end, int N=0) {
  if (beg <= end) {
    int msk = (1 << (end+1)) - 1;
    int b = (1 << beg) - 1;
    return bitset<BMASK_LIMIT>(msk ^ b);
  }

  return getRangeMask(beg, N-1) | getRangeMask(0, end);
}

void triangulateAll(SpannerGraph& graph, bitset<BMASK_LIMIT> bmask) {
  int loc_n = bmask.count();
  if (loc_n <= 3) return;

  vector<int> pos;
  for (int i = 0; (int)pos.size() < loc_n; i++) {
    if (bmask.test(i)) {
      pos.push_back(i);
    }
  }

  // removing beg
  graph.addEdge(pos[1], pos.back());
  bmask.flip(pos[0]);
  triangulateAll(graph, bmask);
  bmask.flip(pos[0]);
  graph.removeEdge(pos[1], pos.back());

  // connecting beg to something
  for (int i = 2; i < loc_n - 1; i++) {
    graph.addEdge(pos[0], pos[i]);

    triangulateAll(graph, getRangeMask(pos[0], pos[i], graph.n) & bmask);
    triangulateAll(graph, getRangeMask(pos[i], pos[0], graph.n) & bmask);
    graph.removeEdge(pos[0], pos[i]);
  }
}

int main() {
  // input
  int N;
  cin >> N;
  vector<Point> P(N);
  for (int i = 0; i < N; i++) {
    cin >> P[i].x >> P[i].y;
  }

  // Points are clockwise -- does this matter?
  SpannerGraph graph(P);

  cerr << "About to triangulate" << endl;
  triangulateAll(graph, (1 << graph.n) - 1);

  cout << graph.n << " " << graph.getBestSpannerRatio() << endl;
  graph.printSegments();

  return 0;
}

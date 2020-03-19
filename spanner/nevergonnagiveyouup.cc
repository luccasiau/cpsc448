#include <algorithm>
#include <bitset>
#include <iostream>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>
#include <vector>
#include "polygon.h"
#define BMASK_LIMIT 32
using namespace std;
const ld epsilon = 1e-9;
typedef pair<int, int> pii;

bitset<BMASK_LIMIT> getRangeMask(int beg, int end, int N=0) {
  if (beg <= end) {
    int msk = (1 << (end+1)) - 1;
    int b = (1 << beg) - 1;
    return bitset<BMASK_LIMIT>(msk ^ b);
  }

  return getRangeMask(beg, N-1) | getRangeMask(0, end);
}

void drawEdges(SpannerGraph& graph, bitset<BMASK_LIMIT> bmask) {
  int loc_n = bmask.count();
  if (loc_n <= 3) return;

  vector<int> pos;
  for (int i = 0; (int)pos.size() < loc_n; i++) {
    if (bmask.test(i)) {
      pos.push_back(i);
    }
  }

  ld biggest = -1;
  ld secBiggest = -1;
  pii bigPair = {0, 0};
  pii secPair = {0, 0};

  for (int i = 0; i < (int)pos.size(); i++) {
    int a = pos[i];
    int b = pos[0];
    if (i < (int)pos.size() - 1) b = pos[i+1];

    ld curD = graph.getPointsDistance(a, b);
    if (curD > biggest + epsilon) {
      secBiggest = biggest;
      secPair = bigPair;
      biggest = curD;
      bigPair = {a, b};
    } else if (curD > secBiggest + epsilon) {
      secBiggest = curD;
      secPair = {a, b};
    }
  }

  int a, b, c, d;
  tie(a, b) = bigPair;
  tie(c, d) = secPair;

  // one point in common
  int x = -1, y = -1;
  if (a == c) {
    x = b; y = d;
  } else if (a == d) {
    x = b; y = c;
  } else if (b == c) {
    x = a; y = d;
  } else if (b == d) {
    x = a; y = c;
  }

  if (x != -1) {
    graph.addEdge(x, y);
    drawEdges(graph, getRangeMask(x, y, graph.n) & bmask);
    drawEdges(graph, getRangeMask(y, x, graph.n) & bmask);
    return;
  }

  // no points in common
  // reminder: points are still in clockwise order
  // so, ABCD is in clockwise order
  if (graph.getPointsDistance(a, c) < graph.getPointsDistance(b, d)) {
    graph.addEdge(a, c);
    drawEdges(graph, getRangeMask(a, c, graph.n) & bmask);
    drawEdges(graph, getRangeMask(c, a, graph.n) & bmask);
  } else {
    graph.addEdge(b, d);
    drawEdges(graph, getRangeMask(b, d, graph.n) & bmask);
    drawEdges(graph, getRangeMask(d, b, graph.n) & bmask);
  }
}

int main() {
  srand(time(NULL));
  // input
  int N;
  cin >> N;
  vector<Point> P(N);
  for (int i = 0; i < N; i++) {
    cin >> P[i].x >> P[i].y;
  }

  SpannerGraph graph(P);
  drawEdges(graph, (1 << graph.n) - 1);

  cout << graph.n << " " << graph.getBestSpannerRatio() << endl;
  graph.printSegments();

  return 0;
}
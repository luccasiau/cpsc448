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
using namespace std;
const ld epsilon = 1e-9;

// draw every edge leading from zero
void init0(SpannerGraph& graph) {
  for (int i = 2; i < graph.n - 1; i++) {
    graph.addEdge(0, i);
  }
}

// from a random node to everyone else
void initRand(SpannerGraph& graph) {
  graph.clearEdges();
  int r = rand() % graph.n;
  for (int i = 0; i < graph.n; i++) {
    if (i != r) graph.addEdge(r, i);
  }
}

// initialize edges in zig-zag
void initZigZag(SpannerGraph& graph) {
  int p1 = 0;
  int p2 = 2;
  bool inc = 1;

  while (!graph.hasEdge(p1, p2)) {
    graph.addEdge(p1, p2);
    if (inc) p2++;
    else p1 = (p1 + graph.n - 1) % graph.n;
    inc ^= 1;
  }
}

// assumes a-b-c-d-a exist and a-c too
// consider swapping a-c for b-d
// return true if solution improves
bool shouldFlip(SpannerGraph& graph, int a, int b, int c, int d) {
  ld curSpanner = graph.getBestSpannerRatio();

  graph.removeEdge(a, c);
  graph.addEdge(b, d);

  ld newSpanner = graph.getBestSpannerRatio();
  
  // improvement happened
  if (newSpanner < curSpanner - epsilon) {
    return true;
  }

  graph.removeEdge(b, d);
  graph.addEdge(a, c);
  return false;
}

// returns true if the edges
// a-b, a-c, a-d, b-c, c-d
// all exist
bool isValidFlip(SpannerGraph& graph, int a, int b, int c, int d) {
  return graph.hasEdge(a, b) && graph.hasEdge(a, c) && graph.hasEdge(a, d) &&
         graph.hasEdge(b, c) && graph.hasEdge(c, d);
}

bool allDifferent(int a, int b, int c, int d) {
  if (a == b || a == c || a == d) return false;
  if (b == c || b == d || c == d) return false;
  return true;
}

bool pivotFlip(SpannerGraph& graph) {
  int n = graph.n;
  for (int i = 0; i < n; i++) {
    if (graph.getDegree(i) < 3) continue;

    for (int b = 0; b < n; b++) {
      for (int c = 0; c < n; c++) {
        for (int d = 0; d < n; d++) {
          if (!allDifferent(i, b, c, d)) continue;
          if (!isValidFlip(graph, i, b, c, d)) continue;
          if (shouldFlip(graph, i, b, c, d)) return true;
        }
      }
    }
  }

  return false;
}

bool pivotFlipRandom(SpannerGraph& graph) {
  int n = graph.n;
  vector<int> A(n), B(n), C(n), D(n);
  for (int i = 0; i < n; i++) {
    A[i] = B[i] = C[i] = D[i] = i;
  }

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  shuffle(A.begin(), A.end(), default_random_engine(seed));
  seed++;
  shuffle(B.begin(), B.end(), default_random_engine(seed));
  seed++;
  shuffle(C.begin(), C.end(), default_random_engine(seed));
  seed++;
  shuffle(D.begin(), D.end(), default_random_engine(seed));

  for (auto a : A) {
    for (auto b : B) {
      for (auto c : C) {
        for (auto d : D) {
          if (!allDifferent(a, b, c, d)) continue;
          if (!isValidFlip(graph, a, b, c, d)) continue;
          if (shouldFlip(graph, a, b, c, d)) return true;
        }
      }
    }
   }
  return false;
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

  // Points are clockwise -- does this matter?
  SpannerGraph graph(P);
  //init0(graph);
  //initZigZag(graph);
  //while(pivotFlip(graph));
  for (int T = 0; T < 25; T++) {
    initRand(graph);
    while(pivotFlipRandom(graph));
  }

  cout << graph.n << " " << graph.getBestSpannerRatio() << endl;
  graph.printSegments();

  return 0;
}
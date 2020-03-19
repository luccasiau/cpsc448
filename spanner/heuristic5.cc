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

// pentagon triangulations
// I should probably improve this;
const bool AC[] = {1, 1, 0, 0, 0};
const bool AD[] = {1, 0, 0, 0, 1};
const bool BD[] = {0, 0, 0, 1, 1};
const bool BE[] = {0, 0, 1, 1, 0};
const bool CE[] = {0, 1, 1, 0, 0};

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
  cerr << "random start = " << r << endl;
  for (int i = 0; i < graph.n; i++) {
    if (i != r) graph.addEdge(r, i);
  }
}

// returns true if the edges a-b-c-d-e-a exist
bool isPentagon(SpannerGraph& graph, int a, int b, int c, int d, int e) {
  return graph.hasEdge(a, b) && graph.hasEdge(b, c) && graph.hasEdge(c, d) &&
         graph.hasEdge(d, e) && graph.hasEdge(e, a);
}

bool allDifferent(int a, int b, int c, int d, int e) {
  if (a == b || a == c || a == d || a == e || b == e) return false;
  if (b == c || b == d || c == d || c == e || d == e) return false;
  return true;
}

void clearInnerPentagon(SpannerGraph& graph, int a, int b, int c, int d, int e) {
  graph.removeEdge(a, c);
  graph.removeEdge(a, d);
  graph.removeEdge(b, d);
  graph.removeEdge(b, e);
  graph.removeEdge(c, e);
}

void addInnerPentagonTriangulation(SpannerGraph& graph, int k, int a, int b,
                                   int c, int d, int e) {
  if (AC[k]) graph.addEdge(a, c);
  if (AD[k]) graph.addEdge(a, d);
  if (BD[k]) graph.addEdge(b, d);
  if (BE[k]) graph.addEdge(b, e);
  if (CE[k]) graph.addEdge(c, e);
}

int getPentagonTriangulationId(SpannerGraph& graph, int a, int b, int c,
                               int d, int e) {
  for (int i = 0; i < 5; i++) {
    if (graph.hasEdge(a, c) != AC[i]) continue;
    if (graph.hasEdge(a, d) != AD[i]) continue;
    if (graph.hasEdge(b, d) != BD[i]) continue;
    if (graph.hasEdge(b, e) != BE[i]) continue;
    if (graph.hasEdge(c, e) != CE[i]) continue;
    return i;
  }
  return 0;  // this should never happen
}

bool shouldFlip(SpannerGraph& graph, int a, int b, int c, int d, int e) {
  ld curSpanner = graph.getBestSpannerRatio();

  int curK = getPentagonTriangulationId(graph, a, b, c, d, e); 

  int bestK = curK;
  for (int k = 0; k < 5; k++) {
    if (k == curK) continue;

    clearInnerPentagon(graph, a, b, c, d, e);
    addInnerPentagonTriangulation(graph, k, a, b, c, d, e);

    if (graph.getBestSpannerRatio() > curSpanner + epsilon) {
      curSpanner = graph.getBestSpannerRatio();
      bestK = k;
    }
  }

  clearInnerPentagon(graph, a, b, c, d, e);
  addInnerPentagonTriangulation(graph, bestK, a, b, c, d, e);
  return bestK != curK;
}

bool pivotFlipRandom(SpannerGraph& graph) {
  int n = graph.n;
  vector<int> A(n), B(n), C(n), D(n), E(n);
  for (int i = 0; i < n; i++) {
    A[i] = B[i] = C[i] = D[i] = E[i] = i;
  }

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  shuffle(A.begin(), A.end(), default_random_engine(seed));
  seed++;
  shuffle(B.begin(), B.end(), default_random_engine(seed));
  seed++;
  shuffle(C.begin(), C.end(), default_random_engine(seed));
  seed++;
  shuffle(D.begin(), D.end(), default_random_engine(seed));
  seed++;
  shuffle(E.begin(), E.end(), default_random_engine(seed));

  for (auto a : A) {
    for (auto b : B) {
      for (auto c : C) {
        for (auto d : D) {
          for (auto e : E) {
            if (!allDifferent(a, b, c, d, e)) continue;
            if (!isPentagon(graph, a, b, c, d, e)) continue;
            if (shouldFlip(graph, a, b, c, d, e)) return true;
          }
        }
      }
    }
   }
  return false;
}

int main() {
  // input
  srand(time(NULL));
  int N;
  cin >> N;
  vector<Point> P(N);
  for (int i = 0; i < N; i++) {
    cin >> P[i].x >> P[i].y;
  }

  // Points are clockwise -- does this matter?
  SpannerGraph graph(P);
  for (int T = 0; T < 25; T++) {
    initRand(graph);
    while(pivotFlipRandom(graph));
  }

  cout << graph.n << " " << graph.getBestSpannerRatio() << endl;
  graph.printSegments();

  return 0;
}
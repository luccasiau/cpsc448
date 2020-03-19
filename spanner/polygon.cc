#include <cmath>
#include <iostream>
#include <vector>
#include "polygon.h"
using namespace std;
typedef long double ld;
const ld INF = 1e15;

ld Point::dist(Point p, Point q) {
  return sqrt((p.x - q.x) * (p.x - q.x) + (p.y - q.y) * (p.y - q.y));
}

SpannerGraph::SpannerGraph(vector<Point> p) : points(p) {
  n = (int)p.size();
  numEdges = 0;
  numTriangulations = 0;
  adj = vector<vector<bool>>(n, vector<bool>(n, false));
  dist = vector<vector<ld>>(n, vector<ld>(n, INF));
  bestSpanner = INF;
  degree = vector<int>(n, 0);

  for (int i = 0; i < n; i++) {
    addEdge(i, i + 1);
  }
}

void SpannerGraph::addEdge(int a, int b) {
  a %= n;
  b %= n;
  if (adj[a % n][b % n]) return;
  
  numEdges++;
  degree[a]++;
  degree[b]++;
  adj[a][b] = true;
  adj[b][a] = true;

  if (numEdges == 2 * n - 3) {
    numTriangulations++;
    measureAndUpdateSpanner();
  }
}

void SpannerGraph::clearEdges() {
  numEdges = n;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j-1 || i == j+1) continue;
      if ((i-j) == n-1 || (j-i) == n-1) continue;
      adj[i][j] = false;
    }
  }
}

void SpannerGraph::removeEdge(int a, int b) {
  a %= n;
  b %= n;
  if (!adj[a % n][b % n]) return;
  
  numEdges--;
  degree[a]--;
  degree[b]--;
  adj[a][b] = false;
  adj[b][a] = false;
}

int SpannerGraph::getDegree(int a) {
  return degree[a];
}

int SpannerGraph::getTriangulations() {
  return numTriangulations;
}

ld SpannerGraph::getBestSpannerRatio() {
  return bestSpanner;
}

vector<vector<bool>> SpannerGraph::getBestSpannerGraph() {
  return bestGraph;
}

bool SpannerGraph::hasEdge(int a, int b) {
  return adj[a][b];
}

void SpannerGraph::printSegments() {
  cerr << "numTriangulations = " << numTriangulations << endl;
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (!bestGraph[i][j]) continue;
      cout << points[i].x << " " << points[i].y << " ";
      cout << points[j].x << " " << points[j].y << endl;
    }
  }
}

void SpannerGraph::measureAndUpdateSpanner() {
  ld cur = measureSpanner();
  if (cur >= bestSpanner)
    return;

  bestSpanner = cur;
  bestGraph = adj;
}

ld SpannerGraph::measureSpanner() {
  buildGraph();
  floydWarshall();

  ld ratio = 1.0;
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      ratio = max(ratio, dist[i][j] / Point::dist(points[i], points[j]));
    }
  }

  return ratio;
}

void SpannerGraph::buildGraph() {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      dist[i][j] = adj[i][j] ? Point::dist(points[i], points[j]) : INF;
    }
  }
}

void SpannerGraph::floydWarshall() {
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
      }
    }
  }
}

ld SpannerGraph::getPointsDistance(int a, int b) {
  a %= n;
  b %= n;
  return Point::dist(points[a], points[b]);
}
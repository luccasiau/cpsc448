#include <algorithm>
#include <bitset>
#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>
#define BMASK_LIMIT 32
typedef long double ld;
using namespace std;
const ld INF = 1e15;

class Point {
  public:
    ld x, y;
    
    Point(ld x_=0, ld y_=0): x(x_), y(y_) {}

    static ld dist(Point p, Point q) {
      return sqrt( (p.x - q.x)*(p.x - q.x) + (p.y - q.y)*(p.y - q.y));
    }
};

class SpannerGraph {
  public:
    int n;
    
    SpannerGraph(vector<Point> p): points(p) {
      n = (int)p.size();
      numEdges = 0;
      numTriangulations = 0;
      adj = vector<vector<bool>>(n, vector<bool>(n, false));
      dist = vector<vector<ld>>(n, vector<ld>(n, INF));

      for (int i = 0; i < n; i++) {
        addEdge(i, i+1);
      }
    }

    void addEdge(int a, int b) {
      if (!adj[a % n][b % n]) numEdges++;

      adj[a % n][b % n] = true;
      adj[b % n][a % n] = true;

      if (numEdges == 2*n-3) {
        numTriangulations++;
        measureAndUpdateSpanner();
      }
    }

    void removeEdge(int a, int b) {
      if (adj[a % n][b % n]) numEdges--;

      adj[a % n][b % n] = false;
      adj[b % n][a % n] = false;
    }

    int getTriangulations() {
      return numTriangulations;
    }

    ld getBestSpannerRatio() {
      return bestSpanner;
    }

    vector<vector<bool>> getBestSpannerGraph() {
      return bestGraph;
    }

    void printSegments() {
      cerr << "numTriangulations = " << numTriangulations << endl;
      for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
          if (!bestGraph[i][j]) continue;
          cout << points[i].x << " " << points[i].y << " ";
          cout << points[j].x << " " << points[j].y << endl;
        }
      }
    }
  
  private:
    int numEdges;
    vector<Point> points;
    vector<vector<bool>> adj;
    vector<vector<ld>> dist;

    // best information
    ld bestSpanner = INF;
    vector<vector<bool>> bestGraph;

    int numTriangulations;

    void measureAndUpdateSpanner() {
      ld cur = measureSpanner();
      if (cur >= bestSpanner) return;

      bestSpanner = cur;
      bestGraph = adj;
    }

    ld measureSpanner() {
      buildGraph();
      floydWarshall();

      ld ratio = 1.0;
      for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
          ratio = max(ratio, dist[i][j]/Point::dist(points[i], points[j]));
        }
      }

      return ratio;
    }

    void buildGraph() {
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          dist[i][j] = adj[i][j] ? Point::dist(points[i], points[j]) : INF;
        }
      }
    }

    void floydWarshall() {
      for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
          for (int j = 0; j < n; j++) {
            dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
          }
        }
      }
    }
};

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
  triangulateAll(graph, (1 << graph.n) - 1);

  cout << graph.n << " " << graph.getBestSpannerRatio() << endl;
  graph.printSegments();

  return 0;
}

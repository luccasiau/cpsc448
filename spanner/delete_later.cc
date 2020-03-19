#include <cmath>
#include <vector>
using namespace std;
typedef long double ld;
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
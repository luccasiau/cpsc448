#include <cmath>
#include <vector>
using namespace std;
typedef long double ld;

class Point {
  public:
    ld x, y;
    
    Point(ld x_=0, ld y_=0): x(x_), y(y_) {}

    static ld dist(Point p, Point q);
};

class SpannerGraph {
  public:
    int n;
    int numEdges;
    
    SpannerGraph(vector<Point> p);
    void addEdge(int a, int b);
    void removeEdge(int a, int b);
    int getTriangulations();
    ld getBestSpannerRatio();
    vector<vector<bool>> getBestSpannerGraph();
    void printSegments();
    int getDegree(int a);
    bool hasEdge(int a, int b);
    void clearEdges();
    ld getPointsDistance(int a, int b);

  private:
    vector<Point> points;
    vector<vector<bool>> adj;
    vector<vector<ld>> dist;
    vector<int> degree;

    // best information
    ld bestSpanner;
    vector<vector<bool>> bestGraph;

    int numTriangulations;

    void measureAndUpdateSpanner();

    ld measureSpanner();
    void buildGraph();

    void floydWarshall();
};
from graph import Graph
import landmarks

g = Graph()
g.build_from_csv('Seoul_Edgelist.csv')
g.select_landmarks()
print(g.landmarks)
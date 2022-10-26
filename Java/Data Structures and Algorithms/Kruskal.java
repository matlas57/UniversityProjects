import java.util.*;

public class Kruskal{

    public static WGraph kruskal(WGraph g)
    {
    	int numSets = g.getNbNodes();
    	DisjointSets sets = new DisjointSets(numSets);
        ArrayList<Edge> sorted = g.listOfEdgesSorted();
        WGraph mst = new WGraph();
        for (Edge e : sorted)
        {
        	//System.out.println(e.nodes[0] + " " + e.nodes[1]);
        	if (IsSafe(sets, e) && !mst.listOfEdgesSorted().contains(e)) // if the edge is safe and not yet in the MST
        	{
        		mst.addEdge(e);												// add the edge to the MST
        		sets.union(e.nodes[0], e.nodes[1]);							// merge the sets that have just been connected
        	}
        }
        return mst;
    }

    public static Boolean IsSafe(DisjointSets p, Edge e)
    {
        if (p.find(e.nodes[0]) != p.find(e.nodes[1]))	// an edge is safe if the two vertices are in different disjoing sets
        {
        	return true;
        }
        return false;
    
    }

    public static void main(String[] args){

        String file = "g1.txt";
        WGraph g = new WGraph(file);
        WGraph t = kruskal(g);
        System.out.println(t);

   } 
}

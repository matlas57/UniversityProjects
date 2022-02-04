import java.util.*;

public class BellmanFord{

    private int[] distances = null;
    private int[] predecessors = null;
    private int source;

    class BellmanFordException extends Exception{
        public BellmanFordException(String str){
            super(str);
        }
    }

    class NegativeWeightException extends BellmanFordException{
        public NegativeWeightException(String str){
            super(str);
        }
    }

    class PathDoesNotExistException extends BellmanFordException{
        public PathDoesNotExistException(String str){
            super(str);
        }
    }

    BellmanFord(WGraph g, int source) throws NegativeWeightException{
        /* Constructor, input a graph and a source
         * Computes the Bellman Ford algorithm to populate the
         * attributes 
         *  distances - at position "n" the distance of node "n" to the source is kept
         *  predecessors - at position "n" the predecessor of node "n" on the path
         *                 to the source is kept
         *  source - the source node
         *
         *  If the node is not reachable from the source, the
         *  distance value must be Integer.MAX_VALUE
         */
    	distances = new int[g.getNbNodes()];									// Initialize the array to store distances to each node to the correct size 
       	for (int i = 0; i < distances.length; i++)								// store max path estimate for all nodes
    	{
    		distances[i] = Integer.MAX_VALUE;
    	}
       	distances[source] = 0;													// distance from source to source is 0
//    	System.out.println("Initialized the distances array");
    	predecessors = new int[g.getNbNodes()];									// Initialize the predecessors to the correct size												// predecessor
    	predecessors[source] = source;											// Predecessor or source is source
    	ArrayList<Edge> edges = g.getEdges();									// get all the edges in the graph
    	for (Edge e : edges)													// go through all edges and relax
    	{
//    		System.out.println("(" + e.nodes[0] + ", " + e.nodes[1] + ")");
    		if (distances[e.nodes[1]] > distances[e.nodes[0]] + e.weight)
    		{
    			distances[e.nodes[1]] = distances[e.nodes[0]] + e.weight;
    			predecessors[e.nodes[1]] = e.nodes[0]; 
//    			System.out.println("relaxed");
    		}
    	}
    	for (Edge e : edges)														// second iteration to see if a negative weight cycle exists
    	{
    		if (distances[e.nodes[1]] > distances[e.nodes[0]] + e.weight)
    		{
    			throw new NegativeWeightException("Found negative weight cycle");
    		}
    	}
    }

    public int[] shortestPath(int destination) throws PathDoesNotExistException{
        /* Returns the list of nodes along the shortest path from 
         * the object source to the input destination
         * If not path exists an Error is thrown
         */
    	if (distances[destination] == Integer.MAX_VALUE)		// if destination cant be reached theres no path TEST THIS CASE
    	{
    		throw new PathDoesNotExistException("No path");
    	}
    	ArrayList<Integer> path = new ArrayList<Integer>();		// initially store the path in an arraylist to make manipulation easier
    	int cur = destination;									// start at the destination			
    	while (cur != source)									// track predecessors backward and add them to the path
    	{
    		path.add(cur);
    		cur = predecessors[cur];		
    	}
    	path.add(source);										// add the source to the path since its required 
    	Collections.reverse(path);								// reverse the array so the path is not backward anymore
    	int[] pathArray = new int[path.size()];					// copy the arraylist into an array
    	for (int i = 0; i < pathArray.length; i++)
    	{
    		pathArray[i] = path.get(i);
    		System.out.println(pathArray[i]);
    	}
    	return pathArray;
    }

    public void printPath(int destination){
        /* Print the path in the format s->n1->n2->destination
         * if the path exists, else catch the Error and 
         * prints it
         */
        try {
            int[] path = this.shortestPath(destination);
            for (int i = 0; i < path.length; i++){
                int next = path[i];
                if (next == destination){
                    System.out.println(destination);
                }
                else {
                    System.out.print(next + "-->");
                }
            }
        }
        catch (Exception e){
            System.out.println(e);
        }
    }

    public static void main(String[] args){
        String file = args[0];
        WGraph g = new WGraph(file);
        try{
            BellmanFord bf = new BellmanFord(g, g.getSource());
            bf.printPath(g.getDestination());
        }
        catch (Exception e){
            System.out.println(e);
        }

   } 
}


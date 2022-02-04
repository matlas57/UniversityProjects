import java.util.*;

public class FordFulkerson 
{

	public static ArrayList<Integer> pathDFS(Integer source, Integer destination, WGraph graph)
	{
		ArrayList<Integer> path = new ArrayList<Integer>();		// Arraylist to store path found by method
		ArrayList<Integer> visited = new ArrayList<Integer>();	// Visited array needed for DFS
		Stack<Integer> stack = new Stack<Integer>();			// create a stack for DFS since it cant be done recursively with current setup
		stack.push(source);										// push the source onto the stack so that its not empty for while loop
		while (!stack.isEmpty())								// go until the stack is empty (or if a path is found)
		{
			Integer vertex = stack.pop();								// pop current vertex from the stack
			if (vertex == destination)									// if the destination is found
			{			
				path.add(vertex);											// add destination as required
				return path;												// return the path		
			}
			else if (!visited.contains(vertex))							// if the current vertex hasnt been visited
			{
				int outDegree = 0;											// store the outdegree of the current vertex 
				for (Edge e : graph.getEdges())								// iterage through neighbors of the current edge
				{
					if (e.nodes[0] == vertex && e.weight > 0)					// if the edge is valid for flow to be pushed through it
					{
						int v2 = e.nodes[1];										// store the neighbor vertex
						if (v2 == destination)										/* if the neighbor is the destination add both the current vertex and 
																						and the destination and return */
						{
							path.add(vertex);
							path.add(v2);
							return path;
						}
						stack.push(e.nodes[1]);										// if the neighbor isnt the destination push it on the stack
						outDegree++;												// increment the outdegree of the current vertex if a neighbor is added
					}
				}
				if (outDegree != 0)										// if the current vertex has positive outdegree then the vertex should be added to the path
				{
					path.add(vertex);
				}
			}
		visited.add(vertex);									// mark the vertex as visited	
		}
		return new ArrayList<Integer>();						// if no path is found return empty list
	}



	public static String fordfulkerson(WGraph graph){
		String answer = "";
		int maxFlow = 0;
		WGraph residual = new WGraph(graph);																	// create a residual graph by duplicating the original graph
		while (pathDFS(residual.getSource(), residual.getDestination(), residual).size() != 0)					// while there is a path in the residual
		{
			ArrayList<Integer> path = pathDFS(residual.getSource(), residual.getDestination(), residual);		// store the path to augment
			//System.out.println(path.toString());
			ArrayList<Edge> edges = residual.getEdges();														/* store the edges of the residual path thay will be used 
																													to find the edges specific to the path */
			ArrayList<Edge> pathEdges = new ArrayList<Edge>();													// new arraylist for only the augmenting path edges
			int bottleneck = Integer.MAX_VALUE;																	/* create an initial bottleneck value that will always be 
																													greater than the actual bottleneck which will be
																													decreased to the true bottleneck value in the following
																													loop */
			for (Edge e : edges)																				// go through all the edges
			{		
				for (int i = 0; i < path.size() - 1; i++)															// go through the path to match with the edges
				{
					if (e.nodes[0] == path.get(i) && e.nodes[1] == path.get(i + 1))									// if the current edge is in the path
					{
						pathEdges.add(e);																			// store in path edges
						if (e.weight < bottleneck)																	// modify bottleneck if necessary
						{
							bottleneck = e.weight;
						}
					}
				}
				
			}
			// the path edges have been stored next modify the residual graph for the next iteration
			for (Edge e : pathEdges)																			// iterate through the edges again		
			{
				if (residual.getEdges().contains(e))															// only the augmenting path edges need to be changed
				{
					residual.setEdge(e.nodes[0], e.nodes[1], e.weight - bottleneck);							// modify the forward edge to have less capacity
					if (residual.getEdge(e.nodes[1], e.nodes[0]) != null)										// if a back edge already exists modify it if not add it
					{
						residual.setEdge(e.nodes[1], e.nodes[0], residual.getEdge(e.nodes[1], e.nodes[0]).weight + bottleneck);
					}
					else 
					{
						residual.addEdge(new Edge(e.nodes[1], e.nodes[0], bottleneck));
					}
				}
			}
			maxFlow += bottleneck;
		}
		answer += maxFlow + "\n" + graph.toString();	
		return answer;
	}
	

	 public static void main(String[] args){
		 String file = args[0];
		 WGraph g = new WGraph(file);
//		 System.out.println(pathDFS(0, 5, g));
		 
		 System.out.println(fordfulkerson(g));
	 }
}


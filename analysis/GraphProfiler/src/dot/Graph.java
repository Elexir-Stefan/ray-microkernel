package dot;

import java.util.ArrayList;


public class Graph {
	
	private ArrayList<Node> nodes;
	
	public Graph() {
		this.nodes = new ArrayList<Node>();
	}
	
	/**
	 * Decides whether a given name is part of the graph
	 * @param label
	 * @return
	 */
	public Node ContainsNode(String name) {
		for(Node n : this.nodes) {
			if (n.name.equals(name)) {
				return n;
			}
		}
		return null;
	}
	
	/**
	 * Determindes whether nodes <code>from</code> and
	 * <code>to</code> are directly (not transitively!)
	 * connected.
	 * @param from
	 * @param to
	 * @return
	 */
	public Edge DirectlyConnected(Node from, String via, Node to) {
		if (this.nodes.contains(from)) {
			return from.GetConnection(via, to);
		} else {
			return null;
		}
	}
	
	public void AddNode(Node node) {
		this.nodes.add(node);
	}

	public ArrayList<Node> GetNodes() {
		return this.nodes;
	}
}
	


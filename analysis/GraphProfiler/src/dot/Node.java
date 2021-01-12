package dot;

import java.util.ArrayList;


/**
 * Represents a DOT node, that is capable of having child struct parts
 * @author stefan
 *
 */
public class Node {
	
	public class SubNode {
		// Fields
		public String subName;
		public String subLabel;
		private Node parent;
		
		// Getter
		public Node GetParent() {
			return this.parent;
		}
		
		/**
		 * Constructor
		 * @param parent
		 * @param subLabel
		 */
		public SubNode(Node parent, String subLabel) {
			this.parent = parent;
			this.subLabel = subLabel;
			this.subName = "S" + subLabel;
		}
		
		@Override
		public boolean equals(Object other) {
			if (other instanceof SubNode) {
				return equals((SubNode)other);
			} else {
				return false;
			}
		}
		
		public boolean equals(SubNode other) {
			return (other.subName.equals(this.subName));
		}
	}
	
	
	public String name;
	public String label;
	
	private ArrayList<Edge> outgoing;
	

	public Node(String name, String label) {
		this.name = name;
		this.label = label;
		this.outgoing = new ArrayList<Edge>();
	}
	
	/**
	 * Adds a new edge to the node starting at the subnode
	 * labelled <code>fromSub</code>.
	 * @param fromSub
	 * @param target
	 */
	public Edge AddEdge(String fromSub, Node target) {
		Edge edge = new Edge(new SubNode(this, fromSub), target);
		this.outgoing.add(edge);
		
		return edge;
	}
	
	public ArrayList<Edge> GetOutgoingEdges() {
		return this.outgoing;
	}
	
	public ArrayList<SubNode> GetSubNodes() {
		ArrayList<SubNode> unique = new ArrayList<SubNode>();
		for(Edge e : this.outgoing) {
			if (!unique.contains(e.sourceSubNode)) {
				unique.add(e.sourceSubNode);
			}
		}
		
		return unique;
	}
	
	/**
	 * Returns whether this node is *somehow* connected
	 * to aother node <code>other</code>
	 * @param other
	 * @return
	 */
	public Edge GetConnection(Node other) {
		for(Edge e : this.outgoing) {
			if (e.target.equals(other)) {
				return e;
			}
		}
		return null;
	}
	
	/**
	 * Returns whether this node is connected
	 * via <code>exit/<code>
	 * to another node <code>other</code>
	 * @param other
	 * @return
	 */
	public Edge GetConnection(String exit, Node other) {
		for(Edge e : this.outgoing) {
			if (e.target.equals(other) && e.sourceSubNode.subLabel.equals(exit)) {
				return e;
			}
		}
		return null;
	}

	
	/**
	 * Produces DOT code for a node
	 * @return
	 */
	public String GetDotCode() {
		StringBuilder subNodes = new StringBuilder();
		boolean firstSubNode = true;
		for(SubNode n : GetSubNodes()) {
			if (!firstSubNode) {
				subNodes.append("|");
			} else {
				firstSubNode = false;
			}
			subNodes.append("<");
			subNodes.append(n.subName);
			subNodes.append("> ");
			subNodes.append(n.subLabel);
		}
		return "\"" + this.name + "\" [label=\"{" + this.label + "|{" + subNodes.toString() + "}}\", shape=record, fontsize=8]";
	}
	
	@Override
	public boolean equals(Object other) {
		if (other instanceof Node) {
			return equals((Node)other);
		} else {
			return false;
		}
	}
	
	public boolean equals(Node other) {
		return this.name.equals(other.name);
	}

	
	
}

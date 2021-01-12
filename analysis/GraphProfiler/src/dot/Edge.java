package dot;

import java.util.ArrayList;

import dot.Node.SubNode;


public class Edge {
	public SubNode sourceSubNode;
	public Node target;
	
	public ArrayList<Long> executionTimes;
	
	private int hitCount = 1;
	/**
	 * Increases the hit count of this node (function) by 1
	 */
	public void hit() {
		this.hitCount++;
	}
	
	public int GetHitCount() {
		return this.hitCount;
	
	}
	
	
	public boolean marked = false;
	
	public Edge(SubNode from, Node to) {
		this.sourceSubNode = from;
		this.target = to;
		
		this.executionTimes = new ArrayList<Long>();
	}
	
	public String GetDotCode(int width, int red, int green, int blue) {
		return GetDotCode(
				"\"" + this.sourceSubNode.GetParent().name + "\":\"" + this.sourceSubNode.subName + "\"", 
				"\"" + this.target.name + "\"", 
				width, 
				String.valueOf(this.hitCount) + "[" + String.valueOf(this.GetAverageExecutionTime()) + "]", 
				red, green, blue); 
	}
	
	private String GetDotCode(String fromNode, String toNode, int width, String label, int red, int green, int blue) {
		return String.format("%s -> %s [style=\"setlinewidth(%d)\" color=\"#%02x%02x%02x\",label=\"%s\"]", fromNode, toNode, width, red, green, blue,label);
	}

	public void AddExecutionTime(long executionTime) {
		this.executionTimes.add(executionTime);
		
	}
	
	public long GetTotalExecutionTime() {
		return this.GetAverageExecutionTime() * this.hitCount;
	}
	
	public long GetAverageExecutionTime() {
		double mean = 0.0;
		for(Long l : this.executionTimes) {
			mean += (double)l;
		}
		
		return (long)(mean / (double)this.executionTimes.size());
	}
}
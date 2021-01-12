package analysis;

import java.util.ArrayList;
import java.util.Stack;

import javax.activity.InvalidActivityException;

import dot.Edge;
import dot.Graph;
import dot.Node;

import parser.FunctionTable;
import parser.ProfilerParser;
import parser.FunctionTable.InvocationSource;
import parser.ProfilerParser.Invocation;


/**
 * Manages the representation and analysis of the gathered data
 * and transforms it to edges ready for DOT output
 * @author stefan
 *
 */
public class CallGraph {
	
	public class NamedInvocation {
		
		int callNum;
	
		public FunctionTable.InvocationSource caller, callee;
		
		public long TSC;
		
		public int depth;
		
		public long callerAbsoluteAddress;
		public long executionTime = 0;
		
		public NamedInvocation(int depth, Invocation inv, FunctionTable table, int callNum) throws InvalidActivityException {
			this.callNum = callNum;
			this.depth = depth;
			this.caller = table.getInvocationByAddress(inv.caller);
			this.callee = table.getInvocationByAddress(inv.callee);
			
			this.TSC = inv.TSC;
		}

		public String GenerateCallerNodeName(InvocationSource source) {

			return "N" + source.functionName + "_" + this.callNum;
		}

		public Node CreateNode(InvocationSource source) {
			return new Node(
					GenerateCallerNodeName(source),
					source.functionName + "\\n[" + Long.toString(source.functionStart, 16) + "]");
		}
		
	}
	
	private ProfilerParser profiler;
	private FunctionTable table;
	
	public CallGraph(ProfilerParser profiler, FunctionTable table) throws InvalidActivityException {
		this.profiler = profiler;
		this.table = table;
	}

	private ArrayList<NamedInvocation> GetCallHistory(String monitorFunction) throws InvalidActivityException {
		ArrayList<NamedInvocation> result = new ArrayList<NamedInvocation>();
		Stack<NamedInvocation> callStack = new Stack<NamedInvocation>();
		
		boolean monitor = true;
		boolean separation = false;
		if (monitorFunction != null) {
			monitor = false;
			separation = monitorFunction.startsWith("!");
			if (separation) {
				monitorFunction = monitorFunction.substring(1);
			}
		}
		int separateCall = 0;
		
		
		for(Invocation i : this.profiler) {
			
			// investigate call direction. Actual call or frame drop?
			if (i.frameDrop) {
				// frame drop
				if (!callStack.empty()) {
					NamedInvocation done = callStack.pop();
					done.executionTime = i.TSC - done.TSC;
					// if monitoring enabled, check for drop below frame
					// to be monitored
					if ((monitorFunction != null) && (done.callee.functionName.equals(monitorFunction))) {
						monitor = false;
						if (separation) separateCall++;
					}
				} else {
					// If monitoring enabled, disable it if we have reached
					// the last call stack frame
					if (monitorFunction != null) {
						monitor = false;
						if (separation) separateCall++;
					}
				}
			} else {
				
				NamedInvocation nInv = new NamedInvocation(callStack.size(), i, this.table, separateCall);
				
				if ((monitorFunction != null) && (nInv.caller.functionName.equals(monitorFunction))) {
					monitor = true;
				}
				
				if (monitor) {
					result.add(nInv);
				}
				callStack.push(nInv);
			}
		}
		
		return result;
	}
	
	/**
	 * Converts the invocations to a graph
	 * @return
	 * @throws InvalidActivityException 
	 */
	public Graph GetGraph(String monitorFunction) throws InvalidActivityException {
		return GetGraph(GetCallHistory(monitorFunction));
	}
	
	/**
	 * Converts the invocations to a graph
	 * @return
	 */
	public Graph GetGraph(ArrayList<NamedInvocation> invocations) {
		Graph graph = new Graph();
		
		for(NamedInvocation inv : invocations) {
			Node callerNode = graph.ContainsNode(inv.GenerateCallerNodeName(inv.caller));
			Node calleeNode = graph.ContainsNode(inv.GenerateCallerNodeName(inv.callee));
			String callerExit = Long.toString(inv.caller.invocationOffset, 16);
			
			if (callerNode == null) {
				// it doesn't exist -> add it first
				callerNode = inv.CreateNode(inv.caller);
				graph.AddNode(callerNode);
			}
			
			if (calleeNode == null) {
				// it doesn't exist -> add it first
				calleeNode = inv.CreateNode(inv.callee);
				graph.AddNode(calleeNode);
			}
			
			// From here on, both nodes exist for sure:
			Edge callArc = graph.DirectlyConnected(callerNode, callerExit, calleeNode);
			if (callArc != null) {
				// Already called... Increase hit count
				callArc.hit();
			} else {
				// Doesn't exist yet
				callArc = callerNode.AddEdge(callerExit, calleeNode);
			}
			
			callArc.AddExecutionTime(inv.executionTime);

		}
		
		return graph;
	}

}

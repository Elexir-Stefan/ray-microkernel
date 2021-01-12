package analysis;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Collections;
import parser.FunctionTable;
import parser.ProfilerParser;
import dot.DotCode;
import dot.Edge;
import dot.Graph;
import dot.Node;

public class Main {

	public static void main(String args[]) {
		if (args.length < 2) {
			ShowUsage();
		} else {
			try {
				InputStream inStream;
				if (args[1].equals("-")) {
					inStream = System.in;
				} else {
					inStream = new FileInputStream(args[1]);
				}
				ProfilerParser parser = new ProfilerParser(inStream);
				FunctionTable table = new FunctionTable(args[0]);
				
				CallGraph analysisGraph = new CallGraph(parser, table);
				
				Graph graph = analysisGraph.GetGraph(args.length >= 3? args[2] : null);
				DotCode.convertToDot(graph, System.out);
				
				PrintStatistics(graph, System.err);
				
			} catch (FileNotFoundException e) {
				System.out.println("Could not open file: " + e.getMessage());
				System.exit(0);
			} catch (IOException e) {
				e.printStackTrace();
			}
			
			
		}
	}
	
	public static void ShowUsage() {
		System.out.println("Usage: dotprofiler symbolmap.gen serialdump [[!]functionName]\n");
		System.out.println("\tsymbolmap.gen\tRay kernel symbol map");
		System.out.println("\tserialdump\tSerial raw dump (use - for stdin)");
		System.out.println("\tfunctionName\tOptional functionName to filter for. Put an");
		System.out.println("\t\t\texclamation mark (!) in front of the name to separate");
		System.out.println("\t\t\teach call to that function in the graph.");
		
	}
	
	/**
	 * Prints sorted statistics about the 10 most CPU intense function chains
	 * @param graph
	 */
	private static void PrintStatistics(Graph graph, PrintStream stream) {
		ArrayList<Edge> sorted = new ArrayList<Edge>();
		for(Node n : graph.GetNodes()) {
			for(Edge e: n.GetOutgoingEdges()) {
				sorted.add(e);
			}
		}
		
		printWorstAverageExeTime(stream, sorted);
		printWorstTotalExeTime(stream, sorted);
	}

	/**
	 * Sorts the functions by their total execution time (hitCount * averageTime)
	 * @param stream
	 * @param sorted
	 */
	private static void printWorstTotalExeTime(PrintStream stream,
			ArrayList<Edge> sorted) {
		Collections.sort(sorted, new TotalExecutionComparator());
		
		int toPrint = 10;
		stream.println("\nThe top " + toPrint + " total time spent in functions are:");
		
		
		printFunctionChains(sorted, toPrint);
	}

	/**
	 * Sorts the functions by their average execution time
	 * @param stream
	 * @param sorted
	 */
	private static void printWorstAverageExeTime(PrintStream stream,
			ArrayList<Edge> sorted) {
		Collections.sort(sorted, new ExecutionTimeComparator());
		
		int toPrint = 10;
		stream.println("\nThe " + toPrint + " most exaustive function chains are:");
		
		
		printFunctionChains(sorted, toPrint);
	}
	
	/**
	 * Prints <code>toPrint</code> function chains with taking
	 * into account, that no functions are listed as root
	 * element (start) which already appeared in a chain before.
	 * 
	 * Avoids things like:
	 * A -> B -> C -> D
	 * B -> C -> D
	 * C -> D
	 * @param sorted
	 * @param toPrint
	 */
	private static void printFunctionChains(ArrayList<Edge> sorted, int toPrint) {
		ArrayList<Edge> blackList = new ArrayList<Edge>();
		int printed = 0;
		int i = 0;
		while((printed < toPrint) && (i < sorted.size())){
			Edge current = sorted.get(i++);
			if (!blackList.contains(current)) {
				ArrayList<Edge> used = printCulprit(current, System.err);
				blackList.addAll(used);
				printed++;
			}
		};
	}



	/**
	 * prints one function chain
	 * @param edge
	 * @param err
	 */
	private static ArrayList<Edge> printCulprit(Edge edge, PrintStream err) {
		ArrayList<Edge> used = new ArrayList<Edge>();
		do {
			err.print(edge.sourceSubNode.GetParent().label + "[" + edge.GetAverageExecutionTime() + "] => ");
			
			used.add(edge);
			
			long max =  0;
			Edge tempMax = null;
			for(Edge next : edge.target.GetOutgoingEdges()) {
				long currExeTime = next.GetAverageExecutionTime();
				if (currExeTime >= max) {
					// Set new maximum
					max = currExeTime;
					tempMax = next;
				}
			}
			edge = tempMax;
		} while(edge != null);
		err.println();
		
		return used;
	}

}

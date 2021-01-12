package dot;

import java.io.IOException;
import java.io.PrintStream;


public class DotCode {

	
	public static void convertToDot(Graph graph, PrintStream writer) throws IOException {
		
		
		writer.println("digraph{");
		writer.println("node [shape=record];");
		
		int red = 128, green = 64, blue = 64;
		
		
		// Write nodes
		for(Node n : graph.GetNodes()) {
			writer.println(n.GetDotCode() + ";");
		}
		
		writer.println("rankdir=LR;");
		
		// Write edges
		for(Node n : graph.GetNodes()) {
			for(Edge e : n.GetOutgoingEdges()) {
				writer.println(e.GetDotCode(1,  red, green, blue) + ";");
			}
		}
		
		writer.println("}");
		writer.close();
		
	}

}

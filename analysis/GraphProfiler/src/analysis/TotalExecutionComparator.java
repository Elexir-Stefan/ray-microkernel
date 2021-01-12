package analysis;

import java.util.Comparator;

import dot.Edge;

public class TotalExecutionComparator implements Comparator<Edge> {

	@Override
	/**
	 * NOTE: Sorts in descending order
	 */
	public int compare(Edge o1, Edge o2) {
		if (o1.GetTotalExecutionTime() > o2.GetTotalExecutionTime()) {
			return -1;
		} else if (o1.GetTotalExecutionTime() < o2.GetTotalExecutionTime()) {
			return 1;
		} else {
			return 0;
		}
	}

}

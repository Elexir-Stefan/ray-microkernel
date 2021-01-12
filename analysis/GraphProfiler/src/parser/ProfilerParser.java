package parser;

import java.io.DataInputStream;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Iterator;

/**
 * Parses a call graph file produced by redirecting the serial port kernel output
 * to a file
 * @author stefan
 *
 */
public class ProfilerParser implements Iterable<ProfilerParser.Invocation>{
	
	/**
	 * Represents a caller->callee relation and saves
	 * the time between last event found
	 * @author stefan
	 *
	 */
	public class Invocation {
		
		public long caller, callee, TSC;
		
		// flags
		public boolean frameDrop = false;
		
		/**
		 * Simply works as a "struct" by saving all the values that belong together
		 * @param caller
		 * @param callee
		 * @param TSC
		 */
		public Invocation(long caller, long callee, long TSC) {
			this.caller = caller;
			this.callee = callee;
			this.TSC = TSC;
		}
	}
	
	// Constants
	public final int PROF_FLG_FRAMEDROP = 1;
	
	private DataInputStream stream;
	private ArrayList<Invocation> invocations;
	
	public ProfilerParser(InputStream stream) throws FileNotFoundException {
		this.stream = new DataInputStream(stream);
		
		this.invocations = ReadWholeFile();
	}
	
	private ArrayList<Invocation> ReadWholeFile() {
		ArrayList<Invocation> result = new ArrayList<Invocation>();
		
		
		try {
			while(true) {
				int flags = readUnsignedByte();
				long callee = readUnsignedInt();
				long caller = readUnsignedInt();
				
				long TSC = readUnsignedLong();
				
				Invocation invocation = new Invocation(caller, callee, TSC);
				invocation.frameDrop = ((flags & PROF_FLG_FRAMEDROP) == PROF_FLG_FRAMEDROP);
				result.add(invocation);
				
			}
		}
		catch(IOException e) {
			
		}
		
		return result;
	}
	
	private long readUnsignedLong() throws IOException {
		long lo = readUnsignedInt();
		long hi = readUnsignedInt();
		
		return (hi << 32) + lo;
	}
	
	private int readUnsignedByte() throws IOException {
		return this.stream.readUnsignedByte();
	}


	private long readUnsignedInt() throws IOException {
		long b4 = this.stream.readUnsignedByte();
		long b3 = this.stream.readUnsignedByte();
		long b2 = this.stream.readUnsignedByte();
		long b1 = this.stream.readUnsignedByte();
		
		return (b1 << 24) + (b2 << 16) + (b3 << 8) + b4;
	}

	@Override
	public Iterator<Invocation> iterator() {
		return this.invocations.iterator();
	}
	
	

}

package parser;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class FunctionTable {
	
	/**
	 * Functions as a kind of struct for symbol name <-> address translations
	 * @author stefan
	 *
	 */
	private class AddressTranslation {
		public String functionName;
		public long linearAddress;
		
		public AddressTranslation(String name, long address) {
			this.functionName = name;
			this.linearAddress = address;
		}
	}
	
	/**
	 * Describes the source of an invocation
	 * @author stefan
	 *
	 */
	public class InvocationSource {
		public String functionName;
		public int subGraph = 0;
		public long functionStart;
		public long invocationOffset;
		
		public long invocationAbsoluteAddress;
		
		public InvocationSource(String functionName, long functionStart, long invocationOffset) {
			this.functionName = functionName;
			this.functionStart = functionStart;
			this.invocationOffset = invocationOffset;
			
			this.invocationAbsoluteAddress = this.functionStart + this.invocationOffset;
		}

	}
	
	private class AscendingComparator implements Comparator<AddressTranslation> {

		@Override
		public int compare(AddressTranslation arg0, AddressTranslation arg1) {
			if (arg0.linearAddress > arg1.linearAddress) {
				return 1;
			} else if (arg0.linearAddress < arg1.linearAddress) {
				return -1;
			} else {
				return 0;
			}
		}
		
	}
	
	private ArrayList<AddressTranslation> kernelSymbols;
	
	/**
	 * Opens the symbol map and parses it to the array list
	 * @param translationFile
	 * @throws IOException
	 */
	public FunctionTable(String translationFile) throws IOException {
		BufferedReader reader = new BufferedReader(new FileReader(translationFile));
		
		// create regex
		Pattern p = Pattern.compile("\\{0x(.*),\\\"(.*)\\\"\\},");
		
		// initialize array
		this.kernelSymbols = new ArrayList<AddressTranslation>();
		
		String line;
		while((line = reader.readLine()) != null) {
			Matcher m = p.matcher(line);
			
			m.find();
			
			String address = m.group(1);
			String funcName = m.group(2);
			
			// add to list
			this.kernelSymbols.add(new AddressTranslation(funcName, Long.parseLong(address, 16)));
		}
		
		// order them ascendingly
		Collections.sort(this.kernelSymbols, new AscendingComparator());
	}
	
	/**
	 * Returns the function's name to the specified address <code>address</code>
	 * @param address Linear memory address of the specified function
	 * @return Functions's name
	 */
	public InvocationSource getInvocationByAddress(long address) {
		String lastName = null;
		long lastFuncStart = 0;
		for(AddressTranslation t : this.kernelSymbols) {
			if (t.linearAddress > address) {
				return new InvocationSource(lastName, lastFuncStart, address - lastFuncStart);
			}
			lastName = t.functionName;
			lastFuncStart = t.linearAddress;
		}
		return null;
	}
}

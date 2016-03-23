/*
	This file is part of FaMaTS.

    FaMaTS is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FaMaTS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with FaMaTS.  If not, see <http://www.gnu.org/licenses/>.

 */
package fr.inria.domain;


import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;


public class Configuration {

	public Map<String, Number> elements;

	public Configuration() {
		this.elements = new HashMap<String, Number>();
	}

	
	public String toString(){
		String res="";
		for(Entry<String,Number> e: elements.entrySet()){
			res+=e.getKey().toString()+ " = "+e.getValue().toString()+"\n";
		}
		return res;
	}


	public void add(String string, double d) {
		this.elements.put(string, d);
		
	}
	public void add(String string, int d) {
		this.elements.put(string, d);
		
	}
}
package fr.inria.writer;

import java.util.Map.Entry;

import fr.inria.domain.Configuration;

public class MOTIVConfigurationWriter {

	public void printConf(String path, Configuration c){
		for(Entry<String,Number> e:c.elements.entrySet()){
			System.out.println(e.getKey()+" = "+e.getValue());
		} 
	}
}

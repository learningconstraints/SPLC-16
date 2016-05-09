package fr.inria.generator;

import java.util.Collection;
import java.util.LinkedList;

import fr.inria.domain.Configuration;
import fr.inria.reader.VMReader;

public class Configator {

	public static void main(String[] args) {
		VMReader rd = new VMReader();
		try {
			rd.parseFile("inputs/MOTIV.vm");
		} catch (Exception e) {
			System.err.println("Couldn't parse VM");
			e.printStackTrace();
		}
		Collection<Configuration> confs = RandomGenerator.generateSetOfConfigurations(100);
		for (Configuration c : confs) {
			try {
				if (rd.reasoner.isValidConf(c))
					System.out.println("ok");
			} catch (ArrayIndexOutOfBoundsException e) {
				// :(
			}
		}
	}

}

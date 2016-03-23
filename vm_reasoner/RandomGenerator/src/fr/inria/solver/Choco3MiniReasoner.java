package fr.inria.solver;

import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Map;
import java.util.Map.Entry;

import org.chocosolver.solver.Solver;
import org.chocosolver.solver.constraints.Constraint;
import org.chocosolver.solver.constraints.IntConstraintFactory;
import org.chocosolver.solver.constraints.LogicalConstraintFactory;
import org.chocosolver.solver.constraints.real.Ibex;
import org.chocosolver.solver.constraints.real.RealConstraint;
import org.chocosolver.solver.variables.BoolVar;
import org.chocosolver.solver.variables.IntVar;
import org.chocosolver.solver.variables.RealVar;
import org.chocosolver.solver.variables.Variable;
import org.chocosolver.solver.variables.VariableFactory;

import fr.inria.domain.Configuration;

public class Choco3MiniReasoner {

	public Map<String, Variable> atributes;
	public Map<String, Variable> features;
	public Collection<Constraint> constraints;
	public double precision;
	public Solver solver;

	public Choco3MiniReasoner() {
		atributes = new HashMap<String, Variable>();
		features = new HashMap<String, Variable>();
		constraints = new LinkedList<Constraint>();
		solver = new Solver();
		this.precision=0.01;
		solver.setPrecision(precision);
	}

	public Variable addRealAtribute(String name, Double min, Double max) {
		RealVar att = VariableFactory.real(name, min, max, precision, solver);
		atributes.put(name, att);
		return att;
	}

	public Variable addBoolAtribute(String name) {
		IntVar att = VariableFactory.bool(name, solver);
		atributes.put(name, att);
		return att;
	}

	public Variable addIntegerAtribute(String name, int min, int max) {
		IntVar att = VariableFactory.integer(name, min, max, solver);
		atributes.put(name, att);
		return att;
	}

	public Variable addFeature(String name) {
		IntVar feature = VariableFactory.bool(name, solver);
		this.features.put(name, feature);
		return feature;
	}

	public Constraint addMandatoryConstraint(Variable parent, Variable child) {
		Constraint b = IntConstraintFactory.arithm((IntVar) child, "=", 1);
		Constraint res = LogicalConstraintFactory.ifThen_reifiable((BoolVar) parent, b);
		this.constraints.add(res);
		return res;
	}

	public Constraint addOptionalConstraint(Variable parent, Variable child) {
		Constraint b = IntConstraintFactory.arithm((IntVar) child, "=", 0);
		Constraint res = LogicalConstraintFactory.ifThen_reifiable(((BoolVar) parent).not(), b);
		this.constraints.add(res);
		return res;
	}

	public Constraint addOrConstraint(Variable... parentAndchilds) {
		int numberOfChilds = parentAndchilds.length - 1;
		IntVar[] childs = new IntVar[numberOfChilds];
		for (int i = 1; i < parentAndchilds.length; i++) {
			childs[i - 1] = (IntVar) parentAndchilds[i];
		}
		Variable parent = parentAndchilds[0];
		Constraint res = LogicalConstraintFactory.ifThenElse_reifiable(
				IntConstraintFactory.arithm((IntVar) parent, "=", 1),
				IntConstraintFactory.sum(childs, "=", VariableFactory.fixed(1, solver)),
				IntConstraintFactory.sum(childs, "=", VariableFactory.fixed(0, solver)));
		this.constraints.add(res);
		return res;

	}

	public Constraint addSetConstraint(Variable... parentAndchilds) {
		int numberOfChilds = parentAndchilds.length - 1;
		IntVar[] childs = new IntVar[numberOfChilds];
		for (int i = 1; i < parentAndchilds.length; i++) {
			childs[i - 1] = (IntVar) parentAndchilds[i];
		}
		Variable parent = parentAndchilds[0];
		Constraint res = LogicalConstraintFactory.ifThenElse_reifiable(
				IntConstraintFactory.arithm((IntVar) parent, "=", 1),
				IntConstraintFactory.sum(childs, "<=", VariableFactory.fixed(numberOfChilds, solver)),
				IntConstraintFactory.sum(childs, "=", VariableFactory.fixed(0, solver)));
		this.constraints.add(res);
		return res;

	}

	public void setConstraints() {
		for (Constraint c : constraints) {
			solver.post(c);
		}
	}


	public void allSolutions() {
		this.setConstraints();
		if (solver.findSolution()) {
			do {
				Variable[] vars = solver.getVars();
				for (Variable var : vars) {
					if (var instanceof IntVar) {
						System.out.println(((IntVar) var).getName() + "=" + ((IntVar) var).getValue());
					} else if (var instanceof RealVar) {
						System.out.println(((RealVar) var).getName() + "=" + ((RealVar) var).getLB() + "~"
								+ ((RealVar) var).getUB());
					}

				}
			} while (solver.nextSolution());
		}
	}

	public boolean isValidConf(Configuration conf){
		this.setConstraints();
		Collection<Constraint> tmpCons=new LinkedList<Constraint>();
		int name=0;
		for(Entry<String,Number> pareja:conf.elements.entrySet()){
			Variable relatedVariable;
			String key = pareja.getKey();
			Number value = pareja.getValue();
			if(key.contains(".")){
				relatedVariable = atributes.get(key);
				Constraint c=null;
				if(relatedVariable instanceof IntVar){
					c=IntConstraintFactory.arithm((IntVar) relatedVariable,"=",((Number)value).intValue());
					tmpCons.add(c);
					solver.post(c);
				}else if(relatedVariable instanceof RealVar){
					RealVar real = VariableFactory.real("realAux"+name, ((Number)value).doubleValue(), ((Number)value).doubleValue(),precision,solver);
					c = new RealConstraint("realAuxCons"+name, "{0}={1}",Ibex.COMPO,(RealVar)relatedVariable,real);
					name++;
					tmpCons.add(c);
					solver.post(c);
				}
				
			}else{
				relatedVariable = features.get(key);
				Constraint c=IntConstraintFactory.arithm((IntVar) relatedVariable,"=",(Integer)value);
				tmpCons.add(c);
				solver.post(c);
			}
			
		}
		
		boolean res =solver.findSolution();
		
		for(Constraint c: constraints){
			solver.unpost(c);
		}
		return res;
	}
	
	public boolean isValid(){
		this.setConstraints();
		return solver.findSolution();
	}
	public boolean isValidConfByStep(Configuration conf){
		this.setConstraints();
		Collection<Constraint> tmpCons=new LinkedList<Constraint>();
		boolean res =true;
		Iterator<Entry<String, Number>> iterator = conf.elements.entrySet().iterator();
		while(iterator.hasNext()&&res){
			Entry<String,Number> pareja=iterator.next();
			Variable relatedVariable;
			String key = pareja.getKey();
			Number value = pareja.getValue();
			if(key.contains(".")){
				relatedVariable = atributes.get(key);
				Constraint c=null;
				if(relatedVariable instanceof IntVar){
					c=IntConstraintFactory.arithm((IntVar) relatedVariable,"=",((Double)value).intValue());
					
				}else if(relatedVariable instanceof RealVar){
					RealVar real = VariableFactory.real("", ((Double)value).doubleValue(), ((Double)value).doubleValue(),precision,solver);
					c = new RealConstraint("", "{0}={1}",Ibex.COMPO,(RealVar)relatedVariable,real);
				}
				tmpCons.add(c);
				solver.post(c);
			}else{
				relatedVariable = features.get(key);
				Constraint c=IntConstraintFactory.arithm((IntVar) relatedVariable,"=",(Integer)value);
				tmpCons.add(c);
				solver.post(c);
			}
			res =solver.findSolution();
			if(!res){
				System.out.println("The constraint "+key+"="+value+"broke the model");
			}
			System.out.print(solver.getPrecision());

		}
		
		
		for(Constraint c: constraints){
			solver.unpost(c);
		}
		return res;
	}
}

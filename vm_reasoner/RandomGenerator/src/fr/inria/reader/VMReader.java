package fr.inria.reader;

import java.util.Collection;
import java.util.LinkedList;

import org.chocosolver.solver.constraints.IntConstraintFactory;
import org.chocosolver.solver.constraints.LogicalConstraintFactory;
import org.chocosolver.solver.constraints.real.Ibex;
import org.chocosolver.solver.constraints.real.IntEqRealConstraint;
import org.chocosolver.solver.constraints.real.RealConstraint;
import org.chocosolver.solver.variables.IntVar;
import org.chocosolver.solver.variables.RealVar;
import org.chocosolver.solver.variables.Variable;
import org.chocosolver.solver.variables.VariableFactory;
import org.eclipse.emf.common.util.EList;
import org.eclipse.emf.common.util.URI;
import org.eclipse.emf.ecore.resource.Resource;
import org.eclipse.xtext.resource.XtextResource;
import org.eclipse.xtext.resource.XtextResourceSet;

import com.google.inject.Injector;

import fr.inria.lang.VMStandaloneSetup;
import fr.inria.lang.vM.And;
import fr.inria.lang.vM.AttrDef;
import fr.inria.lang.vM.Attributes;
import fr.inria.lang.vM.BasicAttrDef;
import fr.inria.lang.vM.BiImplication;
import fr.inria.lang.vM.BooleanAttrDef;
import fr.inria.lang.vM.BooleanExpression;
import fr.inria.lang.vM.BrackedExpression;
import fr.inria.lang.vM.ComplexExpression;
import fr.inria.lang.vM.Constraint;
import fr.inria.lang.vM.Constraints;
import fr.inria.lang.vM.Division;
import fr.inria.lang.vM.EnumAttrDef;
import fr.inria.lang.vM.EnumIntegerDef;
import fr.inria.lang.vM.EnumRealDef;
import fr.inria.lang.vM.EnumStringDef;
import fr.inria.lang.vM.Equality;
import fr.inria.lang.vM.Excludes;
import fr.inria.lang.vM.Feature;
import fr.inria.lang.vM.FeatureDefinition;
import fr.inria.lang.vM.FeatureHierarchy;
import fr.inria.lang.vM.FeaturesGroup;
import fr.inria.lang.vM.Greater;
import fr.inria.lang.vM.Greaterequal;
import fr.inria.lang.vM.Inequality;
import fr.inria.lang.vM.IntegerAttrDefBounded;
import fr.inria.lang.vM.IntegerAttrDefComplement;
import fr.inria.lang.vM.IntegerAttrDefUnbounded;
import fr.inria.lang.vM.LeftImplication;
import fr.inria.lang.vM.Less;
import fr.inria.lang.vM.Lessequal;
import fr.inria.lang.vM.Minus;
import fr.inria.lang.vM.Model;
import fr.inria.lang.vM.Multiplication;
import fr.inria.lang.vM.NumericExpression;
import fr.inria.lang.vM.Or;
import fr.inria.lang.vM.Orgroup;
import fr.inria.lang.vM.PackageDeclaration;
import fr.inria.lang.vM.Plus;
import fr.inria.lang.vM.PrimitiveExpression;
import fr.inria.lang.vM.RealAttrDefBounded;
import fr.inria.lang.vM.RealAttrDefComplement;
import fr.inria.lang.vM.RealAttrDefUnbounded;
import fr.inria.lang.vM.Relationships;
import fr.inria.lang.vM.Requires;
import fr.inria.lang.vM.RightImplication;
import fr.inria.lang.vM.StringExpression;
import fr.inria.lang.vM.VmBlock;
import fr.inria.lang.vM.Xorgroup;
import fr.inria.solver.Choco3MiniReasoner;

public class VMReader {

	Collection<String> models = null;
	public Choco3MiniReasoner reasoner = new Choco3MiniReasoner();
	int precision =0;
	public void parseFile(String fileName) throws Exception {
		Injector injector = new VMStandaloneSetup().createInjectorAndDoEMFRegistration();
		XtextResourceSet resourceSet = injector.getInstance(XtextResourceSet.class);
		resourceSet.addLoadOption(XtextResource.OPTION_RESOLVE_ALL, Boolean.TRUE);
		if (models != null) {
			for (String s : models) {
				resourceSet.getResource(URI.createURI(s), true);

			}
		}

		Resource resource = resourceSet.getResource(URI.createURI(fileName), true);

		Model model = (Model) resource.getContents().get(0);

		VmBlock relationships = null;
		VmBlock attsblock = null;
		VmBlock constratins = null;

		EList<VmBlock> bl = model.getBlocks();
		for (VmBlock block : bl) {
			if (block instanceof Relationships) {
				relationships = block;
			} else if (block instanceof Attributes) {
				attsblock = block;
			} else if (block instanceof Constraints) {
				constratins = block;
			} else if (block instanceof PackageDeclaration) {
				for (VmBlock blockinpkg : ((PackageDeclaration) block).getBlocks()) {
					if (blockinpkg instanceof Relationships) {
						relationships = blockinpkg;
					} else if (blockinpkg instanceof Attributes) {
						attsblock = blockinpkg;

					} else if (blockinpkg instanceof Constraints) {
						constratins = blockinpkg;

					}
				}
			}
		}
		if (((Relationships) relationships) != null) {
			FeatureHierarchy fhs = ((Relationships) relationships).getRoot();
			Variable rootfeat = reasoner.addFeature(fhs.getParent().getName());
			visitFeatureHierarchy(rootfeat, fhs);

		}
		if ((Attributes) attsblock != null) {
			visitAttributes(((Attributes) attsblock).getAttrDefs());
		}
		if ((Constraints) constratins != null) {
			visitConstraints(((Constraints) constratins).getConstraints());
		}
	}

	
	private class ConstraintsParser{
		
		double precision=reasoner.precision;
		
		public org.chocosolver.solver.constraints.Constraint translateArithmetic(ComplexExpression cex){	
			org.chocosolver.solver.constraints.Constraint res = null;
			
			if(cex instanceof Plus) {
				Variable e1 =translateInteger(((Plus) cex).getLeft());
				Variable e2= translateInteger(((Plus) cex).getRight());
				if(e1 instanceof RealVar &&e2 instanceof RealVar){
					res = new RealConstraint("", "{0}+{1}",Ibex.COMPO,(RealVar)e1,(RealVar)e2);
				}else if(e1 instanceof IntVar &&e2 instanceof IntVar){
					res = IntConstraintFactory.arithm((IntVar) e1, "+", (IntVar) e2);
				}else if(e1 instanceof IntVar &&e2 instanceof RealVar){
				//	res = new IntEqRealConstraint((IntVar) e1, (RealVar)e2, precision);
				}else if(e1 instanceof RealVar &&e2 instanceof IntVar){
				//	res = new IntEqRealConstraint((IntVar) e2, (RealVar)e1, precision);
				}
				
				
			} else if (cex instanceof Minus) {
				Variable e1 =translateInteger(((Minus) cex).getLeft());
				Variable e2= translateInteger(((Minus) cex).getRight());
				if(e1 instanceof RealVar &&e2 instanceof RealVar){
					res = new RealConstraint("", "{0}-{1}",Ibex.COMPO,(RealVar)e1,(RealVar)e2);
				}else if(e1 instanceof IntVar &&e2 instanceof IntVar){
					res = IntConstraintFactory.arithm((IntVar) e1, "-", (IntVar) e2);
				}else if(e1 instanceof IntVar &&e2 instanceof RealVar){
				//	res = new IntEqRealConstraint((IntVar) e1, (RealVar)e2, precision);
				}else if(e1 instanceof RealVar &&e2 instanceof IntVar){
				//	res = new IntEqRealConstraint((IntVar) e2, (RealVar)e1, precision);
				}
			} else if (cex instanceof Multiplication) {
				Variable e1 =translateInteger(((Multiplication) cex).getLeft());
				Variable e2= translateInteger(((Multiplication) cex).getRight());
				if(e1 instanceof RealVar &&e2 instanceof RealVar){
					res = new RealConstraint("", "{0}*{1}",Ibex.COMPO,(RealVar)e1,(RealVar)e2);
				}else if(e1 instanceof IntVar &&e2 instanceof IntVar){
					res = IntConstraintFactory.arithm((IntVar) e1, "*", (IntVar) e2);
				}else if(e1 instanceof IntVar &&e2 instanceof RealVar){
				//	res = new IntEqRealConstraint((IntVar) e1, (RealVar)e2, precision);
				}else if(e1 instanceof RealVar &&e2 instanceof IntVar){
				//	res = new IntEqRealConstraint((IntVar) e2, (RealVar)e1, precision);
				}
			} else if (cex instanceof Division) {
				Variable e1 =translateInteger(((Division) cex).getLeft());
				Variable e2= translateInteger(((Division) cex).getRight());
				if(e1 instanceof RealVar &&e2 instanceof RealVar){
					res = new RealConstraint("", "{0}/{1}",Ibex.COMPO,(RealVar)e1,(RealVar)e2);
				}else if(e1 instanceof IntVar &&e2 instanceof IntVar){
					res = IntConstraintFactory.arithm((IntVar) e1, "/", (IntVar) e2);
				}else if(e1 instanceof IntVar &&e2 instanceof RealVar){
				//	res = new IntEqRealConstraint((IntVar) e1, (RealVar)e2, precision);
				}else if(e1 instanceof RealVar &&e2 instanceof IntVar){
				//	res = new IntEqRealConstraint((IntVar) e2, (RealVar)e1, precision);
				}
			} 
			return res;
		}
		
		private Variable translateInteger(ComplexExpression cex) {
			
			Variable res=null;
			if(translateArithmetic(cex)!=null){
				return translateArithmetic(cex).reif();
			}
			if (cex instanceof NumericExpression) {
//				System.err.println("Numeric Expressions are not yet implemented");
				String value=((NumericExpression)cex).getValue();
				if(value!=null){
					try{
						int parseInt = Integer.parseInt(value);
						return VariableFactory.integer(value, parseInt, parseInt, reasoner.solver);
					}catch(NumberFormatException e){
						try{
							double parsefloat = Double.parseDouble(value);
							return VariableFactory.real(value, parsefloat, parsefloat,precision, reasoner.solver);

						}catch(NumberFormatException e2){
							System.err.println("Error parsing numbers");
						}	
					}
				}
				
				// System.out.println(((NumericExpression) cex).getValue());
			} else if (cex instanceof BooleanExpression) {
				// System.out.println(((BooleanExpression) cex).getValue());
				//node.setData(((BooleanExpression) cex).getValue());
			} else if (cex instanceof StringExpression) {
				//node.setData(((StringExpression) cex).getValue());

				// System.out.println(((StringExpression) cex).getValue());
			} else if (cex instanceof PrimitiveExpression) {
				// this is a feature
				if (((PrimitiveExpression) cex).getFeatureID() != null) {
					res = reasoner.features.get(((PrimitiveExpression) cex).getFeatureID().getName());
				}
				// This is an attribute
				if (((PrimitiveExpression) cex).getRefAtt() != null) {
					String featname=((PrimitiveExpression) cex).getRefAtt().getHead().getOwnedFeature().getName();
					String attname= ((PrimitiveExpression) cex).getRefAtt().getName();
					res=reasoner.atributes.get(featname+"."+attname);
				}
			} else {
				// System.out.println(cex);
			}
			return res;

			
		}
		public org.chocosolver.solver.constraints.Constraint translateRelational(ComplexExpression cex){
			
			org.chocosolver.solver.constraints.Constraint res = null;
			if (cex instanceof Equality) {	
				
				Variable e1 =translateInteger(((Equality) cex).getLeft());
				Variable e2= translateInteger(((Equality) cex).getRight());
				if(e1 instanceof RealVar &&e2 instanceof RealVar){
					res = new RealConstraint("", "{0}={1}",Ibex.COMPO,(RealVar)e1,(RealVar)e2);
				}else if(e1 instanceof IntVar &&e2 instanceof IntVar){
					res = IntConstraintFactory.arithm((IntVar) e1, "=", (IntVar) e2);
				}else if(e1 instanceof IntVar &&e2 instanceof RealVar){
					res = new IntEqRealConstraint((IntVar) e1, (RealVar)e2, precision);
				}else if(e1 instanceof RealVar &&e2 instanceof IntVar){
					res = new IntEqRealConstraint((IntVar) e2, (RealVar)e1, precision);
				}
				
			} else if (cex instanceof Inequality) {
				Variable e1 =translateInteger(((Inequality) cex).getLeft());
				Variable e2= translateInteger(((Inequality) cex).getRight());
				if(e1 instanceof RealVar &&e2 instanceof RealVar){
					res = LogicalConstraintFactory.not(new RealConstraint("", "{0}={1}",Ibex.COMPO,(RealVar)e1,(RealVar)e2));
				}else if(e1 instanceof IntVar &&e2 instanceof IntVar){
					res = LogicalConstraintFactory.not(IntConstraintFactory.arithm((IntVar) e1, "=", (IntVar) e2));
				}else if(e1 instanceof IntVar &&e2 instanceof RealVar){
					res = LogicalConstraintFactory.not(new IntEqRealConstraint((IntVar) e1, (RealVar)e2, precision));
				}else if(e1 instanceof RealVar &&e2 instanceof IntVar){
					res = LogicalConstraintFactory.not(new IntEqRealConstraint((IntVar) e2, (RealVar)e1, precision));
				}
			
			} else if (cex instanceof Less) {
				Variable e1 =translateInteger(((Less) cex).getLeft());
				Variable e2= translateInteger(((Less) cex).getRight());
				if(e1 instanceof RealVar &&e2 instanceof RealVar){
					res = new RealConstraint("", "{0}<{1}",Ibex.COMPO,(RealVar)e1,(RealVar)e2);
				}else if(e1 instanceof IntVar &&e2 instanceof IntVar){
					res = IntConstraintFactory.arithm((IntVar) e1, "<", (IntVar) e2);
				}else if(e1 instanceof IntVar &&e2 instanceof RealVar){
					//res = new IntRealConstraint((IntVar) e1, (RealVar)e2, presicion);
				}else if(e1 instanceof RealVar &&e2 instanceof IntVar){
					//res = new IntEqRealConstraint((IntVar) e2, (RealVar)e1, presicion);
				}
		
			} else if (cex instanceof Lessequal) {
				Variable e1 =translateInteger(((Lessequal) cex).getLeft());
				Variable e2= translateInteger(((Lessequal) cex).getRight());
				if(e1 instanceof RealVar &&e2 instanceof RealVar){
					res = new RealConstraint("", "{0}<={1}",Ibex.COMPO,(RealVar)e1,(RealVar)e2);
				}else if(e1 instanceof IntVar &&e2 instanceof IntVar){
					res = IntConstraintFactory.arithm((IntVar) e1, "<=", (IntVar) e2);
				}else if(e1 instanceof IntVar &&e2 instanceof RealVar){
					//res = new IntRealConstraint((IntVar) e1, (RealVar)e2, presicion);
				}else if(e1 instanceof RealVar &&e2 instanceof IntVar){
					//res = new IntEqRealConstraint((IntVar) e2, (RealVar)e1, presicion);
				}
		
			} else if (cex instanceof Greater) {
				Variable e1 =translateInteger(((Greater) cex).getLeft());
				Variable e2= translateInteger(((Greater) cex).getRight());
				if(e1 instanceof RealVar &&e2 instanceof RealVar){
					res = new RealConstraint("", "{0}>{1}",Ibex.COMPO,(RealVar)e1,(RealVar)e2);
				}else if(e1 instanceof IntVar &&e2 instanceof IntVar){
					res = IntConstraintFactory.arithm((IntVar) e1, ">", (IntVar) e2);
				}else if(e1 instanceof IntVar &&e2 instanceof RealVar){
					//res = new IntRealConstraint((IntVar) e1, (RealVar)e2, presicion);
				}else if(e1 instanceof RealVar &&e2 instanceof IntVar){
					//res = new IntEqRealConstraint((IntVar) e2, (RealVar)e1, presicion);
				}
		
			} else if (cex instanceof Greaterequal) {
				Variable e1 =translateInteger(((Greaterequal) cex).getLeft());
				Variable e2= translateInteger(((Greaterequal) cex).getRight());
				if(e1 instanceof RealVar &&e2 instanceof RealVar){
					res = new RealConstraint("", "{0}>={1}",Ibex.COMPO,(RealVar)e1,(RealVar)e2);
				}else if(e1 instanceof IntVar &&e2 instanceof IntVar){
					res = IntConstraintFactory.arithm((IntVar) e1, ">=", (IntVar) e2);
				}else if(e1 instanceof IntVar &&e2 instanceof RealVar){
					//res = new IntRealConstraint((IntVar) e1, (RealVar)e2, presicion);
				}else if(e1 instanceof RealVar &&e2 instanceof IntVar){
					//res = new IntEqRealConstraint((IntVar) e2, (RealVar)e1, presicion);
				}
				
			}
			return res;
		}

		

		public org.chocosolver.solver.constraints.Constraint translateLogical(ComplexExpression cex){
			org.chocosolver.solver.constraints.Constraint c=null;
			if (cex instanceof RightImplication) {
				c=LogicalConstraintFactory.ifThen_reifiable(translateLogical(((RightImplication) cex).getLeft()), translateLogical(((RightImplication) cex).getRight()));
			} else if (cex instanceof LeftImplication) {
				c=LogicalConstraintFactory.ifThen_reifiable(translateLogical(((RightImplication) cex).getRight()),translateLogical(((RightImplication) cex).getLeft()));	
			} else if (cex instanceof BiImplication) {
				org.chocosolver.solver.constraints.Constraint c1=LogicalConstraintFactory.ifThen_reifiable(translateLogical(((RightImplication) cex).getLeft()), translateLogical(((RightImplication) cex).getRight()));
				org.chocosolver.solver.constraints.Constraint c2=LogicalConstraintFactory.ifThen_reifiable(translateLogical(((RightImplication) cex).getRight()),translateLogical(((RightImplication) cex).getLeft()));
				c=LogicalConstraintFactory.and(c1,c2);
			} else if (cex instanceof Or) {
				c=LogicalConstraintFactory.or(translateLogical(((Or) cex).getLeft()),translateLogical(((Or) cex).getRight()));
			} else if (cex instanceof And) {
				c=LogicalConstraintFactory.and(translateLogical(((Or) cex).getLeft()),translateLogical(((Or) cex).getRight()));
			} else if (cex instanceof Excludes) {
				c=LogicalConstraintFactory.ifThen_reifiable(translateLogical(((RightImplication) cex).getLeft()), LogicalConstraintFactory.not(translateLogical(((RightImplication) cex).getRight())));
			} else if (cex instanceof Requires) {
				c=LogicalConstraintFactory.ifThen_reifiable(translateLogical(((RightImplication) cex).getLeft()), translateLogical(((RightImplication) cex).getRight()));
			} else if (cex instanceof BrackedExpression) {
				c= translateLogical(((BrackedExpression) cex).getExpression());
				
			} else {
					c = translateRelational(cex);
				
			}
			return c;
		}
		
	

	}
	
	
	private void visitConstraints(EList<Constraint> con) {
		ConstraintsParser parser = new ConstraintsParser();
		for (Constraint co : con) {
			org.chocosolver.solver.constraints.Constraint c = parser.translateLogical((ComplexExpression) co.getExpression());
			reasoner.constraints.add(c);
			reasoner.solver.post(c);
		}
	}

	private void visitAttributes(EList<AttrDef> att) {

		for (AttrDef atdef : att) {
			if (!atdef.isNotTranslatable() || atdef.isRunTime()) {
				BasicAttrDef at = atdef.getBasicAttrDef();
				
				if (at instanceof BooleanAttrDef) {
					String attname = ((BooleanAttrDef) at).getName().getName();
					String featname = ((BooleanAttrDef) at).getName().getHead().getOwnedFeature().getName();
					Variable addBoolAtribute = reasoner.addBoolAtribute(attname);
					reasoner.atributes.put(featname+'.'+attname, addBoolAtribute);
				} else if (at instanceof IntegerAttrDefBounded) {
					String name = ((IntegerAttrDefBounded) at).getName().getName();
					String featname = ((IntegerAttrDefBounded) at).getName().getHead().getOwnedFeature().getName();

					Integer max = null,min = null;
					EList<IntegerAttrDefComplement> complements = ((IntegerAttrDefBounded) at).getComplements();
					for (IntegerAttrDefComplement complement : complements) {
						max = Integer.parseInt(complement.getMax());
						min = Integer.parseInt(complement.getMin());
					}
					
					Variable addIntegerAtribute = reasoner.addIntegerAtribute(name, min, max);
					reasoner.atributes.put(featname+'.'+name, addIntegerAtribute);
				} else if (at instanceof IntegerAttrDefUnbounded) {
					String name = ((IntegerAttrDefUnbounded) at).getName().getName();
					String featname = ((IntegerAttrDefUnbounded) at).getName().getHead().getOwnedFeature().getName();

					Variable addIntegerAtribute = reasoner.addIntegerAtribute(name, Integer.MIN_VALUE, Integer.MAX_VALUE);
					reasoner.atributes.put(featname+'.'+name, addIntegerAtribute);
				} else if (at instanceof RealAttrDefBounded) {
					String name = ((RealAttrDefBounded) at).getName().getName();
					String featname = ((RealAttrDefBounded) at).getName().getHead().getOwnedFeature().getName();

					EList<RealAttrDefComplement> complements = ((RealAttrDefBounded) at).getComplement();
					Double max = null,min = null;
					try{
					for (RealAttrDefComplement complement : complements) {
						max = Double.parseDouble(complement.getMax());
						min = Double.parseDouble(complement.getMin());
					}}catch(Exception e){System.out.println(name);}
					Variable addRealAtribute = reasoner.addRealAtribute(name, min, max);
					reasoner.atributes.put(featname+'.'+name, addRealAtribute);
				} else if (at instanceof RealAttrDefUnbounded) {
					String name = ((RealAttrDefUnbounded) at).getName().getName();
					String featname = ((RealAttrDefUnbounded) at).getName().getHead().getOwnedFeature().getName();

					Variable addRealAtribute = reasoner.addRealAtribute(name, Double.MIN_VALUE, Double.MAX_VALUE);
					reasoner.atributes.put(featname+'.'+name, addRealAtribute);
				}

				EnumAttrDef enumAttrDef = atdef.getEnumAttrDef();
				if (enumAttrDef instanceof EnumStringDef) {
				//	String name = ((EnumStringDef) enumAttrDef).getName().getName();
					System.err.println("Enumerations are not yet supported");
				} else if (enumAttrDef instanceof EnumIntegerDef) {
				//	String name = ((EnumIntegerDef) enumAttrDef).getName().getName();
					System.err.println("Enumerations are not yet supported");
				} else if (enumAttrDef instanceof EnumRealDef) {
				//	String name = ((EnumRealDef) enumAttrDef).getName().getName();
					System.err.println("Enumerations are not yet supported");
				}

			}
		}
	}

	private void visitFeatureHierarchy(Variable ffeatroot, FeatureHierarchy fh) {
		EList<FeatureDefinition> fhchildren = fh.getChildren();
		for (FeatureDefinition fd : fhchildren) {
			if (fd instanceof Feature) {
				fr.inria.lang.vM.Feature f = (fr.inria.lang.vM.Feature) fd;
				Variable child = reasoner.addFeature(f.getName());
				if (f.isOptional()) {
					reasoner.addOptionalConstraint(ffeatroot, child);
				} else if (!f.isOptional()) {// isMandatory
					reasoner.addMandatoryConstraint(ffeatroot, child);
				}
			}
			if (fd instanceof FeaturesGroup) {
				FeaturesGroup group = (FeaturesGroup) fd;
				EList<FeatureDefinition> groupedFeatures = group.getGroupedFeatures();
				LinkedList<Variable> childs= new LinkedList<>();
				for (FeatureDefinition fdef : groupedFeatures) {

					if (fdef instanceof Feature) {
						String vmfeature = ((Feature) fdef).getName();
						Variable addFeature = reasoner.addFeature(vmfeature);
						childs.add(addFeature);
					} else if (fdef instanceof FeatureHierarchy) {
						String vmfeature = ((FeatureHierarchy) fdef).getParent().getName();
						Variable addFeature = reasoner.addFeature(vmfeature);
						childs.add(addFeature);		
						visitFeatureHierarchy(addFeature, (FeatureHierarchy) fdef);
					}
				}
				if (fd instanceof Xorgroup) {
					childs.add(0, ffeatroot);
					reasoner.addOrConstraint(childs.toArray(new Variable[0]));
				} else if (fd instanceof Orgroup) {
					childs.add(0, ffeatroot);
					reasoner.addSetConstraint(childs.toArray(new Variable[0]));
				}
			} else if (fd instanceof FeatureHierarchy) {
				String dest = ((FeatureHierarchy) fd).getParent().getName();
				Variable addFeature = reasoner.addFeature(dest);
				if (((FeatureHierarchy) fd).getParent().isOptional()) {
					reasoner.addMandatoryConstraint(ffeatroot, addFeature);
				} else if (!((FeatureHierarchy) fd).getParent().isOptional()) {
					reasoner.addOptionalConstraint(ffeatroot, addFeature);
				}
				visitFeatureHierarchy(addFeature, (FeatureHierarchy) fd);
			}
		}
	}

}

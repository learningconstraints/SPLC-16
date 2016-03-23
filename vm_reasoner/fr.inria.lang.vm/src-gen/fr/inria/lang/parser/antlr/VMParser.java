/*
* generated by Xtext
*/
package fr.inria.lang.parser.antlr;

import com.google.inject.Inject;

import org.eclipse.xtext.parser.antlr.XtextTokenStream;
import fr.inria.lang.services.VMGrammarAccess;

public class VMParser extends org.eclipse.xtext.parser.antlr.AbstractAntlrParser {
	
	@Inject
	private VMGrammarAccess grammarAccess;
	
	@Override
	protected void setInitialHiddenTokens(XtextTokenStream tokenStream) {
		tokenStream.setInitialHiddenTokens("RULE_WS", "RULE_ML_COMMENT", "RULE_SL_COMMENT");
	}
	
	@Override
	protected fr.inria.lang.parser.antlr.internal.InternalVMParser createParser(XtextTokenStream stream) {
		return new fr.inria.lang.parser.antlr.internal.InternalVMParser(stream, getGrammarAccess());
	}
	
	@Override 
	protected String getDefaultRuleName() {
		return "Model";
	}
	
	public VMGrammarAccess getGrammarAccess() {
		return this.grammarAccess;
	}
	
	public void setGrammarAccess(VMGrammarAccess grammarAccess) {
		this.grammarAccess = grammarAccess;
	}
	
}
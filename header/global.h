/* 
global.h - global objects declarations
$Revision: 10 $
Columbia Optimizer Framework

  A Joint Research Project of Portland State University 
  and the Oregon Graduate Institute
  Directed by Leonard Shapiro and David Maier
  Supported by NSF Grants IRI-9610013 and IRI-9619977
  
  Note: Variable definitions are in global.cpp
*/

#ifndef GLOBAL_H
#define GLOBAL_H

#include "logop.h"

// Each array maps an integer into the elements of the array at that integer
// location, i.e. maps i to array[i].
// These should probably be in CAT
extern STRING_ARRAY  CollTable;	// CollId to char* name of collection
extern STRING_ARRAY  AttTable;		// AttId to char* name of attribute (including coll name)
extern STRING_ARRAY  IndTable;		// IndId to CollId
extern INT_ARRAY     AttCollTable;	// AttId to CollId
extern STRING_ARRAY  BitIndTable;	//	BitIndex name table

extern CString SQueryFile;		// query file name
extern CString BQueryFile;		// query file for batch queries
extern CString CatFile;		// catalog file name
extern CString CMFile;				// cost model file name
extern CString RSFile;			// rule set file name
extern CString AppDir;					// directory of the application

extern bool FileTrace;		// trace to file flag
extern bool PiggyBack;		// Retain the MEMO structure for use in the subsequent optimization
extern bool COVETrace;		// trace to file flag
extern bool WindowTrace;	// trace to window flag
extern bool TraceFinalSSP;		// global trace flag
extern bool TraceOPEN;		// global trace flag
extern bool TraceSSP;		// global trace flag

extern bool Pruning;		// pruning flag
extern bool CuCardPruning;		// cucard pruning flag
extern bool GlobepsPruning;	// global epsilon pruning flag
extern int	 RadioVal;				// the radio value for queryfile
extern bool SingleLineBatch;	//Single line per query in batch mode
extern bool Halt;				// halt flat
extern int  HaltGrpSize;		// halt when number of plans equals to 100% of group
extern int  HaltWinSize;       // window size for checking the improvement
extern int  HaltImpr;			// halt when the improvement is less than 20%

//GLOBAL_EPS can also be set by the options window.
//GLOBAL_EPS is typically determined as a small percentage of 
// a cost found in a first pass optimization.
//Any subplan costing less than this is taken to be optimal.
extern double GLOBAL_EPS;	// global epsilon value
//if GlobalepsPruning is not set, this value is 0
//otherwise, this value will be reset in main
extern COST GlobalEpsBound;  
extern bool ForGlobalEpsPruning;   

extern int	TraceDepth;		// global Trace depth
extern bool	TraceOn;	// global Trace flag

extern class OPT_STAT  * OptStat;	// Opt statistics object

extern CLASS_STAT ClassStat[];	// class statistics object

extern int CLASS_NUM;		// sizeof of ClassStat

#ifdef USE_MEMORY_MANAGER
/**********  THE FOLLOWING IS FOR BILL'S MEMORY MANAGER  ****/
// Pointer to global memory manager.
extern MEMORY_MANAGER * memory_manager ;

extern BLOCK_ANCHOR *  APPLY_RULE_anchor; 
extern BLOCK_ANCHOR *  ATTR_anchor; 
extern BLOCK_ANCHOR *  BINDERY_anchor; 
extern BLOCK_ANCHOR *  COST_anchor; 
extern BLOCK_ANCHOR *  EQJOIN_anchor; 
extern BLOCK_ANCHOR *  EXPR_anchor; 
extern BLOCK_ANCHOR *  GROUP_anchor; 
extern BLOCK_ANCHOR *  LEAF_OP_anchor; 
extern BLOCK_ANCHOR *  LOG_COLL_PROP_anchor; 
extern BLOCK_ANCHOR *  LOOPS_JOIN_anchor; 
extern BLOCK_ANCHOR *  M_EXPR_anchor; 
extern BLOCK_ANCHOR *  O_EXPR_anchor; 
extern BLOCK_ANCHOR *  O_INPUTS_anchor; 
extern BLOCK_ANCHOR *  SCHEMA_anchor; 

#endif //USE_MEMORY_MANAGER

/**********  END BILL'S MEMORY MANAGER  ****/

extern RULE_SET*	RuleSet;	// Rule set  
extern CAT*		Cat;		// read catalog in
extern QUERY*		Query;		// read query in
extern CM*			Cm;			// read cost model in
extern SSP*		Ssp;		// Create Search space
extern KEYS_SET    IntOrdersSet; // set of interesting orders

extern CFile OutputFile;	// result file
extern CFile OutputCOVE;	// script file
extern PTASKS	PTasks;		// pending task

// **************  include physcial mexpr in group or not *****************
extern bool NO_PHYS_IN_GROUP;
// ************************************************************************

#endif // GLOBAL_H

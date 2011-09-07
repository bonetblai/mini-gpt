
# line 2 "parser.y"
#include "global.h"
#include "problems.h"
#include "domains.h"
#include "actions.h"
#include "effects.h"
#include "formulas.h"
#include "expressions.h"
#include "functions.h"
#include "predicates.h"
#include "terms.h"
#include "types.h"
#include "rational.h"
#include "exceptions.h"
#include <iostream>
#include <map>
#include <typeinfo>

// context of free variables.
class context_t
{
  struct VariableMap : public std::map<std::string,Variable> { };
  std::vector<VariableMap> frames_;

public:
  void push_frame() { frames_.push_back(VariableMap()); }
  void pop_frame() { frames_.pop_back(); }
  void insert( const std::string &name, Variable v )
  {
    frames_.back()[name] = v;
  }
  std::pair<Variable,bool> shallow_find( const std::string &name ) const
  {
    VariableMap::const_iterator vi = frames_.back().find( name );
    if( vi != frames_.back().end() )
      return( std::make_pair( (*vi).second, true ) );
    else
      return( std::make_pair( 0, false ) );
  }
  std::pair<Variable,bool> find( const std::string &name ) const
  {
    for( std::vector<VariableMap>::const_reverse_iterator fi =
	   frames_.rbegin(); fi != frames_.rend(); ++fi )
      {
	VariableMap::const_iterator vi = (*fi).find( name );
	if( vi != (*fi).end() )
	  return( std::make_pair( (*vi).second, true ) );
      }
    return( std::make_pair( 0, false ) );
  }
};

extern int yylex();
extern size_t line_number;
extern char *current_file;
static bool success = true;

static Domain* domain;
static std::map<std::string,Domain*> domains;
static problem_t* problem = NULL;
static Requirements* requirements;

static Function goal_prob_function; 
static Function reward_function;
static Predicate predicate;
static bool parsing_predicate;
static bool repeated_predicate;
static Function function;
static bool parsing_function;
static bool repeated_function;

static ActionSchema* action;
static context_t context;

// predicate for atomic state formula being parsed.
static Predicate atom_predicate;
// whether the predicate of the currently parsed atom was undeclared.
static bool undeclared_atom_predicate;
// whether parsing effect fluents.
static bool effect_fluent;
// function for function application being parsed.
static Function appl_function;
// whether the function of the currently parsed application was undeclared.
static bool undeclared_appl_function;
// paramerers for atomic state formula or function application being parsed.
static TermList term_parameters;
// whether parsing an atom.
static bool parsing_atom;
// whether parsing a function application.
static bool parsing_application;
// quantified variables for effect or formula being parsed.
static VariableList quantified;
// most recently parsed term for equality formula.
static Term eq_term;
// most recently parsed expression for equality formula.
static const Expression* eq_expr;
// the first term for equality formula.
static Term first_eq_term;
// the first expression for equality formula.
static const Expression* first_eq_expr;
// kind of name map being parsed.
static enum { TYPE_KIND, CONSTANT_KIND, OBJECT_KIND, VOID_KIND } name_kind;

static void yyerror( const std::string &s ); 
static void yywarning( const std::string &s );
static void make_domain( const std::string *name );
static void make_problem( const std::string *name, const std::string *domain_name );
static void require_typing( void );
static void require_fluents( void );
static void require_disjunction( void );
static void require_conditional_effects( void );

// returns a simple type with the given name.
static Type make_type( const std::string *name );
// returns the union of the given types.
static Type make_type( const TypeSet& types );
static Term make_term( const std::string *name );

static void make_predicate( const std::string *name );
static void make_function( const std::string *name );
static void make_action( const std::string *name );
static void add_action( void );
static void prepare_forall_effect( void );
static const Effect* make_forall_effect( const Effect& effect );

// adds an outcome to the given probabilistic effect.
static void add_effect_outcome( ProbabilisticEffect& peffect, 
				const Rational* p, const Effect& effect );
static const Effect* make_add_effect( const Atom& atom );
static const Effect* make_delete_effect( const Atom& atom );
static const Effect* make_assignment_effect( unsigned oper,
					     const Application& application,
					     const Expression& expr );

// adds types, constants, or objects to the current domain or problem.
static void add_names( const std::vector<const std::string*>* names, Type type );
static void add_variables( const std::vector<const std::string*>* names, Type type );
static void prepare_atom( const std::string *name );
static void prepare_application( const std::string *name );
static void add_term( const std::string *name );
static const Atom* make_atom( void );
static const Application* make_application( void );
static const Expression* make_subtraction( const Expression& term, const Expression* opt_term );

// creates an atom or fluent for the given name to be used in an equality formula.
static void make_eq_name( const std::string *name );
static const StateFormula* make_equality( void );
static const StateFormula* make_negation( const StateFormula& negand );
static const StateFormula* make_implication( const StateFormula& f1, const StateFormula& f2 );
static void prepare_exists( void );
static void prepare_forall( void );
static const StateFormula* make_exists( const StateFormula& body );
static const StateFormula* make_forall( const StateFormula& body );

# line 174 "parser.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif

{
  unsigned setop;
  const Effect* effect;
  ConjunctiveEffect* ceffect;
  ProbabilisticEffect* peffect;
  const StateFormula* formula;
  const Atom* atom;
  Conjunction* conj;
  Disjunction* disj;
  const Expression* expr;
  const Application* appl;
  Comparison::CmpPredicate comp;
  Type type;
  TypeSet* types;
  std::string* str;
  std::vector<const std::string*>* strs;
  std::vector<const Atom*>* atoms;
  const Rational* num;
} YYSTYPE;
# define DEFINE 257
# define DOMAIN_TOKEN 258
# define PROBLEM 259
# define REQUIREMENTS 260
# define TYPES 261
# define CONSTANTS 262
# define PREDICATES 263
# define FUNCTIONS 264
# define STRIPS 265
# define TYPING 266
# define NEGATIVE_PRECONDITIONS 267
# define DISJUNCTIVE_PRECONDITIONS 268
# define EQUALITY 269
# define EXISTENTIAL_PRECONDITIONS 270
# define UNIVERSAL_PRECONDITIONS 271
# define QUANTIFIED_PRECONDITIONS 272
# define CONDITIONAL_EFFECTS 273
# define FLUENTS 274
# define ADL 275
# define DURATIVE_ACTIONS 276
# define DURATION_INEQUALITIES 277
# define CONTINUOUS_EFFECTS 278
# define PROBABILISTIC_EFFECTS 279
# define REWARDS 280
# define MDP 281
# define ACTION 282
# define PARAMETERS 283
# define PRECONDITION 284
# define EFFECT 285
# define PDOMAIN 286
# define OBJECTS 287
# define INIT 288
# define GOAL 289
# define GOAL_REWARD 290
# define METRIC 291
# define EXPECTED_COST 292
# define GOAL_PROBABILITY 293
# define WHEN 294
# define NOT 295
# define AND 296
# define OR 297
# define IMPLY 298
# define EXISTS 299
# define FORALL 300
# define PROBABILISTIC 301
# define ASSIGN 302
# define SCALE_UP 303
# define SCALE_DOWN 304
# define INCREASE 305
# define DECREASE 306
# define MINIMIZE 307
# define MAXIMIZE 308
# define NUMBER_TOKEN 309
# define OBJECT_TOKEN 310
# define EITHER 311
# define LT 312
# define LE 313
# define GE 314
# define GT 315
# define NAME 316
# define VARIABLE 317
# define NUMBER 318
# define ILLEGAL_TOKEN 319
# define PLUS 320
# define HYPHEN 321
# define STAR 322
# define SLASH 323
# define LEFTPAR 324
# define RIGHTPAR 325
# define LEFTBRACE 326
# define RIGHTBRACE 327
# define EQUAL 328
# define ORBIT 329
# define SYSTEM 330

#include <inttypes.h>

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#define	YYCONST	const
#else
#include <malloc.h>
#include <memory.h>
#define	YYCONST
#endif

#include <values.h>

#if defined(__cplusplus) || defined(__STDC__)

#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
#ifndef yyerror
#if defined(__cplusplus)
	void yyerror(YYCONST char *);
#endif
#endif
#ifndef yylex
	int yylex(void);
#endif
	int yyparse(void);
#if defined(__cplusplus) && defined(__EXTERN_C__)
}
#endif

#endif

#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
YYSTYPE yylval;
YYSTYPE yyval;
typedef int yytabelem;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#if YYMAXDEPTH > 0
int yy_yys[YYMAXDEPTH], *yys = yy_yys;
YYSTYPE yy_yyv[YYMAXDEPTH], *yyv = yy_yyv;
#else	/* user does initial allocation */
int *yys;
YYSTYPE *yyv;
#endif
static int yymaxdepth = YYMAXDEPTH;
# define YYERRCODE 256

# line 1260 "parser.y"


void
yyerror( const char *s )
{
  std::string str( s );
  yyerror( str );
}

static void
yyerror( const std::string &s )
{
  std::cout << "<parser>:" << current_file << ':' << line_number << ": " << s
	    << std::endl;
  success = false;
}

static void
yywarning( const std::string &s )
{
  if( gpt::warning_level > 0 )
    {
      std::cout << "<parser>:" << current_file << ':' << line_number << ": " << s
		<< std::endl;
      if( gpt::warning_level > 1 )
	success = false;
    }
}

static void
make_domain( const std::string *name )
{
  domain = new Domain( *name );
  domains[*name] = domain;
  requirements = &domain->requirements;
  problem = NULL;
  delete name;
}

static void
make_problem( const std::string *name, const std::string *domain_name )
{
  std::map<std::string,Domain*>::const_iterator di = domains.find( *domain_name );
  if( di != domains.end() )
    domain = (*di).second;
  else
    {
      domain = new Domain( *domain_name );
      domains[*domain_name] = domain;
      yyerror( "undeclared domain `" + *domain_name + "' used" );
    }

  // analyze and simplify domain
  if( gpt::domain_analysis )
    domain->analyze_and_simplify();

  requirements = new Requirements( domain->requirements );
  problem = problem_t::allocate( *name, *domain );
  if( requirements->rewards )
    {
      const Application& reward_appl =
	Application::make_application( reward_function, TermList() );
      const Assignment* reward_assignment =
	new Assignment( Assignment::ASSIGN_OP, reward_appl, *new Value(0) );
      problem->add_init_effect( *new AssignmentEffect( *reward_assignment ) );
    }
  delete name;
  delete domain_name;
}

static void
require_typing( void )
{
  if( !requirements->typing )
    {
      yywarning( "assuming `:typing' requirement" );
      requirements->typing = true;
    }
}

static void
require_fluents( void )
{
  if( !requirements->fluents )
    {
      yywarning( "assuming `:fluents' requirement" );
      requirements->fluents = true;
    }
}

static void
require_disjunction( void )
{
  if( !requirements->disjunctive_preconditions )
    {
      yywarning( "assuming `:disjunctive-preconditions' requirement" );
      requirements->disjunctive_preconditions = true;
    }
}

static void
require_conditional_effects( void )
{
  if( !requirements->conditional_effects )
    {
      yywarning( "assuming `:conditional-effects' requirement" );
      requirements->conditional_effects = true;
    }
}

static Type
make_type( const std::string *name )
{
  std::pair<Type,bool> t = domain->types().find_type( *name );
  if( !t.second )
    {
      t.first = domain->types().add_type( *name );
      if( name_kind != TYPE_KIND )
	yywarning( "implicit declaration of type `" + *name + "'" );
    }
  delete name;
  return( t.first );
}

static Type
make_type( const TypeSet& types )
{
  return( domain->types().add_type( types ) );
}

static Term
make_term( const std::string *name )
{
  if( (*name)[0] == '?' )
    {
      std::pair<Variable,bool> v = context.find( *name );
      if( !v.second )
	{
	  if( problem != NULL )
	    v.first = problem->terms().add_variable( OBJECT_TYPE );
	  else
	    v.first = domain->terms().add_variable( OBJECT_TYPE );
	  context.insert( *name, v.first );
	  yyerror( "free variable `" + *name + "' used" );
	}
      delete name;
      return( v.first );
    }
  else
    {
      TermTable& terms = (problem!=NULL?problem->terms():domain->terms());
      const PredicateTable& predicates = domain->predicates();
      std::pair<Object,bool> o = terms.find_object( *name );
      if( !o.second )
	{
	  size_t n = term_parameters.size();
	  if( parsing_atom && (predicates.arity( atom_predicate ) > n) )
	    o.first = terms.add_object( *name, predicates.parameter( atom_predicate, n ) );
	  else
	    o.first = terms.add_object( *name, OBJECT_TYPE );
	}
      delete name;
      return( o.first );
    }
}

static void
make_predicate( const std::string *name )
{
  repeated_predicate = false;
  std::pair<Predicate,bool> p = domain->predicates().find_predicate( *name );
  if( !p.second )
    p.first = domain->predicates().add_predicate( *name );
  else
    {
      repeated_predicate = true;
      yywarning( "ignoring repeated declaration of predicate `" + *name + "'" );
    }
  predicate = p.first;
  parsing_predicate = true;
  delete name;
}

static void
make_function( const std::string *name )
{
  repeated_function = false;
  std::pair<Function,bool> f = domain->functions().find_function( *name );
  if( !f.second )
    f.first = domain->functions().add_function( *name );
  else
    {
      repeated_function = true;
      if( requirements->rewards && (f.first == reward_function) )
	yywarning( "ignoring declaration of reserved function `reward'" );
      else
	yywarning( "ignoring repeated declaration of function `" + *name + "'" );
    }
  function = f.first;
  parsing_function = true;
  delete name;
}

static void
make_action( const std::string *name )
{
  context.push_frame();
  action = new ActionSchema( *name );
  delete name;
}

static void
add_action( void )
{
  context.pop_frame();
  if( domain->find_action( action->name() ) == NULL )
    domain->add_action( *action );
  else
    {
      yywarning( "ignoring repeated declaration of action `" + action->name() + "'" );
      delete action;
    }
  action = NULL;
}

static void
prepare_forall_effect( void )
{
  if( !requirements->conditional_effects )
    {
      yywarning( "assuming `:conditional-effects' requirement" );
      requirements->conditional_effects = true;
    }
  context.push_frame();
  quantified.push_back( NULL_TERM );
}

static const Effect*
make_forall_effect( const Effect& effect )
{
  context.pop_frame();
  QuantifiedEffect* qeffect = new QuantifiedEffect( effect );
  Effect::unregister_use( &effect );
  size_t n = quantified.size() - 1;
  size_t m = n;
  for( ; is_variable( quantified[n] ); --n );
  for( size_t i = n + 1; i <= m; ++i )
    qeffect->add_parameter( quantified[i] );
  quantified.resize( n );
  return( qeffect );
}

static void
add_effect_outcome( ProbabilisticEffect& peffect, const Rational* p, const Effect& effect )
{
  if( !requirements->probabilistic_effects )
    {
      yywarning( "assuming `:probabilistic-effects' requirement" );
      requirements->probabilistic_effects = true;
    }
  if( (*p < 0) || (*p > 1) )
    yyerror( "outcome probability needs to be in the interval [0,1]" );
  if( !peffect.add_outcome( *p, effect ) )
    yyerror( "effect outcome probabilities add up to more than 1" );
  delete p;
}

static const Effect* 
make_add_effect( const Atom& atom )
{
  domain->predicates().make_dynamic( atom.predicate() );
  const Effect *result = new AddEffect( atom );
  StateFormula::unregister_use( &atom );
  return( result );
}

static const Effect* 
make_delete_effect( const Atom& atom )
{
  domain->predicates().make_dynamic( atom.predicate() );
  const Effect *result = new DeleteEffect( atom );
  StateFormula::unregister_use( &atom );
  return( result );
}

static const Effect*
make_assignment_effect( unsigned oper,
			const Application& application,
			const Expression& expr )
{
  if( requirements->rewards && (application.function() == reward_function) )
    {
      if( ((oper != Assignment::INCREASE_OP) && (oper != Assignment::DECREASE_OP)) ||
	  (typeid(expr) != typeid(Value)) )
	yyerror( "only constant reward increments/decrements allowed" );
    }
  else
    require_fluents();
  effect_fluent = false;
  domain->functions().make_dynamic( application.function() );
  const Assignment& assignment = *new Assignment( oper, application, expr );
  //Expression::unregister_use( &expr );
  const Effect* result = new AssignmentEffect( assignment );
  return( result );
}

static void
add_names( const std::vector<const std::string*>* names, Type type )
{
  for( std::vector<const std::string*>::const_iterator si = names->begin(); si != names->end(); si++ )
    {
      const std::string *s = *si;
      if( name_kind == TYPE_KIND )
	{
	  if( *s == OBJECT_NAME )
	    yywarning( "ignoring declaration of reserved type `object'" );
	  else if( *s == NUMBER_NAME )
	    yywarning( "ignoring declaration of reserved type `number'" );
	  else
	    {
	      std::pair<Type,bool> t = domain->types().find_type( *s );
	       if( !t.second )
		 t.first = domain->types().add_type( *s );
	       if( !domain->types().add_supertype( t.first, type ) )
		 yyerror( "cyclic type hierarchy" );
	    }
	}
      else if( name_kind == CONSTANT_KIND )
	{
	  std::pair<Object,bool> o = domain->terms().find_object( *s );
	  if( !o.second )
	    domain->terms().add_object( *s, type );
	  else
	    {
	      TypeSet components;
	      domain->types().components( components, domain->terms().type( o.first ) );
	      components.insert( type );
	      domain->terms().set_type( o.first, make_type( components ) );
	    }
	}
      else // name_kind == OBJECT_KIND 
	{ 
	  if( domain->terms().find_object( *s ).second )
	    yywarning( "ignoring declaration of object `" + *s + "' previously declared as constant" );
	  else
	    {
	      std::pair<Object,bool> o = problem->terms().find_object( *s );
	      if( !o.second )
		problem->terms().add_object( *s, type );
	      else
		{
		  TypeSet components;
		  domain->types().components( components, problem->terms().type( o.first ) );
		  components.insert( type );
		  problem->terms().set_type( o.first, make_type( components ) );
		}
	    }
	}
      delete s;
    }
  delete names;
}

static void
add_variables( const std::vector<const std::string*>* names, Type type )
{
  for( std::vector<const std::string*>::const_iterator si = names->begin(); si != names->end(); ++si )
    {
      const std::string *s = *si;
      if( parsing_predicate )
	{
	  if( !repeated_predicate )
	    domain->predicates().add_parameter( predicate, type );
	}
      else if( parsing_function )
	{
	  if( !repeated_function )
	    domain->functions().add_parameter( function, type );
	}
      else
	{
	  if( context.shallow_find(*s).second )
	    yyerror( "repetition of parameter `" + *s + "'" );
	  else if( context.find(*s).second )
	    yywarning( "shadowing parameter `" + *s + "'" );
	  
	  Variable var;
	  if( problem != NULL )
	    var = problem->terms().add_variable( type );
	  else
	    var = domain->terms().add_variable( type );

	  context.insert( *s, var );
	  if( !quantified.empty() )
	    quantified.push_back( var );
	  else
	    action->add_parameter( var );
	}
      delete s;
    }
  delete names;
}

static void
prepare_atom( const std::string *name )
{
  std::pair<Predicate,bool> p = domain->predicates().find_predicate( *name );
  if( !p.second )
    {
      atom_predicate = domain->predicates().add_predicate( *name );
      undeclared_atom_predicate = true;
      if( problem != NULL )
	yywarning( "undeclared predicate `" + *name + "' used" );
      else
	yywarning( "implicit declaration of predicate `" + *name + "'" );
    }
  else
    {
      atom_predicate = p.first;
      undeclared_atom_predicate = false;
    }
  term_parameters.clear();
  parsing_atom = true;
  delete name;
}

static void 
prepare_application( const std::string *name )
{
  std::pair<Function,bool> f = domain->functions().find_function( *name );
  if( !f.second )
    {
      appl_function = domain->functions().add_function( *name );
      undeclared_appl_function = true;
      if( problem != NULL )
	yywarning( "undeclared function `" + *name + "' used" );
      else
	yywarning( "implicit declaration of function `" + *name + "'" );
    }
  else
    {
      appl_function = f.first;
      undeclared_appl_function = false;
    }

  if( requirements->rewards && (f.first == reward_function) )
    {
      if( false && !effect_fluent )
	yyerror( "reserved function `reward' not allowed here" );
    }
  else
    require_fluents();
  term_parameters.clear();
  parsing_application = true;
  delete name;
}

static void 
add_term( const std::string *name )
{
  Term term = make_term( name );
  const TermTable& terms = (problem!=NULL?problem->terms():domain->terms());
  if( parsing_atom )
    {
      PredicateTable& predicates = domain->predicates();
      size_t n = term_parameters.size();
      if( undeclared_atom_predicate )
	{
	  predicates.add_parameter( atom_predicate, terms.type( term ) );
	}
      else if( (predicates.arity( atom_predicate ) > n) &&
	       !domain->types().subtype( terms.type(term),
					 predicates.parameter(atom_predicate,n) ) )
	yyerror( "type mismatch" );
    }
  else if( parsing_application )
    {
      FunctionTable& functions = domain->functions();
      size_t n = term_parameters.size();
      if( undeclared_appl_function )
	functions.add_parameter( appl_function, terms.type( term ) );
      else if( (functions.arity( appl_function ) > n) &&
	       !domain->types().subtype( terms.type(term),
					 functions.parameter(appl_function,n) ) )
	yyerror( "type mismatch" );
    }
  term_parameters.push_back( term );
}

static const Atom* 
make_atom( void )
{
  size_t n = term_parameters.size();
  if( domain->predicates().arity( atom_predicate ) < n )
    yyerror( "too many parameters passed to predicate `"
	     + domain->predicates().name(atom_predicate) + "'" );
  else if( domain->predicates().arity( atom_predicate ) > n )
    yyerror( "too few parameters passed to predicate `"
	     + domain->predicates().name(atom_predicate) + "'" );
  parsing_atom = false;
  return( &Atom::make_atom( atom_predicate, term_parameters ) );
}

static const Application* 
make_application( void )
{
  size_t n = term_parameters.size();
  if( domain->functions().arity( appl_function ) < n )
    yyerror( "too many parameters passed to function `"
	     + domain->functions().name(appl_function) + "'" );
  else if( domain->functions().arity( appl_function ) > n )
    yyerror( "too few parameters passed to function `"
	     + domain->functions().name(appl_function) + "'" );
  parsing_application = false;
  return( &Application::make_application( appl_function, term_parameters ) );
}

static const Expression* 
make_subtraction( const Expression& term, const Expression* opt_term )
{
  if( opt_term != NULL )
    {
      const Expression *result = new Subtraction( term, *opt_term );
      Expression::unregister_use( &term );
      Expression::unregister_use( opt_term );
      return( result );
    }
  else
    {
      opt_term = new Value( 0 );
      const Expression *result = new Subtraction( *opt_term, term );
      Expression::unregister_use( &term );
      Expression::unregister_use( opt_term );
      return( result );
    }
}

static void 
make_eq_name( const std::string *name )
{
  std::pair<Function,bool> f = domain->functions().find_function( *name );
  if( f.second )
    {
      prepare_application( name );
      eq_expr = make_application();
    }
  else
    {
      // assume this is a term.
      eq_term = make_term( name );
      eq_expr = NULL;
    }
}

static const StateFormula* 
make_equality( void )
{
  if( !requirements->equality )
    {
      yywarning( "assuming `:equality' requirement" );
      requirements->equality = true;
    }
  if( (first_eq_expr != NULL) && (eq_expr != NULL) )
    {
      return( new Comparison( Comparison::EQ_CMP, *first_eq_expr, *eq_expr ) );
    }
  else if( (first_eq_expr == NULL) && (eq_expr == NULL) )
    {
      const TermTable& terms = (problem!=NULL?problem->terms():domain->terms());
      if( domain->types().subtype( terms.type(first_eq_term), terms.type(eq_term) ) ||
	  domain->types().subtype( terms.type(eq_term), terms.type(first_eq_term) ) )
	return( new Equality( first_eq_term, eq_term ) );
      else
	{
	  StateFormula::register_use( &StateFormula::FALSE );
	  return( &StateFormula::FALSE );
	}
    }
  else
    {
      yyerror( "comparison of term and numeric expression" );
      StateFormula::register_use( &StateFormula::FALSE );
      return( &StateFormula::FALSE );
    }
}

static const StateFormula* 
make_negation( const StateFormula& negand )
{
  if( (typeid(negand) == typeid(Atom)) && !requirements->negative_preconditions )
    {
      yywarning( "assuming `:negative-preconditions' requirement" );
      requirements->negative_preconditions = true;
    }
  else if( (typeid(negand) != typeid(Equality)) && (typeid(negand) != typeid(Comparison)) )
    {
      require_disjunction();
    }
  return( &Negation::make_negation( negand ) );
}

static const StateFormula* 
make_implication( const StateFormula& f1, const StateFormula& f2 )
{
  require_disjunction();
  Disjunction* disj = new Disjunction();
  disj->add_disjunct( Negation::make_negation( f1 ) );
  disj->add_disjunct( f2 );
  return( disj );
}

static void 
prepare_exists( void )
{
  if( !requirements->existential_preconditions )
    {
      yywarning( "assuming `:existential-preconditions' requirement" );
      requirements->existential_preconditions = true;
    }
  context.push_frame();
  quantified.push_back( NULL_TERM );
}

static void 
prepare_forall( void )
{
  if( !requirements->universal_preconditions )
    {
      yywarning( "assuming `:universal-preconditions' requirement" );
      requirements->universal_preconditions = true;
    }
  context.push_frame();
  quantified.push_back( NULL_TERM );
}

static const StateFormula* 
make_exists( const StateFormula& body )
{
  context.pop_frame();
  size_t n, m = quantified.size() - 1;
  for( n = m; is_variable( quantified[n] ); --n );
  if( n < m )
    {
      Exists* exists = new Exists;
      for( size_t i = n + 1; i <= m; ++i )
	exists->add_parameter( quantified[i] );
      exists->set_body( body );
      StateFormula::unregister_use( &body );
      quantified.resize( n );
      return( exists );
    }
  else
    {
      quantified.pop_back();
      return( &body );
    }
}

static const StateFormula* 
make_forall( const StateFormula& body )
{
  context.pop_frame();
  size_t n, m = quantified.size() - 1;
  for( n = m; is_variable( quantified[n] ); --n );
  if( n < m )
    {
      Forall* forall = new Forall;
      for( size_t i = n + 1; i <= m; ++i )
	forall->add_parameter( quantified[i] );
      forall->set_body( body );
      StateFormula::unregister_use( &body );
      quantified.resize( n );
      return( forall );
    }
  else
    {
      quantified.pop_back();
      return( &body );
    }
}
static YYCONST yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 308
# define YYLAST 1223
static YYCONST yytabelem yyact[]={

   241,   250,   163,   427,   313,   389,   430,   459,   238,   267,
   458,   426,   207,    14,    37,   242,   239,   230,   324,   341,
   318,   371,   379,    58,   203,   204,   205,   206,   207,   398,
    15,    16,    17,   319,   342,   132,   130,   489,   251,   483,
   397,   424,   228,   201,   202,   203,   204,   205,   204,   205,
   201,   202,   203,   204,   205,   482,   481,    83,   480,   479,
   382,   151,   152,   153,   206,   207,   107,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    18,    19,    20,   206,   207,   206,   207,    36,
   231,   478,   206,   207,   202,   203,   204,   205,   462,   277,
   278,   279,   280,   281,   282,   205,   128,   128,    15,    16,
    17,   155,   156,   150,   149,   154,   273,   274,   275,   276,
   157,   342,   477,   476,   475,   474,   473,   449,   384,   378,
   144,   377,   264,   147,   448,   206,   207,   447,   446,   148,
   445,   162,   444,   206,   207,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    18,    19,    20,   439,   433,   432,   412,    36,   231,   387,
   372,   353,   351,   350,   346,   309,   456,   308,    15,    16,
    17,   301,   263,   260,   236,   198,   161,   158,   128,   136,
   221,    86,    87,    88,    89,    90,    91,    92,    93,    94,
    95,    96,    97,    98,    99,   100,   101,   102,   146,   151,
   152,   153,   249,   245,   142,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    18,    19,    20,   235,   123,    55,    39,    36,   231,    38,
   246,   272,   151,   152,   153,   272,   415,   262,   278,   307,
   128,   124,   217,   215,   283,   213,   211,   307,   209,   155,
   156,   150,   149,   154,   200,   197,   136,   327,   157,   169,
   151,   152,   153,    82,   311,   330,   422,   120,   333,   115,
   453,   111,   328,    80,    75,    70,   290,    65,    57,    45,
    41,     9,   155,   156,   150,   149,   154,     6,   231,   257,
   342,   157,   145,   231,   343,   256,   259,   349,   255,   254,
   314,   234,    54,   347,   194,   196,   307,   352,   349,   196,
   155,   156,   150,   149,   154,   224,   225,   165,   429,   157,
   286,   367,    61,    62,    63,    60,    61,   368,    62,   356,
   374,   453,   376,    60,    60,   272,    62,     8,   383,    12,
    13,    63,   391,   381,   245,    63,    63,   386,   245,    61,
   385,    85,   266,    63,    63,   327,   361,   388,   393,    58,
    59,    60,    61,    62,   108,   352,   405,   396,    63,   143,
   328,   399,   416,   193,   411,    59,    60,    61,    62,   171,
   184,    63,   133,   112,   307,   423,   421,   417,   322,   419,
   245,   420,   245,    60,    61,    62,    63,   271,   434,   435,
   436,   437,   425,   117,   440,   441,   442,   443,   431,   233,
   413,   414,    47,    63,   135,   418,   307,   438,   258,   349,
   151,   152,   153,   455,   416,   175,   186,   451,   452,   185,
    43,   416,   457,   168,   460,   461,   125,   116,   463,   417,
   465,   466,   464,   467,   468,    44,   417,   470,   391,   472,
   360,   182,   469,   245,    15,    16,    17,   181,   177,    64,
   179,   121,   307,   455,   488,   486,   323,   173,   126,   450,
   155,   156,   150,   149,   154,   395,   404,   403,   349,   157,
   402,   401,   122,   400,    15,    16,    17,   487,   485,    56,
   394,    21,    22,    23,    24,    25,    26,    27,    28,    29,
    30,    31,    32,    33,    34,    35,    18,    19,    20,   151,
   152,   153,   336,    36,   335,   334,   270,   406,   407,   408,
   409,    21,    22,    23,    24,    25,    26,    27,    28,    29,
    30,    31,    32,    33,    34,    35,    18,    19,    20,    15,
    16,    17,   188,    36,   231,   325,   289,   277,   278,   160,
   208,   326,   282,   290,   296,   297,   298,   299,   300,   155,
   156,   150,   149,   154,   220,   269,   237,   305,   157,    15,
    16,    17,   167,   129,   321,   316,    21,    22,    23,    24,
    25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
    35,    18,    19,    20,   151,   152,   153,   287,    36,   261,
   212,   199,   363,   364,   365,   366,    21,    22,    23,    24,
    25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
    35,    18,    19,    20,    15,    16,    17,   348,    36,   332,
   268,   329,   219,   362,   355,   214,   359,   484,   218,   216,
   210,   253,   284,   252,   155,   156,   150,   149,   154,   304,
   248,   247,   183,   157,   222,    15,    16,    17,   180,   178,
   176,    21,    22,    23,    24,    25,    26,    27,    28,    29,
    30,    31,    32,    33,    34,    35,    18,    19,    20,   174,
   172,   170,   166,    36,   141,   369,    11,   295,    15,    16,
    17,   370,    21,    22,    23,    24,    25,    26,    27,    28,
    29,    30,    31,    32,    33,    34,    35,    18,    19,    20,
   294,   293,   292,   291,    36,   345,   302,   339,   285,    15,
    16,    17,   338,   454,   310,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    18,    19,    20,   195,   192,   191,   164,    36,   137,   190,
    15,    16,    17,   232,   189,   312,    21,    22,    23,    24,
    25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
    35,    18,    19,    20,   159,   134,   187,   131,    36,   106,
   105,    15,    16,    17,   104,   103,   390,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    18,    19,    20,    84,    53,    76,    71,    36,
    15,    16,    17,    66,    46,    42,    40,   312,    21,    22,
    23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
    33,    34,    35,    18,    19,    20,   151,   152,   153,    10,
    36,     7,    49,     5,     4,   145,     3,    21,    22,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    34,    35,    18,    19,    20,   151,   152,   153,     2,    36,
     1,   357,    50,   315,   320,   151,   152,   153,   223,   229,
   127,   265,   375,   331,   392,   340,   155,   156,   150,   149,
   154,    67,   471,    78,   337,   157,   151,   152,   153,   243,
   288,    48,     0,   240,   410,     0,   151,   152,   153,     0,
     0,   113,     0,     0,     0,   155,   156,   150,   149,   154,
   113,    68,    73,     0,   157,   155,   156,   150,   149,   154,
     0,     0,   244,   380,   157,   151,   152,   153,     0,     0,
   109,     0,   240,   373,     0,   109,   155,   156,   150,   149,
   154,    72,    77,     0,     0,   157,   155,   156,   150,   149,
   154,     0,     0,   317,   354,   157,   151,   152,   153,     0,
     0,     0,     0,   306,   303,   118,   151,   152,   153,     0,
   118,     0,     0,     0,     0,   155,   156,   150,   149,   154,
     0,   151,   152,   153,   157,     0,     0,     0,     0,     0,
     0,     0,   244,   151,   152,   153,     0,     0,     0,     0,
     0,     0,     0,   151,   152,   153,   155,   156,   150,   149,
   154,     0,     0,     0,     0,   157,   155,   156,   150,   149,
   154,     0,     0,   240,     0,   157,   151,   152,   153,     0,
     0,   155,   156,   428,   227,   154,   151,   152,   153,     0,
   157,     0,     0,   155,   156,   150,   149,   154,     0,   358,
     0,     0,   157,   155,   156,   150,   149,   154,     0,     0,
   344,    52,   157,     0,     0,    51,     0,     0,     0,     0,
   317,     0,     0,     0,     0,     0,   155,   156,     0,   227,
   154,     0,     0,     0,     0,   157,   155,   156,     0,   227,
   154,     0,     0,   226,     0,   157,    86,    87,    88,    89,
    90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
   100,   101,   102,    81,    69,    74,    79,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    81,     0,   110,   114,     0,    81,     0,   110,   119,
     0,    81,     0,   114,   119,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    81,     0,     0,   138,    81,     0,     0,   139,
     0,    81,     0,     0,   140,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    81,    81,    81 };
static YYCONST yytabelem yypact[]={

-10000000,-10000000,-10000000,   -27,-10000000,-10000000,    90,   -33,-10000000,    91,
   563,   563,-10000000,-10000000,   -86,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,   -89,-10000000,   -34,
   -35,    26,   -90,   -36,-10000000,   109,   -37,-10000000,   -39,   -40,
   -41,   -51,-10000000,-10000000,   563,-10000000,-10000000,   124,   851,-10000000,
-10000000,-10000000,-10000000,   563,-10000000,   141,-10000000,   -43,   -45,   -51,
    69,-10000000,   -43,   -47,   -51,    82,-10000000,   -45,   -47,   -51,
    73,-10000000,    52,   -91,   -74,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,   563,   563,  -289,   -58,-10000000,-10000000,   -51,
   -51,    74,-10000000,   -51,   -51,    96,-10000000,-10000000,   -51,   -51,
    81,-10000000,-10000000,-10000000,-10000000,-10000000,  -111,   534,-10000000,  -117,
-10000000,-10000000,   347,  -138,  -135,-10000000,   563,    44,   -51,   -51,
   -51,   -55,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,   -58,
-10000000,-10000000,-10000000,-10000000,    30,   -59,  -140,   -60,-10000000,  -237,
   -66,-10000000,   -68,-10000000,   -69,-10000000,   -71,-10000000,   -72,-10000000,
   -72,-10000000,-10000000,  -134,-10000000,   563,   789,   -14,-10000000,     2,
   -14,  -141,    34,-10000000,   719,-10000000,   688,   -14,-10000000,-10000000,
  -244,-10000000,-10000000,   719,  -280,     1,   -11,   -17,-10000000,  -194,
-10000000,  -265,-10000000,  -242,-10000000,  -186,-10000000,  -302,-10000000,-10000000,
-10000000,  -318,-10000000,-10000000,-10000000,-10000000,    -5,-10000000,  -142,   -19,
-10000000,-10000000,-10000000,-10000000,-10000000,  -143,-10000000,-10000000,-10000000,-10000000,
  -196,-10000000,-10000000,-10000000,   262,-10000000,  -144,   563,   659,  -148,
  -150,-10000000,   441,    18,-10000000,-10000000,   766,  -293,   799,-10000000,
-10000000,-10000000,-10000000,-10000000,   237,-10000000,   719,-10000000,-10000000,   719,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,   -18,   756,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,  -151,-10000000,-10000000,-10000000,   -15,-10000000,-10000000,-10000000,
  -152,  -153,   563,-10000000,  -154,   649,-10000000,   347,  -293,-10000000,
   744,-10000000,-10000000,   -14,-10000000,-10000000,   292,-10000000,-10000000,   377,
  -155,   628,-10000000,   719,  -193,  -195,-10000000,   618,  -264,   719,
  -197,   688,-10000000,  -156,   347,   503,-10000000,   472,   -18,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -293,  -287,-10000000,-10000000,
-10000000,-10000000,   237,-10000000,-10000000,-10000000,-10000000,-10000000,   377,-10000000,
   207,-10000000,-10000000,-10000000,-10000000,   589,  -159,   -14,   -14,   -79,
-10000000,-10000000,   -14,   688,-10000000,   688,-10000000,-10000000,   377,   -42,
   563,-10000000,  -284,   729,-10000000,-10000000,  -160,-10000000,-10000000,  -161,
   377,   377,   377,   377,-10000000,  -162,   377,   377,   377,   377,
-10000000,-10000000,-10000000,  -183,  -185,-10000000,-10000000,-10000000,  -187,  -188,
-10000000,  -191,  -198,-10000000,-10000000,   729,-10000000,-10000000,   -48,-10000000,
   408,  -149,-10000000,-10000000,   377,   377,   377,   377,  -227,-10000000,
   377,   377,   377,   377,   719,   719,   688,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,   472,-10000000,-10000000,-10000000,  -199,  -200,-10000000,
  -201,  -202,-10000000,  -203,  -234,  -266,  -267,  -269,  -270,  -286,
   322,   173,  -280,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,    13,  -288,-10000000 };
static YYCONST yytabelem yypgo[]={

     0,   910,    15,   909,    11,     3,   904,   902,   895,   894,
     8,   328,    16,   893,   892,     1,     7,    10,     5,    21,
   891,   890,   889,   379,   888,   884,   139,     0,     4,     2,
    17,    19,   883,   881,    20,   880,   878,   856,   854,   853,
   851,   849,   826,   825,   440,   455,   824,   422,   911,   852,
   882,   823,   818,   817,  1085,   374,   393,   413,  1081,   816,
   815,   361,   795,   478,   794,   790,   789,   392,   787,   786,
    42,   785,   784,   424,   764,   763,   759,   758,   756,   755,
   754,   383,   753,     9,   407,   732,   728,   727,   330,   362,
   725,   723,   722,   721,   720,   697,   696,   694,   692,   443,
   691,   389,   690,   477,   689,   435,   670,   468,   669,   470,
   668,   467,   662,   661,   660,   659,   653,   651,   461,   390,
    18,   643,   641,   640,   639,   575,   526,   525,   524,   522,
    22,   500,     6,   493,   491,   490,   487,   486,   485,   479,
   476,   439,   436,   325,   428,   419 };
static YYCONST yytabelem yyr1[]={

     0,    36,    35,    37,    37,    37,    42,    38,    43,    43,
    43,    43,    45,    45,    45,    47,    47,    47,    47,    47,
    47,    47,    51,    51,    51,    51,    51,    52,    52,    52,
    52,    52,    53,    53,    53,    53,    53,    55,    55,    55,
    56,    56,    56,    57,    57,    57,    54,    54,    58,    44,
    60,    60,    61,    61,    61,    61,    61,    61,    61,    61,
    61,    61,    61,    61,    61,    61,    61,    61,    61,    62,
    46,    64,    48,    49,    66,    50,    65,    65,    69,    68,
    67,    67,    67,    71,    71,    74,    72,    76,    73,    77,
    59,    78,    78,    79,    79,    81,    81,    80,    82,     2,
     2,    85,     2,    87,     2,     2,     6,     6,     8,     8,
    31,     3,     3,    90,     3,     1,     1,     1,     1,     1,
    97,    39,    98,    98,    99,    99,   101,   101,   103,   103,
   105,   105,   107,   107,   109,   109,   111,   111,   113,   100,
   102,   114,   114,   115,   115,   115,     9,     9,     4,     4,
     7,     7,     5,     5,    15,   104,   106,   108,   108,   108,
   110,   110,   118,    32,    32,   112,   112,   119,    34,    33,
    33,    10,   121,    10,   122,    10,    10,    10,   124,    10,
    10,   127,    10,   128,    10,    13,    13,    14,    14,   129,
    12,    12,   131,    11,    11,    20,    20,    20,    20,    16,
    16,    16,    16,    16,    16,   120,   133,   120,   134,   120,
   135,   120,   136,   120,   137,   120,   120,   120,    17,    17,
   138,    19,    19,   139,    18,    18,   130,   130,   130,   132,
   132,    70,   140,    70,    70,    22,    22,    63,   141,    63,
    63,    21,    21,   142,    23,    24,    24,    24,    25,    25,
    25,    25,    75,    40,    41,    96,    86,    89,    83,   123,
   125,   126,    84,    88,    91,    92,    93,    94,    95,   117,
   116,   145,   143,   144,    26,    26,    26,    26,    26,    26,
    26,    27,    27,    27,    28,    29,    29,    29,    29,    29,
    29,    29,    29,    29,    29,    29,    29,    29,    29,    29,
    29,    29,    29,    29,    29,    29,    29,    30 };
static YYCONST yytabelem yyr2[]={

     0,     1,     5,     4,     4,     0,     1,    18,     2,     4,
     2,     0,     2,     4,     2,     2,     2,     2,     4,     4,
     4,     2,     2,     2,     4,     4,     2,     2,     2,     4,
     4,     2,     2,     2,     4,     4,     2,     2,     4,     2,
     2,     4,     2,     2,     4,     2,     2,     4,     2,     8,
     2,     4,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
    11,     1,    11,     8,     1,    10,     4,     0,     1,    11,
     2,     6,     0,     2,     4,     1,     6,     1,    11,     1,
    15,     8,     0,     4,     2,     2,     0,     5,     5,     2,
     9,     1,    17,     1,    13,     9,     5,     1,     5,     7,
     2,     3,     9,     1,    13,     3,     3,     3,     3,     3,
     1,    27,     4,     2,     4,     2,     4,     2,     4,     2,
     4,     2,     4,     2,     4,     2,     2,     0,     1,    11,
     8,     4,     0,     3,    11,     9,     5,     7,     2,     9,
     5,     1,     3,    11,     3,     9,     8,    11,    11,    10,
     4,     2,    11,     5,     3,     4,     2,    15,     7,     5,
     1,     3,     1,    13,     1,    13,     9,     9,     1,    11,
    11,     1,    17,     1,    17,     5,     1,     5,     1,     1,
    11,     3,     1,    11,     3,     3,     3,     3,     3,     3,
    11,    11,    11,    11,     3,     3,     1,    13,     1,    13,
     1,    13,     1,    13,     1,    11,     3,     3,     2,     1,
     1,    11,     3,     1,    11,     3,     5,     5,     0,     5,
     0,     3,     1,     8,     0,     3,     5,     3,     1,     8,
     0,     3,     5,     1,     7,     3,     3,     9,     3,     3,
     5,     5,     2,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2 };
static YYCONST yytabelem yychk[]={

-10000000,   -35,   -36,   -37,   -38,   -39,   324,   -40,   257,   324,
   -41,   -96,   258,   259,   -29,   257,   258,   259,   309,   310,
   311,   294,   295,   296,   297,   298,   299,   300,   301,   302,
   303,   304,   305,   306,   307,   308,   316,   -29,   325,   325,
   -42,   324,   -43,   -44,   -45,   324,   -46,   -47,   -48,   -49,
   -50,   -54,   -58,   -59,   286,   325,   -45,   324,   260,   261,
   262,   263,   264,   282,   -47,   324,   -51,   -49,   -50,   -54,
   324,   -52,   -48,   -50,   -54,   324,   -53,   -48,   -49,   -54,
   324,   -58,   324,   -29,   -60,   -61,   265,   266,   267,   268,
   269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
   279,   280,   281,   -62,   -64,   -65,   -66,   -29,   -55,   -50,
   -54,   324,   -56,   -49,   -54,   324,   -55,   -57,   -48,   -54,
   324,   -56,   -57,   325,   325,   -61,   -63,   -21,   -29,   -63,
   325,   -68,   324,   -67,   -71,   -73,   324,   -77,   -54,   -54,
   -54,   -97,   325,   -23,   -29,   321,   325,   -27,   -26,   310,
   309,   257,   258,   259,   311,   307,   308,   316,   325,   -72,
   -73,   321,   -28,   -29,   -78,   283,   -98,   -44,   -99,   324,
  -100,  -101,  -102,  -103,  -104,  -105,  -106,  -107,  -108,  -109,
  -110,  -111,  -118,  -112,  -119,  -141,  -142,   -69,   -67,   -74,
   -76,   -79,   -80,   -81,   284,   -82,   285,   324,   325,   -99,
   324,   287,   288,   289,   290,   291,   329,   330,  -101,   324,
  -103,   324,  -105,   324,  -107,   324,  -109,   324,  -111,  -118,
  -119,   324,   -63,   -24,  -143,   -26,   324,   310,   -70,   -22,
   -30,   317,   -75,  -145,   309,   -70,   325,   -81,   -10,   -12,
   324,   -27,    -2,    -3,   324,   -12,   -70,  -113,  -114,   -10,
   -15,   318,  -116,  -117,   308,   307,   316,   316,  -144,   311,
   325,   -23,   -30,   325,   328,   -20,   -89,   -83,  -123,  -125,
  -126,   -84,   -27,   312,   313,   314,   315,   295,   296,   297,
   298,   299,   300,   -83,   -84,   -86,   -88,   -89,    -1,   294,
   301,   -91,   -92,   -93,   -94,   -95,   302,   303,   304,   305,
   306,   325,   -63,   325,  -115,   -11,   324,   -27,   325,   325,
   293,   -19,   324,   -28,   292,   -32,   -11,   324,   -34,   326,
   -25,  -143,   -26,  -140,  -120,   318,   324,   -29,   -30,  -122,
   -10,   -13,  -124,   -10,  -127,  -128,  -129,    -6,   -85,   -87,
    -8,   -31,   318,   -12,   324,   -90,   325,   328,   -88,   -27,
   325,   325,   -28,   325,   325,   -11,   -34,   -33,   325,  -143,
   -26,   -70,  -121,   320,   321,   322,   323,   -28,   -16,   318,
   324,   -19,   325,   325,   -10,   -14,   -10,   324,   324,  -130,
   325,    -2,   324,   -10,   325,   -31,    -2,   325,   -19,   -18,
   324,   -28,    -9,   -31,  -131,  -138,   -34,   327,   316,  -120,
  -133,  -134,  -135,  -136,  -137,   -16,   320,   321,   322,   323,
   325,   -10,   325,   -70,   -70,   325,   -29,   -30,   -70,    -2,
    -2,   -16,   318,   -28,   325,   -31,    -4,    -5,   324,   -11,
  -132,  -130,   325,   325,   -16,   -16,   -16,   -16,  -130,   325,
   -16,   -16,   -16,   -16,   325,   325,   325,   325,   325,   325,
  -139,    -4,   -83,   328,   325,   -29,   325,   -16,   -17,   -16,
   -16,   -16,   325,   -16,   -17,   -16,   -16,   -10,   -10,    -2,
  -132,    -7,   -18,   325,   325,   325,   325,   325,   325,   325,
   325,   325,   325,   325,   325,   325,    -5,   324,   -15,   325 };
static YYCONST yytabelem yydef[]={

     1,    -2,     5,     2,     3,     4,     0,     0,   253,     0,
     0,     0,   254,   255,     0,   285,   286,   287,   288,   289,
   290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
   300,   301,   302,   303,   304,   305,   306,     0,     6,     0,
    11,     0,     0,     8,    10,     0,    12,    14,    15,    16,
    17,    21,    46,    48,     0,     7,     9,     0,     0,    69,
    71,    77,    74,     0,    13,     0,    18,    22,    23,    26,
     0,    19,    27,    28,    31,     0,    20,    32,    33,    36,
     0,    47,     0,     0,     0,    50,    52,    53,    54,    55,
    56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
    66,    67,    68,   240,   240,     0,    82,    89,    24,    37,
    39,     0,    25,    40,    42,     0,    29,    30,    43,    45,
     0,    34,    35,   120,    49,    51,     0,   237,   241,     0,
    73,    76,     0,     0,    80,    83,     0,    92,    38,    41,
    44,   137,    70,   238,   242,   243,    72,    78,   281,   282,
   283,   274,   275,   276,   277,   278,   279,   280,    75,    82,
    84,    85,    87,   284,    96,     0,     0,   137,   123,     0,
   137,   125,   137,   127,   137,   129,   137,   131,   137,   133,
   137,   135,   161,   136,   166,   240,     0,   234,    81,     0,
   234,     0,    96,    94,     0,    95,     0,   234,   121,   122,
     0,   138,   142,     0,     0,     0,     0,     0,   124,     0,
   126,     0,   128,     0,   130,     0,   132,     0,   134,   160,
   165,     0,   239,   244,   245,   246,     0,   272,     0,   231,
   235,   307,    86,   252,   271,     0,    90,    93,    97,   171,
     0,   191,    98,    99,     0,   111,     0,   240,     0,     0,
     0,   154,     0,     0,   270,   269,     0,     0,     0,   273,
    79,   232,   236,    88,     0,   174,     0,   186,   178,     0,
   181,   183,   189,   195,   196,   197,   198,   257,   258,   259,
   260,   261,   262,   107,   101,   103,     0,     0,   113,   256,
   263,   115,   116,   117,   118,   119,   264,   265,   266,   267,
   268,    91,     0,   140,   141,   143,     0,   194,   155,   156,
     0,     0,     0,   222,     0,     0,   164,     0,     0,   170,
     0,   248,   249,   234,   172,   205,     0,   216,   217,     0,
     0,     0,   188,     0,     0,     0,   228,     0,     0,     0,
     0,     0,   110,     0,     0,     0,   139,     0,     0,   192,
   157,   159,   220,   158,   162,   163,     0,     0,   247,   250,
   251,   233,     0,   206,   208,   210,   212,   214,     0,   199,
     0,   204,   176,   177,   185,     0,     0,   234,   234,     0,
   100,   106,   234,     0,   105,     0,   108,   112,     0,     0,
     0,   225,     0,     0,   230,   228,     0,   168,   169,     0,
     0,     0,     0,     0,   228,     0,     0,     0,     0,     0,
   179,   187,   180,     0,     0,   190,   226,   227,     0,     0,
   109,     0,     0,   223,   145,     0,   146,   148,     0,   152,
     0,     0,   167,   173,     0,   219,     0,     0,     0,   175,
     0,   219,     0,     0,     0,     0,     0,   104,   114,   144,
   230,   147,   151,     0,   193,   229,   221,     0,     0,   218,
     0,     0,   215,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   207,   209,   211,   213,   200,   201,   202,
   203,   182,   184,   102,   224,   149,   150,     0,     0,   153 };
typedef struct
#ifdef __cplusplus
	yytoktype
#endif
{
#ifdef __cplusplus
const
#endif
char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"DEFINE",	257,
	"DOMAIN_TOKEN",	258,
	"PROBLEM",	259,
	"REQUIREMENTS",	260,
	"TYPES",	261,
	"CONSTANTS",	262,
	"PREDICATES",	263,
	"FUNCTIONS",	264,
	"STRIPS",	265,
	"TYPING",	266,
	"NEGATIVE_PRECONDITIONS",	267,
	"DISJUNCTIVE_PRECONDITIONS",	268,
	"EQUALITY",	269,
	"EXISTENTIAL_PRECONDITIONS",	270,
	"UNIVERSAL_PRECONDITIONS",	271,
	"QUANTIFIED_PRECONDITIONS",	272,
	"CONDITIONAL_EFFECTS",	273,
	"FLUENTS",	274,
	"ADL",	275,
	"DURATIVE_ACTIONS",	276,
	"DURATION_INEQUALITIES",	277,
	"CONTINUOUS_EFFECTS",	278,
	"PROBABILISTIC_EFFECTS",	279,
	"REWARDS",	280,
	"MDP",	281,
	"ACTION",	282,
	"PARAMETERS",	283,
	"PRECONDITION",	284,
	"EFFECT",	285,
	"PDOMAIN",	286,
	"OBJECTS",	287,
	"INIT",	288,
	"GOAL",	289,
	"GOAL_REWARD",	290,
	"METRIC",	291,
	"EXPECTED_COST",	292,
	"GOAL_PROBABILITY",	293,
	"WHEN",	294,
	"NOT",	295,
	"AND",	296,
	"OR",	297,
	"IMPLY",	298,
	"EXISTS",	299,
	"FORALL",	300,
	"PROBABILISTIC",	301,
	"ASSIGN",	302,
	"SCALE_UP",	303,
	"SCALE_DOWN",	304,
	"INCREASE",	305,
	"DECREASE",	306,
	"MINIMIZE",	307,
	"MAXIMIZE",	308,
	"NUMBER_TOKEN",	309,
	"OBJECT_TOKEN",	310,
	"EITHER",	311,
	"LT",	312,
	"LE",	313,
	"GE",	314,
	"GT",	315,
	"NAME",	316,
	"VARIABLE",	317,
	"NUMBER",	318,
	"ILLEGAL_TOKEN",	319,
	"PLUS",	320,
	"HYPHEN",	321,
	"STAR",	322,
	"SLASH",	323,
	"LEFTPAR",	324,
	"RIGHTPAR",	325,
	"LEFTBRACE",	326,
	"RIGHTBRACE",	327,
	"EQUAL",	328,
	"ORBIT",	329,
	"SYSTEM",	330,
	"-unknown-",	-1	/* ends search */
};

#ifdef __cplusplus
const
#endif
char * yyreds[] =
{
	"-no such reduction-",
	"file : /* empty */",
	"file : domains_and_problems",
	"domains_and_problems : domains_and_problems domain_def",
	"domains_and_problems : domains_and_problems problem_def",
	"domains_and_problems : /* empty */",
	"domain_def : LEFTPAR define LEFTPAR domain name RIGHTPAR",
	"domain_def : LEFTPAR define LEFTPAR domain name RIGHTPAR domain_body RIGHTPAR",
	"domain_body : require_def",
	"domain_body : require_def domain_body2",
	"domain_body : domain_body2",
	"domain_body : /* empty */",
	"domain_body2 : types_def",
	"domain_body2 : types_def domain_body3",
	"domain_body2 : domain_body3",
	"domain_body3 : constants_def",
	"domain_body3 : predicates_def",
	"domain_body3 : functions_def",
	"domain_body3 : constants_def domain_body4",
	"domain_body3 : predicates_def domain_body5",
	"domain_body3 : functions_def domain_body6",
	"domain_body3 : structure_defs",
	"domain_body4 : predicates_def",
	"domain_body4 : functions_def",
	"domain_body4 : predicates_def domain_body7",
	"domain_body4 : functions_def domain_body8",
	"domain_body4 : structure_defs",
	"domain_body5 : constants_def",
	"domain_body5 : functions_def",
	"domain_body5 : constants_def domain_body7",
	"domain_body5 : functions_def domain_body9",
	"domain_body5 : structure_defs",
	"domain_body6 : constants_def",
	"domain_body6 : predicates_def",
	"domain_body6 : constants_def domain_body8",
	"domain_body6 : predicates_def domain_body9",
	"domain_body6 : structure_defs",
	"domain_body7 : functions_def",
	"domain_body7 : functions_def structure_defs",
	"domain_body7 : structure_defs",
	"domain_body8 : predicates_def",
	"domain_body8 : predicates_def structure_defs",
	"domain_body8 : structure_defs",
	"domain_body9 : constants_def",
	"domain_body9 : constants_def structure_defs",
	"domain_body9 : structure_defs",
	"structure_defs : structure_def",
	"structure_defs : structure_defs structure_def",
	"structure_def : action_def",
	"require_def : LEFTPAR REQUIREMENTS require_keys RIGHTPAR",
	"require_keys : require_key",
	"require_keys : require_keys require_key",
	"require_key : STRIPS",
	"require_key : TYPING",
	"require_key : NEGATIVE_PRECONDITIONS",
	"require_key : DISJUNCTIVE_PRECONDITIONS",
	"require_key : EQUALITY",
	"require_key : EXISTENTIAL_PRECONDITIONS",
	"require_key : UNIVERSAL_PRECONDITIONS",
	"require_key : QUANTIFIED_PRECONDITIONS",
	"require_key : CONDITIONAL_EFFECTS",
	"require_key : FLUENTS",
	"require_key : ADL",
	"require_key : DURATIVE_ACTIONS",
	"require_key : DURATION_INEQUALITIES",
	"require_key : CONTINUOUS_EFFECTS",
	"require_key : PROBABILISTIC_EFFECTS",
	"require_key : REWARDS",
	"require_key : MDP",
	"types_def : LEFTPAR TYPES",
	"types_def : LEFTPAR TYPES typed_names RIGHTPAR",
	"constants_def : LEFTPAR CONSTANTS",
	"constants_def : LEFTPAR CONSTANTS typed_names RIGHTPAR",
	"predicates_def : LEFTPAR PREDICATES predicate_decls RIGHTPAR",
	"functions_def : LEFTPAR FUNCTIONS",
	"functions_def : LEFTPAR FUNCTIONS function_decls RIGHTPAR",
	"predicate_decls : predicate_decls predicate_decl",
	"predicate_decls : /* empty */",
	"predicate_decl : LEFTPAR predicate",
	"predicate_decl : LEFTPAR predicate variables RIGHTPAR",
	"function_decls : function_decl_seq",
	"function_decls : function_decl_seq function_type_spec function_decls",
	"function_decls : /* empty */",
	"function_decl_seq : function_decl",
	"function_decl_seq : function_decl_seq function_decl",
	"function_type_spec : HYPHEN",
	"function_type_spec : HYPHEN function_type",
	"function_decl : LEFTPAR function",
	"function_decl : LEFTPAR function variables RIGHTPAR",
	"action_def : LEFTPAR ACTION name",
	"action_def : LEFTPAR ACTION name parameters action_body RIGHTPAR",
	"parameters : PARAMETERS LEFTPAR variables RIGHTPAR",
	"parameters : /* empty */",
	"action_body : precondition action_body2",
	"action_body : action_body2",
	"action_body2 : effect",
	"action_body2 : /* empty */",
	"precondition : PRECONDITION formula",
	"effect : EFFECT eff_formula",
	"eff_formula : p_effect",
	"eff_formula : LEFTPAR and eff_formulas RIGHTPAR",
	"eff_formula : LEFTPAR forall",
	"eff_formula : LEFTPAR forall LEFTPAR variables RIGHTPAR eff_formula RIGHTPAR",
	"eff_formula : LEFTPAR when",
	"eff_formula : LEFTPAR when formula eff_formula RIGHTPAR",
	"eff_formula : LEFTPAR probabilistic prob_effs RIGHTPAR",
	"eff_formulas : eff_formulas eff_formula",
	"eff_formulas : /* empty */",
	"prob_effs : probability eff_formula",
	"prob_effs : prob_effs probability eff_formula",
	"probability : NUMBER",
	"p_effect : atomic_term_formula",
	"p_effect : LEFTPAR not atomic_term_formula RIGHTPAR",
	"p_effect : LEFTPAR assign_op",
	"p_effect : LEFTPAR assign_op f_head f_exp RIGHTPAR",
	"assign_op : assign",
	"assign_op : scale_up",
	"assign_op : scale_down",
	"assign_op : increase",
	"assign_op : decrease",
	"problem_def : LEFTPAR define LEFTPAR problem name RIGHTPAR LEFTPAR PDOMAIN name RIGHTPAR",
	"problem_def : LEFTPAR define LEFTPAR problem name RIGHTPAR LEFTPAR PDOMAIN name RIGHTPAR problem_body RIGHTPAR",
	"problem_body : require_def problem_body2",
	"problem_body : problem_body2",
	"problem_body2 : object_decl problem_body3",
	"problem_body2 : problem_body3",
	"problem_body3 : init problem_body4",
	"problem_body3 : problem_body4",
	"problem_body4 : goal problem_body5",
	"problem_body4 : problem_body5",
	"problem_body5 : goal_reward problem_body6",
	"problem_body5 : problem_body6",
	"problem_body6 : metric_spec problem_body7",
	"problem_body6 : problem_body7",
	"problem_body7 : orbit_decls problem_body8",
	"problem_body7 : problem_body8",
	"problem_body8 : system_decls",
	"problem_body8 : /* empty */",
	"object_decl : LEFTPAR OBJECTS",
	"object_decl : LEFTPAR OBJECTS typed_names RIGHTPAR",
	"init : LEFTPAR INIT init_elements RIGHTPAR",
	"init_elements : init_elements init_element",
	"init_elements : /* empty */",
	"init_element : atomic_name_formula",
	"init_element : LEFTPAR EQUAL ground_f_head NUMBER RIGHTPAR",
	"init_element : LEFTPAR probabilistic prob_inits RIGHTPAR",
	"prob_inits : probability simple_init",
	"prob_inits : prob_inits probability simple_init",
	"simple_init : one_init",
	"simple_init : LEFTPAR and one_inits RIGHTPAR",
	"one_inits : one_inits one_init",
	"one_inits : /* empty */",
	"one_init : atomic_name_formula",
	"one_init : LEFTPAR EQUAL ground_f_head value RIGHTPAR",
	"value : NUMBER",
	"goal : LEFTPAR GOAL formula RIGHTPAR",
	"goal_reward : LEFTPAR GOAL_REWARD value RIGHTPAR",
	"metric_spec : LEFTPAR METRIC maximize GOAL_PROBABILITY RIGHTPAR",
	"metric_spec : LEFTPAR METRIC minimize EXPECTED_COST RIGHTPAR",
	"metric_spec : LEFTPAR METRIC maximize f_head RIGHTPAR",
	"orbit_decls : orbit_decls orbit_decl",
	"orbit_decls : orbit_decl",
	"orbit_decl : LEFTPAR ORBIT NAME atoms RIGHTPAR",
	"atoms : atoms atomic_name_formula",
	"atoms : atomic_name_formula",
	"system_decls : system_decls system_decl",
	"system_decls : system_decl",
	"system_decl : LEFTPAR SYSTEM NAME orbit_set orbit_set orbit_set RIGHTPAR",
	"orbit_set : LEFTBRACE orbits RIGHTBRACE",
	"orbits : orbits NAME",
	"orbits : /* empty */",
	"formula : atomic_term_formula",
	"formula : LEFTPAR EQUAL term_or_f_exp",
	"formula : LEFTPAR EQUAL term_or_f_exp term_or_f_exp RIGHTPAR",
	"formula : LEFTPAR binary_comp",
	"formula : LEFTPAR binary_comp f_exp f_exp RIGHTPAR",
	"formula : LEFTPAR not formula RIGHTPAR",
	"formula : LEFTPAR and conjuncts RIGHTPAR",
	"formula : LEFTPAR or",
	"formula : LEFTPAR or disjuncts RIGHTPAR",
	"formula : LEFTPAR imply formula formula RIGHTPAR",
	"formula : LEFTPAR exists",
	"formula : LEFTPAR exists LEFTPAR variables RIGHTPAR formula RIGHTPAR",
	"formula : LEFTPAR forall",
	"formula : LEFTPAR forall LEFTPAR variables RIGHTPAR formula RIGHTPAR",
	"conjuncts : conjuncts formula",
	"conjuncts : /* empty */",
	"disjuncts : disjuncts formula",
	"disjuncts : /* empty */",
	"atomic_term_formula : LEFTPAR predicate",
	"atomic_term_formula : LEFTPAR predicate terms RIGHTPAR",
	"atomic_term_formula : predicate",
	"atomic_name_formula : LEFTPAR predicate",
	"atomic_name_formula : LEFTPAR predicate names RIGHTPAR",
	"atomic_name_formula : predicate",
	"binary_comp : LT",
	"binary_comp : LE",
	"binary_comp : GE",
	"binary_comp : GT",
	"f_exp : NUMBER",
	"f_exp : LEFTPAR PLUS f_exp f_exp RIGHTPAR",
	"f_exp : LEFTPAR HYPHEN f_exp opt_f_exp RIGHTPAR",
	"f_exp : LEFTPAR STAR f_exp f_exp RIGHTPAR",
	"f_exp : LEFTPAR SLASH f_exp f_exp RIGHTPAR",
	"f_exp : f_head",
	"term_or_f_exp : NUMBER",
	"term_or_f_exp : LEFTPAR PLUS",
	"term_or_f_exp : LEFTPAR PLUS f_exp f_exp RIGHTPAR",
	"term_or_f_exp : LEFTPAR HYPHEN",
	"term_or_f_exp : LEFTPAR HYPHEN f_exp opt_f_exp RIGHTPAR",
	"term_or_f_exp : LEFTPAR STAR",
	"term_or_f_exp : LEFTPAR STAR f_exp f_exp RIGHTPAR",
	"term_or_f_exp : LEFTPAR SLASH",
	"term_or_f_exp : LEFTPAR SLASH f_exp f_exp RIGHTPAR",
	"term_or_f_exp : LEFTPAR function",
	"term_or_f_exp : LEFTPAR function terms RIGHTPAR",
	"term_or_f_exp : name",
	"term_or_f_exp : variable",
	"opt_f_exp : f_exp",
	"opt_f_exp : /* empty */",
	"f_head : LEFTPAR function",
	"f_head : LEFTPAR function terms RIGHTPAR",
	"f_head : function",
	"ground_f_head : LEFTPAR function",
	"ground_f_head : LEFTPAR function names RIGHTPAR",
	"ground_f_head : function",
	"terms : terms name",
	"terms : terms variable",
	"terms : /* empty */",
	"names : names name",
	"names : /* empty */",
	"variables : variable_seq",
	"variables : variable_seq type_spec",
	"variables : variable_seq type_spec variables",
	"variables : /* empty */",
	"variable_seq : variable",
	"variable_seq : variable_seq variable",
	"typed_names : name_seq",
	"typed_names : name_seq type_spec",
	"typed_names : name_seq type_spec typed_names",
	"typed_names : /* empty */",
	"name_seq : name",
	"name_seq : name_seq name",
	"type_spec : HYPHEN",
	"type_spec : HYPHEN type",
	"type : object",
	"type : type_name",
	"type : LEFTPAR either types RIGHTPAR",
	"types : object",
	"types : type_name",
	"types : types object",
	"types : types type_name",
	"function_type : number",
	"define : DEFINE",
	"domain : DOMAIN_TOKEN",
	"problem : PROBLEM",
	"when : WHEN",
	"not : NOT",
	"and : AND",
	"or : OR",
	"imply : IMPLY",
	"exists : EXISTS",
	"forall : FORALL",
	"probabilistic : PROBABILISTIC",
	"assign : ASSIGN",
	"scale_up : SCALE_UP",
	"scale_down : SCALE_DOWN",
	"increase : INCREASE",
	"decrease : DECREASE",
	"minimize : MINIMIZE",
	"maximize : MAXIMIZE",
	"number : NUMBER_TOKEN",
	"object : OBJECT_TOKEN",
	"either : EITHER",
	"type_name : DEFINE",
	"type_name : DOMAIN_TOKEN",
	"type_name : PROBLEM",
	"type_name : EITHER",
	"type_name : MINIMIZE",
	"type_name : MAXIMIZE",
	"type_name : NAME",
	"predicate : type_name",
	"predicate : OBJECT_TOKEN",
	"predicate : NUMBER_TOKEN",
	"function : name",
	"name : DEFINE",
	"name : DOMAIN_TOKEN",
	"name : PROBLEM",
	"name : NUMBER_TOKEN",
	"name : OBJECT_TOKEN",
	"name : EITHER",
	"name : WHEN",
	"name : NOT",
	"name : AND",
	"name : OR",
	"name : IMPLY",
	"name : EXISTS",
	"name : FORALL",
	"name : PROBABILISTIC",
	"name : ASSIGN",
	"name : SCALE_UP",
	"name : SCALE_DOWN",
	"name : INCREASE",
	"name : DECREASE",
	"name : MINIMIZE",
	"name : MAXIMIZE",
	"name : NAME",
	"variable : VARIABLE",
};
#endif /* YYDEBUG */
# line	1 "/usr/ccs/bin/yaccpar"
/*
 * Copyright (c) 1993 by Sun Microsystems, Inc.
 */

#pragma ident	"@(#)yaccpar	6.16	99/01/20 SMI"

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#define YYNEW(type)	malloc(sizeof(type) * yynewmax)
#define YYCOPY(to, from, type) \
	(type *) memcpy(to, (char *) from, yymaxdepth * sizeof (type))
#define YYENLARGE( from, type) \
	(type *) realloc((char *) from, yynewmax * sizeof(type))
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-10000000)

/*
** global variables used by the parser
*/
YYSTYPE *yypv;			/* top of value stack */
int *yyps;			/* top of state stack */

int yystate;			/* current state */
int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */
int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */



#ifdef YYNMBCHARS
#define YYLEX()		yycvtok(yylex())
/*
** yycvtok - return a token if i is a wchar_t value that exceeds 255.
**	If i<255, i itself is the token.  If i>255 but the neither 
**	of the 30th or 31st bit is on, i is already a token.
*/
#if defined(__STDC__) || defined(__cplusplus)
int yycvtok(int i)
#else
int yycvtok(i) int i;
#endif
{
	int first = 0;
	int last = YYNMBCHARS - 1;
	int mid;
	wchar_t j;

	if(i&0x60000000){/*Must convert to a token. */
		if( yymbchars[last].character < i ){
			return i;/*Giving up*/
		}
		while ((last>=first)&&(first>=0)) {/*Binary search loop*/
			mid = (first+last)/2;
			j = yymbchars[mid].character;
			if( j==i ){/*Found*/ 
				return yymbchars[mid].tvalue;
			}else if( j<i ){
				first = mid + 1;
			}else{
				last = mid -1;
			}
		}
		/*No entry in the table.*/
		return i;/* Giving up.*/
	}else{/* i is already a token. */
		return i;
	}
}
#else/*!YYNMBCHARS*/
#define YYLEX()		yylex()
#endif/*!YYNMBCHARS*/

/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
#if defined(__STDC__) || defined(__cplusplus)
int yyparse(void)
#else
int yyparse()
#endif
{
	register YYSTYPE *yypvt = 0;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside
	switch should never be executed
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto yyerrlab;
		case 2: goto yynewstate;
	}
#endif

	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

#if YYMAXDEPTH <= 0
	if (yymaxdepth <= 0)
	{
		if ((yymaxdepth = YYEXPAND(0)) <= 0)
		{
			yyerror("yacc initialization error");
			YYABORT;
		}
	}
#endif

	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */
	goto yystack;	/* moved from 6 lines above to here to please C++ */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ yymaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			long yyps_index = (yy_ps - yys);
			long yypv_index = (yy_pv - yyv);
			long yypvt_index = (yypvt - yyv);
			int yynewmax;
#ifdef YYEXPAND
			yynewmax = YYEXPAND(yymaxdepth);
#else
			yynewmax = 2 * yymaxdepth;	/* double table size */
			if (yymaxdepth == YYMAXDEPTH)	/* first time growth */
			{
				char *newyys = (char *)YYNEW(int);
				char *newyyv = (char *)YYNEW(YYSTYPE);
				if (newyys != 0 && newyyv != 0)
				{
					yys = YYCOPY(newyys, yys, int);
					yyv = YYCOPY(newyyv, yyv, YYSTYPE);
				}
				else
					yynewmax = 0;	/* failed */
			}
			else				/* not first time */
			{
				yys = YYENLARGE(yys, int);
				yyv = YYENLARGE(yyv, YYSTYPE);
				if (yys == 0 || yyv == 0)
					yynewmax = 0;	/* failed */
			}
#endif
			if (yynewmax <= yymaxdepth)	/* tables not expanded */
			{
				yyerror( "yacc stack overflow" );
				YYABORT;
			}
			yymaxdepth = yynewmax;

			yy_ps = yys + yyps_index;
			yy_pv = yyv + yypv_index;
			yypvt = yyv + yypvt_index;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			printf( "Received token " );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				printf( "Received token " );
				if ( yychar == 0 )
					printf( "end-of-file\n" );
				else if ( yychar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register YYCONST int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
			skip_init:
				yynerrs++;
				/* FALLTHRU */
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					printf( "Error recovery discards " );
					if ( yychar == 0 )
						printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
case 1:
# line 221 "parser.y"
{
                          success = true;
			  line_number = 1;
			} break;
case 2:
# line 226 "parser.y"
{
			  if( !success )
			    YYERROR;
			} break;
case 6:
# line 239 "parser.y"
{
			  make_domain( yypvt[-1].str );
			} break;
case 52:
# line 316 "parser.y"
{
			  requirements->strips = true;
			} break;
case 53:
# line 320 "parser.y"
{
			  requirements->typing = true;
			} break;
case 54:
# line 324 "parser.y"
{
			  requirements->negative_preconditions = true;
			} break;
case 55:
# line 328 "parser.y"
{
			  requirements->disjunctive_preconditions = true;
			} break;
case 56:
# line 332 "parser.y"
{
			  requirements->equality = true;
			} break;
case 57:
# line 336 "parser.y"
{
			  requirements->existential_preconditions = true;
			} break;
case 58:
# line 340 "parser.y"
{
			  requirements->universal_preconditions = true;
			} break;
case 59:
# line 344 "parser.y"
{
			  requirements->quantified_preconditions();
			} break;
case 60:
# line 348 "parser.y"
{
			  requirements->conditional_effects = true;
			} break;
case 61:
# line 352 "parser.y"
{
			  requirements->fluents = true;
			} break;
case 62:
# line 356 "parser.y"
{
			  requirements->adl();
			} break;
case 63:
# line 360 "parser.y"
{
			  throw Exception( "`:durative-actions' not supported" );
			} break;
case 64:
# line 364 "parser.y"
{
			  throw Exception( "`:duration-inequalities' not supported" );
			} break;
case 65:
# line 368 "parser.y"
{
			  throw Exception( "`:continuous-effects' not supported" );
			} break;
case 66:
# line 372 "parser.y"
{
			  requirements->probabilistic_effects = true;
			  goal_prob_function =
			    domain->functions().add_function( "goal-probability" );
			} break;
case 67:
# line 378 "parser.y"
{
			  requirements->rewards = true;
			  reward_function = domain->functions().add_function( "reward" );
			} break;
case 68:
# line 383 "parser.y"
{
			  requirements->probabilistic_effects = true;
			  requirements->rewards = true;
			  goal_prob_function =
			    domain->functions().add_function( "goal-probability" );
			  reward_function = domain->functions().add_function( "reward" );
			} break;
case 69:
# line 393 "parser.y"
{
			  require_typing();
			  name_kind = TYPE_KIND;
			} break;
case 70:
# line 398 "parser.y"
{
			  name_kind = VOID_KIND;
			} break;
case 71:
# line 404 "parser.y"
{
			  name_kind = CONSTANT_KIND;
			} break;
case 72:
# line 408 "parser.y"
{
			  name_kind = VOID_KIND;
			} break;
case 74:
# line 417 "parser.y"
{
			  require_fluents();
			} break;
case 78:
# line 428 "parser.y"
{
			  make_predicate( yypvt[-0].str );
			} break;
case 79:
# line 432 "parser.y"
{
			  parsing_predicate = false;
			} break;
case 85:
# line 447 "parser.y"
{
			  require_typing();
			} break;
case 87:
# line 454 "parser.y"
{
			  make_function( yypvt[-0].str );
			} break;
case 88:
# line 458 "parser.y"
{
			  parsing_function = false;
			} break;
case 89:
# line 464 "parser.y"
{
			  make_action( yypvt[-0].str );
			} break;
case 90:
# line 468 "parser.y"
{
			  add_action();
			} break;
case 97:
# line 486 "parser.y"
{
			  action->set_precondition( *yypvt[-0].formula );
			  StateFormula::unregister_use( yypvt[-0].formula );
			} break;
case 98:
# line 493 "parser.y"
{
			  action->set_effect( *yypvt[-0].effect );
			  Effect::unregister_use( yypvt[-0].effect );
			} break;
case 100:
# line 501 "parser.y"
{
			  yyval.effect = yypvt[-1].ceffect;
			} break;
case 101:
# line 505 "parser.y"
{
			  prepare_forall_effect();
			} break;
case 102:
# line 509 "parser.y"
{
			  yyval.effect = make_forall_effect( *yypvt[-1].effect );
			} break;
case 103:
# line 513 "parser.y"
{
			  require_conditional_effects();
			} break;
case 104:
# line 517 "parser.y"
{
			  yyval.effect = &ConditionalEffect::make( *yypvt[-2].formula, *yypvt[-1].effect );
			} break;
case 105:
# line 521 "parser.y"
{
			  yyval.effect = yypvt[-1].peffect;
			} break;
case 106:
# line 527 "parser.y"
{
			  yyval.ceffect = yypvt[-1].ceffect;
			  yyval.ceffect->add_conjunct( *yypvt[-0].effect );
			} break;
case 107:
# line 532 "parser.y"
{
			  yyval.ceffect = new ConjunctiveEffect;
			} break;
case 108:
# line 538 "parser.y"
{
			  yyval.peffect = new ProbabilisticEffect;
			  add_effect_outcome( *yyval.peffect, yypvt[-1].num, *yypvt[-0].effect );
			} break;
case 109:
# line 543 "parser.y"
{
			  yyval.peffect = yypvt[-2].peffect;
			  add_effect_outcome( *yyval.peffect, yypvt[-1].num, *yypvt[-0].effect );
			} break;
case 111:
# line 553 "parser.y"
{
			  yyval.effect = make_add_effect( *yypvt[-0].atom );
			} break;
case 112:
# line 557 "parser.y"
{
			  yyval.effect = make_delete_effect( *yypvt[-1].atom );
			} break;
case 113:
# line 561 "parser.y"
{
			  effect_fluent = true;
			} break;
case 114:
# line 565 "parser.y"
{
			  yyval.effect = make_assignment_effect( yypvt[-4].setop, *yypvt[-2].appl, *yypvt[-1].expr );
			} break;
case 115:
# line 571 "parser.y"
{
			  yyval.setop = Assignment::ASSIGN_OP;
			} break;
case 116:
# line 575 "parser.y"
{
			  yyval.setop = Assignment::SCALE_UP_OP;
			} break;
case 117:
# line 579 "parser.y"
{
			  yyval.setop = Assignment::SCALE_DOWN_OP;
			} break;
case 118:
# line 583 "parser.y"
{
			  yyval.setop = Assignment::INCREASE_OP;
			} break;
case 119:
# line 587 "parser.y"
{
			  yyval.setop = Assignment::DECREASE_OP;
			} break;
case 120:
# line 594 "parser.y"
{
			  make_problem( yypvt[-5].str, yypvt[-1].str );
			} break;
case 121:
# line 598 "parser.y"
{
			  delete requirements;
			} break;
case 138:
# line 636 "parser.y"
{
			  name_kind = OBJECT_KIND;
			} break;
case 139:
# line 640 "parser.y"
{
			  name_kind = VOID_KIND;
			} break;
case 143:
# line 653 "parser.y"
{
			  problem->add_init_atom( *yypvt[-0].atom );
			  StateFormula::unregister_use( yypvt[-0].atom );
			} break;
case 144:
# line 658 "parser.y"
{
			  problem->add_init_fluent( *yypvt[-2].appl, *yypvt[-1].num );
			  delete yypvt[-1].num;
			} break;
case 145:
# line 663 "parser.y"
{
			  problem->add_init_effect( *yypvt[-1].peffect );
			  Effect::unregister_use( yypvt[-1].peffect );
			} break;
case 146:
# line 670 "parser.y"
{
			  yyval.peffect = new ProbabilisticEffect;
			  add_effect_outcome( *yyval.peffect, yypvt[-1].num, *yypvt[-0].effect );
			} break;
case 147:
# line 675 "parser.y"
{
			  yyval.peffect = yypvt[-2].peffect;
			  add_effect_outcome( *yyval.peffect, yypvt[-1].num, *yypvt[-0].effect );
			} break;
case 149:
# line 683 "parser.y"
{
			  yyval.effect = yypvt[-1].ceffect;
			} break;
case 150:
# line 689 "parser.y"
{
			  yyval.ceffect = yypvt[-1].ceffect;
			  yyval.ceffect->add_conjunct( *yypvt[-0].effect );
			} break;
case 151:
# line 694 "parser.y"
{
			  yyval.ceffect = new ConjunctiveEffect;
			} break;
case 152:
# line 700 "parser.y"
{
			  yyval.effect = make_add_effect( *yypvt[-0].atom );
			} break;
case 153:
# line 704 "parser.y"
{
			  yyval.effect = make_assignment_effect( Assignment::ASSIGN_OP, *yypvt[-2].appl, *yypvt[-1].expr );
			} break;
case 154:
# line 710 "parser.y"
{
			  yyval.expr = new Value( *yypvt[-0].num );
			  delete yypvt[-0].num;
			} break;
case 155:
# line 717 "parser.y"
{
			  problem->set_goal( *yypvt[-1].formula );
			  StateFormula::unregister_use( yypvt[-1].formula );
			} break;
case 157:
# line 727 "parser.y"
{
			  /* don't care about rewards (blai) */
			  /* problem->set_metric( problem_t::MAXIMIZE_GOAL_PROBABILITY ); */
			} break;
case 158:
# line 732 "parser.y"
{
			  /* don't care about rewards (blai) */
			  /* problem->set_metric( problem_t::MINIMIZE_EXPECTED_COST ); */
			} break;
case 162:
# line 744 "parser.y"
{
			  problem->add_orbit( yypvt[-2].str, yypvt[-1].atoms );
			  delete yypvt[-2].str;
			} break;
case 163:
# line 751 "parser.y"
{
			  yypvt[-1].atoms->push_back( yypvt[-0].atom );
			  yyval.atoms = yypvt[-1].atoms;
			} break;
case 164:
# line 756 "parser.y"
{
			  yyval.atoms = new std::vector<const Atom*>;
			  yyval.atoms->push_back( yypvt[-0].atom );
			} break;
case 167:
# line 767 "parser.y"
{
			  problem->add_system( yypvt[-4].str, yypvt[-3].strs, yypvt[-2].strs, yypvt[-1].strs );
			} break;
case 168:
# line 773 "parser.y"
{
			  yyval.strs = yypvt[-1].strs;
			} break;
case 169:
# line 779 "parser.y"
{
			  yypvt[-1].strs->push_back( yypvt[-0].str );
			  yyval.strs = yypvt[-1].strs;
			} break;
case 170:
# line 784 "parser.y"
{
			  yyval.strs = new std::vector<const std::string*>;
			} break;
case 171:
# line 790 "parser.y"
{
			  yyval.formula = yypvt[-0].atom;
			} break;
case 172:
# line 794 "parser.y"
{
			  first_eq_term = eq_term;
			  first_eq_expr = eq_expr;
			} break;
case 173:
# line 799 "parser.y"
{
			  yyval.formula = make_equality();
			} break;
case 174:
# line 803 "parser.y"
{
			  require_fluents();
			} break;
case 175:
# line 807 "parser.y"
{
			  yyval.formula = new Comparison( yypvt[-4].comp, *yypvt[-2].expr, *yypvt[-1].expr );
			  Expression::unregister_use( yypvt[-2].expr );
			  Expression::unregister_use( yypvt[-1].expr );
			} break;
case 176:
# line 813 "parser.y"
{
			  yyval.formula = make_negation( *yypvt[-1].formula );
			} break;
case 177:
# line 817 "parser.y"
{
			  yyval.formula = yypvt[-1].conj;
			} break;
case 178:
# line 821 "parser.y"
{
			  require_disjunction();
			} break;
case 179:
# line 825 "parser.y"
{
			  yyval.formula = yypvt[-1].disj;
			} break;
case 180:
# line 829 "parser.y"
{
			  yyval.formula = make_implication( *yypvt[-2].formula, *yypvt[-1].formula );
			} break;
case 181:
# line 833 "parser.y"
{
			  prepare_exists();
			} break;
case 182:
# line 837 "parser.y"
{
			  yyval.formula = make_exists( *yypvt[-1].formula );
			} break;
case 183:
# line 841 "parser.y"
{
			  prepare_forall();
			} break;
case 184:
# line 845 "parser.y"
{
			  yyval.formula = make_forall( *yypvt[-1].formula );
			} break;
case 185:
# line 851 "parser.y"
{
			  yyval.conj->add_conjunct( *yypvt[-0].formula );
			} break;
case 186:
# line 855 "parser.y"
{
			  yyval.conj = new Conjunction;
			} break;
case 187:
# line 861 "parser.y"
{
			  yyval.disj->add_disjunct( *yypvt[-0].formula );
			} break;
case 188:
# line 865 "parser.y"
{
			  yyval.disj = new Disjunction;
			} break;
case 189:
# line 871 "parser.y"
{
			  prepare_atom( yypvt[-0].str );
			} break;
case 190:
# line 875 "parser.y"
{
			  yyval.atom = make_atom();
			} break;
case 191:
# line 879 "parser.y"
{
			  prepare_atom( yypvt[-0].str );
			  yyval.atom = make_atom();
			} break;
case 192:
# line 886 "parser.y"
{
			  prepare_atom( yypvt[-0].str );
			} break;
case 193:
# line 890 "parser.y"
{
			  yyval.atom = make_atom();
			} break;
case 194:
# line 894 "parser.y"
{
			  prepare_atom( yypvt[-0].str );
			  yyval.atom = make_atom();
			} break;
case 195:
# line 900 "parser.y"
{ yyval.comp = Comparison::LT_CMP; } break;
case 196:
# line 901 "parser.y"
{ yyval.comp = Comparison::LE_CMP; } break;
case 197:
# line 902 "parser.y"
{ yyval.comp = Comparison::GE_CMP; } break;
case 198:
# line 903 "parser.y"
{ yyval.comp = Comparison::GT_CMP; } break;
case 199:
# line 907 "parser.y"
{
			  yyval.expr = new Value( *yypvt[-0].num );
			  delete yypvt[-0].num;
			} break;
case 200:
# line 912 "parser.y"
{
			  yyval.expr = new Addition( *yypvt[-2].expr, *yypvt[-1].expr );
			  Expression::unregister_use( yypvt[-2].expr );
			  Expression::unregister_use( yypvt[-1].expr );
			} break;
case 201:
# line 918 "parser.y"
{
			  yyval.expr = make_subtraction( *yypvt[-2].expr, yypvt[-1].expr );
			  Expression::unregister_use( yypvt[-2].expr );
			  Expression::unregister_use( yypvt[-1].expr );
			} break;
case 202:
# line 924 "parser.y"
{
			  yyval.expr = new Multiplication( *yypvt[-2].expr, *yypvt[-1].expr );
			  Expression::unregister_use( yypvt[-2].expr );
			  Expression::unregister_use( yypvt[-1].expr );
			} break;
case 203:
# line 930 "parser.y"
{
			  yyval.expr = new Division( *yypvt[-2].expr, *yypvt[-1].expr );
			  Expression::unregister_use( yypvt[-2].expr );
			  Expression::unregister_use( yypvt[-1].expr );
			} break;
case 204:
# line 936 "parser.y"
{
			  yyval.expr = yypvt[-0].appl;
			} break;
case 205:
# line 942 "parser.y"
{
			  require_fluents();
			  eq_expr = new Value( *yypvt[-0].num );
			  delete yypvt[-0].num;
			} break;
case 206:
# line 948 "parser.y"
{
			  require_fluents();
			} break;
case 207:
# line 952 "parser.y"
{
			  eq_expr = new Addition( *yypvt[-2].expr, *yypvt[-1].expr );
			  Expression::unregister_use( yypvt[-2].expr );
			  Expression::unregister_use( yypvt[-1].expr );
			} break;
case 208:
# line 958 "parser.y"
{
			  require_fluents();
			} break;
case 209:
# line 962 "parser.y"
{
			  eq_expr = make_subtraction( *yypvt[-2].expr, yypvt[-1].expr );
			} break;
case 210:
# line 966 "parser.y"
{
			  require_fluents();
			} break;
case 211:
# line 970 "parser.y"
{
			  eq_expr = new Multiplication( *yypvt[-2].expr, *yypvt[-1].expr );
			  Expression::unregister_use( yypvt[-2].expr );
			  Expression::unregister_use( yypvt[-1].expr );
			} break;
case 212:
# line 976 "parser.y"
{
			  require_fluents();
			} break;
case 213:
# line 980 "parser.y"
{
			  eq_expr = new Division( *yypvt[-2].expr, *yypvt[-1].expr );
			  Expression::unregister_use( yypvt[-2].expr );
			  Expression::unregister_use( yypvt[-1].expr );
			} break;
case 214:
# line 986 "parser.y"
{
			  require_fluents();
			  prepare_application(yypvt[-0].str);
			} break;
case 215:
# line 991 "parser.y"
{
			  eq_expr = make_application();
			} break;
case 216:
# line 995 "parser.y"
{
			  make_eq_name( yypvt[-0].str );
			} break;
case 217:
# line 999 "parser.y"
{
			  eq_term = make_term( yypvt[-0].str );
			  eq_expr = NULL;
			} break;
case 219:
# line 1007 "parser.y"
{
			  yyval.expr = NULL;
			} break;
case 220:
# line 1013 "parser.y"
{
			  prepare_application( yypvt[-0].str );
			} break;
case 221:
# line 1017 "parser.y"
{
			  yyval.appl = make_application();
			} break;
case 222:
# line 1021 "parser.y"
{
			  prepare_application( yypvt[-0].str );
			  yyval.appl = make_application();
			} break;
case 223:
# line 1028 "parser.y"
{
			  prepare_application( yypvt[-0].str );
			} break;
case 224:
# line 1032 "parser.y"
{
			  yyval.appl = make_application();
			} break;
case 225:
# line 1036 "parser.y"
{
			  prepare_application( yypvt[-0].str );
			  yyval.appl = make_application();
			} break;
case 226:
# line 1043 "parser.y"
{
			  add_term( yypvt[-0].str );
			} break;
case 227:
# line 1047 "parser.y"
{
			  add_term( yypvt[-0].str );
			} break;
case 229:
# line 1054 "parser.y"
{
			  add_term( yypvt[-0].str );
			} break;
case 231:
# line 1061 "parser.y"
{
			  add_variables( yypvt[-0].strs, OBJECT_TYPE );
			} break;
case 232:
# line 1065 "parser.y"
{
			  add_variables( yypvt[-1].strs, yypvt[-0].type );
			} break;
case 235:
# line 1073 "parser.y"
{
			  yyval.strs = new std::vector<const std::string*>( 1, yypvt[-0].str );
			} break;
case 236:
# line 1077 "parser.y"
{
			  yyval.strs = yypvt[-1].strs;
			  yyval.strs->push_back( yypvt[-0].str );
			} break;
case 237:
# line 1084 "parser.y"
{
			  add_names( yypvt[-0].strs, OBJECT_TYPE );
			} break;
case 238:
# line 1088 "parser.y"
{
			  add_names( yypvt[-1].strs, yypvt[-0].type );
			} break;
case 241:
# line 1096 "parser.y"
{
			  yyval.strs = new std::vector<const std::string*>( 1, yypvt[-0].str );
			} break;
case 242:
# line 1100 "parser.y"
{
			  yyval.strs = yypvt[-1].strs;
			  yyval.strs->push_back( yypvt[-0].str );
			} break;
case 243:
# line 1107 "parser.y"
{
			  require_typing();
			} break;
case 244:
# line 1111 "parser.y"
{
			 yyval.type = yypvt[-0].type;
		       } break;
case 245:
# line 1117 "parser.y"
{
			  yyval.type = OBJECT_TYPE;
			} break;
case 246:
# line 1121 "parser.y"
{
			  yyval.type = make_type( yypvt[-0].str );
			} break;
case 247:
# line 1125 "parser.y"
{
			  yyval.type = make_type( *yypvt[-1].types );
			} break;
case 248:
# line 1131 "parser.y"
{
			  yyval.types = new TypeSet;
			} break;
case 249:
# line 1135 "parser.y"
{
			  yyval.types = new TypeSet;
			  yyval.types->insert( make_type( yypvt[-0].str ) );
			} break;
case 250:
# line 1140 "parser.y"
{
			  yyval.types = yypvt[-1].types;
			} break;
case 251:
# line 1144 "parser.y"
{
			  yyval.types = yypvt[-1].types;
			  yyval.types->insert( make_type( yypvt[-0].str ) );
			} break;
case 253:
# line 1153 "parser.y"
{ delete yypvt[-0].str; } break;
case 254:
# line 1156 "parser.y"
{ delete yypvt[-0].str; } break;
case 255:
# line 1159 "parser.y"
{ delete yypvt[-0].str; } break;
case 256:
# line 1162 "parser.y"
{ delete yypvt[-0].str; } break;
case 257:
# line 1165 "parser.y"
{ delete yypvt[-0].str; } break;
case 258:
# line 1168 "parser.y"
{ delete yypvt[-0].str; } break;
case 259:
# line 1171 "parser.y"
{ delete yypvt[-0].str; } break;
case 260:
# line 1174 "parser.y"
{ delete yypvt[-0].str; } break;
case 261:
# line 1177 "parser.y"
{ delete yypvt[-0].str; } break;
case 262:
# line 1180 "parser.y"
{ delete yypvt[-0].str; } break;
case 263:
# line 1183 "parser.y"
{ delete yypvt[-0].str; } break;
case 264:
# line 1186 "parser.y"
{ delete yypvt[-0].str; } break;
case 265:
# line 1189 "parser.y"
{ delete yypvt[-0].str; } break;
case 266:
# line 1192 "parser.y"
{ delete yypvt[-0].str; } break;
case 267:
# line 1195 "parser.y"
{ delete yypvt[-0].str; } break;
case 268:
# line 1198 "parser.y"
{ delete yypvt[-0].str; } break;
case 269:
# line 1201 "parser.y"
{ delete yypvt[-0].str; } break;
case 270:
# line 1204 "parser.y"
{ delete yypvt[-0].str; } break;
case 271:
# line 1207 "parser.y"
{ delete yypvt[-0].str; } break;
case 272:
# line 1210 "parser.y"
{ delete yypvt[-0].str; } break;
case 273:
# line 1213 "parser.y"
{ delete yypvt[-0].str; } break;
# line	531 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}


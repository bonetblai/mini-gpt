
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
extern YYSTYPE yylval;
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

/*
 * TXTPAR: grab parameters from a text file
 *
 *     27-dec-2021    drafted
 *
 *
 */

/**************** INCLUDE FILES ********************************/ 


#include <stdinc.h>
#include <getparam.h>
#include <table.h>
#include <extstring.h>
#include <ctype.h>

/**************** COMMAND LINE PARAMETERS **********************/

string defv[] = {                /* DEFAULT INPUT PARAMETERS */
    "in=???\n           input file name(s)",
    "expr=\n            formula for parameters after extraction",
    "format=%g\n        format for new output values",
    "seed=0\n           Initial random number",
    "maxline=10000\n    Max number of lines in case a pipe was used",
    "newline=f\n        add newline between output parameters",
    "p#=\n              The word,row,col tuples for given parameter",
    "VERSION=0.2\n      27-dec-2021 PJT",
    NULL
};

string usage = "Extract numeric parameters from a text file";



/**************** GLOBAL VARIABLES *******************************/

string input;			        /* file names */
stream instr;			        /* file streams */

string fmt;                             /* format of new column */

#define MAXCOL          256             /* MAXIMUM number of columns */
#define MLINELEN       8196		/* linelength of catenated */
#define MNEWDAT          80		/* space needed for one number */

#define MAXPAR          100

bool   keepc[MAXCOL+1];                 /* columns to keep (t/f) */
int    ndelc;                           /* actual number of skip columns */

string *fies, selfie;                   /* fie pointers */
int    nfies;                           /* number of fie pointers */
bool   Qfie;				/* boolean if multiple fie's loaded */
bool   Qnewline;                        /* boolean if newline is needed */
bool   Qexpr;

string *lines;
int    nlines, maxline;

string word[MAXPAR];
int    row[MAXPAR];
int    col[MAXPAR];

int    maxpar;

local void setparams(void);
local void convert(stream);
local string *burstfie(string);
local void tab2space(char *);

extern  string *burststring(string, string);
extern  int inifie(string);
extern void dofie(real *, int *, real *, real *);
extern void dmpfie(void);
extern int savefie(int);
extern int loadfie(int);
extern int nemo_file_lines(string, int);


void nemo_main(void)
{
    int i;

    setparams();

    convert (instr);

}

local void setparams(void)
{
    string newcol;                          /* formula for new column */
    string delcol;                          /* which columns not to write */
    int    delc[MAXCOL];                    /* columns to skip for output */
    int    wrc[3];                          /* hold "word","row","col" - temp */
    int i, nwrc;

    input  = getparam("in");
    instr = stropen(input,"r");
    maxline = nemo_file_lines(input, getiparam("maxline"));
    dprintf(1,"Allocated %d lines for file\n",maxline);
    lines = (string *) allocate(maxline * sizeof(string));

    Qexpr = hasvalue("expr");
    newcol = getparam("expr");
    fmt = getparam("format");

    // check all indexed parameters
    maxpar = indexparam("p",-1);
    if (maxpar < 0) error("No parameter directives (p0=, p1, ...) given");
    int ispar;
    for (i=0; i<maxpar; i++) {
      ispar = indexparam("p",i);
      if (ispar) {
	dprintf(0,"%d : %s\n", i, getparam_idx("p",i));
	// just by line, to get us going
	word[i] = "";
	nwrc = nemoinpi(getparam_idx("p",i), wrc, 3);
	if (nwrc < 0) error("parsing p%d=%s", i, getparam_idx("p",i));
	row[i] = wrc[1];
	col[i] = wrc[2];
      } else {
	if (i==0) warning("p0 is missing");
	word[i] = NULL;
      }
    }

    fies = burstfie(newcol);
    nfies = xstrlen(fies,sizeof(string)) - 1;
    if(nfies)dprintf(1,"%d functions to parse\n",nfies);
    for (i=0; i<nfies; i++) {
	dprintf(1,"Saving: %s\n",fies[i]);
        inifie(fies[i]);
        if (savefie(i+1) < 0) error("Could not save fie[%d]: %s\n",i,fies[i]);
	if(nemo_debug(1)) dmpfie();
    }
    Qfie = nfies > 1;

    init_xrandom(getparam("seed"));

    Qnewline = getbparam("newline");

}



local void convert(stream instr)
{
    char   line[MLINELEN];          /* input linelength */
    real   dval[MAXCOL];            /* number of items (values on line) */
    string sval[MAXCOL];            // 
    real   retval;
    char   newdat[MNEWDAT];         /* to store new column in ascii */
    int    nval, i, one=1;
    string *outv;                   /* pointer to vector of strings to write */
    char   *cp, *seps=", \t";       /* column separators  */
    real   errval=0.0;


    /*
     *   read input lines
     */

    nlines=0;                              /* counter for lines read */
    for(;;) {                              /* loop over all lines in file(s) */
      if (get_line(instr, line) < 0)           
	break; 					      /* EOF */
      dprintf(3,"LINE: (%s)\n",line);

      tab2space(line);	          /* work around a Gipsy (?) problem */
      lines[nlines] = strdup(line);
      nlines++;
      if (nlines == maxline) {
	warning("maxline=%d exhausted", maxline);
	break;
      }
    }
    dprintf(0,"Read %d lines\n",nlines);

    /*
     *   extract parameters
     */

    nval = 0;   // this will fill the dval[]
    dprintf(0,"Parsing %d parameters p#=\n", maxpar);
    for (i=0; i<maxpar; i++) {
      if (word[i] == NULL) continue;
      // no word selection yet, this is version 0.1
      // no backwards counting rows yet, this is version 0.1
      dprintf(0,"p%d row=%d col=%d\n", i, row[i], col[i]);
      cp = lines[row[i]-1];
      dprintf(0,"LINE: %s\n", cp);
      // cach the outv, this is still version 0.1
      outv = burststring(cp, seps);
      sval[nval] = outv[col[i]-1];
      nval++;
    }
    if (nval == 0) warning("no output??? should never happen");


    /*
     *  if no expressions give, output parameters "as is" from their textual input
     */

    if (!Qexpr) {
      // simple output of input parameters
      for (i=0; i<nval; i++) {
	if (i>0 && Qnewline) printf("\n");
	printf("%s ", sval[i]);
      }
      printf("\n");
      return;
    }

    /*
     *   convert strings to float values
     */
    
    for (i=0; i<nval; i++) {
      dval[i] = atof(sval[i]);
    }

    /*
     *   apply math and output the new expressions
     */

    line[0] = '\0';
    for(i=0; i<nfies; i++) {
      if (Qfie) loadfie(i+1);
      dofie(dval,&one,&dval[nval+i],&errval);
      dprintf(3," dofie(%d) -> %g  %g\n",i+1,dval[nval+i], errval); //BUG
      strcat(line," ");
      printf(fmt,dval[nval+i]);
      printf(" ");
      if (Qnewline)
	printf("\n");
    }
    if (!Qnewline)
      printf("\n");
    
}

/* burstfie(): to be placed with burststring() later on...
 *
 *	18-feb-92	written		PJT
 */

#define MWRD  256	/* max words in list */
#define MSTR  256	/* max chars per word */

local string *burstfie(string lst)
{
    string wrdbuf[MWRD], *wp;
    char strbuf[MSTR], *sp, *lp;
    int level=0;

    wp = wrdbuf;
    sp = strbuf;
    lp = lst;
    do {						/* scan over list */
	if (*lp == 0 || (*lp == ',' && level == 0)){	/*   is this a sep? */
	    if (sp > strbuf) {				/*     got a word? */
		*sp = 0;
		*wp++ = (string) copxstr(strbuf, sizeof(char));
		if (wp == &wrdbuf[MWRD])		/*       no room? */
		    error("burststring: too many words");
		sp = strbuf;				/*       for next 1 */
	    }
	} else {					/*   part of word */
            if (*lp == '(') level++;
            if (*lp == ')') level--;
	    *sp++ = *lp;				/*     so copy it */
	    if (sp == &strbuf[MSTR])			/*     no room? */
		error("burststring: word too long");
	}
    } while (*lp++ != 0);				/* until list ends */
    *wp = 0;
    return ((string *) copxstr(wrdbuf, sizeof(string)));	/*PPAP*/
}

/*
 * small helper function, replaces tabs by spaces before processing.
 * this prevents me from diving into gipsy parsing routines and fix
 * the problem there 
 * PJT - June 1998.
 */

local void tab2space(char *cp)
{
    while (*cp) {
        if (*cp == '\t') *cp = ' ';
        cp++;
    }
}
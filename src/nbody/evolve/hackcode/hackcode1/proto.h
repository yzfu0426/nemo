/* code.c */
void startrun(void);
void make_testdata(bool cencon);
void stepsystem(void);

/* code_io.c */
int inputdata(string file);
int initoutput(void);
int stopoutput(void);
int output(void);
int savestate(string file);
int restorestate(string file);

/* grav.c */
void hackgrav(bodyptr p);
void hackwalk(proc sub);

/* hackforce.c */
int  input_data(void);
int  read_snapshot(bodyptr *btab_ptr, int *nobj_ptr, stream instr);
void force_calc(void);
void out_result(void);
void write_snapshot(void);

/* load.c */
int maketree(bodyptr btab, int nbody);

/* util.c */
int pickvec(vector x, int cf);

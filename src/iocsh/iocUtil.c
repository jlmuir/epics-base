/* iocUtil.c */
/* Author:  W. Eric Norum Date: 02MAY2000 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <logClient.h>

#include "osiUnistd.h"
#include "epicsThread.h"

#define epicsExportSharedSymbols
#include "ioccrf.h"
#include "iocUtilRegister.h"

/* < (runScript) command */
static const ioccrfArg runScriptArg0 = { "command file name",ioccrfArgString};
static const ioccrfArg * const runScriptArgs[1] = {&runScriptArg0};
static const ioccrfFuncDef runScriptFuncDef = {"<",1,runScriptArgs};
static void runScriptCallFunc(const ioccrfArgBuf *args)
{
    ioccrf (args[0].sval);
}

/* chdir */
static const ioccrfArg chdirArg0 = { "current directory name",ioccrfArgString};
static const ioccrfArg * const chdirArgs[1] = {&chdirArg0};
static const ioccrfFuncDef chdirFuncDef = {"cd",1,chdirArgs};
static void chdirCallFunc(const ioccrfArgBuf *args)
{
    int status;
    status = chdir(args[0].sval);
    if (status) {
        printf ("Invalid directory path ignored\n");
    }
}

/* print current working directory */
static const ioccrfFuncDef pwdFuncDef = { "pwd", 0, 0 };
static void pwdCallFunc (const ioccrfArgBuf *args)
{
    char buf[256];
    char *pwd = getcwd ( buf, sizeof(buf) - 1 );
    if ( pwd ) {
        printf ( "%s\n", pwd );
    }
}

/* show (thread information) */
static const ioccrfArg showArg0 = { "task",ioccrfArgString};
static const ioccrfArg showArg1 = { "task",ioccrfArgString};
static const ioccrfArg showArg2 = { "task",ioccrfArgString};
static const ioccrfArg showArg3 = { "task",ioccrfArgString};
static const ioccrfArg showArg4 = { "task",ioccrfArgString};
static const ioccrfArg showArg5 = { "task",ioccrfArgString};
static const ioccrfArg showArg6 = { "task",ioccrfArgString};
static const ioccrfArg showArg7 = { "task",ioccrfArgString};
static const ioccrfArg showArg8 = { "task",ioccrfArgString};
static const ioccrfArg showArg9 = { "task",ioccrfArgString};
static const ioccrfArg * const showArgs[10] = {
    &showArg0,&showArg1,&showArg2,&showArg3,&showArg4,
    &showArg5,&showArg6,&showArg7,&showArg8,&showArg9,
};
static const ioccrfFuncDef showFuncDef = {"show",10,showArgs};
static void showCallFunc(const ioccrfArgBuf *args)
{
    int i = 0;
    int first = 1;
    int level = 0;
    char *cp;
    epicsThreadId tid;
    unsigned long ltmp;
    char *endp;

    if (((cp = args[i].sval) != NULL)
     && (*cp == '-')) {
        level = atoi (cp + 1);
        i++;
    }
    if ((cp = args[i].sval) == NULL) {
        epicsThreadShowAll (level);
        return;
    }
    for ( ; i < 10 ; i++) {
        if ((cp = args[i].sval) == NULL)
            return;
        ltmp = strtoul (cp, &endp, 16);
        if (*endp) {
            tid = epicsThreadGetId (cp);
            if (!tid) {
                printf ("*** argument %d (%s) is not a valid task name ***\n", i+1, cp);
                continue;
            }
        }
        else {
            tid = (epicsThreadId)ltmp;
        }
        if (first) {
            epicsThreadShow (0, level);
            first = 0;
        }
        epicsThreadShow (tid, level);
    }
}

/* putenv */
static const ioccrfArg putenvArg0 = { "environment_variable=name",ioccrfArgString};
static const ioccrfArg * const putenvArgs[1] = {&putenvArg0};
static const ioccrfFuncDef putenvFuncDef = {"putenv",1,putenvArgs};
static void putenvCallFunc(const ioccrfArgBuf *args)
{
    char *arg0 = args[0].sval;
    char *cp;

    /*
     * Some versions of putenv set the environment to 
     * point to the string that is passed so we have
     * to make a copy before stashing it.
     * Yes, this will cause memory leaks if the same variable is
     * placed in the environment more than once.
     */
    if (arg0) return;
    cp = calloc(strlen(arg0)+1,sizeof(char));
    strcpy(cp,arg0);
    if ((cp == NULL) || putenv (cp)) {
        free (cp);
        printf ("putenv(%s) failed.\n", args[0].sval);
    }
}

/* iocLogInit */
static const ioccrfFuncDef iocLogInitFuncDef = {"iocLogInit",0};
static void iocLogInitCallFunc(const ioccrfArgBuf *args)
{
    iocLogInit ();
}

void epicsShareAPI iocUtilRegister(void)
{
    ioccrfRegister(&runScriptFuncDef,runScriptCallFunc);
    ioccrfRegister(&chdirFuncDef,chdirCallFunc);
    ioccrfRegister(&pwdFuncDef,pwdCallFunc);
    ioccrfRegister(&showFuncDef,showCallFunc);
    ioccrfRegister(&putenvFuncDef,putenvCallFunc);
    ioccrfRegister(&iocLogInitFuncDef,iocLogInitCallFunc);
}

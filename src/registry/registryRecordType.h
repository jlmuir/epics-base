#ifndef INCregistryRecordTypeh
#define INCregistryRecordTypeh

struct dbRecordType;
struct rset;
struct dbBase;

typedef int (*computeSizeOffset)(struct dbRecordType *pdbRecordType);


typedef struct recordTypeLocation {
    struct rset *prset;
    computeSizeOffset sizeOffset;
}recordTypeLocation;
    

#ifdef __cplusplus
extern "C" {
#endif

int registryRecordTypeAdd(const char *name,recordTypeLocation *prtl);
recordTypeLocation *registryRecordTypeFind(const char *name);

/* The following function is generated by registerRecordDeviceDriver/pl */
int registerRecordDeviceDriver(struct dbBase *pdbbase);

#ifdef __cplusplus
}
#endif


#endif /* INCregistryRecordTypeh */

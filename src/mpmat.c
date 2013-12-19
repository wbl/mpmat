#include<stdlib.h>
#include<mpc.h>
#include "mpmat.h"
typedef struct {
  mpfr_prec_t prec;
  long long rows;
  long long cols;
  mpc_t *space;
} mpmat_t;

mpmat mpmat_init(long long r, long long c){
  mpmat_t *res=malloc(sizeof(mpmat_t));
  res->prec=256;
  res->rows=r;
  res->cols=c;
  res->space=malloc(sizeof(mpc_t)*r*c);
  for(long long i=0; i<r*c; i++){
    mpc_init2(res->space[i], res->prec);
  }
  return (void *) res;
}

void mpmat_free(mpmat m){
  mpmat_t *rm = (mpmat_t *) m;
  if(rm->space){
    for(int i=0; i<(rm->rows)*(rm->cols); i++){
      mpc_clear(rm->space[i]);
    }
    free(rm->space);
  }
  free(rm);
}

int mpmat_get(mpc_t v, mpmat m, long long r, long long c){
  mpmat_t *p = (mpmat_t *) m;
  if(p->rows >= r && p->cols >= c){ //one-based indexing
    mpc_set(v, p->space[p->rows*(c-1)+r-1], MPC_RNDNN); //column-major
    return 0;
  }else {
    return -1;
  }
}

int mpmat_set(mpmat m, long long r, long long c, mpc_t v){
  mpmat_t *p = (mpmat_t *) m;
  if(p->rows >= r && p->cols >=c){
    mpc_set(p->space[p->rows*(c-1)+r-1], v, MPC_RNDNN);
    return 0;
  } else {
    return -1;
  }
}

int mpmat_add(mpmat c, mpmat a, mpmat b){
  mpmat_t *cp;
  mpmat_t *ap;
  mpmat_t *bp;
  cp=(mpmat_t *) c;
  ap=(mpmat_t *) a;
  bp=(mpmat_t *) b;
}

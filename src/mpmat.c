#include<stdlib.h>
#include<mpc.h>
#include "mpmat.h"
typedef struct {
  mpfr_prec_t prec;
  long long rows;
  long long cols;
  mpc_t *space;
} mpmat_t;

static int mpmat_resize(mpmat_t c, long long r, long long c){
  for(long long i=0; i<c->rows*c->cols; i++){
    mpc_clear(c->space[i]);
  }
  c->space=(mpc_t *)realloc((void *)c->space, sizeof(mpc_t)*r*c);
  for(long long i=0; i<c*r; i++){
    mpc_init2(c->space[i], c->prec);
  }
  c->rows=r;
  c->cols=c;
  return 0;
}

mpmat mpmat_init(long long r, long long c){
  mpmat_t *res=(mpmat_t*)malloc(sizeof(mpmat_t));
  res->prec=256;
  res->rows=r;
  res->cols=c;
  res->space=(mpc_t *)malloc(sizeof(mpc_t)*r*c);
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
  if((ap->rows != bp->rows) || (ap->cols != bp->cols)){
    return -1;
  }
  // for convience we will resize c if necessary.
  mpmat_resize(cp, ap->rows, ap->cols);
  for(long long i=0; i<ap->rows*ap->cols; i++){
    mpc_add(cp->space[i], ap->space[i], bp->space[i], MPC_RNDNN);
  }
  return 0;
}

int mpmat_sub(mpmat c, mpmat a, mpmat b){
  mpmat_t *cp;
  mpmat_t *ap;
  mpmat_t *bp;
  cp=(mpmat_t *) c;
  ap=(mpmat_t *) a;
  bp=(mpmat_t *) b;
  if((ap->rows != bp->rows) || (ap->cols != bp->cols)){
    return -1;
  }
  // for convience we will resize c if necessary.
  mpmat_resize(cp, ap->rows, ap->cols);
  for(long long i=0; i<ap->rows*ap->cols; i++){
    mpc_sub(cp->space[i], ap->space[i], bp->space[i],MPC_RNDNN);
  }
  return 0;
}

int mpmat_scale(mpmat c, mpmat a, mpc_t alpha){
  mpmat_t *ap=(mpmat_t *) a;
  mpmat_t *cp=(mpmat_t *) c;
  mpmat_resize(cp, ap->rows, ap->cols);
  for(long long i=0; i<ap->rows*ap->cols; i++){
    mpc_mul(cp->space[i], ap->space[i], alpha, MPC_RNDNN);
  }
  return 0;
}

int mpmat_rows(mpmat c){
  return ((mpmat_t *)c)->rows;
}


int mpmat_cols(mpmat c){
  return ((mpmat_t *)c)->cols;
}

int mpmat_mul(mpmat c, mpmat a, mpmat b){
  mpmat_t *ap=(mpmat_t *)a;
  mpmat_t *bp=(mpmat_t *)b;
  mpmat_t *cp=(mpmat_t *)c;
  if(ap->cols != bp->rows){ //compat on mult
    return -1;
  }
  mpmat_resize(cp, ap->rows, bp->cols);
  mpc_t tmp;
  mpc_init2(tmp, ap->prec);
  for(long long i=0; i<ap->rows; i++){
    for(long long j=0; j<bp->cols; j++){
      mpc_set_ui(tmp, 0, MPC_RNDNN);
      for(long long k=0; k<ap->cols; k++){
        //a[i,k]*b[j,k] column major
        mpc_fma(tmp, ap->space[k*ap->cols+i], bp->space[j*bp->cols+k],
                tmp, MPC_RNDNN);
      }
      mpc_set(cp->space[i*cp->cols+j], tmp, MPC_RNDNN);
    }
  }
  return 0;
}
    

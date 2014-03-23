#include<stdio.h>
#include <stdlib.h>
#include<mpc.h>
#include "mpmat.h"
int main(){
  mpmat A = mpmat_init(2,2);
  mpmat B = mpmat_init(2,2);
  mpmat C = mpmat_init(2,2);
  mpc_t tmp;
  mpc_rnd_t rnd=MPC_RNDNN;
  mpc_init2(tmp, 256);
  mpc_set_d(tmp, 1.0, rnd);
  mpmat_set(A,1,1,tmp);
  mpc_set_d(tmp, 2.0, rnd);
  mpmat_set(A,1,2, tmp);
  mpc_set_d(tmp, 3.0, rnd);
  mpmat_set(A, 2, 1, tmp);
  mpc_set_d(tmp, 4.0, rnd);
  mpmat_set(A,2,2,tmp);
  
  mpc_set_d(tmp, 1.0, rnd);
  mpmat_set(B,1,1,tmp);
  mpc_set_d(tmp, 0.0, rnd);
  mpmat_set(B,1,2, tmp);
  mpc_set_d(tmp, 0.0, rnd);
  mpmat_set(B, 2, 1, tmp);
  mpc_set_d(tmp, 1.0, rnd);
  mpmat_set(B,2,2,tmp);

  mpmat_mul(C, A,B);
  for(int i=1; i<=2; i++){
    for(int j=1; j<=2; j++){
      fprintf(stdout, "C[%d,%d]=", i, j);
      mpmat_get(tmp, C, i, j);
      mpc_out_str(stdout, 10, 10,tmp,  rnd);
      fprintf(stdout, "\n");
    }
  }

  exit(0);
}

typedef mpmat *void;
mpmat mpmat_init();
void mpmat_free(mpmat m);
int mpmat_get(mpc_t v, mpmat m, long long r, long long c);
int mpmat_set(mpmat m, long long r, long long c, mpc_t val);
int mpmat_add(mpmat c, mpmat a, mpmat b);
int mpmat_sub(mpmat c, mpmat a, mpmat b);
int mpmat_scale(mpmat c, mpmat a, mpc_t alpha);
int mpmat_mul(mpmat c, mpmat a, mpmat b);

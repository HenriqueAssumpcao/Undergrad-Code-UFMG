#include <stdio.h>
#include <gmp.h>

void mdc_estendido(mpz_t g, mpz_t x, mpz_t y,const mpz_t a, const mpz_t b){
    if(mpz_cmp_ui(b,0) == 0){
        mpz_set_ui(x,1);
        mpz_set_ui(y,0);
        mpz_set(g,a);
    }
    else{
        mpz_t x1,y1,q,r,temp3;

        mpz_inits(x1,y1,q,r,temp3,NULL);
        mpz_tdiv_qr(q,r,a,b);
        mdc_estendido(g,x1,y1,b,r);

        mpz_set(x,y1); 

        mpz_mul(temp3,q,y1);
        mpz_sub(temp3,x1,temp3);
        mpz_set(y,temp3);
    }
};

int inverso_modular(mpz_t r, const mpz_t a, const mpz_t n){
    mpz_t g,temp1,temp2;
    mpz_inits(g,temp1,temp2,NULL);

    mdc_estendido(g,temp1,temp2,a,n);

    if(mpz_cmp_ui(g,1) == 0){
        if(mpz_cmp_ui(temp1,0) < 0){
            if(mpz_cmp_ui(n,0) < 0){
                mpz_sub(r,temp1,n);
            }
            else{
                mpz_add(r,temp1,n);
            }
        }
        else{
            mpz_set(r,temp1);
        }
        mpz_clears(g,temp1,temp2,NULL);
        return 1;
    }
    else{
        mpz_clears(g,temp1,temp2,NULL);
        return 0;
    }

}

void exp_binaria(mpz_t r,const mpz_t b,const mpz_t e,const mpz_t n){
    mpz_t curr_exp,curr_pot;
    mpz_inits(curr_exp,curr_pot,NULL);
    mpz_set(curr_exp,e);
    mpz_set(curr_pot,b);
    mpz_set_ui(r,1);

    while(mpz_cmp_ui(curr_exp,0) > 0){
        if(mpz_odd_p(curr_exp) != 0){
            mpz_mul(r,r,curr_pot);
            mpz_mod(r,r,n);
        }
        mpz_mul(curr_pot,curr_pot,curr_pot);
        mpz_mod(curr_pot,curr_pot,n);
        mpz_tdiv_q_ui(curr_exp,curr_exp,2);
    }
    mpz_clears(curr_exp,curr_pot,NULL);
}


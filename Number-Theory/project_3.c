#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

int MAX_TXT_SIZE = 500,BASE=256;

void codifica(mpz_t r, const char *str){
    int i = 0;
    mpz_t temp;
    mpz_init(temp);
    while(str[i] != '\0'){
        mpz_ui_pow_ui(temp,BASE,i);
        mpz_mul_ui(temp,temp,(int)str[i]);
        mpz_add(r,r,temp);
        i++;
    }
    mpz_clears(temp,NULL);
}

char *decodifica(const mpz_t n){
    char* str = malloc(MAX_TXT_SIZE * sizeof(char));
    int i = 0;
    mpz_t q,r;
    mpz_inits(q,r,NULL);
    mpz_set(q,n);
    while(i < MAX_TXT_SIZE){
        mpz_tdiv_qr_ui(q,r,q,BASE);
        str[i] = (char) mpz_get_ui(r);
        if(mpz_cmp_ui(q,0) == 0){
            str[i+1] = '\0';
            break;
        }
        i++;
    }
    mpz_clears(q,r,NULL);
    return str;
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

void criptografa(mpz_t C, const mpz_t M, const mpz_t n, const mpz_t e){
    exp_binaria(C,M,e,n);
}

void descriptografa(mpz_t M, const mpz_t C, const mpz_t n, const mpz_t d){
    exp_binaria(M,C,d,n);
}

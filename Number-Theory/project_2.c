#include <stdio.h>
#include <gmp.h>


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
int get_qt(mpz_t q, const mpz_t n){
    mpz_t temp1,temp2;
    int t = 0;
    mpz_inits(temp1,temp2,NULL);
    mpz_tdiv_r_ui(temp1,n,2);
    mpz_set(temp2,n);
    while(mpz_cmp_ui(temp1,0) == 0){
        t++;
        mpz_tdiv_q_ui(temp2,temp2,2);
        mpz_tdiv_r_ui(temp1,temp2,2);
    }
    mpz_set(q,temp2);
    return t;
}

void numero_aleatorio(mpz_t r, gmp_randstate_t rnd, const mpz_t n) {
    do {
        mpz_urandomm(r, rnd, n);
    } while (mpz_cmp_ui(r, 2) < 0);
}


int talvez_primo(const mpz_t a, const mpz_t n, const mpz_t n1, unsigned int t, const mpz_t q){
    mpz_t temp,r;
    mpz_inits(temp,r,NULL);
    mpz_mod(temp,n,a);
    if(mpz_cmp_ui(temp,0) == 0){
        mpz_clears(temp,r,NULL);
        return 1;
    }

    mpz_tdiv_r_ui(temp,n,2);
    if(mpz_cmp_ui(temp,2) > 0 && mpz_cmp_ui(temp,0) == 0){
        mpz_clears(temp,r,NULL);
        return 0;
    }
    else{
        exp_binaria(r,a,q,n);
        if(mpz_cmp_ui(r,1) == 0){
            mpz_clears(temp,r,NULL);
            return 1;
        }
        else{
            for(int i = 0; i < t; i++){
                mpz_add_ui(temp,r,1);
                mpz_mod(temp,temp,n);
                if(mpz_cmp_ui(temp,0) == 0){
                    mpz_clears(temp,r,NULL);
                    return 1;
                }
                mpz_mul(r,r,r);
                mpz_mod(r,r,n);
            }
            mpz_clears(temp,r,NULL);
            return 0;
        }
        }
}

int provavelmente_primo(const mpz_t n, unsigned int k, gmp_randstate_t rnd){
    mpz_t r,q,n1;
    mpz_inits(r,q,n1,NULL);
    mpz_sub_ui(n1,n,1);
    int curr_result,t = get_qt(q,n1);
    for(int i = 0; i < k; i++){
        numero_aleatorio(r,rnd,n);
        curr_result = talvez_primo(r,n,n1,t,q);
        if(curr_result == 0){
            mpz_clears(r,q,n1,NULL);
            return 0;
        }
    }
    mpz_clears(r,q,n1,NULL);
    return 1;
}

void primo_aleatorio(mpz_t r, unsigned int b, gmp_randstate_t rnd){
    mpz_t lb;
    mpz_init(lb);
    mpz_ui_pow_ui(lb,2,b-1);
    int is_prime;
    while(1 == 1){
        mpz_urandomb(r, rnd, b);
        if(mpz_cmp(r,lb) >= 0){
            is_prime = provavelmente_primo(r,50,rnd);
            if(is_prime){
                break;
            }
        }
    }
    mpz_clears(lb,NULL);
}

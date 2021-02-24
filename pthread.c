#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct{
    double a;
    double b;
    double area;
    int traps;
    int func;
    int index;
} thread_arg;

double f1(double x){
    return 5;
}

double f2(double x){
    return sin(2.0*3.141592*x) + cos(5.0*3.141592*x);
}

void calc_trap_per_thread(int *threads, int n_threads, int traps){
    int r, q, i;
    q = traps/n_threads;
    r = traps % n_threads;

    for(i=0; i < n_threads; i++){
        threads[i] = q;
    }
    
    if(r != 0){
        for(i=0;r != 0; r--){
            threads[i]++;
            i++;
        }
    }
}

void* calcThreads(void* args){
    thread_arg *arg = args;
    int i = 0;
    double x;
    double h = ((arg->b)-(arg->a))/arg->traps;
    if(arg->func == 0){

        arg->area = (f1(arg->a) + f1(arg->b))/2;
    
        for(i = 1; i < arg->traps; i++){
            x = arg->a + i*h;
            arg->area += f1(x);
        }

    }else{

        arg->area = (f2(arg->a) + f2(arg->b))/2;
    
        for(i = 1; i < arg->traps; i++){
            x = arg->a + i*h;
            arg->area += f2(x);
        }

    }

    arg->area = h*arg->area;

    pthread_exit(NULL);

}

int main(){
    int trap, n_threads, func, i, retorno;
    double a, b, area = 0;
    void* thread_return;

    printf("Dê os numeros de Trapézios e de Threads, respectivamente:\n");

    scanf("%d %d", &trap, &n_threads);

    printf("Digite a função que deseja executar 'f1(x) = 5':0 'f2(x) = sin(2.0 * x) + cos(5.0 * x)':1\n");

    scanf("%d",&func);

    printf("Digite o intervalo(a,b) para calcular a integral\n");

    scanf("%le %le", &a, &b);

    pthread_t threads[n_threads];

    thread_arg params[n_threads];

    int trap_per_thread[n_threads];

    calc_trap_per_thread(trap_per_thread, n_threads, trap);

    for(i = 0; i < n_threads; i++){
        printf("Thread [%d]: Vai calcular %d trapézio(s)\n", i, trap_per_thread[i]);
    }

    for(i = 0; i < n_threads; i++){
        params[i].area = 0;
        params[i].traps = trap_per_thread[i];
        params[i].a = i*((b-a)/trap)*params[i].traps;
        params[i].b = (i+1)*((b-a)/trap)*params[i].traps;
        params[i].func = func;
        pthread_create(&threads[i], NULL, calcThreads, (void*)(params+i));
    }

    for(i = 0;i < n_threads; i++){
        retorno = pthread_join(threads[i],&thread_return);
        area = params[i].area + area;
    }

    printf("Area: %.2le\n",area);

    return 0;
}
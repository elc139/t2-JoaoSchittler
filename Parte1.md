Trabalho 2 - Programação Paralela - João Gabriel Schittler
--------------------------------------------------
Pthreads
--------------------------------------------------
### Particionamento
Cada thread lida com uma parte dos vetores, indicada pelo identificador i;
for (i = 0; i < nthreads; i++)
      pthread_create(&threads[i], &attr, dotprod_worker, (void *) i);

### Comunicação
Mutexes são usados no primeiro programa para evitar uma condição de corrida entre as threads ao contabilizar a soma do produto escalar.
pthread_mutex_lock (&mutexsum);
dotdata.c += mysum;
pthread_mutex_unlock (&mutexsum);

### Aglomeração
Cada thread é criada com um identificador offset, que é usado para descobrir que parte dos produto a thread vai executar.
int start = offset*wsize;
int end = start + wsize;

### Mapeamento
Os vetores são separados em  n partes iguais, n sendo o numero de threads, cada thread executa sua parte.
for (i = start; i < end ; i++)
         mysum += (a[i] * b[i]);
### Speedup
Speedup(2) = 1325752/950464 = 1.394;
Speedup(4) = 1325752/724917 = 1.828;
Speedup(8) = 1325752/469879 = 2.821;
### Tabela de Speedup 
Worksize é dividido pelo número de threads

| Nthreads/(Size/Reps)| 1M/2K |1M/4K   | 1M/8K   |2M/2K  | 2M/4K |  2M/8K |
| --------------------| ------| ------ | ------- |------ | ------| ------ |
| 2 threads           | 1.394 |  1.379 |  1.403  | 1.526 | 1.235 | 1.086  |           
| 4 threads           | 1.828 |  1.441 |  1.627  | 1.526 | 1.287 | 1.140  | 
| 8 threads           | 2.821 |  2.360 |  3.040  | 1.439 | 1.103 | 1.087  |
--------------
A diferença entre dotprod e dotprod2 é que dotprod2 não usa mutexes, isso gera uma condição de corrida no código de dotprod2, tornando o resultado do programa incerto.

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

struct Args
{
    bool flag_cycle;
    bool* flag_used;
    const char *pattern;//исходный поиск
    char *line;//искомое
    long int x0;//начало
    long int x1;//конец
};

char* read_pattern(char *filename)
{
    FILE *fp;
    long int lSize;
    char *buffer;
    fp = fopen ( filename , "rb" );
    if( !fp )
        perror(filename),exit(1);
    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );
    printf("Size of %s = %ld\n", filename, lSize);
    /* allocate memory for entire content */
    buffer = (char*)calloc( 1, lSize+1 );
    if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

    /* copy the file into the buffer */
    if( 1 != fread(buffer , lSize, 1 , fp) )
        fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);
    fclose(fp);
    return buffer;
}

void* seek_substring_KMP (void *ptr)
{
    struct Args * a = (struct Args*)ptr;
    long int ofst = 0;
    long int M = strlen(a->line);
    long int * pi =(long int *)malloc(M * sizeof(long int)); //динамический массив длины М

    //Вычисление префикс-функции
    pi[0] = 0;
    printf("Part of this thread is ");
    for (int p = a->x0; p < a->x1; p++)
        printf("%c", a->pattern[p]);
    printf("\n");
    for(long int i = 1; i < M; i++)
    {
        while(ofst > 0 && a->line[ofst] != a->line[i])
            ofst = pi[ofst - 1];
        if(a->line[ofst] == a->line[i])
            ofst++;
        pi[i] = ofst;
    }
    //поиск
    for(long int i = a->x0, j = 0; i < a->x1; i++)
    {
        while(j > 0 && a->line[j] != a->pattern[i])
            j = pi[j - 1];

        if(a->line[j] == a->pattern[i])
            j++;
        if (j == M)
        {
            if(a->flag_cycle) {
                if(i-j+1 < M && !(*a->flag_used)){
                    printf("Pos = %ld\n", i - j + 1);
                    *a->flag_used = true;
                }
            } else printf("Pos = %ld\n", i - j + 1);
        }
    }
    free (pi); /* освобождение памяти массива pi */
    pthread_exit(NULL);
}

int main(int argc, char** argv)
{

    //if(argc!=4){
    // return 0;
    //}
    //char * arg1 = (char*)malloc(sizeof(char)*256);
    //char * arg2= (char*)malloc(sizeof(char)*256);
    //char * arg3 = (char*)malloc(sizeof(char)*256);
    //scanf("%s",arg1);// = "search.txt";
    //scanf("%s",arg2);
    //scanf("%s",arg3);
    //argv[2] = "str.txt";
    //argv[3] = "3";
    //argv[1] = arg1;
    //argv[2] = arg2;
    //argv[3] = arg3;

    //char * pattern = read_pattern(argv[1]);//чтение строки
    //char * line = read_pattern(argv[2]);//чтение искомого образца
    char * pattern = (char*)malloc((sizeof(char) * 500001));
    char * line = (char*)malloc((sizeof(char) * 500001));
    scanf("%s", pattern);
    scanf("%s", line);
    bool flag = false;

    if (strlen(pattern) == strlen(line)){
        char * buf = (char*)malloc(sizeof(char)*(strlen(pattern)*2+1));
        buf = strncat(buf, pattern, strlen(pattern) + 1);
        buf = strncat(buf, pattern, strlen(pattern) + 1);
        buf[strlen(pattern)*2 + 1] = 0;
        free(pattern);
        pattern = buf;
        flag = true;
    }

    //int threads_count = atoi(argv[3]);//заданное количество потоков
    int threads_count = 1;
    scanf("%d", &threads_count);
    printf("Threads count = %d\n", threads_count);
    int NUM = ceil((strlen(pattern)/(threads_count)));//количество кусков
    struct Args * a = (struct Args*)malloc(threads_count * sizeof(struct Args));//объект аргументов
    pthread_t *threads = (pthread_t*)malloc(threads_count*sizeof(pthread_t));//массив потоков
    int error_code;

    bool check = false;

    for(int i = 0; i < threads_count; i++)
    {
        //соответсвтвие каждого куска потоку
        a[i].flag_used = &check;
        a[i].flag_cycle = flag;
        a[i].pattern = pattern;
        a[i].line = line;
        a[i].x0 = i * NUM;
        if(i == threads_count - 1)
        {
            a[i].x1 = strlen(pattern) - 1;
        }
        else
        {
            a[i].x1 = i * NUM + strlen(line) + NUM - 1;
        }
    }
    clock_t t = clock();
    //создание потоков
    for(int i = 0 ; i < threads_count; i++)
    {
        error_code = pthread_create( &threads[i], NULL, seek_substring_KMP, (void*) &a[i]);
        if(error_code)
        {
            fprintf(stderr,"Error - pthread_create() return code: %d\n",error_code);
            exit(0);
        }
        //else printf("Thread %d is created\n",i);

    }
    //ожидание завершения потоков
    for(int i = 0; i < threads_count; i++)
    {
        pthread_join(threads[i],NULL);
    }
    t = clock() - t;
    free(a);
    free(threads);
    printf("Work time = %f\n",((float)t)/CLOCKS_PER_SEC);
    return 0;
}

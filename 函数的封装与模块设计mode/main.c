#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#define MAX_NUMBER 100
#define MIN_NUMBER 1
#define MAX_TIMES 10
int MakeNumber(void);
void GuessNumber(const int number);
int IsValidNum(const int number);
int IsRight(const int number, const int guess);
int main()
{
    int number;
    char reply;
    srand(time(NULL));
    do{
        number = MakeNumber(); /*��������������*/
        GuessNumber(number); /*�û�������*/
        printf("Do you want to continue(Y/n or y/n)?"); /*��ʾ�Ƿ����*/
        scanf(" %c", &reply);
    }while(reply == 'Y' || reply == 'y');
    return 0;
}

/*�����һ������*/
void GuessNumber(const int number)
{
    int guess;
    int count = 0;
    int right = 0;
    int ret;
    do{
        printf("Try %d:",count + 1);
        ret = scanf("%d", &guess);
        while(ret != 1 || IsValidNum(guess))
        {
            printf("Input error!\n");
            while(getchar() != '\n');
            printf("Try %d:", count + 1);
            ret = scanf("%d", &guess);
        }
        count++;
        right = IsRight(number,guess);
    }while(!right && count <= MAX_TIMES);
    if(right)
        printf("Congratulations! You' re so cool!\n");
    else
        printf("Mission failed after %d attempts n",MAX_TIMES);
}

/*����ڶ�������*/
int MakeNumber(void)
{
    int number;
    number = (rand() % (MAX_NUMBER - MIN_NUMBER + 1)) + MIN_NUMBER;
    assert(number >= MIN_NUMBER && number <= MAX_NUMBER);
    return number;
}

/*�������������*/
int IsValidNum(const int number)
{
    if(number >= MAX_NUMBER && number <= MIN_NUMBER)
        return 1;
    else
        return 0;
}

/*���庯��*/
int IsRight(const int number, const int guess)
{
    if(guess < number)
    {
        printf("Wrong! Too small!\n");
        return 0;
    }
    else if(guess > number)
    {
        printf("Wrong! Too big!\n");
        return 0;
    }
    else return 1;
}

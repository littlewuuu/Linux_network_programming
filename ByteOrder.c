#include<stdio.h>
void byteorder()
{
/*
结构体和共用体的区别在于：结构体的各个成员会占用不同的内存，互相之间没有影响；
而共用体的所有成员占用同一段内存，修改一个成员会影响其余所有成员。

结构体占用的内存大于等于所有成员占用的内存的总和（成员之间可能会存在缝隙），共用体占用的内存等于最长的成员占用的内存。
共用体使用了内存覆盖技术，同一时刻只能保存一个成员的值，如果对新的成员赋值，就会把原来成员的值覆盖掉。
*/
    union  
    {
        short value;
        char union_bytes[sizeof(short)];
    } test;
    test.value = 0x0102; //十六进制 0102 二进制：1 00000010

    if((test.union_bytes[0] == 1)&&(test.union_bytes[1] == 2))
    {
        printf("big endian\n");
    }
    else if((test.union_bytes[0] == 2)&& (test.union_bytes[1] == 1))
    {
        printf("little endian\n");
    }
    else
    {
        printf("unknown...\n");
    }
}

int main(void)
{
    byteorder();
}

/*
#include <stdio.h>
#include <stdlib.h>

#define TOTAL 4  //人员总数

struct{
    char name[20];
    int num;
    char sex;
    char profession;
    union{
        float score;
        char course[20];
    } sc;
} bodys[TOTAL];

int main(){
    int i;
    //输入人员信息
    for(i=0; i<TOTAL; i++){
        printf("Input info: ");
        scanf("%s %d %c %c", bodys[i].name, &(bodys[i].num), &(bodys[i].sex), &(bodys[i].profession));
        if(bodys[i].profession == 's'){  //如果是学生
            scanf("%f", &bodys[i].sc.score);
        }else{  //如果是老师
            scanf("%s", bodys[i].sc.course);
        }
        fflush(stdin);
    }

    //输出人员信息
    printf("\nName\t\tNum\tSex\tProfession\tScore / Course\n");
    for(i=0; i<TOTAL; i++){
        if(bodys[i].profession == 's'){  //如果是学生
            printf("%s\t%d\t%c\t%c\t\t%f\n", bodys[i].name, bodys[i].num, bodys[i].sex, bodys[i].profession, bodys[i].sc.score);
        }else{  //如果是老师
            printf("%s\t%d\t%c\t%c\t\t%s\n", bodys[i].name, bodys[i].num, bodys[i].sex, bodys[i].profession, bodys[i].sc.course);
        }
    }
    return 0;
}

*/
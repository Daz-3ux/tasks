#include<stdio.h>
#include<string.h>
#define MAX 10

int in_within(char,const char*);
char* mygets(char* ,int);

int main(int argc, char **argv)
{
    char input[MAX];     
	char ch;
	int found;          
	
	puts("Enter a string: ");     
	while (mygets(input, MAX) && input[0] != '\0'){
		puts("Enter a character: ");
		ch = getchar();
		while (getchar() != '\n'){       
			continue;
        }
		found = in_within(ch,input);         
		if (found == 0)
			printf("%c not found in string.\n", ch);         
		else
			printf("%c found in string %s\n", ch, input);
			puts("Next string: ");
	}
	puts("Done.\n");          
	
	return 0;
}

int in_within(char ch,const char* str)
{
	while (*str){
        if (*str != ch){
            ++str;
        }else{
            return 1;
        }
    }
    return 0;
}

char* mygets(char* str,int n)
{
    char *ret;
    char *find;

    ret = fgets(str, n, stdin);
    /*处理ENTER*/
    if(ret){
        find = strchr(str, '\n');//在str中搜索\n,\
        返回位置及后续字符，若未找到则返回NULL
        if (find){//若不是空指针
            *find = '\0';//把\n换成\0表示输入结束！！！！！
        }else{//要是没找到\n的话
            while (getchar() != '\n')//处理缓冲行
                continue;
        }
    }
    return ret;
}
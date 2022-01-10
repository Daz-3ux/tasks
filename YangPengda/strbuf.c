#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stddef.h>
#include<assert.h>//作用是如果它的条件返回错误，则终止程序执行

struct strbuf {
    int len;     //当前缓冲区（字符串）长度
    int alloc;   //当前缓冲区（字符串）容量,也就是我们每次动态分配的数组大小
    char *buf;   //缓冲区（字符串）
};

void strbuf_init(struct strbuf *sf, size_t alloc);
//初始化sf结构体，容量为 alloc。
void strbuf_attach(struct strbuf *sf, void *str, size_t len, size_t alloc);
//将字符串填充到sf中，长度为len,容量为alloc。
void strbuf_release(struct strbuf *sf);
//释放sb结构体的内存。
void strbuf_swap(struct strbuf *a, struct strbuf *b);
//交换两个 strbuf。
char *strbuf_detach(struct strbuf *sf, size_t *sz);
//将sb中的原始内存取出并获得其长度。
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second);
//比较两个strbuf的内存是否相同。1表示相同，0表示不同
void strbuf_reset(struct strbuf *sf);
//清空 sb。


int main() {
    struct strbuf sf;
    strbuf_init(&sf, 10);
    strbuf_attach(&sf, (void*)"xiyou", 5, 10);
    assert(strcmp(sf.buf, "xiyou") == 0);//如果它的条件返回错误，则终止程序执行
    printf("sf: %s\n",sf.buf);

    struct strbuf test;
    strbuf_init(&test, 20);
    strbuf_attach(&test, (void*)"xupt",4,20);
    printf("test: %s\n", test.buf);

    strbuf_swap(&sf, &test);
    printf("sf: %s test: %s\n", sf.buf,test.buf);
    
    struct strbuf test2;
    strbuf_init(&test2, 20);
    strbuf_attach(&test2, (void*)"xupt",4,20);
    printf("test2: %s\n", test2.buf);
    int i = strbuf_cmp(&sf,&test);
    int j = strbuf_cmp(&sf,&test2);

    printf("%d   %d\n",i,j);//不同为0，相同为1

    strbuf_release(&sf);
    if(!printf("%s",sf.buf)){
        printf("Nice!\n");
    }
}

void strbuf_init(struct strbuf *sf, size_t alloc)//初始化sf结构体，容量为 alloc。
{
    // struct strbuf
    // {
    //   int len;   //当前缓冲区（字符串）长度
    //   int alloc; //当前缓冲区（字符串）容量,也就是我们每次动态分配的数组大小
    //   char *buf; //缓冲区（字符串）
    // }
    sf -> len = 0;
    sf -> alloc = alloc;
    sf -> buf = (char*)malloc(sizeof(char) * alloc);
    if(sf -> buf == NULL){
        printf("Error:初始化失败");
    }
}

void strbuf_attach(struct strbuf *sf, void *str, size_t len, size_t alloc)//将字符串填充到sf中，长度为len,容量为alloc。
{
    if(len < alloc){
        sf -> len = len;
        strcpy(sf->buf, (char*)str);
    }else{
        printf("Error:当前容量不足");
    }
}

void strbuf_release(struct strbuf *sf)//释放sb结构体的内存。
{
    free(sf->buf);
    sf -> alloc = 0;
}

void strbuf_swap(struct strbuf *a, struct strbuf *b)//交换两个 strbuf。
{
    struct strbuf temp = *a;
    *a = *b;
    *b = temp;
}

char *strbuf_detach(struct strbuf *sf, size_t *sz)//将sb中的原始内存取出并获得其长度。
{
    char *ret = sf -> buf;
    *sz = sf -> len;
    strbuf_init(sf,0);
    return ret;
}

int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)//比较两个strbuf的内存是否相同。
{
    // size_t len = a->len < b->len ? a->len: b->len;
	// int cmp = memcmp(a->buf, b->buf, len);
	// if (cmp)
	// 	return cmp;
	// return a->len < b->len ? -1: a->len != b->len;
    int len = first->len < second->len ? first->len : second->len;
    int cmp = memcmp(first->buf, second->buf, len);
    if(cmp){//如果字符串不相等
        return 0;//0代表内存不同
    }else{//如果字符串相等则继续比较当前缓存区大小
        if(first->len != second->len){
            return 0;
        }
    }

    return 1;
}

void strbuf_reset(struct strbuf *sf)//清空 sb。
{
    sf -> len = 0;
    sf -> buf = NULL;
}
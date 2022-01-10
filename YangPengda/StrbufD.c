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
void strbuf_attach(struct strbuf *sf, void *str, size_t len, size_t alloc);
void strbuf_grow(struct strbuf *sb, size_t extra);
void strbuf_add(struct strbuf *sb, const void *data, size_t len);
static inline void strbuf_setlen(struct strbuf *sb, size_t len);

ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint);
//为 sb 直接扩容 hint ? hint : 8192 大小， 然后将文件描述符为 fd 的所有文件内容读取到 sb 中。
ssize_t strbuf_read_file(struct strbuf *sb, const char *path, size_t hint);
//为 sb 直接扩容 hint ? hint : 8192 大小， 然后将路径为 path 的所有文件内容读取到 sb 中。
int strbuf_getline(struct strbuf *sb, FILE *fp);
//将 将文件句柄为 fp 的一行内容读取到 sb 。


int main(int argc, char **argv)
{

}

ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
//为 sb 直接扩容 hint ? hint : 8192 大小， 然后将文件描述符为 fd 的所有文件内容读取到 sb 中。
{
    
}

ssize_t strbuf_read_file(struct strbuf *sb, const char *path, size_t hint)
//为 sb 直接扩容 hint ? hint : 8192 大小， 然后将路径为 path 的所有文件内容读取到 sb 中。
{

}

int strbuf_getline(struct strbuf *sb, FILE *fp)//将 将文件句柄为 fp 的一行内容读取到 sb 。
{

}








void strbuf_grow(struct strbuf *sb, size_t extra)//将sb的长度扩大extra
{
    strbuf_setlen(sb, extra);
}
void strbuf_add(struct strbuf *sb, const void *data, size_t len)//向sb追加长度为len的数据 data。
{
    strbuf_setlen(sb,len);
    while(sb->len > sb->alloc){
        sb->alloc *= 2;
        sb->buf = (char*)realloc(sb->buf,sizeof(char) * sb->alloc + 1);
    }
    strcat(sb->buf,(char*)data);
}
void strbuf_addch(struct strbuf *sb, int c)//向sb追加一个字符c。
{
    if(sb->len+1 > sb->alloc){
        sb->alloc *= 2;
        sb->buf = (char*)realloc(sb->buf,sizeof(char) * sb->alloc + 1);
    }
    memset(sb->buf+ sb->len,(char)c,sizeof(char));
    strbuf_setlen(sb,1);
}

static inline void strbuf_setlen(struct strbuf *sb, size_t len)//设置sb的长度len。
{
    sb->len += len;
}
void strbuf_init(struct strbuf *sb, size_t alloc)//初始化sf结构体，容量为 alloc。
{
    sb -> len = 0;
    sb -> alloc = alloc;
    sb -> buf = (char*)malloc(sizeof(char) * alloc);
    if(sb -> buf == NULL){
        printf("Error:初始化失败");
    }
}
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc)//将字符串填充到sf中，长度为len,容量为alloc。
{
    if(len < alloc){
        sb -> len = len;
        strcpy(sb->buf, (char*)str);
    }else{
        printf("Error:当前容量不足");
    }
}

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
void strbuf_reset(struct strbuf *sf);
void strbuf_release(struct strbuf *sf);

void strbuf_grow(struct strbuf *sb, size_t extra);
//将sb的长度扩大extra。
void strbuf_add(struct strbuf *sb, const void *data, size_t len);
//向sb追加长度为len的数据 data。
void strbuf_addch(struct strbuf *sb, int c);
//向sb追加一个字符c。
void strbuf_addstr(struct strbuf *sb, const char *s);
//向sb追加一个字符串s。
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2);
//向一个sb追加另一个strbuf的数据。
static inline void strbuf_setlen(struct strbuf *sb, size_t len);
//设置sb的长度len。
static inline size_t strbuf_avail(const struct strbuf *sb);
//计算sb目前仍可以向后追加的字符串长度。
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len);
//向sb内存坐标为pos位置插入长度为len的数据data。

int main(int argc, char **argv)
{
    struct strbuf sb;
    strbuf_init(&sb, 10);
    strbuf_attach(&sb, (void*)"xiyou", 5, 10);
    assert(strcmp(sb.buf, "xiyou") == 0);
    printf("sb: %s\n", sb.buf);

    size_t i1 = strbuf_avail(&sb);
    printf("第一次剩余空间：%zd\n",i1);

    strbuf_add(&sb,(char*)"XUPTtessssssssssssst",20);
    printf("sb: %s\n", sb.buf);

    strbuf_addch(&sb,'F');
    printf("sb: %s\n", sb.buf);

    char arr[] = "||abcdefg";
    char *str = arr;
    strbuf_addstr(&sb, str);
    printf("sb: %s\n", sb.buf);

    struct strbuf sb2;
    strbuf_init(&sb2, 10);
    strbuf_attach(&sb2, (void*)"GOOOOD", 5, 10);
    strbuf_addbuf(&sb,&sb2);
    printf("sb: %s\n", sb.buf);

    size_t i2 = strbuf_avail(&sb);
    printf("第二次剩余空间2：%zd\n",i2);

    strbuf_insert(&sb,3,(char*)"YEAH",4);//向sb内存坐标为pos位置插入长度为len的数据data。
    printf("sb: %s\n", sb.buf);

    strbuf_insert(&sb,3,(char*)"YEAHYEAHYEAHYEAHYEAHYEAHYEAHYEAHYEAHYEAHYEAH",44);
    printf("sb: %s\n", sb.buf);

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

void strbuf_addstr(struct strbuf *sb, const char *s)//向sb追加一个字符串s。
{
    int len = strlen(s);
    strbuf_setlen(sb,len);
    while(sb->len > sb->alloc){
        sb->alloc *= 2;
        sb->buf = (char*)realloc(sb->buf,sizeof(char) * sb->alloc + 1);
    }
    strcat(sb->buf,s);
}

void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)//向一个sb追加另一个strbuf的数据。
{
    strbuf_grow(sb , sb2->len);
    while(sb->len > sb->alloc){
        sb->alloc *= 2;
        sb->buf = (char*)realloc(sb->buf,sizeof(char) * sb->alloc + 1);
    }
    strcat(sb->buf,sb2->buf);
}

static inline void strbuf_setlen(struct strbuf *sb, size_t len)//设置sb的长度len。
{
    sb->len += len;
}

static inline size_t strbuf_avail(const struct strbuf *sb)//计算sb目前仍可以向后追加的字符串长度。
{
    return sb->alloc - sb->len;
}

void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)//向sb内存坐标为pos位置插入长度为len的数据data。
{
    // if(pos+len >= sb->alloc){
    //     strbuf_setlen(sb, pos+len - sb->len);
    //     while(sb->len > sb->alloc){
    //        sb->alloc *= 2;
    //         sb->buf = (char*)realloc(sb->buf,sizeof(char) * sb->alloc + 1);
    //     }
    //     char *temp = (char*)malloc(sizeof(char)*sb->len);
    //     strncpy(temp,sb->buf,pos);
    //     strcat(temp,(char*)data);
    // }else{
        while(sb->len > sb->alloc){
        sb->alloc *= 2;
        sb->buf = (char*)realloc(sb->buf,sizeof(char) * sb->alloc + 1);
    }
        char *temp = (char*)malloc(sizeof(char)*sb->len);
        strncpy(temp,sb->buf,pos);
        strcat(temp,(char*)data);
        strcat(temp,sb->buf+pos+len);
        strcpy(sb->buf,temp);
    //}
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

void strbuf_reset(struct strbuf *sb)//清空 sb。
{
    sb -> len = 0;
    sb -> buf = NULL;
}
#include "strbuf.h"

void strbuf_init(struct strbuf *sb, size_t alloc)//初始化sf结构体，容量为 alloc。
{
    sb->len = 0;
    sb->alloc = alloc;
    if(alloc != 0){
        sb->buf = (char *) malloc(sizeof(char) * alloc);
        if (sb->buf == NULL) {
            printf("Error:初始化失败");
        }
    }
}

void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc)//将字符串填充到sf中，长度为len,容量为alloc。
{
    //strbuf_init(sb, alloc);
    sb->len = len;
    sb->alloc = alloc;
    sb->buf = (char *) str;
    //sb->buf[sb->len] = '\0';
}

void strbuf_release(struct strbuf *sb)//释放sb结构体的内存。
{
    if (sb->alloc) {
        free(sb->buf);
        sb->alloc = 0;
    }
}

void strbuf_swap(struct strbuf *a, struct strbuf *b)//交换两个 strbuf。
{
    int tempLen;
    tempLen = a->len;
    a->len = b->len;
    b->len = tempLen;

    int tempAlloc;
    tempAlloc = a->alloc;
    a->alloc = b->alloc;
    b->alloc = tempAlloc;

    //char *temp = (char *) malloc(sizeof(char) * a->len);
    struct strbuf temp = *a;
    a->buf = b->buf;
    b->buf = temp.buf;

    //free(temp);
}

char *strbuf_detach(struct strbuf *sb, size_t *sz)//将sb中的原始内存取出并获得其长度。
{
    //char *ret = sb->buf;
    *sz = sb->alloc;
    return sb->buf;
}

int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)//比较两个strbuf的内存是否相同。
{
    size_t len = first->len < second->len ? first->len : second->len;//取较小
    int cmp = memcmp(first->buf, second->buf, len);
    if (cmp) {   //如果字符串不相等
        return 1;//1代表不同
    } else {     //如果字符串相等则继续比较当前缓存区大小
        if (first->len != second->len) {
            return 1;
        } else if (first->len == second->len) {
            return 0;
        }
    }

    return 0;
}

void strbuf_reset(struct strbuf *sb)//清空 sb。
{
    strbuf_setlen(sb,0);
}

void strbuf_grow(struct strbuf *sb, size_t extra)//将sb的长度扩大extra
{
    if(sb->len+extra < sb->alloc){
        return;
    }
    if(sb->alloc == 0){
        sb->buf = NULL;
    }
    
    sb->alloc = sb->len + extra+1;
    sb->buf = (char *) realloc(sb->buf,sizeof(char)*(sb->alloc));
    // if (sb->alloc == 0){
	// 	sb->buf[0] = '\0';
    // }
}

void strbuf_add(struct strbuf *sb, const void *data, size_t len)//向sb追加长度为len的数据 data。
{
    strbuf_grow(sb, len);
	memcpy(sb->buf + sb->len, data, len);
	// sb->len += len;
    // sb->buf[sb->len] = '\0';
    strbuf_setlen(sb,sb->len + len);
}

void strbuf_addch(struct strbuf *sb, int c)//向sb追加一个字符c。
{
    if (sb->len + 1 >= sb->alloc) {
        sb->alloc *= 2;
        sb->buf = (char *) realloc(sb->buf, sizeof(char) * sb->alloc);
    }
    memset(sb->buf + sb->len, (char) c, sizeof(char));
    sb->len += 1;
    sb->buf[sb->len] = '\0';
}

void strbuf_addstr(struct strbuf *sb, const char *s)//向sb追加一个字符串s。
{
    int len = strlen(s);
    // while(sb->len + len > sb->alloc){
    //     strbuf_grow(sb,len);
    // }
    // for (int i = 0;s[i]; i++){
    //     sb->buf[sb->len++] = s[i];
    // }
    // sb->buf[sb->len] = '\0';
    strbuf_add(sb,s,len);
}

void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)//向一个sb追加另一个strbuf的数据。
{
    // strbuf_grow(sb, sb2->len);
    // while (sb->len > sb->alloc) {
    //     sb->alloc *= 2;
    //     sb->buf = (char *) realloc(sb->buf, sizeof(char) * sb->alloc + 1);
    // }
    strbuf_grow(sb, sb2->len);
    strbuf_addstr(sb,sb2->buf);
}

void strbuf_setlen(struct strbuf *sb, size_t len)//设置sb的长度len。
{
    sb->len = len;
    sb->buf[sb->len] = '\0';
}

size_t strbuf_avail(const struct strbuf *sb)//计算sb目前仍可以向后追加的字符串长度。
{
    return sb->alloc - sb->len -1;
}

void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)//向sb内存坐标为pos位置插入长度为len的数据data。
{
    strbuf_grow(sb, len);
    // memmove(sb->buf+pos+len,sb->buf+pos,sb->len -pos);
    // memmove(sb->buf+pos,data,len);
    char *temp = (char*)malloc(sizeof(char) *sb->alloc);
    if(temp == NULL){
        printf("Error");
        exit(1);
    }
    memmove(temp,sb->buf,pos);
    memmove(temp+pos,data,len);
    memmove(temp+pos+len,sb->buf + pos,sb->len-pos);
    memmove(sb->buf,temp,sb->len+len);
    free(temp);
    strbuf_setlen(sb,sb->len +len);
}

void strbuf_rtrim(struct strbuf *sb)//去除 sb 缓冲区右端的所有 空格，tab, '\t'。
{
    while (sb->len > 0 && (sb->buf[sb->len - 1] == '\t' || sb->buf[sb->len - 1] == ' ')) {
        sb->len--;//栈
    }
    sb->buf[sb->len] = '\0';//有效字符串后面的一位为'\0'
}

void strbuf_ltrim(struct strbuf *sb)//去除 sb 缓冲区左端的所有 空格，tab, '\t'。
{
    char *temp = sb->buf;
    while (sb->len > 0 && (sb->len > 0 && ((*temp) == '\t' || (*temp) == ' '))) {
        temp++;
        sb->len--;
    }
    memmove(sb->buf, temp, sb->len);//避免重叠部分出错
    //free(temp);
}

void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)//删除 sb 缓冲区从 pos 坐标长度为 len 的内容。
{
    char *temp = (char *) malloc(sizeof(char) * sb->len);
    if (temp == NULL) {
        printf("在remove函数分配内存失败");
    }
    memcpy(temp, sb->buf, pos);
    memmove(temp + pos, sb->buf + pos+len, sb->len - len - pos);
    memmove(sb->buf, temp, sb->len - len);
    strbuf_setlen(sb,sb->len-len);
    free(temp);
}

ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
//为 sb 直接扩容 hint ? hint : 8192 大小， 然后将文件描述符为 fd 的所有文件内容读取到 sb 中。
{
    FILE *fp = fdopen(fd, "r");
    if(fp == NULL){
        printf("Error");
        exit(1);
    }

    char ch;
    int i = 0;
    for(; (ch = fgetc(fp)) != EOF;i++){
        if(sb->len +1 <= sb->alloc){
            sb->alloc += (hint ? hint: 8192);
            sb->buf = (char*)realloc(sb->buf, sb->len + sb->alloc);
            strbuf_addch(sb,ch);
        }
    }
    fclose(fp);
    
    return i;
}

// ssize_t strbuf_read_file(struct strbuf *sb, const char *path, size_t hint)
// //为 sb 直接扩容 hint ? hint : 8192 大小， 然后将路径为 path 的所有文件内容读取到 sb 中。
// {
//     return NULL;
// }

int strbuf_getline(struct strbuf *sb, FILE *fp)//将 将文件句柄为 fp 的一行内容读取到 sb 。
{
    int ch;
    do{
        ch = fgetc(fp);
        if(feof(fp) || ch == EOF || ch == '\n'){
            break ;
        }
        if(strbuf_avail(sb) < 1){
            strbuf_grow(sb,1);
        }
        strbuf_addch(sb,ch);
    }while(1);


    return 0;
}

bool strbuf_begin_judge(char *target_str, const char *str, int strnlen) 
//实现判断一个 strbuf 是否以指定字符串开头的功能【strnlen是源字符串的长度，真坑爹】
{
    if(str == NULL){
        return true;
    }
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (target_str[i] != str[i]) {
            return false;
        }
    }
    return true;
}

char *strbuf_get_mid_buf(char *target_buf, int begin, int end, int len) 
//获取字符串从坐标 [begin, end] 的所有内容
{
    if(target_buf == NULL){
        return NULL;
    }
    char *temp = (char *) malloc(sizeof(char) * (end - begin+2));
    int cnt = 0;
    for (int i = begin; i < end; i++) {
        temp[cnt++] = target_buf[i];
    }
    temp[cnt] = '\0';

    return temp;
}

struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
//将长度为 len 的字符串 str 根据切割字符 terminator 切成多个 strbuf,并从结果返回
//max 可以用来限定最大切割数量。
//返回 struct strbuf 的指针数组，数组的最后元素为 NULL
{
    struct strbuf **ret = (struct strbuf **)malloc(sizeof(struct strbuf*));

    char flag[2];
    flag[0] = (char)(terminator);
    flag[1] = '\0';

    char remove_const[len+1];
    memcpy(remove_const,str,len+1);
    for(int i = 0; i < len; i++){
        if(remove_const[i] == '\0'){
            remove_const[i] = '!';
        }
    }

    char *mystr = strtok(remove_const,flag);  
    // if(mystr == NULL){
    //     ret = (struct strbuf **)realloc(ret,sizeof(struct strbuf*)*3);
    // }
    int cnt = 0;
    while(mystr != NULL && cnt < max){
        int mylen = strlen(mystr);
        for(int i = 0; i < mylen; i++){
            if(mystr[i] == '!'){
                mystr[i] = '\0';
            }
        }
        struct strbuf *temp = (struct strbuf *)malloc(sizeof(struct strbuf));
        strbuf_init(temp,mylen+1);
        //strbuf_addstr(temp,mystr);
        strbuf_add(temp, mystr,mylen);

        ret = (struct strbuf **)realloc(ret,sizeof(struct strbuf*)*(cnt+2));
        ret[cnt++] = temp;
        mystr = strtok(NULL,flag);
        //free(temp);
    }
    ret[cnt] = NULL;
    //free(remove_const);
    // return NULL;
    return ret;
}


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define bool char
#define true 1
#define false 0
#define GARBAGE 0xA3
typedef char byte;


/**
 * @brief 申请内存
 *
 * @param[out] *ppv 指向申请的内存
 * @param[in] size 申请内存的字节数
 *
 * @return true for success.
 */
bool newMemory(void** ppv,unsigned int size){
	if(ppv == NULL)  return false;
	if(size <= 0)  return false;

	byte** ppb = (byte **)ppv;
	*ppb = (byte *)malloc(size);
	if(*ppb != NULL){
		memset(*ppb,GARBAGE,size);
		return true;
	}
	return false;
}

/**
 * @brief 将一个大于0的整数转化为对应的字符串
 *
 * @param[out] str 转换所得的字符串
 * @param[in] num 待转换的正整数
 *
 * @return true for success.
 */
bool uintToStr(byte** str,int num){
	if(num < 0) return false;
	if(str == NULL) return false;

	byte s[12];

    //将num各个数位上的数字提取出来(从低位到高位)
	int i = 0;
	do{
		s[i++] = num % 10 + '0';
	}
	while((num /= 10)>0);

	if(newMemory((void **)str,i+1) == false){
    	return false;
    }

    i = i-1;
    int k = 0;
	while(i >= 0){
		*((*str)+k) = s[i--];
		k++;
	}
	*(*str+k) = '\0';

	return true;
}

/**
 * @brief replace str with num 
 *        给定一段内存old，将其中出现的str字符串替换为整数num对应的字符串，替换后的内存为new
 *
 * @param[in] old 原始内存的首地址
 * @Param[in] oldLen 原始内存的长度
 * @param[in] str 被替换的字符串
 * @param[in] num 用来替换str的整数
 * @param[out] new 新内存的首地址
 * @param[out] newLen 新内存的长度
 *
 * @return true for success.
 */
bool replace(byte* old,int oldLen,const char* str,int num,byte** new,int* newLen){
	if(old == NULL)  return false;
	if(oldLen <= 0)  return false;
	if(str == NULL)  return false;
	if(strlen(str) == 0)  return false;
	if(num < 0)  return false;
	if(new == NULL)  return false;
	if(newLen == NULL) return false;

	int strLen = strlen(str);

    *newLen = oldLen;
    if(newMemory((void**)new,*newLen) == false){
    	return false;
    }
    memcpy(*new,old,oldLen);

	byte* strNum;
	uintToStr(&strNum,num);
	int strNumLen = strlen(strNum);

    int i;
	for(i=0;i<*newLen-strLen+1;i++){
		if(memcmp((*new+i),str,strLen) == 0){
 			byte* tmp;
 			if(newMemory((void **)&tmp,*newLen-strLen+strNumLen+1) == false){
 				return false;
 			}
 			memcpy(tmp,*new,i);
 			memcpy(tmp+i,strNum,strNumLen);
 			memcpy(tmp+i+strNumLen,*new+i+strLen,*newLen-i-strLen);
 			*newLen = *newLen-strLen+strNumLen;
 			*(tmp+*newLen) = '\0';
 			free(*new);
 			*new = tmp;
 			i = i - 1 + strNumLen;
		}
	}
	return true;
}


int main(){
	char* a = "abc%HM%cdef%VLAN";
	char* new;
	int newLen;
	char* b = "HM";
	printf("\nbefore:\n");
	printf("%s\n",a);
	printf("replace %s with %d\n",b,23456789);
	if(replace(a,strlen(a),b,23456789,(byte **)&new,&newLen)){
		printf("after:\n");
		printf("%s\n",new);
	}

	printf("\n\nbefore:\n");
	printf("%s\n",a);
	printf("replace %s with %d\n",b,2);
	if(replace(a,strlen(a),b,2,&new,&newLen)){
		printf("after:\n");
		printf("%s\n",new);
	}
	return 0;
}
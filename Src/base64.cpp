#include <iostream>
#include <cstdio>
#include <string>
#include <map>
#include <cstring>
using namespace std;
 
unsigned char* base64_encode(const char* str0)
{
	unsigned char* str = (unsigned char*)str0;	//转为unsigned char无符号,移位操作时可以防止错误
	unsigned char base64_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";//也可以用map,这里用数组其实更方便
	long len;				//base64处理后的字符串长度
	long str_len;			//源字符串长度
	long flag;				//用于标识模3后的余数
	unsigned char* res;		//返回的字符串
	str_len = strlen((const char*)str);
	switch (str_len % 3)	//判断模3的余数
	{
	case 0:flag = 0; len = str_len / 3 * 4; break;
	case 1:flag = 1; len = (str_len / 3 + 1) * 4; break;
	case 2:flag = 2; len = (str_len / 3 + 1) * 4; break;
	}
	res = (unsigned char*)malloc(sizeof(unsigned char) * len + 1);
	for (int i = 0, j = 0; j < str_len - flag; j += 3, i += 4)//先处理整除部分
	{
		//注意&运算和位移运算的优先级,是先位移后与或非,括号不对有可能导致错误
		res[i] = base64_map[str[j] >> 2];
		res[i + 1] = base64_map[(str[j] & 0x3) << 4 | str[j + 1] >> 4];
		res[i + 2] = base64_map[(str[j + 1] & 0xf) << 2 | (str[j + 2] >> 6)];
		res[i + 3] = base64_map[str[j + 2] & 0x3f];
	}
	//不满足被三整除时,要矫正
	switch (flag)
	{
	case 0:break;	//满足时直接退出
	case 1:res[len - 4] = base64_map[str[str_len - 1] >> 2];	//只剩一个字符时,右移两位得到高六位
		res[len - 3] = base64_map[(str[str_len - 1] & 0x3) << 4];//获得低二位再右移四位,自动补0
		res[len - 2] = res[len - 1] = '='; break;				//最后两个补=
	case 2:
		res[len - 4] = base64_map[str[str_len - 2] >> 2];				//剩两个字符时,右移两位得高六位
		res[len - 3] = base64_map[(str[str_len - 2] & 0x3) << 4 | str[str_len - 1] >> 4];	//第一个字符低二位和第二个字符高四位
		res[len - 2] = base64_map[(str[str_len - 1] & 0xf) << 2];	//第二个字符低四位,左移两位自动补0
		res[len - 1] = '=';											//最后一个补=
		break;
	}
	res[len] = '\0';	//补上字符串结束标识
	return res;
}
 
int main(){
    char str[100];
    scanf("/home/pi/Code/clsscan/img/test.png", str);
    //str="/home/pi/Code/clsscan/img/test.png"
    printf("加密后的字符串是:%s",base64_encode(str));
}


 
 
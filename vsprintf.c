/*
 *  linux/kernel/vsprintf.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/* vsprintf.c -- Lars Wirzenius & Linus Torvalds. */
/*
 * Wirzenius wrote this portably, Torvalds fucked it up :-)
 */

#include <stdarg.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>

//simple_strtoul作用为将一个字符串转化为无符号长整型
unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base) //cp指向字符串开始处，endp指向分析后字符串末尾处，base为进制数，值为0则自动判断进制数，0x表示十六进制，0表示八进制，其余为十进制
{
	unsigned long result = 0,value;

	if (!base) {  //如果base为0，cp非0开头，则base为十进制
		base = 10; 
		if (*cp == '0') { //如果*cp以0开头，则base为八进制
			base = 8;
			cp++;
			if ((*cp == 'x') && isxdigit(cp[1])) { //如cp以0x开头且后面为数字，则base为十六进制 isxdigit 检查参数是否为16进制数字
				cp++;
				base = 16;
			}
		}
	}
    // islower 检查参数是否为小写英文字母 toupper将字符转换为大写英文字母
    // while循环判断cp中第一个不是数字的字母，如果*cp为数字则value=*cp-'0'；如果*cp为字符value=*cp-’a‘+10，利用base判断value是否合法。
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp) 
	    ? toupper(*cp) : *cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	}
    // 使endp指向字符串有效数字的结束地址
	if (endp) 
		*endp = (char *)cp;
	return result;
}

/* we use this so that we can do without the ctype library */ //使用宏定义来实现isdigit功能，不用导入ctype库
#define is_digit(c)	((c) >= '0' && (c) <= '9') //判断一个字符是否在0-9之间

static int skip_atoi(const char **s) //将数字字符串转化为整型
{
	int i=0;
    //判断字符是不是数字，是数字就进行累加
	while (is_digit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}
// 利用二进制的0和1来判断输出格式
#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define SMALL	64		/* use 'abcdef' instead of 'ABCDEF' */

// 没看懂，查了一下大概是内嵌汇编，定义除法运算
#define do_div(n,base) ({ \
int __res; \
__asm__("divl %4":"=a" (n),"=d" (__res):"0" (n),"1" (0),"r" (base)); \
__res; })

static char * number(char * str, int num, int base, int size, int precision
	,int type) //将数字转化为数字字符串
{
	char c,sign,tmp[36]; // c为标志位，sign为符号位
	const char *digits="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; //定义一个存储了全部数字和大写字母的静态字符指针
	int i;

	if (type&SMALL) digits="0123456789abcdefghijklmnopqrstuvwxyz"; // 如果是小写则把字符串中大写字母改写为小写字母
	if (type&LEFT) type &= ~ZEROPAD; // 如果是左对齐，去掉零标志位
	if (base<2 || base>36) // 判断base值是否正确
		return 0;
	c = (type & ZEROPAD) ? '0' : ' ' ; //如果设置了零标志位，则补零，否则补空格
	if (type&SIGN && num<0) { // 如果设定了符号位且数字为负，则补上负号
		sign='-';
		num = -num;
	} else // 如果设置标志位PLUS，则在正数前加+号，设置SPACE标志位则表示符号位用空格填充，否则用0填充
		sign=(type&PLUS) ? '+' : ((type&SPACE) ? ' ' : 0);
    if (sign) size--; // 如果符号位为非零值，则size长度减一
	if (type&SPECIAL) // 如果设置了SPECIAL标志位，则如果base为16位，size长度减二， base为8位，size长度减一（0和0x）
		if (base==16) size -= 2;
		else if (base==8) size--;
	i=0;
    //将数字num转化为字符串存到tmp数组中
	if (num==0)
		tmp[i++]='0';
	else while (num!=0)
		tmp[i++]=digits[do_div(num,base)];//对num进行除法运算，索引digits字符串并写入tmp
	if (i>precision) precision=i; //precision是精度，如果i超出精度值则将精度值初始化为i
	size -= precision; // size长度减去精度长度
	if (!(type&(ZEROPAD+LEFT))) // 如果没有补零和左对齐，则补空格
		while(size-->0)
			*str++ = ' ';
	if (sign) // 如果有符号位，就写入符号位
		*str++ = sign;
	if (type&SPECIAL) //如果是八进制或十六进制，则补上0和0x
		if (base==8)
			*str++ = '0';
		else if (base==16) {
			*str++ = '0';
			*str++ = digits[33];
		}
	if (!(type&LEFT)) //没有设置左对齐，则根据其他标志位进行填充
		while(size-->0)
			*str++ = c;
	while(i<precision--) //用0填充精度
		*str++ = '0';
	while(i-->0) // 写入数字部分
		*str++ = tmp[i];
	while(size-->0) // 用空格填充剩余部分
		*str++ = ' ';
	return str; 
}

int vsprintf(char *buf, const char *fmt, va_list args) //fmt指针存的格式说明符号，buf是待写入数据的字符串
{
	int len;
	int i;
	char * str;
	char *s;
	int *ip;

	int flags;		/* flags to number() */

	int field_width;	/* width of output field */
	int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
	int qualifier;		/* 'h', 'l', or 'L' for integer fields */
    
	for (str=buf ; *fmt ; ++fmt) { //扫描传入的buf字符串
		if (*fmt != '%') { //如果没有“%”，则正常写入str中
			*str++ = *fmt;
			continue;
		}
			
		/* process flags */
        // 如果是%号
		flags = 0;
		repeat: //repeat 重复
			++fmt;		/* this also skips first '%' */
            ////取%后面的一个字符, 判断是否为'-', '+', ' ', '#', '0'
			switch (*fmt) {
				case '-': flags |= LEFT; goto repeat;　//左对齐
				case '+': flags |= PLUS; goto repeat;　// 右对齐
				case ' ': flags |= SPACE; goto repeat;　//空格
				case '#': flags |= SPECIAL; goto repeat;　//输出8进制或16进制
				case '0': flags |= ZEROPAD; goto repeat; //零标志位，补零
				}
		
		/* get field width */
		field_width = -1;　// 初始化输出宽度
		if (is_digit(*fmt))　//如果下一个字符是数字
			field_width = skip_atoi(&fmt);　//将字符转化为整型并输出宽度更新为该值
		else if (*fmt == '*') { //如果下一个字符为*
			/* it's the next argument */
			field_width = va_arg(args, int);　//由下一个参数确定输出宽度
			if (field_width < 0) {　//如果下一个参数为负数，则取相反数并左对齐
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/* get the precision */
		precision = -1;　//初始化精度
		if (*fmt == '.') { //如果下一个字符为'.'，说明设定输出精度
			++fmt;	
			if (is_digit(*fmt))　//如果下一个字符是数字，将输出精度更新为该值
				precision = skip_atoi(&fmt);
			else if (*fmt == '*') {　//如果下一个字符为'*'，由下一个参数确定输出精度
				/* it's the next argument */
				precision = va_arg(args, int);
			}
			if (precision < 0)　//精度不能小于0
				precision = 0;
		}

		/* get the conversion qualifier */
        //限定输出为什么类型，ｈ为short型, l为long型, L为long double型
		qualifier = -1;　
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') { //如果下一个字符为h, l, Ｌ, 将其更新为qualifier
			qualifier = *fmt;　
			++fmt;
        
		switch (*fmt) { 
		case 'c': // 将参数以字符的形式输出
			if (!(flags & LEFT))　//不是左对齐则在前面补齐空格
				while (--field_width > 0)
					*str++ = ' ';
			*str++ = (unsigned char) va_arg(args, int); // 填充参数列表中的值
			while (--field_width > 0) //是左对齐则在后面补齐空格
				*str++ = ' ';
			break;

		case 's':　// 将参数以字符串的形式输出
			s = va_arg(args, char *); //取参数列表中的字符串
			if (!s) //如果s为0，则字符串置为NULL
				s = "<NULL>";
			len = strlen(s);　//将len更新为字符串长度
			if (precision < 0)　//如果精度小于0，则将其更新为len，大于0则将len更新为精度大小
				precision = len;
			else if (len > precision) 
				len = precision;

			if (!(flags & LEFT)) //同样，判断是否为左对齐，根据情况补齐空格
				while (len < field_width--)
					*str++ = ' ';
			for (i = 0; i < len; ++i) //填充字符串
				*str++ = *s++;
			while (len < field_width--)
				*str++ = ' ';
			break;

		case 'o': // 将参数以8进制数输出
			str = number(str, va_arg(args, unsigned long), 8,
				field_width, precision, flags);
			break;
        
		case 'p': // 将参数以指针方式输出
			if (field_width == -1) { //如果没有设置输出宽度，则设置输出宽度为8，标志位为零标志位
				field_width = 8;
				flags |= ZEROPAD;
			}
			str = number(str,
				(unsigned long) va_arg(args, void *), 16,
				field_width, precision, flags);
			break;
        
		case 'x': // 输出16进制数
			flags |= SMALL; // 设置标志位为SMALL，输出小写字母
		case 'X':
			str = number(str, va_arg(args, unsigned long), 16,
				field_width, precision, flags);
			break;

		case 'd'://  表示有符号整形输出
		case 'i'://　表示有符号输出
			flags |= SIGN;
        case 'u'://  无符号输出
			str = number(str, va_arg(args, unsigned long), 10,
				field_width, precision, flags);
			break;

		case 'n':// 表示将到目前为止转换的字符个数保存到ip指针指定的位置
			ip = va_arg(args, int *);
			*ip = (str - buf);
			break;

		default:// 如果不是%，则写入一个%，如果字符串还没遍历完则继续写入字符，否则退出循环
			if (*fmt != '%')
				*str++ = '%';
			if (*fmt)
				*str++ = *fmt;
			else
				--fmt;
			break;
		}
	}
	*str = '\0';　//末尾补\0表示结束
	return str-buf;
}

int sprintf(char * buf, const char *fmt, ...)　//利用变参函数实现sprintf
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsprintf(buf,fmt,args);
	va_end(args);
	return i;
}


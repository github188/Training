#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define SALARY_NORMAL 10
#define SALARY_OVER_TIME (SALARY_NORMAL * 1.5) //Kelly: 記得加括號
#define OVER_TIME 40
#define TAX_LOW 300
#define TAX_HIGH 450
#define TAX_LOW_PERCENTAGE 0.15
#define TAX_MIDDLE_PERCENTAGE 0.2
#define TAX_HIGH_PERCENTAGE 0.5
#define TRUE 1
#define FALSE 0


int not_numeric(char cInput)      //明偉: 用ASCII table判斷input有沒有字元:  < 48 or > 57 但是小數點46不要包括進去
{
	if((cInput - '\0') != 46  &&  ((cInput - '\0') < 48 || (cInput - '\0') > 57))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


int get_worktime(char *cWorkHour)
{
	int i = 0;
	while(cWorkHour[i] != '\0')
        {
                if(not_numeric(cWorkHour[i]))
		{
			printf("Input can't not contain character or be negative number!!!\n");
			return FALSE;
		}
		else
		{
			i += 1;
		}
	}
	return TRUE;

}


int main()
{
	char cWorkHour[1000] = {0};
	float fWorkHour;
	float fSalary = 0.0;
	float fTax = 0.0;
	float fProfit = 0.0;
	//1. 依照題意, work hour應該是可輸入小數點的數字, 而非int
	//2. 你能夠拉出一個function: get_worktime(), 這function是接收使用者輸入的資料, 並且檢查是否合法?
	//3. 所謂的合法, 指的是是否為數字, 且為符合題意的數字. ex: -1, 1/4, abc..這種必須被濾掉

	printf("Enter the working hours:\n");

	if (!scanf("%s", cWorkHour))
	{
		printf("Error\n");
		exit(EXIT_FAILURE);
	}

	if (get_worktime(cWorkHour))
	{
		fWorkHour = atof(cWorkHour);		//明偉: 一切沒問題就轉成float型態以供計算
		if (fWorkHour <= OVER_TIME)
		{
	 		fSalary = fWorkHour * SALARY_NORMAL;
		}
		else
		{
			fSalary = (OVER_TIME * SALARY_NORMAL) + (fWorkHour - OVER_TIME) * SALARY_OVER_TIME;
		}
		printf("Your salary is %f\n", fSalary);
		if (fSalary >= TAX_HIGH)
		{
			fTax = fSalary * TAX_HIGH_PERCENTAGE;
		}
		else if (fSalary <= TAX_LOW)
		{
			fTax = fSalary * TAX_LOW_PERCENTAGE;
		}
		else
		{
			fTax = fSalary * TAX_MIDDLE_PERCENTAGE;
		}
		printf("Your Tax is %f\n", fTax);
		fProfit = fSalary - fTax;
		printf("Your profit is %f\n", fProfit);
	}
	return 0;
}


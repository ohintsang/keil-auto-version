#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUILD_NUM_STR	"BUILD_NUMBER"
#define BUILD_DATE_STR	"BUILD_DATE"
#define MONTH_STR	"MM"
#define DAY_STR		"DD"
#define YEAR_STR	"YY"

int ConfigBuildDate(char *argv[], char *DATE_CODE)
{
	FILE *fp;
	char str[128];

	char *foundstr;
	int temp;
	int ret = 0;

    fp=fopen(argv[1], "r+");
	if(fp==NULL)
	{
		printf("error opening file %s. \n", argv[1]);
		exit(1);
	}

	while(!feof(fp))
	{
		if((foundstr=strstr(str, BUILD_DATE_STR)) != NULL) /* if string found */
		{
			sscanf(foundstr, "%*s \"%[^\t\n\"]s", DATE_CODE);

			if (DATE_CODE)
            {
                printf("Build Date found - %s[%d]\n", DATE_CODE, strlen(DATE_CODE));

                if (strcmp(DATE_CODE, __DATE__) == 0) //Date match, inc build number
                {
                    printf("Date match, inc build number\n");
                    ret = 1;
                }
                else
                {
                    printf("Date not match, reset build number\n");
                }
            }

			temp = ftell(fp) - strlen(foundstr)+strlen(BUILD_DATE_STR) ;
			fseek(fp, (long)temp, SEEK_SET);
			fprintf(fp,"	 \"%s\" \n", __DATE__);
			fflush(fp);
			fseek(fp, 0L, SEEK_END);
		}
		fgets(str, 127, fp);
	}
	fclose(fp);

	return ret;
}

int IncBuildNumber(char *argv[], int flag)
{
	FILE *fp;
	unsigned int number;
	char str[128], str2[64];
	char *foundstr;
	int temp;

    fp=fopen(argv[1], "r+");
	if(fp==NULL)
	{
		printf("error opening file %s. \n", argv[1]);
		exit(1);
	}

	*str = 0;
	while(!feof(fp))
	{
		if((foundstr=strstr(str, BUILD_NUM_STR)) != NULL) /* if string found */
		{
			sscanf(foundstr, "%*s %s", str2);
			if (flag)
            {
                number = atoi(str2);
                number++;
            }
            else
            {
                number = 0;
            }
			temp = ftell(fp) - strlen(foundstr)+strlen(BUILD_NUM_STR) ;
			fseek(fp, (long)temp, SEEK_SET);
			fprintf(fp,"	 %-4d\n", (int)number);
			fflush(fp);
			fseek(fp, 0L, SEEK_END);
		}
		fgets(str, 127, fp);
	}
	fclose(fp);

	return 0;
}


int ConfigVersionField(char *argv[], char val[], char keyword[])
{
	FILE *fp;
	char str[128];

	char *foundstr;
	int temp;
	int ret = 0;

    fp=fopen(argv[1], "r+");
	if(fp==NULL)
	{
		printf("error opening file %s. \n", argv[1]);
		exit(1);
	}

	while(!feof(fp))
	{
		if((foundstr=strstr(str, keyword)) != NULL) /* if string found */
		{
            // printf("keyword found\n");
			// printf("%s\n", foundstr);
			temp = ftell(fp) - strlen(foundstr)+strlen(keyword) ;
			fseek(fp, (long)temp, SEEK_SET);
			fprintf(fp,"	 0x%02s\n", val);
			fflush(fp);
			fseek(fp, 0L, SEEK_END);
		}
		fgets(str, 127, fp);
	}
	fclose(fp);

	return ret;
}


int ConfigVersionNumber(char *argv[], char *DATE_CODE)
{
	char *month, *date, *year, *saveptr;
	char YY[2] = "11";
	char MM[2] = "22";
	char DD[2] = "33";
    month = strtok_r(DATE_CODE, " \t\n", &saveptr);
    date = strtok_r(NULL, " \t\n", &saveptr);
    year = strtok_r(NULL, " \t\n", &saveptr);

    // printf("month[%d] = %s\n", strlen(month), month);
    // printf("date[%d] = %s\n", strlen(date), date);
    // printf("year[%d] = %s\n", strlen(year), year);

	strncpy(YY, year+2, 2);
    printf("YY[%d] = %s\n", strlen(YY), YY);
	ConfigVersionField(argv, YY, YEAR_STR);

	strcpy(DD, date);
    printf("DD[%d] = %s\n", strlen(DD), DD);
	ConfigVersionField(argv, DD, DAY_STR);

	if (0);
	else if (strncmp(month, "Jan", 3) == 0)
	{
		strcpy(MM, "01");
	}
	else if (strncmp(month, "Feb", 3) == 0)
	{
		strcpy(MM, "02");
	}
	else if (strncmp(month, "Mar", 3) == 0)
	{
		strcpy(MM, "03");
	}
	else if (strncmp(month, "Apr", 3) == 0)
	{
		strcpy(MM, "04");
	}
	else if (strncmp(month, "May", 3) == 0)
	{
		strcpy(MM, "05");
	}
	else if (strncmp(month, "Jun", 3) == 0)
	{
		strcpy(MM, "06");
	}
	else if (strncmp(month, "Jul", 3) == 0)
	{
		strcpy(MM, "07");
	}
	else if (strncmp(month, "Aug", 3) == 0)
	{
		strcpy(MM, "08");
	}
	else if (strncmp(month, "Sep", 3) == 0)
	{
		strcpy(MM, "09");
	}
	else if (strncmp(month, "Oct", 3) == 0)
	{
		strcpy(MM, "10");
	}
	else if (strncmp(month, "Nov", 3) == 0)
	{
		strcpy(MM, "11");
	}
	else if (strncmp(month, "Dec", 3) == 0)
	{
		strcpy(MM, "12");
	}
    printf("MM[%d] = %s\n", strlen(MM), MM);
	ConfigVersionField(argv, MM, MONTH_STR);

    return 0;
}


int main(int argc, char *argv[])
{
	if(argc <= 1)
	{	printf("Increment Build number, Ver: 1.0  by: D. Marten \n");
		printf("  Usage: incbld <filename) \n");
		printf("  where filename is name of file to search for string\n");
		printf("  BUILD_NUMBER (ususally contained in a headder file. \n");
		printf("  The number following will be incremented by 1 and saved. \n\n\n");
		exit(1);
	}
    char DATE_CODE[128] = "";
	int isDateMatch = ConfigBuildDate(argv, DATE_CODE);
    IncBuildNumber(argv, isDateMatch);

    // printf("DATECODE = %s\n", DATE_CODE);
    ConfigVersionNumber(argv, DATE_CODE);

	return(0);
}

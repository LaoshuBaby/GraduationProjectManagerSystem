#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
extern "C"
{
#include"sqlite\sqlite3.h"
};


#define W 800
#define H 450
#define MAXLEN 300
#define MAXNUM 100

#define ROLE_STU 1
#define ROLE_TEA 2
#define ROLE_CHA 3
#define PROGRESS1 1
#define PROGRESS2 2
#define PROGRESS3 3
#define PROGRESS4 4 
#define PROGRESS5 5
#define PROGRESS0 0

FILE *flog = fopen("log.txt", "w");
char CurrentID[13 + 1];
int CurrentRole = 0;
char CurrentItem[20 + 1];
char CurrentItemExtern[20 + 1];

int ItemNum = 0;
int ItemRestri = 5;
int ItemPage = 0;
int ItemCurrentNum = 1;
int ItemCurrentPage = 1;
int ItemTempCount = 0;
int Start = (ItemCurrentPage - 1)*ItemRestri-1;
int End = (ItemCurrentPage)*ItemRestri;
int flag_IsChairman = 0;
int flag_Process = 0;

//sql_CallBack_Func
int sql_CallBack_Func(void *notUsed, int argc, char **argv, char **azColName)
{
	int i;
	printf("%d\n", argc);
	for (i = 0; i < argc; ++i)
	{
		printf("%s=%s ", azColName[i], argv[i] ? argv[i] : "NULL");

	}
	printf("\nprint finished \n");
	return 0;
}

//sql_CallBack_Func_Login
int sql_CallBack_Func_Login(void *notUsed, int argc, char **argv, char **azColName)
{
	int flag_UserNotExist = 0;
	int LogPrint(int mode, char *LogString);
	int WindowStuMain(void);
	int WindowTeaMain(void);
	if (argc <= 1)
	{
		flag_UserNotExist = 1;
		char Msg_flag_UserNotExist[] = { "flag_UserNotExist" };
		LogPrint(1, Msg_flag_UserNotExist);
		return 0;
	}
	int i = 0;
	for (i = 0; i <= 13; ++i)
	{
		CurrentID[i] = argv[0][i];
	}
	CurrentID[13] = '\0';

	if (argv[0] != NULL)
	{
		if (argv[0][0] == 'S' || argv[2][1] == '1')
		{
			char Msg_UserIsStu[] = { "Msg_UserIsStu" };
			LogPrint(1, Msg_UserIsStu);
			WindowStuMain();
			return 0;
			//按说这里应该再查一次学生表里有没有这个用户，这里简化掉了
		}

		if (argv[0][0] == 'T' || argv[2][1] == '2')
		{
			char Msg_UserIsTea[] = { "Msg_UserIsTea" };
			LogPrint(1, Msg_UserIsTea);
			WindowTeaMain();
			return 0;
			//按说这里应该再查一次学生表里有没有这个用户，这里简化掉了
		}
	}
}


//sql_CallBack_Func_NewSubmit
int sql_CallBack_Func_NewSubmit(void *notUsed, int argc, char **argv, char **azColName)
{
	return 0;
}

//sql_CallBack_Func_InitItemNum
int sql_CallBack_Func_InitItemNum(void *notUsed, int argc, char **argv, char **azColName)
{
	ItemNum += 1;
	return 0;
}

int sql_CallBack_Func_Admin_StuL = 100 + 10;
int sql_CallBack_Func_Admin_StuU = 90 + 10;
int sql_CallBack_Func_Admin_StuDelta = 20;

//sql_CallBack_Func_Admin_Stu
int sql_CallBack_Func_Admin_Stu(void *notUsed, int argc, char **argv, char **azColName)
{
	void CharToTchar(const char * _char, TCHAR * tchar);
	ItemTempCount += 1;
	if (ItemTempCount >= Start && ItemTempCount <= End)
	{
		int i = 0;
		for (i = 0; i < argc; ++i)
		{
			printf("%s=%s ", azColName[i], argv[i] ? argv[i] : "NULL");

		}
		printf("\n");

		TCHAR ItemPrint_TCHAR[13 + 1];
		char ItemPrint_CHAR[13 + 1];
		for (i = 0; i < 13; ++i)
		{
			ItemPrint_CHAR[i] = argv[0][i];
		}
		ItemPrint_CHAR[13] = '\0';
		CharToTchar(ItemPrint_CHAR, ItemPrint_TCHAR);
		outtextxy(sql_CallBack_Func_Admin_StuL, sql_CallBack_Func_Admin_StuU+ sql_CallBack_Func_Admin_StuDelta*(ItemTempCount-ItemRestri*(ItemCurrentPage-1)-1), ItemPrint_TCHAR);
	}
	if (ItemTempCount == End)
	{
		printf("\n page %d print finished \n",ItemCurrentPage);
	}
	return 0;
}

int sql_CallBack_Func_Admin_TeaL = 440 + 10;
int sql_CallBack_Func_Admin_TeaU = 90 + 10;
int sql_CallBack_Func_Admin_TeaDelta = 20;

//sql_CallBack_Func_Admin_Tea
int sql_CallBack_Func_Admin_Tea(void *notUsed, int argc, char **argv, char **azColName)
{
	void CharToTchar(const char * _char, TCHAR * tchar);
	ItemTempCount += 1;
	if (ItemTempCount >= Start && ItemTempCount <= End)
	{
		int i = 0;
		for (i = 0; i < argc; ++i)
		{
			printf("%s=%s ", azColName[i], argv[i] ? argv[i] : "NULL");

		}
		printf("\n");

		TCHAR ItemPrint_TCHAR[13 + 1];
		char ItemPrint_CHAR[13 + 1];
		for (i = 0; i < 13; ++i)
		{
			ItemPrint_CHAR[i] = argv[0][i];
		}
		ItemPrint_CHAR[13] = '\0';
		CharToTchar(ItemPrint_CHAR, ItemPrint_TCHAR);
		outtextxy(sql_CallBack_Func_Admin_TeaL, sql_CallBack_Func_Admin_TeaU + sql_CallBack_Func_Admin_TeaDelta * (ItemTempCount - ItemRestri * (ItemCurrentPage - 1) - 1), ItemPrint_TCHAR);
	}
	if (ItemTempCount == End)
	{
		printf("\n page %d print finished \n", ItemCurrentPage);
	}
	return 0;
}

int sql_CallBack_Func_Admin_DelStuL = 75+80;
int sql_CallBack_Func_Admin_DelStuU = 120;
int sql_CallBack_Func_Admin_DelStuDelta = 30;

//sql_CallBack_Func_Admin_DelStu
int sql_CallBack_Func_Admin_DelStu(void *notUsed, int argc, char **argv, char **azColName)
{
	void CharToTchar(const char * _char, TCHAR * tchar);
	ItemTempCount += 1;
	if (ItemTempCount >= Start && ItemTempCount <= End)
	{
		int i = 0;
		for (i = 0; i < argc; ++i)
		{
			printf("%s=%s ", azColName[i], argv[i] ? argv[i] : "NULL");
		}
		printf("\n");

		TCHAR SNO_TCHAR[13 + 1];
		TCHAR SNAME_TCHAR[13 + 1];
		TCHAR SGENDER_TCHAR[13 + 1];
		TCHAR SMAJOR_TCHAR[13 + 1];
		TCHAR SCLASS_TCHAR[13 + 1];
		TCHAR STELE_TCHAR[13 + 1];
		TCHAR SEMAIL_TCHAR[13 + 1];
		TCHAR SOTHERS_TCHAR[13 + 1];

		char SNO_CHAR[13 + 1];
		char SNAME_CHAR[13 + 1];
		char SGENDER_CHAR[13 + 1];
		char SMAJOR_CHAR[13 + 1];
		char SCLASS_CHAR[13 + 1];
		char STELE_CHAR[13 + 1];
		char SEMAIL_CHAR[13 + 1];
		char SOTHERS_CHAR[13 + 1];

		for (i = 0; i < 13; ++i)
		{
			SNO_CHAR[i] = argv[0][i];
			SNAME_CHAR[i] = argv[1][i];
			SGENDER_CHAR[i] = argv[2][i];
			SMAJOR_CHAR[i] = argv[3][i];
			SCLASS_CHAR[i] = argv[4][i];
			STELE_CHAR[i] = argv[5][i];
			SEMAIL_CHAR[i] = argv[6][i];
			SOTHERS_CHAR[i] = argv[7][i];
			CurrentItem[i] = argv[0][i];
		}
		SNO_CHAR[13] = '\0';
		SNAME_CHAR[13] = '\0';
		SGENDER_CHAR[13] = '\0';
		SMAJOR_CHAR[13] = '\0';
		SCLASS_CHAR[13] = '\0';
		STELE_CHAR[13] = '\0';
		SEMAIL_CHAR[13] = '\0';
		SOTHERS_CHAR[13] = '\0';
		CurrentItem[13] = '\0';
		CharToTchar(SNO_CHAR, SNO_TCHAR);
		CharToTchar(SNAME_CHAR, SNAME_TCHAR);
		CharToTchar(SGENDER_CHAR, SGENDER_TCHAR);
		CharToTchar(SMAJOR_CHAR, SMAJOR_TCHAR);
		CharToTchar(SCLASS_CHAR, SCLASS_TCHAR);
		CharToTchar(STELE_CHAR, STELE_TCHAR);
		CharToTchar(SEMAIL_CHAR, SEMAIL_TCHAR);
		CharToTchar(SOTHERS_CHAR, SOTHERS_TCHAR);
		outtextxy(sql_CallBack_Func_Admin_DelStuL, sql_CallBack_Func_Admin_DelStuU, SNO_TCHAR);
		sql_CallBack_Func_Admin_DelStuU += sql_CallBack_Func_Admin_DelStuDelta;
		outtextxy(sql_CallBack_Func_Admin_DelStuL, sql_CallBack_Func_Admin_DelStuU, SNAME_TCHAR);
		sql_CallBack_Func_Admin_DelStuU += sql_CallBack_Func_Admin_DelStuDelta;
		outtextxy(sql_CallBack_Func_Admin_DelStuL, sql_CallBack_Func_Admin_DelStuU, SGENDER_TCHAR);
		sql_CallBack_Func_Admin_DelStuU += sql_CallBack_Func_Admin_DelStuDelta;
		outtextxy(sql_CallBack_Func_Admin_DelStuL, sql_CallBack_Func_Admin_DelStuU, SMAJOR_TCHAR);
		sql_CallBack_Func_Admin_DelStuU += sql_CallBack_Func_Admin_DelStuDelta;
		outtextxy(sql_CallBack_Func_Admin_DelStuL, sql_CallBack_Func_Admin_DelStuU, SCLASS_TCHAR);
		sql_CallBack_Func_Admin_DelStuU += sql_CallBack_Func_Admin_DelStuDelta;
		outtextxy(sql_CallBack_Func_Admin_DelStuL, sql_CallBack_Func_Admin_DelStuU, STELE_TCHAR);
		sql_CallBack_Func_Admin_DelStuU += sql_CallBack_Func_Admin_DelStuDelta;
		outtextxy(sql_CallBack_Func_Admin_DelStuL, sql_CallBack_Func_Admin_DelStuU, SEMAIL_TCHAR);
		sql_CallBack_Func_Admin_DelStuU += sql_CallBack_Func_Admin_DelStuDelta;
		outtextxy(sql_CallBack_Func_Admin_DelStuL, sql_CallBack_Func_Admin_DelStuU, SOTHERS_TCHAR);
		sql_CallBack_Func_Admin_DelStuU = 120;
	}
	if (ItemTempCount == End)
	{
		printf("\n page %d print finished \n", ItemCurrentPage);
	}
	return 0;
}

int sql_CallBack_Func_Admin_DelTeaL = 75 + 80;
int sql_CallBack_Func_Admin_DelTeaU = 120;
int sql_CallBack_Func_Admin_DelTeaDelta = 30;

//sql_CallBack_Func_Admin_DelTea
int sql_CallBack_Func_Admin_DelTea(void *notUsed, int argc, char **argv, char **azColName)
{
	void CharToTchar(const char * _char, TCHAR * tchar);
	ItemTempCount += 1;
	if (ItemTempCount >= Start && ItemTempCount <= End)
	{
		int i = 0;
		for (i = 0; i < argc; ++i)
		{
			printf("%s=%s ", azColName[i], argv[i] ? argv[i] : "NULL");
		}
		printf("\n");

		TCHAR TNO_TCHAR[13 + 1];
		TCHAR TNAME_TCHAR[13 + 1];
		TCHAR TGENDER_TCHAR[13 + 1];
		TCHAR TTITLE_TCHAR[13 + 1];
		TCHAR TFIELD_TCHAR[13 + 1];
		TCHAR TTELE_TCHAR[13 + 1];
		TCHAR TEMAIL_TCHAR[13 + 1];
		TCHAR TCHAIRMAN_TCHAR[13 + 1];

		char TNO_CHAR[13 + 1];
		char TNAME_CHAR[13 + 1];
		char TGENDER_CHAR[13 + 1];
		char TTITLE_CHAR[13 + 1];
		char TFIELD_CHAR[13 + 1];
		char TTELE_CHAR[13 + 1];
		char TEMAIL_CHAR[13 + 1];
		char TCHAIRMAN_CHAR[13 + 1];

		for (i = 0; i < 13; ++i)
		{
			TNO_CHAR[i] = argv[0][i];
			TNAME_CHAR[i] = argv[1][i];
			TGENDER_CHAR[i] = argv[2][i];
			TTITLE_CHAR[i] = argv[3][i];
			TFIELD_CHAR[i] = argv[4][i];
			TTELE_CHAR[i] = argv[5][i];
			TEMAIL_CHAR[i] = argv[6][i];
			TCHAIRMAN_CHAR[i] = argv[7][i];
			CurrentItem[i] = argv[0][i];
		}
		TNO_CHAR[13] = '\0';
		TNAME_CHAR[13] = '\0';
		TGENDER_CHAR[13] = '\0';
		TTITLE_CHAR[13] = '\0';
		TFIELD_CHAR[13] = '\0';
		TTELE_CHAR[13] = '\0';
		TEMAIL_CHAR[13] = '\0';
		TCHAIRMAN_CHAR[13] = '\0';
		CurrentItem[13] = '\0';
		CharToTchar(TNO_CHAR, TNO_TCHAR);
		CharToTchar(TNAME_CHAR, TNAME_TCHAR);
		CharToTchar(TGENDER_CHAR, TGENDER_TCHAR);
		CharToTchar(TTITLE_CHAR, TTITLE_TCHAR);
		CharToTchar(TFIELD_CHAR, TFIELD_TCHAR);
		CharToTchar(TTELE_CHAR, TTELE_TCHAR);
		CharToTchar(TEMAIL_CHAR, TEMAIL_TCHAR);
		CharToTchar(TCHAIRMAN_CHAR, TCHAIRMAN_TCHAR);
		TCHAR IsChairman[] = { TEXT("是") };
		TCHAR NotChairman[] = { TEXT("否") };
		outtextxy(sql_CallBack_Func_Admin_DelTeaL, sql_CallBack_Func_Admin_DelTeaU, TNO_TCHAR);
		sql_CallBack_Func_Admin_DelTeaU += sql_CallBack_Func_Admin_DelTeaDelta;
		outtextxy(sql_CallBack_Func_Admin_DelTeaL, sql_CallBack_Func_Admin_DelTeaU, TNAME_TCHAR);
		sql_CallBack_Func_Admin_DelTeaU += sql_CallBack_Func_Admin_DelTeaDelta;
		outtextxy(sql_CallBack_Func_Admin_DelTeaL, sql_CallBack_Func_Admin_DelTeaU, TGENDER_TCHAR);
		sql_CallBack_Func_Admin_DelTeaU += sql_CallBack_Func_Admin_DelTeaDelta;
		outtextxy(sql_CallBack_Func_Admin_DelTeaL, sql_CallBack_Func_Admin_DelTeaU, TTITLE_TCHAR);
		sql_CallBack_Func_Admin_DelTeaU += sql_CallBack_Func_Admin_DelTeaDelta;
		outtextxy(sql_CallBack_Func_Admin_DelTeaL, sql_CallBack_Func_Admin_DelTeaU, TFIELD_TCHAR);
		sql_CallBack_Func_Admin_DelTeaU += sql_CallBack_Func_Admin_DelTeaDelta;
		outtextxy(sql_CallBack_Func_Admin_DelTeaL, sql_CallBack_Func_Admin_DelTeaU, TTELE_TCHAR);
		sql_CallBack_Func_Admin_DelTeaU += sql_CallBack_Func_Admin_DelTeaDelta;
		outtextxy(sql_CallBack_Func_Admin_DelTeaL, sql_CallBack_Func_Admin_DelTeaU, TEMAIL_TCHAR);
		sql_CallBack_Func_Admin_DelTeaU += sql_CallBack_Func_Admin_DelTeaDelta;
		if (TCHAIRMAN_CHAR[0] == 1 || TCHAIRMAN_CHAR[0] == '1')
		{
			outtextxy(sql_CallBack_Func_Admin_DelTeaL, sql_CallBack_Func_Admin_DelTeaU, IsChairman);
		}
		else if (TCHAIRMAN_CHAR[0] == 0 || TCHAIRMAN_CHAR[0] == '0')
		{
			outtextxy(sql_CallBack_Func_Admin_DelTeaL, sql_CallBack_Func_Admin_DelTeaU, NotChairman);
		}
		else
		{
			outtextxy(sql_CallBack_Func_Admin_DelTeaL, sql_CallBack_Func_Admin_DelTeaU, TCHAIRMAN_TCHAR);
		}
		sql_CallBack_Func_Admin_DelTeaU = 120;
	}
	if (ItemTempCount == End)
	{
		printf("\n page %d print finished \n", ItemCurrentPage);
	}
	return 0;
}

int sql_CallBack_Func_Stu_SelectProjectL = 75 + 80;
int sql_CallBack_Func_Stu_SelectProjectU = 120;
int sql_CallBack_Func_Stu_SelectProjectDelta = 30;

//sql_CallBack_Func_Stu_SelectProject
int sql_CallBack_Func_Stu_SelectProject(void *notUsed, int argc, char **argv, char **azColName)
{
	void CharToTchar(const char * _char, TCHAR * tchar);
	ItemTempCount += 1;
	if (ItemTempCount >= Start && ItemTempCount <= End)
	{
		int i = 0;
		for (i = 0; i < argc; ++i)
		{
			printf("%s=%s ", azColName[i], argv[i] ? argv[i] : "NULL");
		}
		printf("\n");

		TCHAR PNO_TCHAR[13 + 1];
		TCHAR PNAME_TCHAR[13 + 1];
		TCHAR PMAJOR_TCHAR[13 + 1];
		TCHAR TNO_TCHAR[13 + 1];
		TCHAR PABSTRUCT_TCHAR[13 + 1];

		char PNO_CHAR[13 + 1];
		char PNAME_CHAR[13 + 1];
		char PMAJOR_CHAR[13 + 1];
		char TNO_CHAR[13 + 1];
		char PABSTRUCT_CHAR[13 + 1];

		for (i = 0; i < 13; ++i)
		{
			PNO_CHAR[i] = argv[0][i];
			PNAME_CHAR[i] = argv[1][i];
			PMAJOR_CHAR[i] = argv[2][i];
			TNO_CHAR[i] = argv[3][i];
			PABSTRUCT_CHAR[i] = argv[4][i];
			CurrentItem[i] = argv[0][i];
			CurrentItemExtern[i] = argv[3][i];
		}
		PNO_CHAR[13] = '\0';
		PNAME_CHAR[13] = '\0';
		PMAJOR_CHAR[13] = '\0';
		TNO_CHAR[13] = '\0';
		PABSTRUCT_CHAR[13] = '\0';
		CurrentItem[13] = '\0';
		CurrentItemExtern[13] = '\0';
		CharToTchar(PNO_CHAR, PNO_TCHAR);
		CharToTchar(PNAME_CHAR, PNAME_TCHAR);
		CharToTchar(PMAJOR_CHAR, PMAJOR_TCHAR);
		CharToTchar(TNO_CHAR, TNO_TCHAR);
		CharToTchar(PABSTRUCT_CHAR, PABSTRUCT_TCHAR);
		outtextxy(sql_CallBack_Func_Stu_SelectProjectL, sql_CallBack_Func_Stu_SelectProjectU, PNO_TCHAR);
		sql_CallBack_Func_Stu_SelectProjectU += sql_CallBack_Func_Stu_SelectProjectDelta;
		outtextxy(sql_CallBack_Func_Stu_SelectProjectL, sql_CallBack_Func_Stu_SelectProjectU, PNAME_TCHAR);
		sql_CallBack_Func_Stu_SelectProjectU += sql_CallBack_Func_Stu_SelectProjectDelta;
		outtextxy(sql_CallBack_Func_Stu_SelectProjectL, sql_CallBack_Func_Stu_SelectProjectU, PMAJOR_TCHAR);
		sql_CallBack_Func_Stu_SelectProjectU += sql_CallBack_Func_Stu_SelectProjectDelta;
		outtextxy(sql_CallBack_Func_Stu_SelectProjectL, sql_CallBack_Func_Stu_SelectProjectU, TNO_TCHAR);
		sql_CallBack_Func_Stu_SelectProjectU += sql_CallBack_Func_Stu_SelectProjectDelta;
		outtextxy(sql_CallBack_Func_Stu_SelectProjectL, sql_CallBack_Func_Stu_SelectProjectU, PABSTRUCT_TCHAR);
		sql_CallBack_Func_Stu_SelectProjectU = 120;
	}
	if (ItemTempCount == End)
	{
		printf("\n page %d print finished \n", ItemCurrentPage);
	}
	return 0;
}

int sql_CallBack_Func_Stu_UploadProjectL = 120 + 75;
int sql_CallBack_Func_Stu_UploadProjectU = 150;
int sql_CallBack_Func_Stu_UploadProjectDelta = 30;

//sql_CallBack_Func_Stu_UploadProject
int sql_CallBack_Func_Stu_UploadProject(void *notUsed, int argc, char **argv, char **azColName)
{
	void CharToTchar(const char * _char, TCHAR * tchar);
	ItemTempCount += 1;
	if (ItemTempCount >= Start && ItemTempCount <= End)
	{
		int i = 0;
		for (i = 0; i < argc; ++i)
		{
			printf("%s=%s ", azColName[i], argv[i] ? argv[i] : "NULL");
		}
		printf("\n");

		TCHAR PNO_TCHAR[13 + 1];
		TCHAR PNAME_TCHAR[13 + 1];
		TCHAR PMAJOR_TCHAR[13 + 1];
		TCHAR TNO_TCHAR[13 + 1];
		TCHAR PABSTRUCT_TCHAR[13 + 1];

		char PNO_CHAR[13 + 1];
		char PNAME_CHAR[13 + 1];
		char PMAJOR_CHAR[13 + 1];
		char TNO_CHAR[13 + 1];
		char PABSTRUCT_CHAR[13 + 1];

		for (i = 0; i < 13; ++i)
		{
			PNO_CHAR[i] = argv[0][i];
			PNAME_CHAR[i] = argv[1][i];
			PMAJOR_CHAR[i] = argv[2][i];
			TNO_CHAR[i] = argv[3][i];
			PABSTRUCT_CHAR[i] = argv[4][i];
		}
		PNO_CHAR[13] = '\0';
		PNAME_CHAR[13] = '\0';
		PMAJOR_CHAR[13] = '\0';
		TNO_CHAR[13] = '\0';
		PABSTRUCT_CHAR[13] = '\0';
		CharToTchar(PNO_CHAR, PNO_TCHAR);
		CharToTchar(PNAME_CHAR, PNAME_TCHAR);
		CharToTchar(PMAJOR_CHAR, PMAJOR_TCHAR);
		CharToTchar(TNO_CHAR, TNO_TCHAR);
		CharToTchar(PABSTRUCT_CHAR, PABSTRUCT_TCHAR);
		outtextxy(sql_CallBack_Func_Stu_UploadProjectL, sql_CallBack_Func_Stu_UploadProjectU, PNO_TCHAR);
		sql_CallBack_Func_Stu_UploadProjectU += sql_CallBack_Func_Stu_UploadProjectDelta;
		outtextxy(sql_CallBack_Func_Stu_UploadProjectL, sql_CallBack_Func_Stu_UploadProjectU, PNAME_TCHAR);
		sql_CallBack_Func_Stu_UploadProjectU += sql_CallBack_Func_Stu_UploadProjectDelta;
		outtextxy(sql_CallBack_Func_Stu_UploadProjectL, sql_CallBack_Func_Stu_UploadProjectU, PMAJOR_TCHAR);
		sql_CallBack_Func_Stu_UploadProjectU += sql_CallBack_Func_Stu_UploadProjectDelta;
		outtextxy(sql_CallBack_Func_Stu_UploadProjectL, sql_CallBack_Func_Stu_UploadProjectU, TNO_TCHAR);
		sql_CallBack_Func_Stu_UploadProjectU += sql_CallBack_Func_Stu_UploadProjectDelta;
		outtextxy(sql_CallBack_Func_Stu_UploadProjectL, sql_CallBack_Func_Stu_UploadProjectU, PABSTRUCT_TCHAR);
		sql_CallBack_Func_Stu_UploadProjectU = 120;

		flag_Process = argv[5][0] - '0';
	}
	if (ItemTempCount == End)
	{
		printf("\n page %d print finished \n", ItemCurrentPage);
	}
	return 0;
}

int sql_CallBack_Func_Chairman_ReviewProjectL = 75 + 80;
int sql_CallBack_Func_Chairman_ReviewProjectU = 120;
int sql_CallBack_Func_Chairman_ReviewProjectDelta = 30;

//sql_CallBack_Func_Chairman_ReviewProject
int sql_CallBack_Func_Chairman_ReviewProject(void *notUsed, int argc, char **argv, char **azColName)
{
	void CharToTchar(const char * _char, TCHAR * tchar);
	ItemTempCount += 1;
	if (ItemTempCount >= Start && ItemTempCount <= End)
	{
		int i = 0;
		for (i = 0; i < argc; ++i)
		{
			printf("%s=%s ", azColName[i], argv[i] ? argv[i] : "NULL");
		}
		printf("\n");

		TCHAR PNO_TCHAR[13 + 1];
		TCHAR PNAME_TCHAR[13 + 1];
		TCHAR PMAJOR_TCHAR[13 + 1];
		TCHAR TNO_TCHAR[13 + 1];
		TCHAR PABSTRUCT_TCHAR[13 + 1];
		TCHAR SNO_TCHAR[13 + 1];

		char PNO_CHAR[13 + 1];
		char PNAME_CHAR[13 + 1];
		char PMAJOR_CHAR[13 + 1];
		char TNO_CHAR[13 + 1];
		char PABSTRUCT_CHAR[13 + 1];
		char SNO_CHAR[13 + 1];

		for (i = 0; i < 13; ++i)
		{
			PNO_CHAR[i] = argv[0][i];
			PNAME_CHAR[i] = argv[1][i];
			PMAJOR_CHAR[i] = argv[2][i];
			TNO_CHAR[i] = argv[3][i];
			PABSTRUCT_CHAR[i] = argv[4][i];
			SNO_CHAR[i] = argv[5][i];
			CurrentItem[i] = argv[0][i];
			CurrentItemExtern[i] = argv[3][i];
		}
		PNO_CHAR[13] = '\0';
		PNAME_CHAR[13] = '\0';
		PMAJOR_CHAR[13] = '\0';
		TNO_CHAR[13] = '\0';
		PABSTRUCT_CHAR[13] = '\0';
		SNO_CHAR[13] = '\0';
		CurrentItem[13] = '\0';
		CurrentItemExtern[13] = '\0';
		CharToTchar(PNO_CHAR, PNO_TCHAR);
		CharToTchar(PNAME_CHAR, PNAME_TCHAR);
		CharToTchar(PMAJOR_CHAR, PMAJOR_TCHAR);
		CharToTchar(TNO_CHAR, TNO_TCHAR);
		CharToTchar(PABSTRUCT_CHAR, PABSTRUCT_TCHAR);
		CharToTchar(SNO_CHAR, SNO_TCHAR);
		outtextxy(sql_CallBack_Func_Chairman_ReviewProjectL, sql_CallBack_Func_Chairman_ReviewProjectU, PNO_TCHAR);
		sql_CallBack_Func_Chairman_ReviewProjectU += sql_CallBack_Func_Chairman_ReviewProjectDelta;
		outtextxy(sql_CallBack_Func_Chairman_ReviewProjectL, sql_CallBack_Func_Chairman_ReviewProjectU, PNAME_TCHAR);
		sql_CallBack_Func_Chairman_ReviewProjectU += sql_CallBack_Func_Chairman_ReviewProjectDelta;
		outtextxy(sql_CallBack_Func_Chairman_ReviewProjectL, sql_CallBack_Func_Chairman_ReviewProjectU, PMAJOR_TCHAR);
		sql_CallBack_Func_Chairman_ReviewProjectU += sql_CallBack_Func_Chairman_ReviewProjectDelta;
		outtextxy(sql_CallBack_Func_Chairman_ReviewProjectL, sql_CallBack_Func_Chairman_ReviewProjectU, TNO_TCHAR);
		sql_CallBack_Func_Chairman_ReviewProjectU += sql_CallBack_Func_Chairman_ReviewProjectDelta;
		outtextxy(sql_CallBack_Func_Chairman_ReviewProjectL, sql_CallBack_Func_Chairman_ReviewProjectU, PABSTRUCT_TCHAR);
		sql_CallBack_Func_Chairman_ReviewProjectU += sql_CallBack_Func_Chairman_ReviewProjectDelta;
		outtextxy(sql_CallBack_Func_Chairman_ReviewProjectL, sql_CallBack_Func_Chairman_ReviewProjectU, SNO_TCHAR);
		sql_CallBack_Func_Chairman_ReviewProjectU = 120;
	}
	if (ItemTempCount == End)
	{
		printf("\n page %d print finished \n", ItemCurrentPage);
	}
	return 0;
}

//sql_CallBack_Func_IsChairman
int sql_CallBack_Func_IsChairman(void *notUsed, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i < argc; ++i)
	{
		printf("%s=%s ", azColName[i], argv[i] ? argv[i] : "NULL");

	}
	if (argv[0][0] == 1 || argv[0][0] == '1')
	{
		printf("IsChairman\n");
		flag_IsChairman = 1;
		return 1;
	}
	if (argv[0][0] == 0 || argv[0][0] == '0')
	{
		printf("NotChairman\n");
		flag_IsChairman = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

//Sqlite-open
sqlite3 *DB;
char *errmsg = 0;
const char db_file[] = "GraduationProject.db";
int exec = sqlite3_open(db_file, &DB);

//TimeStampPrint
#define YEAR 0
#define MONTH 1
#define DAY 2
#define HOUR 3
#define MINITE 4
#define SECOND 5
int TimestampPrint(int mode)
{
	//如果需要写入flog请使mode==0;
	//Init
	int dayOfMonth[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 };
	float timeZone = 8.0;
	int timeBuffer[6];
	long timeStampUnix = time(NULL);//BUGsample：1439762642
	//Compute
	int leapYear = 0;
	unsigned char i = 0;
	long days = (timeStampUnix + 3600 * timeZone) / 86400 + 1;//计算时间戳的天数
	int year4 = (int)(days / 1461);//四年周期数
	int remain = (int)(days % 1461);//不足四年周期剩下的天数
	timeBuffer[YEAR] = 1970 + year4 * 4;//得到时间戳范围内满足四年周期的年数
	//剩下的天数小于一年@365不处理
	if (remain < 730)
	{
		timeBuffer[YEAR]++;
		remain -= 365;//第三年@365+365+365
	}
	else if (remain < 1095)
	{
		timeBuffer[YEAR] += 2;
		remain -= 730;//第四年
	}
	else {
		timeBuffer[YEAR] += 3;
		remain -= 1095;
		leapYear = 1;
	}

	//剩下一年内的天数 用12个月天数循环减
	//直到剩下天数<=0 则找到对应月份
	//剩下天数则为日期
	for (i = 0; i < 12; i++) 
	{
		//year4复用于暂存每月天数
		//闰年2月补一天
		year4 = (i == 1 && leapYear) ? (dayOfMonth[i] + 1) : dayOfMonth[i];
		days = remain - year4;//days复用于暂存减去当前月份天数的剩余天数
		if (days <= 0) 
		{
			timeBuffer[MONTH] = i + 1;
			if (days == 0) 
			{
				timeBuffer[DAY] = year4;
			}
			else 
			{
				timeBuffer[DAY] = remain;
			}
			break;
		}
		remain = (int)days;
	}
	timeBuffer[SECOND] = (int)(timeStampUnix % 60);
	timeBuffer[MINITE] = (int)(timeStampUnix / 60 % 60);
	while (timeStampUnix > 86400) 
	{
		timeStampUnix -= 86400;
	}

	if (timeStampUnix == 86400) 
	{
		timeBuffer[HOUR] = 0;
	}
	else 
	{
		timeBuffer[HOUR] = (int)(timeStampUnix / 3600) + timeZone;
	}
	if (timeBuffer[HOUR] >= 24) 
	{
		timeBuffer[HOUR] -= 24;
	}
	//Output
	if (mode == 0)
	{
		fprintf(flog,"%d-%d-%d %d:%d:%d", timeBuffer[0], timeBuffer[1], timeBuffer[2], timeBuffer[3], timeBuffer[4], timeBuffer[5]);
	}
	else
	{
		printf("%d-%d-%d %d:%d:%d", timeBuffer[0], timeBuffer[1], timeBuffer[2], timeBuffer[3], timeBuffer[4], timeBuffer[5]);
	}
	return 0;
}

//LogPrint
#define INFO 1
#define DEBUG 0
#define WARN 2
#define ERROR 3
int LogPrint(int mode, char *LogString)
{
	printf("[");
	fprintf(flog,"[");
	TimestampPrint(1);
	TimestampPrint(0);
	printf("] ");
	fprintf(flog, "] ");
	if (mode == INFO)
	{
		printf("[INFO]");
		fprintf(flog, "[INFO]");
	}
	else if (mode == DEBUG)
	{
		printf("[DEBUG]");
		fprintf(flog, "[DEBUG]");
	}
	else if (mode == WARN)
	{
		printf("[WARN]");
		fprintf(flog, "[WARN]");
	}
	else if (mode == ERROR)
	{
		printf("[ERROR]");
		fprintf(flog, "[ERROR]");
	}
	printf(" %s\n",LogString);
	fprintf(flog, " %s\n", LogString);
	return 0;
}

//TCHAR2char   
void TcharToChar(const TCHAR * tchar, char * _char)
{
	int iLength;
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);//获取字节长度 
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);//将tchar值赋给_char    
}

//char2TCHAR
void CharToTchar(const char * _char, TCHAR * tchar)
{
	int iLength;

	iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
}

int WindowLogin(void)
{
	int WindowAdmin(void);
	cleardevice();
	FlushMouseMsgBuffer();
	setcliprgn(NULL);

	char Msg_initloginwindow[] = { "initloginwindow" };
	LogPrint(INFO, Msg_initloginwindow);

	TCHAR WindowLoginSlogan[] = { TEXT("欢迎使用毕业设计管理系统") };
	outtextxy(300, 50, WindowLoginSlogan);

	TCHAR WindowLoginUsername[] = { TEXT("用户名") };
	TCHAR WindowLoginPassword[] = { TEXT("密码") };
	TCHAR WindowLoginLogin[] = { TEXT("登录系统") };
	TCHAR WindowLoginForget[] = { TEXT("忘记密码") };
	TCHAR WindowLoginIDNotCorrect[] = { TEXT("用户名或密码不正确") };
	TCHAR WindowLoginAskyouradmin[] = { TEXT("请联系系统管理员重置") };

	//模拟按钮-登录系统
	int WindowLoginLoginL = 250;
	int WindowLoginLoginU = 300;
	int WindowLoginLoginR = WindowLoginLoginL + 125;
	int WindowLoginLoginD = WindowLoginLoginU + 50;
	rectangle(WindowLoginLoginL, WindowLoginLoginU, WindowLoginLoginR, WindowLoginLoginD);
	outtextxy(WindowLoginLoginL+30, WindowLoginLoginU+16, WindowLoginLogin);
	//模拟按钮-忘记密码
	int WindowLoginForgetL = 425;
	int WindowLoginForgetU = 300;
	int WindowLoginForgetR = WindowLoginForgetL + 125;
	int WindowLoginForgetD = WindowLoginForgetU + 50;
	rectangle(WindowLoginForgetL, WindowLoginForgetU, WindowLoginForgetR, WindowLoginForgetD);
	outtextxy(WindowLoginForgetL+30, WindowLoginForgetU+16, WindowLoginForget);

	//输入框-用户名
	int WindowLoginUsernameL = 350;
	int WindowLoginUsernameU = 150;
	int WindowLoginUsernameR = WindowLoginUsernameL + 200;
	int WindowLoginUsernameD = WindowLoginUsernameU + 20;
	rectangle(WindowLoginUsernameL, WindowLoginUsernameU, WindowLoginUsernameR, WindowLoginUsernameD);
	outtextxy(250, 150, WindowLoginUsername);
	//输入框-密码
	int WindowLoginPasswordL = 350;
	int WindowLoginPasswordU = 200;
	int WindowLoginPasswordR = WindowLoginPasswordL + 200;
	int WindowLoginPasswordD = WindowLoginPasswordU + 20;
	rectangle(WindowLoginPasswordL, WindowLoginPasswordU, WindowLoginPasswordR, WindowLoginPasswordD);
	outtextxy(250, 200, WindowLoginPassword);

	HRGN LoginNotice = CreateRectRgn(0, WindowLoginLoginD+1, W, H);
	HRGN LoginUsername = CreateRectRgn(WindowLoginUsernameL+1, WindowLoginUsernameU+1, WindowLoginUsernameR-1, WindowLoginUsernameD-1);
	HRGN LoginPassword = CreateRectRgn(WindowLoginPasswordL+1, WindowLoginPasswordU+1, WindowLoginPasswordR-1, WindowLoginPasswordD-1);

	TCHAR Username_TCHAR[13+1] = { 0 };
	TCHAR Password_TCHAR[13+1] = { 0 };
	char Username_CHAR[13 + 1];
	char Password_CHAR[13 + 1];

	MOUSEMSG mouselogin;
	while (true)
	{
		mouselogin = GetMouseMsg();
		switch (mouselogin.uMsg)
		{
		case WM_LBUTTONDOWN:
		{
			if (mouselogin.x >= WindowLoginLoginL && mouselogin.x <= WindowLoginLoginR && mouselogin.y >= WindowLoginLoginU && mouselogin.y <= WindowLoginLoginD)
			{//LoginButton
				setcliprgn(LoginNotice);
				clearcliprgn();//用于接下来的提示
				setcliprgn(NULL);
				Sleep(500);
				setcliprgn(LoginUsername);
				clearcliprgn();
				setcliprgn(NULL);
				setcliprgn(LoginPassword);
				clearcliprgn();
				setcliprgn(NULL);
				char Msg_login[] = { "request a login" };
				LogPrint(INFO, Msg_login);
				//登录检验部分
				char Username_ADMIN[] = { "admin" };
				char Password_ADMIN[] = { "123456" };
				TcharToChar(Username_TCHAR, Username_CHAR);
				TcharToChar(Password_TCHAR, Password_CHAR);
				//首先检验是否是管理员账号
				if (strcmp(Username_ADMIN, Username_CHAR) == 0 && strcmp(Password_ADMIN, Password_CHAR) == 0)
				{
					WindowAdmin();
				}
				//其他的应该去sqlite查询用户名密码是否正确
				else
				{
					//先进行防注入预处理
					int i = 0;
					int flag_Injection = 0;
					char Msg_sqliteInjected[] = { "Msg_sqliteInjected" };
					for (i = 0; i < 13; ++i)
					{
						if (Username_CHAR[i] == ';' || Username_CHAR[i] == '#' || Password_CHAR[i] == ';' || Password_CHAR[i] == '#')
						{
							flag_Injection = 1;
							LogPrint(WARN, Msg_sqliteInjected);
							break;
						}
					}
					if (flag_Injection == 1)
					{
						outtextxy(WindowLoginLoginL - 10, WindowLoginLoginU + 65, WindowLoginIDNotCorrect);
					}
					else//无注入风险
					{
						//组装SQL语句——虽然有风险但已经进行过检查了，因此后续可以直接拼装
						char sql_WindowLoginP1[MAXLEN] = { "SELECT * FROM USER WHERE(UID=='" };
						char sql_WindowLoginP2[MAXLEN] = { "'AND UPASSWORD=='" };
						char sql_WindowLoginP3[MAXLEN] = { "');" };
						//char sql_WindowLoginPX[MAXLEN] = { "0" };
						strcat(sql_WindowLoginP1, Username_CHAR);
						strcat(sql_WindowLoginP1, sql_WindowLoginP2);
						strcat(sql_WindowLoginP1, Password_CHAR);
						strcat(sql_WindowLoginP1, sql_WindowLoginP3);
						const char *sql_WindowLogin = sql_WindowLoginP1;
						LogPrint(DEBUG, (char*)sql_WindowLogin);
						exec = sqlite3_exec(DB, sql_WindowLogin, sql_CallBack_Func_Login, 0, &errmsg);
						//接下来的判定在回调函数中进行
						//如果查询出错，就会回到这里来	
						if (exec != SQLITE_OK)//SQLITE_ABORT
						{
							char Msg_sqliteFailedToSelectLogin[] = { "FAILED TO SELECT USER WHEN LOGIN" };
							LogPrint(ERROR, Msg_sqliteFailedToSelectLogin);
							LogPrint(ERROR, errmsg);
						}
						else//SQLITE_OK
						{
							char Msg_sqliteIsOpen[] = { "SUCCEED IN SELECT USER WHEN LOGIN" };
							LogPrint(INFO, Msg_sqliteIsOpen);
						}
						//如果没有查到，就会回到这里来，都是输入错误
						outtextxy(WindowLoginLoginL - 10, WindowLoginLoginU + 65, WindowLoginIDNotCorrect);
						char Msg_IDnotCorrect[] = { "Msg_IDnotCorrect" };
						LogPrint(INFO, Msg_IDnotCorrect);
					}
				}


			}
			else if (mouselogin.x >= WindowLoginForgetL && mouselogin.x <= WindowLoginForgetR && mouselogin.y >= WindowLoginForgetU && mouselogin.y <= WindowLoginForgetD)
			{//Forgetbutton
				setcliprgn(LoginNotice);
				clearcliprgn();//用于接下来的提示
				setcliprgn(NULL);
				Sleep(500);
				outtextxy(WindowLoginForgetL - 20, WindowLoginForgetU + 65, WindowLoginAskyouradmin);
				char Msg_forgetten[] = { "forgot password" };
				LogPrint(INFO, Msg_forgetten);
			}
			else if (mouselogin.x >= WindowLoginUsernameL && mouselogin.x <= WindowLoginUsernameR && mouselogin.y >= WindowLoginUsernameU && mouselogin.y <= WindowLoginUsernameD)
			{//Username
				setcliprgn(LoginNotice);
				clearcliprgn();//用于接下来的提示
				setcliprgn(NULL);
				Sleep(500);
				setcliprgn(LoginUsername);
				clearcliprgn();
				setcliprgn(NULL);
				InputBox(Username_TCHAR, 13 + 1, TEXT("请输入用户名(不超过13个字符)"));
				outtextxy(WindowLoginUsernameL+1, WindowLoginUsernameU+2, Username_TCHAR);
				char Msg_UsernameInputted[] = { "Msg_UsernameInputted" };
				LogPrint(INFO, Msg_UsernameInputted);
				TcharToChar(Username_TCHAR, Username_CHAR);
				LogPrint(DEBUG, Username_CHAR);

			}
			else if (mouselogin.x >= WindowLoginPasswordL && mouselogin.x <= WindowLoginPasswordR && mouselogin.y >= WindowLoginPasswordU && mouselogin.y <= WindowLoginPasswordD)
			{//Password
				setcliprgn(LoginNotice);
				clearcliprgn();//用于接下来的提示
				setcliprgn(NULL);
				Sleep(500);
				setcliprgn(LoginPassword);
				clearcliprgn();
				setcliprgn(NULL);
				InputBox(Password_TCHAR, 13 + 1, TEXT("请输入密码(不超过13个字符)"));
				outtextxy(WindowLoginPasswordL + 1, WindowLoginPasswordU + 2, Password_TCHAR);
				char Msg_PasswordInputted[] = { "Msg_PasswordInputted" };
				LogPrint(INFO, Msg_PasswordInputted);
				TcharToChar(Password_TCHAR, Password_CHAR);
				LogPrint(DEBUG, Password_CHAR);
			}
		}
		default:
			break;
		}
	}
	system("pause");
	return 0;
}

int WindowAdmin(void)
{
	int WindowAdminNewStu(void);
	int WindowAdminNewTea(void);
	int WindowAdminDelStu(void);
	int WindowAdminDelTea(void);
	////
	cleardevice();
	Sleep(150);
	setcliprgn(NULL);
	setpolyfillmode(WINDING);
	setfillcolor(RED);
	solidrectangle(0, 0, W, H);

	setbkmode(TRANSPARENT);
	char Msg_initadminwindow[] = { "initadminwindow" };
	LogPrint(INFO, Msg_initadminwindow);

	TCHAR WindowAdminSlogan[] = { TEXT("指挥官，欢迎回来") };
	outtextxy(330, 20, WindowAdminSlogan);

	TCHAR WindowAdminAllStu[] = { TEXT("当前所有学生") };
	TCHAR WindowAdminAllTea[] = { TEXT("当前所有教师") };

	outtextxy(180, 70, WindowAdminAllStu);
	outtextxy(520, 70, WindowAdminAllTea);

	//展示框-学生
	int WindowAdminAllStuL = 100;
	int WindowAdminAllStuU = 90;
	int WindowAdminAllStuR = WindowAdminAllStuL + 250;
	int WindowAdminAllStuD = WindowAdminAllStuU + 310;
	rectangle(WindowAdminAllStuL, WindowAdminAllStuU, WindowAdminAllStuR, WindowAdminAllStuD);

	//展示框-教师
	int WindowAdminAllTeaL = 440;
	int WindowAdminAllTeaU = 90;
	int WindowAdminAllTeaR = WindowAdminAllTeaL + 250;
	int WindowAdminAllTeaD = WindowAdminAllTeaU + 310;
	rectangle(WindowAdminAllTeaL, WindowAdminAllTeaU, WindowAdminAllTeaR, WindowAdminAllTeaD);

		//新建学生
	int WindowAdminNewStuL = WindowAdminAllStuL;
	int WindowAdminNewStuU = 360;
	int WindowAdminNewStuR = WindowAdminNewStuL + 250;
	int WindowAdminNewStuD = WindowAdminNewStuU + 20;
	rectangle(WindowAdminNewStuL, WindowAdminNewStuU, WindowAdminNewStuR, WindowAdminNewStuD);
		//删除学生
	int WindowAdminDelStuL = WindowAdminAllStuL;
	int WindowAdminDelStuU = 380;
	int WindowAdminDelStuR = WindowAdminDelStuL + 250;
	int WindowAdminDelStuD = WindowAdminDelStuU + 20;
	rectangle(WindowAdminDelStuL, WindowAdminDelStuU, WindowAdminDelStuR, WindowAdminDelStuD);
		//新建教师
	int WindowAdminNewTeaL = WindowAdminAllTeaL;
	int WindowAdminNewTeaU = 360;
	int WindowAdminNewTeaR = WindowAdminNewTeaL + 250;
	int WindowAdminNewTeaD = WindowAdminNewTeaU + 20;
	rectangle(WindowAdminNewTeaL, WindowAdminNewTeaU, WindowAdminNewTeaR, WindowAdminNewTeaD);
		//删除教师
	int WindowAdminDelTeaL = WindowAdminAllTeaL;
	int WindowAdminDelTeaU = 380;
	int WindowAdminDelTeaR = WindowAdminDelTeaL + 250;
	int WindowAdminDelTeaD = WindowAdminDelTeaU + 20;
	rectangle(WindowAdminDelTeaL, WindowAdminDelTeaU, WindowAdminDelTeaR, WindowAdminDelTeaD);

		//学生上一页
	int WindowAdminStuPageUpL = WindowAdminAllStuL;
	int WindowAdminStuPageUpU = 325;
	int WindowAdminStuPageUpR = WindowAdminStuPageUpL + 125;
	int WindowAdminStuPageUpD = WindowAdminStuPageUpU + 35;
	rectangle(WindowAdminStuPageUpL, WindowAdminStuPageUpU, WindowAdminStuPageUpR, WindowAdminStuPageUpD);
		//学生下一页
	int WindowAdminStuPageDownL = 225;
	int WindowAdminStuPageDownU = 325;
	int WindowAdminStuPageDownR = WindowAdminStuPageDownL + 125;
	int WindowAdminStuPageDownD = WindowAdminStuPageDownU + 35;
	rectangle(WindowAdminStuPageDownL, WindowAdminStuPageDownU, WindowAdminStuPageDownR, WindowAdminStuPageDownD);
		//教师上一页
	int WindowAdminTeaPageUpL = WindowAdminAllTeaL;
	int WindowAdminTeaPageUpU = 325;
	int WindowAdminTeaPageUpR = WindowAdminTeaPageUpL + 125;
	int WindowAdminTeaPageUpD = WindowAdminTeaPageUpU + 35;
	rectangle(WindowAdminTeaPageUpL, WindowAdminTeaPageUpU, WindowAdminTeaPageUpR, WindowAdminTeaPageUpD);
		//教师下一页
	int WindowAdminTeaPageDownL = 565;
	int WindowAdminTeaPageDownU = 325;
	int WindowAdminTeaPageDownR = WindowAdminTeaPageDownL + 125;
	int WindowAdminTeaPageDownD = WindowAdminTeaPageDownU + 35;
	rectangle(WindowAdminTeaPageDownL, WindowAdminTeaPageDownU, WindowAdminTeaPageDownR, WindowAdminTeaPageDownD);

	setpolyfillmode(WINDING);
	setfillcolor(BLACK);
	solidrectangle(WindowAdminAllStuL + 1, WindowAdminAllStuU + 1, WindowAdminAllStuR - 1, WindowAdminStuPageUpU - 1);
	solidrectangle(WindowAdminAllTeaL + 1, WindowAdminAllTeaU + 1, WindowAdminAllTeaR - 1, WindowAdminTeaPageUpU - 1);
	setfillcolor(DARKGRAY);
	solidrectangle(WindowAdminNewStuL + 1, WindowAdminNewStuU + 1, WindowAdminNewStuR - 1, WindowAdminNewStuD - 1);
    solidrectangle(WindowAdminDelStuL + 1, WindowAdminDelStuU + 1, WindowAdminDelStuR - 1, WindowAdminDelStuD - 1);
	solidrectangle(WindowAdminNewTeaL + 1, WindowAdminNewTeaU + 1, WindowAdminNewTeaR - 1, WindowAdminNewTeaD - 1);
	solidrectangle(WindowAdminDelTeaL + 1, WindowAdminDelTeaU + 1, WindowAdminDelTeaR - 1, WindowAdminDelTeaD - 1);
	solidrectangle(WindowAdminStuPageUpL + 1, WindowAdminStuPageUpU + 1, WindowAdminStuPageUpR - 1, WindowAdminStuPageUpD - 1);
	solidrectangle(WindowAdminStuPageDownL + 1, WindowAdminStuPageDownU + 1, WindowAdminStuPageDownR - 1, WindowAdminStuPageDownD - 1);
	solidrectangle(WindowAdminTeaPageUpL + 1, WindowAdminTeaPageUpU + 1, WindowAdminTeaPageUpR - 1, WindowAdminTeaPageUpD - 1);
	solidrectangle(WindowAdminTeaPageDownL + 1, WindowAdminTeaPageDownU + 1, WindowAdminTeaPageDownR - 1, WindowAdminTeaPageDownD - 1);

	//返回按钮
	int WindowAdminBackL = 35;
	int WindowAdminBackU = 20;
	int WindowAdminBackR = WindowAdminBackL + 75;
	int WindowAdminBackD = WindowAdminBackU + 25;
	rectangle(WindowAdminBackL, WindowAdminBackU, WindowAdminBackR, WindowAdminBackD);
	TCHAR WindowAdminBackButton[] = { TEXT("返回") };
	//返回图标
	outtextxy(WindowAdminBackL+35, WindowAdminBackU+4, WindowAdminBackButton);
	setfillcolor(YELLOW);
	int BackCircleX = WindowAdminBackL + 5;
	int BackCircleY = WindowAdminBackU + 5;
	solidcircle(BackCircleX + 7,BackCircleY + 7,7);
	setfillcolor(RED);
	solidrectangle(BackCircleX + 5, BackCircleY + 6, BackCircleX + 12, BackCircleY + 8);
	POINT BackTriangle[] = { {BackCircleX + 1, BackCircleY + 7}, {BackCircleX + 6, BackCircleY + 3}, {BackCircleX + 6, BackCircleY + 11} };
	solidpolygon(BackTriangle, 3);

	TCHAR WindowAdminNewStuTCHAR[] = { TEXT("一键招生") };
	TCHAR WindowAdminDelStuTCHAR[] = { TEXT("光速退学") };
	TCHAR WindowAdminNewTeaTCHAR[] = { TEXT("一键招工") };
	TCHAR WindowAdminDelTeaTCHAR[] = { TEXT("光速下课") };
	outtextxy(WindowAdminNewStuL + 90, WindowAdminNewStuU + 2, WindowAdminNewStuTCHAR);
	outtextxy(WindowAdminDelStuL + 90, WindowAdminDelStuU + 2, WindowAdminDelStuTCHAR);
	outtextxy(WindowAdminNewTeaL + 90, WindowAdminNewTeaU + 2, WindowAdminNewTeaTCHAR);
	outtextxy(WindowAdminDelTeaL + 90, WindowAdminDelTeaU + 2, WindowAdminDelTeaTCHAR);

	TCHAR WindowAdminStuPageUpTCHAR[] = { TEXT("上一页学生") };
	TCHAR WindowAdminStuPageDownTCHAR[] = { TEXT("下一页学生") };
	TCHAR WindowAdminTeaPageUpTCHAR[] = { TEXT("上一页教师") };
	TCHAR WindowAdminTeaPageDownTCHAR[] = { TEXT("下一页教师") };
	outtextxy(WindowAdminStuPageUpL + 25, WindowAdminStuPageUpU + 8, WindowAdminStuPageUpTCHAR);
	outtextxy(WindowAdminStuPageDownL + 25, WindowAdminStuPageDownU + 8, WindowAdminStuPageDownTCHAR);
	outtextxy(WindowAdminTeaPageUpL + 25, WindowAdminTeaPageUpU + 8, WindowAdminTeaPageUpTCHAR);
	outtextxy(WindowAdminTeaPageDownL + 25, WindowAdminTeaPageDownU + 8, WindowAdminTeaPageDownTCHAR);

	//查询并绘制学生和教师列表
	char sql_WindowAdminAllStu[MAXLEN] = { "SELECT SNO FROM STUDENT;" };
	LogPrint(DEBUG, (char*)sql_WindowAdminAllStu);
	exec = sqlite3_exec(DB, sql_WindowAdminAllStu, sql_CallBack_Func_InitItemNum, 0, &errmsg);
	if (exec != SQLITE_OK)//SQLITE_ABORT
	{
		//如果出错，就会回到这里来	
		char Msg_sqliteFailedToSelectStu[] = { "FAILED TO INIT Stu ItemNum" };
		LogPrint(ERROR, Msg_sqliteFailedToSelectStu);
		LogPrint(ERROR, errmsg);
	}
	else//SQLITE_OK
	{
		char Msg_sqliteSucceedToSelectStu[] = { "SUCCEED IN INIT Stu ItemNum" };
		LogPrint(INFO, Msg_sqliteSucceedToSelectStu);

		ItemTempCount = 0;
		ItemRestri = 10;
		ItemPage = (ItemNum / ItemRestri) + 1;

		ItemCurrentPage = 1;
		Start = (ItemCurrentPage - 1)*ItemRestri+1;
		End = (ItemCurrentPage)*ItemRestri;

		exec = sqlite3_exec(DB, sql_WindowAdminAllStu, sql_CallBack_Func_Admin_Stu, 0, &errmsg);
	}
	char sql_WindowAdminAllTea[MAXLEN] = { "SELECT TNO FROM TEACHER;" };
	LogPrint(DEBUG, (char*)sql_WindowAdminAllTea);
	exec = sqlite3_exec(DB, sql_WindowAdminAllTea, sql_CallBack_Func_InitItemNum, 0, &errmsg);
	if (exec != SQLITE_OK)//SQLITE_ABORT
	{
		//如果出错，就会回到这里来	
		char Msg_sqliteFailedToSelectTea[] = { "FAILED TO INIT Tea ItemNum" };
		LogPrint(ERROR, Msg_sqliteFailedToSelectTea);
		LogPrint(ERROR, errmsg);
	}
	else//SQLITE_OK
	{
		char Msg_sqliteSucceedToSelectTea[] = { "SUCCEED IN INIT Tea ItemNum" };
		LogPrint(INFO, Msg_sqliteSucceedToSelectTea);

		ItemTempCount = 0;
		ItemRestri = 10;
		ItemPage = (ItemNum / ItemRestri) + 1;

		ItemCurrentPage = 1;
		Start = (ItemCurrentPage - 1)*ItemRestri + 1;
		End = (ItemCurrentPage)*ItemRestri;

		exec = sqlite3_exec(DB, sql_WindowAdminAllTea, sql_CallBack_Func_Admin_Tea, 0, &errmsg);
	}

	MOUSEMSG mouseadmin;
	while (true)
	{
		mouseadmin = GetMouseMsg();
		switch (mouseadmin.uMsg)
		{
		case WM_LBUTTONDOWN:
		{
			if (mouseadmin.x >= WindowAdminBackL && mouseadmin.x <= WindowAdminBackR && mouseadmin.y >= WindowAdminBackU && mouseadmin.y <= WindowAdminBackD)
			{//BackButton
				char Msg_AdminGologin[] = { "request a AdminGologin" };
				LogPrint(INFO, Msg_AdminGologin);
				TCHAR AdminQuit_TCHAR[3 + 1];
				char AdminQuit_CHAR[3 + 1];
				InputBox(AdminQuit_TCHAR, 3 + 1, TEXT("确定要退出吗？（Y/N）"));
				TcharToChar(AdminQuit_TCHAR, AdminQuit_CHAR);
				if (AdminQuit_CHAR[0] == 'Y' || AdminQuit_CHAR[0] == 'y')
				{
					WindowLogin();
				}
			}
			else if (mouseadmin.x >= WindowAdminNewStuL && mouseadmin.x <= WindowAdminNewStuR && mouseadmin.y >= WindowAdminNewStuU && mouseadmin.y <= WindowAdminNewStuD)
			{//Msg_WindowAdminNewStu
				char Msg_WindowAdminNewStu[] = { "Msg_WindowAdminNewStu-一键招生" };
				LogPrint(INFO, Msg_WindowAdminNewStu);
				WindowAdminNewStu();
			}
			else if (mouseadmin.x >= WindowAdminDelStuL && mouseadmin.x <= WindowAdminDelStuR && mouseadmin.y >= WindowAdminDelStuU && mouseadmin.y <= WindowAdminDelStuD)
			{//Msg_WindowAdminDelStu
				char Msg_WindowAdminDelStu[] = { "Msg_WindowAdminDelStu-光速退学" };
				LogPrint(INFO, Msg_WindowAdminDelStu);
				WindowAdminDelStu();
			}
			else if (mouseadmin.x >= WindowAdminNewTeaL && mouseadmin.x <= WindowAdminNewTeaR && mouseadmin.y >= WindowAdminNewTeaU && mouseadmin.y <= WindowAdminNewTeaD)
			{//Msg_WindowAdminNewTea
				char Msg_WindowAdminNewTea[] = { "Msg_WindowAdminNewTea-一键招工" };
				LogPrint(INFO, Msg_WindowAdminNewTea);
				WindowAdminNewTea();
			}
			else if (mouseadmin.x >= WindowAdminDelTeaL && mouseadmin.x <= WindowAdminDelTeaR && mouseadmin.y >= WindowAdminDelTeaU && mouseadmin.y <= WindowAdminDelTeaD)
			{//Msg_WindowAdminDelTea
				char Msg_WindowAdminDelTea[] = { "Msg_WindowAdminDelTea-光速下课" };
				LogPrint(INFO, Msg_WindowAdminDelTea);
				WindowAdminDelTea();
			}
			else if (mouseadmin.x >= WindowAdminStuPageUpL && mouseadmin.x <= WindowAdminStuPageUpR && mouseadmin.y >= WindowAdminStuPageUpU && mouseadmin.y <= WindowAdminStuPageUpD)
			{//Msg_WindowAdminStuPageUp
				char Msg_WindowAdminStuPageUp[] = { "Msg_WindowAdminStuPageUp-上一页学生" };
				LogPrint(INFO, Msg_WindowAdminStuPageUp);
				if (ItemCurrentPage - 1 >= 1)
				{
					setfillcolor(BLACK);
					solidrectangle(WindowAdminAllStuL + 1, WindowAdminAllStuU + 1, WindowAdminAllStuR - 1, WindowAdminStuPageUpU - 1);
					ItemTempCount = 0;
					ItemCurrentPage -= 1;
					Start = (ItemCurrentPage - 1)*ItemRestri + 1;
					End = (ItemCurrentPage)*ItemRestri;
					exec = sqlite3_exec(DB, sql_WindowAdminAllStu, sql_CallBack_Func_Admin_Stu, 0, &errmsg);
				}
			}
			else if (mouseadmin.x >= WindowAdminStuPageDownL && mouseadmin.x <= WindowAdminStuPageDownR && mouseadmin.y >= WindowAdminStuPageDownU && mouseadmin.y <= WindowAdminStuPageDownD)
			{//Msg_WindowAdminStuPageDown
				char Msg_WindowAdminStuPageDown[] = { "Msg_WindowAdminStuPageDown-下一页学生" };
				LogPrint(INFO, Msg_WindowAdminStuPageDown);
				if (ItemCurrentPage + 1 <= ItemPage)
				{
					setfillcolor(BLACK);
					solidrectangle(WindowAdminAllStuL + 1, WindowAdminAllStuU + 1, WindowAdminAllStuR - 1, WindowAdminStuPageUpU - 1);
					ItemTempCount = 0;
					ItemCurrentPage += 1;
					Start = (ItemCurrentPage - 1)*ItemRestri + 1;
					End = (ItemCurrentPage)*ItemRestri;
					exec = sqlite3_exec(DB, sql_WindowAdminAllStu, sql_CallBack_Func_Admin_Stu, 0, &errmsg);
				}
			}
			else if (mouseadmin.x >= WindowAdminTeaPageUpL && mouseadmin.x <= WindowAdminTeaPageUpR && mouseadmin.y >= WindowAdminTeaPageUpU && mouseadmin.y <= WindowAdminTeaPageUpD)
			{//Msg_WindowAdminTeaPageUp
				char Msg_WindowAdminTeaPageUp[] = { "Msg_WindowAdminTeaPageUp-上一页教师" };
				LogPrint(INFO, Msg_WindowAdminTeaPageUp);
				if (ItemCurrentPage - 1 >= 1)
				{
					setfillcolor(BLACK);
					solidrectangle(WindowAdminAllTeaL + 1, WindowAdminAllTeaU + 1, WindowAdminAllTeaR - 1, WindowAdminTeaPageUpU - 1);
					ItemTempCount = 0;
					ItemCurrentPage -= 1;
					Start = (ItemCurrentPage - 1)*ItemRestri + 1;
					End = (ItemCurrentPage)*ItemRestri;
					exec = sqlite3_exec(DB, sql_WindowAdminAllTea, sql_CallBack_Func_Admin_Tea, 0, &errmsg);
				}
			}
			else if (mouseadmin.x >= WindowAdminTeaPageDownL && mouseadmin.x <= WindowAdminTeaPageDownR && mouseadmin.y >= WindowAdminTeaPageDownU && mouseadmin.y <= WindowAdminTeaPageDownD)
			{//Msg_WindowAdminTeaPageDown
				char Msg_WindowAdminTeaPageDown[] = { "Msg_WindowAdminTeaPageDown-下一页教师" };
				LogPrint(INFO, Msg_WindowAdminTeaPageDown);
				if (ItemCurrentPage + 1 <= ItemPage)
				{
					setfillcolor(BLACK);
					solidrectangle(WindowAdminAllTeaL + 1, WindowAdminAllTeaU + 1, WindowAdminAllTeaR - 1, WindowAdminTeaPageUpU - 1);
					ItemTempCount = 0;
					ItemCurrentPage += 1;
					Start = (ItemCurrentPage - 1)*ItemRestri + 1;
					End = (ItemCurrentPage)*ItemRestri;
					exec = sqlite3_exec(DB, sql_WindowAdminAllTea, sql_CallBack_Func_Admin_Tea, 0, &errmsg);
				}
			}
			else
			{
				;
			}
		}
		default:
			break;
		}
	}

	return 0;
}

int WindowAdminNewStu(void)
{
	cleardevice();
	FlushMouseMsgBuffer();
	setcliprgn(NULL);
	setpolyfillmode(WINDING);
	setfillcolor(RED);
	solidrectangle(0, 0, W, H);
	Sleep(350);

	setbkmode(TRANSPARENT);
	char Msg_initWindowAdminNewStu[] = { "initWindowAdminNewStu" };
	LogPrint(INFO, Msg_initWindowAdminNewStu);

	TCHAR WindowAdminNewStuSlogan[] = { TEXT("指挥官，欢迎招生！") };
	outtextxy(330, 20, WindowAdminNewStuSlogan);

	TCHAR WindowAdminNewStuSNO[] = { TEXT("学号") };
	TCHAR WindowAdminNewStuSNAME[] = { TEXT("姓名") };
	TCHAR WindowAdminNewStuSGENDER[] = { TEXT("性别") };
	TCHAR WindowAdminNewStuSMAJOR[] = { TEXT("专业") };
	TCHAR WindowAdminNewStuSCLASS[] = { TEXT("班级") };
	TCHAR WindowAdminNewStuSTELE[] = { TEXT("电话") };
	TCHAR WindowAdminNewStuSEMAIL[] = { TEXT("邮箱") };
	TCHAR WindowAdminNewStuSOTHERS[] = { TEXT("备注") };

	int WindowAdminNewStuInfoColumnL = 225;
	int WindowAdminNewStuInfoColumnU = 100;
	int WindowAdminNewStuInfoColumnX = 100;
	int WindowAdminNewStuInfoColumnY = 35;	

	TCHAR WindowAdminNewStuNote[] = { TEXT("请您完善下列学生信息，其中学号是必填项") };
	outtextxy(WindowAdminNewStuInfoColumnL, WindowAdminNewStuInfoColumnU-50, WindowAdminNewStuNote);

	//学号框
	int WindowAdminNewStuSNOL = WindowAdminNewStuInfoColumnL+ WindowAdminNewStuInfoColumnX;
	int WindowAdminNewStuSNOU = WindowAdminNewStuInfoColumnU;
	int WindowAdminNewStuSNOR = WindowAdminNewStuSNOL + 250;
	int WindowAdminNewStuSNOD = WindowAdminNewStuSNOU + 20;
	rectangle(WindowAdminNewStuSNOL, WindowAdminNewStuSNOU, WindowAdminNewStuSNOR, WindowAdminNewStuSNOD);
	outtextxy(WindowAdminNewStuInfoColumnL, WindowAdminNewStuInfoColumnU, WindowAdminNewStuSNO);
	WindowAdminNewStuInfoColumnU += WindowAdminNewStuInfoColumnY;
	//姓名框
	int WindowAdminNewStuSNAMEL = WindowAdminNewStuInfoColumnL + WindowAdminNewStuInfoColumnX;
	int WindowAdminNewStuSNAMEU = WindowAdminNewStuInfoColumnU;
	int WindowAdminNewStuSNAMER = WindowAdminNewStuSNAMEL + 250;
	int WindowAdminNewStuSNAMED = WindowAdminNewStuSNAMEU + 20;
	rectangle(WindowAdminNewStuSNAMEL, WindowAdminNewStuSNAMEU, WindowAdminNewStuSNAMER, WindowAdminNewStuSNAMED);
	outtextxy(WindowAdminNewStuInfoColumnL, WindowAdminNewStuInfoColumnU, WindowAdminNewStuSNAME);
	WindowAdminNewStuInfoColumnU += WindowAdminNewStuInfoColumnY;
	//性别框
	int WindowAdminNewStuSGENDERL = WindowAdminNewStuInfoColumnL + WindowAdminNewStuInfoColumnX;
	int WindowAdminNewStuSGENDERU = WindowAdminNewStuInfoColumnU;
	int WindowAdminNewStuSGENDERR = WindowAdminNewStuSGENDERL + 250;
	int WindowAdminNewStuSGENDERD = WindowAdminNewStuSGENDERU + 20;
	rectangle(WindowAdminNewStuSGENDERL, WindowAdminNewStuSGENDERU, WindowAdminNewStuSGENDERR, WindowAdminNewStuSGENDERD);
	outtextxy(WindowAdminNewStuInfoColumnL, WindowAdminNewStuInfoColumnU, WindowAdminNewStuSGENDER);
	WindowAdminNewStuInfoColumnU += WindowAdminNewStuInfoColumnY;
	//专业框
	int WindowAdminNewStuSMAJORL = WindowAdminNewStuInfoColumnL + WindowAdminNewStuInfoColumnX;
	int WindowAdminNewStuSMAJORU = WindowAdminNewStuInfoColumnU;
	int WindowAdminNewStuSMAJORR = WindowAdminNewStuSMAJORL + 250;
	int WindowAdminNewStuSMAJORD = WindowAdminNewStuSMAJORU + 20;
	rectangle(WindowAdminNewStuSMAJORL, WindowAdminNewStuSMAJORU, WindowAdminNewStuSMAJORR, WindowAdminNewStuSMAJORD);
	outtextxy(WindowAdminNewStuInfoColumnL, WindowAdminNewStuInfoColumnU, WindowAdminNewStuSMAJOR);
	WindowAdminNewStuInfoColumnU += WindowAdminNewStuInfoColumnY;
	//班级框
	int WindowAdminNewStuSCLASSL = WindowAdminNewStuInfoColumnL + WindowAdminNewStuInfoColumnX;
	int WindowAdminNewStuSCLASSU = WindowAdminNewStuInfoColumnU;
	int WindowAdminNewStuSCLASSR = WindowAdminNewStuSCLASSL + 250;
	int WindowAdminNewStuSCLASSD = WindowAdminNewStuSCLASSU + 20;
	rectangle(WindowAdminNewStuSCLASSL, WindowAdminNewStuSCLASSU, WindowAdminNewStuSCLASSR, WindowAdminNewStuSCLASSD);
	outtextxy(WindowAdminNewStuInfoColumnL, WindowAdminNewStuInfoColumnU, WindowAdminNewStuSCLASS);
	WindowAdminNewStuInfoColumnU += WindowAdminNewStuInfoColumnY;
	//电话框
	int WindowAdminNewStuSTELEL = WindowAdminNewStuInfoColumnL + WindowAdminNewStuInfoColumnX;
	int WindowAdminNewStuSTELEU = WindowAdminNewStuInfoColumnU;
	int WindowAdminNewStuSTELER = WindowAdminNewStuSTELEL + 250;
	int WindowAdminNewStuSTELED = WindowAdminNewStuSTELEU + 20;
	rectangle(WindowAdminNewStuSTELEL, WindowAdminNewStuSTELEU, WindowAdminNewStuSTELER, WindowAdminNewStuSTELED);
	outtextxy(WindowAdminNewStuInfoColumnL, WindowAdminNewStuInfoColumnU, WindowAdminNewStuSTELE);
	WindowAdminNewStuInfoColumnU += WindowAdminNewStuInfoColumnY;
	//邮箱框
	int WindowAdminNewStuSEMAILL = WindowAdminNewStuInfoColumnL + WindowAdminNewStuInfoColumnX;
	int WindowAdminNewStuSEMAILU = WindowAdminNewStuInfoColumnU;
	int WindowAdminNewStuSEMAILR = WindowAdminNewStuSEMAILL + 250;
	int WindowAdminNewStuSEMAILD = WindowAdminNewStuSEMAILU + 20;
	rectangle(WindowAdminNewStuSEMAILL, WindowAdminNewStuSEMAILU, WindowAdminNewStuSEMAILR, WindowAdminNewStuSEMAILD);
	outtextxy(WindowAdminNewStuInfoColumnL, WindowAdminNewStuInfoColumnU, WindowAdminNewStuSEMAIL);
	WindowAdminNewStuInfoColumnU += WindowAdminNewStuInfoColumnY;
	//备注框
	int WindowAdminNewStuSOTHERSL = WindowAdminNewStuInfoColumnL + WindowAdminNewStuInfoColumnX;
	int WindowAdminNewStuSOTHERSU = WindowAdminNewStuInfoColumnU;
	int WindowAdminNewStuSOTHERSR = WindowAdminNewStuSOTHERSL + 250;
	int WindowAdminNewStuSOTHERSD = WindowAdminNewStuSOTHERSU + 20;
	rectangle(WindowAdminNewStuSOTHERSL, WindowAdminNewStuSOTHERSU, WindowAdminNewStuSOTHERSR, WindowAdminNewStuSOTHERSD);
	outtextxy(WindowAdminNewStuInfoColumnL, WindowAdminNewStuInfoColumnU, WindowAdminNewStuSOTHERS);
	WindowAdminNewStuInfoColumnU += WindowAdminNewStuInfoColumnY;

	TCHAR WindowAdminNewStuSubmit[] = { TEXT("确认提交") };

	//确认提交
	int WindowAdminNewStuSubmitL = WindowAdminNewStuInfoColumnL + WindowAdminNewStuInfoColumnX;
	int WindowAdminNewStuSubmitU = WindowAdminNewStuInfoColumnU + 10;
	int WindowAdminNewStuSubmitR = WindowAdminNewStuSubmitL + 125;
	int WindowAdminNewStuSubmitD = WindowAdminNewStuSubmitU + 35;
	rectangle(WindowAdminNewStuSubmitL, WindowAdminNewStuSubmitU, WindowAdminNewStuSubmitR, WindowAdminNewStuSubmitD);
	setfillcolor(DARKGRAY);
	solidrectangle(WindowAdminNewStuSubmitL + 1, WindowAdminNewStuSubmitU + 1, WindowAdminNewStuSubmitR - 1, WindowAdminNewStuSubmitD - 1);
	outtextxy(WindowAdminNewStuSubmitL+30, WindowAdminNewStuSubmitU+10, WindowAdminNewStuSubmit);

	//返回按钮
	int WindowAdminNewStuBackL = 35;
	int WindowAdminNewStuBackU = 20;
	int WindowAdminNewStuBackR = WindowAdminNewStuBackL + 75;
	int WindowAdminNewStuBackD = WindowAdminNewStuBackU + 25;
	rectangle(WindowAdminNewStuBackL, WindowAdminNewStuBackU, WindowAdminNewStuBackR, WindowAdminNewStuBackD);
	TCHAR WindowAdminBackNewStuButton[] = { TEXT("返回") };
	//返回图标
	outtextxy(WindowAdminNewStuBackL + 35, WindowAdminNewStuBackU + 4, WindowAdminBackNewStuButton);
	setfillcolor(YELLOW);
	int BackCircleX = WindowAdminNewStuBackL + 5;
	int BackCircleY = WindowAdminNewStuBackU + 5;
	solidcircle(BackCircleX + 7, BackCircleY + 7, 7);
	setfillcolor(RED);
	solidrectangle(BackCircleX + 5, BackCircleY + 6, BackCircleX + 12, BackCircleY + 8);
	POINT BackTriangle[] = { {BackCircleX + 1, BackCircleY + 7}, {BackCircleX + 6, BackCircleY + 3}, {BackCircleX + 6, BackCircleY + 11} };
	solidpolygon(BackTriangle, 3);

	setfillcolor(BLACK);
	solidrectangle(WindowAdminNewStuSNOL + 1, WindowAdminNewStuSNOU + 1, WindowAdminNewStuSNOR - 1, WindowAdminNewStuSNOD - 1);
	solidrectangle(WindowAdminNewStuSNAMEL + 1, WindowAdminNewStuSNAMEU + 1, WindowAdminNewStuSNAMER - 1, WindowAdminNewStuSNAMED - 1);
	solidrectangle(WindowAdminNewStuSGENDERL + 1, WindowAdminNewStuSGENDERU + 1, WindowAdminNewStuSGENDERR - 1, WindowAdminNewStuSGENDERD - 1);
	solidrectangle(WindowAdminNewStuSMAJORL + 1, WindowAdminNewStuSMAJORU + 1, WindowAdminNewStuSMAJORR - 1, WindowAdminNewStuSMAJORD - 1);
	solidrectangle(WindowAdminNewStuSCLASSL + 1, WindowAdminNewStuSCLASSU + 1, WindowAdminNewStuSCLASSR - 1, WindowAdminNewStuSCLASSD - 1);
	solidrectangle(WindowAdminNewStuSTELEL + 1, WindowAdminNewStuSTELEU + 1, WindowAdminNewStuSTELER - 1, WindowAdminNewStuSTELED - 1);
	solidrectangle(WindowAdminNewStuSEMAILL + 1, WindowAdminNewStuSEMAILU + 1, WindowAdminNewStuSEMAILR - 1, WindowAdminNewStuSEMAILD - 1);
	solidrectangle(WindowAdminNewStuSOTHERSL + 1, WindowAdminNewStuSOTHERSU + 1, WindowAdminNewStuSOTHERSR - 1, WindowAdminNewStuSOTHERSD - 1);

	TCHAR SNO_TCHAR[13 + 1];
	TCHAR SNAME_TCHAR[13 + 1];
	TCHAR SGENDER_TCHAR[13 + 1];
	TCHAR SMAJOR_TCHAR[13 + 1];
	TCHAR SCLASS_TCHAR[13 + 1];
	TCHAR STELE_TCHAR[13 + 1];
	TCHAR SEMAIL_TCHAR[13 + 1];
	TCHAR SOTHERS_TCHAR[13 + 1];

	char SNO_CHAR[13 + 1];
	char SNAME_CHAR[13 + 1];
	char SGENDER_CHAR[13 + 1];
	char SMAJOR_CHAR[13 + 1];
	char SCLASS_CHAR[13 + 1];
	char STELE_CHAR[13 + 1];
	char SEMAIL_CHAR[13 + 1];
	char SOTHERS_CHAR[13 + 1];

	int flag_SNOIsFinish = 0;
	int flag_SNAMEIsFinish = 0;
	int flag_SGENDERIsFinish = 0;
	int flag_SMAJORIsFinish = 0;
	int flag_SCLASSIsFinish = 0;
	int flag_STELEIsFinish = 0;
	int flag_SEMAILIsFinish = 0;
	int flag_SOTHERSIsFinish = 0;

	MOUSEMSG mouseadminNewStu;
	while (true)
	{
		mouseadminNewStu = GetMouseMsg();
		switch (mouseadminNewStu.uMsg)
		{
		case WM_LBUTTONDOWN:
		{
			if (mouseadminNewStu.x >= WindowAdminNewStuBackL && mouseadminNewStu.x <= WindowAdminNewStuBackR && mouseadminNewStu.y >= WindowAdminNewStuBackU && mouseadminNewStu.y <= WindowAdminNewStuBackD)
			{//BackButton
				char Msg_AdminNewStuGoAdmin[] = { "request a AdminNewStuGoAdmin" };
				LogPrint(INFO, Msg_AdminNewStuGoAdmin);
				WindowAdmin();
			}
			else if (mouseadminNewStu.x >= WindowAdminNewStuSNOL && mouseadminNewStu.x <= WindowAdminNewStuSNOR && mouseadminNewStu.y >= WindowAdminNewStuSNOU && mouseadminNewStu.y <= WindowAdminNewStuSNOD)
			{//Msg_WindowAdminNewStuSNO
				setfillcolor(BLACK);
				solidrectangle(WindowAdminNewStuSNOL + 1, WindowAdminNewStuSNOU + 1, WindowAdminNewStuSNOR - 1, WindowAdminNewStuSNOD - 1);
				setfillcolor(RED);
				solidrectangle(WindowAdminNewStuSubmitL + 174, WindowAdminNewStuSubmitU + 9, W, H);
				char Msg_WindowAdminNewStuSNO[] = { "Msg_WindowAdminNewStuSNO-学号" };
				LogPrint(INFO, Msg_WindowAdminNewStuSNO);
				InputBox(SNO_TCHAR, 13 + 1, TEXT("请输入学号(不超过13个字符)"));
				outtextxy(WindowAdminNewStuSNOL + 1, WindowAdminNewStuSNOU + 2, SNO_TCHAR);
				TcharToChar(SNO_TCHAR, SNO_CHAR);
				if (SNO_CHAR[0] != 'S' || SNO_CHAR[1] != 'T' || SNO_CHAR[2] != 'U')
				{
					setfillcolor(BLACK);
					solidrectangle(WindowAdminNewStuSNOL + 1, WindowAdminNewStuSNOU + 1, WindowAdminNewStuSNOR - 1, WindowAdminNewStuSNOD - 1);
					TCHAR STUWARN[] = { TEXT("请保证学生学号以STU开头") };
					Sleep(250);
					outtextxy(WindowAdminNewStuSubmitL + 175, WindowAdminNewStuSubmitU + 10, STUWARN);
				}
				LogPrint(DEBUG, SNO_CHAR);
				flag_SNOIsFinish = 1;
			}
			else if (mouseadminNewStu.x >= WindowAdminNewStuSNAMEL && mouseadminNewStu.x <= WindowAdminNewStuSNAMER && mouseadminNewStu.y >= WindowAdminNewStuSNAMEU && mouseadminNewStu.y <= WindowAdminNewStuSNAMED)
			{//Msg_WindowAdminNewStuSNAME
				setfillcolor(BLACK);
				solidrectangle(WindowAdminNewStuSNAMEL + 1, WindowAdminNewStuSNAMEU + 1, WindowAdminNewStuSNAMER - 1, WindowAdminNewStuSNAMED - 1);
				char Msg_WindowAdminNewStuSNAME[] = { "Msg_WindowAdminNewStuSNAME-姓名" };
				LogPrint(INFO, Msg_WindowAdminNewStuSNAME);
				InputBox(SNAME_TCHAR, 13 + 1, TEXT("请输入姓名(不超过13个字符)"));
				outtextxy(WindowAdminNewStuSNAMEL + 1, WindowAdminNewStuSNAMEU + 2, SNAME_TCHAR);
				TcharToChar(SNAME_TCHAR, SNAME_CHAR);
				LogPrint(DEBUG, SNAME_CHAR);
				flag_SNAMEIsFinish = 1;
			}
			else if (mouseadminNewStu.x >= WindowAdminNewStuSGENDERL && mouseadminNewStu.x <= WindowAdminNewStuSGENDERR && mouseadminNewStu.y >= WindowAdminNewStuSGENDERU && mouseadminNewStu.y <= WindowAdminNewStuSGENDERD)
			{//Msg_WindowAdminNewStuSGENDER
				setfillcolor(BLACK);
				solidrectangle(WindowAdminNewStuSGENDERL + 1, WindowAdminNewStuSGENDERU + 1, WindowAdminNewStuSGENDERR - 1, WindowAdminNewStuSGENDERD - 1);
				char Msg_WindowAdminNewStuSGENDER[] = { "Msg_WindowAdminNewStuSGENDER-性别" };
				LogPrint(INFO, Msg_WindowAdminNewStuSGENDER);
				InputBox(SGENDER_TCHAR, 13 + 1, TEXT("请输入性别(不超过13个字符)"));
				outtextxy(WindowAdminNewStuSGENDERL + 1, WindowAdminNewStuSGENDERU + 2, SGENDER_TCHAR);
				TcharToChar(SGENDER_TCHAR, SGENDER_CHAR);
				LogPrint(DEBUG, SGENDER_CHAR);
				flag_SGENDERIsFinish = 1;
			}
			else if (mouseadminNewStu.x >= WindowAdminNewStuSMAJORL && mouseadminNewStu.x <= WindowAdminNewStuSMAJORR && mouseadminNewStu.y >= WindowAdminNewStuSMAJORU && mouseadminNewStu.y <= WindowAdminNewStuSMAJORD)
			{//Msg_WindowAdminNewStuSMAJOR
				setfillcolor(BLACK);
				solidrectangle(WindowAdminNewStuSMAJORL + 1, WindowAdminNewStuSMAJORU + 1, WindowAdminNewStuSMAJORR - 1, WindowAdminNewStuSMAJORD - 1);
				char Msg_WindowAdminNewStuSMAJOR[] = { "Msg_WindowAdminNewStuSMAJOR-专业" };
				LogPrint(INFO, Msg_WindowAdminNewStuSMAJOR);
				InputBox(SMAJOR_TCHAR, 13 + 1, TEXT("请输入专业(不超过13个字符)"));
				outtextxy(WindowAdminNewStuSMAJORL + 1, WindowAdminNewStuSMAJORU + 2, SMAJOR_TCHAR);
				TcharToChar(SMAJOR_TCHAR, SMAJOR_CHAR);
				LogPrint(DEBUG, SMAJOR_CHAR);
				flag_SMAJORIsFinish = 1;
			}
			else if (mouseadminNewStu.x >= WindowAdminNewStuSCLASSL && mouseadminNewStu.x <= WindowAdminNewStuSCLASSR && mouseadminNewStu.y >= WindowAdminNewStuSCLASSU && mouseadminNewStu.y <= WindowAdminNewStuSCLASSD)
			{//Msg_WindowAdminNewStuSCLASS
				setfillcolor(BLACK);
				solidrectangle(WindowAdminNewStuSCLASSL + 1, WindowAdminNewStuSCLASSU + 1, WindowAdminNewStuSCLASSR - 1, WindowAdminNewStuSCLASSD - 1);
				char Msg_WindowAdminNewStuSCLASS[] = { "Msg_WindowAdminNewStuSCLASS-班级" };
				LogPrint(INFO, Msg_WindowAdminNewStuSCLASS);
				InputBox(SCLASS_TCHAR, 13 + 1, TEXT("请输入班级(不超过13个字符)"));
				outtextxy(WindowAdminNewStuSCLASSL + 1, WindowAdminNewStuSCLASSU + 2, SCLASS_TCHAR);
				TcharToChar(SCLASS_TCHAR, SCLASS_CHAR);
				LogPrint(DEBUG, SCLASS_CHAR);
				flag_SCLASSIsFinish = 1;
			}
			else if (mouseadminNewStu.x >= WindowAdminNewStuSTELEL && mouseadminNewStu.x <= WindowAdminNewStuSTELER && mouseadminNewStu.y >= WindowAdminNewStuSTELEU && mouseadminNewStu.y <= WindowAdminNewStuSTELED)
			{//Msg_WindowAdminNewStuSTELE
				setfillcolor(BLACK);
				solidrectangle(WindowAdminNewStuSTELEL + 1, WindowAdminNewStuSTELEU + 1, WindowAdminNewStuSTELER - 1, WindowAdminNewStuSTELED - 1);
				char Msg_WindowAdminNewStuSTELE[] = { "Msg_WindowAdminNewStuSTELE-电话" };
				LogPrint(INFO, Msg_WindowAdminNewStuSTELE);
				InputBox(STELE_TCHAR, 13 + 1, TEXT("请输入电话(不超过13个字符)"));
				outtextxy(WindowAdminNewStuSTELEL + 1, WindowAdminNewStuSTELEU + 2, STELE_TCHAR);
				TcharToChar(STELE_TCHAR, STELE_CHAR);
				LogPrint(DEBUG, STELE_CHAR);
				flag_STELEIsFinish = 1;
			}
			else if (mouseadminNewStu.x >= WindowAdminNewStuSEMAILL && mouseadminNewStu.x <= WindowAdminNewStuSEMAILR && mouseadminNewStu.y >= WindowAdminNewStuSEMAILU && mouseadminNewStu.y <= WindowAdminNewStuSEMAILD)
			{//Msg_WindowAdminNewStuSEMAIL
				setfillcolor(BLACK);
				solidrectangle(WindowAdminNewStuSEMAILL + 1, WindowAdminNewStuSEMAILU + 1, WindowAdminNewStuSEMAILR - 1, WindowAdminNewStuSEMAILD - 1);
				char Msg_WindowAdminNewStuSEMAIL[] = { "Msg_WindowAdminNewStuSEMAIL-邮箱" };
				LogPrint(INFO, Msg_WindowAdminNewStuSEMAIL);
				InputBox(SEMAIL_TCHAR, 13 + 1, TEXT("请输入邮箱(不超过13个字符)"));
				outtextxy(WindowAdminNewStuSEMAILL + 1, WindowAdminNewStuSEMAILU + 2, SEMAIL_TCHAR);
				TcharToChar(SEMAIL_TCHAR, SEMAIL_CHAR);
				LogPrint(DEBUG, SEMAIL_CHAR);
				flag_SEMAILIsFinish = 1;
			}
			else if (mouseadminNewStu.x >= WindowAdminNewStuSOTHERSL && mouseadminNewStu.x <= WindowAdminNewStuSOTHERSR && mouseadminNewStu.y >= WindowAdminNewStuSOTHERSU && mouseadminNewStu.y <= WindowAdminNewStuSOTHERSD)
			{//Msg_WindowAdminNewStuSOTHERS
			    setfillcolor(BLACK);
				solidrectangle(WindowAdminNewStuSOTHERSL + 1, WindowAdminNewStuSOTHERSU + 1, WindowAdminNewStuSOTHERSR - 1, WindowAdminNewStuSOTHERSD - 1);
				char Msg_WindowAdminNewStuSOTHERS[] = { "Msg_WindowAdminNewStuSOTHERS-备注" };
				LogPrint(INFO, Msg_WindowAdminNewStuSOTHERS);
				InputBox(SOTHERS_TCHAR, 13 + 1, TEXT("请输入备注(不超过13个字符)"));
				outtextxy(WindowAdminNewStuSOTHERSL + 1, WindowAdminNewStuSOTHERSU + 2, SOTHERS_TCHAR);
				TcharToChar(SOTHERS_TCHAR, SOTHERS_CHAR);
				LogPrint(DEBUG, SOTHERS_CHAR);
				flag_SOTHERSIsFinish = 1;
			}
			else if (mouseadminNewStu.x >= WindowAdminNewStuSubmitL && mouseadminNewStu.x <= WindowAdminNewStuSubmitR && mouseadminNewStu.y >= WindowAdminNewStuSubmitU && mouseadminNewStu.y <= WindowAdminNewStuSubmitD)
			{//Msg_WindowAdminNewStuSubmit
			    setfillcolor(RED);
			    solidrectangle(WindowAdminNewStuSubmitL + 174, WindowAdminNewStuSubmitU + 9, W, H);
				char Msg_WindowAdminNewStuSubmit[] = { "Msg_WindowAdminNewStuSubmit-提交" };
				LogPrint(INFO, Msg_WindowAdminNewStuSubmit);
				//先判断会不会死掉
				//严格版本if (flag_SNOIsFinish == 1 && flag_SNAMEIsFinish == 1 && flag_SGENDERIsFinish == 1 && flag_SMAJORIsFinish == 1 && flag_SCLASSIsFinish == 1 && flag_STELEIsFinish == 1 && flag_SEMAILIsFinish == 1 && flag_SOTHERSIsFinish == 1)
			    if (flag_SNOIsFinish == 1)
				{
					//组装SQL语句
					char sql_WindowNewStuP1[MAXLEN] = { "INSERT INTO STUDENT VALUES('" };
					char sql_WindowNewStuP2[MAXLEN] = { "','" };
					char sql_WindowNewStuP3[MAXLEN] = { "');" };
					strcat(sql_WindowNewStuP1, SNO_CHAR);
					if (flag_SNAMEIsFinish == 1)
					{
						strcat(sql_WindowNewStuP1, sql_WindowNewStuP2);
						strcat(sql_WindowNewStuP1, SNAME_CHAR);
					}
					else
					{
						strcat(sql_WindowNewStuP1, sql_WindowNewStuP2);
					}
					if (flag_SGENDERIsFinish == 1)
					{
						strcat(sql_WindowNewStuP1, sql_WindowNewStuP2);
						strcat(sql_WindowNewStuP1, SGENDER_CHAR);
					}
					else
					{
						strcat(sql_WindowNewStuP1, sql_WindowNewStuP2);
					}
					if (flag_SMAJORIsFinish == 1)
					{
						strcat(sql_WindowNewStuP1, sql_WindowNewStuP2);
						strcat(sql_WindowNewStuP1, SMAJOR_CHAR);
					}
					else
					{
						strcat(sql_WindowNewStuP1, sql_WindowNewStuP2);
					}
					if (flag_SCLASSIsFinish == 1)
					{
						strcat(sql_WindowNewStuP1, sql_WindowNewStuP2);
						strcat(sql_WindowNewStuP1, SCLASS_CHAR);
					}
					else
					{
						strcat(sql_WindowNewStuP1, sql_WindowNewStuP2);
					}
					if (flag_STELEIsFinish == 1)
					{
						strcat(sql_WindowNewStuP1, sql_WindowNewStuP2);
						strcat(sql_WindowNewStuP1, STELE_CHAR);
					}
					else
					{
						strcat(sql_WindowNewStuP1, sql_WindowNewStuP2);
					}
					if (flag_SEMAILIsFinish == 1)
					{
						strcat(sql_WindowNewStuP1, sql_WindowNewStuP2);
						strcat(sql_WindowNewStuP1, SEMAIL_CHAR);
					}
					else
					{
						strcat(sql_WindowNewStuP1, sql_WindowNewStuP2);
					}
					if (flag_SOTHERSIsFinish == 0)
					{
						//提示需要补全至少备注的字段
						char Msg_SubmitButNotFinishedSOTHERS[] = { "Msg_SubmitButNotFinishedSOTHERS" };
						LogPrint(WARN, Msg_SubmitButNotFinishedSOTHERS);
					}
					else
					{
						strcat(sql_WindowNewStuP1, sql_WindowNewStuP2);
						strcat(sql_WindowNewStuP1, SOTHERS_CHAR);
						strcat(sql_WindowNewStuP1, sql_WindowNewStuP3);
						const char *sql_WindowNewStu = sql_WindowNewStuP1;
						LogPrint(DEBUG, (char*)sql_WindowNewStu);
						exec = sqlite3_exec(DB, sql_WindowNewStu, sql_CallBack_Func_NewSubmit, 0, &errmsg);
						//回调函数是空的
						if (exec != SQLITE_OK)//SQLITE_ABORT
						{
							//如果插入出错，就会回到这里来	
							char Msg_sqliteFailedToSelectLogin[] = { "FAILED TO INSERT STUDENT" };
							LogPrint(ERROR, Msg_sqliteFailedToSelectLogin);
							LogPrint(ERROR, errmsg);
						}
						else//SQLITE_OK
						{
							//如果插入成功，就会回到这里来	
							//组装完成并插入了这个学生
							char Msg_sqliteIsOpen[] = { "SUCCEED IN INSERT STUDENT" };
							LogPrint(INFO, Msg_sqliteIsOpen);
							
							//然后设定密码
							char StuPassword_CHAR[13 + 1];
							TCHAR StuPassword_TCHAR[13 + 1];
							InputBox(StuPassword_TCHAR, 13 + 1, TEXT("请为该学生设定登陆密码(不超过13个字符)"));
							TcharToChar(StuPassword_TCHAR, StuPassword_CHAR);
							LogPrint(DEBUG, StuPassword_CHAR);

							//设定密码这段可以再来一遍
							char sql_WindowNewStuInsertUserP1[MAXLEN] = { "INSERT INTO USER VALUES('" };
							char sql_WindowNewStuInsertUserP2[MAXLEN] = { "','" };
							char sql_WindowNewStuInsertUserP3[MAXLEN] = { "'," };
							char sql_WindowNewStuInsertUserP4[MAXLEN] = { ");" };
							char sql_WindowNewStuInsertUserPStu[MAXLEN] = { "1" };
							strcat(sql_WindowNewStuInsertUserP1, SNO_CHAR);
							strcat(sql_WindowNewStuInsertUserP1, sql_WindowNewStuInsertUserP2);
							strcat(sql_WindowNewStuInsertUserP1, StuPassword_CHAR);
							strcat(sql_WindowNewStuInsertUserP1, sql_WindowNewStuInsertUserP3);
							strcat(sql_WindowNewStuInsertUserP1, sql_WindowNewStuInsertUserPStu);
							strcat(sql_WindowNewStuInsertUserP1, sql_WindowNewStuInsertUserP4);
							const char *sql_WindowNewStuInsertUser = sql_WindowNewStuInsertUserP1;
							LogPrint(DEBUG, (char*)sql_WindowNewStuInsertUser);
							exec = sqlite3_exec(DB, sql_WindowNewStuInsertUser, sql_CallBack_Func_NewSubmit, 0, &errmsg);
							if (exec != SQLITE_OK)//SQLITE_ABORT
							{
								//如果插入出错，就会回到这里来	
								char Msg_sqliteFailedToSelectLogin2[] = { "FAILED TO INSERT USER" };
								LogPrint(ERROR, Msg_sqliteFailedToSelectLogin2);
								LogPrint(ERROR, errmsg);
							}
							else//SQLITE_OK
							{
								char Msg_sqliteIsOpen2[] = { "SUCCEED IN INSERT USER" };
								LogPrint(INFO, Msg_sqliteIsOpen2);
								setfillcolor(BLACK);
								solidrectangle(WindowAdminNewStuSNOL + 1, WindowAdminNewStuSNOU + 1, WindowAdminNewStuSNOR - 1, WindowAdminNewStuSNOD - 1);
								solidrectangle(WindowAdminNewStuSNAMEL + 1, WindowAdminNewStuSNAMEU + 1, WindowAdminNewStuSNAMER - 1, WindowAdminNewStuSNAMED - 1);
								solidrectangle(WindowAdminNewStuSGENDERL + 1, WindowAdminNewStuSGENDERU + 1, WindowAdminNewStuSGENDERR - 1, WindowAdminNewStuSGENDERD - 1);
								solidrectangle(WindowAdminNewStuSMAJORL + 1, WindowAdminNewStuSMAJORU + 1, WindowAdminNewStuSMAJORR - 1, WindowAdminNewStuSMAJORD - 1);
								solidrectangle(WindowAdminNewStuSCLASSL + 1, WindowAdminNewStuSCLASSU + 1, WindowAdminNewStuSCLASSR - 1, WindowAdminNewStuSCLASSD - 1);
								solidrectangle(WindowAdminNewStuSTELEL + 1, WindowAdminNewStuSTELEU + 1, WindowAdminNewStuSTELER - 1, WindowAdminNewStuSTELED - 1);
								solidrectangle(WindowAdminNewStuSEMAILL + 1, WindowAdminNewStuSEMAILU + 1, WindowAdminNewStuSEMAILR - 1, WindowAdminNewStuSEMAILD - 1);
								solidrectangle(WindowAdminNewStuSOTHERSL + 1, WindowAdminNewStuSOTHERSU + 1, WindowAdminNewStuSOTHERSR - 1, WindowAdminNewStuSOTHERSD - 1);
								TCHAR NewStuSuccess[] = { TEXT("创建用户成功") };
								Sleep(250);
								outtextxy(WindowAdminNewStuSubmitL + 175, WindowAdminNewStuSubmitU + 10, NewStuSuccess);
							}
						}
					}
				}
				else
				{
					//提示需要补全至少学号的字段
					char Msg_SubmitButNotFinishedSNO[] = { "Msg_SubmitButNotFinishedSNO" };
					LogPrint(WARN, Msg_SubmitButNotFinishedSNO);
				}
			}
			else
			{
				;
			}
		}
		default:
			break;
		}
	}

	return 0;
}

int WindowAdminDelStu(void)
{
	cleardevice();
	FlushMouseMsgBuffer();
	setcliprgn(NULL);
	setpolyfillmode(WINDING);
	setfillcolor(RED);
	solidrectangle(0, 0, W, H);
	Sleep(350);

	setbkmode(TRANSPARENT);
	char Msg_initWindowAdminDelStu[] = { "initWindowAdminDelStu" };
	LogPrint(INFO, Msg_initWindowAdminDelStu);

	TCHAR WindowAdminDelStuSlogan[] = { TEXT("指挥官，要退学谁？") };
	outtextxy(330, 20, WindowAdminDelStuSlogan);

	TCHAR WindowAdminDelStuPageUp[] = { TEXT("上一页") };
	TCHAR WindowAdminDelStuPageDown[] = { TEXT("下一页") };
	TCHAR WindowAdminDelStuDelete[] = { TEXT("开除这个学生") };
	TCHAR WindowAdminDelStuTrick[] = { TEXT("谭校封生") };

	//概览窗口
	int WindowAdminDelStuViewAllL = 50;
	int WindowAdminDelStuViewAllU = 100;
	int WindowAdminDelStuViewAllR = WindowAdminDelStuViewAllL + 460;
	int WindowAdminDelStuViewAllD = WindowAdminDelStuViewAllU + 300;
	rectangle(WindowAdminDelStuViewAllL, WindowAdminDelStuViewAllU, WindowAdminDelStuViewAllR, WindowAdminDelStuViewAllD);

	//上一页按钮
	int WindowAdminDelStuPageUpL = 550;
	int WindowAdminDelStuPageUpU = 115;
	int WindowAdminDelStuPageUpR = WindowAdminDelStuPageUpL + 65;
	int WindowAdminDelStuPageUpD = WindowAdminDelStuPageUpU + 95;
	rectangle(WindowAdminDelStuPageUpL, WindowAdminDelStuPageUpU, WindowAdminDelStuPageUpR, WindowAdminDelStuPageUpD);

	//下一页按钮
	int WindowAdminDelStuPageDownL = 685;
	int WindowAdminDelStuPageDownU = 115;
	int WindowAdminDelStuPageDownR = WindowAdminDelStuPageDownL + 65;
	int WindowAdminDelStuPageDownD = WindowAdminDelStuPageDownU + 95;
	rectangle(WindowAdminDelStuPageDownL, WindowAdminDelStuPageDownU, WindowAdminDelStuPageDownR, WindowAdminDelStuPageDownD);

	//页面选择按钮
	int WindowAdminDelStuPageSelectL = 640;
	int WindowAdminDelStuPageSelectU = 115;
	int WindowAdminDelStuPageSelectR = WindowAdminDelStuPageSelectL + 20;
	int WindowAdminDelStuPageSelectD = WindowAdminDelStuPageSelectU + 95;
	rectangle(WindowAdminDelStuPageSelectL, WindowAdminDelStuPageSelectU, WindowAdminDelStuPageSelectR, WindowAdminDelStuPageSelectD);

	//开除按钮
	int WindowAdminDelStuDeleteL = 550;
	int WindowAdminDelStuDeleteU = 340;
	int WindowAdminDelStuDeleteR = WindowAdminDelStuDeleteL + 200;
	int WindowAdminDelStuDeleteD = WindowAdminDelStuDeleteU + 45;
	rectangle(WindowAdminDelStuDeleteL, WindowAdminDelStuDeleteU, WindowAdminDelStuDeleteR, WindowAdminDelStuDeleteD);

	//插画
	rectangle(550, 230, 750, 320);
	IMAGE DelStuimg;
	loadimage(&DelStuimg, TEXT("Pic_DelStuAndTea.bmp"));
	int DelStuimgW, DelStuimgH;
	DelStuimgW = DelStuimg.getwidth();
	DelStuimgH = DelStuimg.getheight();
	putimage(550, 230, &DelStuimg);

	//返回按钮
	int WindowAdminDelStuBackL = 35;
	int WindowAdminDelStuBackU = 20;
	int WindowAdminDelStuBackR = WindowAdminDelStuBackL + 75;
	int WindowAdminDelStuBackD = WindowAdminDelStuBackU + 25;
	rectangle(WindowAdminDelStuBackL, WindowAdminDelStuBackU, WindowAdminDelStuBackR, WindowAdminDelStuBackD);
	TCHAR WindowAdminBackDelStuButton[] = { TEXT("返回") };
	//返回图标
	outtextxy(WindowAdminDelStuBackL + 35, WindowAdminDelStuBackU + 4, WindowAdminBackDelStuButton);
	setfillcolor(YELLOW);
	int BackCircleX = WindowAdminDelStuBackL + 5;
	int BackCircleY = WindowAdminDelStuBackU + 5;
	solidcircle(BackCircleX + 7, BackCircleY + 7, 7);
	setfillcolor(RED);
	solidrectangle(BackCircleX + 5, BackCircleY + 6, BackCircleX + 12, BackCircleY + 8);
	POINT BackTriangle[] = { {BackCircleX + 1, BackCircleY + 7}, {BackCircleX + 6, BackCircleY + 3}, {BackCircleX + 6, BackCircleY + 11} };
	solidpolygon(BackTriangle, 3);

	setfillcolor(DARKGRAY);
	solidrectangle(WindowAdminDelStuViewAllL + 1, WindowAdminDelStuViewAllU + 1, WindowAdminDelStuViewAllR - 1, WindowAdminDelStuViewAllD - 1);
	setfillcolor(LIGHTGRAY);
	solidrectangle(WindowAdminDelStuPageUpL + 1, WindowAdminDelStuPageUpU + 1, WindowAdminDelStuPageUpR - 1, WindowAdminDelStuPageUpD - 1);
	solidrectangle(WindowAdminDelStuPageDownL + 1, WindowAdminDelStuPageDownU + 1, WindowAdminDelStuPageDownR - 1, WindowAdminDelStuPageDownD - 1);
	solidrectangle(WindowAdminDelStuPageSelectL + 1, WindowAdminDelStuPageSelectU + 1, WindowAdminDelStuPageSelectR - 1, WindowAdminDelStuPageSelectD - 1);
	setfillcolor(LIGHTRED);
	solidrectangle(WindowAdminDelStuDeleteL + 1, WindowAdminDelStuDeleteU + 1, WindowAdminDelStuDeleteR - 1, WindowAdminDelStuDeleteD - 1);

	outtextxy(WindowAdminDelStuPageUpL + 11, WindowAdminDelStuPageUpU + 38, WindowAdminDelStuPageUp);
	outtextxy(WindowAdminDelStuPageDownL + 11, WindowAdminDelStuPageDownU + 38, WindowAdminDelStuPageDown);
	outtextxy(WindowAdminDelStuDeleteL + 55, WindowAdminDelStuDeleteU + 15, WindowAdminDelStuDelete);

	TCHAR WindowAdminDelStuSNO[] = { TEXT("学号") };
	TCHAR WindowAdminDelStuSNAME[] = { TEXT("姓名") };
	TCHAR WindowAdminDelStuSGENDER[] = { TEXT("性别") };
	TCHAR WindowAdminDelStuSMAJOR[] = { TEXT("专业") };
	TCHAR WindowAdminDelStuSCLASS[] = { TEXT("班级") };
	TCHAR WindowAdminDelStuSTELE[] = { TEXT("电话") };
	TCHAR WindowAdminDelStuSEMAIL[] = { TEXT("邮箱") };
	TCHAR WindowAdminDelStuSOTHERS[] = { TEXT("备注") };
	int WindowAdminDelStuInfoColumnL = 75;
	int WindowAdminDelStuInfoColumnU = 120;
	int WindowAdminDelStuInfoColumnX = 0;
	int WindowAdminDelStuInfoColumnY = 30;

	//学号
	int WindowAdminDelStuSNOL = WindowAdminDelStuInfoColumnL + WindowAdminDelStuInfoColumnX;
	int WindowAdminDelStuSNOU = WindowAdminDelStuInfoColumnU;
	outtextxy(WindowAdminDelStuInfoColumnL, WindowAdminDelStuInfoColumnU, WindowAdminDelStuSNO);
	WindowAdminDelStuInfoColumnU += WindowAdminDelStuInfoColumnY;
	//姓名
	int WindowAdminDelStuSNAMEL = WindowAdminDelStuInfoColumnL + WindowAdminDelStuInfoColumnX;
	int WindowAdminDelStuSNAMEU = WindowAdminDelStuInfoColumnU;
	outtextxy(WindowAdminDelStuInfoColumnL, WindowAdminDelStuInfoColumnU, WindowAdminDelStuSNAME);
	WindowAdminDelStuInfoColumnU += WindowAdminDelStuInfoColumnY;
	//性别
	int WindowAdminDelStuSGENDERL = WindowAdminDelStuInfoColumnL + WindowAdminDelStuInfoColumnX;
	int WindowAdminDelStuSGENDERU = WindowAdminDelStuInfoColumnU;
	outtextxy(WindowAdminDelStuInfoColumnL, WindowAdminDelStuInfoColumnU, WindowAdminDelStuSGENDER);
	WindowAdminDelStuInfoColumnU += WindowAdminDelStuInfoColumnY;
	//专业
	int WindowAdminDelStuSMAJORL = WindowAdminDelStuInfoColumnL + WindowAdminDelStuInfoColumnX;
	int WindowAdminDelStuSMAJORU = WindowAdminDelStuInfoColumnU;
	outtextxy(WindowAdminDelStuInfoColumnL, WindowAdminDelStuInfoColumnU, WindowAdminDelStuSMAJOR);
	WindowAdminDelStuInfoColumnU += WindowAdminDelStuInfoColumnY;
	//班级
	int WindowAdminDelStuSCLASSL = WindowAdminDelStuInfoColumnL + WindowAdminDelStuInfoColumnX;
	int WindowAdminDelStuSCLASSU = WindowAdminDelStuInfoColumnU;
	outtextxy(WindowAdminDelStuInfoColumnL, WindowAdminDelStuInfoColumnU, WindowAdminDelStuSCLASS);
	WindowAdminDelStuInfoColumnU += WindowAdminDelStuInfoColumnY;
	//电话
	int WindowAdminDelStuSTELEL = WindowAdminDelStuInfoColumnL + WindowAdminDelStuInfoColumnX;
	int WindowAdminDelStuSTELEU = WindowAdminDelStuInfoColumnU;
	outtextxy(WindowAdminDelStuInfoColumnL, WindowAdminDelStuInfoColumnU, WindowAdminDelStuSTELE);
	WindowAdminDelStuInfoColumnU += WindowAdminDelStuInfoColumnY;
	//邮箱
	int WindowAdminDelStuSEMAILL = WindowAdminDelStuInfoColumnL + WindowAdminDelStuInfoColumnX;
	int WindowAdminDelStuSEMAILU = WindowAdminDelStuInfoColumnU;
	outtextxy(WindowAdminDelStuInfoColumnL, WindowAdminDelStuInfoColumnU, WindowAdminDelStuSEMAIL);
	WindowAdminDelStuInfoColumnU += WindowAdminDelStuInfoColumnY;
	//备注
	int WindowAdminDelStuSOTHERSL = WindowAdminDelStuInfoColumnL + WindowAdminDelStuInfoColumnX;
	int WindowAdminDelStuSOTHERSU = WindowAdminDelStuInfoColumnU;
	outtextxy(WindowAdminDelStuInfoColumnL, WindowAdminDelStuInfoColumnU, WindowAdminDelStuSOTHERS);
	WindowAdminDelStuInfoColumnU = 120;

	//查询等待删除的学生
	char sql_WindowAdminAllDelStu[MAXLEN] = { "SELECT * FROM STUDENT;" };
	LogPrint(DEBUG, (char*)sql_WindowAdminAllDelStu);
	exec = sqlite3_exec(DB, sql_WindowAdminAllDelStu, sql_CallBack_Func_InitItemNum, 0, &errmsg);
	if (exec != SQLITE_OK)//SQLITE_ABORT
	{
		//如果出错，就会回到这里来	
		char Msg_sqliteFailedToSelectDelStu[] = { "FAILED TO INIT Stu ItemNum" };
		LogPrint(ERROR, Msg_sqliteFailedToSelectDelStu);
		LogPrint(ERROR, errmsg);
	}
	else//SQLITE_OK
	{
		char Msg_sqliteSucceedToSelectDelStu[] = { "SUCCEED IN INITDelStu ItemNum" };
		LogPrint(INFO, Msg_sqliteSucceedToSelectDelStu);

		ItemTempCount = 0;
		ItemRestri = 1;
		ItemPage = (ItemNum / ItemRestri) + 1;

		ItemCurrentPage = 1;
		Start = (ItemCurrentPage - 1)*ItemRestri + 1;
		End = (ItemCurrentPage)*ItemRestri;

		exec = sqlite3_exec(DB, sql_WindowAdminAllDelStu, sql_CallBack_Func_Admin_DelStu, 0, &errmsg);
	}

	//int PageNum_INT = ItemCurrentPage;

	//后续可能需要自己整一套itoa的东西
	//不过反正是在win上不用考虑跨平台问题，反正定死是MSVC了，那就随便用itoa吧

	TCHAR PageNum_TCHAR[3 + 1];
	char PageNum_CHAR[3 + 1];

	//今日不造轮子

	//显示初始状态的页面


	MOUSEMSG mouseadminDelStu;
	while (true)
	{
		mouseadminDelStu = GetMouseMsg();
		switch (mouseadminDelStu.uMsg)
		{
		case WM_LBUTTONDOWN:
		{
			if (mouseadminDelStu.x >= WindowAdminDelStuBackL && mouseadminDelStu.x <= WindowAdminDelStuBackR && mouseadminDelStu.y >= WindowAdminDelStuBackU && mouseadminDelStu.y <= WindowAdminDelStuBackD)
			{//BackButton
				char Msg_AdminDelStuGoAdmin[] = { "request a AdminDelStuGoAdmin" };
				LogPrint(INFO, Msg_AdminDelStuGoAdmin);
				WindowAdmin();
			}
			else if (mouseadminDelStu.x >= WindowAdminDelStuPageUpL && mouseadminDelStu.x <= WindowAdminDelStuPageUpR && mouseadminDelStu.y >= WindowAdminDelStuPageUpU && mouseadminDelStu.y <= WindowAdminDelStuPageUpD)
			{//Msg_WindowAdminDelStuPageUp
				char Msg_WindowAdminDelStuPageUp[] = { "Msg_WindowAdminDelStuPageUp-上一页" };
				LogPrint(INFO, Msg_WindowAdminDelStuPageUp);
				if (ItemCurrentPage - 1 >= 1)
				{
					setfillcolor(DARKGRAY);
					solidrectangle(WindowAdminDelStuInfoColumnL + 40, WindowAdminDelStuViewAllU + 1, WindowAdminDelStuViewAllR - 1, WindowAdminDelStuViewAllD - 1);
					ItemTempCount = 0;
					ItemCurrentPage -= 1;
					Start = (ItemCurrentPage - 1)*ItemRestri + 1;
					End = (ItemCurrentPage)*ItemRestri;
					exec = sqlite3_exec(DB, sql_WindowAdminAllDelStu, sql_CallBack_Func_Admin_DelStu, 0, &errmsg);
				}
			}
			else if (mouseadminDelStu.x >= WindowAdminDelStuPageDownL && mouseadminDelStu.x <= WindowAdminDelStuPageDownR && mouseadminDelStu.y >= WindowAdminDelStuPageDownU && mouseadminDelStu.y <= WindowAdminDelStuPageDownD)
			{//Msg_WindowAdminDelStuPageDown
				char Msg_WindowAdminDelStuPageDown[] = { "Msg_WindowAdminDelStuPageDown-下一页" };
				LogPrint(INFO, Msg_WindowAdminDelStuPageDown);
				if (ItemCurrentPage + 1 <= ItemPage)
				{
					setfillcolor(DARKGRAY);
					solidrectangle(WindowAdminDelStuInfoColumnL + 40, WindowAdminDelStuViewAllU + 1, WindowAdminDelStuViewAllR - 1, WindowAdminDelStuViewAllD - 1);
					ItemTempCount = 0;
					ItemCurrentPage += 1;
					Start = (ItemCurrentPage - 1)*ItemRestri + 1;
					End = (ItemCurrentPage)*ItemRestri;
					exec = sqlite3_exec(DB, sql_WindowAdminAllDelStu, sql_CallBack_Func_Admin_DelStu, 0, &errmsg);
				}
			}
			else if (mouseadminDelStu.x >= WindowAdminDelStuPageSelectL && mouseadminDelStu.x <= WindowAdminDelStuPageSelectR && mouseadminDelStu.y >= WindowAdminDelStuPageSelectU && mouseadminDelStu.y <= WindowAdminDelStuPageSelectD)
			{//Msg_WindowAdminDelStuPageSelect
				//是废的
				char Msg_WindowAdminDelStuPageSelect[] = { "Msg_WindowAdminDelStuPageSelect-选择页面" };
				LogPrint(INFO, Msg_WindowAdminDelStuPageSelect);
				InputBox(PageNum_TCHAR, 3 + 1, TEXT("请输入想要跳转到的页面(不超过3个字符)"));
				outtextxy(WindowAdminDelStuPageSelectL + 6, WindowAdminDelStuPageSelectU + 40, PageNum_TCHAR);
				TcharToChar(PageNum_TCHAR, PageNum_CHAR);
				LogPrint(DEBUG, PageNum_CHAR);
			}
			else if (mouseadminDelStu.x >= WindowAdminDelStuDeleteL && mouseadminDelStu.x <= WindowAdminDelStuDeleteR && mouseadminDelStu.y >= WindowAdminDelStuDeleteU && mouseadminDelStu.y <= WindowAdminDelStuDeleteD)
			{//Msg_WindowAdminDelStuDelete
				char Msg_WindowAdminDelStuDelete[] = { "Msg_WindowAdminDelStuDelete-删号某同学" };
				LogPrint(INFO, Msg_WindowAdminDelStuDelete);
				char sql_WindowAdminDelStuP1[MAXLEN] = { "DELETE FROM STUDENT WHERE(SNO=='" };
				char sql_WindowAdminDelStuP2[MAXLEN] = { "');" };
				char sql_WindowAdminDelStuP3[MAXLEN] = { "DELETE FROM USER WHERE(UID=='" };
				char sql_WindowAdminDelStuP4[MAXLEN] = { "');" };
				strcat(sql_WindowAdminDelStuP1, CurrentItem);
				strcat(sql_WindowAdminDelStuP1, sql_WindowAdminDelStuP2);
				strcat(sql_WindowAdminDelStuP3, CurrentItem);
				strcat(sql_WindowAdminDelStuP3, sql_WindowAdminDelStuP4);
				const char* sql_WindowAdminDelStu = sql_WindowAdminDelStuP1;
				const char* sql_WindowAdminDelStuUser = sql_WindowAdminDelStuP3;
				LogPrint(DEBUG, (char*)sql_WindowAdminDelStu);
				LogPrint(DEBUG, (char*)sql_WindowAdminDelStuUser);
				exec = sqlite3_exec(DB, sql_WindowAdminDelStu, sql_CallBack_Func_InitItemNum, 0, &errmsg);
				if (exec != SQLITE_OK)//SQLITE_ABORT
				{
					//如果出错，就会回到这里来	
					char Msg_sqliteFailedToDelStu[] = { "FAILED TO DelStu" };
					LogPrint(ERROR, Msg_sqliteFailedToDelStu);
					LogPrint(ERROR, errmsg);
				}
				else//SQLITE_OK
				{
					char Msg_sqliteSucceedInDelStu[] = { "SUCCEED IN DelStu" };
					LogPrint(INFO, Msg_sqliteSucceedInDelStu);
					exec = sqlite3_exec(DB, sql_WindowAdminDelStuUser, sql_CallBack_Func_InitItemNum, 0, &errmsg);
					if (exec != SQLITE_OK)//SQLITE_ABORT
					{
						//如果出错，就会回到这里来	
						char Msg_sqliteFailedToDelStu2[] = { "FAILED TO DelStu" };
						LogPrint(ERROR, Msg_sqliteFailedToDelStu2);
						LogPrint(ERROR, errmsg);
					}
					else//SQLITE_OK
					{
						char Msg_sqliteSucceedInDelStu2[] = { "SUCCEED IN DelStu" };
						LogPrint(INFO, Msg_sqliteSucceedInDelStu2);
					}
				}
				WindowAdmin();
			}
		}
		default:
			break;
		}
	}

	return 0;
}

int WindowAdminNewTea(void)
{
	cleardevice();
	FlushMouseMsgBuffer();
	setcliprgn(NULL);
	setpolyfillmode(WINDING);
	setfillcolor(RED);
	solidrectangle(0, 0, W, H);
	Sleep(350);

	setbkmode(TRANSPARENT);
	char Msg_initWindowAdminNewTea[] = { "initWindowAdminNewTea" };
	LogPrint(INFO, Msg_initWindowAdminNewTea);

	TCHAR WindowAdminNewTeaSlogan[] = { TEXT("指挥官，欢迎招聘！") };
	outtextxy(330, 20, WindowAdminNewTeaSlogan);

	TCHAR WindowAdminNewTeaTNO[] = { TEXT("教师号") };
	TCHAR WindowAdminNewTeaTNAME[] = { TEXT("姓名") };
	TCHAR WindowAdminNewTeaTGENDER[] = { TEXT("性别") };
	TCHAR WindowAdminNewTeaTTITLE[] = { TEXT("职称") };
	TCHAR WindowAdminNewTeaTFIELD[] = { TEXT("方向") };
	TCHAR WindowAdminNewTeaTTELE[] = { TEXT("电话") };
	TCHAR WindowAdminNewTeaTEMAIL[] = { TEXT("邮箱") };
	TCHAR WindowAdminNewTeaTCHAIRMAN[] = { TEXT("是否系主任") };

	int WindowAdminNewTeaInfoColumnL = 225;
	int WindowAdminNewTeaInfoColumnU = 80;
	int WindowAdminNewTeaInfoColumnX = 100;
	int WindowAdminNewTeaInfoColumnY = 30;

	TCHAR WindowAdminNewTeaNote[] = { TEXT("请您完善下列学生信息，其中教师号是必填项") };
	outtextxy(WindowAdminNewTeaInfoColumnL, WindowAdminNewTeaInfoColumnU - 30, WindowAdminNewTeaNote);

	//教师号框
	int WindowAdminNewTeaTNOL = WindowAdminNewTeaInfoColumnL + WindowAdminNewTeaInfoColumnX;
	int WindowAdminNewTeaTNOU = WindowAdminNewTeaInfoColumnU;
	int WindowAdminNewTeaTNOR = WindowAdminNewTeaTNOL + 250;
	int WindowAdminNewTeaTNOD = WindowAdminNewTeaTNOU + 20;
	rectangle(WindowAdminNewTeaTNOL, WindowAdminNewTeaTNOU, WindowAdminNewTeaTNOR, WindowAdminNewTeaTNOD);
	outtextxy(WindowAdminNewTeaInfoColumnL, WindowAdminNewTeaInfoColumnU, WindowAdminNewTeaTNO);
	WindowAdminNewTeaInfoColumnU += WindowAdminNewTeaInfoColumnY;
	//姓名框
	int WindowAdminNewTeaTNAMEL = WindowAdminNewTeaInfoColumnL + WindowAdminNewTeaInfoColumnX;
	int WindowAdminNewTeaTNAMEU = WindowAdminNewTeaInfoColumnU;
	int WindowAdminNewTeaTNAMER = WindowAdminNewTeaTNAMEL + 250;
	int WindowAdminNewTeaTNAMED = WindowAdminNewTeaTNAMEU + 20;
	rectangle(WindowAdminNewTeaTNAMEL, WindowAdminNewTeaTNAMEU, WindowAdminNewTeaTNAMER, WindowAdminNewTeaTNAMED);
	outtextxy(WindowAdminNewTeaInfoColumnL, WindowAdminNewTeaInfoColumnU, WindowAdminNewTeaTNAME);
	WindowAdminNewTeaInfoColumnU += WindowAdminNewTeaInfoColumnY;
	//性别框
	int WindowAdminNewTeaTGENDERL = WindowAdminNewTeaInfoColumnL + WindowAdminNewTeaInfoColumnX;
	int WindowAdminNewTeaTGENDERU = WindowAdminNewTeaInfoColumnU;
	int WindowAdminNewTeaTGENDERR = WindowAdminNewTeaTGENDERL + 250;
	int WindowAdminNewTeaTGENDERD = WindowAdminNewTeaTGENDERU + 20;
	rectangle(WindowAdminNewTeaTGENDERL, WindowAdminNewTeaTGENDERU, WindowAdminNewTeaTGENDERR, WindowAdminNewTeaTGENDERD);
	outtextxy(WindowAdminNewTeaInfoColumnL, WindowAdminNewTeaInfoColumnU, WindowAdminNewTeaTGENDER);
	WindowAdminNewTeaInfoColumnU += WindowAdminNewTeaInfoColumnY;
	//职称框
	int WindowAdminNewTeaTTITLEL = WindowAdminNewTeaInfoColumnL + WindowAdminNewTeaInfoColumnX;
	int WindowAdminNewTeaTTITLEU = WindowAdminNewTeaInfoColumnU;
	int WindowAdminNewTeaTTITLER = WindowAdminNewTeaTTITLEL + 250;
	int WindowAdminNewTeaTTITLED = WindowAdminNewTeaTTITLEU + 20;
	rectangle(WindowAdminNewTeaTTITLEL, WindowAdminNewTeaTTITLEU, WindowAdminNewTeaTTITLER, WindowAdminNewTeaTTITLED);
	outtextxy(WindowAdminNewTeaInfoColumnL, WindowAdminNewTeaInfoColumnU, WindowAdminNewTeaTTITLE);
	WindowAdminNewTeaInfoColumnU += WindowAdminNewTeaInfoColumnY;
	//方向框
	int WindowAdminNewTeaTFIELDL = WindowAdminNewTeaInfoColumnL + WindowAdminNewTeaInfoColumnX;
	int WindowAdminNewTeaTFIELDU = WindowAdminNewTeaInfoColumnU;
	int WindowAdminNewTeaTFIELDR = WindowAdminNewTeaTFIELDL + 250;
	int WindowAdminNewTeaTFIELDD = WindowAdminNewTeaTFIELDU + 20;
	rectangle(WindowAdminNewTeaTFIELDL, WindowAdminNewTeaTFIELDU, WindowAdminNewTeaTFIELDR, WindowAdminNewTeaTFIELDD);
	outtextxy(WindowAdminNewTeaInfoColumnL, WindowAdminNewTeaInfoColumnU, WindowAdminNewTeaTFIELD);
	WindowAdminNewTeaInfoColumnU += WindowAdminNewTeaInfoColumnY;
	//电话框
	int WindowAdminNewTeaTTELEL = WindowAdminNewTeaInfoColumnL + WindowAdminNewTeaInfoColumnX;
	int WindowAdminNewTeaTTELEU = WindowAdminNewTeaInfoColumnU;
	int WindowAdminNewTeaTTELER = WindowAdminNewTeaTTELEL + 250;
	int WindowAdminNewTeaTTELED = WindowAdminNewTeaTTELEU + 20;
	rectangle(WindowAdminNewTeaTTELEL, WindowAdminNewTeaTTELEU, WindowAdminNewTeaTTELER, WindowAdminNewTeaTTELED);
	outtextxy(WindowAdminNewTeaInfoColumnL, WindowAdminNewTeaInfoColumnU, WindowAdminNewTeaTTELE);
	WindowAdminNewTeaInfoColumnU += WindowAdminNewTeaInfoColumnY;
	//邮箱框
	int WindowAdminNewTeaTEMAILL = WindowAdminNewTeaInfoColumnL + WindowAdminNewTeaInfoColumnX;
	int WindowAdminNewTeaTEMAILU = WindowAdminNewTeaInfoColumnU;
	int WindowAdminNewTeaTEMAILR = WindowAdminNewTeaTEMAILL + 250;
	int WindowAdminNewTeaTEMAILD = WindowAdminNewTeaTEMAILU + 20;
	rectangle(WindowAdminNewTeaTEMAILL, WindowAdminNewTeaTEMAILU, WindowAdminNewTeaTEMAILR, WindowAdminNewTeaTEMAILD);
	outtextxy(WindowAdminNewTeaInfoColumnL, WindowAdminNewTeaInfoColumnU, WindowAdminNewTeaTEMAIL);
	WindowAdminNewTeaInfoColumnU += WindowAdminNewTeaInfoColumnY;
	//系主任确认框
	int flag_TCHAIRMAN = 0;//0为非系主任，1为系主任
	int WindowAdminNewTeaTCHAIRMANL = WindowAdminNewTeaInfoColumnL + WindowAdminNewTeaInfoColumnX;
	int WindowAdminNewTeaTCHAIRMANU = WindowAdminNewTeaInfoColumnU + 20;
	int WindowAdminNewTeaTCHAIRMANR = WindowAdminNewTeaTCHAIRMANL + 250;
	int WindowAdminNewTeaTCHAIRMAND = WindowAdminNewTeaTCHAIRMANU + 20;
	int WindowAdminNewTeaTCHAIRMAN_Off_L = WindowAdminNewTeaTCHAIRMANL + 5;
	int WindowAdminNewTeaTCHAIRMAN_Off_R = WindowAdminNewTeaTCHAIRMAN_Off_L + 50;
	int WindowAdminNewTeaTCHAIRMAN_On_R = WindowAdminNewTeaTCHAIRMANR - 5;
	int WindowAdminNewTeaTCHAIRMAN_On_L = WindowAdminNewTeaTCHAIRMAN_On_R - 50;
	int WindowAdminNewTeaTCHAIRMAN_OnOff_U = WindowAdminNewTeaTCHAIRMANU + 2;
	int WindowAdminNewTeaTCHAIRMAN_OnOff_D = WindowAdminNewTeaTCHAIRMAND - 2;
	rectangle(WindowAdminNewTeaTCHAIRMANL, WindowAdminNewTeaTCHAIRMANU, WindowAdminNewTeaTCHAIRMANR, WindowAdminNewTeaTCHAIRMAND);
	outtextxy(WindowAdminNewTeaInfoColumnL, WindowAdminNewTeaInfoColumnU+22, WindowAdminNewTeaTCHAIRMAN);

	TCHAR WindowAdminNewTeaSubmit[] = { TEXT("确认提交") };

	//确认提交
	int WindowAdminNewTeaSubmitL = WindowAdminNewTeaInfoColumnL + WindowAdminNewTeaInfoColumnX;
	int WindowAdminNewTeaSubmitU = WindowAdminNewTeaInfoColumnU + 70;
	int WindowAdminNewTeaSubmitR = WindowAdminNewTeaSubmitL + 125;
	int WindowAdminNewTeaSubmitD = WindowAdminNewTeaSubmitU + 35;
	rectangle(WindowAdminNewTeaSubmitL, WindowAdminNewTeaSubmitU, WindowAdminNewTeaSubmitR, WindowAdminNewTeaSubmitD);
	setfillcolor(DARKGRAY);
	solidrectangle(WindowAdminNewTeaSubmitL + 1, WindowAdminNewTeaSubmitU + 1, WindowAdminNewTeaSubmitR - 1, WindowAdminNewTeaSubmitD - 1);
	outtextxy(WindowAdminNewTeaSubmitL + 30, WindowAdminNewTeaSubmitU + 10, WindowAdminNewTeaSubmit);

	//返回按钮
	int WindowAdminNewTeaBackL = 35;
	int WindowAdminNewTeaBackU = 20;
	int WindowAdminNewTeaBackR = WindowAdminNewTeaBackL + 75;
	int WindowAdminNewTeaBackD = WindowAdminNewTeaBackU + 25;
	rectangle(WindowAdminNewTeaBackL, WindowAdminNewTeaBackU, WindowAdminNewTeaBackR, WindowAdminNewTeaBackD);
	TCHAR WindowAdminBackNewTeaButton[] = { TEXT("返回") };
	//返回图标
	outtextxy(WindowAdminNewTeaBackL + 35, WindowAdminNewTeaBackU + 4, WindowAdminBackNewTeaButton);
	setfillcolor(YELLOW);
	int BackCircleX = WindowAdminNewTeaBackL + 5;
	int BackCircleY = WindowAdminNewTeaBackU + 5;
	solidcircle(BackCircleX + 7, BackCircleY + 7, 7);
	setfillcolor(RED);
	solidrectangle(BackCircleX + 5, BackCircleY + 6, BackCircleX + 12, BackCircleY + 8);
	POINT BackTriangle[] = { {BackCircleX + 1, BackCircleY + 7}, {BackCircleX + 6, BackCircleY + 3}, {BackCircleX + 6, BackCircleY + 11} };
	solidpolygon(BackTriangle, 3);

	setfillcolor(BLACK);
	solidrectangle(WindowAdminNewTeaTNOL + 1, WindowAdminNewTeaTNOU + 1, WindowAdminNewTeaTNOR - 1, WindowAdminNewTeaTNOD - 1);
	solidrectangle(WindowAdminNewTeaTNAMEL + 1, WindowAdminNewTeaTNAMEU + 1, WindowAdminNewTeaTNAMER - 1, WindowAdminNewTeaTNAMED - 1);
	solidrectangle(WindowAdminNewTeaTGENDERL + 1, WindowAdminNewTeaTGENDERU + 1, WindowAdminNewTeaTGENDERR - 1, WindowAdminNewTeaTGENDERD - 1);
	solidrectangle(WindowAdminNewTeaTTITLEL + 1, WindowAdminNewTeaTTITLEU + 1, WindowAdminNewTeaTTITLER - 1, WindowAdminNewTeaTTITLED - 1);
	solidrectangle(WindowAdminNewTeaTFIELDL + 1, WindowAdminNewTeaTFIELDU + 1, WindowAdminNewTeaTFIELDR - 1, WindowAdminNewTeaTFIELDD - 1);
	solidrectangle(WindowAdminNewTeaTTELEL + 1, WindowAdminNewTeaTTELEU + 1, WindowAdminNewTeaTTELER - 1, WindowAdminNewTeaTTELED - 1);
	solidrectangle(WindowAdminNewTeaTEMAILL + 1, WindowAdminNewTeaTEMAILU + 1, WindowAdminNewTeaTEMAILR - 1, WindowAdminNewTeaTEMAILD - 1);
	setfillcolor(BLACK);
	solidrectangle(WindowAdminNewTeaTCHAIRMANL + 1, WindowAdminNewTeaTCHAIRMANU + 1, WindowAdminNewTeaTCHAIRMANR - 1, WindowAdminNewTeaTCHAIRMAND - 1);

	//在填色之后绘制开关框
	if (flag_TCHAIRMAN == 0)
	{
		//OffButton
		rectangle(WindowAdminNewTeaTCHAIRMAN_Off_L, WindowAdminNewTeaTCHAIRMAN_OnOff_U, WindowAdminNewTeaTCHAIRMAN_Off_R, WindowAdminNewTeaTCHAIRMAN_OnOff_D);
		setfillcolor(LIGHTRED);
		solidrectangle(WindowAdminNewTeaTCHAIRMAN_Off_L + 1, WindowAdminNewTeaTCHAIRMAN_OnOff_U + 1, WindowAdminNewTeaTCHAIRMAN_Off_R - 1, WindowAdminNewTeaTCHAIRMAN_OnOff_D - 1);
	}
	else
	{
		//OnButton
		rectangle(WindowAdminNewTeaTCHAIRMAN_On_L, WindowAdminNewTeaTCHAIRMAN_OnOff_U, WindowAdminNewTeaTCHAIRMAN_On_R, WindowAdminNewTeaTCHAIRMAN_OnOff_D);
		setfillcolor(GREEN);
		solidrectangle(WindowAdminNewTeaTCHAIRMAN_On_L + 1, WindowAdminNewTeaTCHAIRMAN_OnOff_U + 1, WindowAdminNewTeaTCHAIRMAN_On_R - 1, WindowAdminNewTeaTCHAIRMAN_OnOff_D - 1);
	}
	WindowAdminNewTeaInfoColumnU += WindowAdminNewTeaInfoColumnY;
	

	TCHAR TNO_TCHAR[13 + 1];
	TCHAR TNAME_TCHAR[13 + 1];
	TCHAR TGENDER_TCHAR[13 + 1];
	TCHAR TTITLE_TCHAR[13 + 1];
	TCHAR TFIELD_TCHAR[13 + 1];
	TCHAR TTELE_TCHAR[13 + 1];
	TCHAR TEMAIL_TCHAR[13 + 1];
	TCHAR TCHAIRMAN_TCHAR[13 + 1];

	char TNO_CHAR[13 + 1];
	char TNAME_CHAR[13 + 1];
	char TGENDER_CHAR[13 + 1];
	char TTITLE_CHAR[13 + 1];
	char TFIELD_CHAR[13 + 1];
	char TTELE_CHAR[13 + 1];
	char TEMAIL_CHAR[13 + 1];
	char TCHAIRMAN_CHAR[13 + 1];

	int flag_TNOIsFinish = 0;
	int flag_TNAMEIsFinish = 0;
	int flag_TGENDERIsFinish = 0;
	int flag_TTITLEIsFinish = 0;
	int flag_TFIELDIsFinish = 0;
	int flag_TTELEIsFinish = 0;
	int flag_TEMAILIsFinish = 0;
	int flag_TCHAIRMANIsFinish = 0;

	flag_TCHAIRMANIsFinish = 1;

	MOUSEMSG mouseadminNewTea;
	while (true)
	{
		mouseadminNewTea = GetMouseMsg();
		switch (mouseadminNewTea.uMsg)
		{
		case WM_LBUTTONDOWN:
		{
			if (mouseadminNewTea.x >= WindowAdminNewTeaBackL && mouseadminNewTea.x <= WindowAdminNewTeaBackR && mouseadminNewTea.y >= WindowAdminNewTeaBackU && mouseadminNewTea.y <= WindowAdminNewTeaBackD)
			{//BackButton
				char Msg_AdminNewTeaGoAdmin[] = { "request a AdminNewTeaGoAdmin" };
				LogPrint(INFO, Msg_AdminNewTeaGoAdmin);
				WindowAdmin();
			}
			else if (mouseadminNewTea.x >= WindowAdminNewTeaTNOL && mouseadminNewTea.x <= WindowAdminNewTeaTNOR && mouseadminNewTea.y >= WindowAdminNewTeaTNOU && mouseadminNewTea.y <= WindowAdminNewTeaTNOD)
			{//Msg_WindowAdminNewTeaTNO
				setfillcolor(BLACK);
				solidrectangle(WindowAdminNewTeaTNOL + 1, WindowAdminNewTeaTNOU + 1, WindowAdminNewTeaTNOR - 1, WindowAdminNewTeaTNOD - 1);
				setfillcolor(RED);
				solidrectangle(WindowAdminNewTeaSubmitL + 174, WindowAdminNewTeaSubmitU + 9, W, H);
				char Msg_WindowAdminNewTeaTNO[] = { "Msg_WindowAdminNewTeaTNO-教师号" };
				LogPrint(INFO, Msg_WindowAdminNewTeaTNO);
				InputBox(TNO_TCHAR, 13 + 1, TEXT("请输入教师号(不超过13个字符)"));
				outtextxy(WindowAdminNewTeaTNOL + 1, WindowAdminNewTeaTNOU + 2, TNO_TCHAR);
				TcharToChar(TNO_TCHAR, TNO_CHAR);
				if (TNO_CHAR[0] != 'T' || TNO_CHAR[1] != 'E' || TNO_CHAR[2] != 'A')
				{
					setfillcolor(BLACK);
					solidrectangle(WindowAdminNewTeaTNOL + 1, WindowAdminNewTeaTNOU + 1, WindowAdminNewTeaTNOR - 1, WindowAdminNewTeaTNOD - 1);
					TCHAR TEAWARN[] = { TEXT("请保证教师编号以TEA开头") };
					Sleep(250);
					outtextxy(WindowAdminNewTeaSubmitL + 175, WindowAdminNewTeaSubmitU + 10, TEAWARN);
				}
				LogPrint(DEBUG, TNO_CHAR);
				flag_TNOIsFinish = 1;
			}
			else if (mouseadminNewTea.x >= WindowAdminNewTeaTNAMEL && mouseadminNewTea.x <= WindowAdminNewTeaTNAMER && mouseadminNewTea.y >= WindowAdminNewTeaTNAMEU && mouseadminNewTea.y <= WindowAdminNewTeaTNAMED)
			{//Msg_WindowAdminNewTeaTNAME
				setfillcolor(BLACK);
				solidrectangle(WindowAdminNewTeaTNAMEL + 1, WindowAdminNewTeaTNAMEU + 1, WindowAdminNewTeaTNAMER - 1, WindowAdminNewTeaTNAMED - 1);
				char Msg_WindowAdminNewTeaTNAME[] = { "Msg_WindowAdminNewTeaTNAME-姓名" };
				LogPrint(INFO, Msg_WindowAdminNewTeaTNAME);
				InputBox(TNAME_TCHAR, 13 + 1, TEXT("请输入姓名(不超过13个字符)"));
				outtextxy(WindowAdminNewTeaTNAMEL + 1, WindowAdminNewTeaTNAMEU + 2, TNAME_TCHAR);
				TcharToChar(TNAME_TCHAR, TNAME_CHAR);
				LogPrint(DEBUG, TNAME_CHAR);
				flag_TNAMEIsFinish = 1;
			}
			else if (mouseadminNewTea.x >= WindowAdminNewTeaTGENDERL && mouseadminNewTea.x <= WindowAdminNewTeaTGENDERR && mouseadminNewTea.y >= WindowAdminNewTeaTGENDERU && mouseadminNewTea.y <= WindowAdminNewTeaTGENDERD)
			{//Msg_WindowAdminNewTeaTGENDER
				setfillcolor(BLACK);
				solidrectangle(WindowAdminNewTeaTGENDERL + 1, WindowAdminNewTeaTGENDERU + 1, WindowAdminNewTeaTGENDERR - 1, WindowAdminNewTeaTGENDERD - 1);
				char Msg_WindowAdminNewTeaTGENDER[] = { "Msg_WindowAdminNewTeaTGENDER-性别" };
				LogPrint(INFO, Msg_WindowAdminNewTeaTGENDER);
				InputBox(TGENDER_TCHAR, 13 + 1, TEXT("请输入性别(不超过13个字符)"));
				outtextxy(WindowAdminNewTeaTGENDERL + 1, WindowAdminNewTeaTGENDERU + 2, TGENDER_TCHAR);
				TcharToChar(TGENDER_TCHAR, TGENDER_CHAR);
				LogPrint(DEBUG, TGENDER_CHAR);
				flag_TGENDERIsFinish = 1;
			}
			else if (mouseadminNewTea.x >= WindowAdminNewTeaTTITLEL && mouseadminNewTea.x <= WindowAdminNewTeaTTITLER && mouseadminNewTea.y >= WindowAdminNewTeaTTITLEU && mouseadminNewTea.y <= WindowAdminNewTeaTTITLED)
			{//Msg_WindowAdminNewTeaTTITLE
				setfillcolor(BLACK);
				solidrectangle(WindowAdminNewTeaTTITLEL + 1, WindowAdminNewTeaTTITLEU + 1, WindowAdminNewTeaTTITLER - 1, WindowAdminNewTeaTTITLED - 1);
				char Msg_WindowAdminNewTeaTTITLE[] = { "Msg_WindowAdminNewTeaTTITLE-职称" };
				LogPrint(INFO, Msg_WindowAdminNewTeaTTITLE);
				InputBox(TTITLE_TCHAR, 13 + 1, TEXT("请输入职称(不超过13个字符)"));
				outtextxy(WindowAdminNewTeaTTITLEL + 1, WindowAdminNewTeaTTITLEU + 2, TTITLE_TCHAR);
				TcharToChar(TTITLE_TCHAR, TTITLE_CHAR);
				LogPrint(DEBUG, TTITLE_CHAR);
				flag_TTITLEIsFinish = 1;
			}
			else if (mouseadminNewTea.x >= WindowAdminNewTeaTFIELDL && mouseadminNewTea.x <= WindowAdminNewTeaTFIELDR && mouseadminNewTea.y >= WindowAdminNewTeaTFIELDU && mouseadminNewTea.y <= WindowAdminNewTeaTFIELDD)
			{//Msg_WindowAdminNewTeaTFIELD
				setfillcolor(BLACK);
				solidrectangle(WindowAdminNewTeaTFIELDL + 1, WindowAdminNewTeaTFIELDU + 1, WindowAdminNewTeaTFIELDR - 1, WindowAdminNewTeaTFIELDD - 1);
				char Msg_WindowAdminNewTeaTFIELD[] = { "Msg_WindowAdminNewTeaTFIELD-方向" };
				LogPrint(INFO, Msg_WindowAdminNewTeaTFIELD);
				InputBox(TFIELD_TCHAR, 13 + 1, TEXT("请输入方向(不超过13个字符)"));
				outtextxy(WindowAdminNewTeaTFIELDL + 1, WindowAdminNewTeaTFIELDU + 2, TFIELD_TCHAR);
				TcharToChar(TFIELD_TCHAR, TFIELD_CHAR);
				LogPrint(DEBUG, TFIELD_CHAR);
				flag_TFIELDIsFinish = 1;
			}
			else if (mouseadminNewTea.x >= WindowAdminNewTeaTTELEL && mouseadminNewTea.x <= WindowAdminNewTeaTTELER && mouseadminNewTea.y >= WindowAdminNewTeaTTELEU && mouseadminNewTea.y <= WindowAdminNewTeaTTELED)
			{//Msg_WindowAdminNewTeaTTELE
				setfillcolor(BLACK);
				solidrectangle(WindowAdminNewTeaTTELEL + 1, WindowAdminNewTeaTTELEU + 1, WindowAdminNewTeaTTELER - 1, WindowAdminNewTeaTTELED - 1);
				char Msg_WindowAdminNewTeaTTELE[] = { "Msg_WindowAdminNewTeaTTELE-电话" };
				LogPrint(INFO, Msg_WindowAdminNewTeaTTELE);
				InputBox(TTELE_TCHAR, 13 + 1, TEXT("请输入电话(不超过13个字符)"));
				outtextxy(WindowAdminNewTeaTTELEL + 1, WindowAdminNewTeaTTELEU + 2, TTELE_TCHAR);
				TcharToChar(TTELE_TCHAR, TTELE_CHAR);
				LogPrint(DEBUG, TTELE_CHAR);
				flag_TTELEIsFinish = 1;
			}
			else if (mouseadminNewTea.x >= WindowAdminNewTeaTEMAILL && mouseadminNewTea.x <= WindowAdminNewTeaTEMAILR && mouseadminNewTea.y >= WindowAdminNewTeaTEMAILU && mouseadminNewTea.y <= WindowAdminNewTeaTEMAILD)
			{//Msg_WindowAdminNewTeaTEMAIL
				setfillcolor(BLACK);
				solidrectangle(WindowAdminNewTeaTEMAILL + 1, WindowAdminNewTeaTEMAILU + 1, WindowAdminNewTeaTEMAILR - 1, WindowAdminNewTeaTEMAILD - 1);
				char Msg_WindowAdminNewTeaTEMAIL[] = { "Msg_WindowAdminNewTeaTEMAIL-邮箱" };
				LogPrint(INFO, Msg_WindowAdminNewTeaTEMAIL);
				InputBox(TEMAIL_TCHAR, 13 + 1, TEXT("请输入邮箱(不超过13个字符)"));
				outtextxy(WindowAdminNewTeaTEMAILL + 1, WindowAdminNewTeaTEMAILU + 2, TEMAIL_TCHAR);
				TcharToChar(TEMAIL_TCHAR, TEMAIL_CHAR);
				LogPrint(DEBUG, TEMAIL_CHAR);
				flag_TEMAILIsFinish = 1;
			}
			else if (mouseadminNewTea.x >= WindowAdminNewTeaTCHAIRMANL && mouseadminNewTea.x <= WindowAdminNewTeaTCHAIRMANR && mouseadminNewTea.y >= WindowAdminNewTeaTCHAIRMANU && mouseadminNewTea.y <= WindowAdminNewTeaTCHAIRMAND)
			{//Msg_WindowAdminNewTeaTCHAIRMAN
				setfillcolor(BLACK);
				solidrectangle(WindowAdminNewTeaTCHAIRMANL + 1, WindowAdminNewTeaTCHAIRMANU + 1, WindowAdminNewTeaTCHAIRMANR - 1, WindowAdminNewTeaTCHAIRMAND - 1);
				if (flag_TCHAIRMAN == 0)
				{//当前是Off，变为On
					flag_TCHAIRMAN = 1;
					rectangle(WindowAdminNewTeaTCHAIRMAN_On_L, WindowAdminNewTeaTCHAIRMAN_OnOff_U, WindowAdminNewTeaTCHAIRMAN_On_R, WindowAdminNewTeaTCHAIRMAN_OnOff_D);
					setfillcolor(GREEN);
					solidrectangle(WindowAdminNewTeaTCHAIRMAN_On_L + 1, WindowAdminNewTeaTCHAIRMAN_OnOff_U + 1, WindowAdminNewTeaTCHAIRMAN_On_R - 1, WindowAdminNewTeaTCHAIRMAN_OnOff_D - 1);
					char Msg_WindowAdminNewTeaTCHAIRMAN_Off2On[] = { "Msg_WindowAdminNewTeaTCHAIRMAN_Off2On" };
					LogPrint(INFO, Msg_WindowAdminNewTeaTCHAIRMAN_Off2On);
					flag_TCHAIRMANIsFinish = 1;
				}
				else
				{
					//当前是On，变为Off
					flag_TCHAIRMAN = 0;
					rectangle(WindowAdminNewTeaTCHAIRMAN_Off_L, WindowAdminNewTeaTCHAIRMAN_OnOff_U, WindowAdminNewTeaTCHAIRMAN_Off_R, WindowAdminNewTeaTCHAIRMAN_OnOff_D);
					setfillcolor(LIGHTRED);
					solidrectangle(WindowAdminNewTeaTCHAIRMAN_Off_L + 1, WindowAdminNewTeaTCHAIRMAN_OnOff_U + 1, WindowAdminNewTeaTCHAIRMAN_Off_R - 1, WindowAdminNewTeaTCHAIRMAN_OnOff_D - 1);
					char Msg_WindowAdminNewTeaTCHAIRMAN_On2Off[] = { "Msg_WindowAdminNewTeaTCHAIRMAN_On2Off" };
					LogPrint(INFO, Msg_WindowAdminNewTeaTCHAIRMAN_On2Off);
					flag_TCHAIRMANIsFinish = 1;
				}
			}
			else if (mouseadminNewTea.x >= WindowAdminNewTeaSubmitL && mouseadminNewTea.x <= WindowAdminNewTeaSubmitR && mouseadminNewTea.y >= WindowAdminNewTeaSubmitU && mouseadminNewTea.y <= WindowAdminNewTeaSubmitD)
			{//Msg_WindowAdminNewTeaSubmit
				setfillcolor(RED);
				solidrectangle(WindowAdminNewTeaSubmitL + 174, WindowAdminNewTeaSubmitU + 9, W, H);
				char Msg_WindowAdminNewTeaSubmit[] = { "Msg_WindowAdminNewTeaSubmit-提交" };
				LogPrint(INFO, Msg_WindowAdminNewTeaSubmit);
				//先判断会不会死掉
				//严格版本if (flag_TNOIsFinish == 1 && flag_TNAMEIsFinish == 1 && flag_TGENDERIsFinish == 1 && flag_TTITLEIsFinish == 1 && flag_TFIELDIsFinish == 1 && flag_TTELEIsFinish == 1 && flag_TEMAILIsFinish == 1 && flag_TCHAIRMANIsFinish == 1)
				if (flag_TNOIsFinish == 1)
				{
					//组装SQL语句
					char sql_WindowNewTeaP1[MAXLEN] = { "INSERT INTO TEACHER VALUES('" };
					char sql_WindowNewTeaP2[MAXLEN] = { "','" };
					char sql_WindowNewTeaP3[MAXLEN] = { "');" };
					char sql_WindowNewTea_IsChairman[3] = { "1" };
					char sql_WindowNewTea_NotChairman[3] = { "0" };
					strcat(sql_WindowNewTeaP1, TNO_CHAR);
					if (flag_TNAMEIsFinish == 1)
					{
						strcat(sql_WindowNewTeaP1, sql_WindowNewTeaP2);
						strcat(sql_WindowNewTeaP1, TNAME_CHAR);
					}
					else
					{
						strcat(sql_WindowNewTeaP1, sql_WindowNewTeaP2);
					}
					if (flag_TGENDERIsFinish == 1)
					{
						strcat(sql_WindowNewTeaP1, sql_WindowNewTeaP2);
						strcat(sql_WindowNewTeaP1, TGENDER_CHAR);
					}
					else
					{
						strcat(sql_WindowNewTeaP1, sql_WindowNewTeaP2);
					}
					if (flag_TTITLEIsFinish == 1)
					{
						strcat(sql_WindowNewTeaP1, sql_WindowNewTeaP2);
						strcat(sql_WindowNewTeaP1, TTITLE_CHAR);
					}
					else
					{
						strcat(sql_WindowNewTeaP1, sql_WindowNewTeaP2);
					}
					if (flag_TFIELDIsFinish == 1)
					{
						strcat(sql_WindowNewTeaP1, sql_WindowNewTeaP2);
						strcat(sql_WindowNewTeaP1, TFIELD_CHAR);
					}
					else
					{
						strcat(sql_WindowNewTeaP1, sql_WindowNewTeaP2);
					}
					if (flag_TTELEIsFinish == 1)
					{
						strcat(sql_WindowNewTeaP1, sql_WindowNewTeaP2);
						strcat(sql_WindowNewTeaP1, TTELE_CHAR);
					}
					else
					{
						strcat(sql_WindowNewTeaP1, sql_WindowNewTeaP2);
					}
					if (flag_TEMAILIsFinish == 1)
					{
						strcat(sql_WindowNewTeaP1, sql_WindowNewTeaP2);
						strcat(sql_WindowNewTeaP1, TEMAIL_CHAR);
					}
					else
					{
						strcat(sql_WindowNewTeaP1, sql_WindowNewTeaP2);
					}
					if (flag_TCHAIRMANIsFinish == 0)
					{
						//提示需要补全至少备注的字段
						char Msg_SubmitButNotFinishedTCHAIRMAN[] = { "Msg_SubmitButNotFinishedTCHAIRMAN" };
						LogPrint(WARN, Msg_SubmitButNotFinishedTCHAIRMAN);
					}
					else
					{
						if (flag_TCHAIRMAN == 1)//是系主任
						{
							strcat(sql_WindowNewTeaP1, sql_WindowNewTeaP2);
							strcat(sql_WindowNewTeaP1, sql_WindowNewTea_IsChairman);
							strcat(sql_WindowNewTeaP1, sql_WindowNewTeaP3);
						}
						else//不是系主任
						{
							strcat(sql_WindowNewTeaP1, sql_WindowNewTeaP2);
							strcat(sql_WindowNewTeaP1, sql_WindowNewTea_NotChairman);
							strcat(sql_WindowNewTeaP1, sql_WindowNewTeaP3);
						}

						const char *sql_WindowNewTea = sql_WindowNewTeaP1;
						LogPrint(DEBUG, (char*)sql_WindowNewTea);
						exec = sqlite3_exec(DB, sql_WindowNewTea, sql_CallBack_Func_NewSubmit, 0, &errmsg);
						//回调函数是空的
						if (exec != SQLITE_OK)//SQLITE_ABORT
						{
							//如果插入出错，就会回到这里来	
							char Msg_sqliteFailedToSelectLogin[] = { "FAILED TO INSERT TEACHER" };
							LogPrint(ERROR, Msg_sqliteFailedToSelectLogin);
							LogPrint(ERROR, errmsg);
						}
						else//SQLITE_OK
						{
							//如果插入成功，就会回到这里来	
							//组装完成并插入了这个学生
							char Msg_sqliteIsOpen[] = { "SUCCEED IN INSERT TEACHER" };
							LogPrint(INFO, Msg_sqliteIsOpen);

							//然后设定密码
							char TeaPassword_CHAR[13 + 1];
							TCHAR TeaPassword_TCHAR[13 + 1];
							InputBox(TeaPassword_TCHAR, 13 + 1, TEXT("请为该教师设定登陆密码(不超过13个字符)"));
							TcharToChar(TeaPassword_TCHAR, TeaPassword_CHAR);
							LogPrint(DEBUG, TeaPassword_CHAR);

							//设定密码这段可以再来一遍
							char sql_WindowNewTeaInsertUserP1[MAXLEN] = { "INSERT INTO USER VALUES('" };
							char sql_WindowNewTeaInsertUserP2[MAXLEN] = { "','" };
							char sql_WindowNewTeaInsertUserP3[MAXLEN] = { "'," };
							char sql_WindowNewTeaInsertUserP4[MAXLEN] = { ");" };
							char sql_WindowNewTeaInsertUserPTea[MAXLEN] = { "2" };
							strcat(sql_WindowNewTeaInsertUserP1, TNO_CHAR);
							strcat(sql_WindowNewTeaInsertUserP1, sql_WindowNewTeaInsertUserP2);
							strcat(sql_WindowNewTeaInsertUserP1, TeaPassword_CHAR);
							strcat(sql_WindowNewTeaInsertUserP1, sql_WindowNewTeaInsertUserP3);
							strcat(sql_WindowNewTeaInsertUserP1, sql_WindowNewTeaInsertUserPTea);
							strcat(sql_WindowNewTeaInsertUserP1, sql_WindowNewTeaInsertUserP4);
							const char *sql_WindowNewTeaInsertUser = sql_WindowNewTeaInsertUserP1;
							LogPrint(DEBUG, (char*)sql_WindowNewTeaInsertUser);
							exec = sqlite3_exec(DB, sql_WindowNewTeaInsertUser, sql_CallBack_Func_NewSubmit, 0, &errmsg);
							if (exec != SQLITE_OK)//SQLITE_ABORT
							{
								//如果插入出错，就会回到这里来	
								char Msg_sqliteFailedToSelectLogin2[] = { "FAILED TO INSERT USER" };
								LogPrint(ERROR, Msg_sqliteFailedToSelectLogin2);
								LogPrint(ERROR, errmsg);
							}
							else//SQLITE_OK
							{
								char Msg_sqliteIsOpen2[] = { "SUCCEED IN INSERT USER" };
								LogPrint(INFO, Msg_sqliteIsOpen2);
								setfillcolor(BLACK);
								solidrectangle(WindowAdminNewTeaTNOL + 1, WindowAdminNewTeaTNOU + 1, WindowAdminNewTeaTNOR - 1, WindowAdminNewTeaTNOD - 1);
								solidrectangle(WindowAdminNewTeaTNAMEL + 1, WindowAdminNewTeaTNAMEU + 1, WindowAdminNewTeaTNAMER - 1, WindowAdminNewTeaTNAMED - 1);
								solidrectangle(WindowAdminNewTeaTGENDERL + 1, WindowAdminNewTeaTGENDERU + 1, WindowAdminNewTeaTGENDERR - 1, WindowAdminNewTeaTGENDERD - 1);
								solidrectangle(WindowAdminNewTeaTTITLEL + 1, WindowAdminNewTeaTTITLEU + 1, WindowAdminNewTeaTTITLER - 1, WindowAdminNewTeaTTITLED - 1);
								solidrectangle(WindowAdminNewTeaTFIELDL + 1, WindowAdminNewTeaTFIELDU + 1, WindowAdminNewTeaTFIELDR - 1, WindowAdminNewTeaTFIELDD - 1);
								solidrectangle(WindowAdminNewTeaTTELEL + 1, WindowAdminNewTeaTTELEU + 1, WindowAdminNewTeaTTELER - 1, WindowAdminNewTeaTTELED - 1);
								solidrectangle(WindowAdminNewTeaTEMAILL + 1, WindowAdminNewTeaTEMAILU + 1, WindowAdminNewTeaTEMAILR - 1, WindowAdminNewTeaTEMAILD - 1);
								TCHAR NewTeaSuccess[] = { TEXT("创建用户成功") };
								Sleep(250);
								outtextxy(WindowAdminNewTeaSubmitL + 175, WindowAdminNewTeaSubmitU + 10, NewTeaSuccess);
							}
						}
					}
				}
			}
			else
			{
				;
			}
		}
		default:
			break;
		}
	}

	return 0;
}

int WindowAdminDelTea(void)
{
	cleardevice();
	FlushMouseMsgBuffer();
	setcliprgn(NULL);
	setpolyfillmode(WINDING);
	setfillcolor(RED);
	solidrectangle(0, 0, W, H);
	Sleep(350);

	setbkmode(TRANSPARENT);
	char Msg_initWindowAdminDelTea[] = { "initWindowAdminDelTea" };
	LogPrint(INFO, Msg_initWindowAdminDelTea);

	TCHAR WindowAdminDelTeaSlogan[] = { TEXT("指挥官，要下课谁？") };
	outtextxy(330, 20, WindowAdminDelTeaSlogan);

	TCHAR WindowAdminDelTeaPageUp[] = { TEXT("上一页") };
	TCHAR WindowAdminDelTeaPageDown[] = { TEXT("下一页") };
	TCHAR WindowAdminDelTeaDelete[] = { TEXT("剔除这个老师") };

	//概览窗口
	int WindowAdminDelTeaViewAllL = 50;
	int WindowAdminDelTeaViewAllU = 100;
	int WindowAdminDelTeaViewAllR = WindowAdminDelTeaViewAllL + 460;
	int WindowAdminDelTeaViewAllD = WindowAdminDelTeaViewAllU + 300;
	rectangle(WindowAdminDelTeaViewAllL, WindowAdminDelTeaViewAllU, WindowAdminDelTeaViewAllR, WindowAdminDelTeaViewAllD);

	//上一页按钮
	int WindowAdminDelTeaPageUpL = 550;
	int WindowAdminDelTeaPageUpU = 115;
	int WindowAdminDelTeaPageUpR = WindowAdminDelTeaPageUpL + 65;
	int WindowAdminDelTeaPageUpD = WindowAdminDelTeaPageUpU + 95;
	rectangle(WindowAdminDelTeaPageUpL, WindowAdminDelTeaPageUpU, WindowAdminDelTeaPageUpR, WindowAdminDelTeaPageUpD);

	//下一页按钮
	int WindowAdminDelTeaPageDownL = 685;
	int WindowAdminDelTeaPageDownU = 115;
	int WindowAdminDelTeaPageDownR = WindowAdminDelTeaPageDownL + 65;
	int WindowAdminDelTeaPageDownD = WindowAdminDelTeaPageDownU + 95;
	rectangle(WindowAdminDelTeaPageDownL, WindowAdminDelTeaPageDownU, WindowAdminDelTeaPageDownR, WindowAdminDelTeaPageDownD);

	//页面选择按钮
	int WindowAdminDelTeaPageSelectL = 640;
	int WindowAdminDelTeaPageSelectU = 115;
	int WindowAdminDelTeaPageSelectR = WindowAdminDelTeaPageSelectL + 20;
	int WindowAdminDelTeaPageSelectD = WindowAdminDelTeaPageSelectU + 95;
	rectangle(WindowAdminDelTeaPageSelectL, WindowAdminDelTeaPageSelectU, WindowAdminDelTeaPageSelectR, WindowAdminDelTeaPageSelectD);

	//开除按钮
	int WindowAdminDelTeaDeleteL = 550;
	int WindowAdminDelTeaDeleteU = 340;
	int WindowAdminDelTeaDeleteR = WindowAdminDelTeaDeleteL + 200;
	int WindowAdminDelTeaDeleteD = WindowAdminDelTeaDeleteU + 45;
	rectangle(WindowAdminDelTeaDeleteL, WindowAdminDelTeaDeleteU, WindowAdminDelTeaDeleteR, WindowAdminDelTeaDeleteD);

	//插画
	rectangle(550, 230, 750, 320);
	IMAGE DelTeaimg;
	loadimage(&DelTeaimg, TEXT("Pic_DelStuAndTea.bmp"));
	int DelTeaimgW, DelTeaimgH;
	DelTeaimgW = DelTeaimg.getwidth();
	DelTeaimgH = DelTeaimg.getheight();
	putimage(550, 230, &DelTeaimg);

	//返回按钮
	int WindowAdminDelTeaBackL = 35;
	int WindowAdminDelTeaBackU = 20;
	int WindowAdminDelTeaBackR = WindowAdminDelTeaBackL + 75;
	int WindowAdminDelTeaBackD = WindowAdminDelTeaBackU + 25;
	rectangle(WindowAdminDelTeaBackL, WindowAdminDelTeaBackU, WindowAdminDelTeaBackR, WindowAdminDelTeaBackD);
	TCHAR WindowAdminBackDelTeaButton[] = { TEXT("返回") };
	//返回图标
	outtextxy(WindowAdminDelTeaBackL + 35, WindowAdminDelTeaBackU + 4, WindowAdminBackDelTeaButton);
	setfillcolor(YELLOW);
	int BackCircleX = WindowAdminDelTeaBackL + 5;
	int BackCircleY = WindowAdminDelTeaBackU + 5;
	solidcircle(BackCircleX + 7, BackCircleY + 7, 7);
	setfillcolor(RED);
	solidrectangle(BackCircleX + 5, BackCircleY + 6, BackCircleX + 12, BackCircleY + 8);
	POINT BackTriangle[] = { {BackCircleX + 1, BackCircleY + 7}, {BackCircleX + 6, BackCircleY + 3}, {BackCircleX + 6, BackCircleY + 11} };
	solidpolygon(BackTriangle, 3);

	setfillcolor(DARKGRAY);
	solidrectangle(WindowAdminDelTeaViewAllL + 1, WindowAdminDelTeaViewAllU + 1, WindowAdminDelTeaViewAllR - 1, WindowAdminDelTeaViewAllD - 1);
	setfillcolor(LIGHTGRAY);
	solidrectangle(WindowAdminDelTeaPageUpL + 1, WindowAdminDelTeaPageUpU + 1, WindowAdminDelTeaPageUpR - 1, WindowAdminDelTeaPageUpD - 1);
	solidrectangle(WindowAdminDelTeaPageDownL + 1, WindowAdminDelTeaPageDownU + 1, WindowAdminDelTeaPageDownR - 1, WindowAdminDelTeaPageDownD - 1);
	solidrectangle(WindowAdminDelTeaPageSelectL + 1, WindowAdminDelTeaPageSelectU + 1, WindowAdminDelTeaPageSelectR - 1, WindowAdminDelTeaPageSelectD - 1);
	setfillcolor(LIGHTRED);
	solidrectangle(WindowAdminDelTeaDeleteL + 1, WindowAdminDelTeaDeleteU + 1, WindowAdminDelTeaDeleteR - 1, WindowAdminDelTeaDeleteD - 1);

	outtextxy(WindowAdminDelTeaPageUpL + 11, WindowAdminDelTeaPageUpU + 38, WindowAdminDelTeaPageUp);
	outtextxy(WindowAdminDelTeaPageDownL + 11, WindowAdminDelTeaPageDownU + 38, WindowAdminDelTeaPageDown);
	outtextxy(WindowAdminDelTeaDeleteL + 55, WindowAdminDelTeaDeleteU + 15, WindowAdminDelTeaDelete);

	//int PageNum_INT = 0;
	TCHAR WindowAdminDelTeaTNO[] = { TEXT("教师号") };
	TCHAR WindowAdminDelTeaTNAME[] = { TEXT("姓名") };
	TCHAR WindowAdminDelTeaTGENDER[] = { TEXT("性别") };
	TCHAR WindowAdminDelTeaTTITLE[] = { TEXT("职称") };
	TCHAR WindowAdminDelTeaTFIELD[] = { TEXT("方向") };
	TCHAR WindowAdminDelTeaTTELE[] = { TEXT("电话") };
	TCHAR WindowAdminDelTeaTEMAIL[] = { TEXT("邮箱") };
	TCHAR WindowAdminDelTeaTCHAIRMAN[] = { TEXT("系主任") };
	int WindowAdminDelTeaInfoColumnL = 75;
	int WindowAdminDelTeaInfoColumnU = 120;
	int WindowAdminDelTeaInfoColumnX = 0;
	int WindowAdminDelTeaInfoColumnY = 30;

	//学号
	int WindowAdminDelTeaSNOL = WindowAdminDelTeaInfoColumnL + WindowAdminDelTeaInfoColumnX;
	int WindowAdminDelTeaSNOU = WindowAdminDelTeaInfoColumnU;
	outtextxy(WindowAdminDelTeaInfoColumnL, WindowAdminDelTeaInfoColumnU, WindowAdminDelTeaTNO);
	WindowAdminDelTeaInfoColumnU += WindowAdminDelTeaInfoColumnY;
	//姓名
	int WindowAdminDelTeaSNAMEL = WindowAdminDelTeaInfoColumnL + WindowAdminDelTeaInfoColumnX;
	int WindowAdminDelTeaSNAMEU = WindowAdminDelTeaInfoColumnU;
	outtextxy(WindowAdminDelTeaInfoColumnL, WindowAdminDelTeaInfoColumnU, WindowAdminDelTeaTNAME);
	WindowAdminDelTeaInfoColumnU += WindowAdminDelTeaInfoColumnY;
	//性别
	int WindowAdminDelTeaSGENDERL = WindowAdminDelTeaInfoColumnL + WindowAdminDelTeaInfoColumnX;
	int WindowAdminDelTeaSGENDERU = WindowAdminDelTeaInfoColumnU;
	outtextxy(WindowAdminDelTeaInfoColumnL, WindowAdminDelTeaInfoColumnU, WindowAdminDelTeaTGENDER);
	WindowAdminDelTeaInfoColumnU += WindowAdminDelTeaInfoColumnY;
	//专业
	int WindowAdminDelTeaSMAJORL = WindowAdminDelTeaInfoColumnL + WindowAdminDelTeaInfoColumnX;
	int WindowAdminDelTeaSMAJORU = WindowAdminDelTeaInfoColumnU;
	outtextxy(WindowAdminDelTeaInfoColumnL, WindowAdminDelTeaInfoColumnU, WindowAdminDelTeaTTITLE);
	WindowAdminDelTeaInfoColumnU += WindowAdminDelTeaInfoColumnY;
	//班级
	int WindowAdminDelTeaSCLASSL = WindowAdminDelTeaInfoColumnL + WindowAdminDelTeaInfoColumnX;
	int WindowAdminDelTeaSCLASSU = WindowAdminDelTeaInfoColumnU;
	outtextxy(WindowAdminDelTeaInfoColumnL, WindowAdminDelTeaInfoColumnU, WindowAdminDelTeaTFIELD);
	WindowAdminDelTeaInfoColumnU += WindowAdminDelTeaInfoColumnY;
	//电话
	int WindowAdminDelTeaSTELEL = WindowAdminDelTeaInfoColumnL + WindowAdminDelTeaInfoColumnX;
	int WindowAdminDelTeaSTELEU = WindowAdminDelTeaInfoColumnU;
	outtextxy(WindowAdminDelTeaInfoColumnL, WindowAdminDelTeaInfoColumnU, WindowAdminDelTeaTTELE);
	WindowAdminDelTeaInfoColumnU += WindowAdminDelTeaInfoColumnY;
	//邮箱
	int WindowAdminDelTeaSEMAILL = WindowAdminDelTeaInfoColumnL + WindowAdminDelTeaInfoColumnX;
	int WindowAdminDelTeaSEMAILU = WindowAdminDelTeaInfoColumnU;
	outtextxy(WindowAdminDelTeaInfoColumnL, WindowAdminDelTeaInfoColumnU, WindowAdminDelTeaTEMAIL);
	WindowAdminDelTeaInfoColumnU += WindowAdminDelTeaInfoColumnY;
	//备注
	int WindowAdminDelTeaSOTHERSL = WindowAdminDelTeaInfoColumnL + WindowAdminDelTeaInfoColumnX;
	int WindowAdminDelTeaSOTHERSU = WindowAdminDelTeaInfoColumnU;
	outtextxy(WindowAdminDelTeaInfoColumnL, WindowAdminDelTeaInfoColumnU, WindowAdminDelTeaTCHAIRMAN);
	WindowAdminDelTeaInfoColumnU = 120;
	//查询等待删除的老师
	char sql_WindowAdminAllDelTea[MAXLEN] = { "SELECT * FROM TEACHER;" };
	LogPrint(DEBUG, (char*)sql_WindowAdminAllDelTea);
	exec = sqlite3_exec(DB, sql_WindowAdminAllDelTea, sql_CallBack_Func_InitItemNum, 0, &errmsg);
	if (exec != SQLITE_OK)//SQLITE_ABORT
	{
		//如果出错，就会回到这里来	
		char Msg_sqliteFailedToSelectDelTea[] = { "FAILED TO INIT Tea ItemNum" };
		LogPrint(ERROR, Msg_sqliteFailedToSelectDelTea);
		LogPrint(ERROR, errmsg);
	}
	else//SQLITE_OK
	{
		char Msg_sqliteSucceedToSelectDelTea[] = { "SUCCEED IN INITDelTea ItemNum" };
		LogPrint(INFO, Msg_sqliteSucceedToSelectDelTea);

		ItemTempCount = 0;
		ItemRestri = 1;
		ItemPage = (ItemNum / ItemRestri) + 1;

		ItemCurrentPage = 1;
		Start = (ItemCurrentPage - 1)*ItemRestri + 1;
		End = (ItemCurrentPage)*ItemRestri;

		exec = sqlite3_exec(DB, sql_WindowAdminAllDelTea, sql_CallBack_Func_Admin_DelTea, 0, &errmsg);
	}

	//后续可能需要自己整一套itoa的东西
	//不过反正是在win上不用考虑跨平台问题，反正定死是MSVC了，那就随便用itoa吧

	TCHAR PageNum_TCHAR[3 + 1];
	char PageNum_CHAR[3 + 1];

	//今日不造轮子

	//显示初始状态的页面


	MOUSEMSG mouseadminDelTea;
	while (true)
	{
		mouseadminDelTea = GetMouseMsg();
		switch (mouseadminDelTea.uMsg)
		{
		case WM_LBUTTONDOWN:
		{
			if (mouseadminDelTea.x >= WindowAdminDelTeaBackL && mouseadminDelTea.x <= WindowAdminDelTeaBackR && mouseadminDelTea.y >= WindowAdminDelTeaBackU && mouseadminDelTea.y <= WindowAdminDelTeaBackD)
			{//BackButton
				char Msg_AdminDelTeaGoAdmin[] = { "request a AdminDelTeaGoAdmin" };
				LogPrint(INFO, Msg_AdminDelTeaGoAdmin);
				WindowAdmin();
			}
			else if (mouseadminDelTea.x >= WindowAdminDelTeaPageUpL && mouseadminDelTea.x <= WindowAdminDelTeaPageUpR && mouseadminDelTea.y >= WindowAdminDelTeaPageUpU && mouseadminDelTea.y <= WindowAdminDelTeaPageUpD)
			{//Msg_WindowAdminDelTeaPageUp
				char Msg_WindowAdminDelTeaPageUp[] = { "Msg_WindowAdminDelTeaPageUp-上一页" };
				LogPrint(INFO, Msg_WindowAdminDelTeaPageUp);
				if (ItemCurrentPage - 1 >= 1)
				{
					setfillcolor(DARKGRAY);
					solidrectangle(WindowAdminDelTeaInfoColumnL + 50, WindowAdminDelTeaViewAllU + 1, WindowAdminDelTeaViewAllR - 1, WindowAdminDelTeaViewAllD - 1);
					ItemTempCount = 0;
					ItemCurrentPage -= 1;
					Start = (ItemCurrentPage - 1)*ItemRestri + 1;
					End = (ItemCurrentPage)*ItemRestri;
					exec = sqlite3_exec(DB, sql_WindowAdminAllDelTea, sql_CallBack_Func_Admin_DelTea, 0, &errmsg);
				}
			}
			else if (mouseadminDelTea.x >= WindowAdminDelTeaPageDownL && mouseadminDelTea.x <= WindowAdminDelTeaPageDownR && mouseadminDelTea.y >= WindowAdminDelTeaPageDownU && mouseadminDelTea.y <= WindowAdminDelTeaPageDownD)
			{//Msg_WindowAdminDelTeaPageDown
				char Msg_WindowAdminDelTeaPageDown[] = { "Msg_WindowAdminDelTeaPageDown-下一页" };
				LogPrint(INFO, Msg_WindowAdminDelTeaPageDown);
				if (ItemCurrentPage + 1 <= ItemPage)
				{
					setfillcolor(DARKGRAY);
					solidrectangle(WindowAdminDelTeaInfoColumnL + 50, WindowAdminDelTeaViewAllU + 1, WindowAdminDelTeaViewAllR - 1, WindowAdminDelTeaViewAllD - 1);
					ItemTempCount = 0;
					ItemCurrentPage += 1;
					Start = (ItemCurrentPage - 1)*ItemRestri + 1;
					End = (ItemCurrentPage)*ItemRestri;
					exec = sqlite3_exec(DB, sql_WindowAdminAllDelTea, sql_CallBack_Func_Admin_DelTea, 0, &errmsg);
				}
			}
			else if (mouseadminDelTea.x >= WindowAdminDelTeaPageSelectL && mouseadminDelTea.x <= WindowAdminDelTeaPageSelectR && mouseadminDelTea.y >= WindowAdminDelTeaPageSelectU && mouseadminDelTea.y <= WindowAdminDelTeaPageSelectD)
			{//Msg_WindowAdminDelTeaPageSelect
				char Msg_WindowAdminDelTeaPageSelect[] = { "Msg_WindowAdminDelTeaPageSelect-选择页面" };
				LogPrint(INFO, Msg_WindowAdminDelTeaPageSelect);
				InputBox(PageNum_TCHAR, 3 + 1, TEXT("请输入想要跳转到的页面(不超过3个字符)"));
				outtextxy(WindowAdminDelTeaPageSelectL + 6, WindowAdminDelTeaPageSelectU + 40, PageNum_TCHAR);
				TcharToChar(PageNum_TCHAR, PageNum_CHAR);
				LogPrint(DEBUG, PageNum_CHAR);
			}
			else if (mouseadminDelTea.x >= WindowAdminDelTeaDeleteL && mouseadminDelTea.x <= WindowAdminDelTeaDeleteR && mouseadminDelTea.y >= WindowAdminDelTeaDeleteU && mouseadminDelTea.y <= WindowAdminDelTeaDeleteD)
			{//Msg_WindowAdminDelTeaDelete
				char Msg_WindowAdminDelTeaDelete[] = { "Msg_WindowAdminDelTeaDelete-下课某老师" };
				LogPrint(INFO, Msg_WindowAdminDelTeaDelete);
				char sql_WindowAdminDelTeaP1[MAXLEN] = { "DELETE FROM TEACHER WHERE(TNO=='" };
				char sql_WindowAdminDelTeaP2[MAXLEN] = { "');" };
				char sql_WindowAdminDelTeaP3[MAXLEN] = { "DELETE FROM USER WHERE(UID=='" };
				char sql_WindowAdminDelTeaP4[MAXLEN] = { "');" };
				strcat(sql_WindowAdminDelTeaP1, CurrentItem);
				strcat(sql_WindowAdminDelTeaP1, sql_WindowAdminDelTeaP2);
				strcat(sql_WindowAdminDelTeaP3, CurrentItem);
				strcat(sql_WindowAdminDelTeaP3, sql_WindowAdminDelTeaP4);
				const char* sql_WindowAdminDelTea = sql_WindowAdminDelTeaP1;
				const char* sql_WindowAdminDelTeaUser = sql_WindowAdminDelTeaP3;
				LogPrint(DEBUG, (char*)sql_WindowAdminDelTea);
				LogPrint(DEBUG, (char*)sql_WindowAdminDelTeaUser);
				exec = sqlite3_exec(DB, sql_WindowAdminDelTea, sql_CallBack_Func_InitItemNum, 0, &errmsg);
				if (exec != SQLITE_OK)//SQLITE_ABORT
				{
					//如果出错，就会回到这里来	
					char Msg_sqliteFailedToDelTea[] = { "FAILED TO DelTea" };
					LogPrint(ERROR, Msg_sqliteFailedToDelTea);
					LogPrint(ERROR, errmsg);
				}
				else//SQLITE_OK
				{
					char Msg_sqliteSucceedInDelTea[] = { "SUCCEED IN DelTea" };
					LogPrint(INFO, Msg_sqliteSucceedInDelTea);
					exec = sqlite3_exec(DB, sql_WindowAdminDelTeaUser, sql_CallBack_Func_InitItemNum, 0, &errmsg);
					if (exec != SQLITE_OK)//SQLITE_ABORT
					{
						//如果出错，就会回到这里来	
						char Msg_sqliteFailedToDelTea2[] = { "FAILED TO DelTea" };
						LogPrint(ERROR, Msg_sqliteFailedToDelTea2);
						LogPrint(ERROR, errmsg);
					}
					else//SQLITE_OK
					{
						char Msg_sqliteSucceedInDelTea2[] = { "SUCCEED IN DelTea" };
						LogPrint(INFO, Msg_sqliteSucceedInDelTea2);
					}
				}
				WindowAdmin();
			}
		}
		default:
			break;
		}
	}


	return 0;
}

int WindowStuMain(void)
{
	int WindowStuSelectProject(void);
	int WindowStuUploadProject(void);
	cleardevice();
	FlushMouseMsgBuffer();
	setcliprgn(NULL);
	setpolyfillmode(WINDING);
	setfillcolor(GREEN);
	solidrectangle(0, 0, W, H);
	Sleep(350);

	setbkmode(TRANSPARENT);
	char Msg_initWindowStuMain[] = { "initWindowStuMain" };
	LogPrint(INFO, Msg_initWindowStuMain);

	//读取当前ID
	LogPrint(DEBUG, CurrentID);
	TCHAR CurrentID_TCHAR[13 + 1];
	CharToTchar(CurrentID, CurrentID_TCHAR);
	TCHAR WindowStuMainSlogan[] = { TEXT("同学你好，祝毕业顺利") };
	outtextxy(280, 20, CurrentID_TCHAR);
	outtextxy(385, 20, WindowStuMainSlogan);

	TCHAR WindowStuSelectProjectTEXT[] = { TEXT("选题") };
	TCHAR WindowStuUploadProjectTEXT[] = { TEXT("上传进度") };
	TCHAR WindowStuSelectedProjectWARN[] = { TEXT("您已经选过题目了") };
	TCHAR WindowStuUnSelectedProjectWARN[] = { TEXT("您还没有选定题目") };

	//选题框
	int WindowStuSelectProjectL = 135;
	int WindowStuSelectProjectU = 155;
	int WindowStuSelectProjectR = WindowStuSelectProjectL + 120;
	int WindowStuSelectProjectD = WindowStuSelectProjectU + 45;
	rectangle(WindowStuSelectProjectL, WindowStuSelectProjectU, WindowStuSelectProjectR, WindowStuSelectProjectD);


	//上传进度框
	int WindowStuUploadProjectL = 135;
	int WindowStuUploadProjectU = 250;
	int WindowStuUploadProjectR = WindowStuUploadProjectL + 120;
	int WindowStuUploadProjectD = WindowStuUploadProjectU + 45;
	rectangle(WindowStuUploadProjectL, WindowStuUploadProjectU, WindowStuUploadProjectR, WindowStuUploadProjectD);


	//插图框
	rectangle(400, 150 - 50, W - 100, H - 50);
	IMAGE StuMainimg;
	loadimage(&StuMainimg, TEXT("Pic_StuMain.bmp"));
	int StuMainimgW, StuMainimgH;
	StuMainimgW = StuMainimg.getwidth();
	StuMainimgH = StuMainimg.getheight();
	putimage(400 + 1, 150 - 50 + 1, &StuMainimg);

	setfillcolor(LIGHTGRAY);
	solidrectangle(WindowStuSelectProjectL + 1, WindowStuSelectProjectU + 1, WindowStuSelectProjectR - 1, WindowStuSelectProjectD - 1);
	solidrectangle(WindowStuUploadProjectL + 1, WindowStuUploadProjectU + 1, WindowStuUploadProjectR - 1, WindowStuUploadProjectD - 1);

	outtextxy(WindowStuSelectProjectL + 45, WindowStuSelectProjectU + 16, WindowStuSelectProjectTEXT);
	outtextxy(WindowStuUploadProjectL + 30, WindowStuUploadProjectU + 15, WindowStuUploadProjectTEXT);

	//返回按钮
	int WindowStuMainBackL = 35;
	int WindowStuMainBackU = 20;
	int WindowStuMainBackR = WindowStuMainBackL + 75;
	int WindowStuMainBackD = WindowStuMainBackU + 25;
	rectangle(WindowStuMainBackL, WindowStuMainBackU, WindowStuMainBackR, WindowStuMainBackD);
	TCHAR WindowStuMainBackLoginButton[] = { TEXT("返回") };
	//返回图标
	outtextxy(WindowStuMainBackL + 35, WindowStuMainBackU + 4, WindowStuMainBackLoginButton);
	setfillcolor(YELLOW);
	int BackCircleX = WindowStuMainBackL + 5;
	int BackCircleY = WindowStuMainBackU + 5;
	solidcircle(BackCircleX + 7, BackCircleY + 7, 7);
	setfillcolor(WHITE);
	solidrectangle(BackCircleX + 5, BackCircleY + 6, BackCircleX + 12, BackCircleY + 8);
	POINT BackTriangle[] = { {BackCircleX + 1, BackCircleY + 7}, {BackCircleX + 6, BackCircleY + 3}, {BackCircleX + 6, BackCircleY + 11} };
	solidpolygon(BackTriangle, 3);

	MOUSEMSG mouseStuMain;
	while (true)
	{
		mouseStuMain = GetMouseMsg();
		switch (mouseStuMain.uMsg)
		{
		case WM_LBUTTONDOWN:
		{
			if (mouseStuMain.x >= WindowStuMainBackL && mouseStuMain.x <= WindowStuMainBackR && mouseStuMain.y >= WindowStuMainBackU && mouseStuMain.y <= WindowStuMainBackD)
			{//BackButton
				char Msg_StuMainGoLogin[] = { "request a Msg_StuMainGoLogin" };
				LogPrint(INFO, Msg_StuMainGoLogin);
				TCHAR StuQuit_TCHAR[3 + 1];
				char StuQuit_CHAR[3 + 1];
				InputBox(StuQuit_TCHAR, 3 + 1, TEXT("确定要退出吗？（Y/N）"));
				TcharToChar(StuQuit_TCHAR, StuQuit_CHAR);
				if (StuQuit_CHAR[0] == 'Y' || StuQuit_CHAR[0] == 'y')
				{
					WindowLogin();
				}
			}
			else if (mouseStuMain.x >= WindowStuSelectProjectL && mouseStuMain.x <= WindowStuSelectProjectR && mouseStuMain.y >= WindowStuSelectProjectU && mouseStuMain.y <= WindowStuSelectProjectD)
			{//Msg_WindowStuMainSelectProject
				char Msg_WindowStuMainSelectProject[] = { "Msg_WindowStuMainSelectProject-选题" };
				LogPrint(INFO, Msg_WindowStuMainSelectProject);
				char sql_WindowNewStuInsertGraduationAccessP1[MAXLEN] = { "SELECT SNO FROM GRADUATION WHERE(SNO=='" };
				char sql_WindowNewStuInsertGraduationAccessP2[MAXLEN] = { "');" };
				strcat(sql_WindowNewStuInsertGraduationAccessP1, CurrentID);//PNO
				strcat(sql_WindowNewStuInsertGraduationAccessP1, sql_WindowNewStuInsertGraduationAccessP2);
				const char *sql_WindowNewStuInsertGraduationAccess = sql_WindowNewStuInsertGraduationAccessP1;
				LogPrint(DEBUG, (char*)sql_WindowNewStuInsertGraduationAccess);
				ItemNum = 0;
				exec = sqlite3_exec(DB, sql_WindowNewStuInsertGraduationAccess, sql_CallBack_Func_InitItemNum, 0, &errmsg);
				if (ItemNum >= 1)
				{
					outtextxy(WindowStuUploadProjectL, WindowStuUploadProjectU + 65, WindowStuSelectedProjectWARN);
				}
				else
				{
					WindowStuSelectProject();
				}
			}
			else if (mouseStuMain.x >= WindowStuUploadProjectL && mouseStuMain.x <= WindowStuUploadProjectR && mouseStuMain.y >= WindowStuUploadProjectU && mouseStuMain.y <= WindowStuUploadProjectD)
			{//Msg_WindowStuMainUploadProject
				char Msg_WindowStuMainUploadProject[] = { "Msg_WindowStuMainUploadProject-上传进度" };
				LogPrint(INFO, Msg_WindowStuMainUploadProject);
				char sql_WindowNewStuInsertGraduationAccessP1[MAXLEN] = { "SELECT SNO FROM GRADUATION WHERE(SNO=='" };
				char sql_WindowNewStuInsertGraduationAccessP2[MAXLEN] = { "');" };
				strcat(sql_WindowNewStuInsertGraduationAccessP1, CurrentID);//PNO
				strcat(sql_WindowNewStuInsertGraduationAccessP1, sql_WindowNewStuInsertGraduationAccessP2);
				const char *sql_WindowNewStuInsertGraduationAccess = sql_WindowNewStuInsertGraduationAccessP1;
				LogPrint(DEBUG, (char*)sql_WindowNewStuInsertGraduationAccess);
				ItemNum = 0;
				exec = sqlite3_exec(DB, sql_WindowNewStuInsertGraduationAccess, sql_CallBack_Func_InitItemNum, 0, &errmsg);
				if (ItemNum >= 1)
				{
					WindowStuUploadProject();
				}
				else
				{
					outtextxy(WindowStuUploadProjectL, WindowStuUploadProjectU + 65, WindowStuUnSelectedProjectWARN);
				}
			}
		}
		default:
			break;
		}
	}

	return 0;
}

int WindowTeaMain(void)
{
	int WindowTeaCreateProject(void);
	int WindowTeaReviewProject(void);
	int WindowChairmanMain(void);
	////
	cleardevice();
	FlushMouseMsgBuffer();
	setcliprgn(NULL);
	setpolyfillmode(WINDING);
	setfillcolor(BLUE);
	solidrectangle(0, 0, W, H);
	Sleep(350);

	setbkmode(TRANSPARENT);
	char Msg_initWindowTeaMain[] = { "initWindowTeaMain" };
	LogPrint(INFO, Msg_initWindowTeaMain);

	LogPrint(DEBUG, CurrentID);
	TCHAR CurrentID_TCHAR[13 + 1];
	CharToTchar(CurrentID, CurrentID_TCHAR);
	TCHAR WindowTeaMainSlogan[] = { TEXT("老师，您辛苦了") };
	outtextxy(280, 20, CurrentID_TCHAR);
	outtextxy(385, 20, WindowTeaMainSlogan);

	TCHAR WindowTeaCreateProjectTEXT[] = { TEXT("开题") };
	TCHAR WindowTeaReviewProjectTEXT[] = { TEXT("阶段审批") };
	TCHAR WindowTeaLogintoChairmanTEXT[] = { TEXT("我其实是系主任") };//只有有系主任权限才可以点的进去哦否则要报错的喂
	TCHAR WindowNotChairmanProjectWARN[] = { TEXT("您不是系主任，无权审批") };

	//开题框
	int WindowTeaCreateProjectL = 120;
	int WindowTeaCreateProjectU = 140;
	int WindowTeaCreateProjectR = WindowTeaCreateProjectL + 200;
	int WindowTeaCreateProjectD = WindowTeaCreateProjectU + 40;
	rectangle(WindowTeaCreateProjectL, WindowTeaCreateProjectU, WindowTeaCreateProjectR, WindowTeaCreateProjectD);

	//阶段审批框
	int WindowTeaReviewProjectL = 120;
	int WindowTeaReviewProjectU = 220;
	int WindowTeaReviewProjectR = WindowTeaReviewProjectL + 200;
	int WindowTeaReviewProjectD = WindowTeaReviewProjectU + 40;
	rectangle(WindowTeaReviewProjectL, WindowTeaReviewProjectU, WindowTeaReviewProjectR, WindowTeaReviewProjectD);

	//变身系主任框
	int WindowTeaLogintoChairmanL = 120;
	int WindowTeaLogintoChairmanU = 300;
	int WindowTeaLogintoChairmanR = WindowTeaLogintoChairmanL + 200;
	int WindowTeaLogintoChairmanD = WindowTeaLogintoChairmanU + 40;
	rectangle(WindowTeaLogintoChairmanL, WindowTeaLogintoChairmanU, WindowTeaLogintoChairmanR, WindowTeaLogintoChairmanD);

	setfillcolor(LIGHTGRAY);
	solidrectangle(WindowTeaCreateProjectL + 1, WindowTeaCreateProjectU + 1, WindowTeaCreateProjectR - 1, WindowTeaCreateProjectD - 1);
	solidrectangle(WindowTeaReviewProjectL + 1, WindowTeaReviewProjectU + 1, WindowTeaReviewProjectR - 1, WindowTeaReviewProjectD - 1);
	solidrectangle(WindowTeaLogintoChairmanL + 1, WindowTeaLogintoChairmanU + 1, WindowTeaLogintoChairmanR - 1, WindowTeaLogintoChairmanD - 1);

	outtextxy(WindowTeaCreateProjectL + 80, WindowTeaCreateProjectU + 12, WindowTeaCreateProjectTEXT);
	outtextxy(WindowTeaReviewProjectL + 65, WindowTeaReviewProjectU + 12, WindowTeaReviewProjectTEXT);
	outtextxy(WindowTeaLogintoChairmanL + 45, WindowTeaLogintoChairmanU + 12, WindowTeaLogintoChairmanTEXT);

	//插图框
	rectangle(410, 90, W - 100, H - 50);
	IMAGE TeaMainimg;
	loadimage(&TeaMainimg, TEXT("Pic_TeaMain.bmp"));
	int TeaMainimgW, TeaMainimgH;
	TeaMainimgW = TeaMainimg.getwidth();
	TeaMainimgH = TeaMainimg.getheight();
	putimage(410 + 1, 90 + 1, &TeaMainimg);

	//返回按钮
	int WindowTeaMainBackL = 35;
	int WindowTeaMainBackU = 20;
	int WindowTeaMainBackR = WindowTeaMainBackL + 75;
	int WindowTeaMainBackD = WindowTeaMainBackU + 25;
	rectangle(WindowTeaMainBackL, WindowTeaMainBackU, WindowTeaMainBackR, WindowTeaMainBackD);
	TCHAR WindowTeaMainBackLoginButton[] = { TEXT("返回") };
	//返回图标
	outtextxy(WindowTeaMainBackL + 35, WindowTeaMainBackU + 4, WindowTeaMainBackLoginButton);
	setfillcolor(YELLOW);
	int BackCircleX = WindowTeaMainBackL + 5;
	int BackCircleY = WindowTeaMainBackU + 5;
	solidcircle(BackCircleX + 7, BackCircleY + 7, 7);
	setfillcolor(WHITE);
	solidrectangle(BackCircleX + 5, BackCircleY + 6, BackCircleX + 12, BackCircleY + 8);
	POINT BackTriangle[] = { {BackCircleX + 1, BackCircleY + 7}, {BackCircleX + 6, BackCircleY + 3}, {BackCircleX + 6, BackCircleY + 11} };
	solidpolygon(BackTriangle, 3);

	MOUSEMSG mouseTeaMain;
	while (true)
	{
		mouseTeaMain = GetMouseMsg();
		switch (mouseTeaMain.uMsg)
		{
		case WM_LBUTTONDOWN:
		{
			if (mouseTeaMain.x >= WindowTeaMainBackL && mouseTeaMain.x <= WindowTeaMainBackR && mouseTeaMain.y >= WindowTeaMainBackU && mouseTeaMain.y <= WindowTeaMainBackD)
			{//BackButton
				char Msg_TeaMainGoLogin[] = { "request a Msg_TeaMainGoLogin" };
				LogPrint(INFO, Msg_TeaMainGoLogin);
				TCHAR TeaQuit_TCHAR[3 + 1];
				char TeaQuit_CHAR[3 + 1];
				InputBox(TeaQuit_TCHAR, 3 + 1, TEXT("确定要退出吗？（Y/N）"));
				TcharToChar(TeaQuit_TCHAR, TeaQuit_CHAR);
				if (TeaQuit_CHAR[0] == 'Y' || TeaQuit_CHAR[0] == 'y')
				{
					WindowLogin();
				}
			}
			else if (mouseTeaMain.x >= WindowTeaCreateProjectL && mouseTeaMain.x <= WindowTeaCreateProjectR && mouseTeaMain.y >= WindowTeaCreateProjectU && mouseTeaMain.y <= WindowTeaCreateProjectD)
			{//Msg_WindowTeaCreateProject
				char Msg_WindowTeaCreateProject[] = { "Msg_WindowTeaCreateProject-开题" };
				LogPrint(INFO, Msg_WindowTeaCreateProject);
				WindowTeaCreateProject();
			}
			else if (mouseTeaMain.x >= WindowTeaReviewProjectL && mouseTeaMain.x <= WindowTeaReviewProjectR && mouseTeaMain.y >= WindowTeaReviewProjectU && mouseTeaMain.y <= WindowTeaReviewProjectD)
			{//Msg_WindowTeaReviewProject
				char Msg_WindowTeaReviewProject[] = { "Msg_WindowTeaReviewProject-阶段审批" };
				LogPrint(INFO, Msg_WindowTeaReviewProject);
				WindowTeaReviewProject();
			}
			else if (mouseTeaMain.x >= WindowTeaLogintoChairmanL && mouseTeaMain.x <= WindowTeaLogintoChairmanR && mouseTeaMain.y >= WindowTeaLogintoChairmanU && mouseTeaMain.y <= WindowTeaLogintoChairmanD)
			{//Msg_WindowTeaLogintoChairman
				char Msg_WindowTeaLogintoChairman[] = { "Msg_WindowTeaLogintoChairman-进入系主任页面" };
				LogPrint(INFO, Msg_WindowTeaLogintoChairman);
				char sql_WindowWhetherTeaIsChairmanP1[MAXLEN] = { "SELECT TCHAIRMAN FROM TEACHER WHERE(TNO=='" };
				char sql_WindowWhetherTeaIsChairmanP2[MAXLEN] = { "');" };
				strcat(sql_WindowWhetherTeaIsChairmanP1, CurrentID);//TNO
				strcat(sql_WindowWhetherTeaIsChairmanP1, sql_WindowWhetherTeaIsChairmanP2);
				const char *sql_WindowWhetherTeaIsChairman = sql_WindowWhetherTeaIsChairmanP1;
				LogPrint(DEBUG, (char*)sql_WindowWhetherTeaIsChairman);
				exec = sqlite3_exec(DB, sql_WindowWhetherTeaIsChairman, sql_CallBack_Func_IsChairman, 0, &errmsg);
				if (flag_IsChairman == 1)
				{
					//是系主任
					WindowChairmanMain();
				}
				else
				{
					//不是系主任
					outtextxy(WindowTeaLogintoChairmanL, WindowTeaLogintoChairmanU + 65, WindowNotChairmanProjectWARN);
				}
			}
		}
		default:
			break;
		}
	}


	return 0;
}

int WindowChairmanMain(void)
{
	int WindowChairmanReviewSelectProject(void);
	int WindowChairmanReviewUploadProject(void);
	cleardevice();
	FlushMouseMsgBuffer();
	setcliprgn(NULL);
	setpolyfillmode(WINDING);
	setfillcolor(BROWN);
	solidrectangle(0, 0, W, H);
	Sleep(350);

	setbkmode(TRANSPARENT);
	char Msg_initWindowAdminDelStu[] = { "initWindowAdminDelStu" };
	LogPrint(INFO, Msg_initWindowAdminDelStu);

	//读取当前ID
	LogPrint(DEBUG, CurrentID);
	TCHAR CurrentID_TCHAR[13 + 1];
	CharToTchar(CurrentID, CurrentID_TCHAR);
	TCHAR WindowChairmanMainSlogan[] = { TEXT("主任，有什么本系统帮得上的吗？") };
	outtextxy(230, 20, CurrentID_TCHAR);
	outtextxy(335, 20, WindowChairmanMainSlogan);

	TCHAR WindowChairmanReviewSelectProjectTEXT[] = { TEXT("审批学生开题") };
	TCHAR WindowChairmanReviewUploadProjectTEXT[] = { TEXT("审批毕设进度") };

	//选题框
	int WindowStuSelectProjectL = 135;
	int WindowStuSelectProjectU = 155;
	int WindowStuSelectProjectR = WindowStuSelectProjectL + 150;
	int WindowStuSelectProjectD = WindowStuSelectProjectU + 45;
	rectangle(WindowStuSelectProjectL, WindowStuSelectProjectU, WindowStuSelectProjectR, WindowStuSelectProjectD);


	//上传进度框
	int WindowStuUploadProjectL = 135;
	int WindowStuUploadProjectU = 250;
	int WindowStuUploadProjectR = WindowStuUploadProjectL + 150;
	int WindowStuUploadProjectD = WindowStuUploadProjectU + 45;
	rectangle(WindowStuUploadProjectL, WindowStuUploadProjectU, WindowStuUploadProjectR, WindowStuUploadProjectD);


	//插图框
	rectangle(410, 90, W - 100, H - 50);
	IMAGE TeaMainimg;
	loadimage(&TeaMainimg, TEXT("Pic_TeaMain.bmp"));
	int TeaMainimgW, TeaMainimgH;
	TeaMainimgW = TeaMainimg.getwidth();
	TeaMainimgH = TeaMainimg.getheight();
	putimage(410 + 1, 90 + 1, &TeaMainimg);

	setfillcolor(LIGHTGRAY);
	solidrectangle(WindowStuSelectProjectL + 1, WindowStuSelectProjectU + 1, WindowStuSelectProjectR - 1, WindowStuSelectProjectD - 1);
	solidrectangle(WindowStuUploadProjectL + 1, WindowStuUploadProjectU + 1, WindowStuUploadProjectR - 1, WindowStuUploadProjectD - 1);

	outtextxy(WindowStuSelectProjectL + 20, WindowStuSelectProjectU + 16, WindowChairmanReviewSelectProjectTEXT);
	outtextxy(WindowStuUploadProjectL + 20, WindowStuUploadProjectU + 15, WindowChairmanReviewUploadProjectTEXT);

	//返回按钮
	int WindowStuMainBackL = 35;
	int WindowStuMainBackU = 20;
	int WindowStuMainBackR = WindowStuMainBackL + 75;
	int WindowStuMainBackD = WindowStuMainBackU + 25;
	rectangle(WindowStuMainBackL, WindowStuMainBackU, WindowStuMainBackR, WindowStuMainBackD);
	TCHAR WindowStuMainBackLoginButton[] = { TEXT("返回") };
	//返回图标
	outtextxy(WindowStuMainBackL + 35, WindowStuMainBackU + 4, WindowStuMainBackLoginButton);
	setfillcolor(YELLOW);
	int BackCircleX = WindowStuMainBackL + 5;
	int BackCircleY = WindowStuMainBackU + 5;
	solidcircle(BackCircleX + 7, BackCircleY + 7, 7);
	setfillcolor(WHITE);
	solidrectangle(BackCircleX + 5, BackCircleY + 6, BackCircleX + 12, BackCircleY + 8);
	POINT BackTriangle[] = { {BackCircleX + 1, BackCircleY + 7}, {BackCircleX + 6, BackCircleY + 3}, {BackCircleX + 6, BackCircleY + 11} };
	solidpolygon(BackTriangle, 3);

	MOUSEMSG mouseStuMain;
	while (true)
	{
		mouseStuMain = GetMouseMsg();
		switch (mouseStuMain.uMsg)
		{
		case WM_LBUTTONDOWN:
		{
			if (mouseStuMain.x >= WindowStuMainBackL && mouseStuMain.x <= WindowStuMainBackR && mouseStuMain.y >= WindowStuMainBackU && mouseStuMain.y <= WindowStuMainBackD)
			{//BackButton
				char Msg_StuMainGoLogin[] = { "request a Msg_StuMainGoLogin" };
				LogPrint(INFO, Msg_StuMainGoLogin);
				WindowTeaMain();
			}
			else if (mouseStuMain.x >= WindowStuSelectProjectL && mouseStuMain.x <= WindowStuSelectProjectR && mouseStuMain.y >= WindowStuSelectProjectU && mouseStuMain.y <= WindowStuSelectProjectD)
			{//Msg_WindowStuMainSelectProject
				char Msg_WindowStuMainSelectProject[] = { "Msg_WindowStuMainSelectProject-审批学生开题" };
				LogPrint(INFO, Msg_WindowStuMainSelectProject);
				WindowChairmanReviewSelectProject();
			}
			else if (mouseStuMain.x >= WindowStuUploadProjectL && mouseStuMain.x <= WindowStuUploadProjectR && mouseStuMain.y >= WindowStuUploadProjectU && mouseStuMain.y <= WindowStuUploadProjectD)
			{//Msg_WindowStuMainUploadProject
				char Msg_WindowStuMainUploadProject[] = { "Msg_WindowStuMainUploadProject-审批毕设进度" };
				LogPrint(INFO, Msg_WindowStuMainUploadProject);
				WindowChairmanReviewUploadProject();
			}
		}
		default:
			break;
		}
	}
	return 0;
}

int WindowTeaCreateProject(void)
{
	cleardevice();
	FlushMouseMsgBuffer();
	setcliprgn(NULL);
	setpolyfillmode(WINDING);
	setfillcolor(BLUE);
	solidrectangle(0, 0, W, H);
	Sleep(350);

	setbkmode(TRANSPARENT);
	char Msg_initWindowTeaCreateProject[] = { "initWindowTeaCreateProject" };
	LogPrint(INFO, Msg_initWindowTeaCreateProject);

	TCHAR WindowTeaCreateProjectSlogan[] = { TEXT("老师您好，您有什么毕设题目的点子吗？") };
	outtextxy(260, 20, WindowTeaCreateProjectSlogan);

	TCHAR WindowTeaCreateProjectPNO[] = { TEXT("题目编号") };
	TCHAR WindowTeaCreateProjectPNAME[] = { TEXT("题目名称") };
	TCHAR WindowTeaCreateProjectPMAJOR[] = { TEXT("题目专业") };
	TCHAR WindowTeaCreateProjectTNO[] = { TEXT("命题导师") };
	TCHAR WindowTeaCreateProjectPABSTRUCT[] = { TEXT("内容简介") };

	int WindowTeaCreateProjectInfoColumnL = 225;
	int WindowTeaCreateProjectInfoColumnU = 150;
	int WindowTeaCreateProjectInfoColumnX = 100;
	int WindowTeaCreateProjectInfoColumnY = 35;

	TCHAR WindowTeaCreateProjectNote[] = { TEXT("请您完善您开的题目的信息，其中题目编号与命题导师是必填项") };
	outtextxy(WindowTeaCreateProjectInfoColumnL, WindowTeaCreateProjectInfoColumnU - 50, WindowTeaCreateProjectNote);

	//题目编号
	int WindowTeaCreateProjectPNOL = WindowTeaCreateProjectInfoColumnL + WindowTeaCreateProjectInfoColumnX;
	int WindowTeaCreateProjectPNOU = WindowTeaCreateProjectInfoColumnU;
	int WindowTeaCreateProjectPNOR = WindowTeaCreateProjectPNOL + 250;
	int WindowTeaCreateProjectPNOD = WindowTeaCreateProjectPNOU + 20;
	rectangle(WindowTeaCreateProjectPNOL, WindowTeaCreateProjectPNOU, WindowTeaCreateProjectPNOR, WindowTeaCreateProjectPNOD);
	outtextxy(WindowTeaCreateProjectInfoColumnL, WindowTeaCreateProjectInfoColumnU, WindowTeaCreateProjectPNO);
	WindowTeaCreateProjectInfoColumnU += WindowTeaCreateProjectInfoColumnY;
	//题目名称
	int WindowTeaCreateProjectPNAMEL = WindowTeaCreateProjectInfoColumnL + WindowTeaCreateProjectInfoColumnX;
	int WindowTeaCreateProjectPNAMEU = WindowTeaCreateProjectInfoColumnU;
	int WindowTeaCreateProjectPNAMER = WindowTeaCreateProjectPNAMEL + 250;
	int WindowTeaCreateProjectPNAMED = WindowTeaCreateProjectPNAMEU + 20;
	rectangle(WindowTeaCreateProjectPNAMEL, WindowTeaCreateProjectPNAMEU, WindowTeaCreateProjectPNAMER, WindowTeaCreateProjectPNAMED);
	outtextxy(WindowTeaCreateProjectInfoColumnL, WindowTeaCreateProjectInfoColumnU, WindowTeaCreateProjectPNAME);
	WindowTeaCreateProjectInfoColumnU += WindowTeaCreateProjectInfoColumnY;
	//题目专业
	int WindowTeaCreateProjectPMAJORL = WindowTeaCreateProjectInfoColumnL + WindowTeaCreateProjectInfoColumnX;
	int WindowTeaCreateProjectPMAJORU = WindowTeaCreateProjectInfoColumnU;
	int WindowTeaCreateProjectPMAJORR = WindowTeaCreateProjectPMAJORL + 250;
	int WindowTeaCreateProjectPMAJORD = WindowTeaCreateProjectPMAJORU + 20;
	rectangle(WindowTeaCreateProjectPMAJORL, WindowTeaCreateProjectPMAJORU, WindowTeaCreateProjectPMAJORR, WindowTeaCreateProjectPMAJORD);
	outtextxy(WindowTeaCreateProjectInfoColumnL, WindowTeaCreateProjectInfoColumnU, WindowTeaCreateProjectPMAJOR);
	WindowTeaCreateProjectInfoColumnU += WindowTeaCreateProjectInfoColumnY;
	//命题导师
	int WindowTeaCreateProjectTNOL = WindowTeaCreateProjectInfoColumnL + WindowTeaCreateProjectInfoColumnX;
	int WindowTeaCreateProjectTNOU = WindowTeaCreateProjectInfoColumnU;
	int WindowTeaCreateProjectTNOR = WindowTeaCreateProjectTNOL + 250;
	int WindowTeaCreateProjectTNOD = WindowTeaCreateProjectTNOU + 20;
	rectangle(WindowTeaCreateProjectTNOL, WindowTeaCreateProjectTNOU, WindowTeaCreateProjectTNOR, WindowTeaCreateProjectTNOD);
	outtextxy(WindowTeaCreateProjectInfoColumnL, WindowTeaCreateProjectInfoColumnU, WindowTeaCreateProjectTNO);
	WindowTeaCreateProjectInfoColumnU += WindowTeaCreateProjectInfoColumnY;
	//内容简介
	int WindowTeaCreateProjectPABSTRUCTL = WindowTeaCreateProjectInfoColumnL + WindowTeaCreateProjectInfoColumnX;
	int WindowTeaCreateProjectPABSTRUCTU = WindowTeaCreateProjectInfoColumnU;
	int WindowTeaCreateProjectPABSTRUCTR = WindowTeaCreateProjectPABSTRUCTL + 250;
	int WindowTeaCreateProjectPABSTRUCTD = WindowTeaCreateProjectPABSTRUCTU + 20;
	rectangle(WindowTeaCreateProjectPABSTRUCTL, WindowTeaCreateProjectPABSTRUCTU, WindowTeaCreateProjectPABSTRUCTR, WindowTeaCreateProjectPABSTRUCTD);
	outtextxy(WindowTeaCreateProjectInfoColumnL, WindowTeaCreateProjectInfoColumnU, WindowTeaCreateProjectPABSTRUCT);
	WindowTeaCreateProjectInfoColumnU += WindowTeaCreateProjectInfoColumnY;


	TCHAR WindowTeaCreateProjectSubmit[] = { TEXT("确认提交") };

	//确认提交
	int WindowTeaCreateProjectSubmitL = WindowTeaCreateProjectInfoColumnL + WindowTeaCreateProjectInfoColumnX;
	int WindowTeaCreateProjectSubmitU = WindowTeaCreateProjectInfoColumnU + 10;
	int WindowTeaCreateProjectSubmitR = WindowTeaCreateProjectSubmitL + 125;
	int WindowTeaCreateProjectSubmitD = WindowTeaCreateProjectSubmitU + 35;
	rectangle(WindowTeaCreateProjectSubmitL, WindowTeaCreateProjectSubmitU, WindowTeaCreateProjectSubmitR, WindowTeaCreateProjectSubmitD);
	setfillcolor(DARKGRAY);
	solidrectangle(WindowTeaCreateProjectSubmitL + 1, WindowTeaCreateProjectSubmitU + 1, WindowTeaCreateProjectSubmitR - 1, WindowTeaCreateProjectSubmitD - 1);
	outtextxy(WindowTeaCreateProjectSubmitL + 30, WindowTeaCreateProjectSubmitU + 10, WindowTeaCreateProjectSubmit);

	//返回按钮
	int WindowTeaCreateProjectBackL = 35;
	int WindowTeaCreateProjectBackU = 20;
	int WindowTeaCreateProjectBackR = WindowTeaCreateProjectBackL + 75;
	int WindowTeaCreateProjectBackD = WindowTeaCreateProjectBackU + 25;
	rectangle(WindowTeaCreateProjectBackL, WindowTeaCreateProjectBackU, WindowTeaCreateProjectBackR, WindowTeaCreateProjectBackD);
	TCHAR WindowAdminBackCreateProjectButton[] = { TEXT("返回") };
	//返回图标
	outtextxy(WindowTeaCreateProjectBackL + 35, WindowTeaCreateProjectBackU + 4, WindowAdminBackCreateProjectButton);
	setfillcolor(YELLOW);
	int BackCircleX = WindowTeaCreateProjectBackL + 5;
	int BackCircleY = WindowTeaCreateProjectBackU + 5;
	solidcircle(BackCircleX + 7, BackCircleY + 7, 7);
	setfillcolor(RED);
	solidrectangle(BackCircleX + 5, BackCircleY + 6, BackCircleX + 12, BackCircleY + 8);
	POINT BackTriangle[] = { {BackCircleX + 1, BackCircleY + 7}, {BackCircleX + 6, BackCircleY + 3}, {BackCircleX + 6, BackCircleY + 11} };
	solidpolygon(BackTriangle, 3);

	setfillcolor(BLACK);
	solidrectangle(WindowTeaCreateProjectPNOL + 1, WindowTeaCreateProjectPNOU + 1, WindowTeaCreateProjectPNOR - 1, WindowTeaCreateProjectPNOD - 1);
	solidrectangle(WindowTeaCreateProjectPNAMEL + 1, WindowTeaCreateProjectPNAMEU + 1, WindowTeaCreateProjectPNAMER - 1, WindowTeaCreateProjectPNAMED - 1);
	solidrectangle(WindowTeaCreateProjectPMAJORL + 1, WindowTeaCreateProjectPMAJORU + 1, WindowTeaCreateProjectPMAJORR - 1, WindowTeaCreateProjectPMAJORD - 1);
	solidrectangle(WindowTeaCreateProjectTNOL + 1, WindowTeaCreateProjectTNOU + 1, WindowTeaCreateProjectTNOR - 1, WindowTeaCreateProjectTNOD - 1);
	solidrectangle(WindowTeaCreateProjectPABSTRUCTL + 1, WindowTeaCreateProjectPABSTRUCTU + 1, WindowTeaCreateProjectPABSTRUCTR - 1, WindowTeaCreateProjectPABSTRUCTD - 1);

	TCHAR PNO_TCHAR[13 + 1];
	TCHAR PNAME_TCHAR[13 + 1];
	TCHAR PMAJOR_TCHAR[13 + 1];
	TCHAR PABSTRUCT_TCHAR[13 + 1];

	char PNO_CHAR[13 + 1];
	char PNAME_CHAR[13 + 1];
	char PMAJOR_CHAR[13 + 1];
	char PABSTRUCT_CHAR[13 + 1];

	int flag_PNOIsFinish = 0;
	int flag_PNAMEIsFinish = 0;
	int flag_PMAJORIsFinish = 0;
	int flag_TNOIsFinish = 0;
	int flag_PABSTRUCTIsFinish = 0;

	TCHAR CurrentID_TCHAR[13 + 1];
	CharToTchar(CurrentID, CurrentID_TCHAR);
	outtextxy(WindowTeaCreateProjectTNOL + 1, WindowTeaCreateProjectTNOU + 2, CurrentID_TCHAR);
	flag_TNOIsFinish = 1;

	MOUSEMSG mouseTeaCreateProject;
	while (true)
	{
		mouseTeaCreateProject = GetMouseMsg();
		switch (mouseTeaCreateProject.uMsg)
		{
		case WM_LBUTTONDOWN:
		{
			if (mouseTeaCreateProject.x >= WindowTeaCreateProjectBackL && mouseTeaCreateProject.x <= WindowTeaCreateProjectBackR && mouseTeaCreateProject.y >= WindowTeaCreateProjectBackU && mouseTeaCreateProject.y <= WindowTeaCreateProjectBackD)
			{//BackButton
				char Msg_TeaCreateProjectGoTeaMain[] = { "request a TeaCreateProjectGoTeaMain" };
				LogPrint(INFO, Msg_TeaCreateProjectGoTeaMain);
				WindowTeaMain();
			}
			else if (mouseTeaCreateProject.x >= WindowTeaCreateProjectPNOL && mouseTeaCreateProject.x <= WindowTeaCreateProjectPNOR && mouseTeaCreateProject.y >= WindowTeaCreateProjectPNOU && mouseTeaCreateProject.y <= WindowTeaCreateProjectPNOD)
			{//Msg_WindowTeaCreateProjectPNO
				setfillcolor(BLACK);
				solidrectangle(WindowTeaCreateProjectPNOL + 1, WindowTeaCreateProjectPNOU + 1, WindowTeaCreateProjectPNOR - 1, WindowTeaCreateProjectPNOD - 1);
				setfillcolor(BLUE);
				solidrectangle(WindowTeaCreateProjectSubmitL + 174, WindowTeaCreateProjectSubmitU + 9, W, H);
				char Msg_WindowTeaCreateProjectPNO[] = { "Msg_WindowTeaCreateProjectPNO-题目编号" };
				LogPrint(INFO, Msg_WindowTeaCreateProjectPNO);
				InputBox(PNO_TCHAR, 13 + 1, TEXT("请输入题目编号(不超过13个字符)"));
				TcharToChar(PNO_TCHAR, PNO_CHAR);
				//进行重复与否的检验
				char sql_WindowTeaCreateProjectP1[MAXLEN] = { "SELECT PNO FROM PROJECT WHERE(PNO=='" };
				char sql_WindowTeaCreateProjectP2[MAXLEN] = { "');" };
				strcat(sql_WindowTeaCreateProjectP1, PNO_CHAR);
				strcat(sql_WindowTeaCreateProjectP1, sql_WindowTeaCreateProjectP2);
				const char *sql_WindowTeaCreateProject = sql_WindowTeaCreateProjectP1;
				LogPrint(DEBUG, (char*)sql_WindowTeaCreateProject);
				ItemNum = 0;
				exec = sqlite3_exec(DB, sql_WindowTeaCreateProject, sql_CallBack_Func_InitItemNum, 0, &errmsg);
				if (exec != SQLITE_OK)//SQLITE_ABORT
				{
					//如果出错，就会回到这里来	
					char Msg_sqliteFailedToSelect[] = { "FAILED TO INIT ItemNum" };
					LogPrint(ERROR, Msg_sqliteFailedToSelect);
					LogPrint(ERROR, errmsg);
				}
				else//SQLITE_OK
				{
					char Msg_sqliteSucceedToSelect[] = { "SUCCEED IN INIT ItemNum" };
					LogPrint(INFO, Msg_sqliteSucceedToSelect);
				}
				if (ItemNum >= 1)
				{
					TCHAR PNOIsSame[] = { TEXT("题目编号与已有的重复") };
					outtextxy(WindowTeaCreateProjectSubmitL + 175, WindowTeaCreateProjectSubmitU + 10, PNOIsSame);
				}
				else//没有重复
				{
					outtextxy(WindowTeaCreateProjectPNOL + 1, WindowTeaCreateProjectPNOU + 2, PNO_TCHAR);
					LogPrint(DEBUG, PNO_CHAR);
					flag_PNOIsFinish = 1;
				}
			}
			else if (mouseTeaCreateProject.x >= WindowTeaCreateProjectPNAMEL && mouseTeaCreateProject.x <= WindowTeaCreateProjectPNAMER && mouseTeaCreateProject.y >= WindowTeaCreateProjectPNAMEU && mouseTeaCreateProject.y <= WindowTeaCreateProjectPNAMED)
			{//Msg_WindowTeaCreateProjectPNAME
				setfillcolor(BLACK);
				solidrectangle(WindowTeaCreateProjectPNAMEL + 1, WindowTeaCreateProjectPNAMEU + 1, WindowTeaCreateProjectPNAMER - 1, WindowTeaCreateProjectPNAMED - 1);
				char Msg_WindowTeaCreateProjectPNAME[] = { "Msg_WindowTeaCreateProjectPNAME-题目名称" };
				LogPrint(INFO, Msg_WindowTeaCreateProjectPNAME);
				InputBox(PNAME_TCHAR, 13 + 1, TEXT("请输入题目名称(不超过13个字符)"));
				outtextxy(WindowTeaCreateProjectPNAMEL + 1, WindowTeaCreateProjectPNAMEU + 2, PNAME_TCHAR);
				TcharToChar(PNAME_TCHAR, PNAME_CHAR);
				LogPrint(DEBUG, PNAME_CHAR);
				flag_PNAMEIsFinish = 1;
			}
			else if (mouseTeaCreateProject.x >= WindowTeaCreateProjectPMAJORL && mouseTeaCreateProject.x <= WindowTeaCreateProjectPMAJORR && mouseTeaCreateProject.y >= WindowTeaCreateProjectPMAJORU && mouseTeaCreateProject.y <= WindowTeaCreateProjectPMAJORD)
			{//Msg_WindowTeaCreateProjectPMAJOR
				setfillcolor(BLACK);
				solidrectangle(WindowTeaCreateProjectPMAJORL + 1, WindowTeaCreateProjectPMAJORU + 1, WindowTeaCreateProjectPMAJORR - 1, WindowTeaCreateProjectPMAJORD - 1);
				char Msg_WindowTeaCreateProjectPMAJOR[] = { "Msg_WindowTeaCreateProjectPMAJOR-题目专业" };
				LogPrint(INFO, Msg_WindowTeaCreateProjectPMAJOR);
				InputBox(PMAJOR_TCHAR, 13 + 1, TEXT("请输入题目专业(不超过13个字符)"));
				outtextxy(WindowTeaCreateProjectPMAJORL + 1, WindowTeaCreateProjectPMAJORU + 2, PMAJOR_TCHAR);
				TcharToChar(PMAJOR_TCHAR, PMAJOR_CHAR);
				LogPrint(DEBUG, PMAJOR_CHAR);
				flag_PMAJORIsFinish = 1;
			}
			else if (mouseTeaCreateProject.x >= WindowTeaCreateProjectTNOL && mouseTeaCreateProject.x <= WindowTeaCreateProjectTNOR && mouseTeaCreateProject.y >= WindowTeaCreateProjectTNOU && mouseTeaCreateProject.y <= WindowTeaCreateProjectTNOD)
			{//Msg_WindowTeaCreateProjectTNO
				setfillcolor(BLACK);
				solidrectangle(WindowTeaCreateProjectTNOL + 1, WindowTeaCreateProjectTNOU + 1, WindowTeaCreateProjectTNOR - 1, WindowTeaCreateProjectTNOD - 1);
				char Msg_WindowTeaCreateProjectTNO[] = { "Msg_WindowTeaCreateProjectTNO-命题导师" };
				LogPrint(INFO, Msg_WindowTeaCreateProjectTNO);
				outtextxy(WindowTeaCreateProjectTNOL + 1, WindowTeaCreateProjectTNOU + 2, CurrentID_TCHAR);
				LogPrint(DEBUG, CurrentID);
				flag_TNOIsFinish = 1;
			}
			else if (mouseTeaCreateProject.x >= WindowTeaCreateProjectPABSTRUCTL && mouseTeaCreateProject.x <= WindowTeaCreateProjectPABSTRUCTR && mouseTeaCreateProject.y >= WindowTeaCreateProjectPABSTRUCTU && mouseTeaCreateProject.y <= WindowTeaCreateProjectPABSTRUCTD)
			{//Msg_WindowTeaCreateProjectPABSTRUCT
				setfillcolor(BLACK);
				solidrectangle(WindowTeaCreateProjectPABSTRUCTL + 1, WindowTeaCreateProjectPABSTRUCTU + 1, WindowTeaCreateProjectPABSTRUCTR - 1, WindowTeaCreateProjectPABSTRUCTD - 1);
				char Msg_WindowTeaCreateProjectPABSTRUCT[] = { "Msg_WindowTeaCreateProjectPABSTRUCT-内容简介" };
				LogPrint(INFO, Msg_WindowTeaCreateProjectPABSTRUCT);
				InputBox(PABSTRUCT_TCHAR, 13 + 1, TEXT("请输入内容简介(不超过13个字符)"));
				outtextxy(WindowTeaCreateProjectPABSTRUCTL + 1, WindowTeaCreateProjectPABSTRUCTU + 2, PABSTRUCT_TCHAR);
				TcharToChar(PABSTRUCT_TCHAR, PABSTRUCT_CHAR);
				LogPrint(DEBUG, PABSTRUCT_CHAR);
				flag_PABSTRUCTIsFinish = 1;
			}

			else if (mouseTeaCreateProject.x >= WindowTeaCreateProjectSubmitL && mouseTeaCreateProject.x <= WindowTeaCreateProjectSubmitR && mouseTeaCreateProject.y >= WindowTeaCreateProjectSubmitU && mouseTeaCreateProject.y <= WindowTeaCreateProjectSubmitD)
			{//Msg_WindowTeaCreateProjectSubmit
				setfillcolor(BLUE);
				solidrectangle(WindowTeaCreateProjectSubmitL + 174, WindowTeaCreateProjectSubmitU + 9, W, H);
				char Msg_WindowTeaCreateProjectSubmit[] = { "Msg_WindowTeaCreateProjectSubmit-提交" };
				LogPrint(INFO, Msg_WindowTeaCreateProjectSubmit);
				//先判断会不会死掉
				//严格版本if (flag_PNOIsFinish == 1 && flag_PNAMEIsFinish == 1 && flag_PMAJORIsFinish == 1 && flag_TNOIsFinish == 1 && flag_PABSTRUCTIsFinish == 1 && flag_STELEIsFinish == 1 && flag_SEMAILIsFinish == 1 && flag_SOTHERSIsFinish == 1)
				if (flag_PNOIsFinish == 1 && flag_TNOIsFinish == 1)
				{
					//组装SQL语句
					char sql_WindowCreateProjectP1[MAXLEN] = { "INSERT INTO PROJECT VALUES('" };
					char sql_WindowCreateProjectP2[MAXLEN] = { "','" };
					char sql_WindowCreateProjectP3[MAXLEN] = { "');" };
					strcat(sql_WindowCreateProjectP1, PNO_CHAR);
					if (flag_PNAMEIsFinish == 1)
					{
						strcat(sql_WindowCreateProjectP1, sql_WindowCreateProjectP2);
						strcat(sql_WindowCreateProjectP1, PNAME_CHAR);
					}
					else
					{
						strcat(sql_WindowCreateProjectP1, sql_WindowCreateProjectP2);
					}
					if (flag_PMAJORIsFinish == 1)
					{
						strcat(sql_WindowCreateProjectP1, sql_WindowCreateProjectP2);
						strcat(sql_WindowCreateProjectP1, PMAJOR_CHAR);
					}
					else
					{
						strcat(sql_WindowCreateProjectP1, sql_WindowCreateProjectP2);
					}
					if (flag_TNOIsFinish == 1)
					{
						strcat(sql_WindowCreateProjectP1, sql_WindowCreateProjectP2);
						strcat(sql_WindowCreateProjectP1, CurrentID);
					}
					else
					{
						strcat(sql_WindowCreateProjectP1, sql_WindowCreateProjectP2);
					}
					if (flag_PABSTRUCTIsFinish == 0)
					{
						//提示需要补全至少内容简介的字段
						char Msg_SubmitButNotFinishedPABSTRUCT[] = { "Msg_SubmitButNotFinishedPABSTRUCT" };
						LogPrint(WARN, Msg_SubmitButNotFinishedPABSTRUCT);
					}
					else
					{
						strcat(sql_WindowCreateProjectP1, sql_WindowCreateProjectP2);
						strcat(sql_WindowCreateProjectP1, PABSTRUCT_CHAR);
						strcat(sql_WindowCreateProjectP1, sql_WindowCreateProjectP3);
						const char *sql_WindowCreateProject = sql_WindowCreateProjectP1;
						LogPrint(DEBUG, (char*)sql_WindowCreateProject);
						exec = sqlite3_exec(DB, sql_WindowCreateProject, sql_CallBack_Func_NewSubmit, 0, &errmsg);
						//回调函数是空的
						if (exec != SQLITE_OK)//SQLITE_ABORT
						{
							//如果插入出错，就会回到这里来	
							char Msg_sqliteFailedToSubmitProject[] = { "FAILED TO SUBMIT PROJECT" };
							LogPrint(ERROR, Msg_sqliteFailedToSubmitProject);
							LogPrint(ERROR, errmsg);
						}
						else//SQLITE_OK
						{
							//如果插入成功，就会回到这里来	
							//组装完成，成功开题
							char Msg_sqliteSucceedToSubmitProject[] = { "SUCCEED IN SUBMIT PROJECT" };
							LogPrint(INFO, Msg_sqliteSucceedToSubmitProject);
							setfillcolor(BLACK);
							solidrectangle(WindowTeaCreateProjectPNOL + 1, WindowTeaCreateProjectPNOU + 1, WindowTeaCreateProjectPNOR - 1, WindowTeaCreateProjectPNOD - 1);
							solidrectangle(WindowTeaCreateProjectPNAMEL + 1, WindowTeaCreateProjectPNAMEU + 1, WindowTeaCreateProjectPNAMER - 1, WindowTeaCreateProjectPNAMED - 1);
							solidrectangle(WindowTeaCreateProjectPMAJORL + 1, WindowTeaCreateProjectPMAJORU + 1, WindowTeaCreateProjectPMAJORR - 1, WindowTeaCreateProjectPMAJORD - 1);
							solidrectangle(WindowTeaCreateProjectPABSTRUCTL + 1, WindowTeaCreateProjectPABSTRUCTU + 1, WindowTeaCreateProjectPABSTRUCTR - 1, WindowTeaCreateProjectPABSTRUCTD - 1);
							TCHAR CreateProjectSuccess[] = { TEXT("开题成功") };
							Sleep(250);
							outtextxy(WindowTeaCreateProjectSubmitL + 175, WindowTeaCreateProjectSubmitU + 10, CreateProjectSuccess);
						}
					}
				}
				else
				{
					//提示需要补全至少题目编号与命题导师的字段
					char Msg_SubmitButNotFinishedPNOorTNO[] = { "Msg_SubmitButNotFinishedPNOorTNO" };
					LogPrint(WARN, Msg_SubmitButNotFinishedPNOorTNO);
				}
			}
		}
		}
	}
	return 0;
}

int WindowChairmanReviewSelectProject(void)
{
	cleardevice();
	FlushMouseMsgBuffer();
	setcliprgn(NULL);
	setpolyfillmode(WINDING);
	setfillcolor(BROWN);
	solidrectangle(0, 0, W, H);
	Sleep(350);

	setbkmode(TRANSPARENT);
	char Msg_initWindowChairmanReviewProject[] = { "initWindowChairmanReviewProject" };
	LogPrint(INFO, Msg_initWindowChairmanReviewProject);

	TCHAR WindowChairmanReviewProjectSlogan[] = { TEXT("主任，同学选这个题目，您看合适吗？") };
	outtextxy(285, 20, WindowChairmanReviewProjectSlogan);

	TCHAR WindowChairmanReviewProjectPageUp[] = { TEXT("上一页") };
	TCHAR WindowChairmanReviewProjectPageDown[] = { TEXT("下一页") };
	TCHAR WindowChairmanReviewProjectSubmit[] = { TEXT("允许该同学设计该题") };

	//概览窗口
	int WindowChairmanReviewProjectViewAllL = 50;
	int WindowChairmanReviewProjectViewAllU = 100;
	int WindowChairmanReviewProjectViewAllR = WindowChairmanReviewProjectViewAllL + 460;
	int WindowChairmanReviewProjectViewAllD = WindowChairmanReviewProjectViewAllU + 300;
	rectangle(WindowChairmanReviewProjectViewAllL, WindowChairmanReviewProjectViewAllU, WindowChairmanReviewProjectViewAllR, WindowChairmanReviewProjectViewAllD);

	//上一页按钮
	int WindowChairmanReviewProjectPageUpL = 550;
	int WindowChairmanReviewProjectPageUpU = 115;
	int WindowChairmanReviewProjectPageUpR = WindowChairmanReviewProjectPageUpL + 65;
	int WindowChairmanReviewProjectPageUpD = WindowChairmanReviewProjectPageUpU + 95;
	rectangle(WindowChairmanReviewProjectPageUpL, WindowChairmanReviewProjectPageUpU, WindowChairmanReviewProjectPageUpR, WindowChairmanReviewProjectPageUpD);

	//下一页按钮
	int WindowChairmanReviewProjectPageDownL = 685;
	int WindowChairmanReviewProjectPageDownU = 115;
	int WindowChairmanReviewProjectPageDownR = WindowChairmanReviewProjectPageDownL + 65;
	int WindowChairmanReviewProjectPageDownD = WindowChairmanReviewProjectPageDownU + 95;
	rectangle(WindowChairmanReviewProjectPageDownL, WindowChairmanReviewProjectPageDownU, WindowChairmanReviewProjectPageDownR, WindowChairmanReviewProjectPageDownD);

	//页面选择按钮
	int WindowChairmanReviewProjectPageSelectL = 640;
	int WindowChairmanReviewProjectPageSelectU = 115;
	int WindowChairmanReviewProjectPageSelectR = WindowChairmanReviewProjectPageSelectL + 20;
	int WindowChairmanReviewProjectPageSelectD = WindowChairmanReviewProjectPageSelectU + 95;
	rectangle(WindowChairmanReviewProjectPageSelectL, WindowChairmanReviewProjectPageSelectU, WindowChairmanReviewProjectPageSelectR, WindowChairmanReviewProjectPageSelectD);

	//确认批准按钮
	int WindowChairmanReviewProjectSubmitL = 550;
	int WindowChairmanReviewProjectSubmitU = 340;
	int WindowChairmanReviewProjectSubmitR = WindowChairmanReviewProjectSubmitL + 200;
	int WindowChairmanReviewProjectSubmitD = WindowChairmanReviewProjectSubmitU + 45;
	rectangle(WindowChairmanReviewProjectSubmitL, WindowChairmanReviewProjectSubmitU, WindowChairmanReviewProjectSubmitR, WindowChairmanReviewProjectSubmitD);

	//返回按钮
	int WindowChairmanReviewProjectBackL = 35;
	int WindowChairmanReviewProjectBackU = 20;
	int WindowChairmanReviewProjectBackR = WindowChairmanReviewProjectBackL + 75;
	int WindowChairmanReviewProjectBackD = WindowChairmanReviewProjectBackU + 25;
	rectangle(WindowChairmanReviewProjectBackL, WindowChairmanReviewProjectBackU, WindowChairmanReviewProjectBackR, WindowChairmanReviewProjectBackD);
	TCHAR WindowAdminBackSelectProjectButton[] = { TEXT("返回") };
	//返回图标
	outtextxy(WindowChairmanReviewProjectBackL + 35, WindowChairmanReviewProjectBackU + 4, WindowAdminBackSelectProjectButton);
	setfillcolor(YELLOW);
	int BackCircleX = WindowChairmanReviewProjectBackL + 5;
	int BackCircleY = WindowChairmanReviewProjectBackU + 5;
	solidcircle(BackCircleX + 7, BackCircleY + 7, 7);
	setfillcolor(BROWN);
	solidrectangle(BackCircleX + 5, BackCircleY + 6, BackCircleX + 12, BackCircleY + 8);
	POINT BackTriangle[] = { {BackCircleX + 1, BackCircleY + 7}, {BackCircleX + 6, BackCircleY + 3}, {BackCircleX + 6, BackCircleY + 11} };
	solidpolygon(BackTriangle, 3);

	setfillcolor(DARKGRAY);
	solidrectangle(WindowChairmanReviewProjectViewAllL + 1, WindowChairmanReviewProjectViewAllU + 1, WindowChairmanReviewProjectViewAllR - 1, WindowChairmanReviewProjectViewAllD - 1);
	setfillcolor(LIGHTGRAY);
	solidrectangle(WindowChairmanReviewProjectPageUpL + 1, WindowChairmanReviewProjectPageUpU + 1, WindowChairmanReviewProjectPageUpR - 1, WindowChairmanReviewProjectPageUpD - 1);
	solidrectangle(WindowChairmanReviewProjectPageDownL + 1, WindowChairmanReviewProjectPageDownU + 1, WindowChairmanReviewProjectPageDownR - 1, WindowChairmanReviewProjectPageDownD - 1);
	solidrectangle(WindowChairmanReviewProjectPageSelectL + 1, WindowChairmanReviewProjectPageSelectU + 1, WindowChairmanReviewProjectPageSelectR - 1, WindowChairmanReviewProjectPageSelectD - 1);
	setfillcolor(BLUE);
	solidrectangle(WindowChairmanReviewProjectSubmitL + 1, WindowChairmanReviewProjectSubmitU + 1, WindowChairmanReviewProjectSubmitR - 1, WindowChairmanReviewProjectSubmitD - 1);

	outtextxy(WindowChairmanReviewProjectPageUpL + 11, WindowChairmanReviewProjectPageUpU + 38, WindowChairmanReviewProjectPageUp);
	outtextxy(WindowChairmanReviewProjectPageDownL + 11, WindowChairmanReviewProjectPageDownU + 38, WindowChairmanReviewProjectPageDown);
	outtextxy(WindowChairmanReviewProjectSubmitL+15, WindowChairmanReviewProjectSubmitU + 15, WindowChairmanReviewProjectSubmit);

	TCHAR WindowChairmanReviewProjectPNO[] = { TEXT("题目编号") };
	TCHAR WindowChairmanReviewProjectPNAME[] = { TEXT("题目名称") };
	TCHAR WindowChairmanReviewProjectPMAJOR[] = { TEXT("题目专业") };
	TCHAR WindowChairmanReviewProjectTNO[] = { TEXT("命题导师") };
	TCHAR WindowChairmanReviewProjectPABSTRUCT[] = { TEXT("内容简介") };
	TCHAR WindowChairmanReviewProjectSNO[] = { TEXT("选题学生") };
	int WindowChairmanReviewProjectInfoColumnL = 75;
	int WindowChairmanReviewProjectInfoColumnU = 120;
	int WindowChairmanReviewProjectInfoColumnX = 0;
	int WindowChairmanReviewProjectInfoColumnY = 30;

	//题目编号
	int WindowChairmanReviewProjectPNOL = WindowChairmanReviewProjectInfoColumnL + WindowChairmanReviewProjectInfoColumnX;
	int WindowChairmanReviewProjectPNOU = WindowChairmanReviewProjectInfoColumnU;
	outtextxy(WindowChairmanReviewProjectInfoColumnL, WindowChairmanReviewProjectInfoColumnU, WindowChairmanReviewProjectPNO);
	WindowChairmanReviewProjectInfoColumnU += WindowChairmanReviewProjectInfoColumnY;
	//题目名称
	int WindowChairmanReviewProjectPNAMEL = WindowChairmanReviewProjectInfoColumnL + WindowChairmanReviewProjectInfoColumnX;
	int WindowChairmanReviewProjectPNAMEU = WindowChairmanReviewProjectInfoColumnU;
	outtextxy(WindowChairmanReviewProjectInfoColumnL, WindowChairmanReviewProjectInfoColumnU, WindowChairmanReviewProjectPNAME);
	WindowChairmanReviewProjectInfoColumnU += WindowChairmanReviewProjectInfoColumnY;
	//题目专业
	int WindowChairmanReviewProjectPMAJORL = WindowChairmanReviewProjectInfoColumnL + WindowChairmanReviewProjectInfoColumnX;
	int WindowChairmanReviewProjectPMAJORU = WindowChairmanReviewProjectInfoColumnU;
	outtextxy(WindowChairmanReviewProjectInfoColumnL, WindowChairmanReviewProjectInfoColumnU, WindowChairmanReviewProjectPMAJOR);
	WindowChairmanReviewProjectInfoColumnU += WindowChairmanReviewProjectInfoColumnY;
	//命题导师
	int WindowChairmanReviewProjectTNOL = WindowChairmanReviewProjectInfoColumnL + WindowChairmanReviewProjectInfoColumnX;
	int WindowChairmanReviewProjectTNOU = WindowChairmanReviewProjectInfoColumnU;
	outtextxy(WindowChairmanReviewProjectInfoColumnL, WindowChairmanReviewProjectInfoColumnU, WindowChairmanReviewProjectTNO);
	WindowChairmanReviewProjectInfoColumnU += WindowChairmanReviewProjectInfoColumnY;
	//内容简介
	int WindowChairmanReviewProjectPABSTRUCTL = WindowChairmanReviewProjectInfoColumnL + WindowChairmanReviewProjectInfoColumnX;
	int WindowChairmanReviewProjectPABSTRUCTU = WindowChairmanReviewProjectInfoColumnU;
	outtextxy(WindowChairmanReviewProjectInfoColumnL, WindowChairmanReviewProjectInfoColumnU, WindowChairmanReviewProjectPABSTRUCT);
	WindowChairmanReviewProjectInfoColumnU += WindowChairmanReviewProjectInfoColumnY;
	//选题学生
	int WindowChairmanReviewProjectSNOL = WindowChairmanReviewProjectInfoColumnL + WindowChairmanReviewProjectInfoColumnX;
	int WindowChairmanReviewProjectSNOU = WindowChairmanReviewProjectInfoColumnU;
	outtextxy(WindowChairmanReviewProjectInfoColumnL, WindowChairmanReviewProjectInfoColumnU, WindowChairmanReviewProjectSNO);
	WindowChairmanReviewProjectInfoColumnU = 120;

	//查询所有题库里的题目
	char sql_WindowAdminAllSelectProject[MAXLEN] = { "SELECT PROJECT.PNO,PROJECT.PNAME,PROJECT.PMAJOR,PROJECT.TNO,PROJECT.PABSTRUCT,GRADUATION.SNO  FROM PROJECT,GRADUATION WHERE((GRADUATION.PNO==PROJECT.PNO) AND (GRADUATION.PROCESS==0));" };
	LogPrint(DEBUG, (char*)sql_WindowAdminAllSelectProject);
	exec = sqlite3_exec(DB, sql_WindowAdminAllSelectProject, sql_CallBack_Func_InitItemNum, 0, &errmsg);
	if (exec != SQLITE_OK)//SQLITE_ABORT
	{
		//如果出错，就会回到这里来	
		char Msg_sqliteFailedToSelectProject[] = { "FAILED TO INIT Prj ItemNum" };
		LogPrint(ERROR, Msg_sqliteFailedToSelectProject);
		LogPrint(ERROR, errmsg);
	}
	else//SQLITE_OK
	{
		char Msg_sqliteSucceedToSelectProject[] = { "SUCCEED IN INIT Prj ItemNum" };
		LogPrint(INFO, Msg_sqliteSucceedToSelectProject);

		ItemTempCount = 0;
		ItemRestri = 1;
		ItemPage = (ItemNum / ItemRestri) + 1;

		ItemCurrentPage = 1;
		Start = (ItemCurrentPage - 1)*ItemRestri + 1;
		End = (ItemCurrentPage)*ItemRestri;

		exec = sqlite3_exec(DB, sql_WindowAdminAllSelectProject, sql_CallBack_Func_Chairman_ReviewProject, 0, &errmsg);
	}

	MOUSEMSG mouseChairmanReviewProject;
	while (true)
	{
		mouseChairmanReviewProject = GetMouseMsg();
		switch (mouseChairmanReviewProject.uMsg)
		{
		case WM_LBUTTONDOWN:
		{
			if (mouseChairmanReviewProject.x >= WindowChairmanReviewProjectBackL && mouseChairmanReviewProject.x <= WindowChairmanReviewProjectBackR && mouseChairmanReviewProject.y >= WindowChairmanReviewProjectBackU && mouseChairmanReviewProject.y <= WindowChairmanReviewProjectBackD)
			{//BackButton
				char Msg_ChairmanReviewProjectGoAdmin[] = { "request a ChairmanReviewProjectGoAdmin" };
				LogPrint(INFO, Msg_ChairmanReviewProjectGoAdmin);
				WindowChairmanMain();
			}
			else if (mouseChairmanReviewProject.x >= WindowChairmanReviewProjectPageUpL && mouseChairmanReviewProject.x <= WindowChairmanReviewProjectPageUpR && mouseChairmanReviewProject.y >= WindowChairmanReviewProjectPageUpU && mouseChairmanReviewProject.y <= WindowChairmanReviewProjectPageUpD)
			{//Msg_WindowChairmanReviewProjectPageUp
				char Msg_WindowChairmanReviewProjectPageUp[] = { "Msg_WindowChairmanReviewProjectPageUp-上一页" };
				LogPrint(INFO, Msg_WindowChairmanReviewProjectPageUp);
				if (ItemCurrentPage - 1 >= 1)
				{
					setfillcolor(DARKGRAY);
					solidrectangle(WindowChairmanReviewProjectInfoColumnL + 75, WindowChairmanReviewProjectViewAllU + 1, WindowChairmanReviewProjectViewAllR - 1, WindowChairmanReviewProjectViewAllD - 1);
					ItemTempCount = 0;
					ItemCurrentPage -= 1;
					Start = (ItemCurrentPage - 1)*ItemRestri + 1;
					End = (ItemCurrentPage)*ItemRestri;
					exec = sqlite3_exec(DB, sql_WindowAdminAllSelectProject, sql_CallBack_Func_Chairman_ReviewProject, 0, &errmsg);
				}
			}
			else if (mouseChairmanReviewProject.x >= WindowChairmanReviewProjectPageDownL && mouseChairmanReviewProject.x <= WindowChairmanReviewProjectPageDownR && mouseChairmanReviewProject.y >= WindowChairmanReviewProjectPageDownU && mouseChairmanReviewProject.y <= WindowChairmanReviewProjectPageDownD)
			{//Msg_WindowChairmanReviewProjectPageDown
				char Msg_WindowChairmanReviewProjectPageDown[] = { "Msg_WindowChairmanReviewProjectPageDown-下一页" };
				LogPrint(INFO, Msg_WindowChairmanReviewProjectPageDown);
				if (ItemCurrentPage + 1 <= ItemPage)
				{
					setfillcolor(DARKGRAY);
					solidrectangle(WindowChairmanReviewProjectInfoColumnL + 75, WindowChairmanReviewProjectViewAllU + 1, WindowChairmanReviewProjectViewAllR - 1, WindowChairmanReviewProjectViewAllD - 1);
					ItemTempCount = 0;
					ItemCurrentPage += 1;
					Start = (ItemCurrentPage - 1)*ItemRestri + 1;
					End = (ItemCurrentPage)*ItemRestri;
					exec = sqlite3_exec(DB, sql_WindowAdminAllSelectProject, sql_CallBack_Func_Chairman_ReviewProject, 0, &errmsg);
				}
			}
			else if (mouseChairmanReviewProject.x >= WindowChairmanReviewProjectPageSelectL && mouseChairmanReviewProject.x <= WindowChairmanReviewProjectPageSelectR && mouseChairmanReviewProject.y >= WindowChairmanReviewProjectPageSelectU && mouseChairmanReviewProject.y <= WindowChairmanReviewProjectPageSelectD)
			{//Msg_WindowChairmanReviewProjectPageSelect
				//是废的
				char Msg_WindowChairmanReviewProjectPageSelect[] = { "Msg_WindowChairmanReviewProjectPageSelect-选择页面" };
				LogPrint(INFO, Msg_WindowChairmanReviewProjectPageSelect);
				//InputBox(PageNum_TCHAR, 3 + 1, TEXT("请输入想要跳转到的页面(不超过3个字符)"));
				//outtextxy(WindowChairmanReviewProjectPageSelectL + 6, WindowChairmanReviewProjectPageSelectU + 40, PageNum_TCHAR);
				//TcharToChar(PageNum_TCHAR, PageNum_CHAR);
				//LogPrint(DEBUG, PageNum_CHAR);
			}
			else if (mouseChairmanReviewProject.x >= WindowChairmanReviewProjectSubmitL && mouseChairmanReviewProject.x <= WindowChairmanReviewProjectSubmitR && mouseChairmanReviewProject.y >= WindowChairmanReviewProjectSubmitU && mouseChairmanReviewProject.y <= WindowChairmanReviewProjectSubmitD)
			{//Msg_WindowChairmanReviewProjectSubmit
				char Msg_WindowChairmanReviewProjectSubmit[] = { "Msg_WindowChairmanReviewProjectSubmit-批准某开题" };
				LogPrint(INFO, Msg_WindowChairmanReviewProjectSubmit);
				//CurrentItemExtern-TNO
				//CurrentItem-PNO
				char sql_WindowNewStuInsertGraduationP1[MAXLEN] = { "UPDATE GRADUATION SET PROCESS=1 WHERE(PNO=='" };
				char sql_WindowNewStuInsertGraduationP2[MAXLEN] = { "');" };
				strcat(sql_WindowNewStuInsertGraduationP1, CurrentItem);//PNO
				strcat(sql_WindowNewStuInsertGraduationP1, sql_WindowNewStuInsertGraduationP2);
				const char *sql_WindowNewStuInsertGraduation = sql_WindowNewStuInsertGraduationP1;
				LogPrint(DEBUG, (char*)sql_WindowNewStuInsertGraduation);
				exec = sqlite3_exec(DB, sql_WindowNewStuInsertGraduation, sql_CallBack_Func_NewSubmit, 0, &errmsg);
				if (exec != SQLITE_OK)//SQLITE_ABORT
				{
					//如果插入出错，就会回到这里来	
					char Msg_sqliteFailedToApproveProject[] = { "FAILED TO APPROVE PROJECT" };
					LogPrint(ERROR, Msg_sqliteFailedToApproveProject);
					LogPrint(ERROR, errmsg);
				}
				else//SQLITE_OK
				{
					char Msg_sqliteSucceedInApproveProject[] = { "SUCCEED IN APPROVE PROJECT" };
					LogPrint(INFO, Msg_sqliteSucceedInApproveProject);
					TCHAR ApproveProjectSuccess[] = { TEXT("该开题已批准") };
					Sleep(250);
					outtextxy(WindowChairmanReviewProjectSubmitL + 55, WindowChairmanReviewProjectSubmitU + 55, ApproveProjectSuccess);
				}
				Sleep(550);
				WindowChairmanMain();
			}
		}
		default:
			break;
		}
	}

	return 0;
}

int WindowTeaReviewProject(void)
{
	cleardevice();
	FlushMouseMsgBuffer();
	setcliprgn(NULL);
	setpolyfillmode(WINDING);
	setfillcolor(BLUE);
	solidrectangle(0, 0, W, H);
	Sleep(350);

	setbkmode(TRANSPARENT);
	char Msg_initWindowTeaReviewProject[] = { "initWindowTeaReviewProject" };
	LogPrint(INFO, Msg_initWindowTeaReviewProject);

	TCHAR WindowTeaReviewProjectSlogan[] = { TEXT("老师，同学的论文进展，您满意吗？") };
	outtextxy(285, 20, WindowTeaReviewProjectSlogan);

	TCHAR WindowTeaReviewProjectPageUp[] = { TEXT("上一页") };
	TCHAR WindowTeaReviewProjectPageDown[] = { TEXT("下一页") };
	TCHAR WindowTeaReviewProjectMid[] = { TEXT("中期检查通过") };
	TCHAR WindowTeaReviewProjectAdvice[] = { TEXT("导师意见通过") };

	//概览窗口
	int WindowTeaReviewProjectViewAllL = 50;
	int WindowTeaReviewProjectViewAllU = 100;
	int WindowTeaReviewProjectViewAllR = WindowTeaReviewProjectViewAllL + 460;
	int WindowTeaReviewProjectViewAllD = WindowTeaReviewProjectViewAllU + 300;
	rectangle(WindowTeaReviewProjectViewAllL, WindowTeaReviewProjectViewAllU, WindowTeaReviewProjectViewAllR, WindowTeaReviewProjectViewAllD);

	//上一页按钮
	int WindowTeaReviewProjectPageUpL = 550;
	int WindowTeaReviewProjectPageUpU = 115;
	int WindowTeaReviewProjectPageUpR = WindowTeaReviewProjectPageUpL + 65;
	int WindowTeaReviewProjectPageUpD = WindowTeaReviewProjectPageUpU + 95;
	rectangle(WindowTeaReviewProjectPageUpL, WindowTeaReviewProjectPageUpU, WindowTeaReviewProjectPageUpR, WindowTeaReviewProjectPageUpD);

	//下一页按钮
	int WindowTeaReviewProjectPageDownL = 685;
	int WindowTeaReviewProjectPageDownU = 115;
	int WindowTeaReviewProjectPageDownR = WindowTeaReviewProjectPageDownL + 65;
	int WindowTeaReviewProjectPageDownD = WindowTeaReviewProjectPageDownU + 95;
	rectangle(WindowTeaReviewProjectPageDownL, WindowTeaReviewProjectPageDownU, WindowTeaReviewProjectPageDownR, WindowTeaReviewProjectPageDownD);

	//页面选择按钮
	int WindowTeaReviewProjectPageSelectL = 640;
	int WindowTeaReviewProjectPageSelectU = 115;
	int WindowTeaReviewProjectPageSelectR = WindowTeaReviewProjectPageSelectL + 20;
	int WindowTeaReviewProjectPageSelectD = WindowTeaReviewProjectPageSelectU + 95;
	rectangle(WindowTeaReviewProjectPageSelectL, WindowTeaReviewProjectPageSelectU, WindowTeaReviewProjectPageSelectR, WindowTeaReviewProjectPageSelectD);

	//中期检查通过按钮
	int WindowTeaReviewProjectMidL = 550;
	int WindowTeaReviewProjectMidU = 340;
	int WindowTeaReviewProjectMidR = WindowTeaReviewProjectMidL + 95;
	int WindowTeaReviewProjectMidD = WindowTeaReviewProjectMidU + 45;
	rectangle(WindowTeaReviewProjectMidL, WindowTeaReviewProjectMidU, WindowTeaReviewProjectMidR, WindowTeaReviewProjectMidD);

	//导师意见通过按钮
	int WindowTeaReviewProjectAdviceL = 655;
	int WindowTeaReviewProjectAdviceU = 340;
	int WindowTeaReviewProjectAdviceR = WindowTeaReviewProjectAdviceL + 95;
	int WindowTeaReviewProjectAdviceD = WindowTeaReviewProjectAdviceU + 45;
	rectangle(WindowTeaReviewProjectAdviceL, WindowTeaReviewProjectAdviceU, WindowTeaReviewProjectAdviceR, WindowTeaReviewProjectAdviceD);

	//返回按钮
	int WindowTeaReviewProjectBackL = 35;
	int WindowTeaReviewProjectBackU = 20;
	int WindowTeaReviewProjectBackR = WindowTeaReviewProjectBackL + 75;
	int WindowTeaReviewProjectBackD = WindowTeaReviewProjectBackU + 25;
	rectangle(WindowTeaReviewProjectBackL, WindowTeaReviewProjectBackU, WindowTeaReviewProjectBackR, WindowTeaReviewProjectBackD);
	TCHAR WindowAdminBackSelectProjectButton[] = { TEXT("返回") };
	//返回图标
	outtextxy(WindowTeaReviewProjectBackL + 35, WindowTeaReviewProjectBackU + 4, WindowAdminBackSelectProjectButton);
	setfillcolor(YELLOW);
	int BackCircleX = WindowTeaReviewProjectBackL + 5;
	int BackCircleY = WindowTeaReviewProjectBackU + 5;
	solidcircle(BackCircleX + 7, BackCircleY + 7, 7);
	setfillcolor(BROWN);
	solidrectangle(BackCircleX + 5, BackCircleY + 6, BackCircleX + 12, BackCircleY + 8);
	POINT BackTriangle[] = { {BackCircleX + 1, BackCircleY + 7}, {BackCircleX + 6, BackCircleY + 3}, {BackCircleX + 6, BackCircleY + 11} };
	solidpolygon(BackTriangle, 3);

	setfillcolor(DARKGRAY);
	solidrectangle(WindowTeaReviewProjectViewAllL + 1, WindowTeaReviewProjectViewAllU + 1, WindowTeaReviewProjectViewAllR - 1, WindowTeaReviewProjectViewAllD - 1);
	setfillcolor(LIGHTGRAY);
	solidrectangle(WindowTeaReviewProjectPageUpL + 1, WindowTeaReviewProjectPageUpU + 1, WindowTeaReviewProjectPageUpR - 1, WindowTeaReviewProjectPageUpD - 1);
	solidrectangle(WindowTeaReviewProjectPageDownL + 1, WindowTeaReviewProjectPageDownU + 1, WindowTeaReviewProjectPageDownR - 1, WindowTeaReviewProjectPageDownD - 1);
	solidrectangle(WindowTeaReviewProjectPageSelectL + 1, WindowTeaReviewProjectPageSelectU + 1, WindowTeaReviewProjectPageSelectR - 1, WindowTeaReviewProjectPageSelectD - 1);
	solidrectangle(WindowTeaReviewProjectMidL + 1, WindowTeaReviewProjectMidU + 1, WindowTeaReviewProjectMidR - 1, WindowTeaReviewProjectMidD - 1);
	solidrectangle(WindowTeaReviewProjectAdviceL + 1, WindowTeaReviewProjectAdviceU + 1, WindowTeaReviewProjectAdviceR - 1, WindowTeaReviewProjectAdviceD - 1);

	outtextxy(WindowTeaReviewProjectPageUpL + 11, WindowTeaReviewProjectPageUpU + 38, WindowTeaReviewProjectPageUp);
	outtextxy(WindowTeaReviewProjectPageDownL + 11, WindowTeaReviewProjectPageDownU + 38, WindowTeaReviewProjectPageDown);
	outtextxy(WindowTeaReviewProjectMidL, WindowTeaReviewProjectMidU + 15, WindowTeaReviewProjectMid);
	outtextxy(WindowTeaReviewProjectAdviceL, WindowTeaReviewProjectAdviceU + 15, WindowTeaReviewProjectAdvice);

	TCHAR WindowTeaReviewProjectPNO[] = { TEXT("题目编号") };
	TCHAR WindowTeaReviewProjectPNAME[] = { TEXT("题目名称") };
	TCHAR WindowTeaReviewProjectPMAJOR[] = { TEXT("题目专业") };
	TCHAR WindowTeaReviewProjectTNO[] = { TEXT("命题导师") };
	TCHAR WindowTeaReviewProjectPABSTRUCT[] = { TEXT("内容简介") };
	TCHAR WindowTeaReviewProjectSNO[] = { TEXT("选题学生") };
	int WindowTeaReviewProjectInfoColumnL = 75;
	int WindowTeaReviewProjectInfoColumnU = 120;
	int WindowTeaReviewProjectInfoColumnX = 0;
	int WindowTeaReviewProjectInfoColumnY = 30;

	//题目编号
	int WindowTeaReviewProjectPNOL = WindowTeaReviewProjectInfoColumnL + WindowTeaReviewProjectInfoColumnX;
	int WindowTeaReviewProjectPNOU = WindowTeaReviewProjectInfoColumnU;
	outtextxy(WindowTeaReviewProjectInfoColumnL, WindowTeaReviewProjectInfoColumnU, WindowTeaReviewProjectPNO);
	WindowTeaReviewProjectInfoColumnU += WindowTeaReviewProjectInfoColumnY;
	//题目名称
	int WindowTeaReviewProjectPNAMEL = WindowTeaReviewProjectInfoColumnL + WindowTeaReviewProjectInfoColumnX;
	int WindowTeaReviewProjectPNAMEU = WindowTeaReviewProjectInfoColumnU;
	outtextxy(WindowTeaReviewProjectInfoColumnL, WindowTeaReviewProjectInfoColumnU, WindowTeaReviewProjectPNAME);
	WindowTeaReviewProjectInfoColumnU += WindowTeaReviewProjectInfoColumnY;
	//题目专业
	int WindowTeaReviewProjectPMAJORL = WindowTeaReviewProjectInfoColumnL + WindowTeaReviewProjectInfoColumnX;
	int WindowTeaReviewProjectPMAJORU = WindowTeaReviewProjectInfoColumnU;
	outtextxy(WindowTeaReviewProjectInfoColumnL, WindowTeaReviewProjectInfoColumnU, WindowTeaReviewProjectPMAJOR);
	WindowTeaReviewProjectInfoColumnU += WindowTeaReviewProjectInfoColumnY;
	//命题导师
	int WindowTeaReviewProjectTNOL = WindowTeaReviewProjectInfoColumnL + WindowTeaReviewProjectInfoColumnX;
	int WindowTeaReviewProjectTNOU = WindowTeaReviewProjectInfoColumnU;
	outtextxy(WindowTeaReviewProjectInfoColumnL, WindowTeaReviewProjectInfoColumnU, WindowTeaReviewProjectTNO);
	WindowTeaReviewProjectInfoColumnU += WindowTeaReviewProjectInfoColumnY;
	//内容简介
	int WindowTeaReviewProjectPABSTRUCTL = WindowTeaReviewProjectInfoColumnL + WindowTeaReviewProjectInfoColumnX;
	int WindowTeaReviewProjectPABSTRUCTU = WindowTeaReviewProjectInfoColumnU;
	outtextxy(WindowTeaReviewProjectInfoColumnL, WindowTeaReviewProjectInfoColumnU, WindowTeaReviewProjectPABSTRUCT);
	WindowTeaReviewProjectInfoColumnU += WindowTeaReviewProjectInfoColumnY;
	//选题学生
	int WindowTeaReviewProjectSNOL = WindowTeaReviewProjectInfoColumnL + WindowTeaReviewProjectInfoColumnX;
	int WindowTeaReviewProjectSNOU = WindowTeaReviewProjectInfoColumnU;
	outtextxy(WindowTeaReviewProjectInfoColumnL, WindowTeaReviewProjectInfoColumnU, WindowTeaReviewProjectSNO);
	WindowTeaReviewProjectInfoColumnU = 120;

	//查询所有题库里的题目
	char sql_WindowAdminAllSelectProjectP1[MAXLEN] = { "SELECT PROJECT.PNO,PROJECT.PNAME,PROJECT.PMAJOR,PROJECT.TNO,PROJECT.PABSTRUCT,GRADUATION.SNO  FROM PROJECT,GRADUATION WHERE((GRADUATION.PNO==PROJECT.PNO) AND (GRADUATION.PROCESS==1 OR GRADUATION.PROCESS==2) AND (GRADUATION.TNO=='" };
	char sql_WindowAdminAllSelectProjectP2[MAXLEN] = { "'));" };
	strcat(sql_WindowAdminAllSelectProjectP1, CurrentID);
	strcat(sql_WindowAdminAllSelectProjectP1, sql_WindowAdminAllSelectProjectP2);
	const char* sql_WindowAdminAllSelectProject = sql_WindowAdminAllSelectProjectP1;
	LogPrint(DEBUG, (char*)sql_WindowAdminAllSelectProject);
	exec = sqlite3_exec(DB, sql_WindowAdminAllSelectProject, sql_CallBack_Func_InitItemNum, 0, &errmsg);
	if (exec != SQLITE_OK)//SQLITE_ABORT
	{
		//如果出错，就会回到这里来	
		char Msg_sqliteFailedToSelectProject[] = { "FAILED TO INIT Prj ItemNum" };
		LogPrint(ERROR, Msg_sqliteFailedToSelectProject);
		LogPrint(ERROR, errmsg);
	}
	else//SQLITE_OK
	{
		char Msg_sqliteSucceedToSelectProject[] = { "SUCCEED IN INIT Prj ItemNum" };
		LogPrint(INFO, Msg_sqliteSucceedToSelectProject);

		ItemTempCount = 0;
		ItemRestri = 1;
		ItemPage = (ItemNum / ItemRestri) + 1;

		ItemCurrentPage = 1;
		Start = (ItemCurrentPage - 1)*ItemRestri + 1;
		End = (ItemCurrentPage)*ItemRestri;

		exec = sqlite3_exec(DB, sql_WindowAdminAllSelectProject, sql_CallBack_Func_Chairman_ReviewProject, 0, &errmsg);
	}

	MOUSEMSG mouseTeaReviewProject;
	while (true)
	{
		mouseTeaReviewProject = GetMouseMsg();
		switch (mouseTeaReviewProject.uMsg)
		{
		case WM_LBUTTONDOWN:
		{
			if (mouseTeaReviewProject.x >= WindowTeaReviewProjectBackL && mouseTeaReviewProject.x <= WindowTeaReviewProjectBackR && mouseTeaReviewProject.y >= WindowTeaReviewProjectBackU && mouseTeaReviewProject.y <= WindowTeaReviewProjectBackD)
			{//BackButton
				char Msg_TeaReviewProjectGoAdmin[] = { "request a TeaReviewProjectGoAdmin" };
				LogPrint(INFO, Msg_TeaReviewProjectGoAdmin);
				WindowTeaMain();
			}
			else if (mouseTeaReviewProject.x >= WindowTeaReviewProjectPageUpL && mouseTeaReviewProject.x <= WindowTeaReviewProjectPageUpR && mouseTeaReviewProject.y >= WindowTeaReviewProjectPageUpU && mouseTeaReviewProject.y <= WindowTeaReviewProjectPageUpD)
			{//Msg_WindowTeaReviewProjectPageUp
				char Msg_WindowTeaReviewProjectPageUp[] = { "Msg_WindowTeaReviewProjectPageUp-上一页" };
				LogPrint(INFO, Msg_WindowTeaReviewProjectPageUp);
				if (ItemCurrentPage - 1 >= 1)
				{
					setfillcolor(DARKGRAY);
					solidrectangle(WindowTeaReviewProjectInfoColumnL + 75, WindowTeaReviewProjectViewAllU + 1, WindowTeaReviewProjectViewAllR - 1, WindowTeaReviewProjectViewAllD - 1);
					ItemTempCount = 0;
					ItemCurrentPage -= 1;
					Start = (ItemCurrentPage - 1)*ItemRestri + 1;
					End = (ItemCurrentPage)*ItemRestri;
					exec = sqlite3_exec(DB, sql_WindowAdminAllSelectProject, sql_CallBack_Func_Chairman_ReviewProject, 0, &errmsg);
				}
			}
			else if (mouseTeaReviewProject.x >= WindowTeaReviewProjectPageDownL && mouseTeaReviewProject.x <= WindowTeaReviewProjectPageDownR && mouseTeaReviewProject.y >= WindowTeaReviewProjectPageDownU && mouseTeaReviewProject.y <= WindowTeaReviewProjectPageDownD)
			{//Msg_WindowTeaReviewProjectPageDown
				char Msg_WindowTeaReviewProjectPageDown[] = { "Msg_WindowTeaReviewProjectPageDown-下一页" };
				LogPrint(INFO, Msg_WindowTeaReviewProjectPageDown);
				if (ItemCurrentPage + 1 <= ItemPage)
				{
					setfillcolor(DARKGRAY);
					solidrectangle(WindowTeaReviewProjectInfoColumnL + 75, WindowTeaReviewProjectViewAllU + 1, WindowTeaReviewProjectViewAllR - 1, WindowTeaReviewProjectViewAllD - 1);
					ItemTempCount = 0;
					ItemCurrentPage += 1;
					Start = (ItemCurrentPage - 1)*ItemRestri + 1;
					End = (ItemCurrentPage)*ItemRestri;
					exec = sqlite3_exec(DB, sql_WindowAdminAllSelectProject, sql_CallBack_Func_Chairman_ReviewProject, 0, &errmsg);
				}
			}
			else if (mouseTeaReviewProject.x >= WindowTeaReviewProjectPageSelectL && mouseTeaReviewProject.x <= WindowTeaReviewProjectPageSelectR && mouseTeaReviewProject.y >= WindowTeaReviewProjectPageSelectU && mouseTeaReviewProject.y <= WindowTeaReviewProjectPageSelectD)
			{//Msg_WindowTeaReviewProjectPageSelect
				//是废的
				char Msg_WindowTeaReviewProjectPageSelect[] = { "Msg_WindowTeaReviewProjectPageSelect-选择页面" };
				LogPrint(INFO, Msg_WindowTeaReviewProjectPageSelect);
				//InputBox(PageNum_TCHAR, 3 + 1, TEXT("请输入想要跳转到的页面(不超过3个字符)"));
				//outtextxy(WindowTeaReviewProjectPageSelectL + 6, WindowTeaReviewProjectPageSelectU + 40, PageNum_TCHAR);
				//TcharToChar(PageNum_TCHAR, PageNum_CHAR);
				//LogPrint(DEBUG, PageNum_CHAR);
			}
			else if (mouseTeaReviewProject.x >= WindowTeaReviewProjectMidL && mouseTeaReviewProject.x <= WindowTeaReviewProjectMidR && mouseTeaReviewProject.y >= WindowTeaReviewProjectMidU && mouseTeaReviewProject.y <= WindowTeaReviewProjectMidD)
			{//Msg_WindowTeaReviewProjectSubmit
				char Msg_WindowTeaReviewProjectMid[] = { "Msg_WindowTeaReviewProjectMid-中期检查通过" };
				LogPrint(INFO, Msg_WindowTeaReviewProjectMid);
				//CurrentItemExtern-TNO
				//CurrentItem-PNO
				char sql_WindowNewStuInsertGraduationP1[MAXLEN] = { "UPDATE GRADUATION SET PROCESS=2 WHERE(PNO=='" };
				char sql_WindowNewStuInsertGraduationP2[MAXLEN] = { "');" };
				strcat(sql_WindowNewStuInsertGraduationP1, CurrentItem);//PNO
				strcat(sql_WindowNewStuInsertGraduationP1, sql_WindowNewStuInsertGraduationP2);
				const char *sql_WindowNewStuInsertGraduation = sql_WindowNewStuInsertGraduationP1;
				LogPrint(DEBUG, (char*)sql_WindowNewStuInsertGraduation);
				exec = sqlite3_exec(DB, sql_WindowNewStuInsertGraduation, sql_CallBack_Func_NewSubmit, 0, &errmsg);
				if (exec != SQLITE_OK)//SQLITE_ABORT
				{
					//如果出错，就会回到这里来	
					char Msg_sqliteFailedToApproveProject[] = { "FAILED TO APPROVE PROJECT" };
					LogPrint(ERROR, Msg_sqliteFailedToApproveProject);
					LogPrint(ERROR, errmsg);
				}
				else//SQLITE_OK
				{
					char Msg_sqliteSucceedInApproveProject[] = { "SUCCEED IN APPROVE PROJECT" };
					LogPrint(INFO, Msg_sqliteSucceedInApproveProject);
					TCHAR ApproveProjectSuccess[] = { TEXT("中期已通过") };
					Sleep(250);
					outtextxy(WindowTeaReviewProjectMidL, WindowTeaReviewProjectMidU + 55, ApproveProjectSuccess);
				}
				Sleep(550);
				WindowTeaMain();
			}
			else if (mouseTeaReviewProject.x >= WindowTeaReviewProjectAdviceL && mouseTeaReviewProject.x <= WindowTeaReviewProjectAdviceR && mouseTeaReviewProject.y >= WindowTeaReviewProjectAdviceU && mouseTeaReviewProject.y <= WindowTeaReviewProjectAdviceD)
			{//Msg_WindowTeaReviewProjectSubmit
				char Msg_WindowTeaReviewProjectMid[] = { "Msg_WindowTeaReviewProjectMid-导师意见通过" };
				LogPrint(INFO, Msg_WindowTeaReviewProjectMid);
				//CurrentItemExtern-TNO
				//CurrentItem-PNO
				char sql_WindowNewStuInsertGraduationP1[MAXLEN] = { "UPDATE GRADUATION SET PROCESS=3 WHERE(PNO=='" };
				char sql_WindowNewStuInsertGraduationP2[MAXLEN] = { "');" };
				strcat(sql_WindowNewStuInsertGraduationP1, CurrentItem);//PNO
				strcat(sql_WindowNewStuInsertGraduationP1, sql_WindowNewStuInsertGraduationP2);
				const char *sql_WindowNewStuInsertGraduation = sql_WindowNewStuInsertGraduationP1;
				LogPrint(DEBUG, (char*)sql_WindowNewStuInsertGraduation);
				exec = sqlite3_exec(DB, sql_WindowNewStuInsertGraduation, sql_CallBack_Func_NewSubmit, 0, &errmsg);
				if (exec != SQLITE_OK)//SQLITE_ABORT
				{
					//如果出错，就会回到这里来	
					char Msg_sqliteFailedToApproveProject[] = { "FAILED TO APPROVE PROJECT" };
					LogPrint(ERROR, Msg_sqliteFailedToApproveProject);
					LogPrint(ERROR, errmsg);
				}
				else//SQLITE_OK
				{
					char Msg_sqliteSucceedInApproveProject[] = { "SUCCEED IN APPROVE PROJECT" };
					LogPrint(INFO, Msg_sqliteSucceedInApproveProject);
					TCHAR ApproveProjectSuccess[] = { TEXT("导师已通过") };
					Sleep(250);
					outtextxy(WindowTeaReviewProjectAdviceL, WindowTeaReviewProjectMidU + 55, ApproveProjectSuccess);
				}
				Sleep(550);
				WindowTeaMain();
			}
		}
		default:
			break;
		}
	}

	return 0;
}

int WindowStuSelectProject(void)
{
	cleardevice();
	FlushMouseMsgBuffer();
	setcliprgn(NULL);
	setpolyfillmode(WINDING);
	setfillcolor(GREEN);
	solidrectangle(0, 0, W, H);
	Sleep(350);

	setbkmode(TRANSPARENT);
	char Msg_initWindowStuSelectProject[] = { "initWindowStuSelectProject" };
	LogPrint(INFO, Msg_initWindowStuSelectProject);

	TCHAR WindowStuSelectProjectSlogan[] = { TEXT("小同学，想选哪个题目？") };
	outtextxy(285, 20, WindowStuSelectProjectSlogan);

	TCHAR WindowStuSelectProjectPageUp[] = { TEXT("上一页") };
	TCHAR WindowStuSelectProjectPageDown[] = { TEXT("下一页") };
	TCHAR WindowStuSelectProjectSubmit[] = { TEXT("选择这个题目") };

	//概览窗口
	int WindowStuSelectProjectViewAllL = 50;
	int WindowStuSelectProjectViewAllU = 100;
	int WindowStuSelectProjectViewAllR = WindowStuSelectProjectViewAllL + 460;
	int WindowStuSelectProjectViewAllD = WindowStuSelectProjectViewAllU + 300;
	rectangle(WindowStuSelectProjectViewAllL, WindowStuSelectProjectViewAllU, WindowStuSelectProjectViewAllR, WindowStuSelectProjectViewAllD);

	//上一页按钮
	int WindowStuSelectProjectPageUpL = 550;
	int WindowStuSelectProjectPageUpU = 115;
	int WindowStuSelectProjectPageUpR = WindowStuSelectProjectPageUpL + 65;
	int WindowStuSelectProjectPageUpD = WindowStuSelectProjectPageUpU + 95;
	rectangle(WindowStuSelectProjectPageUpL, WindowStuSelectProjectPageUpU, WindowStuSelectProjectPageUpR, WindowStuSelectProjectPageUpD);

	//下一页按钮
	int WindowStuSelectProjectPageDownL = 685;
	int WindowStuSelectProjectPageDownU = 115;
	int WindowStuSelectProjectPageDownR = WindowStuSelectProjectPageDownL + 65;
	int WindowStuSelectProjectPageDownD = WindowStuSelectProjectPageDownU + 95;
	rectangle(WindowStuSelectProjectPageDownL, WindowStuSelectProjectPageDownU, WindowStuSelectProjectPageDownR, WindowStuSelectProjectPageDownD);

	//页面选择按钮
	int WindowStuSelectProjectPageSelectL = 640;
	int WindowStuSelectProjectPageSelectU = 115;
	int WindowStuSelectProjectPageSelectR = WindowStuSelectProjectPageSelectL + 20;
	int WindowStuSelectProjectPageSelectD = WindowStuSelectProjectPageSelectU + 95;
	rectangle(WindowStuSelectProjectPageSelectL, WindowStuSelectProjectPageSelectU, WindowStuSelectProjectPageSelectR, WindowStuSelectProjectPageSelectD);

	//选择按钮
	int WindowStuSelectProjectSubmitL = 550;
	int WindowStuSelectProjectSubmitU = 340;
	int WindowStuSelectProjectSubmitR = WindowStuSelectProjectSubmitL + 200;
	int WindowStuSelectProjectSubmitD = WindowStuSelectProjectSubmitU + 45;
	rectangle(WindowStuSelectProjectSubmitL, WindowStuSelectProjectSubmitU, WindowStuSelectProjectSubmitR, WindowStuSelectProjectSubmitD);

	//返回按钮
	int WindowStuSelectProjectBackL = 35;
	int WindowStuSelectProjectBackU = 20;
	int WindowStuSelectProjectBackR = WindowStuSelectProjectBackL + 75;
	int WindowStuSelectProjectBackD = WindowStuSelectProjectBackU + 25;
	rectangle(WindowStuSelectProjectBackL, WindowStuSelectProjectBackU, WindowStuSelectProjectBackR, WindowStuSelectProjectBackD);
	TCHAR WindowAdminBackSelectProjectButton[] = { TEXT("返回") };
	//返回图标
	outtextxy(WindowStuSelectProjectBackL + 35, WindowStuSelectProjectBackU + 4, WindowAdminBackSelectProjectButton);
	setfillcolor(YELLOW);
	int BackCircleX = WindowStuSelectProjectBackL + 5;
	int BackCircleY = WindowStuSelectProjectBackU + 5;
	solidcircle(BackCircleX + 7, BackCircleY + 7, 7);
	setfillcolor(GREEN);
	solidrectangle(BackCircleX + 5, BackCircleY + 6, BackCircleX + 12, BackCircleY + 8);
	POINT BackTriangle[] = { {BackCircleX + 1, BackCircleY + 7}, {BackCircleX + 6, BackCircleY + 3}, {BackCircleX + 6, BackCircleY + 11} };
	solidpolygon(BackTriangle, 3);

	setfillcolor(DARKGRAY);
	solidrectangle(WindowStuSelectProjectViewAllL + 1, WindowStuSelectProjectViewAllU + 1, WindowStuSelectProjectViewAllR - 1, WindowStuSelectProjectViewAllD - 1);
	setfillcolor(LIGHTGRAY);
	solidrectangle(WindowStuSelectProjectPageUpL + 1, WindowStuSelectProjectPageUpU + 1, WindowStuSelectProjectPageUpR - 1, WindowStuSelectProjectPageUpD - 1);
	solidrectangle(WindowStuSelectProjectPageDownL + 1, WindowStuSelectProjectPageDownU + 1, WindowStuSelectProjectPageDownR - 1, WindowStuSelectProjectPageDownD - 1);
	solidrectangle(WindowStuSelectProjectPageSelectL + 1, WindowStuSelectProjectPageSelectU + 1, WindowStuSelectProjectPageSelectR - 1, WindowStuSelectProjectPageSelectD - 1);
	setfillcolor(LIGHTGREEN);
	solidrectangle(WindowStuSelectProjectSubmitL + 1, WindowStuSelectProjectSubmitU + 1, WindowStuSelectProjectSubmitR - 1, WindowStuSelectProjectSubmitD - 1);

	outtextxy(WindowStuSelectProjectPageUpL + 11, WindowStuSelectProjectPageUpU + 38, WindowStuSelectProjectPageUp);
	outtextxy(WindowStuSelectProjectPageDownL + 11, WindowStuSelectProjectPageDownU + 38, WindowStuSelectProjectPageDown);
	outtextxy(WindowStuSelectProjectSubmitL + 55, WindowStuSelectProjectSubmitU + 15, WindowStuSelectProjectSubmit);

	TCHAR WindowStuSelectProjectPNO[] = { TEXT("题目编号") };
	TCHAR WindowStuSelectProjectPNAME[] = { TEXT("题目名称") };
	TCHAR WindowStuSelectProjectPMAJOR[] = { TEXT("题目专业") };
	TCHAR WindowStuSelectProjectTNO[] = { TEXT("命题导师") };
	TCHAR WindowStuSelectProjectPABSTRUCT[] = { TEXT("内容简介") };
	int WindowStuSelectProjectInfoColumnL = 75;
	int WindowStuSelectProjectInfoColumnU = 120;
	int WindowStuSelectProjectInfoColumnX = 0;
	int WindowStuSelectProjectInfoColumnY = 30;

	//题目编号
	int WindowStuSelectProjectPNOL = WindowStuSelectProjectInfoColumnL + WindowStuSelectProjectInfoColumnX;
	int WindowStuSelectProjectPNOU = WindowStuSelectProjectInfoColumnU;
	outtextxy(WindowStuSelectProjectInfoColumnL, WindowStuSelectProjectInfoColumnU, WindowStuSelectProjectPNO);
	WindowStuSelectProjectInfoColumnU += WindowStuSelectProjectInfoColumnY;
	//题目名称
	int WindowStuSelectProjectPNAMEL = WindowStuSelectProjectInfoColumnL + WindowStuSelectProjectInfoColumnX;
	int WindowStuSelectProjectPNAMEU = WindowStuSelectProjectInfoColumnU;
	outtextxy(WindowStuSelectProjectInfoColumnL, WindowStuSelectProjectInfoColumnU, WindowStuSelectProjectPNAME);
	WindowStuSelectProjectInfoColumnU += WindowStuSelectProjectInfoColumnY;
	//题目专业
	int WindowStuSelectProjectPMAJORL = WindowStuSelectProjectInfoColumnL + WindowStuSelectProjectInfoColumnX;
	int WindowStuSelectProjectPMAJORU = WindowStuSelectProjectInfoColumnU;
	outtextxy(WindowStuSelectProjectInfoColumnL, WindowStuSelectProjectInfoColumnU, WindowStuSelectProjectPMAJOR);
	WindowStuSelectProjectInfoColumnU += WindowStuSelectProjectInfoColumnY;
	//命题导师
	int WindowStuSelectProjectTNOL = WindowStuSelectProjectInfoColumnL + WindowStuSelectProjectInfoColumnX;
	int WindowStuSelectProjectTNOU = WindowStuSelectProjectInfoColumnU;
	outtextxy(WindowStuSelectProjectInfoColumnL, WindowStuSelectProjectInfoColumnU, WindowStuSelectProjectTNO);
	WindowStuSelectProjectInfoColumnU += WindowStuSelectProjectInfoColumnY;
	//内容简介
	int WindowStuSelectProjectPABSTRUCTL = WindowStuSelectProjectInfoColumnL + WindowStuSelectProjectInfoColumnX;
	int WindowStuSelectProjectPABSTRUCTU = WindowStuSelectProjectInfoColumnU;
	outtextxy(WindowStuSelectProjectInfoColumnL, WindowStuSelectProjectInfoColumnU, WindowStuSelectProjectPABSTRUCT);
	WindowStuSelectProjectInfoColumnU = 120;

	//查询所有题库里的题目
	char sql_WindowAdminAllSelectProject[MAXLEN] = { "SELECT * FROM PROJECT WHERE(PNO!=(SELECT PROJECT.PNO FROM PROJECT,GRADUATION WHERE(GRADUATION.PNO==PROJECT.PNO)));" };
	LogPrint(DEBUG, (char*)sql_WindowAdminAllSelectProject);
	exec = sqlite3_exec(DB, sql_WindowAdminAllSelectProject, sql_CallBack_Func_InitItemNum, 0, &errmsg);
	if (exec != SQLITE_OK)//SQLITE_ABORT
	{
		//如果出错，就会回到这里来	
		char Msg_sqliteFailedToSelectProject[] = { "FAILED TO INIT Prj ItemNum" };
		LogPrint(ERROR, Msg_sqliteFailedToSelectProject);
		LogPrint(ERROR, errmsg);
	}
	else//SQLITE_OK
	{
		char Msg_sqliteSucceedToSelectProject[] = { "SUCCEED IN INIT Prj ItemNum" };
		LogPrint(INFO, Msg_sqliteSucceedToSelectProject);

		ItemTempCount = 0;
		ItemRestri = 1;
		ItemPage = (ItemNum / ItemRestri) + 1;

		ItemCurrentPage = 1;
		Start = (ItemCurrentPage - 1)*ItemRestri + 1;
		End = (ItemCurrentPage)*ItemRestri;

		exec = sqlite3_exec(DB, sql_WindowAdminAllSelectProject, sql_CallBack_Func_Stu_SelectProject, 0, &errmsg);
	}

	//int PageNum_INT = ItemCurrentPage;
	//后续可能需要自己整一套itoa的东西
	//不过反正是在win上不用考虑跨平台问题，反正定死是MSVC了，那就随便用itoa吧

	TCHAR PageNum_TCHAR[3 + 1];
	char PageNum_CHAR[3 + 1];

	//今日不造轮子
	//显示初始状态的页面

	MOUSEMSG mouseStuSelectProject;
	while (true)
	{
		mouseStuSelectProject = GetMouseMsg();
		switch (mouseStuSelectProject.uMsg)
		{
		case WM_LBUTTONDOWN:
		{
			if (mouseStuSelectProject.x >= WindowStuSelectProjectBackL && mouseStuSelectProject.x <= WindowStuSelectProjectBackR && mouseStuSelectProject.y >= WindowStuSelectProjectBackU && mouseStuSelectProject.y <= WindowStuSelectProjectBackD)
			{//BackButton
				char Msg_StuSelectProjectGoAdmin[] = { "request a StuSelectProjectGoAdmin" };
				LogPrint(INFO, Msg_StuSelectProjectGoAdmin);
				WindowStuMain();
			}
			else if (mouseStuSelectProject.x >= WindowStuSelectProjectPageUpL && mouseStuSelectProject.x <= WindowStuSelectProjectPageUpR && mouseStuSelectProject.y >= WindowStuSelectProjectPageUpU && mouseStuSelectProject.y <= WindowStuSelectProjectPageUpD)
			{//Msg_WindowStuSelectProjectPageUp
				char Msg_WindowStuSelectProjectPageUp[] = { "Msg_WindowStuSelectProjectPageUp-上一页" };
				LogPrint(INFO, Msg_WindowStuSelectProjectPageUp);
				if (ItemCurrentPage - 1 >= 1)
				{
					setfillcolor(DARKGRAY);
					solidrectangle(WindowStuSelectProjectInfoColumnL + 75, WindowStuSelectProjectViewAllU + 1, WindowStuSelectProjectViewAllR - 1, WindowStuSelectProjectViewAllD - 1);
					ItemTempCount = 0;
					ItemCurrentPage -= 1;
					Start = (ItemCurrentPage - 1)*ItemRestri + 1;
					End = (ItemCurrentPage)*ItemRestri;
					exec = sqlite3_exec(DB, sql_WindowAdminAllSelectProject, sql_CallBack_Func_Stu_SelectProject, 0, &errmsg);
				}
			}
			else if (mouseStuSelectProject.x >= WindowStuSelectProjectPageDownL && mouseStuSelectProject.x <= WindowStuSelectProjectPageDownR && mouseStuSelectProject.y >= WindowStuSelectProjectPageDownU && mouseStuSelectProject.y <= WindowStuSelectProjectPageDownD)
			{//Msg_WindowStuSelectProjectPageDown
				char Msg_WindowStuSelectProjectPageDown[] = { "Msg_WindowStuSelectProjectPageDown-下一页" };
				LogPrint(INFO, Msg_WindowStuSelectProjectPageDown);
				if (ItemCurrentPage + 1 <= ItemPage)
				{
					setfillcolor(DARKGRAY);
					solidrectangle(WindowStuSelectProjectInfoColumnL + 75, WindowStuSelectProjectViewAllU + 1, WindowStuSelectProjectViewAllR - 1, WindowStuSelectProjectViewAllD - 1);
					ItemTempCount = 0;
					ItemCurrentPage += 1;
					Start = (ItemCurrentPage - 1)*ItemRestri + 1;
					End = (ItemCurrentPage)*ItemRestri;
					exec = sqlite3_exec(DB, sql_WindowAdminAllSelectProject, sql_CallBack_Func_Stu_SelectProject, 0, &errmsg);
				}
			}
			else if (mouseStuSelectProject.x >= WindowStuSelectProjectPageSelectL && mouseStuSelectProject.x <= WindowStuSelectProjectPageSelectR && mouseStuSelectProject.y >= WindowStuSelectProjectPageSelectU && mouseStuSelectProject.y <= WindowStuSelectProjectPageSelectD)
			{//Msg_WindowStuSelectProjectPageSelect
				//是废的
				char Msg_WindowStuSelectProjectPageSelect[] = { "Msg_WindowStuSelectProjectPageSelect-选择页面" };
				LogPrint(INFO, Msg_WindowStuSelectProjectPageSelect);
				InputBox(PageNum_TCHAR, 3 + 1, TEXT("请输入想要跳转到的页面(不超过3个字符)"));
				outtextxy(WindowStuSelectProjectPageSelectL + 6, WindowStuSelectProjectPageSelectU + 40, PageNum_TCHAR);
				TcharToChar(PageNum_TCHAR, PageNum_CHAR);
				LogPrint(DEBUG, PageNum_CHAR);
			}
			else if (mouseStuSelectProject.x >= WindowStuSelectProjectSubmitL && mouseStuSelectProject.x <= WindowStuSelectProjectSubmitR && mouseStuSelectProject.y >= WindowStuSelectProjectSubmitU && mouseStuSelectProject.y <= WindowStuSelectProjectSubmitD)
			{//Msg_WindowStuSelectProjectSubmit
				char Msg_WindowStuSelectProjectSubmit[] = { "Msg_WindowStuSelectProjectSubmit-选中某题目" };
				LogPrint(INFO, Msg_WindowStuSelectProjectSubmit);
				//CurrentItemExtern-TNO
				//CurrentItem-PNO
				char sql_WindowNewStuInsertGraduationP1[MAXLEN] = { "INSERT INTO GRADUATION VALUES('" };
				char sql_WindowNewStuInsertGraduationP2[MAXLEN] = { "','" };
				char sql_WindowNewStuInsertGraduationP3[MAXLEN] = { "'," };
				char sql_WindowNewStuInsertGraduationP4[MAXLEN] = { ");" };
				char sql_WindowNewStuInsertGraduationPROCESS[MAXLEN] = { "0" };
				strcat(sql_WindowNewStuInsertGraduationP1, CurrentItem);//PNO
				strcat(sql_WindowNewStuInsertGraduationP1, sql_WindowNewStuInsertGraduationP2);
				strcat(sql_WindowNewStuInsertGraduationP1, CurrentID);//SNO
				strcat(sql_WindowNewStuInsertGraduationP1, sql_WindowNewStuInsertGraduationP2);
				strcat(sql_WindowNewStuInsertGraduationP1, CurrentItemExtern);//TNO
				strcat(sql_WindowNewStuInsertGraduationP1, sql_WindowNewStuInsertGraduationP3);
				strcat(sql_WindowNewStuInsertGraduationP1, sql_WindowNewStuInsertGraduationPROCESS);//PROCESS
				strcat(sql_WindowNewStuInsertGraduationP1, sql_WindowNewStuInsertGraduationP4);
				const char *sql_WindowNewStuInsertGraduation = sql_WindowNewStuInsertGraduationP1;
				LogPrint(DEBUG, (char*)sql_WindowNewStuInsertGraduation);
				exec = sqlite3_exec(DB, sql_WindowNewStuInsertGraduation, sql_CallBack_Func_NewSubmit, 0, &errmsg);
				if (exec != SQLITE_OK)//SQLITE_ABORT
				{
					//如果插入出错，就会回到这里来	
					char Msg_sqliteFailedToInsertGraduation[] = { "FAILED TO INSERT USER" };
					LogPrint(ERROR, Msg_sqliteFailedToInsertGraduation);
					LogPrint(ERROR, errmsg);
				}
				else//SQLITE_OK
				{
					char Msg_sqliteIsOpen2[] = { "SUCCEED IN INSERT USER" };
					LogPrint(INFO, Msg_sqliteIsOpen2);
					TCHAR SelectProjectSuccess[] = { TEXT("选题成功") };
					Sleep(250);
					outtextxy(WindowStuSelectProjectSubmitL + 55, WindowStuSelectProjectSubmitU + 55, SelectProjectSuccess);
				}
				Sleep(550);
				WindowStuMain();
			}
		}
		default:
			break;
		}
	}

	return 0;
}

int WindowStuUploadProject(void)
{
	cleardevice();
	FlushMouseMsgBuffer();
	setcliprgn(NULL);
	setpolyfillmode(WINDING);
	setfillcolor(GREEN);
	solidrectangle(0, 0, W, H);
	Sleep(350);

	setbkmode(TRANSPARENT);
	char Msg_initWindowStuUploadProject[] = { "initWindowStuUploadProject" };
	LogPrint(INFO, Msg_initWindowStuUploadProject);

	TCHAR WindowStuUploadProjectSlogan[] = { TEXT("同学，论文写的怎么样了呀？") };
	outtextxy(285, 20, WindowStuUploadProjectSlogan);

	TCHAR WindowStuUploadProjectPageUp[] = { TEXT("上一页") };
	TCHAR WindowStuUploadProjectPageDown[] = { TEXT("下一页") };
	TCHAR WindowStuUploadProjectSubmit[] = { TEXT("选择这个题目") };
	TCHAR WindowStuUploadProjectStagenow[] = { TEXT("您当前的阶段为") };
	TCHAR WindowStuUploadProjectStagenext[] = { TEXT("您下一个阶段为") };
	TCHAR WindowStuUploadProjectStage0now[] = { TEXT("您已选题") };
	TCHAR WindowStuUploadProjectStage1now[] = { TEXT("开题报告系主任已审核") };
	TCHAR WindowStuUploadProjectStage2now[] = { TEXT("中期验收导师已通过") };
	TCHAR WindowStuUploadProjectStage3now[] = { TEXT("导师意见环节导师已通过") };
	TCHAR WindowStuUploadProjectStage4now[] = { TEXT("系主任审核已通过") };
	TCHAR WindowStuUploadProjectStage5now[] = { TEXT("毕业答辩已通过") };
	TCHAR WindowStuUploadProjectStage0next[] = { TEXT("系主任审核开题报告") };
	TCHAR WindowStuUploadProjectStage1next[] = { TEXT("导师中期验收") };
	TCHAR WindowStuUploadProjectStage2next[] = { TEXT("导师意见") };
	TCHAR WindowStuUploadProjectStage3next[] = { TEXT("系主任审核") };
	TCHAR WindowStuUploadProjectStage4next[] = { TEXT("毕业答辩") };
	TCHAR WindowStuUploadProjectStage5next[] = { TEXT("恭喜您顺利毕业！") };
	TCHAR WindowStuUploadProjectUpload[] = { TEXT("提交本环节进度") };
	TCHAR WindowStuUploadProjectReply[] = { TEXT("预约答辩的时间") };
	TCHAR WindowStuUploadProjectGreat[] = { TEXT("领取毕业证书") };

	//概览窗口
	int WindowStuUploadProjectViewAllL = 50;
	int WindowStuUploadProjectViewAllU = 80;
	int WindowStuUploadProjectViewAllR = WindowStuUploadProjectViewAllL + 700;
	int WindowStuUploadProjectViewAllD = WindowStuUploadProjectViewAllU + 30;
	rectangle(WindowStuUploadProjectViewAllL, WindowStuUploadProjectViewAllU, WindowStuUploadProjectViewAllR, WindowStuUploadProjectViewAllD);
	setfillcolor(DARKGRAY);
	solidrectangle(WindowStuUploadProjectViewAllL + 1, WindowStuUploadProjectViewAllU + 1, WindowStuUploadProjectViewAllR - 1, WindowStuUploadProjectViewAllD - 1);

	//Stage1
	int WindowStuUploadProjectStage1L = 50;
	int WindowStuUploadProjectStage1U = WindowStuUploadProjectViewAllU + 5;
	int WindowStuUploadProjectStage1R = WindowStuUploadProjectStage1L + 140;
	int WindowStuUploadProjectStage1D = WindowStuUploadProjectStage1U + 20;
	rectangle(WindowStuUploadProjectStage1L, WindowStuUploadProjectStage1U, WindowStuUploadProjectStage1R, WindowStuUploadProjectStage1D);

	//Stage2
	int WindowStuUploadProjectStage2L = WindowStuUploadProjectStage1R;
	int WindowStuUploadProjectStage2U = WindowStuUploadProjectViewAllU + 5;
	int WindowStuUploadProjectStage2R = WindowStuUploadProjectStage2L + 140;
	int WindowStuUploadProjectStage2D = WindowStuUploadProjectStage2U + 20;
	rectangle(WindowStuUploadProjectStage2L, WindowStuUploadProjectStage2U, WindowStuUploadProjectStage2R, WindowStuUploadProjectStage2D);

	//Stage3
	int WindowStuUploadProjectStage3L = WindowStuUploadProjectStage2R;
	int WindowStuUploadProjectStage3U = WindowStuUploadProjectViewAllU + 5;
	int WindowStuUploadProjectStage3R = WindowStuUploadProjectStage3L + 140;
	int WindowStuUploadProjectStage3D = WindowStuUploadProjectStage3U + 20;
	rectangle(WindowStuUploadProjectStage3L, WindowStuUploadProjectStage3U, WindowStuUploadProjectStage3R, WindowStuUploadProjectStage3D);

	//Stage4
	int WindowStuUploadProjectStage4L = WindowStuUploadProjectStage3R;
	int WindowStuUploadProjectStage4U = WindowStuUploadProjectViewAllU + 5;
	int WindowStuUploadProjectStage4R = WindowStuUploadProjectStage4L + 140;
	int WindowStuUploadProjectStage4D = WindowStuUploadProjectStage4U + 20;
	rectangle(WindowStuUploadProjectStage4L, WindowStuUploadProjectStage4U, WindowStuUploadProjectStage4R, WindowStuUploadProjectStage4D);

	//Stage5
	int WindowStuUploadProjectStage5L = WindowStuUploadProjectStage4R;
	int WindowStuUploadProjectStage5U = WindowStuUploadProjectViewAllU + 5;
	int WindowStuUploadProjectStage5R = WindowStuUploadProjectStage5L + 140;
	int WindowStuUploadProjectStage5D = WindowStuUploadProjectStage5U + 20;
	rectangle(WindowStuUploadProjectStage5L, WindowStuUploadProjectStage5U, WindowStuUploadProjectStage5R, WindowStuUploadProjectStage5D);

	//提交本环节进度按钮
	int WindowStuUploadProjectSubmitL = 300;
	int WindowStuUploadProjectSubmitU = 340;
	int WindowStuUploadProjectSubmitR = WindowStuUploadProjectSubmitL + 200;
	int WindowStuUploadProjectSubmitD = WindowStuUploadProjectSubmitU + 45;
	rectangle(WindowStuUploadProjectSubmitL, WindowStuUploadProjectSubmitU, WindowStuUploadProjectSubmitR, WindowStuUploadProjectSubmitD);

	//返回按钮
	int WindowStuUploadProjectBackL = 35;
	int WindowStuUploadProjectBackU = 20;
	int WindowStuUploadProjectBackR = WindowStuUploadProjectBackL + 75;
	int WindowStuUploadProjectBackD = WindowStuUploadProjectBackU + 25;
	rectangle(WindowStuUploadProjectBackL, WindowStuUploadProjectBackU, WindowStuUploadProjectBackR, WindowStuUploadProjectBackD);
	TCHAR WindowAdminBackUploadProjectButton[] = { TEXT("返回") };
	//返回图标
	outtextxy(WindowStuUploadProjectBackL + 35, WindowStuUploadProjectBackU + 4, WindowAdminBackUploadProjectButton);
	setfillcolor(YELLOW);
	int BackCircleX = WindowStuUploadProjectBackL + 5;
	int BackCircleY = WindowStuUploadProjectBackU + 5;
	solidcircle(BackCircleX + 7, BackCircleY + 7, 7);
	setfillcolor(GREEN);
	solidrectangle(BackCircleX + 5, BackCircleY + 6, BackCircleX + 12, BackCircleY + 8);
	POINT BackTriangle[] = { {BackCircleX + 1, BackCircleY + 7}, {BackCircleX + 6, BackCircleY + 3}, {BackCircleX + 6, BackCircleY + 11} };
	solidpolygon(BackTriangle, 3);

	setfillcolor(BROWN);
	solidrectangle(WindowStuUploadProjectSubmitL + 1, WindowStuUploadProjectSubmitU + 1, WindowStuUploadProjectSubmitR - 1, WindowStuUploadProjectSubmitD - 1);

	TCHAR WindowStuUploadProjectPNO[] = { TEXT("题目编号") };
	TCHAR WindowStuUploadProjectPNAME[] = { TEXT("题目名称") };
	TCHAR WindowStuUploadProjectPMAJOR[] = { TEXT("题目专业") };
	TCHAR WindowStuUploadProjectTNO[] = { TEXT("命题导师") };
	TCHAR WindowStuUploadProjectPABSTRUCT[] = { TEXT("内容简介") };
	int WindowStuUploadProjectInfoColumnL = 120;
	int WindowStuUploadProjectInfoColumnU = 150;
	int WindowStuUploadProjectInfoColumnX = 0;
	int WindowStuUploadProjectInfoColumnY = 30;

	//题目编号
	int WindowStuUploadProjectPNOL = WindowStuUploadProjectInfoColumnL + WindowStuUploadProjectInfoColumnX;
	int WindowStuUploadProjectPNOU = WindowStuUploadProjectInfoColumnU;
	outtextxy(WindowStuUploadProjectInfoColumnL, WindowStuUploadProjectInfoColumnU, WindowStuUploadProjectPNO);
	WindowStuUploadProjectInfoColumnU += WindowStuUploadProjectInfoColumnY;
	//题目名称
	int WindowStuUploadProjectPNAMEL = WindowStuUploadProjectInfoColumnL + WindowStuUploadProjectInfoColumnX;
	int WindowStuUploadProjectPNAMEU = WindowStuUploadProjectInfoColumnU;
	outtextxy(WindowStuUploadProjectInfoColumnL, WindowStuUploadProjectInfoColumnU, WindowStuUploadProjectPNAME);
	WindowStuUploadProjectInfoColumnU += WindowStuUploadProjectInfoColumnY;
	//题目专业
	int WindowStuUploadProjectPMAJORL = WindowStuUploadProjectInfoColumnL + WindowStuUploadProjectInfoColumnX;
	int WindowStuUploadProjectPMAJORU = WindowStuUploadProjectInfoColumnU;
	outtextxy(WindowStuUploadProjectInfoColumnL, WindowStuUploadProjectInfoColumnU, WindowStuUploadProjectPMAJOR);
	WindowStuUploadProjectInfoColumnU += WindowStuUploadProjectInfoColumnY;
	//命题导师
	int WindowStuUploadProjectTNOL = WindowStuUploadProjectInfoColumnL + WindowStuUploadProjectInfoColumnX;
	int WindowStuUploadProjectTNOU = WindowStuUploadProjectInfoColumnU;
	outtextxy(WindowStuUploadProjectInfoColumnL, WindowStuUploadProjectInfoColumnU, WindowStuUploadProjectTNO);
	WindowStuUploadProjectInfoColumnU += WindowStuUploadProjectInfoColumnY;
	//内容简介
	int WindowStuUploadProjectPABSTRUCTL = WindowStuUploadProjectInfoColumnL + WindowStuUploadProjectInfoColumnX;
	int WindowStuUploadProjectPABSTRUCTU = WindowStuUploadProjectInfoColumnU;
	outtextxy(WindowStuUploadProjectInfoColumnL, WindowStuUploadProjectInfoColumnU, WindowStuUploadProjectPABSTRUCT);
	WindowStuUploadProjectInfoColumnU = 120;

	int WindowStuUploadProjectNoticeL = 450;
	int WindowStuUploadProjectNoticeU = 150;
	int WindowStuUploadProjectNoticeY = 80;
	outtextxy(WindowStuUploadProjectSubmitL+45, WindowStuUploadProjectSubmitU+15, WindowStuUploadProjectUpload);
	outtextxy(WindowStuUploadProjectNoticeL, WindowStuUploadProjectNoticeU, WindowStuUploadProjectStagenow);
	WindowStuUploadProjectNoticeU += WindowStuUploadProjectNoticeY;
	outtextxy(WindowStuUploadProjectNoticeL, WindowStuUploadProjectNoticeU, WindowStuUploadProjectStagenext);
	WindowStuUploadProjectNoticeU = 185;

	//查询我的题目
	flag_Process = 0;
	char sql_WindowStuMyProjectP1[MAXLEN] = { "SELECT PROJECT.PNO,PROJECT.PNAME,PROJECT.PMAJOR,PROJECT.TNO,PROJECT.PABSTRUCT,GRADUATION.PROCESS FROM PROJECT,GRADUATION WHERE((GRADUATION.SNO=='" };
	char sql_WindowStuMyProjectP2[MAXLEN] = { "') AND (GRADUATION.PNO==PROJECT.PNO));" };
	strcat(sql_WindowStuMyProjectP1, CurrentID);
	strcat(sql_WindowStuMyProjectP1, sql_WindowStuMyProjectP2);
	const char* sql_WindowStuMyProject = sql_WindowStuMyProjectP1;
	LogPrint(DEBUG, (char*)sql_WindowStuMyProject);
	exec = sqlite3_exec(DB, sql_WindowStuMyProject, sql_CallBack_Func_InitItemNum, 0, &errmsg);
	if (exec != SQLITE_OK)//SQLITE_ABORT
	{
		//如果出错，就会回到这里来	
		char Msg_sqliteFailedToUploadProject[] = { "FAILED TO INIT Prj ItemNum" };
		LogPrint(ERROR, Msg_sqliteFailedToUploadProject);
		LogPrint(ERROR, errmsg);
	}
	else//SQLITE_OK
	{
		char Msg_sqliteSucceedToUploadProject[] = { "SUCCEED IN INIT Prj ItemNum" };
		LogPrint(INFO, Msg_sqliteSucceedToUploadProject);

		ItemTempCount = 0;
		ItemRestri = 1;
		ItemPage = (ItemNum / ItemRestri) + 1;

		ItemCurrentPage = 1;
		Start = (ItemCurrentPage - 1)*ItemRestri + 1;
		End = (ItemCurrentPage)*ItemRestri;

		exec = sqlite3_exec(DB, sql_WindowStuMyProject, sql_CallBack_Func_Stu_UploadProject, 0, &errmsg);
	}
	//查询进度
	setfillcolor(YELLOW);
	if (flag_Process == 0)
	{
		outtextxy(WindowStuUploadProjectNoticeL, WindowStuUploadProjectNoticeU, WindowStuUploadProjectStage0now);
		WindowStuUploadProjectNoticeU += WindowStuUploadProjectNoticeY;
		outtextxy(WindowStuUploadProjectNoticeL, WindowStuUploadProjectNoticeU, WindowStuUploadProjectStage0next);
	}
	if (flag_Process == 1)
	{
		outtextxy(WindowStuUploadProjectNoticeL, WindowStuUploadProjectNoticeU, WindowStuUploadProjectStage1now);
		WindowStuUploadProjectNoticeU += WindowStuUploadProjectNoticeY;
		outtextxy(WindowStuUploadProjectNoticeL, WindowStuUploadProjectNoticeU, WindowStuUploadProjectStage1next);
		solidrectangle(WindowStuUploadProjectStage1L + 1, WindowStuUploadProjectStage1U + 1, WindowStuUploadProjectStage1R - 1, WindowStuUploadProjectStage1D - 1);
	}
	if (flag_Process == 2)
	{
		outtextxy(WindowStuUploadProjectNoticeL, WindowStuUploadProjectNoticeU, WindowStuUploadProjectStage2now);
		WindowStuUploadProjectNoticeU += WindowStuUploadProjectNoticeY;
		outtextxy(WindowStuUploadProjectNoticeL, WindowStuUploadProjectNoticeU, WindowStuUploadProjectStage2next);
		solidrectangle(WindowStuUploadProjectStage1L + 1, WindowStuUploadProjectStage1U + 1, WindowStuUploadProjectStage1R - 1, WindowStuUploadProjectStage1D - 1);
		solidrectangle(WindowStuUploadProjectStage2L + 1, WindowStuUploadProjectStage2U + 1, WindowStuUploadProjectStage2R - 1, WindowStuUploadProjectStage2D - 1);
	}
	if (flag_Process == 3)
	{
		outtextxy(WindowStuUploadProjectNoticeL, WindowStuUploadProjectNoticeU, WindowStuUploadProjectStage3now);
		WindowStuUploadProjectNoticeU += WindowStuUploadProjectNoticeY;
		outtextxy(WindowStuUploadProjectNoticeL, WindowStuUploadProjectNoticeU, WindowStuUploadProjectStage3next);
		solidrectangle(WindowStuUploadProjectStage1L + 1, WindowStuUploadProjectStage1U + 1, WindowStuUploadProjectStage1R - 1, WindowStuUploadProjectStage1D - 1);
		solidrectangle(WindowStuUploadProjectStage2L + 1, WindowStuUploadProjectStage2U + 1, WindowStuUploadProjectStage2R - 1, WindowStuUploadProjectStage2D - 1);
		solidrectangle(WindowStuUploadProjectStage3L + 1, WindowStuUploadProjectStage3U + 1, WindowStuUploadProjectStage3R - 1, WindowStuUploadProjectStage3D - 1);
	}
	if (flag_Process == 4)
	{
		outtextxy(WindowStuUploadProjectNoticeL, WindowStuUploadProjectNoticeU, WindowStuUploadProjectStage4now);
		WindowStuUploadProjectNoticeU += WindowStuUploadProjectNoticeY;
		outtextxy(WindowStuUploadProjectNoticeL, WindowStuUploadProjectNoticeU, WindowStuUploadProjectStage4next);
		solidrectangle(WindowStuUploadProjectStage1L + 1, WindowStuUploadProjectStage1U + 1, WindowStuUploadProjectStage1R - 1, WindowStuUploadProjectStage1D - 1);
		solidrectangle(WindowStuUploadProjectStage2L + 1, WindowStuUploadProjectStage2U + 1, WindowStuUploadProjectStage2R - 1, WindowStuUploadProjectStage2D - 1);
		solidrectangle(WindowStuUploadProjectStage3L + 1, WindowStuUploadProjectStage3U + 1, WindowStuUploadProjectStage3R - 1, WindowStuUploadProjectStage3D - 1);
		solidrectangle(WindowStuUploadProjectStage4L + 1, WindowStuUploadProjectStage4U + 1, WindowStuUploadProjectStage4R - 1, WindowStuUploadProjectStage4D - 1);
		setfillcolor(BROWN);
		solidrectangle(WindowStuUploadProjectSubmitL + 1, WindowStuUploadProjectSubmitU + 1, WindowStuUploadProjectSubmitR - 1, WindowStuUploadProjectSubmitD - 1);
		outtextxy(WindowStuUploadProjectSubmitL + 45, WindowStuUploadProjectSubmitU + 15, WindowStuUploadProjectReply);
	}
	if (flag_Process == 5)
	{
		outtextxy(WindowStuUploadProjectNoticeL, WindowStuUploadProjectNoticeU, WindowStuUploadProjectStage5now);
		WindowStuUploadProjectNoticeU += WindowStuUploadProjectNoticeY;
		outtextxy(WindowStuUploadProjectNoticeL, WindowStuUploadProjectNoticeU, WindowStuUploadProjectStage5next);
		solidrectangle(WindowStuUploadProjectStage1L + 1, WindowStuUploadProjectStage1U + 1, WindowStuUploadProjectStage1R - 1, WindowStuUploadProjectStage1D - 1);
		solidrectangle(WindowStuUploadProjectStage2L + 1, WindowStuUploadProjectStage2U + 1, WindowStuUploadProjectStage2R - 1, WindowStuUploadProjectStage2D - 1);
		solidrectangle(WindowStuUploadProjectStage3L + 1, WindowStuUploadProjectStage3U + 1, WindowStuUploadProjectStage3R - 1, WindowStuUploadProjectStage3D - 1);
		solidrectangle(WindowStuUploadProjectStage4L + 1, WindowStuUploadProjectStage4U + 1, WindowStuUploadProjectStage4R - 1, WindowStuUploadProjectStage4D - 1);
		solidrectangle(WindowStuUploadProjectStage5L + 1, WindowStuUploadProjectStage5U + 1, WindowStuUploadProjectStage5R - 1, WindowStuUploadProjectStage5D - 1);
		setfillcolor(BROWN);
		solidrectangle(WindowStuUploadProjectSubmitL + 1, WindowStuUploadProjectSubmitU + 1, WindowStuUploadProjectSubmitR - 1, WindowStuUploadProjectSubmitD - 1);
		outtextxy(WindowStuUploadProjectSubmitL + 52, WindowStuUploadProjectSubmitU + 15, WindowStuUploadProjectGreat);
	}

	MOUSEMSG mouseStuUploadProject;
	while (true)
	{
		mouseStuUploadProject = GetMouseMsg();
		switch (mouseStuUploadProject.uMsg)
		{
		case WM_LBUTTONDOWN:
		{
			if (mouseStuUploadProject.x >= WindowStuUploadProjectBackL && mouseStuUploadProject.x <= WindowStuUploadProjectBackR && mouseStuUploadProject.y >= WindowStuUploadProjectBackU && mouseStuUploadProject.y <= WindowStuUploadProjectBackD)
			{//BackButton
				char Msg_StuUploadProjectGoAdmin[] = { "request a StuUploadProjectGoAdmin" };
				LogPrint(INFO, Msg_StuUploadProjectGoAdmin);
				WindowStuMain();
			}
			else if (mouseStuUploadProject.x >= WindowStuUploadProjectSubmitL && mouseStuUploadProject.x <= WindowStuUploadProjectSubmitR && mouseStuUploadProject.y >= WindowStuUploadProjectSubmitU && mouseStuUploadProject.y <= WindowStuUploadProjectSubmitD)
			{//Msg_WindowStuUploadProjectSubmit
				char Msg_WindowStuUploadProjectSubmit[] = { "Msg_WindowStuUploadProjectSubmit-提交本环节进度" };
				LogPrint(INFO, Msg_WindowStuUploadProjectSubmit);
				Sleep(550);
				WindowStuMain();
			}
		}
		default:
			break;
		}
	}

	return 0;
}

int WindowChairmanReviewUploadProject(void)
{
	cleardevice();
	FlushMouseMsgBuffer();
	setcliprgn(NULL);
	setpolyfillmode(WINDING);
	setfillcolor(BROWN);
	solidrectangle(0, 0, W, H);
	Sleep(350);

	setbkmode(TRANSPARENT);
	char Msg_initWindowChairmanReviewUploadProject[] = { "initWindowChairmanReviewUploadProject" };
	LogPrint(INFO, Msg_initWindowChairmanReviewUploadProject);

	TCHAR WindowChairmanReviewUploadProjectSlogan[] = { TEXT("主任，同学的论文进展，您通过吗？") };
	outtextxy(285, 20, WindowChairmanReviewUploadProjectSlogan);

	TCHAR WindowChairmanReviewUploadProjectPageUp[] = { TEXT("上一页") };
	TCHAR WindowChairmanReviewUploadProjectPageDown[] = { TEXT("下一页") };
	TCHAR WindowChairmanReviewUploadProjectMid[] = { TEXT("主任审核通过") };
	TCHAR WindowChairmanReviewUploadProjectAdvice[] = { TEXT("毕业答辩通过") };

	//概览窗口
	int WindowChairmanReviewUploadProjectViewAllL = 50;
	int WindowChairmanReviewUploadProjectViewAllU = 100;
	int WindowChairmanReviewUploadProjectViewAllR = WindowChairmanReviewUploadProjectViewAllL + 460;
	int WindowChairmanReviewUploadProjectViewAllD = WindowChairmanReviewUploadProjectViewAllU + 300;
	rectangle(WindowChairmanReviewUploadProjectViewAllL, WindowChairmanReviewUploadProjectViewAllU, WindowChairmanReviewUploadProjectViewAllR, WindowChairmanReviewUploadProjectViewAllD);

	//上一页按钮
	int WindowChairmanReviewUploadProjectPageUpL = 550;
	int WindowChairmanReviewUploadProjectPageUpU = 115;
	int WindowChairmanReviewUploadProjectPageUpR = WindowChairmanReviewUploadProjectPageUpL + 65;
	int WindowChairmanReviewUploadProjectPageUpD = WindowChairmanReviewUploadProjectPageUpU + 95;
	rectangle(WindowChairmanReviewUploadProjectPageUpL, WindowChairmanReviewUploadProjectPageUpU, WindowChairmanReviewUploadProjectPageUpR, WindowChairmanReviewUploadProjectPageUpD);

	//下一页按钮
	int WindowChairmanReviewUploadProjectPageDownL = 685;
	int WindowChairmanReviewUploadProjectPageDownU = 115;
	int WindowChairmanReviewUploadProjectPageDownR = WindowChairmanReviewUploadProjectPageDownL + 65;
	int WindowChairmanReviewUploadProjectPageDownD = WindowChairmanReviewUploadProjectPageDownU + 95;
	rectangle(WindowChairmanReviewUploadProjectPageDownL, WindowChairmanReviewUploadProjectPageDownU, WindowChairmanReviewUploadProjectPageDownR, WindowChairmanReviewUploadProjectPageDownD);

	//页面选择按钮
	int WindowChairmanReviewUploadProjectPageSelectL = 640;
	int WindowChairmanReviewUploadProjectPageSelectU = 115;
	int WindowChairmanReviewUploadProjectPageSelectR = WindowChairmanReviewUploadProjectPageSelectL + 20;
	int WindowChairmanReviewUploadProjectPageSelectD = WindowChairmanReviewUploadProjectPageSelectU + 95;
	rectangle(WindowChairmanReviewUploadProjectPageSelectL, WindowChairmanReviewUploadProjectPageSelectU, WindowChairmanReviewUploadProjectPageSelectR, WindowChairmanReviewUploadProjectPageSelectD);

	//主任审核通过按钮
	int WindowChairmanReviewUploadProjectMidL = 550;
	int WindowChairmanReviewUploadProjectMidU = 340;
	int WindowChairmanReviewUploadProjectMidR = WindowChairmanReviewUploadProjectMidL + 95;
	int WindowChairmanReviewUploadProjectMidD = WindowChairmanReviewUploadProjectMidU + 45;
	rectangle(WindowChairmanReviewUploadProjectMidL, WindowChairmanReviewUploadProjectMidU, WindowChairmanReviewUploadProjectMidR, WindowChairmanReviewUploadProjectMidD);

	//毕业答辩通过按钮
	int WindowChairmanReviewUploadProjectAdviceL = 655;
	int WindowChairmanReviewUploadProjectAdviceU = 340;
	int WindowChairmanReviewUploadProjectAdviceR = WindowChairmanReviewUploadProjectAdviceL + 95;
	int WindowChairmanReviewUploadProjectAdviceD = WindowChairmanReviewUploadProjectAdviceU + 45;
	rectangle(WindowChairmanReviewUploadProjectAdviceL, WindowChairmanReviewUploadProjectAdviceU, WindowChairmanReviewUploadProjectAdviceR, WindowChairmanReviewUploadProjectAdviceD);

	//返回按钮
	int WindowChairmanReviewUploadProjectBackL = 35;
	int WindowChairmanReviewUploadProjectBackU = 20;
	int WindowChairmanReviewUploadProjectBackR = WindowChairmanReviewUploadProjectBackL + 75;
	int WindowChairmanReviewUploadProjectBackD = WindowChairmanReviewUploadProjectBackU + 25;
	rectangle(WindowChairmanReviewUploadProjectBackL, WindowChairmanReviewUploadProjectBackU, WindowChairmanReviewUploadProjectBackR, WindowChairmanReviewUploadProjectBackD);
	TCHAR WindowAdminBackSelectProjectButton[] = { TEXT("返回") };
	//返回图标
	outtextxy(WindowChairmanReviewUploadProjectBackL + 35, WindowChairmanReviewUploadProjectBackU + 4, WindowAdminBackSelectProjectButton);
	setfillcolor(YELLOW);
	int BackCircleX = WindowChairmanReviewUploadProjectBackL + 5;
	int BackCircleY = WindowChairmanReviewUploadProjectBackU + 5;
	solidcircle(BackCircleX + 7, BackCircleY + 7, 7);
	setfillcolor(BROWN);
	solidrectangle(BackCircleX + 5, BackCircleY + 6, BackCircleX + 12, BackCircleY + 8);
	POINT BackTriangle[] = { {BackCircleX + 1, BackCircleY + 7}, {BackCircleX + 6, BackCircleY + 3}, {BackCircleX + 6, BackCircleY + 11} };
	solidpolygon(BackTriangle, 3);

	setfillcolor(DARKGRAY);
	solidrectangle(WindowChairmanReviewUploadProjectViewAllL + 1, WindowChairmanReviewUploadProjectViewAllU + 1, WindowChairmanReviewUploadProjectViewAllR - 1, WindowChairmanReviewUploadProjectViewAllD - 1);
	setfillcolor(LIGHTGRAY);
	solidrectangle(WindowChairmanReviewUploadProjectPageUpL + 1, WindowChairmanReviewUploadProjectPageUpU + 1, WindowChairmanReviewUploadProjectPageUpR - 1, WindowChairmanReviewUploadProjectPageUpD - 1);
	solidrectangle(WindowChairmanReviewUploadProjectPageDownL + 1, WindowChairmanReviewUploadProjectPageDownU + 1, WindowChairmanReviewUploadProjectPageDownR - 1, WindowChairmanReviewUploadProjectPageDownD - 1);
	solidrectangle(WindowChairmanReviewUploadProjectPageSelectL + 1, WindowChairmanReviewUploadProjectPageSelectU + 1, WindowChairmanReviewUploadProjectPageSelectR - 1, WindowChairmanReviewUploadProjectPageSelectD - 1);
	solidrectangle(WindowChairmanReviewUploadProjectMidL + 1, WindowChairmanReviewUploadProjectMidU + 1, WindowChairmanReviewUploadProjectMidR - 1, WindowChairmanReviewUploadProjectMidD - 1);
	solidrectangle(WindowChairmanReviewUploadProjectAdviceL + 1, WindowChairmanReviewUploadProjectAdviceU + 1, WindowChairmanReviewUploadProjectAdviceR - 1, WindowChairmanReviewUploadProjectAdviceD - 1);

	outtextxy(WindowChairmanReviewUploadProjectPageUpL + 11, WindowChairmanReviewUploadProjectPageUpU + 38, WindowChairmanReviewUploadProjectPageUp);
	outtextxy(WindowChairmanReviewUploadProjectPageDownL + 11, WindowChairmanReviewUploadProjectPageDownU + 38, WindowChairmanReviewUploadProjectPageDown);
	outtextxy(WindowChairmanReviewUploadProjectMidL, WindowChairmanReviewUploadProjectMidU + 15, WindowChairmanReviewUploadProjectMid);
	outtextxy(WindowChairmanReviewUploadProjectAdviceL, WindowChairmanReviewUploadProjectAdviceU + 15, WindowChairmanReviewUploadProjectAdvice);

	TCHAR WindowChairmanReviewUploadProjectPNO[] = { TEXT("题目编号") };
	TCHAR WindowChairmanReviewUploadProjectPNAME[] = { TEXT("题目名称") };
	TCHAR WindowChairmanReviewUploadProjectPMAJOR[] = { TEXT("题目专业") };
	TCHAR WindowChairmanReviewUploadProjectTNO[] = { TEXT("命题导师") };
	TCHAR WindowChairmanReviewUploadProjectPABSTRUCT[] = { TEXT("内容简介") };
	TCHAR WindowChairmanReviewUploadProjectSNO[] = { TEXT("选题学生") };
	int WindowChairmanReviewUploadProjectInfoColumnL = 75;
	int WindowChairmanReviewUploadProjectInfoColumnU = 120;
	int WindowChairmanReviewUploadProjectInfoColumnX = 0;
	int WindowChairmanReviewUploadProjectInfoColumnY = 30;

	//题目编号
	int WindowChairmanReviewUploadProjectPNOL = WindowChairmanReviewUploadProjectInfoColumnL + WindowChairmanReviewUploadProjectInfoColumnX;
	int WindowChairmanReviewUploadProjectPNOU = WindowChairmanReviewUploadProjectInfoColumnU;
	outtextxy(WindowChairmanReviewUploadProjectInfoColumnL, WindowChairmanReviewUploadProjectInfoColumnU, WindowChairmanReviewUploadProjectPNO);
	WindowChairmanReviewUploadProjectInfoColumnU += WindowChairmanReviewUploadProjectInfoColumnY;
	//题目名称
	int WindowChairmanReviewUploadProjectPNAMEL = WindowChairmanReviewUploadProjectInfoColumnL + WindowChairmanReviewUploadProjectInfoColumnX;
	int WindowChairmanReviewUploadProjectPNAMEU = WindowChairmanReviewUploadProjectInfoColumnU;
	outtextxy(WindowChairmanReviewUploadProjectInfoColumnL, WindowChairmanReviewUploadProjectInfoColumnU, WindowChairmanReviewUploadProjectPNAME);
	WindowChairmanReviewUploadProjectInfoColumnU += WindowChairmanReviewUploadProjectInfoColumnY;
	//题目专业
	int WindowChairmanReviewUploadProjectPMAJORL = WindowChairmanReviewUploadProjectInfoColumnL + WindowChairmanReviewUploadProjectInfoColumnX;
	int WindowChairmanReviewUploadProjectPMAJORU = WindowChairmanReviewUploadProjectInfoColumnU;
	outtextxy(WindowChairmanReviewUploadProjectInfoColumnL, WindowChairmanReviewUploadProjectInfoColumnU, WindowChairmanReviewUploadProjectPMAJOR);
	WindowChairmanReviewUploadProjectInfoColumnU += WindowChairmanReviewUploadProjectInfoColumnY;
	//命题导师
	int WindowChairmanReviewUploadProjectTNOL = WindowChairmanReviewUploadProjectInfoColumnL + WindowChairmanReviewUploadProjectInfoColumnX;
	int WindowChairmanReviewUploadProjectTNOU = WindowChairmanReviewUploadProjectInfoColumnU;
	outtextxy(WindowChairmanReviewUploadProjectInfoColumnL, WindowChairmanReviewUploadProjectInfoColumnU, WindowChairmanReviewUploadProjectTNO);
	WindowChairmanReviewUploadProjectInfoColumnU += WindowChairmanReviewUploadProjectInfoColumnY;
	//内容简介
	int WindowChairmanReviewUploadProjectPABSTRUCTL = WindowChairmanReviewUploadProjectInfoColumnL + WindowChairmanReviewUploadProjectInfoColumnX;
	int WindowChairmanReviewUploadProjectPABSTRUCTU = WindowChairmanReviewUploadProjectInfoColumnU;
	outtextxy(WindowChairmanReviewUploadProjectInfoColumnL, WindowChairmanReviewUploadProjectInfoColumnU, WindowChairmanReviewUploadProjectPABSTRUCT);
	WindowChairmanReviewUploadProjectInfoColumnU += WindowChairmanReviewUploadProjectInfoColumnY;
	//选题学生
	int WindowChairmanReviewUploadProjectSNOL = WindowChairmanReviewUploadProjectInfoColumnL + WindowChairmanReviewUploadProjectInfoColumnX;
	int WindowChairmanReviewUploadProjectSNOU = WindowChairmanReviewUploadProjectInfoColumnU;
	outtextxy(WindowChairmanReviewUploadProjectInfoColumnL, WindowChairmanReviewUploadProjectInfoColumnU, WindowChairmanReviewUploadProjectSNO);
	WindowChairmanReviewUploadProjectInfoColumnU = 120;

	//查询所有题库里的题目
	char sql_WindowAdminAllSelectProject[MAXLEN] = { "SELECT PROJECT.PNO,PROJECT.PNAME,PROJECT.PMAJOR,PROJECT.TNO,PROJECT.PABSTRUCT,GRADUATION.SNO  FROM PROJECT,GRADUATION WHERE((GRADUATION.PNO==PROJECT.PNO) AND (GRADUATION.PROCESS==3 OR GRADUATION.PROCESS==4));" };
	LogPrint(DEBUG, (char*)sql_WindowAdminAllSelectProject);
	exec = sqlite3_exec(DB, sql_WindowAdminAllSelectProject, sql_CallBack_Func_InitItemNum, 0, &errmsg);
	if (exec != SQLITE_OK)//SQLITE_ABORT
	{
		//如果出错，就会回到这里来	
		char Msg_sqliteFailedToSelectProject[] = { "FAILED TO INIT Prj ItemNum" };
		LogPrint(ERROR, Msg_sqliteFailedToSelectProject);
		LogPrint(ERROR, errmsg);
	}
	else//SQLITE_OK
	{
		char Msg_sqliteSucceedToSelectProject[] = { "SUCCEED IN INIT Prj ItemNum" };
		LogPrint(INFO, Msg_sqliteSucceedToSelectProject);

		ItemTempCount = 0;
		ItemRestri = 1;
		ItemPage = (ItemNum / ItemRestri) + 1;

		ItemCurrentPage = 1;
		Start = (ItemCurrentPage - 1)*ItemRestri + 1;
		End = (ItemCurrentPage)*ItemRestri;

		exec = sqlite3_exec(DB, sql_WindowAdminAllSelectProject, sql_CallBack_Func_Chairman_ReviewProject, 0, &errmsg);
	}

	MOUSEMSG mouseChairmanReviewUploadProject;
	while (true)
	{
		mouseChairmanReviewUploadProject = GetMouseMsg();
		switch (mouseChairmanReviewUploadProject.uMsg)
		{
		case WM_LBUTTONDOWN:
		{
			if (mouseChairmanReviewUploadProject.x >= WindowChairmanReviewUploadProjectBackL && mouseChairmanReviewUploadProject.x <= WindowChairmanReviewUploadProjectBackR && mouseChairmanReviewUploadProject.y >= WindowChairmanReviewUploadProjectBackU && mouseChairmanReviewUploadProject.y <= WindowChairmanReviewUploadProjectBackD)
			{//BackButton
				char Msg_ChairmanReviewUploadProjectGoAdmin[] = { "request a ChairmanReviewUploadProjectGoAdmin" };
				LogPrint(INFO, Msg_ChairmanReviewUploadProjectGoAdmin);
				WindowChairmanMain();
			}
			else if (mouseChairmanReviewUploadProject.x >= WindowChairmanReviewUploadProjectPageUpL && mouseChairmanReviewUploadProject.x <= WindowChairmanReviewUploadProjectPageUpR && mouseChairmanReviewUploadProject.y >= WindowChairmanReviewUploadProjectPageUpU && mouseChairmanReviewUploadProject.y <= WindowChairmanReviewUploadProjectPageUpD)
			{//Msg_WindowChairmanReviewUploadProjectPageUp
				char Msg_WindowChairmanReviewUploadProjectPageUp[] = { "Msg_WindowChairmanReviewUploadProjectPageUp-上一页" };
				LogPrint(INFO, Msg_WindowChairmanReviewUploadProjectPageUp);
				if (ItemCurrentPage - 1 >= 1)
				{
					setfillcolor(DARKGRAY);
					solidrectangle(WindowChairmanReviewUploadProjectInfoColumnL + 75, WindowChairmanReviewUploadProjectViewAllU + 1, WindowChairmanReviewUploadProjectViewAllR - 1, WindowChairmanReviewUploadProjectViewAllD - 1);
					ItemTempCount = 0;
					ItemCurrentPage -= 1;
					Start = (ItemCurrentPage - 1)*ItemRestri + 1;
					End = (ItemCurrentPage)*ItemRestri;
					exec = sqlite3_exec(DB, sql_WindowAdminAllSelectProject, sql_CallBack_Func_Chairman_ReviewProject, 0, &errmsg);
				}
			}
			else if (mouseChairmanReviewUploadProject.x >= WindowChairmanReviewUploadProjectPageDownL && mouseChairmanReviewUploadProject.x <= WindowChairmanReviewUploadProjectPageDownR && mouseChairmanReviewUploadProject.y >= WindowChairmanReviewUploadProjectPageDownU && mouseChairmanReviewUploadProject.y <= WindowChairmanReviewUploadProjectPageDownD)
			{//Msg_WindowChairmanReviewUploadProjectPageDown
				char Msg_WindowChairmanReviewUploadProjectPageDown[] = { "Msg_WindowChairmanReviewUploadProjectPageDown-下一页" };
				LogPrint(INFO, Msg_WindowChairmanReviewUploadProjectPageDown);
				if (ItemCurrentPage + 1 <= ItemPage)
				{
					setfillcolor(DARKGRAY);
					solidrectangle(WindowChairmanReviewUploadProjectInfoColumnL + 75, WindowChairmanReviewUploadProjectViewAllU + 1, WindowChairmanReviewUploadProjectViewAllR - 1, WindowChairmanReviewUploadProjectViewAllD - 1);
					ItemTempCount = 0;
					ItemCurrentPage += 1;
					Start = (ItemCurrentPage - 1)*ItemRestri + 1;
					End = (ItemCurrentPage)*ItemRestri;
					exec = sqlite3_exec(DB, sql_WindowAdminAllSelectProject, sql_CallBack_Func_Chairman_ReviewProject, 0, &errmsg);
				}
			}
			else if (mouseChairmanReviewUploadProject.x >= WindowChairmanReviewUploadProjectPageSelectL && mouseChairmanReviewUploadProject.x <= WindowChairmanReviewUploadProjectPageSelectR && mouseChairmanReviewUploadProject.y >= WindowChairmanReviewUploadProjectPageSelectU && mouseChairmanReviewUploadProject.y <= WindowChairmanReviewUploadProjectPageSelectD)
			{//Msg_WindowChairmanReviewUploadProjectPageSelect
				//是废的
				char Msg_WindowChairmanReviewUploadProjectPageSelect[] = { "Msg_WindowChairmanReviewUploadProjectPageSelect-选择页面" };
				LogPrint(INFO, Msg_WindowChairmanReviewUploadProjectPageSelect);
				//InputBox(PageNum_TCHAR, 3 + 1, TEXT("请输入想要跳转到的页面(不超过3个字符)"));
				//outtextxy(WindowChairmanReviewUploadProjectPageSelectL + 6, WindowChairmanReviewUploadProjectPageSelectU + 40, PageNum_TCHAR);
				//TcharToChar(PageNum_TCHAR, PageNum_CHAR);
				//LogPrint(DEBUG, PageNum_CHAR);
			}
			else if (mouseChairmanReviewUploadProject.x >= WindowChairmanReviewUploadProjectMidL && mouseChairmanReviewUploadProject.x <= WindowChairmanReviewUploadProjectMidR && mouseChairmanReviewUploadProject.y >= WindowChairmanReviewUploadProjectMidU && mouseChairmanReviewUploadProject.y <= WindowChairmanReviewUploadProjectMidD)
			{//Msg_WindowChairmanReviewUploadProjectSubmit
				char Msg_WindowChairmanReviewUploadProjectMid[] = { "Msg_WindowChairmanReviewUploadProjectMid-主任审核通过" };
				LogPrint(INFO, Msg_WindowChairmanReviewUploadProjectMid);
				//CurrentItemExtern-TNO
				//CurrentItem-PNO
				char sql_WindowNewStuInsertGraduationP1[MAXLEN] = { "UPDATE GRADUATION SET PROCESS=4 WHERE(PNO=='" };
				char sql_WindowNewStuInsertGraduationP2[MAXLEN] = { "');" };
				strcat(sql_WindowNewStuInsertGraduationP1, CurrentItem);//PNO
				strcat(sql_WindowNewStuInsertGraduationP1, sql_WindowNewStuInsertGraduationP2);
				const char *sql_WindowNewStuInsertGraduation = sql_WindowNewStuInsertGraduationP1;
				LogPrint(DEBUG, (char*)sql_WindowNewStuInsertGraduation);
				exec = sqlite3_exec(DB, sql_WindowNewStuInsertGraduation, sql_CallBack_Func_NewSubmit, 0, &errmsg);
				if (exec != SQLITE_OK)//SQLITE_ABORT
				{
					//如果出错，就会回到这里来	
					char Msg_sqliteFailedToApproveProject[] = { "FAILED TO APPROVE PROJECT" };
					LogPrint(ERROR, Msg_sqliteFailedToApproveProject);
					LogPrint(ERROR, errmsg);
				}
				else//SQLITE_OK
				{
					char Msg_sqliteSucceedInApproveProject[] = { "SUCCEED IN APPROVE PROJECT" };
					LogPrint(INFO, Msg_sqliteSucceedInApproveProject);
					TCHAR ApproveProjectSuccess[] = { TEXT("主任已通过") };
					Sleep(250);
					outtextxy(WindowChairmanReviewUploadProjectMidL, WindowChairmanReviewUploadProjectMidU + 55, ApproveProjectSuccess);
				}
				Sleep(550);
				WindowChairmanMain();
			}
			else if (mouseChairmanReviewUploadProject.x >= WindowChairmanReviewUploadProjectAdviceL && mouseChairmanReviewUploadProject.x <= WindowChairmanReviewUploadProjectAdviceR && mouseChairmanReviewUploadProject.y >= WindowChairmanReviewUploadProjectAdviceU && mouseChairmanReviewUploadProject.y <= WindowChairmanReviewUploadProjectAdviceD)
			{//Msg_WindowChairmanReviewUploadProjectSubmit
				char Msg_WindowChairmanReviewUploadProjectMid[] = { "Msg_WindowChairmanReviewUploadProjectMid-毕业答辩通过" };
				LogPrint(INFO, Msg_WindowChairmanReviewUploadProjectMid);
				//CurrentItemExtern-TNO
				//CurrentItem-PNO
				char sql_WindowNewStuInsertGraduationP1[MAXLEN] = { "UPDATE GRADUATION SET PROCESS=5 WHERE(PNO=='" };
				char sql_WindowNewStuInsertGraduationP2[MAXLEN] = { "');" };
				strcat(sql_WindowNewStuInsertGraduationP1, CurrentItem);//PNO
				strcat(sql_WindowNewStuInsertGraduationP1, sql_WindowNewStuInsertGraduationP2);
				const char *sql_WindowNewStuInsertGraduation = sql_WindowNewStuInsertGraduationP1;
				LogPrint(DEBUG, (char*)sql_WindowNewStuInsertGraduation);
				exec = sqlite3_exec(DB, sql_WindowNewStuInsertGraduation, sql_CallBack_Func_NewSubmit, 0, &errmsg);
				if (exec != SQLITE_OK)//SQLITE_ABORT
				{
					//如果出错，就会回到这里来	
					char Msg_sqliteFailedToApproveProject[] = { "FAILED TO APPROVE PROJECT" };
					LogPrint(ERROR, Msg_sqliteFailedToApproveProject);
					LogPrint(ERROR, errmsg);
				}
				else//SQLITE_OK
				{
					char Msg_sqliteSucceedInApproveProject[] = { "SUCCEED IN APPROVE PROJECT" };
					LogPrint(INFO, Msg_sqliteSucceedInApproveProject);
					TCHAR ApproveProjectSuccess[] = { TEXT("答辩已通过") };
					Sleep(250);
					outtextxy(WindowChairmanReviewUploadProjectAdviceL, WindowChairmanReviewUploadProjectMidU + 55, ApproveProjectSuccess);
				}
				Sleep(550);
				WindowChairmanMain();
			}
		}
		default:
			break;
		}
	}

	return 0;
}

int main(int argc,char *argv[])
{
	if (exec == 1)
	{
		char Msg_sqliteIsNotOpen[] = { "Sqlite is not open!" };
		LogPrint(ERROR, Msg_sqliteIsNotOpen);
		LogPrint(ERROR, errmsg);
	}
	else
	{
		char Msg_sqliteIsOpen[] = { "Sqlite is Open!" };
		LogPrint(INFO, Msg_sqliteIsOpen);
	}
	const char *sql_header = ".header on";
	exec = sqlite3_exec(DB, sql_header, sql_CallBack_Func, 0, &errmsg);
	const char *sql_mode = ".mode csv";
	exec = sqlite3_exec(DB, sql_mode, sql_CallBack_Func, 0, &errmsg);
	//graduation_128px_1271452_easyicon.net.ico
	HWND HWND_Title = initgraph(W, H, SHOWCONSOLE);
	SetWindowTextA(HWND_Title, "毕业设计管理系统");
	char Msg_init[] = { "initgraph" };
	LogPrint(INFO, Msg_init);
	WindowLogin();
	system("pause");
	return 0;
}
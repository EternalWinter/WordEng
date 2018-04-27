#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "includes/term.h"
#include "includes/logo.h"

#define RESET "\033[0m"
#define GREEN "\033[1;32m"
#define RED   "\033[1;31m"
#define CYAN  "\033[1;36m"
#define GRAY  "\033[2m"
#define WHITE  "\033[37m"
#define BLACK "\033[0;30m" 
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"
#define NUNDERLINE "\033[4m"

#define gotoxy(x,y)		printf(ESC "[%d;%dH", y, x);
#define resetcolor() printf(ESC "[0m")
#define set_display_atrib(color) 	printf(ESC "[%dm",color); // цвет фона
    
    
    // https://habrahabr.ru/post/119436/

typedef struct {
    int index;  // индекс нужного слова
    char *english; // само слово
    char *russian; // переводы
} database; // структура данных о слове

typedef struct {
    char *name; // имя 
    int level; // уровень 1-3
    int words; // количество слов 1-4
    int fail; // количество ошибок за весь период
    int *index; // индексы выученных слов
} profile_data; // структура профиля

int max_index; // количество строк в базе слов
int max_learn;


void delay(int msec) {
    float milli_seconds = msec; 
    float start_time = clock();
    while (clock() <= start_time + milli_seconds);
}

int guestion_console(); // выбор интерфейса
//если нет файла профиля
int hello_console();
int name_console(char *name);
int level_console(int level);
int words_console(int words);
// если есть
int learn_console(database, profile_data, int max_index, int max_learn);
int retry_console(database, profile_data, int max_index, int max_learn);
//всплывающие окна
int profile_console(database, profile_data, int max_index, int max_learn, int form);
void about_console();

void logo();
int alignment(char* slovo, int pol);
void frame(int dlina, int shirina);

int main () {
    database main_data;
    main_data.index = 1;
    main_data.english = "Rabbit";
    main_data.russian = "Заяц, кролик";

    profile_data main_profile;
    main_profile.name = "Alex";
    main_profile.level = 1;
    main_profile.words = 4;
    main_profile.fail = 66;
    max_index = 6665;
    max_learn = 65;

  /* guestion_console();
   delay(900000);
   hello_console();
   delay(900000);
   name_console(main_profile.name);
   delay(900000);
   level_console(main_profile.level);
   delay(900000);

    words_console(main_profile.words);
    learn_console(main_data, main_profile, max_index, max_learn);
    delay(1000000);
    retry_console(main_data, main_profile, max_index, max_learn);

*/
  /*  about_console();
    delay(9000000);
    profile_console(main_data, main_profile, max_index, max_learn);
    return 0;*/
    learn_console(main_data, main_profile, max_index, max_learn);
}

int guestion_console() {
    int xx = 54;
    int yy = 14;
    char ansv[0];
    system("clear");
    frame(yy,xx);
    gotoxy(alignment("Выберите интерфейс", xx/2),6);
    printf("Выберите интерфейс");
    gotoxy(alignment("Консольный(C)   Графический(G)", xx/2),7);
    printf("%sКонсольный(C)   Графический(G)%s", CYAN, RESET);
    gotoxy(alignment("Для консольного интерфейса разверните", xx/2),11);
    printf("%sДля консольного интерфейса разверните",RED);
    gotoxy(alignment("консоль на весь экран!", xx/2),12);
    printf("консоль на весь экран!%s\n", RESET);
    gotoxy(xx/2,9);
    scanf("%c", ansv);
	return 0;
}

int hello_console() {
    int xx = 54;
    int yy = 32;
    system("clear");
    frame(yy,xx);
    logo();
    gotoxy(alignment("Добро подаловать!", xx/2),28);
    printf("Добро подаловать!");
    gotoxy(alignment("Для начала вы должны заполнить профиль:)", xx/2),29);
    printf("%sДля начала вы должны заполнить профиль:)%s\n\n", GRAY, RESET);
    gotoxy(alignment("Начать!(N)", xx/2),30);
    printf("%sНачать!(N)%s\n", RED, RESET);
	return 0;
}

int name_console(char *name) {
    int xx = 54;
    int yy = 32;
    system("clear");
    frame(yy,xx);
    gotoxy(alignment("Как вас зовут?", xx/2),14);
    printf("%sКак вас зовут?", CYAN);
    gotoxy(17,15);
    name = (char*)malloc(100);
    gotoxy(alignment("Далее(N)", xx/2),24);
    printf("%sДалее(N)%s\n\n", RED, RESET);
    gotoxy(18,16);
    printf("%s", CYAN);
    scanf("%s", name);
    name = realloc (name, strlen(name));
    printf("%s", RESET);
	return 0;
}

int level_console(int level) {
    int xx = 54;
    int yy = 32;
    system("clear");
    frame(yy,xx);
    char leveli;
    gotoxy(alignment("Каковы твои знания английского языка?",xx/2),6);
    printf("%sКаковы твои знания английского языка?%s", WHITE, RESET);
    gotoxy(alignment("Низкий(E)",xx/2),8);
    printf("%sНизкий(E)%s", WHITE, RESET);
    gotoxy(alignment("Средний(N)",xx/2),10);
    printf("%sСредний(N)%s", WHITE, RESET);
    gotoxy(alignment("Высокий(H)",xx/2),12);
    printf("%sВысокий(H)%s", WHITE, RESET);
    gotoxy(xx/2,14);
    scanf("%c", &leveli);
    if (leveli == 'E' ){
        //level = 1;
    }
    if (leveli == 'N') {
        //level = 2;
    }
    if (leveli == 'H') {
        //level = 3;
    }
	return 0;
}

int words_console(int words) {
    int xx = 54;
    int yy = 32;
    system("clear");
    frame(yy,xx);
    int leveli;
    gotoxy(alignment("Сколько слов вы хотите учить за раз?", xx/2),15);
    printf("%sСколько слов вы хотите учить за раз?%s", WHITE, RESET);
    gotoxy(2,17);
    printf("\t\t1\t2\t3\t4");
    gotoxy(26,18);
    scanf("%d", &leveli);
	return 0;
}

int learn_console(database data, profile_data profile, int max_index, int max_learn) {
    int xx = 54;
    int yy = 32;
    int form = 1;
    char next[1];
    system("clear");
    frame(yy,xx);
    gotoxy(2,5);
    printf(" Учить(L) Повторение(R) Профиль(P) Справка(A)");
    gotoxy(2,7);
    printf("%s Слов: %d\t\t\t\t%d/%d слов", GRAY, max_index, 1, profile.words);
    gotoxy(2,8);
    printf(" Выученных: %d", max_learn);
    gotoxy(2,9);
    printf(" Ошибок: %d%s", profile.fail, RESET);
    gotoxy(alignment(data.english, xx/2),11);
    //центрировать по первому принту - выполнено
    printf("%s%s%s%s", UNDERLINE, CYAN, data.english, RESET);
    gotoxy(alignment(data.russian, xx/2),14);
    printf("%s", data.russian);
    gotoxy(alignment("Далее(N)", xx/2),20);
    printf("%sДалее(N)%s", RED, RESET);
    scanf("%s", next);
    gotoxy(xx/2,22);
}

int retry_console(database data, profile_data profile, int max_index, int max_learn) {
    int xx = 54;
    int yy = 32;
    system("clear");
    frame(yy,xx);
    gotoxy(2,5);
    printf(" Учить(L) Повторение(R) Профиль(P) Справка(A)");
    gotoxy(2,7);
    printf("%s Слов: %d\t\t\t\t%d/%d слов", GRAY, max_index, 1, profile.words);
    gotoxy(2,8);
    printf(" Выученных: %d", max_learn);
    gotoxy(2,9);
    printf(" Ошибок: %d%s", profile.fail, RESET);
    gotoxy(alignment(data.english,xx/2),12);
    //центрировать по первому принту
    printf("%s%s%s%s", UNDERLINE, CYAN, data.english, RESET);
    gotoxy(12,18);
    printf("Ответ:");

    gotoxy(alignment("НЕВЕРНО!",xx/2),22);
    printf("%sНЕВЕРНО!%s", RED, RESET); /// надо условие

    gotoxy(alignment("Стоп(S)        Далее(N)",xx/2),28);
    printf("%sСтоп(S)        %sДалее(N)%s", RED, GREEN, RESET);
    gotoxy(12,19);
    char *russ =(char*)malloc(100);
    printf("%s",CYAN);
    scanf("%s", russ);
    printf("%s",RESET);
    return 0;
}


int profile_console(database data, profile_data profile, int max_index, int max_learn, int form) {
    int xx = 54;
    int yy = 16;
    int yyk = yy + 10;
    char exit[1];
    gotoxy(xx,yy); 
    printf("┤");
    gotoxy(1,yy); 
    printf("├");
    for (int i = 2; i < xx; i++) {
        for (int j = yy; j < yyk; j++){
             gotoxy(i,j);
             printf(" ");
        }
    }
    for ( int i = 2; i < xx; i++) {
        gotoxy(i,yy); printf("─");
    }
    gotoxy(xx - 8,yy); 
    printf("%s Exit(Q)%s", RED , RESET);\
    yy++;
    gotoxy(alignment("Wordeng", xx/2),yy); 
    printf("%s%sWordeng%s", RED, UNDERLINE, RESET);
    yy++;
    gotoxy(alignment("Учи английский с радостью!", xx/2),yy); 
    printf("%sУчи английский с радостью!%s", GRAY, RESET);
    yy++;
    gotoxy(alignment( profile.name, xx/2),yy);
    printf("%s", profile.name);
    yy = yy + 2;
    gotoxy(alignment( "Выученных:", xx/2),yy);
    printf("Выученных: %d", max_learn);
    yy++;
    gotoxy(alignment( "Ошибок: ", xx/2),yy);
    printf("Ошибок: %d", profile.fail);
    yy++;
    gotoxy(xx,yyk); 
    printf("┤");
    gotoxy(1,yyk); 
    printf("├");
    for ( int i = 2; i < xx; i++) {
        gotoxy(i,yyk); printf("─");
    }
    yy++;
    gotoxy(alignment( "Назад (R)", xx/2),yy);
    printf("Назад (R)");
    yy++;
    gotoxy(alignment( "Назад (R)", xx/2),yy);
    scanf("%s", exit);
}

void about_console() {
    int xx = 54;
    int yy = 18;
    system("clear");
    frame(yy,xx);

    gotoxy(alignment("WordEnd", xx/2),5);
    printf("WordEnd");

    gotoxy(alignment("1.0 Beta", xx/2),7);
    printf("1.0 Beta");

    gotoxy(alignment("Программа для заучивания слов иностранного языка", xx/2),9);
    printf("Программа для заучивания слов иностранного языка");

    gotoxy(alignment("(английского)", xx/2),10);
    printf("(английского)");

    gotoxy(alignment("Курсовая работа студентов Сибирского", xx/2),12);
    printf("Курсовая работа студентов Сибирского");

    gotoxy(alignment("государственного университета телекоммуникации", xx/2),13);
    printf("государственного университета телекоммуникации");

    gotoxy(alignment("и информатики", xx/2),14);
    printf("и информатики");

    gotoxy(alignment("Разработчики", xx/2),17);
    printf("Разработчики");

}



int alignment(char* slovo, int pol) {
    int otstup, polovinaslova;
    polovinaslova = strlen(slovo) / 4 + 1;
    otstup = pol - polovinaslova;
    return otstup;
}

void frame(int dlina, int shirina) {
    gotoxy(1,1);
    printf("┌");
    for ( int i = 2; i < shirina; i++) {
        gotoxy(i,1); printf("─");
    }
    gotoxy(shirina,1);
    printf("┐");
    gotoxy(shirina - 8,1); 
    printf("%s Exit(Q)%s", RED , RESET);\
    gotoxy(alignment("Wordeng", shirina/2),2); 
    printf("%s%sWordeng%s", RED, UNDERLINE, RESET);
    gotoxy(alignment("Учи английский с радостью!", shirina/2),3); 
    printf("%sУчи английский с радостью!%s", GRAY, RESET);
    for (int i = 2; i < dlina; i++) {
        gotoxy(1,i); printf("│");
        gotoxy(shirina,i); printf("│");
    }
    gotoxy(1,4); 
    printf("├");
    for ( int i = 2; i < shirina; i++) {
        gotoxy(i,4); printf("─");
    }
    gotoxy(shirina,4); 
    printf("┤");
    gotoxy(1,dlina);
    printf("└");
    for ( int i = 2; i < shirina; i++) {
        gotoxy(i,dlina); printf("─");
    }
    printf("┘");
}
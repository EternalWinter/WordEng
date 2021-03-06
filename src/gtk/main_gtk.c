#include "../global.h"
#include "headers/gtkwidgets.h"
#include "headers/widget_build.h"
#include "headers/build_window.h"
#include "../body_main.h"
#include "headers/rendering.h"
#include "headers/signals.h"
#include "../correct_start.h"

void main_dialoggtk();
void main_window_profilegtk();
void windowgtk_destroy_cb();
void on_header_destroy();
void menubar_about_activate_cb();
void menubar_profile_activate_cb();
void learn_rand_up_label();
void retry_rand_up_label();
void main_window_massage(int i);
void on_massage_destroy();
void status_text();
int correct_index();

int mass_age = 0;

void main_dialoggtk() {
    dialoggtk = create_dialoggtk();
    gtk_widget_show(dialoggtk);
}

void main_window_profilegtk() {
    profilegtk = create_profilegtk();
    gtk_widget_show(profilegtk);
}

void windowgtk_destroy_cb() {
    gtk_main_quit();
}

void on_header_destroy() {
    if (words) {
        save_profile(1);
    }
    gtk_main_quit();
}

void menubar_about_activate_cb() {
    main_dialoggtk();
}

void menubar_profile_activate_cb() {
    main_window_profilegtk();

    if (level == 1) {
        gtk_label_set_text(GTK_LABEL(profile_level), "Низкий");
    }
    if (level == 2) {
        gtk_label_set_text(GTK_LABEL(profile_level), "Средний");
    }
    if (level == 3) {
        gtk_label_set_text(GTK_LABEL(profile_level), "Высокий");
    }
    char *str = (char*) malloc(sizeof(char) * 100);
    wcstombs(str, name, sizeof(wchar_t) * wcslen(name) + 1);
    gtk_label_set_text(GTK_LABEL(profile_name), str);
    sprintf(str, "%d", fail);
    gtk_label_set_text(GTK_LABEL(profile_fail), str);
    sprintf(str, "%d", max_learn);
    gtk_label_set_text(GTK_LABEL(profile_words), str);
    free(str);
}

void learn_rand_up_label() {
    char *str = (char*) malloc(sizeof(char) * 1000);
    if (learn_rand() == -1) {
        windowgtk_destroy_cb();
    }
    wcstombs(str, english, sizeof(wchar_t) * wcslen(english) + 1);
    gtk_label_set_text(GTK_LABEL(learn_eng), str);
    wcstombs(str, russian, sizeof(wchar_t) * wcslen(russian) + 1);
    gtk_label_set_text(GTK_LABEL(learn_rus), str);
    free(str);
}

void retry_rand_up_label() {
    char *str = (char*) malloc(sizeof(char) * 1000);
    if (retry_rand() == -1) {
        windowgtk_destroy_cb();
    }  
    wcstombs(str, english, sizeof(wchar_t) * wcslen(english) + 1);
    gtk_label_set_text(GTK_LABEL(retry_english), str);
    free(str);
}

void main_window_massage(int i) {
    massage = create_massage();
    switch (i) {
        case 0:
            save_profile(0);
            max_learn = 0;
            mass_age = 0;
            i_words = 0;
            break;
        case 1:
            gtk_label_set_text(GTK_LABEL(levelup), "DONE LEARN ENGLISH!");
            clear_window();
            mass_age = 1;
            break;
    }
    gtk_widget_show(massage);
}

void on_massage_destroy() {
    if (mass_age == 1) {
        remove("./data/profile/profile.txt");
        gtk_main_quit();
    } else {
        menubar_learn_activate_cb();
    }
}

void status_text() {
    char *str = (char*) malloc(100);
    sprintf(str, "%d", max_index);
    gtk_label_set_text(GTK_LABEL(stats_words_word), str);
    sprintf(str, "%d", max_learn);
    gtk_label_set_text(GTK_LABEL(stats_words_learn), str);
    sprintf(str, "%d", fail);
    gtk_label_set_text(GTK_LABEL(stats_words_fail), str);
    sprintf(str, "%d", i_words);
    gtk_label_set_text(GTK_LABEL(stats_words_num), str);
    free(str);
}

int correct_index() {
    if (max_index == max_learn) {
        if (level != 3) {
            level++;
            free(index_arr);
            if (load_max_index() == -1) {
                wprintf(L"ERROR OPEN DATA!\n");
                gtk_main_quit();
            }
            main_window_massage(0);
            status_text();
            return -1;
        } else {
            main_window_massage(1);
            return -1;
        }
    }
    return 0;
}

int main_gtk(int argc, char *argv[]) {
    FILE *pfile;
    pfile = fopen("./data/profile/profile.txt", "r");
    gtk_init(&argc, &argv);
    windowgtk = create_windowgtk();
    gtk_widget_show(windowgtk);
    if (pfile == NULL) {
        edit_profile_1();
    } else {
        fclose(pfile);
        if (load_profile() == -1) {
            edit_profile_1();
        } else {
            if (load_max_index() == -1) {
                wprintf(L"ERROR OPEN DATA!\n");
                gtk_main_quit();        
            }
            correct_start_program();
            menubar_learn_activate_cb();
            if (max_learn < words) {
                gtk_widget_set_visible(menubar_retry, FALSE);
            } else {
                gtk_widget_set_sensitive(menubar_retry, TRUE);
                gtk_widget_set_visible(menubar_retry, TRUE);
            }
        }
    }
    gtk_main();
    return 0;
}
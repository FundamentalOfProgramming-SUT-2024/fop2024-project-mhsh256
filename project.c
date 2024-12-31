#include<ncurses.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<ctype.h>


char user_names[100][20];
FILE *fptr;
char user_name[50];
char password [31];
char email[50];

void pair_colors();
int make_or_load_user();
void which_page_to_go(); // connect made_or_load_user to next pages
void create_user();
void login();
void game_menu();
void profile_menu();
void Hall_of_Heroes();



int main(){
    srand(time(NULL));
    initscr();
    start_color();
    pair_colors();
    keypad(stdscr , TRUE);
    mvprintw(15 ,40, "press any key to continue");
    getch();
    which_page_to_go(make_or_load_user());
    endwin();
    return 0;
}

void pair_colors(){
    init_pair(1,COLOR_BLACK,COLOR_WHITE); // to select something
    init_pair(2,COLOR_BLACK,COLOR_RED); // to alert
}

//---------------------------------first page
int make_or_load_user(){
    clear();
    noecho();
    int which = 0;
    int key;
    move(6,1);
    attron(COLOR_PAIR(1));
    mvprintw(6,1,"create new user");
    attroff(COLOR_PAIR(1));
    mvprintw(7,1,"login");
    mvprintw(8,1,"game menu");
    mvprintw(9,1,"profile menu");
    mvprintw(10,1,"Hall of Heroes");
    while (1)
    {
        key = getch();
        if (key == KEY_UP && which > 0)
        {
            which--;
        }else if (key == KEY_DOWN &&  which < 4)
        {
            which ++;
        }
        clear();
        if (which == 0)
        {
            attron(COLOR_PAIR(1));
            mvprintw(6,1,"create new user");
            attroff(COLOR_PAIR(1));
            mvprintw(7,1,"login");
            mvprintw(8,1,"game menu");
            mvprintw(9,1,"profile menu");
            mvprintw(10,1,"Hall of Heroes");
        }else if (which == 1)
        {
            mvprintw(6,1,"create new user");
            attron(COLOR_PAIR(1));
            mvprintw(7,1,"login");
            attroff(COLOR_PAIR(1));
            mvprintw(8,1,"game menu");
            mvprintw(9,1,"profile menu");
            mvprintw(10,1,"Hall of Heroes");
        }else if (which == 2)
        {
            mvprintw(6,1,"create new user");
            mvprintw(7,1,"login");
            attron(COLOR_PAIR(1));
            mvprintw(8,1,"game menu");
            attroff(COLOR_PAIR(1));
            mvprintw(9,1,"profile menu");
            mvprintw(10,1,"Hall of Heroes");
        }else if (which == 3)
        {
            mvprintw(6,1,"create new user");
            mvprintw(7,1,"login");
            mvprintw(8,1,"game menu");
            attron(COLOR_PAIR(1));
            mvprintw(9,1,"profile menu");
            attroff(COLOR_PAIR(1));
            mvprintw(10,1,"Hall of Heroes");
        }else if (which == 4)
        {
            mvprintw(6,1,"create new user");
            mvprintw(7,1,"login");
            mvprintw(8,1,"game menu");
            mvprintw(9,1,"profile menu");
            attron(COLOR_PAIR(1));
            mvprintw(10,1,"Hall of Heroes");
            attroff(COLOR_PAIR(1));
        }
        
        refresh();
        if (key == '\n')
        {
            break;
        }
    }
    echo();
    return which;
}
void which_page_to_go(int n){
    if (n == 0)
    {
        create_user();
    }else if (n == 1)
    {
        login();
    }else if (n == 2)
    {
        game_menu();
    }else if (n == 3)
    {
        profile_menu();
    }else if (n == 4)
    {
        Hall_of_Heroes();
    } 
}
// ---------------------------------------- create user
char random_char(const char *charset, int size) {
    return charset[rand() % size];
}
void generate_password(char *password, int length) {
    const char uppercase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char lowercase[] = "abcdefghijklmnopqrstuvwxyz";
    const char digits[] = "0123456789";
    const char all_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$+=-/?^&*()";
    
    int i;
    
    password[0] = random_char(uppercase, sizeof(uppercase) - 1);
    password[1] = random_char(lowercase, sizeof(lowercase) - 1);
    password[2] = random_char(digits, sizeof(digits) - 1);
    for (i = 3; i < length; i++) {
        password[i] = random_char(all_chars, sizeof(all_chars) - 1);
    }
    password[length] = '\0';
    for (i = 0; i < length; i++) {
        int j = rand() % length;
        char temp = password[i];
        password[i] = password[j];
        password[j] = temp;
    }
}
int is_email_valid(char *email) {
    char *at_sign = strchr(email, '@');
    if (!at_sign || at_sign == email) {
        return 0; 
    }
    char *dot = strchr(at_sign + 1, '.');
    if (!dot || dot == at_sign + 1) {
        return 0; 
    }
    if (*(dot + 1) == '\0') {
        return 0; 
    }
    return 1;
}
void create_user(){
    int num_users = 0;
    fptr = fopen("user_names.txt","r");
    while (fgets(user_names[num_users], 20, fptr)) {
        user_names[num_users][strcspn(user_names[num_users], "\n")] = '\0';
        size_t len = strlen(user_names[num_users]);
        // user_names[num_users][len - 1] = '\0';
        num_users++;
    }
    fclose(fptr);
    
    while (1) // to input user name
    {
        clear();
        attron(A_BOLD);
        mvprintw(1,1,"CREAT NEW USER!!!");
        attroff(A_BOLD);
        mvprintw(6,1,"user name: ");
        getstr(user_name);
        int user_name_exists = 0;
        for (int i = 0; i < num_users; i++)
        {
            // mvprintw(10,10,"%s %ld" , user_name , strlen(user_name));
            // mvprintw(11,10,"%s %ld" , user_names[i] , strlen(user_names[i]));
            // getch();
            if (strcmp(user_name,user_names[i]) == 0)
            {
                attron(COLOR_PAIR(2));
                mvprintw(7,1,"this user name already exists");
                attroff(COLOR_PAIR(2));
                user_name_exists ++;
                refresh();
                getch();
                break;
            }
            
        }
        
        if (user_name[0] == ' ' || user_name[0] == '\t')
        {
            attron(COLOR_PAIR(2));
            mvprintw(7,1,"it can't start with space");
            attroff(COLOR_PAIR(2));
            getch();
        }else if (user_name[strlen(user_name) - 1] == ' ' || user_name[strlen(user_name) - 1] == '\t')
        {
            attron(COLOR_PAIR(2));
            mvprintw(7,1,"it can't end with space");
            attroff(COLOR_PAIR(2));
            getch();
        }else if (strlen(user_name) > 20)
        {
            attron(COLOR_PAIR(2));
            mvprintw(7,1,"user name lengh must not exeed 20 characters");
            attroff(COLOR_PAIR(2));
            getch();
        }else if (strlen(user_name) == 0)
        {
            attron(COLOR_PAIR(2));
            mvprintw(7,1,"please enter something");
            attroff(COLOR_PAIR(2));
            getch();
        }
        
        
        
        else{
            if (user_name_exists == 0)
            {
                break;
            }
            
        }
        refresh();
    }
    while (1) // to input password
    {
        clear();
        attron(A_BOLD);
        mvprintw(1,1,"CREAT NEW USER!!!");
        attroff(A_BOLD);
        mvprintw(6,1,"user name: %s" , user_name);
        mvprintw(7,1,"password(if you want a random one just input Y): ");
        refresh();
        getstr(password);
        int is_there_digit = 0;
        int is_captal = 0;
        int is_lower = 0;
        for (int i = 0; i < strlen(password); i++)
        {
            if (password[i] > 47 && password[i] < 58)
            {
                is_there_digit ++;
            }else if (password[i] < 123 && password[i] > 96)
            {
                is_lower ++;
            }else if (password[i] < 91 && password[i] > 64)
            {
                is_captal ++;
            }
        }
        if (password[0] == ' ' || password[0] == '\t')
        {
            attron(COLOR_PAIR(2));
            mvprintw(8,1,"it can't start with space");
            attroff(COLOR_PAIR(2));
            getch();
        }else if (password[strlen(password) - 1] == ' ' || password[strlen(password) - 1] == '\t')
        {
            attron(COLOR_PAIR(2));
            mvprintw(8,1,"it can't end with space");
            attroff(COLOR_PAIR(2));
            getch();
        }else if (strlen(password) == 0)
        {
            attron(COLOR_PAIR(2));
            mvprintw(8,1,"please enter something");
            attroff(COLOR_PAIR(2));
            getch();
        }
        else if (strcmp(password, "Y") == 0)
        {
            generate_password(password,10);
            mvprintw(8,1,"this is your random password %s" , password);
            getch();
            break;
        }
        
        else if (strlen(password) < 7)
        {
            attron(COLOR_PAIR(2));
            mvprintw(8,1,"password lengh must be at least 7 characters");
            attroff(COLOR_PAIR(2));
            getch();
        }else if (is_there_digit == 0)
        {
            attron(COLOR_PAIR(2));
            mvprintw(8,1,"there must be a digit");
            attroff(COLOR_PAIR(2));
            getch();
        }else if (is_captal == 0)
        {
            attron(COLOR_PAIR(2));
            mvprintw(8,1,"there must be a capital letter");
            attroff(COLOR_PAIR(2));
            getch();
        }else if (is_lower == 0)
        {
            attron(COLOR_PAIR(2));
            mvprintw(8,1,"there must be a lower case letter");
            attroff(COLOR_PAIR(2));
            getch();
        }else{
            break;
        }
        refresh(); 
    }
    while (1) // to input email
    {
        clear();
        attron(A_BOLD);
        mvprintw(1,1,"CREAT NEW USER!!!");
        attroff(A_BOLD);
        mvprintw(6,1,"user name: %s" , user_name);
        mvprintw(7,1,"password: %s" , password);
        mvprintw(8,1,"email(valid input is something like xxx@yyy.zzz): ");
        getstr(email);
        if (is_email_valid(email) == 0)
        {
            attron(COLOR_PAIR(2));
            mvprintw(9,1,"email is not valid");
            attroff(COLOR_PAIR(2));
            getch();
        }
        else if (email[0] == ' ' || email[0] == '\t')
        {
            attron(COLOR_PAIR(2));
            mvprintw(8,1,"it can't start with space");
            attroff(COLOR_PAIR(2));
            getch();
        }else if (email[strlen(email) - 1] == ' ' || email[strlen(email) - 1] == '\t')
        {
            attron(COLOR_PAIR(2));
            mvprintw(8,1,"it can't end with space");
            attroff(COLOR_PAIR(2));
            getch();
        }else if (strlen(email) == 0)
        {
            attron(COLOR_PAIR(2));
            mvprintw(8,1,"please enter something");
            attroff(COLOR_PAIR(2));
            getch();
        }
        else{
            break;
        }
        
    }
    fptr = fopen("user_names.txt" , "a");
    fprintf(fptr, "%s\n" , user_name);
    fclose(fptr);
    fptr = fopen("passwords.txt" , "a");
    fprintf(fptr, "%s\n" , password);
    fclose(fptr);
    fptr = fopen("emails.txt" , "a");
    fprintf(fptr, "%s\n" , email);
    fclose(fptr);


    login();// go to the next page
    
    
    // getch();
}
void login(){
    printw("1");
}
void game_menu(){
    printw("2");
}
void profile_menu(){
    printw("3");
}
void Hall_of_Heroes(){
    printw("4");
}
#include<ncurses.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<ctype.h>


int num_users;
int color_of_main_character = 0; // 0:red  , 1:blue , 2:white
int difficulty = 1; // 0: easy   , 1:medium , 2:hard
char user_names[100][20];
char passwords [100][100];
char emails [100][100];
FILE *fptr;
char user_name[50];
char password [31];
char email[50];


void get_informations();
void pair_colors();
int make_or_load_user();
void which_page_to_go(); // connect made_or_load_user to next pages
void create_user();
void login();
void game_menu();
void profile_menu();
void Hall_of_Heroes();
void resume_game();
void settings();




int main(){
    get_informations();
    srand(time(NULL));
    initscr();
    start_color();
    pair_colors();
    keypad(stdscr , TRUE);
    mvprintw(15 ,40, "press any key to continue");
    getch();
    make_or_load_user();
    clear();
    mvprintw(1,3,"salam zendegi");
    getch();
    endwin();
    return 0;
}
//----------------------- painting
void pair_colors(){
    init_pair(1,COLOR_BLACK,COLOR_WHITE); // to select something
    init_pair(2,COLOR_BLACK,COLOR_RED); // to alert
    init_pair(3,COLOR_BLACK,COLOR_GREEN); // to congratulate
}

//-----------------------------------get informations
void get_informations(){
    num_users = 0;
    fptr = fopen("user_names.txt","r");
    while (fgets(user_names[num_users], 20, fptr)) {
        user_names[num_users][strcspn(user_names[num_users], "\n")] = '\0';
        size_t len = strlen(user_names[num_users]);
        // user_names[num_users][len - 1] = '\0';
        num_users++;
    }
    fclose(fptr);
    fptr = fopen("passwords.txt","r");
    int num_pass = 0;
    while (fgets(passwords[num_pass], 20, fptr)) {
        passwords[num_pass][strcspn(passwords[num_pass], "\n")] = '\0';
        size_t len = strlen(passwords[num_pass]);
        // user_names[num_users][len - 1] = '\0';
        num_pass++;
    }
    fclose(fptr);
    fptr = fopen("emails.txt","r");
    num_pass = 0;
    while (fgets(emails[num_pass], 20, fptr)) {
        emails[num_pass][strcspn(emails[num_pass], "\n")] = '\0';
        size_t len = strlen(emails[num_pass]);
        // user_names[num_users][len - 1] = '\0';
        num_pass++;
    }
    fclose(fptr);
}

//---------------------------------first page
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
    which_page_to_go(which);
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
    attron(COLOR_PAIR(3));
    mvprintw(15,1,"user created successfully!");
    attroff(COLOR_PAIR(3));
    getch();
    make_or_load_user();// go to the next page
    
    
    // getch();
}



//-------------------------------------login
void login(){
    int which_user;
    while (1) // to enter user name
    {
        clear();
        attron(A_BOLD);
        mvprintw(1,1,"LOGIN PAGE!!!");
        attroff(A_BOLD);
        mvprintw(6,1,"enter your user name(if you have not enter Y): ");
        getstr(user_name);
        if (strcmp(user_name,"Y") == 0)
        {
            create_user();
        }
        int is_user_name_exist = -1;
        
        for (int i = 0; i < num_users; i++)
        {
            if (strcmp(user_name,user_names[i]) == 0)
            {
                is_user_name_exist = i;
                break;
            }
        }
        which_user = is_user_name_exist;
        if (is_user_name_exist == -1)
        {
            attron(COLOR_PAIR(2));
            mvprintw(7,1,"user name is not exist");
            attroff(COLOR_PAIR(2));
            refresh();
            getch();
        }else{
            break;
        }
    }
    while (1) // enter password
    {
        clear();
        attron(A_BOLD);
        mvprintw(1,1,"LOGIN PAGE!!!");
        attroff(A_BOLD);
        mvprintw(6,1,"enter your user name: %s" , user_name);
        mvprintw(7,1,"enter your password(if you don't remember enter Y): ");
        getstr(password);
        if (strcmp(password , "Y") == 0)
        {
            mvprintw(8,1,"enter you email: ");
            getstr(email);
            if (strcmp(email , emails[which_user]) == 0)
            {
                attron(COLOR_PAIR(3));
                mvprintw(9,1,"it is your password: %s" , passwords[which_user]);
                attroff(COLOR_PAIR(3));
                getch();
            }else{
                attron(COLOR_PAIR(2));
                mvprintw(9,1,"it is not your email!");
                attroff(COLOR_PAIR(2));
                getch();
            }
        }else{
            if (strcmp(password,passwords[which_user]) == 0)
            {
                attron(COLOR_PAIR(3));
                mvprintw(8,1,"welcome to rogue(M.H.Shirazi version)");
                attroff(COLOR_PAIR(3));
                getch();
                break;
            }else
            {
                attron(COLOR_PAIR(2));
                mvprintw(8,1,"this is not your password!");
                attroff(COLOR_PAIR(2));
                getch();
            } 
        } 
    }
    game_menu();
}

// ------------------------------------ game menu
void game_menu(){
    int which = 0;
    while (1)
    {
        clear();
        int key;
        attron(A_BOLD);
        mvprintw(1,1,"GAME MENU");
        // mvprintw(3,1 , "%d" , which);
        attroff(A_BOLD);
        if (which == 0)
        {
            attron(COLOR_PAIR(1));
            mvprintw(6,1,"create new game");
            attroff(COLOR_PAIR(1));
            mvprintw(7,1,"resume geme");
            mvprintw(8,1,"Hall of Heroes");
            mvprintw(9,1,"settings");
            mvprintw(10,1,"profile menu");
        }else if (which == 1)
        {
            mvprintw(6,1,"create new game");
            attron(COLOR_PAIR(1));
            mvprintw(7,1,"resume geme");
            attroff(COLOR_PAIR(1));
            mvprintw(8,1,"Hall of Heroes");
            mvprintw(9,1,"settings");
            mvprintw(10,1,"profile menu");
        }else if (which == 2)
        {
            mvprintw(6,1,"create new game");
            mvprintw(7,1,"resume geme");
            attron(COLOR_PAIR(1));
            mvprintw(8,1,"Hall of Heroes");
            attroff(COLOR_PAIR(1));
            mvprintw(9,1,"settings");
            mvprintw(10,1,"profile menu");
        }else if (which == 3)
        {
            mvprintw(6,1,"create new game");
            mvprintw(7,1,"resume geme");
            mvprintw(8,1,"Hall of Heroes");
            attron(COLOR_PAIR(1));
            mvprintw(9,1,"settings");
            attroff(COLOR_PAIR(1));
            mvprintw(10,1,"profile menu");
        }else if (which == 4)
        {
            mvprintw(6,1,"create new game");
            mvprintw(7,1,"resume geme");
            mvprintw(8,1,"Hall of Heroes");
            mvprintw(9,1,"settings");
            attron(COLOR_PAIR(1));
            mvprintw(10,1,"profile menu");
            attroff(COLOR_PAIR(1));
        }
        key = getch();
        // mvprintw(14,1,"%c" , key);
        if (key == KEY_UP && which > 0)
        {
            which--;

        }else if (key == KEY_DOWN &&  which < 4)
        {
            which ++;
            // mvprintw(15,1,"salam");
        }else if (key == '\n')
        {
            break;
        }
        // getch();
        
        refresh();
        
    }
    if (which == 0)
    {
        resume_game();
    }else if (which == 1)
    {
        resume_game();
    }else if (which == 2)
    {
        Hall_of_Heroes();
    }else if (which == 3)
    {
        settings();
    }else if (which == 4)
    {
        profile_menu();
    } 
}

//---------------------------------------settings
void settings(){
    int which = 0;
    while (1) // difficuly settings
    {
        int key;
        clear();
        attron(A_BOLD);
        mvprintw(1,1,"SETTINGS");
        attroff(A_BOLD);
        mvprintw(6,1,"difficulty level: ");
        if (which == 0)
        {
            attron(COLOR_PAIR(1));
            mvprintw(7,3,"easy(i am nooooob)");
            attroff(COLOR_PAIR(1));
            mvprintw(8,3,"medium");
            mvprintw(9,3,"hard(my recommendation: don't choose this)");
        }else if (which == 1)
        {
            mvprintw(7,3,"easy(i am nooooob)");
            attron(COLOR_PAIR(1));
            mvprintw(8,3,"medium");
            attroff(COLOR_PAIR(1));
            mvprintw(9,3,"hard(my recommendation: don't choose this)");
        }else if (which == 2)
        {
            mvprintw(7,3,"easy(i am nooooob)");
            mvprintw(8,3,"medium");
            attron(COLOR_PAIR(1));
            mvprintw(9,3,"hard(my recommendation: don't choose this)");
            attroff(COLOR_PAIR(1));
        }
        key = getch();
        // mvprintw(14,1,"%c" , key);
        if (key == KEY_UP && which > 0)
        {
            which--;

        }else if (key == KEY_DOWN &&  which < 2)
        {
            which ++;
            // mvprintw(15,1,"salam");
        }else if (key == '\n')
        {
            break;
        }
        refresh();
    }
    difficulty = which;
    which = 0;
    while (1) // main character settings
    {
        clear();
        int key;
        attron(A_BOLD);
        mvprintw(1,1,"SETTINGS");
        attroff(A_BOLD);
        if (difficulty == 0)
        {
            mvprintw(6,1,"difficulty level: easy");
        }else if (difficulty == 1)
        {
            mvprintw(6,1,"difficulty level: medium");
        }else if (difficulty == 2)
        {
            mvprintw(6,1,"difficulty level: hard");
        }
        mvprintw(7,1,"main character color: ");
        if (which == 0)
        {
            attron(COLOR_PAIR(1));
            mvprintw(8,3,"red");
            attroff(COLOR_PAIR(1));
            mvprintw(9,3,"blue");
            mvprintw(10,3,"white");
        }else if (which == 1)
        {
            mvprintw(8,3,"red");
            attron(COLOR_PAIR(1));
            mvprintw(9,3,"blue");
            attroff(COLOR_PAIR(1));
            mvprintw(10,3,"white");
        }else if (which == 2)
        {
            mvprintw(8,3,"red");
            mvprintw(9,3,"blue");
            attron(COLOR_PAIR(1));
            mvprintw(10,3,"white");
            attroff(COLOR_PAIR(1));
        }
        key = getch();
        // mvprintw(14,1,"%c" , key);
        if (key == KEY_UP && which > 0)
        {
            which--;

        }else if (key == KEY_DOWN &&  which < 2)
        {
            which ++;
            // mvprintw(15,1,"salam");
        }else if (key == '\n')
        {
            break;
        }
    }
    color_of_main_character = which;
    attron(COLOR_PAIR(3));
    mvprintw(15,1,"settings applied successfully");
    attroff(COLOR_PAIR(3));
    getch();
}

//------------------------------- not developed yet
void resume_game(){
    printw("7");
}


void profile_menu(){
    printw("3");
}
void Hall_of_Heroes(){
    printw("4");
}
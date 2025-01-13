// mohammad hossein shirazi 403110499

//------------------------includes
#include<ncurses.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<ctype.h>
#include<math.h>
#include<float.h>




//-------------------------------defines
#define MIN_ROOM_SIZE 6
#define MAX_ROOM_SIZE 12
#define MIN_ROOM_NUMBER 6
#define MAX_ROOM_NUMBER 9
#define MAP_WIDTH 30
#define MAP_LENTGH 120


//------------------------------structs
typedef struct 
{
    int x,y;
    char what_kind_of_cell;
    int discover;
    int color;
}Cell;

typedef struct 
{
    int x , y;
    int length , width;
    int connecting_rooms[10];
}Room;


//------------------------------variables
int message = 3; // 3:nothing 0:new floor 1:new room 2:found gold
char all_messages[5][50];
int num_users;
int resume_or_not = 0;
Cell map[MAP_WIDTH][MAP_LENTGH];
Room rooms[20];
int discoverd_rooms[9];
int num_rooms;
int color_of_main_character = 0; // 0:white  , 5:red , 10:blue
int difficulty = 1; // 0: easy   , 1:medium , 2:hard
char user_names[100][20];
char passwords [100][100];
char emails [100][100];
FILE *fptr;
char user_name[50];
char password [31];
char email[50];
int hero_x;
int hero_y;
char under_hero_kind = '.';
int under_hero_color = 0;
int show_all_map = 0;
int which_floor = 0;


//------------------------------functions
void set_messages();
void messages();
void random_map_generate();
void initialize_map();
void print_map();
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
void create_game();
void settings();
void discover_room(int room);
int in_which_room(int x , int y);
void tester_print();
void tester_discover();
void move_hero();
void fast_travel();




int main(){
    get_informations();
    set_messages();
    srand(time(NULL));
    initscr();
    start_color();
    pair_colors();
    keypad(stdscr , TRUE);
    
    mvprintw(15 ,40, "press any key to continue");
    getch();
    make_or_load_user();
    
    clear();
    initialize_map();
    random_map_generate();
    discover_room(0);
    while (1)
    {
        clear();
        if (show_all_map)
        {
            tester_print();
        }else{
            print_map();
        }
        int direction = getch();
        
        
        if (direction == 'M')
        {
            if (show_all_map)
            {
                show_all_map = 0;
            }else
                show_all_map = 1;
            
        }
        else if (direction == 'f')
        {
            direction = getch();
            fast_travel(direction);
        }
        
        
        else if (direction == 'q')
        {
            break;
        }
        move_hero(direction);
    }
    
    // tester_print();
    // tester_discover();
    
    
    
    
    getch();
    endwin();
    return 0;
}
//----------------------- painting
void pair_colors(){
    init_color(100,600,300,0);
    init_pair(0,COLOR_WHITE , COLOR_BLACK);
    init_pair(1,COLOR_BLACK,COLOR_WHITE); // to select something
    init_pair(2,COLOR_BLACK,COLOR_RED); // to alert
    init_pair(3,COLOR_BLACK,COLOR_GREEN); // to congratulate
    init_pair(4,100,COLOR_BLACK); // path
    init_pair(5,COLOR_RED,COLOR_BLACK);
    init_pair(6,COLOR_YELLOW,COLOR_BLACK); // walls
    init_pair(7,COLOR_GREEN,COLOR_BLACK); // stairs
    init_pair(10,COLOR_BLUE,COLOR_BLACK);
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
            break;
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
        if (strcmp(user_name,"Y") == 0)
        {
            break;
        }
        
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
        resume_or_not = 0;
    }else if (which == 1)
    {
        resume_or_not = 1;
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
            mvprintw(8,3,"white");
            attroff(COLOR_PAIR(1));
            mvprintw(9,3,"red");
            mvprintw(10,3,"blue");
        }else if (which == 1)
        {
            mvprintw(8,3,"white");
            attron(COLOR_PAIR(1));
            mvprintw(9,3,"red");
            attroff(COLOR_PAIR(1));
            mvprintw(10,3,"blue");
        }else if (which == 2)
        {
            mvprintw(8,3,"white");
            mvprintw(9,3,"red");
            attron(COLOR_PAIR(1));
            mvprintw(10,3,"blue");
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
    color_of_main_character = 5  * which;
    attron(COLOR_PAIR(3));
    mvprintw(15,1,"settings applied successfully");
    attroff(COLOR_PAIR(3));
    getch();
}


//----------------------------------initiallize map
void initialize_map(){
    for (int i = 0; i < MAP_WIDTH; i++)
    {
        for (int j = 0; j < MAP_LENTGH; j++)
        {
            map[i][j].x = i;
            map[i][j].y = j;
            map[i][j].what_kind_of_cell = ' ';
            map[i][j].discover = 0;
            map[i][j].color = 0;
        }
    }
    for (int i = 0; i < 9; i++)
    {
        discoverd_rooms[i] = 0;
    }
    
}

// ----------------------------------- print map
void print_map(){
    for (int i = 0; i < MAP_WIDTH; i++)
    {
        for(int j = 0 ; j< MAP_LENTGH; j++){
            if (map[i][j].discover)
            {
                attron(COLOR_PAIR(map[i][j].color));
                mvprintw(i,j,"%c" ,map[i][j].what_kind_of_cell);
                attroff(COLOR_PAIR(map[i][j].color));
            }else{
                mvprintw(i,j," ");
            }
            
        }
    }   
}

//--------------------------------random map generation
int in_which_room(int x , int y){
    for (int i = 0; i < num_rooms; i++)
    {
        if (x >= rooms[i].x && x <= rooms[i].x + rooms[i].width
            && y >= rooms[i].y && y <= rooms[i].y + rooms[i].length)
        {
            return i;
        }
        
    }
    return -1;
    
}
int rooms_overlap(int i , int j){
    if (rooms[i].x+1 >= rooms[j].x && rooms[i].x <= rooms[j].x + rooms[j].width+1) 
    {
        if (rooms[i].y+1 >= rooms[j].y && rooms[i].y <= rooms[j].y + rooms[j].length+5)
        {
            return 1;
        }else if (rooms[j].y+1 >= rooms[i].y && rooms[j].y <= rooms[i].y + rooms[i].length+5)
        {
            return 1;
        }
    }else if (rooms[j].x+1 >= rooms[i].x && rooms[j].x <= rooms[i].x + rooms[i].width+5) 
    {
        if (rooms[i].y+1 >= rooms[j].y && rooms[i].y <= rooms[j].y + rooms[j].length+5)
        {
            return 1;
        }else if (rooms[j].y+1 >= rooms[i].y && rooms[j].y <= rooms[i].y + rooms[i].length+5)
        {
            return 1;
        }
    }
    return 0;
}

void room_generator(int i){
    while (1)
    {
        rooms[i].length  = MIN_ROOM_SIZE + rand() % (MAX_ROOM_SIZE - MIN_ROOM_SIZE);
        rooms[i].width  = MIN_ROOM_SIZE + rand() % (MAX_ROOM_SIZE - MIN_ROOM_SIZE);
        rooms[i].x = 5 + rand() % (MAP_WIDTH - rooms[i].width);
        rooms[i].y = rand() % (MAP_LENTGH - rooms[i].length);
        int w = 1;
        for (int j = 0; j < i; j++)
        {
            if (rooms_overlap(j,i))
            {
                w = 0;
            }
        }if (rooms[i].x+rooms[i].width >= MAP_WIDTH || rooms[i].y + rooms[i].length >= MAP_LENTGH)
        {
            w = 0;
        }
        
        if (w)
        {
            break;
        }  
    }
}

int find_closest_room(int room_index) {
    int min_distance_squared = 100000;
    int closest_room = -1;

    int x1 = rooms[room_index].x + rooms[room_index].width / 2.0;
    int y1 = rooms[room_index].y + rooms[room_index].length / 2.0;

    for (int i = 0; i < num_rooms; i++) {
        if (i == room_index) continue;

        int x2 = rooms[i].x + rooms[i].width / 2;
        int y2 = rooms[i].y + rooms[i].length / 2;

        // محاسبه مربع فاصله
        int distance_squared = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);

        if (distance_squared < min_distance_squared) {
            min_distance_squared = distance_squared;
            closest_room = i;
        }
    }
    return closest_room;
}

void generate_path(int room_index , int closest_room){
    int x_current = rooms[room_index].x + rooms[room_index].width / 2;
    int x_goal = rooms[closest_room].x + rooms[closest_room].width / 2;
    int y_current = rooms[room_index].y + rooms[room_index].length / 2;
    int y_goal = rooms[closest_room].y + rooms[closest_room].length / 2;
    while (x_current != x_goal || y_current != y_goal)
    {
        int last_x = x_current;
        int last_y = y_current;
        // تعیین جهت حرکت
        int direction = rand() % 2;
        if (direction == 1) {
            if (x_goal < x_current) {
                x_current--;
            } else if (x_goal > x_current) {
                x_current++;
            }
        } else {
            if (y_goal < y_current) {
                y_current--;
            } else if (y_goal > y_current) {
                y_current++;
            }
        }

        if (map[x_current][y_current].what_kind_of_cell == '|' || map[x_current][y_current].what_kind_of_cell == '_')
        {
            map[x_current][y_current].what_kind_of_cell = '+';
            if (map[last_x][last_y].what_kind_of_cell == '+')
            {
                if (last_x == rooms[room_index].x || last_x == rooms[room_index].x + rooms[room_index].width)
                {
                    map[last_x][last_y].what_kind_of_cell = '_';
                }else if(last_y == rooms[room_index].y || last_y == rooms[room_index].y + rooms[room_index].length)
                {
                    map[last_x][last_y].what_kind_of_cell = '|';
                }
            }else if (map[last_x][last_y].what_kind_of_cell == '#')
            {
                int which_room = in_which_room(x_current,y_current);
                rooms[which_room].connecting_rooms[room_index] = 1;
                rooms[room_index].connecting_rooms[which_room] = 1;
                break;
            }
        }else if (map[x_current][y_current].what_kind_of_cell == ' ')
        {
            map[x_current][y_current].what_kind_of_cell = '#';
            map[x_current][y_current].color = 4;
        }
        
        
        
    }
    
}


void random_map_generate(){
    num_rooms = MIN_ROOM_NUMBER + rand() % (MAX_ROOM_NUMBER - MIN_ROOM_NUMBER-1);
    
    
    for (int i = 0; i < num_rooms; i++)
    {
        room_generator(i);
        for (int j ; j < num_rooms; j++){
            rooms[i].connecting_rooms[j] = 0;
        }
        for (int j = rooms[i].x;j <= rooms[i].x+rooms[i].width ; j ++)
        {
            for (int t = rooms[i].y ; t <= rooms[i].y + rooms[i].length; t ++)
            {
                map[j][t].what_kind_of_cell = '.';
                map[j][t].discover = 0;
            }
            // mvprintw(j,1,"%d" , j); 
        }
        for (int j = rooms[i].x; j <= rooms[i].x+rooms[i].width; j++)
        {
            map[j][rooms[i].y].what_kind_of_cell = '|';
            map[j][rooms[i].y].color = 6;
            map[j][rooms[i].y+rooms[i].length].what_kind_of_cell = '|';
            map[j][rooms[i].y+rooms[i].length].color = 6;
            
        }
        for (int j = rooms[i].y; j <= rooms[i].y+rooms[i].length; j++)
        {
            map[rooms[i].x][j].what_kind_of_cell = '_';
            map[rooms[i].x][j].color = 6;
            map[rooms[i].x + rooms[i].width][j].what_kind_of_cell = '_';
            map[rooms[i].x + rooms[i].width][j].color = 6;
        }
        
        // mvprintw(1,1,"%d" ,rooms[i].x);
        // getch();
    }
    
    //-------------------------------------path
    int root = 1 + rand()%(num_rooms - 2);
    for (int i = 0; i < num_rooms; i++)
    {
        generate_path(i,root);
    }
    for (int i = 1; i < num_rooms; i+= 2)
    {
        int close = find_closest_room(i);
        if (close != root && rooms[close].connecting_rooms[i] == 0)
        {
            generate_path(i,close);
        }
        
    }
    hero_x = rooms[0].x + 1 + (rand()%(rooms[0].width -1));
    hero_y = rooms[0].y + 1 + (rand()%(rooms[0].length -1));
    map[hero_x][hero_y].what_kind_of_cell = '&';
    map[hero_x][hero_y].color = color_of_main_character;


    //--------------------------stairs
    int x_stairs = rooms[num_rooms-1].x + 1 + rand()%(rooms[num_rooms-1].width - 2);
    int y_stairs = rooms[num_rooms-1].y + 1 + rand()%(rooms[num_rooms-1].length - 2);
    map[x_stairs][y_stairs].what_kind_of_cell = '/';
    map[x_stairs][y_stairs].color = 7;
    
    
    
    
}

//--------------------------------messages
void set_messages(){
    strcpy(all_messages[0] , "You have entered a new floor.");
    strcpy(all_messages[1] , "You have entered a new room.");
    strcpy(all_messages[2] , "You found a piece of gold.");
}

void messages(){
    for (int i = 0; i < 50; i++)
    {
        map[0][i].what_kind_of_cell = ' ';
    }
    
    for (int i = 0; i < strlen(all_messages[message]); i++)
    {
        map[0][i].what_kind_of_cell = all_messages[message][i];
    }
}


//---------------------------------- game actions
void discover_room(int room){
    mvprintw(1,1,"%d %d",rooms[room].x , rooms[room].x + rooms[room].width);
    for (int i = rooms[room].x; i <= rooms[room].x + rooms[room].width; i++)
    {
        
        for (int j = rooms[room].y; j <= rooms[room].y + rooms[room].length; j++)
        {
            map[i][j].discover = 1;
            // mvprintw(i,j,"%d" , map[i][j].discover);
        }
    }
    discoverd_rooms[room] = 1;
}

int is_move_allowed(int direction){
    int next_x = hero_x;
    int next_y = hero_y;
    if (direction == '1')
    {
        next_x ++;
        next_y --;
    }else if (direction == '3')
    {
        next_x ++;
        next_y ++;
    }else if (direction == '9')
    {
        next_x --;
        next_y ++;
    }else if (direction == '7')
    {
        next_x --;
        next_y --;
    }else if (direction == '2')
    {
        next_x ++;
    }else if (direction == '8')
    {
        next_x --;
    }else if (direction == '4')
    {
        next_y --;
    }else if (direction == '6')
    {
        next_y ++;
    }
    if (map[next_x][next_y].what_kind_of_cell == '+' || map[next_x][next_y].what_kind_of_cell == '.'
    || map[next_x][next_y].what_kind_of_cell == '#')
    {
        return 1;
    }
    return 0;
}

void move_hero(int direction){
    int next_x = hero_x;
    int next_y = hero_y;
    if (direction == '1')
    {
        next_x ++;
        next_y --;
    }else if (direction == '3')
    {
        next_x ++;
        next_y ++;
    }else if (direction == '9')
    {
        next_x --;
        next_y ++;
    }else if (direction == '7')
    {
        next_x --;
        next_y --;
    }else if (direction == '2')
    {
        next_x ++;
    }else if (direction == '8')
    {
        next_x --;
    }else if (direction == '4')
    {
        next_y --;
    }else if (direction == '6')
    {
        next_y ++;
    }
    if (is_move_allowed(direction))
    {
        map[hero_x][hero_y].what_kind_of_cell = under_hero_kind;
        map[hero_x][hero_y].color = under_hero_color;
        under_hero_kind = map[next_x][next_y].what_kind_of_cell;
        under_hero_color = map[next_x][next_y].color;
        hero_x = next_x;
        hero_y = next_y;
        map[hero_x][hero_y].what_kind_of_cell = '&';
        map[hero_x][hero_y].color = color_of_main_character;
        map[hero_x+1][hero_y].discover = 1;
        map[hero_x+1][hero_y+1].discover = 1;
        map[hero_x+1][hero_y-1].discover = 1;
        map[hero_x-1][hero_y+1].discover = 1;
        map[hero_x][hero_y+1].discover = 1;
        map[hero_x-1][hero_y-1].discover = 1;
        map[hero_x][hero_y-1].discover = 1;
        map[hero_x-1][hero_y].discover = 1;
        if (under_hero_kind == '+')
        {
            discover_room(in_which_room(hero_x,hero_y));
        }
        
    }  
}

void fast_travel(int direction){
    while (is_move_allowed(direction))
    {
        move_hero(direction);
    }
    
}
//----------------------------------tester
void tester_print(){
    for (int i = 0; i < MAP_WIDTH; i++)
    {
        for (int j = 0; j < MAP_LENTGH; j++)
        {
            attron(COLOR_PAIR(map[i][j].color));
            mvprintw(i,j,"%c" , map[i][j].what_kind_of_cell);
            attroff(COLOR_PAIR(map[i][j].color));
        }
        
    }
    
}

void tester_discover(){
    for (int i = 0; i < MAP_WIDTH; i++)
    {
        for (int j = 0; j < MAP_LENTGH; j++)
        {
            mvprintw(i,j,"%d" , map[i][j].discover);
        }
        
    }
}
//------------------------------- not developed yet



void create_game(){
    clear();
}

void resume_game(){
    printw("7");
}


void profile_menu(){
    printw("3");
}
void Hall_of_Heroes(){
    printw("4");
}
// mohammad hossein shirazi 403110499

//------------------------includes
#include<ncurses.h>
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
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
#define SPELL_TIME 10
#define MAX_HEALTH 1000
#define MAX_HUNGER 1000
#define ENEMY_DAMAGE 40 //undead damage
#define MAX_TREASURE_WIDTH 20
#define MAX_TREASURE_LENTGH 90
#define MIN_TREASURE_WIDTH 10
#define MIN_TREASURE_LENTGH 50
#define MIN_TREASURE_ENEMIES 25
#define MAX_TREASURE_ENEMIES 50
#define TRAP_DAMAGE 30


//------------------------------structs
typedef struct 
{
    int x,y;
    char what_kind_of_cell;
    int discover;
    int color;
    int lost_weapons[5];
}Cell;

typedef struct 
{
    int x , y;
    int length , width;
    int connecting_rooms[10];
    int theme;
}Room;

typedef struct 
{
    int x , y,helth,damage,kind;
    char name[30];
}Enemy;

typedef struct {
    char username[20];
    char password[100];
    char email[100];
    int gold;
    int score;
    int game_played;
} User;


//------------------------------variables
int message = 0; 
char all_messages[50][75];
int userCount;
int resume_or_not = 0;
Cell map[MAP_WIDTH][MAP_LENTGH];
Room rooms[20];
User users[100];
int discoverd_rooms[9];
int num_rooms;
int color_of_main_character = 0; // 0:white  , 5:red , 10:blue
int difficulty = 1; // 2: easy   , 1:medium , 0:hard
// char user_names[100][20];
// char passwords [100][100];
// char emails [100][100];
FILE *fptr;
char user_name[50] = "";
char password [31];
char email[50];
int hero_x;
int hero_y;
char under_hero_kind = '.';
int under_hero_color = 0;
int show_all_map = 0;
int which_floor = 0;
int x_stairs = 0;
int y_stairs = 0;
int direction = 0;
int gold = 0;
int window_y;
int window_x;
int spells[3] = {0};// 0:health  1:speed  2:damage
int weapons[6] = {1,0,0,0,0,1};//0:mace  1:dagger  2:magic wand  3:normal arrow  4:sword  5:no weapon selected
int range_of_weapons[6] = {0,5,10,5,0,0};
int weapon_damage[6] = {5,12,15,5,10,0}; 
int selected_weapon = 0;
int game_times[10] = {0}; // 0:game time,1:last_food,2:health spell,3:speed spell,4:damage spell
int health = MAX_HEALTH;
int hunger_level = MAX_HUNGER;
int foods[5] = {0};// 0: no food ,<30:rotten food , <80:normal food , <100 odd: magic food ,<100 even: excellent food
char my_food[5][20] = {"nothing" , "nothing","nothing","nothing","nothing"};
char normal_foods[5][20] = {"Bread & Butter","Apple Pie","Roasted Chicken","Spaghetti","Vegetable Soup"};
char magic_foods[5][20] = {"Phoenix Feather","Dragon's Egg","Enchanted Mushroom","Goblin Stew","Elf Fruit Salad"};
char excellent_foods[5][20] = {"Ghormeh Sabzi","Lobster Thermidor","Caviar","Steak","Lasagna"};
Enemy monsters[100];
int all_enemies = 1;
int where_are_enemies[MAP_WIDTH][MAP_LENTGH] = {0};
int where_are_traps[MAP_WIDTH][MAP_LENTGH] = {0};
int score = 0;
int treasure_enemies = 1;
int which_user = 0;
int login_or_not = 0;
int music_pid = -1;
int which_song = 0;
int song_stop_or_play = 1;


//------------------------------functions
void set_messages();
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
void see_next_room();
void weapon_menu();
void hunger_and_health_menu();
void use_spell(int spell);
void passage_time();
void double_move(int direction);
void kill_enemy();
void attack();
void treasure_room();
void read_users_from_file();
void writeUsersToFile();
void initialize_users();
void sort_by_score();
void play_music(const char *filename);
void play_choosen_music();





int main(){
    read_users_from_file("user_info.txt",users);
    initialize_users();
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
    writeUsersToFile("user_info.txt",users);
    game_times[0] = SPELL_TIME * 3;
    
    play_choosen_music();
    
    
    for (int i = 1; i < 6; i++)
    {
        initialize_map();
        random_map_generate();
        if (i == 5)
        {
            initialize_map();
            treasure_room();
        }
        
        discover_room(0);
        clear();
        if (health <= 0)
        {
            break;
        }
        
        if (show_all_map)
        {
            tester_print();
        }else{
            print_map();
        }
        while (i)
        {
            direction = getch();
            //show all map
            if (direction == 'M')
            {
                if (show_all_map)
                {
                    show_all_map = 0;
                }else
                    show_all_map = 1;
                
            }
            // fast travel
            else if (direction == 'f')
            {
                direction = getch();
                fast_travel(direction);
            }
            //spells
            else if (direction == 'H')
            {
                if (spells[0])
                {
                    spells[0] -- ;
                    game_times[2] = game_times[0];
                    message = 22;
                }else
                    message = 25;  
            }
            else if (direction == 'S')
            {
                if (spells[1])
                {
                    spells[1] -- ;
                    game_times[3] = game_times[0];
                    message = 23;
                }else
                    message = 25;  
            }
            else if (direction == 'D')
            {
                if (spells[2])
                {
                    spells[2] -- ;
                    game_times[4] = game_times[0];
                    message = 24;
                }else
                    message = 25;  
            }
             
            // stop and play again music
            else if (direction == 'P')
            {
                if (song_stop_or_play)
                {
                    if (music_pid != -1) {
                        kill(music_pid, SIGKILL);
                        music_pid = -1;
                    }
                }else{
                    play_choosen_music();
                }
                song_stop_or_play = 1 - song_stop_or_play;
            }
            
            //attack enemies
            else if (direction == ' ')
            {
                attack();
            }
            
            
            // quit
            else if (direction == 'q')
            {
                break;
            }
            // put weapon in pack
            else if (direction == 'w')
            {
                selected_weapon = 5;
            }
            // weapon menu
            else if (direction == 'I')
            {
                weapon_menu();
            }
            // hunger menu
            else if (direction == 'E')
            {
                hunger_and_health_menu();
            }
            // move
            if (game_times[0] - game_times[3] < SPELL_TIME * 2)
            {
                double_move(direction);
            }else
                move_hero(direction);
            
            // print
            clear();
            if (show_all_map)
            {
                tester_print();
            }else{
                print_map();
            }

            //stairs
            if (under_hero_kind == '/' && (under_hero_color == 7 || under_hero_color == 11))
            {
                int go_to_next_floor = getch();
                if (go_to_next_floor == 'y'){
                    which_floor++;
                    message = 0;
                    if (under_hero_color == 11)
                    {
                        message = 30;
                    }
                    score += 111;
                    
                    break;
                }
                
            }
            //gold
            else if (under_hero_kind == 'g')
            {
                message = 4;
                clear();
                if (show_all_map)
                {
                    tester_print();
                }else{
                    print_map();
                }
                int get_gold = getch();
                if (get_gold == 'y')
                {
                    message = 2;
                    gold += 1;
                    under_hero_kind = '.';
                    if (under_hero_color == 11)
                    {
                        gold += 4;
                    }
                    
                    under_hero_color = 0;
                }else{
                    message = 5;
                }
            }
            //window
            else if (under_hero_kind == '=')
            {
                see_next_room();
                clear();
                if (show_all_map)
                {
                    tester_print();
                }else{
                    print_map();
                }
            }
            // spell
            else if (under_hero_kind == 'T')
            {
                message = 9;
                if (show_all_map)
                {
                    tester_print();
                }else{
                    print_map();
                }
                int get_spell = getch();
                if (get_spell == 'y')
                {
                    message = under_hero_color - 7;
                    spells[under_hero_color-13] ++;
                    under_hero_kind = '.';
                    under_hero_color = 0;
                    score += 17;
                }else{
                    message = 10;
                }
            }
            // weapon
            else if (under_hero_kind == '?')
            {
                message = 11;
                if (show_all_map)
                {
                    tester_print();
                }else{
                    print_map();
                }
                int get_weapon = getch();
                if (get_weapon == 'y')
                {
                    int which_weapon = 1 + rand() % 4;
                    if (which_weapon != 4){
                        message = 11 + which_weapon;
                        weapons[which_weapon] += (which_weapon + 1) * 5;
                        if (which_weapon == 2)
                            weapons[which_weapon] -= 7;
                        
                    }
                    else{
                        if (weapons[4])
                            message = 16;
                        else
                            message = 15; 
                        weapons[4] ++;
                    }
                    under_hero_color = 0;
                    under_hero_kind = '.';
                    score += 11;
                }else{
                    message = 17;
                }
            }
            //food
            else if (under_hero_kind == 'f')
            {
                message = 18;
                if (show_all_map)
                {
                    tester_print();
                }else{
                    print_map();
                }
                int get_spell = getch();
                if (get_spell == 'y')
                {
                    int free_plate = 0;
                    for (free_plate = 0; free_plate < 5; free_plate++)
                    {
                        if (foods[free_plate] == 0)
                            break;
                    }
                    if (free_plate == 5)
                    {
                        message = 21;
                    }else{
                        foods[free_plate] = 1 + rand() % 100;
                        if (foods[free_plate] == 0)
                        {
                            strcpy(my_food[free_plate],"nothing");
                        }else if (foods[free_plate] <= 80 && foods[free_plate] > 0)
                        {
                            strcpy(my_food[free_plate],normal_foods[rand() % 5]);
                        }else if (foods[free_plate] % 2 == 1 && foods[free_plate] > 80)
                        {
                            strcpy(my_food[free_plate],magic_foods[rand() % 5]);
                        }else if (foods[free_plate] %2 == 0 && foods[free_plate] > 80)
                        {
                            strcpy(my_food[free_plate],excellent_foods[rand() % 5]);
                        }
                        under_hero_kind = '.';
                        under_hero_color = 0;
                        message = 19;
                    }
                    
                    
                }else{
                    message = 20;
                }
            }
            
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    if (j == i && i == 0)
                    {
                        continue;
                    }
                    if (where_are_enemies[hero_x+i][hero_y + j])
                    {
                        health -= monsters[where_are_enemies[hero_x+i][hero_y+j]].damage;
                    }
                                        
                }
                
            }
            

            if (where_are_traps[hero_x][hero_y])
            {
                where_are_traps[hero_x][hero_y] = 0;
                message = 31;
                health -= TRAP_DAMAGE;
                under_hero_color = 2;
            }
            

            if (health <= 0)
            {
                break;
            }
            if (which_floor == 4 && treasure_enemies == 0)
            {
                break;
            }
            
            
            
            
            
            
            
        }
    }
    clear();
    score += gold * 31;
    if (health < 0)
    {
        attron(COLOR_PAIR(2));
        attron(A_BOLD);
        mvprintw(7,30,"You died!");
        attroff(COLOR_PAIR(2));
        attroff(A_BOLD);
        mvprintw(10,25,"score: %d gold: %d" , score,gold);
    }else{
        attron(COLOR_PAIR(3));
        attron(A_BOLD);
        mvprintw(7,30,"Victory!");
        attroff(COLOR_PAIR(3));
        attroff(A_BOLD);
        mvprintw(10,25,"score: %d gold: %d" , score , gold);
    }

    getch();
    if (login_or_not)
    {
        users[which_user-1].game_played++;
        users[which_user-1].score = score;
        users[which_user-1].gold = gold;
    }
    
    writeUsersToFile("user_info.txt",users);  
    
    // tester_print();
    // tester_discover();
    kill(music_pid, SIGKILL);
    
    
    endwin();
    return 0;
}
//------------------------------------------------------------- painting
void pair_colors(){
    init_color(100,600,300,0);
    init_color(101,239,250,192);
    init_color(102,1000,8,863);
    init_color(103,1000,161,47);
    init_color(104,200,98,400);
    init_color(105,310,310,310);
    init_color(106,67,788,27);
    init_color(107,902,851,459);
    init_color(108,1000,980,980);
    init_color(109,627,321,176);
    init_color(110,612,612,612);
    init_color(111,729,451,176);
    init_pair(0,COLOR_WHITE , COLOR_BLACK);
    init_pair(1,COLOR_BLACK,COLOR_WHITE); // to select something
    init_pair(2,COLOR_BLACK,COLOR_RED); // to alert
    init_pair(3,COLOR_BLACK,COLOR_GREEN); // to congratulate
    init_pair(4,100,COLOR_BLACK); // path
    init_pair(5,COLOR_RED,COLOR_BLACK);
    init_pair(6,101,COLOR_BLACK); // walls
    init_pair(7,COLOR_GREEN,COLOR_BLACK); // stairs up
    init_pair(8,COLOR_RED,COLOR_BLACK); // stairs down
    init_pair(9,COLOR_YELLOW,COLOR_BLACK); // gold
    init_pair(10,COLOR_BLUE,COLOR_BLACK);
    init_pair(11,COLOR_BLACK,COLOR_YELLOW); // black gold
    init_pair(12,COLOR_CYAN,COLOR_BLACK); // window
    init_pair(13,COLOR_BLACK,102); // health spell
    init_pair(14,COLOR_BLACK,COLOR_BLUE); // speed spell
    init_pair(15,COLOR_BLACK,103); // damage spell
    init_pair(16,COLOR_YELLOW,104); // secret weapon
    init_pair(17,105,105); // empty part of health
    init_pair(18,COLOR_GREEN,COLOR_GREEN);
    init_pair(19,COLOR_YELLOW,COLOR_YELLOW);
    init_pair(20,COLOR_RED,COLOR_RED);
    init_pair(21,COLOR_BLACK , COLOR_MAGENTA);
    init_pair(22,106,107);// food
    init_pair(23,108,109);// enemy
    init_pair(24,COLOR_WHITE,110);//silver medal
    init_pair(25,COLOR_BLACK,111);// bronze medal
}

//------------------------------------------------------------------get informations
// void get_informations(){
//     userCount = 0;
//     fptr = fopen("user_names.txt","r");
//     while (fgets(user_names[num_users], 20, fptr)) {
//         user_names[num_users][strcspn(user_names[num_users], "\n")] = '\0';
//         size_t len = strlen(user_names[num_users]);
//         // user_names[num_users][len - 1] = '\0';
//         num_users++;
//     }
//     fclose(fptr);
//     fptr = fopen("passwords.txt","r");
//     int num_pass = 0;
//     while (fgets(passwords[num_pass], 20, fptr)) {
//         passwords[num_pass][strcspn(passwords[num_pass], "\n")] = '\0';
//         size_t len = strlen(passwords[num_pass]);
//         // user_names[num_users][len - 1] = '\0';
//         num_pass++;
//     }
//     fclose(fptr);
//     fptr = fopen("emails.txt","r");
//     num_pass = 0;
//     while (fgets(emails[num_pass], 20, fptr)) {
//         emails[num_pass][strcspn(emails[num_pass], "\n")] = '\0';
//         size_t len = strlen(emails[num_pass]);
//         // user_names[num_users][len - 1] = '\0';
//         num_pass++;
//     }
//     fclose(fptr);
// }
void initialize_users(){
    for (int i = 0; i < 100; i++)
    {
        users[i].gold = 0;
        users[i].score = 0;
        users[i].game_played = 0;
    }
    
}

void read_users_from_file(const char *filename, User users[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    userCount = 0;
    while (userCount < 100 &&
           fscanf(file, "%s %s %s %d %d %d", 
                  users[userCount].username,
                  users[userCount].password,
                  users[userCount].email,
                  &users[userCount].gold,
                  &users[userCount].score,
                  &users[userCount].game_played) == 6) {
        (userCount)++;
    }

    fclose(file);
}

void writeUsersToFile(const char *filename, User users[]) {
    FILE *file = fopen(filename, "w"); 
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s %s %s %d %d %d\n", 
                users[i].username,
                users[i].password,
                users[i].email,
                users[i].gold,
                users[i].score,
                users[i].game_played);
    }

    fclose(file);
}
//--------------------------------------------------------------------first page
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

// ----------------------------------------------------------------- create user
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
        for (int i = 0; i < userCount; i++)
        {
            // mvprintw(10,10,"%s %ld" , user_name , strlen(user_name));
            // mvprintw(11,10,"%s %ld" , user_names[i] , strlen(user_names[i]));
            // getch();
            if (strcmp(user_name,users[i].username) == 0)
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
    strcpy(users[userCount].username , user_name);
    strcpy(users[userCount].password,password);
    strcpy(users[userCount].email,email);
    userCount ++;
    attron(COLOR_PAIR(3));
    mvprintw(15,1,"user created successfully!");
    attroff(COLOR_PAIR(3));
    writeUsersToFile("user_info.txt",users);
    getch();
    make_or_load_user();// go to the next page
    
    
    // getch();
}



//-------------------------------------------------------------------------login
void login(){
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
        
        for (int i = 0; i < userCount; i++)
        {
            if (strcmp(user_name,users[i].username) == 0)
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
            if (strcmp(email , users[which_user].email) == 0)
            {
                attron(COLOR_PAIR(3));
                mvprintw(9,1,"it is your password: %s" , users[which_user].password);
                attroff(COLOR_PAIR(3));
                getch();
            }else{
                attron(COLOR_PAIR(2));
                mvprintw(9,1,"it is not your email!");
                attroff(COLOR_PAIR(2));
                getch();
            }
        }else{
            if (strcmp(password,users[which_user].password) == 0)
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
    if (strcmp(user_name,"Y")!=0)
    {
        login_or_not = 1;
    }
    
    game_menu();
}

// ----------------------------------------------------------------- game menu
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

//-------------------------------------------------------------------settings
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
    difficulty = 2 - which;
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
    which = 0;
    while (1)
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
        
        if (color_of_main_character == 0)
        {
            mvprintw(7,1,"main character color: white");
        }else if (color_of_main_character == 1)
        {
            mvprintw(7,1,"main character color: red");
        }else if (color_of_main_character == 1)
        {
            mvprintw(7,1,"main character color: blue");
        }
        
        mvprintw(8,1,"music genre");
        if (which == 0)
        {
            attron(COLOR_PAIR(1));
            mvprintw(9,3,"nothing");
            attroff(COLOR_PAIR(1));
            mvprintw(10,3,"death");
            mvprintw(11,3,"deep");
            mvprintw(12,3,"viking");
        }else if (which == 1)
        {
            mvprintw(9,3,"nothing");
            attron(COLOR_PAIR(1));
            mvprintw(10,3,"death");
            attroff(COLOR_PAIR(1));
            mvprintw(11,3,"deep");
            mvprintw(12,3,"viking");
        }else if (which == 2)
        {
            mvprintw(9,3,"nothing");
            mvprintw(10,3,"death");
            attron(COLOR_PAIR(1));
            mvprintw(11,3,"deep");
            attroff(COLOR_PAIR(1));
            mvprintw(12,3,"viking");
        }else if (which == 3)
        {
            mvprintw(9,3,"nothing");
            mvprintw(10,3,"death");
            mvprintw(11,3,"deep");
            attron(COLOR_PAIR(1));
            mvprintw(12,3,"viking");
            attroff(COLOR_PAIR(1));
        }
        key = getch();
        // mvprintw(14,1,"%c" , key);
        if (key == KEY_UP && which > 0)
        {
            which--;

        }else if (key == KEY_DOWN &&  which < 3)
        {
            which ++;
            // mvprintw(15,1,"salam");
        }else if (key == '\n')
        {
            break;
        }
        
        
    }
    which_song = which;
    attron(COLOR_PAIR(3));
    mvprintw(15,1,"settings applied successfully");
    attroff(COLOR_PAIR(3));
    getch();
}


//----------------------------------------------------------------initiallize map
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
            where_are_enemies[i][j] = 0;
            where_are_traps[i][j] = 0;
            for (int t = 0; t < 5; t++)
            {
                map[i][j].lost_weapons[t] = 0;
            }
            
        }
    }
    for (int i = 0; i < 9; i++)
    {
        discoverd_rooms[i] = 0;
    }
    
}

// ----------------------------------------------------------------- print map
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
    mvprintw(0,0,"%s",all_messages[message]); 
    mvprintw(0,75,"GOLD: %d" , gold);
    if (selected_weapon == 0)
    {
        mvprintw(0,85 , "WEAPON: MACE");
    }else if (selected_weapon == 1)
    {
        mvprintw(0,85,"WEAPON: DAGGER");
    }else if (selected_weapon == 2)
    {
        mvprintw(0,85,"WEAPON: MAGIC WAND");
    }else if (selected_weapon == 3)
    {
        mvprintw(0,85,"WEAPON: NORMALL ARROW");
    }else if (selected_weapon == 4)
    {
        mvprintw(0,85,"WEAPON: SWORD");
    }else if (selected_weapon == 5)
    {
        mvprintw(0,85,"WEAPON: NOTHING");
    }
    mvprintw(0,120,"HEALTH: %d" , health/10);
    
    mvprintw(MAP_WIDTH + 2 , 0 ,"SPELLS H: %d S: %d D:%d" , spells[0] , spells[1],spells[2]);
    mvprintw(MAP_WIDTH+2 , 50,"floor: %d" , which_floor);
}

//-------------------------------------------------------------random map generation
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
    rooms[i].theme = 1;
}

void treasure_generator(int i){
    while (1)
    {
        rooms[0].length  = MIN_TREASURE_LENTGH + rand() % (MAX_TREASURE_LENTGH - MIN_TREASURE_LENTGH);
        rooms[0].width  = MIN_TREASURE_WIDTH + rand() % (MAX_TREASURE_WIDTH - MIN_TREASURE_WIDTH);
        rooms[0].x = 5 + rand() % (MAP_WIDTH - rooms[0].width);
        rooms[0].y = rand() % (MAP_LENTGH - rooms[0].length);
        int w = 1;
        if (rooms[i].x+rooms[i].width >= MAP_WIDTH || rooms[i].y + rooms[i].length >= MAP_LENTGH)
        {
            w = 0;
        }
        
        if (w)
        {
            break;
        }  
    }
    rooms[i].theme = 1;
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

void set_windows(){
    int x = window_x;
    int y = window_y;
    if (map[window_x][window_y-1].what_kind_of_cell == '.')
    {
        while (y < MAP_LENTGH - 1 && map[x][y].what_kind_of_cell != '|' 
        && map[x][y].what_kind_of_cell != '_')
            y ++;
    }else{
        while (y > 0 && map[x][y].what_kind_of_cell != '|' && map[x][y].what_kind_of_cell != '_'
        && map[x][y].what_kind_of_cell != '+')
            y --;
    }
    if (map[x][y].what_kind_of_cell == '_' || map[x][y].what_kind_of_cell == '|')
    {
        map[x][y].what_kind_of_cell = '=';
        map[x][y].color = 12;
    }
    
    
    
}

void random_map_generate(){
    if(which_floor == 0)
        room_generator(0);
    else{
        rooms[0].length = rooms[num_rooms - 1].length;
        rooms[0].width = rooms[num_rooms - 1].width;
        rooms[0].x = rooms[num_rooms - 1].x;
        rooms[0].y = rooms[num_rooms - 1].y;
    }
    num_rooms = MIN_ROOM_NUMBER + rand() % (MAX_ROOM_NUMBER - MIN_ROOM_NUMBER-1);
    for (int i = 1; i < num_rooms; i++)
    {
        room_generator(i);
    }
    
    for (int i = 0; i < num_rooms; i++)
    {
        for (int j = 0; j < 10; j++)
        {
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
    for (int i = 1; i < num_rooms; i += 2)
    {
        int close = find_closest_room(i);
        if (close != root && rooms[close].connecting_rooms[i] == 0)
        {
            generate_path(i,close);
        }
        
    }
    //----------------------------gold
    int num_of_golds;
    for (int i = 0; i < num_rooms; i++)
    {
        num_of_golds = rand() % (3 + difficulty);
        for (int j = 0; j < num_of_golds; j++)
        {
            int gold_x = rooms[i].x + 1 + rand()%(rooms[i].width-2);
            int gold_y = rooms[i].y + 1 + rand()%(rooms[i].length - 2);
            map[gold_x][gold_y].what_kind_of_cell = 'g';
            map[gold_x][gold_y].color = 9;
            int black_gold = rand()%10;
            if (black_gold == 1)
            {
                map[gold_x][gold_y].color = 11;
            }
            
        }
        
    }
    
    //---------------------------------spell
    int num_of_spells;
    for (int i = 0; i < num_rooms; i++)
    {
        num_of_spells = rand() % (2+difficulty);
        for (int j = 0; j < num_of_spells - 1; j++)
        {
            int spell_x = rooms[i].x + 1 + rand()%(rooms[i].width-2);
            int spell_y = rooms[i].y + 1 + rand()%(rooms[i].length - 2);
            map[spell_x][spell_y].what_kind_of_cell = 'T';
            int spell_color = 13 + rand() % 3;
            map[spell_x][spell_y].color = spell_color;
        }
        
    }
    
    //-----------------------------------weapon
    int num_of_weapons;
    for (int i = 0; i < num_rooms; i++)
    {
        num_of_weapons = (rand() % (4)) - 2; // no matter what is difficulty
        for (int j = 0; j < num_of_weapons; j++)
        {
            int weapon_x = rooms[i].x + 1 + rand()%(rooms[i].width-2);
            int weapon_y = rooms[i].y + 1 + rand()%(rooms[i].length - 2);
            map[weapon_x][weapon_y].what_kind_of_cell = '?';
            map[weapon_x][weapon_y].color = 16;
            
        }
    }

    //-----------------food
    int num_of_foods;
    for (int i = 0; i < num_rooms; i++)
    {
        num_of_foods = (rand() % (3)) - 1; // no matter what is difficulty
        for (int j = 0; j < num_of_foods; j++)
        {
            int food_x = rooms[i].x + 1 + rand()%(rooms[i].width-2);
            int food_y = rooms[i].y + 1 + rand()%(rooms[i].length - 2);
            map[food_x][food_y].what_kind_of_cell = 'f';
            map[food_x][food_y].color = 22;
            
        }
    }
    
    //-----------------trap
    int num_of_traps;
    for (int i = 0; i < num_rooms; i++)
    {
        num_of_traps = (rand() % (3)); // no matter what is difficulty
        for (int j = 0; j < num_of_traps; j++)
        {
            int trap_x = rooms[i].x + 1 + rand()%(rooms[i].width-2);
            int trap_y = rooms[i].y + 1 + rand()%(rooms[i].length - 2);
            where_are_traps[trap_x][trap_y] = 1;
        }
    }
    //-----------------enemy
    int num_of_enemies;
    for (int i = 0; i < num_rooms; i++)
    {
        num_of_enemies = rand() % 3; // no matter what is difficulty
        for (int j = 0; j < num_of_enemies; j++)
        {
            int enemy_x = rooms[i].x + 1 + rand()%(rooms[i].width-2);
            int enemy_y = rooms[i].y + 1 + rand()%(rooms[i].length - 2);
            map[enemy_x][enemy_y].color = 23;
            monsters[all_enemies].x = enemy_x;
            monsters[all_enemies].y = enemy_y;
            int kind_of_enemy = rand() % 5;
            monsters[all_enemies].kind = kind_of_enemy;
            where_are_enemies[enemy_x][enemy_y] = all_enemies;
            if (kind_of_enemy == 0)
            {
                map[enemy_x][enemy_y].what_kind_of_cell = 'D';
                monsters[all_enemies].helth = 5;
                strcpy(monsters[all_enemies].name , "Demon");
                monsters[all_enemies].damage = ENEMY_DAMAGE / 4;
            }else if (kind_of_enemy == 1)
            {
                map[enemy_x][enemy_y].what_kind_of_cell = 'F';
                monsters[all_enemies].helth = 10;
                strcpy(monsters[all_enemies].name , "Fire Breathing Monster");
                monsters[all_enemies].damage = ENEMY_DAMAGE / 2;
            }else if (kind_of_enemy == 2)
            {
                map[enemy_x][enemy_y].what_kind_of_cell = 'G';
                monsters[all_enemies].helth = 15;
                strcpy(monsters[all_enemies].name , "Giant");
                monsters[all_enemies].damage = ENEMY_DAMAGE / 3;
            }else if (kind_of_enemy == 3)
            {
                map[enemy_x][enemy_y].what_kind_of_cell = 'S';
                monsters[all_enemies].helth = 20;
                strcpy(monsters[all_enemies].name , "Snake");
                monsters[all_enemies].damage = ENEMY_DAMAGE * 5 / 4;
            }else if (kind_of_enemy == 4)
            {
                map[enemy_x][enemy_y].what_kind_of_cell = 'U';
                monsters[all_enemies].helth = 30;
                strcpy(monsters[all_enemies].name , "Undead");
                monsters[all_enemies].damage = ENEMY_DAMAGE;
            } 
            all_enemies ++;
        }
    }

    //-----------------hero
    if (which_floor == 0)
    {
        hero_x = rooms[0].x + 1 + (rand()%(rooms[0].width -2));
        hero_y = rooms[0].y + 1 + (rand()%(rooms[0].length -2));
    }else{
        under_hero_color = 8;
        under_hero_kind = '/';
    }
    
    map[hero_x][hero_y].what_kind_of_cell = '&';
    map[hero_x][hero_y].color = color_of_main_character;


    
    //--------------------------stairs
    x_stairs = rooms[num_rooms-1].x + 1 + rand()%(rooms[num_rooms-1].width - 2);
    y_stairs = rooms[num_rooms-1].y + 1 + rand()%(rooms[num_rooms-1].length - 2);
    map[x_stairs][y_stairs].what_kind_of_cell = '/';
    map[x_stairs][y_stairs].color = 7;
    if (which_floor == 4)
    {
        map[x_stairs][y_stairs].color = 11;
    }
    

    //------------------------------------window
    int window_room = rand() % num_rooms;
    window_y = rand() % 2;
    if (window_y == 1)
    {
        window_y = rooms[window_room].y;
    }else
        window_y = rooms[window_room].y + rooms[window_room].length;
    window_x = rooms[window_room].x + rand()%(rooms[window_room].width);
    for (int i = 0; i < 5 && map[window_x][window_y].what_kind_of_cell == '+'; i++)
    {
        window_x = rooms[window_room].x+1 + rand()%(rooms[window_room].width - 2);
    }
    map[window_x][window_y].what_kind_of_cell = '=';
    map[window_x][window_y].color = 12;
    set_windows();
    
    
    
    
    
}

void treasure_room(){
    treasure_generator(0);
    for (int j = rooms[0].x;j <= rooms[0].x+rooms[0].width ; j ++)
    {
        for (int t = rooms[0].y ; t <= rooms[0].y + rooms[0].length; t ++)
        {
            map[j][t].what_kind_of_cell = '.';
            map[j][t].discover = 0;
        }
            // mvprintw(j,1,"%d" , j); 
    }
    for (int j = rooms[0].x; j <= rooms[0].x+rooms[0].width; j++)
    {
        map[j][rooms[0].y].what_kind_of_cell = '|';
        map[j][rooms[0].y].color = 6;
        map[j][rooms[0].y+rooms[0].length].what_kind_of_cell = '|';
        map[j][rooms[0].y+rooms[0].length].color = 6;
            
    }
    for (int j = rooms[0].y; j <= rooms[0].y+rooms[0].length; j++)
    {
        map[rooms[0].x][j].what_kind_of_cell = '_';
        map[rooms[0].x][j].color = 6;
        map[rooms[0].x + rooms[0].width][j].what_kind_of_cell = '_';
        map[rooms[0].x + rooms[0].width][j].color = 6;
    }
        
    //----------------------------gold
    int num_of_golds;
    num_of_golds = rand() % (10 + difficulty);
    for (int j = 0; j < num_of_golds; j++)
    {
        int gold_x = rooms[0].x + 1 + rand()%(rooms[0].width-2);
        int gold_y = rooms[0].y + 1 + rand()%(rooms[0].length - 2);
        map[gold_x][gold_y].what_kind_of_cell = 'g';
        map[gold_x][gold_y].color = 9;
        int black_gold = rand()%10;
        if (black_gold == 1)
        {
            map[gold_x][gold_y].color = 11;
        }
        
    }
        
    //-----------------trap
    int num_of_traps;
    for (int i = 0; i < num_rooms; i++)
    {
        num_of_traps = (rand() % 20); // no matter what is difficulty
        for (int j = 0; j < num_of_traps; j++)
        {
            int trap_x = rooms[i].x + 1 + rand()%(rooms[i].width-2);
            int trap_y = rooms[i].y + 1 + rand()%(rooms[i].length - 2);
            where_are_traps[trap_x][trap_y] = 1;
        }
    }
    
    
    

    
    //-----------------enemy
    int num_of_enemies = MIN_TREASURE_ENEMIES + rand() % (MAX_TREASURE_ENEMIES - MIN_TREASURE_ENEMIES); // no matter what is difficulty
    treasure_enemies = num_of_enemies;
    for (int j = 0; j < num_of_enemies; j++)
    {
        int enemy_x = rooms[0].x + 1 + rand()%(rooms[0].width-2);
        int enemy_y = rooms[0].y + 1 + rand()%(rooms[0].length - 2);
        map[enemy_x][enemy_y].color = 23;
        monsters[all_enemies].x = enemy_x;
        monsters[all_enemies].y = enemy_y;
        int kind_of_enemy = rand() % 5;
        monsters[all_enemies].kind = kind_of_enemy;
        if (where_are_enemies[enemy_x][enemy_y])
        {
            treasure_enemies --;
        }
        
        where_are_enemies[enemy_x][enemy_y] = all_enemies;
        if (kind_of_enemy == 0)
        {
            map[enemy_x][enemy_y].what_kind_of_cell = 'D';
            monsters[all_enemies].helth = 5;
            strcpy(monsters[all_enemies].name , "Demon");
            monsters[all_enemies].damage = ENEMY_DAMAGE / 4;
        }else if (kind_of_enemy == 1)
        {
            map[enemy_x][enemy_y].what_kind_of_cell = 'F';
            monsters[all_enemies].helth = 10;
            strcpy(monsters[all_enemies].name , "Fire Breathing Monster");
            monsters[all_enemies].damage = ENEMY_DAMAGE / 2;
        }else if (kind_of_enemy == 2)
        {
            map[enemy_x][enemy_y].what_kind_of_cell = 'G';
            monsters[all_enemies].helth = 15;
            strcpy(monsters[all_enemies].name , "Giant");
            monsters[all_enemies].damage = ENEMY_DAMAGE / 3;
        }else if (kind_of_enemy == 3)
        {
            map[enemy_x][enemy_y].what_kind_of_cell = 'S';
            monsters[all_enemies].helth = 20;
            strcpy(monsters[all_enemies].name , "Snake");
            monsters[all_enemies].damage = ENEMY_DAMAGE * 5 / 4;
        }else if (kind_of_enemy == 4)
        {
            map[enemy_x][enemy_y].what_kind_of_cell = 'U';
            monsters[all_enemies].helth = 30;
            strcpy(monsters[all_enemies].name , "Undead");
            monsters[all_enemies].damage = ENEMY_DAMAGE;
        } 
        all_enemies ++;
    }

    //-----------------hero
    
    hero_x = rooms[0].x + 1 + (rand()%(rooms[0].width -2));
    hero_y = rooms[0].y + 1 + (rand()%(rooms[0].length -2));
    map[hero_x][hero_y].what_kind_of_cell = '&';
    map[hero_x][hero_y].color = color_of_main_character;


}
//----------------------------------------------------------------messages
void set_messages(){
    strcpy(all_messages[0] , "You have entered a new floor.");
    strcpy(all_messages[1] , "You have entered a new room.");
    strcpy(all_messages[2] , "You found a piece of gold.");
    strcpy(all_messages[3],"You have left the room.");
    strcpy(all_messages[4],"Do you want to get the gold?");
    strcpy(all_messages[5],"You droped the gold!");
    strcpy(all_messages[6], "You found a health spell.");
    strcpy(all_messages[7], "You found a speed spell.");
    strcpy(all_messages[8], "You found a damage spell.");
    strcpy(all_messages[9],"Do you want to get the spell?");
    strcpy(all_messages[10],"You droped the spell!");
    strcpy(all_messages[11],"Do you want to get the weapon?");
    strcpy(all_messages[12],"You found 10 daggers.");
    strcpy(all_messages[13],"You found 8 magics.");
    strcpy(all_messages[14],"You found 20 normal arrows.");
    strcpy(all_messages[15],"You found a sword.");
    strcpy(all_messages[16],"Oops! You found a sword again.");
    strcpy(all_messages[17],"You droped the weapon!");
    strcpy(all_messages[18],"Do you want to get the food?");
    strcpy(all_messages[19],"You found some food.");
    strcpy(all_messages[20],"You droped the food!");
    strcpy(all_messages[21],"You don't have enough space for a new food.");
    strcpy(all_messages[22],"The celestial healers have descended upon the earth once more.");
    strcpy(all_messages[23],"You used the speed spell. The world slows down in your eyes.");
    strcpy(all_messages[24],"The damage spell will lead you to certain victory.");
    strcpy(all_messages[25],"You have not the spell.");
    strcpy(all_messages[26],"Attacking monsters barehanded isn't a good idea.");
    strcpy(all_messages[27],"You have slain an enemy!");
    strcpy(all_messages[28],"You hit the enemy.");
    strcpy(all_messages[29],"You did not hit anything.");
    strcpy(all_messages[30],"You have entered the treasure room. So close to victory!");
    strcpy(all_messages[31],"You stepped on a trap.");
    
    
}




//---------------------------------------------------------------- game actions
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

void see_next_room(){
    int x = hero_x;
    int y = hero_y;
    if (map[hero_x][hero_y-1].what_kind_of_cell == '.' || map[hero_x][hero_y-1].what_kind_of_cell == '_')
    {
        while (y < MAP_LENTGH - 1 && map[x][y].what_kind_of_cell != '=' 
        && map[x][y].what_kind_of_cell != '+')
            y ++;
    }else{
        while (y > 0 && map[x][y].what_kind_of_cell != '=' && map[x][y].what_kind_of_cell != '+')
            y --;
    }
    
    discover_room(in_which_room(x,y));
}

void kill_enemy(int x , int y){
    where_are_enemies[x][y] = 0;
    message = 27;
    map[x][y].what_kind_of_cell = '.';
    map[x][y].color = 0;
    if (which_floor == 4)
    {
        treasure_enemies --;
    }
    score += 11+ rand() % 20;
    
}

void hit_enemy(int power , int x , int y){
    monsters[where_are_enemies[x][y]].helth -= power;
    if (game_times[0]-game_times[4] < SPELL_TIME)
    {
        monsters[where_are_enemies[x][y]].helth -= power;
    }
    message = 28;
    
    if (monsters[where_are_enemies[x][y]].helth <= 0)
    {
        kill_enemy(x,y);
    }
    
}

void attack(){
    if (selected_weapon == 5)// nothing
    {
        message = 26;
    }else if (selected_weapon == 0)// mace
    {
        int hit = 0;
        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                if (j == 0 && i == 0)
                {
                    continue;
                }
                if (where_are_enemies[hero_x + i][hero_y + j])
                {
                    hit_enemy(5,hero_x+i,hero_y+j);
                    hit ++;
                }
                
            }
            
        }
        
        if (hit == 0)
        {
            message = 29;
        }
        
        
    }else if (selected_weapon == 1)// dagger
    {
        int direction = getch();
        weapons[selected_weapon]--;
        if (weapons[selected_weapon] == 0)
        {
            selected_weapon = 5;
        }
        
        int dis_x , dis_y;
        if (direction == '9')
        {
            dis_x = -1;
            dis_y = 1;    
        }else if (direction == '8')
        {
            dis_x = -1;
            dis_y = 0;
        }else if (direction == '7')
        {
            dis_x = -1;
            dis_y = -1;
        }else if (direction == '6')
        {
            dis_x = 0;
            dis_y = 1;
        }else if (direction == '4')
        {
            dis_x = 0;
            dis_y = -1;
        }else if (direction == '3')
        {
            dis_x = 1;
            dis_y = 1;
        }else if (direction == '2')
        {
            dis_x = 1;
            dis_y = 0;
        }else if (direction == '1')
        {
            dis_x = 1;
            dis_y = -1;
        }  
        int next_x = hero_x + dis_x;
        int next_y = hero_y + dis_y;
        int damage = 12;
        int range = 5;
        if (map[next_x][next_y].what_kind_of_cell != '_' && map[next_x][next_y].what_kind_of_cell != '|' &&
        direction <= '9' && direction >= '1' && direction != '5')
        {
            for (int i = 0; i < range; i++)
            {
                if (where_are_enemies[next_x][next_y])
                {
                    hit_enemy(12,next_x,next_y);
                    
                }else if (map[next_x][next_y].what_kind_of_cell == '_' || map[next_x][next_y].what_kind_of_cell == '|')
                {
                    map[next_x-dis_x][next_y-dis_y].lost_weapons[1] ++;
                    break;
                }
                
                next_x += dis_x;
                next_y += dis_y;
                
           }
        }
        
        
    }else if (selected_weapon == 2)// magic wand
    {
        int direction = getch();
        weapons[selected_weapon]--;
        if (weapons[selected_weapon] == 0)
        {
            selected_weapon = 5;
        }
        int dis_x , dis_y;
        if (direction == '9')
        {
            dis_x = -1;
            dis_y = 1;    
        }else if (direction == '8')
        {
            dis_x = -1;
            dis_y = 0;
        }else if (direction == '7')
        {
            dis_x = -1;
            dis_y = -1;
        }else if (direction == '6')
        {
            dis_x = 0;
            dis_y = 1;
        }else if (direction == '4')
        {
            dis_x = 0;
            dis_y = -1;
        }else if (direction == '3')
        {
            dis_x = 1;
            dis_y = 1;
        }else if (direction == '2')
        {
            dis_x = 1;
            dis_y = 0;
        }else if (direction == '1')
        {
            dis_x = 1;
            dis_y = -1;
        }  
        int next_x = hero_x + dis_x;
        int next_y = hero_y + dis_y;
        int damage = 15;
        int range = 10;
        if (map[next_x][next_y].what_kind_of_cell != '_' && map[next_x][next_y].what_kind_of_cell != '|' &&
        direction <= '9' && direction >= '1' && direction != '5')
        {
            for (int i = 0; i < range; i++)
            {
                if (where_are_enemies[next_x][next_y])
                {
                    hit_enemy(15,next_x,next_y);
                    
                }else if (map[next_x][next_y].what_kind_of_cell == '_' || map[next_x][next_y].what_kind_of_cell == '|')
                {
                    map[next_x-dis_x][next_y-dis_y].lost_weapons[2] ++;
                    break;
                }
                
                next_x += dis_x;
                next_y += dis_y;
                
           }
        }
    }else if (selected_weapon == 3)// normal arrow
    {
        int direction = getch();
        weapons[selected_weapon]--;
        if (weapons[selected_weapon] == 0)
        {
            selected_weapon = 5;
        }
        int dis_x , dis_y;
        if (direction == '9')
        {
            dis_x = -1;
            dis_y = 1;    
        }else if (direction == '8')
        {
            dis_x = -1;
            dis_y = 0;
        }else if (direction == '7')
        {
            dis_x = -1;
            dis_y = -1;
        }else if (direction == '6')
        {
            dis_x = 0;
            dis_y = 1;
        }else if (direction == '4')
        {
            dis_x = 0;
            dis_y = -1;
        }else if (direction == '3')
        {
            dis_x = 1;
            dis_y = 1;
        }else if (direction == '2')
        {
            dis_x = 1;
            dis_y = 0;
        }else if (direction == '1')
        {
            dis_x = 1;
            dis_y = -1;
        }  
        int next_x = hero_x + dis_x;
        int next_y = hero_y + dis_y;
        int damage = 5;
        int range = 5;
        if (map[next_x][next_y].what_kind_of_cell != '_' && map[next_x][next_y].what_kind_of_cell != '|' &&
        direction <= '9' && direction >= '1' && direction != '5')
        {
            for (int i = 0; i < range; i++)
            {
                if (where_are_enemies[next_x][next_y])
                {
                    hit_enemy(5,next_x,next_y);
                    
                }else if (map[next_x][next_y].what_kind_of_cell == '_' || map[next_x][next_y].what_kind_of_cell == '|')
                {
                    map[next_x-dis_x][next_y-dis_y].lost_weapons[3] ++;
                    break;
                }
                
                next_x += dis_x;
                next_y += dis_y;
                
           }
        }
    }else if (selected_weapon == 4)// sword
    {
        int hit = 0;
        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                if (j == 0 && i == 0)
                {
                    continue;
                }
                if (where_are_enemies[hero_x + i][hero_y + j])
                {
                    hit_enemy(10,hero_x+i,hero_y+j);
                    hit ++;
                }
                
            }
            
        }
        
        if (hit == 0)
        {
            message = 29;
        }
    }
    
    
    
    
    
    
}

//-----------------------------------------------------------------move
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
    || map[next_x][next_y].what_kind_of_cell == '#' || map[next_x][next_y].what_kind_of_cell == '/' 
    || map[next_x][next_y].what_kind_of_cell == 'g' || map[next_x][next_y].what_kind_of_cell == '='
    || map[next_x][next_y].what_kind_of_cell == 'T' || map[next_x][next_y].what_kind_of_cell == '?'
    || map[next_x][next_y].what_kind_of_cell == 'f')
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
        int enter_or_quit_room = 0;
        if (under_hero_kind == '#')
        {
            enter_or_quit_room = 1;
        }
        
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
            if (enter_or_quit_room)
            {
                message = 1;
            }else{
                message = 3;
            }
            
            discover_room(in_which_room(hero_x,hero_y));
        }
        passage_time();
        
    }  
}

void double_move(int direction){
    move_hero(direction);
    move_hero(direction);
}

void fast_travel(int direction){
    while (is_move_allowed(direction))
    {
        move_hero(direction);
    }
    
}
//-------------------------------------------------------------------tester
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
    mvprintw(0,0,"%s",all_messages[message]); 
    mvprintw(0,75,"GOLD: %d" , gold);
    if (selected_weapon == 0)
    {
        mvprintw(0,85 , "WEAPON: MACE");
    }else if (selected_weapon == 1)
    {
        mvprintw(0,85,"WEAPON: DAGGER");
    }else if (selected_weapon == 2)
    {
        mvprintw(0,85,"WEAPON: MAGIC WAND");
    }else if (selected_weapon == 3)
    {
        mvprintw(0,85,"WEAPON: NORMALL ARROW");
    }else if (selected_weapon == 4)
    {
        mvprintw(0,85,"WEAPON: SWORD");
    }else if (selected_weapon == 5)
    {
        mvprintw(0,85,"WEAPON: NOTHING");
    }
    mvprintw(0,120,"HEALTH: %d" , health/10);
    mvprintw(MAP_WIDTH + 2 , 0 ,"SPELLS H: %d S: %d D:%d" , spells[0] , spells[1],spells[2]);
    mvprintw(MAP_WIDTH+2 , 50,"floor: %d" , which_floor);

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


//--------------------------------------------------------------------weapon

void weapon_menu(){
    while (1)
    {
        clear();
        attron(A_BOLD);
        mvprintw(0,0,"WEAPON MENU");
        attroff(A_BOLD);
        mvprintw(5,0,"SHORT RANGE WEAPONS:");
        mvprintw(6,5,"Mace                 damage: %d    ammo count: %d   (click m to choose)" , weapon_damage[0] , weapons[0]);
        mvprintw(7,5,"Sword                damage: %d    ammo count: %d   (click s to choose)" , weapon_damage[4] , weapons[4]);
        mvprintw(10,0,"LONG RANGE WEAPONS:");
        mvprintw(11,5,"Dagger              damage: %d    range: %d    ammo count: %d   (click d to choose)", weapon_damage[1] , range_of_weapons[1],weapons[1]);
        mvprintw(12,5,"Magic wand          damage: %d    range: %d    ammo count: %d   (click w to choose)", weapon_damage[2] , range_of_weapons[2] , weapons[2]);
        mvprintw(13,5,"normal arrow        damage: %d    range: %d    ammo count: %d   (click a to choose)", weapon_damage[3] , range_of_weapons[3] , weapons[3]);
        int choosen_weapon = getch();
        if (choosen_weapon == 'm' || choosen_weapon == 's' || choosen_weapon == 'd' 
        || choosen_weapon == 'w'  || choosen_weapon == 'a')
        {
            if (selected_weapon == 5)
            {
                int success = 1;
                if (choosen_weapon == 'm')
                    selected_weapon = 0;
                else if(choosen_weapon == 's'){
                    if (weapons[4])
                    {
                       selected_weapon = 4; 
                    }else{
                        attron(COLOR_PAIR(2));
                        mvprintw(20,0,"You have not that weapon");
                        attroff(COLOR_PAIR(2));
                        success = 0;
                    }
                }
                    
                else if (choosen_weapon == 'd'){
                    if (weapons[1])
                    {
                       selected_weapon = 1; 
                    }else{
                        attron(COLOR_PAIR(2));
                        mvprintw(20,0,"You have not that weapon");
                        attroff(COLOR_PAIR(2));
                        success = 0;
                    }
                }
                else if (choosen_weapon == 'w'){
                    if (weapons[2])
                    {
                       selected_weapon = 2; 
                    }else{
                        attron(COLOR_PAIR(2));
                        mvprintw(20,0,"You have not that weapon");
                        attroff(COLOR_PAIR(2));
                        success = 0;
                    }
                }
                else if(choosen_weapon == 'a'){
                    if (weapons[3])
                    {
                       selected_weapon = 3; 
                    }else{
                        attron(COLOR_PAIR(2));
                        mvprintw(20,0,"You have not that weapon");
                        attroff(COLOR_PAIR(2));
                        success = 0;
                    }
                }
                if (success)
                {
                    attron(COLOR_PAIR(3));
                    mvprintw(20,0,"Your defult weapon changed success fully");
                    attroff(COLOR_PAIR(3));
                }
                
            }else{
               attron(COLOR_PAIR(2));
                mvprintw(20 , 0 , "First put your perevious weapon in your backpack.");
                attroff(COLOR_PAIR(2));
            }
            getch();
            break;
        }else if (choosen_weapon == 'q')
        {
            break;
        }
        
        else{
            attron(COLOR_PAIR(2));
            mvprintw(20 , 0 , "please enter something between {m,s,d,w,a}");
            attroff(COLOR_PAIR(2));
            getch();
        }
        
    }
    
}

//---------------------------------------------------------------------food
void hunger_and_health_menu(){
    while (1)
    {
        clear();
        mvprintw(2,0,"HEALTH: ");
        mvprintw(4,0,"HUNGER LEVEL: ");
        attron(COLOR_PAIR(17));
        for (int i = 0; i < 100; i++)
        {
            mvprintw(2,i+15," ");
            mvprintw(4,i+15 ," ");
        }
        attroff(COLOR_PAIR(17));
        
        if (health > 3 * MAX_HEALTH / 4)
        {
            attron(COLOR_PAIR(18));
            for (int i = 0; i < health * 100 / MAX_HEALTH; i++)
            {
                mvprintw(2,i+15," ");
            }
            attroff(COLOR_PAIR(18));
        }
        else if (health >= MAX_HEALTH / 2)
        {
            attron(COLOR_PAIR(19));
            for (int i = 0; i < health * 100 / MAX_HEALTH; i++)
            {
                mvprintw(2,i+15," ");
            }
            attroff(COLOR_PAIR(19));
        }else{
            attron(COLOR_PAIR(20));
            for (int i = 0; i < health * 100 / MAX_HEALTH; i++)
            {
                mvprintw(2,i+15," ");
            }
            attroff(COLOR_PAIR(20));
        }

        if (hunger_level > 3 * MAX_HUNGER / 4)
        {
            attron(COLOR_PAIR(18));
            for (int i = 0; i < hunger_level * 100 / MAX_HUNGER; i++)
            {
                mvprintw(4,i+15," ");
            }
            attroff(COLOR_PAIR(18));
        }
        else if (hunger_level >= MAX_HEALTH /2)
        {
            attron(COLOR_PAIR(19));
            for (int i = 0; i < hunger_level * 100 / MAX_HUNGER; i++)
            {
                mvprintw(4,i+15," ");
            }
            attroff(COLOR_PAIR(19));
        }else{
            attron(COLOR_PAIR(20));
            for (int i = 0; i < hunger_level * 100 / MAX_HUNGER; i++)
            {
                mvprintw(4,i+15," ");
            }
            attroff(COLOR_PAIR(20));
        }
        
        
        attron(A_BOLD);
        mvprintw(7,0,"FOODS: ");
        for (int i = 0; i < 5; i++)
        {
            mvprintw(8+i,9,"%s",my_food[i]);
        }
        
        attroff(A_BOLD);
        int which_food = getch();
        if (which_food == 'q')
        {
            break;
        }else if (which_food >= '1' && which_food <= '5')
        {
            which_food -= '1';
            attron(COLOR_PAIR(21));
            if (foods[which_food])
            {
                if (foods[which_food] <= 30)
                {
                    mvprintw(20,0,"You have unknowingly consumed a fatal toxin. The clock is ticking.");
                    health -= (3 - difficulty) * 10;
                    if (hunger_level > MAX_HUNGER - (difficulty + 1) * 5)
                    {
                        hunger_level = MAX_HUNGER;
                    }else{
                        hunger_level += (MAX_HUNGER + 1) * 5;
                    }
                    
                }else if (foods[which_food] <= 80)
                {
                    health = MAX_HEALTH;
                    if (hunger_level > MAX_HUNGER - (difficulty + 1) * 10)
                    {
                        hunger_level =  MAX_HUNGER;
                    }else{
                        hunger_level += (difficulty + 1) * 10;
                    }
                    
                    mvprintw(20,0,"You ate some food.");
                }else if (foods[which_food] % 2 == 1)
                {
                    health = MAX_HEALTH;
                    if (hunger_level > MAX_HUNGER - (3 - difficulty) * 10)
                    {
                        hunger_level = 100;
                    }else
                        hunger_level += (3 - difficulty) * 10;

                    
                    mvprintw(20,0,"You partook of a fabled repast, and now the speed of legends courses through your veins.");
                    // use_spell(1);
                }else if (foods[which_food] % 2 == 0)
                {
                    health = MAX_HEALTH;
                    if (hunger_level > 100 - (difficulty + 1) * 10)
                    {
                        hunger_level = MAX_HUNGER;
                    }else
                        hunger_level += (difficulty + 1) * 10;
                    
                    mvprintw(20,0,"You have tasted the pinnacle of earthly cuisine, and now possess a power that will etch your name into the annals of history!");
                    // use_spell(2);
                }
                
                
                strcpy(my_food[which_food],"nothing");
                foods[which_food] = 0;
            }else{
                mvprintw(20,0,"You decided not to eat anything");
            }
            attroff(COLOR_PAIR(21));
            getch();
            break;
        }else{
            attron(COLOR_PAIR(2));
            mvprintw(20,0,"Please enter something between 1 and 5");
            attroff(COLOR_PAIR(2));
        }
        
        

    }
    
}

//----------------------------------------------------------------------time
void passage_time(){
    game_times[0] ++;
    if (hunger_level > 0)
    {
        hunger_level -= 4;
    }
    
    
    if (hunger_level <= MAX_HUNGER / 4)
    {
        health -= 4;
    }if (hunger_level > MAX_HUNGER / 2)
    {
        if (health > MAX_HEALTH - 2 * (1 + difficulty))
        {
            health = MAX_HEALTH;
        }else
            health += 2 * (1 + difficulty); 
    }
    if (game_times[0] - game_times[2] < SPELL_TIME)
    {
        health += (1 + difficulty);
    }
    
    
}


//----------------------------------------------------------------------hall of heroes
void sort_by_score(){
    for (int i = 0; i < userCount - 1; i++)
    {
        for (int j = 0; j < userCount - i - 1; j++)
        {
            if (users[j].score<users[j+1].score)
            {
                User temp = users[j];
                users[j] = users[j+1];
                users[j+1] = temp;
                if (which_user == j)
                {
                    which_user ++;
                }else if (which_user == j+1)
                {
                    which_user --;
                }
                
                
            }   
        } 
    }
}


void Hall_of_Heroes(){
    read_users_from_file("user_info.txt",users);
    sort_by_score();
    int which_page = 1; // 1 means first and 0 means second
    while (1)
    {
        clear();
        attron(A_BOLD);
        mvprintw(1,1,"HALL OF HEROES");
        attroff(A_BOLD);
        mvprintw(3,20,"PLAYER");
        mvprintw(3,40,"SCORE");
        mvprintw(3,60,"GOLD");
        mvprintw(3,80,"EXPERIENCE");
        if (which_page)
        {
            attron(COLOR_PAIR(11));
            mvprintw(4,1,"THE CHOOSEN ONE");
            mvprintw(4,20,"%s",users[0].username);
            mvprintw(4,40,"%d" , users[0].score);
            mvprintw(4,60 , "%d" , users[0].gold);
            mvprintw(4,80,"%d",users[0].game_played);
            attroff(COLOR_PAIR(11));
            attron(COLOR_PAIR(24));
            mvprintw(5,1,"THE SHADOW");
            mvprintw(5,20,"%s",users[1].username);
            mvprintw(5,40,"%d" , users[1].score);
            mvprintw(5,60 , "%d" , users[1].gold);
            mvprintw(5,80,"%d",users[1].game_played);
            attroff(COLOR_PAIR(24));
            attron(COLOR_PAIR(25));
            mvprintw(6,1,"THE LAST SAMURAI");
            mvprintw(6,20,"%s",users[2].username);
            mvprintw(6,40,"%d" , users[2].score);
            mvprintw(6,60 , "%d" , users[2].gold);
            mvprintw(6,80,"%d",users[2].game_played);
            attroff(COLOR_PAIR(25));
            for (int i = 3; i < 10; i++)
            {
                if (i == which_user)
                {
                    attron(A_BOLD);
                    attron(A_ITALIC);
                }
                
                attron(COLOR_PAIR(i % 2));
                mvprintw(i+4,20,"%s",users[i].username);
                mvprintw(i+4,40,"%d" , users[i].score);
                mvprintw(i+4,60 , "%d" , users[i].gold);
                mvprintw(i+4,80,"%d",users[i].game_played);
                attroff(COLOR_PAIR(i % 2));
                if (i == which_user)
                {
                    attroff(A_BOLD);
                    attroff(A_ITALIC);
                }
                
            }
            
        }else{
            for (int i = 10; i < 20; i++)
            {
                if (i == which_user)
                {
                    attron(A_BOLD);
                    attron(A_ITALIC);
                }
                
                attron(COLOR_PAIR(i % 2));
                mvprintw(i-6,20,"%s",users[i].username);
                mvprintw(i-6,40,"%d" , users[i].score);
                mvprintw(i-6,60 , "%d" , users[i].gold);
                mvprintw(i-6,80,"%d",users[i].game_played);
                attroff(COLOR_PAIR(i % 2));
                if (i == which_user)
                {
                    attroff(A_BOLD);
                    attroff(A_ITALIC);
                }
                
            }
        }
        int next_or_not = getch();
        if (next_or_not == ' ')
        {
            if (userCount > 10)
            {
                which_page = 1 - which_page;
            }
        }else{
            break;
        }
        
        
    }
    make_or_load_user();
}

//-------------------------------music

void play_music(const char *filename) {
    if (music_pid != -1) {
        kill(music_pid, SIGKILL); 
    }

    music_pid = fork(); 
    if (music_pid == 0) { 
        execlp("mpg123", "mpg123", "-q", filename, NULL);
        exit(0);
    }
}

void play_choosen_music(){
    if (which_song == 1)
    {
        play_music("01 TOOL - Vicarious.mp3");
    }else if (which_song == 2)
    {
        play_music("05 Agalloch - Not Unlike The Waves.mp3");
    }else if (which_song == 3)
    {
        play_music("08 Great Hall Awaits A Fallen Brother.mp3");
    }
}

//------------------------------- not developed yet

void profile_menu(){
    int salam = 1;
}


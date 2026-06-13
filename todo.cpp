#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

/*
read file mode and write file mode
*/
const string TASKS = "todo_data/tasks.txt";
const string USER = "todo_data/user.txt";
const string MASCOT = "todo_data/mascot.txt";

vector<vector<string>> FILE_LINES;
vector<int> TaskIDs;

void read_tasks () {
    ifstream file(TASKS);
    if (file.is_open()) {
        int iterator = 1;
        while (!file.eof()) {
            string time_token_date,
                   time_token_time,
                   first_line_token, line;
            
            file >> time_token_date  
                 >> time_token_time
                 >> first_line_token;
            getline(file, line);

            auto time_token = time_token_date  + " "
                            + time_token_time;
            if (time_token!=" ") {
                TaskIDs.push_back(iterator);
                vector<string> tokens = {time_token, first_line_token+line};
                FILE_LINES.push_back(tokens);
            }

            iterator++;
        }

        // cout << "-- Tasks Ready --" << endl;
    } else {
        cout << "-- Failed to load Tasks --" << endl;
        system("pause");
    }
}

void write_tasks () {
    ofstream file(TASKS, std::ios::trunc);
    int iterator = 0;
    for (auto & t : FILE_LINES) {
        if (iterator < FILE_LINES.size() - 1)
            file << t[0] << " " << t[1] << endl;
        else
            file << t[0] << " " << t[1];
        
        iterator++;
    }
}

string get_time_now() {
    time_t now = time(nullptr);
    struct tm *now_comp = localtime(&now);

    string month = (now_comp->tm_mon + 1 > 9) ? to_string(now_comp->tm_mon + 1): "0"+to_string(now_comp->tm_mon + 1);
    string day = (now_comp->tm_mday > 9) ? to_string(now_comp->tm_mday): "0"+to_string(now_comp->tm_mday);
    string hour = (now_comp->tm_hour > 9) ? to_string(now_comp->tm_hour): "0"+to_string(now_comp->tm_hour);
    string minutes = (now_comp->tm_min > 9) ? to_string(now_comp->tm_min): "0"+to_string(now_comp->tm_min);

    string date = day + "/" 
                + month + "/"
                + to_string(now_comp->tm_year + 1900);
    string time = hour + ":" + minutes;
    return date + " " + time;
}

void display_tasks () {
    cout << "# Your Tasks #" << endl;

    int iterator = 0;
    if (FILE_LINES.size()!=0) {
    for (auto & s : FILE_LINES) {
        cout << "[" << TaskIDs[iterator] << "] ";
        for (auto & t : s)
            cout << t << "    ";
        cout << endl;

        iterator ++;
    }
    } else {
        cout << "# No Tasks In Database #" << endl;
        FILE_LINES.clear();
        TaskIDs.clear();
    }
}

void create_tasks (const string & time, const string & task) {
    vector<string> new_task = {time, task};
    FILE_LINES.push_back(new_task);
}

void delete_task (const int & task_id) {
    if (task_id <= TaskIDs.size()) {

    auto id = task_id - 1;

    FILE_LINES.erase(FILE_LINES.begin() + id);
    TaskIDs.erase(TaskIDs.begin() + id);

    for(auto & i : TaskIDs)
        if (i > id + 1)
            i--;
    }else {
        cout << "Invalid Task ID" << endl;
        system("pause");
    }
}

void modify_task (const int & task_id, const string & modified_string) {
    if (task_id <= TASKS.size()) {

        auto id = task_id - 1;
    
        FILE_LINES[id][1] = modified_string;
        FILE_LINES[id][0] = get_time_now();
    }else {
        cout << "Invalid Task ID" << endl;
        system("pause");
    }
}

vector<vector<string>> Mascots;
void load_mascots() {
    ifstream mascots(MASCOT);
    if (mascots.is_open()) {
        int iterator = 1;
        vector<string> mascot_chunk;
        while (!mascots.eof()) {
            string mascot_line;
            getline(mascots, mascot_line);

            // if (mascot_line!="")
                mascot_chunk.push_back(mascot_line);
            if (iterator % 4 == 0) {
                Mascots.push_back(mascot_chunk);
                mascot_chunk.clear();
            }
            iterator++;
        }
    }
}

void mascot () {
    int max = Mascots.size(), min = 0;

    time_t seed = time(nullptr);
    srand(seed);
    int random_mascot = rand() % (max - min) + min;
    // cout << random_mascot << endl;

    for (auto & m : Mascots[random_mascot]) {
        cout << m << endl;
    }
}

vector<string> USER_CRED = {"0"};
bool load_user_credentials() {
    ifstream user(USER);
    string HAS_ACC, USER_NAM, user_name, PWD, password;
    bool   has_account;
    
    user >> HAS_ACC >> has_account;
    if(has_account == false) return 0;
    user >> USER_NAM >> user_name >> PWD >> password;
    USER_CRED = {to_string(has_account), user_name, password};

    return has_account;
}

void write_login_credentials () {
    if (USER_CRED[0] == "1") {
    ofstream user(USER);
    user << "HAS_ACCOUNT: " << USER_CRED[0] << endl;
    user << "USERNAME: " << USER_CRED[1] << endl;
    user << "PASSWORD: " << USER_CRED[2];
    }
}

bool login_failed = false;
void gotch_u() {
    cout << " (\\__/)    +---------------------------------+" << endl;
    cout << " ( ! !)  <{ YOU IMPOSTER... GO AWAY.. ~baka~ |" << endl;
    cout << "  /  |     +---------------------------------+" << endl;
    cout << "`````````````````````````````````````````````" << endl;
    cout << " # LOGIN FAILED #" << endl;
    login_failed = true;
    system("pause");


}

int authenticate() {
    if (load_user_credentials()==true) {
        string username, password;
        cout << " (\\__/)    +-------------------------------+" << endl;
        cout << " ( - -)  <{ YOU CAN'T PASS WITHOUT LOGIN.. |" << endl;
        cout << "  /  |     +-------------------------------+" << endl;
        cout << "`````````````````````````````````````````````" << endl;
        cout << "       # Enter USERNAME : ";
        cin  >> username;
        if (username != USER_CRED[1]) {gotch_u(); return 0;}
        cout << "       # Enter PASSWORD : ";
        cin  >> password;
        if (password != USER_CRED[2]) {gotch_u(); return 0;};

        cout << " # LOGIN SUCCESSFULL #" << endl;
        system("cls");
    } 

    return 0;
}

void ask_account() {
    if (USER_CRED[0] == "0") {
        cout << "         # Create Account # [A]" << endl;
    } else {
        cout << "         # Account Panel  # [A]" << endl;
    }
}

void delete_account (bool AlsoClearData) {
    string password;
    cout << "         # Enter your PASSWORD: ";
    cin  >> password;
    if (password==USER_CRED[2]) {
        USER_CRED = {"0"};
        ofstream clear_user(USER, std::ios::trunc);
        if (AlsoClearData == true) ofstream clear_task(TASKS, std::ios::trunc);

        cout << " (\\__/)    +-------------+" << endl;
        cout << " ( T T)  <{ ~Aarigato~.. |" << endl;
        cout << "  /  |     +-------------+" << endl;
        system("pause");
    } else {
        cout << " (\\__/)    +--------------------------------------------------+" << endl;
        cout << " ( . .)  <{ HUH-?...DIDN'T EXPECT YOU TO ENTER WRONG PASSWORD |" << endl;
        cout << "  /  |     +--------------------------------------------------+" << endl;

        cout << "         # ACCOUNT DELETION FAILED # " << endl;
        system("pause");

    }
}

bool admin_panel_exit = false;
void Account_Panel() {
    cout << "          (\\__/)  " << endl;
    cout << "          ( > o)  " << endl;
    cout << "````````````````````````````````" << endl;
    cout << "         # Change USERNAME [U] #" << endl;
    cout << "         # Change PASSWORD [P] #" << endl;
    cout << "         # Delete Account  [D] #" << endl;///
    cout << "         # EXIT Panel      [E] #" << endl;
    cout << "         # >>> ";
    string input = "k";
    cin >> input;

    if (input == "u" || input == "U") {
        string new_username;
        cout << "               # Enter new USRNAME: ";
        cin  >> new_username;
        USER_CRED[1] = new_username;
        cout << "               # New USERNAME: " << USER_CRED[1] << endl;
        system("pause");

    } else if (input == "p" || input == "P") {
        string new_password, confirm_new_password;
        cout << "               # Enter   new PASSWORD: ";
        cin  >> new_password;
        while(new_password!=confirm_new_password) {
            cout << "               # Confirm new PASSWORD: ";
            cin  >> confirm_new_password;
            if (new_password!=confirm_new_password) {
                cout << "               # Confirm Password not same as above # " << endl;
                cout << "               # Please re-enter # " << endl;
            }
        }

        USER_CRED[2] = confirm_new_password;
        cout << "               # New PASSWORD: " << USER_CRED[2] << endl;
        system("pause");

    } else if (input == "d" || input == "D") {
        string response;
        cout << "               # Do you wish to keep your current tasks ? [y/n]: ";
        cin  >> response;
        if (response == "y" || response == "Y") { 
            delete_account(1); 
            admin_panel_exit = true;
        }
        else if (response == "n" || response == "N") {
            delete_account(0);
            admin_panel_exit = true;
        }
        else {
            cout << "           # Not a valid response #" << endl;
            system("pause");
            admin_panel_exit = true;


        }
    } else if (input == "e" || input == "E") {
        admin_panel_exit = true;
    } else {

    }
}

void account_manager() {
    admin_panel_exit = false;
    if (USER_CRED[0] == "0") {
        string entered_username, entered_password, entered_confirmation_password;
        cout << "         # Let's Create Your Account #" << endl;
        cout << "               # Enter   your USERNAME: ";
        cin  >> entered_username;
        cout << "               # Enter   your PASSWORD: ";
        cin  >> entered_password;
        while(entered_password!=entered_confirmation_password) {
            cout << "               # confirm your PASSWORD: ";
            cin  >> entered_confirmation_password;
            if(entered_password!=entered_confirmation_password) 
                cout << "               # Confirm Password not same as above # " << endl;
                cout << "               # Please re-enter # " << endl;
        }

        USER_CRED[0] = "1";
        USER_CRED.push_back(entered_username);
        USER_CRED.push_back(entered_password);

        cout << "         # Next time use these credentials to LOG IN [E: to exit] # >>> " ;
        cin >> entered_confirmation_password;

    } else {
        cout << "         # Welcome " << USER_CRED[1] << " what's in your mind #" << endl;
        while(admin_panel_exit == false) {
            system("cls");
            Account_Panel();
        }
    }
}

void greeting () {
    if (USER_CRED[0] == "0") {
        cout << "       # WELCOME #" << endl;
    } else {
        cout << "       # WELCOME [" << USER_CRED[1] << "] #" << endl;
    }
}

void ask_user () {
    cout << endl;
    // cout << " (\\__/)    +--------------------+" << endl;
    // cout << " ( o o)  <{ What's in your mind |" << endl;
    // cout << "  /  |     +--------------------+" << endl;
    mascot();
    cout << "`````````````````````````````````" << endl;
    greeting();
    cout << "         # Create a Task  # [C]" << endl;
    cout << "         # Delete a Task  # [D]" << endl;
    cout << "         # Modify a Task  # [M]" << endl;
    ask_account();
    cout << "         # >>> ";
    string input = "k";
    cin >> input;

    if (input == "c" || input == "C") {

        cout << "       # Create your tasks {<dbl ENTER> to exit}:" << endl;
        int ignore_id = 1;
        string task = "def";

        while (task!="") {
            int current_id;
            if (TaskIDs.size() != 0) 
                current_id = TaskIDs[TaskIDs.size()-1];
            else
                current_id = 0;


            cout << "           # [" << current_id + 1 << "]: ";
            cin.ignore(ignore_id);
            getline(cin, task);

            if (task!=""){
                TaskIDs.push_back(current_id + 1);
                create_tasks(get_time_now(), task);
            }

            ignore_id = 0;
        }

    } else if (input == "d" || input == "D") {
        cout << "       # Delete your tasks {0 to exit}[ID]:" << endl;
        int Deleting_id;
        cout << "           >>> ";
        cin >> Deleting_id;
        delete_task(Deleting_id);
    } else if (input == "m" || input == "M") {
        cout << "       # Modify your tasks {0 to exit}[ID]:" << endl;
        int Modifying_id;
        cout << "           >>> ";
        cin >> Modifying_id;
        if (Modifying_id <= TaskIDs.size()) {
        cout << "       # Current task: " << FILE_LINES[Modifying_id - 1][1] << endl;
        cout << "       # Enter new task: ";
        string task;
        cin.ignore();
        getline(cin, task);        
        modify_task(Modifying_id, task);
        } else {
            cout << "Invalid ID" << endl;
            system("pause");
        }
    }
    else if (input == "a" || input == "A") {
        account_manager();
    } else {
        
    }

}

int main () {
    system("cls");

    read_tasks();
    load_mascots();
    // cout << Mascots.size();
    authenticate();

    while(!login_failed) {

        display_tasks();
        ask_user();

        system("cls");
        write_tasks();
        write_login_credentials();
    }

    cout << "# SESSION TERMINATED #" << endl;
    cout << endl;
    system("pause");

    return 0;
}
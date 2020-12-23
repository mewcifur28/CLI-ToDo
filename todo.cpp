#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

void add_item(string);
void delete_item(int);
void done_item(int);
void display_items(void);
void display_stats(void);
void display_usage(void);
string get_curr_date(void);
void remove_item_from_file(int, bool);

int main(int argc, char* argv[])
{
    if (argc == 1) {
        display_usage();
    }

    else {
        string command(argv[1]);

        if (command == "add") {
            if (argc == 2) {
                cout << "Error: Missing todo string. Nothing added!";
            }
            else {
                add_item(argv[2]);
            }
        }

        else if (command == "ls") {
            display_items();
        }

        else if (command == "del") {
            if (argc == 2) {
                cout << "Error: Missing NUMBER for deleting todo.";
            }
            else {
                int number = stoi(argv[2]);
                delete_item(number);
            }
        }

        else if (command == "done") {
            if (argc == 2) {
                cout << "Error: Missing NUMBER for marking todo as done.";
            }
            else {
                int number = stoi(argv[2]);
                done_item(number);
            }
        }

        else if (command == "help") {
            display_usage();
        }

        else if (command == "report") {
            display_stats();
        }
    }

    return 0;
}

void add_item(string item) {

    // write item to file
    ofstream fout;
    ifstream fin;
    fin.open("todo.txt");
    fout.open("todo.txt", ios::app);
    fout << item << "\n";
    fin.close();
    fout.close();

    // print item
    cout << "Added todo: \"" << item << "\"";
}

void delete_item(int number) {

    int pending = 0;

    if ((static_cast<bool>(std::ifstream("todo.txt")))) {
        ifstream fin;
        fin.open("todo.txt");
        string item;
        while (getline(fin, item)) {
            pending++;
        }
        fin.close();
    }

    if (number > pending || number <= 0) {
        cout << "Error: todo #" << number << " does not exist. Nothing deleted.";
        return;
    }

    remove_item_from_file(number, false);
    cout << "Deleted todo #" << number;
}

void done_item(int number) {

    int pending = 0;

    if ((static_cast<bool>(std::ifstream("todo.txt")))) {
        ifstream fin;
        fin.open("todo.txt");
        string item;
        while (getline(fin, item)) {
            pending++;
        }
        fin.close();
    }

    if (number > pending || number <= 0) {
        cout << "Error: todo #" << number << " does not exist.";
        return;
    }

    remove_item_from_file(number, true);
    cout << "Marked todo #" << number << " as done.";
}

void display_items(void) {

    if (static_cast<bool>(std::ifstream("todo.txt"))) {

        ifstream fin;
        fin.open("todo.txt");
        string item;
        vector<string> items;

        while (getline(fin, item)) {
            items.push_back(item);
        }

        fin.close();

        if (items.empty()) {
            cout << "There are no pending todos!";
        }

        for (int i = items.size() - 1; i >= 0; i--) {
            cout << "[" << i+1 << "]" << " " << items[i] << "\n";
        }
    }

    else {
        cout << "There are no pending todos!";
    }
}

void display_stats(void) {
    int pending = 0, completed = 0;

    if ((static_cast<bool>(std::ifstream("todo.txt")))) {
        ifstream fin;
        fin.open("todo.txt");
        string item;
        while (getline(fin, item)) {
            pending++;
        }
        fin.close();
    }

    if ((static_cast<bool>(std::ifstream("done.txt")))) {
        ifstream fin;
        fin.open("done.txt");
        string item;
        while (getline(fin, item)) {
            completed++;
        }
        fin.close();
    }

    cout << get_curr_date() << " Pending : " << pending << " Completed " << completed;
}

void display_usage(void) {

    cout << "Usage :-\n";
    cout << "$ ./todo add \"todo item\"  # Add a new todo\n";
    cout << "$ ./todo ls               # Show remaining todos\n";
    cout << "$ ./todo del NUMBER       # Delete a todo\n";
    cout << "$ ./todo done NUMBER      # Complete a todo\n";
    cout << "$ ./todo help             # Show usage\n";
    cout << "$ ./todo report           # Statistics";
}

string get_curr_date(void) {

    time_t now = time(0);
    tm *ltm = localtime(&now);

    string curr_time = "";
    curr_time += (to_string(1900 + ltm->tm_year) + "-");
    curr_time += (to_string(1 + ltm->tm_mon) + "-");
    curr_time += to_string(ltm->tm_mday);

    return curr_time;
}

void remove_item_from_file(int number, bool isCompleted) {

    ifstream fin;
    fin.open("todo.txt");
    string item;
    vector<string> items;

    while (getline(fin, item)) {
        items.push_back(item);
    }
    
    fin.close();
    ofstream fout, dout;
    fout.open("todo.txt");
    dout.open("done.txt", ios::app);

    int items_size = items.size();
    for (int i = 0; i < items_size; i++) {
        if (i+1 != number) {
            fout << items[i] << "\n";
        }
        else if (isCompleted) {
            dout << "x " << get_curr_date() << " " << items[i] << "\n";
        }
    }

    fout.close();
    dout.close();
}

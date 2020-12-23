#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
 
using namespace std;
 
void todo_usage(void) {
 
    cout << "Usage :-\n";
    cout << "$ ./todo add \"todo item\"  # Add a new todo\n";
    cout << "$ ./todo ls               # Show remaining todos\n";
    cout << "$ ./todo del NUMBER       # Delete a todo\n";
    cout << "$ ./todo done NUMBER      # Complete a todo\n";
    cout << "$ ./todo help             # Show usage\n";
    cout << "$ ./todo report           # Statistics";
}
 
string return_curr_date(void) {
 
    time_t now = time(0);
    tm *ltm = localtime(&now);
    string curr_time = "";
    curr_time += (to_string(1900 + ltm->tm_year) + "-");
    curr_time += (to_string(1 + ltm->tm_mon) + "-");
    curr_time += to_string(ltm->tm_mday);
    return curr_time;
}
 
void modify_stats(int pending = 0, int completed = 0) {
 
    if (static_cast<bool>(std::ifstream("mystats.txt"))) {
 
        ifstream fin;
        fin.open("mystats.txt");
        string number;
        int stats[2] = {0};
        int i = 0;
        while (getline(fin, number)) {
            stats[i++] = stoi(number);
        }
        fin.close();
        stats[0] += pending;
        stats[1] += completed;
 
        ofstream fout;
        fout.open("mystats.txt");
        fout << to_string(stats[0]);
        fout << "\n";
        fout << to_string(stats[1]);
        fout.close();
    }
 
    else {
 
        ofstream fout;
        fout.open("mystats.txt");
        fout << to_string(pending);
        fout << "\n";
        fout << to_string(completed);
        fout.close();
    }
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
 
    // modify stats
    modify_stats(1, 0);
 
    // print item
    cout << "Added todo: \"" << item << "\"";
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
 
void remove_item_from_file(int number, bool isCompleted = false) {
 
    if (!(static_cast<bool>(std::ifstream("mystats.txt")))) {
        return;
    }
 
    ifstream fin;
    fin.open("mystats.txt");
    int pending;
    fin >> pending;
    fin.close();
 
    fin.open("todo.txt");
    string item;
    vector<string> items;
 
    while (getline(fin, item)) {
        items.push_back(item);
    }
 
    fin.close();
    ofstream fout, dout;
    fout.open("todo.txt");
    dout.open("done.txt");
 
    for (int i = 0; i < pending; i++) {
        if (i+1 != number) {
            fout << items[i] << "\n";
        }
        else if (isCompleted) {
            dout << "x " << return_curr_date() << " " << items[i] << "\n";
        }
    }
}
 
void delete_item(int number) {
 
    if (!(static_cast<bool>(std::ifstream("mystats.txt")))) {
        cout << "Error: todo #" << number << " does not exist. Nothing deleted.";
        return;    
    }
 
    ifstream fin;
    fin.open("mystats.txt");
    int pending;
    fin >> pending;
    fin.close();
 
    if (number > pending || number <= 0) {
        cout << "Error: todo #" << number << " does not exist. Nothing deleted.";
        return;
    }
 
    remove_item_from_file(number);
    cout << "Deleted todo #" << number;
    modify_stats(-1, 0);
}
 
void done_item(int number) {
 
    if (!(static_cast<bool>(std::ifstream("mystats.txt")))) {
        cout << "Error: todo #" << number << " does not exist";
        return;    
    }
 
    ifstream fin;
    fin.open("mystats.txt");
    int pending;
    fin >> pending;
    fin.close();
 
    if (number > pending || number <= 0) {
        cout << "Error: todo #" << number << " does not exist.";
        return;
    }
 
    remove_item_from_file(number, true);
    cout << "Marked todo #" << number << " as done.";
    modify_stats(-1, 1);
}
 
void display_stats(void) {
 
    int stats[2] = {0};
 
    if (static_cast<bool>(std::ifstream("mystats.txt"))) {
 
        ifstream fin;
        fin.open("mystats.txt");
        string number;
        int i = 0;
        while (getline(fin, number)) {
            stats[i++] = stoi(number);
        }
        fin.close();
    }
 
    cout << return_curr_date() << " Pending : " << stats[0] << " Completed : " << stats[1];   
}
 
int main(int argc, char* argv[])
{
    if (argc == 1) {
 
        todo_usage();
    }
 
    else {
 
        if (argv[1][0] == 'a') {
 
            if (argc == 2) {
                cout << "Error: Missing todo string. Nothing added!";
            }
            else {
                add_item(argv[2]);
            }
        }
 
        else if (argv[1][0] == 'l') {
 
            display_items();
        }
 
        else if (argv[1][0] == 'd' && argv[1][1] == 'e') {
 
            if (argc == 2) {
                cout << "Error: Missing NUMBER for deleting todo.";
            }
            else {
                int number = stoi(argv[2]);
                delete_item(number);
            }
        }
 
        else if (argv[1][0] == 'd' && argv[1][1] == 'o') {
 
            if (argc == 2) {
                cout << "Error: Missing NUMBER for marking todo as done.";
            }
            else {
                int number = stoi(argv[2]);
                done_item(number);
            }
        }
 
        else if (argv[1][0] == 'h') {
 
            todo_usage();
        }
 
        else if (argv[1][0] == 'r') {
 
            display_stats();
        }
    }
 
    return 0;
}

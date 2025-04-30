#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

using namespace std;

void validate_integer_input(int &num) {
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter an integer.\n";
        num = 0;
    }
}

enum Data_Types {
    db_int,
    db_bool,
    db_string
};

class Field {
  public:
    string name;
    Data_Types data_type;

    Field() {
    }

    Field(string field_name) {
        name = field_name;

        cout << "Choose field data type (1. int, 2. string, 3. bool)" << endl;
        int selection = -1;

        while (selection == -1) {
            cout << "Choose selection (1/2/3): ";
            cin >> selection;

            validate_integer_input(selection);

            if (selection == 1)
                data_type = db_int;
            else if (selection == 2)
                data_type = db_string;
            else if (selection == 3)
                data_type = db_bool;
            else {
                selection = -1;
                cout << "invlaid selection" << endl;
            }
        }
    }
};

class Entry {
  public:
    int entry_size = 0;

    int id = 0;
    vector<variant<int, bool, string>> data;

    Entry(int entry_id, vector<Field> &fields) {
        entry_size = fields.size();
        data.resize(entry_size);

        id = entry_id;

        for (int i = 0; i < entry_size; i++) {
            if (fields[i].data_type == db_int) {
                int value;
                cout << "Enter value for field " << fields[i].name << " (int): ";
                cin >> value;
                data[i] = value;
            } else if (fields[i].data_type == db_string) {
                string value;
                cout << "Enter value for field " << fields[i].name << " (string): ";
                cin >> value;
                data[i] = value;
            } else if (fields[i].data_type == db_bool) {
                bool value;
                cout << "Enter value for field" << fields[i].name << " (bool): ";
                cin >> value;
                data[i] = value;
            }
        }

        cout << endl << endl;
    }
};

class Table {
  public:
    string table_name;

    int no_of_fields = 0;
    vector<Field> field_list;
    unordered_set<string> field_names_list;

    int no_of_entries = 0;
    vector<Entry> entries;

    void print_table() {
        cout << "-----------------------------------------" << endl;
        for (auto &field : field_list) {
            cout << field.name << "\t |";
        }
        cout << endl;

        for (auto &entry : entries) {
            for (auto &value : entry.data) {
                visit( [](auto &&arg) {
                        if constexpr (is_same_v<decltype(arg), bool>)
                            cout << (arg ? "true" : "false") << "\t |";
                        else
                            cout << arg << "\t |";
                    },value);
            }

            cout << endl;
        }

        cout << "-----------------------------------------" << endl;
    }

    void add_entry() {
        entries.push_back(Entry(no_of_entries, field_list));
    }

    Table(string table_name) {
        this->table_name = table_name;

        while (no_of_fields == 0) {
            cout << "Enter number of fields: ";
            cin >> no_of_fields;

            validate_integer_input(no_of_fields);
        }

        field_list.resize(no_of_fields);
        for (int i = 0; i < no_of_fields; i++) {
            string field_name;

            while (field_name.empty()) {
                cout << "Enter name for the field " << i + 1 << ": ";
                cin >> field_name;

                if (field_names_list.insert(field_name).second == false) {
                    cout << "Field name already in use" << endl;
                    field_name = "";
                }
            }

            field_list[i] = Field(field_name);
        }

        cout << endl << endl << "Table " << table_name << " created successfully!" << endl;
    }
};

int main() {
    unordered_map<string, int> table_names;
    vector<Table> tables;
    int table_id = 0;

    while (true) {

        int selection = -1;

        cout << "1. View List of Tables \n2. Make a Table \n3. View a Table \n4. Add entries to a Table \n5. Exit "
                "\n\nEnter your Selection(1/2/3/4/5): ";

        cin >> selection;
        validate_integer_input(selection);

        cout << endl;

        if (selection == 1) {
            cout << "These are the following tables in the system: " << endl;
            cout << "----------------------------------------------" << endl;

            for (auto table : tables)
                cout << table.table_name << endl;

            cout << "----------------------------------------------" << endl << endl;
        }

        else if (selection == 2) {
            string table_name;

            while (table_name.empty()) {
                cout << "Enter Table Name: ";
                cin >> table_name;
                if (table_names.count(table_name)) {
                    cout << "Invalid name, name already exists" << endl;
                    table_name = "";
                }
            }

            Table table(table_name);
            tables.push_back(table);
            table_names[table_name] = table_id;
            table_id++;
        }

        else if (selection == 3) {
            string table_name;

            cout << "Enter table name to be viewed: ";
            cin >> table_name;

            if (!table_names.count(table_name))
                cout << "This table doesn't exists";

            else
                tables[table_names[table_name]].print_table();
        }

        else if (selection == 4) {
            string table_name;

            cout << "Enter table name to add entry: ";
            cin >> table_name;

            if (!table_names.count(table_name))
                cout << "This table doesn't exists";

            else
                tables[table_names[table_name]].add_entry();
        }

        else if (selection == 5) {
            // save these changes somewhere and make it easily accessible even after the program is killed.
            cout << "Saving the changes and exiting the system";
            break;
        }

        else {
            cout << "Invalid selection!!" << endl << endl;
        }
    }
}
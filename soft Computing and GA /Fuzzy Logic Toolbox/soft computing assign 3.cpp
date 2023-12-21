#include <iostream>
#include <string>
#include <vector>
#define line cout << "_________________________________________________" << endl;
#define ll cout<<endl<<"____"<<endl;
#define stop cout << "\n(Press x to finish)\n";
using namespace std;
// Struct definitions
struct system_info {
    string name;
    string brief_description;
};
struct variable {
    string name;
    string type;
    double lower_bound;
    double upper_bound;
};
struct fuzzy_set {
    variable var;
    string name;
    string type;
    vector<double> values;
};
struct fuzzy_rule {
    variable in_var1;
    variable in_var2;
    fuzzy_set in_set1;
    fuzzy_set in_set2;
    string op;
    variable out_var;
    fuzzy_set out_set;
};
struct FuzzificationResult {
    string variable_name;
    string fuzzy_set_name;
    double degree_of_membership;
};
struct Rule_result {
    fuzzy_set output_set;
    double degree_of_membership;
    variable output_var;
};
void show_menu() {
    cout << "Main Menu:" << endl;
    cout << "==========" << endl;
    cout << "1- Add variables." << endl;
    cout << "2- Add fuzzy sets to an existing variable." << endl;
    cout << "3- Add rules." << endl;
    cout << "4- Run the simulation on crisp values." << endl;
    cout << "0- QUIT" << endl;
}
system_info set_info() {
    system_info info;
    cout << "Enter the system’s name and a brief description" << endl;
    cout << "Name: ";
    getline(cin, info.name);
    cout << "Brief Description: ";
    getline(cin, info.brief_description);
    return info;
}
bool in_vars(vector<variable>vars) {
    for (auto it : vars) {
        if (it.type == "IN")
            return true;
    }
    return false;
}
bool out_vars(vector<variable>vars) {
    for (auto it : vars) {
        if (it.type == "OUT")
            return true;
    }
    return false;
}
bool var_exists(vector<variable> vars, string name) {
    for (auto it : vars) {
        if (it.name == name)
            return true;
    }
    return false;
}
bool set_exists(vector<fuzzy_set> sets, string name) {
    for (auto it : sets) {
        if (it.name == name)
            return true;
    }
    return false;
}
variable search_for_variable(vector<variable>v, string name) {
    for (auto it : v) {
        if (name == it.name)
            return it;
    }
    return variable();
}
double search_for_membership(vector< FuzzificationResult> results, variable var,fuzzy_set set ) {
    for (auto result : results) {
        if (var.name == result.variable_name && set.name == result.fuzzy_set_name)
            return result.degree_of_membership;
    }
    return 0;
}
fuzzy_set search_for_set(vector<fuzzy_set>v, string name) {
    for (auto it : v) {
        if (name == it.name)
            return it;
    }
    return fuzzy_set();
}
double get_centriod(fuzzy_set set) {
    double sum = 0;
    for (auto pnt : set.values) {
        sum += pnt;
    }
    return (sum / set.values.size());
}
void get_info(const system_info& info) {
    cout << "System Information:" << endl;
    cout << "Name: " << info.name << endl;
    cout << "Brief Description: " << info.brief_description << endl;
}
void get_vars(const vector<variable>& variables) {
    cout << "\n System Variables \n";
    for (const auto& it : variables) {
        cout << it.name << "\t" << it.type << "\t[" << it.lower_bound << "," << it.upper_bound << "]\n";
    }
    line;
}
void get_fuzzy_set(const vector<fuzzy_set>& sets) {
    cout << "\n System Fuzzy Sets \n";

    for (const auto& it : sets) {
        cout << it.name << "\t" << it.type << "\t";
        cout << "related variable: " << it.var.name << "\t";
        cout << "values: ";

        for (const auto& value : it.values) {
            cout << value << " ";
        }

        cout << endl;
    }
    line;
}
void get_fuzzy_rule(const vector<fuzzy_rule>& rules) {
    cout << "\nSystem rules\n";
    for (const auto& rule : rules) {
        cout << rule.in_var1.name << ' ' << rule.in_set1.name << ' ' << rule.op << ' ' <<
            rule.in_var2.name << ' ' << rule.in_set2.name << " => " << rule.out_var.name << ' ' << rule.out_set.name << endl;
    }
    line;
}
vector<variable> set_vars() {
    stop;
    vector<variable> variables;
    variable var;
    while (true) {
        cout << "Enter the variable’s name: ";
        cin >> var.name;
        if (var.name == "x") {
            break;
        }
        cout << "Enter the variable's type (IN or OUT): ";
        cin >> var.type;
        while (var.type != "IN" && var.type != "OUT") {
            cout << "Invalid type! Please enter IN or OUT: ";
            cin >> var.type;
        }
        cout << "Enter the variable's range:\n";
        cout << "Lower bound: ";
        cin >> var.lower_bound;
        cout << "Upper bound: ";
        cin >> var.upper_bound;
        while (var.upper_bound < var.lower_bound) {
            cout << "Lower bound cannot be higher than upper bound. Enter them again.\n";
            cout << "Lower bound: ";
            cin >> var.lower_bound;
            cout << "Upper bound: ";
            cin >> var.upper_bound;
        }
        ll;
        variables.push_back(var);
    }
    return variables;
}
vector<fuzzy_set> set_fuzzy_set(const vector<variable>& variables) {
    stop;
    vector<fuzzy_set> sets;
    string var_name;
    while (true) {
        stop;
        cout << "Enter the variable’s name: ";
        cin >> var_name;

        if (var_name == "x") {
            break;
        }

        while (!var_exists(variables, var_name)) {
            cout << "\nVariable does not exist! Enter another name.\n";
            cout << "Here is the list of variables that exist:\n";
            get_vars(variables);
            cin >> var_name;
        }

        fuzzy_set set;
        for (const auto& it : variables) {
            if (it.name == var_name) {
                set.var = it;
                break;
            }
        }

        while (true) {
            stop;
            cout << "Enter Fuzzy Set name: ";
            cin >> set.name;
            if (set.name == "x") {
                break;
            }
            cout << "Enter Fuzzy Set type (TRI/TRAP): ";
            cin >> set.type;
            while (set.type != "TRI" && set.type != "TRAP") {
                cout << "Allowed types are only TRI/TRAP\n";
                cout << "Enter Fuzzy Set type (TRI/TRAP): ";
                cin >> set.type;
            }
            cout << "Enter Fuzzy Values: ";
            if (set.type == "TRI") {
                double x1, x2, x3;
                cin >> x1 >> x2 >> x3;
                set.values = { x1, x2, x3 };
            }
            else if (set.type == "TRAP") {
                double x1, x2, x3, x4;
                cin >> x1 >> x2 >> x3 >> x4;
                set.values = { x1, x2, x3, x4 };
            }
            ll
                sets.push_back(set);
        }
    }
    return sets;
}
vector<fuzzy_rule> set_fuzzy_rule(const vector<variable>& variables, const vector<fuzzy_set>& sets) {
    stop;
    cout << "Enter the rules in this format:\n";
    cout << "\n\nIN_variable set operator IN_variable set => OUT_variable set\n\n";
    vector<fuzzy_rule> rules;
    while (true) {
        fuzzy_rule rule;
        string x;
        variable var;
        fuzzy_set set;
        cout << "Enter input variable 1: ";
        cin >> x;
        if (x == "x") {
            break;
        }
        while (!var_exists(variables, x)) {
            cout << "\nVariable does not exist! Enter another name.\n";
            cout << "Here is the list of variables that exist:\n";
            get_vars(variables);
            cout << "Enter input variable 1: ";
            cin >> x;
        }
        rule.in_var1 = search_for_variable(variables, x);
        x = "\0"; // clear buffer
        cout << "Enter fuzzy set for variable 1: ";
        cin >> x;
        while (!set_exists(sets, x)) {
            cout << "\Set does not exist! Enter another name.\n";
            cout << "Here is the list of Sets that exist:\n";
            get_fuzzy_set(sets);
            cout << "Enter fuzzy set for variable 1: ";
            cin >> x;
        }
        set = search_for_set(sets, x);
        rule.in_set1 = set;
        x = "\0"; // clear buffer

        cout << "Enter operator (and/or/and_not/or_not): ";
        cin >> rule.op;

        cout << "Enter input variable 2: ";
        cin >> x;

        while (!var_exists(variables, x)) {
            cout << "\nVariable does not exist! Enter another name.\n";
            cout << "Here is the list of variables that exist:\n";
            get_vars(variables);
            cout << "Enter input variable 2: ";
            cin >> x;
        }
        var = search_for_variable(variables, x);
        rule.in_var2 = var;
        x = "\0"; // clear buffer

        cout << "Enter fuzzy set for variable 2: ";
        cin >> x;

        while (!set_exists(sets, x)) {
            cout << "\Set does not exist! Enter another name.\n";
            cout << "Here is the list of Sets that exist:\n";
            get_fuzzy_set(sets);
            cout << "Enter fuzzy set for variable 1: ";
            cin >> x;
        }

        set = search_for_set(sets, x);
        rule.in_set2 = set;
        x = "\0"; // clear buffer

        cout << "Enter output variable : ";
        cin >> x;

        while (!var_exists(variables, x)) {
            cout << "\nVariable does not exist! Enter another name.\n";
            cout << "Here is the list of variables that exist:\n";
            get_vars(variables);
            cout << "Enter output variable : ";
            cin >> x;
        }

        var = search_for_variable(variables, x);
        rule.out_var = var;
        x = "\0"; // clear buffer

        cout << "Enter fuzzy set for output variable : ";
        cin >> x;

        while (!set_exists(sets, x)) {
            cout << "\Set does not exist! Enter another name.\n";
            cout << "Here is the list of Sets that exist:\n";
            get_fuzzy_set(sets);
            cout << "Enter fuzzy set for output variable : ";
            cin >> x;
        }

        set = search_for_set(sets, x);
        rule.out_set = set;

        cout << "\n Rule: " << rule.in_var1.name << ' ' << rule.in_set1.name << ' ' << rule.op << ' ' <<
            rule.in_var2.name << ' ' << rule.in_set2.name << " => " << rule.out_var.name << ' ' << rule.out_set.name;
        ll
            rules.push_back(rule);
    }
    line;
    return rules;
}
// 1. Fuzzification
double degree_of_membership_TRI(double a, double b, double c, double crisp_value) {
    if (crisp_value <= a || crisp_value >= c) {
        return 0.0;
    }
    else if (crisp_value <= b) {
        return (crisp_value - a) / (b - a);
    }
    else {
        return (c - crisp_value) / (c - b);
    }
}
double degree_of_membership_TRAP(double a, double b, double c, double d, double crisp_value) {
    if (crisp_value <= a || crisp_value >= d) {
        return 0.0;
    }
    else if (crisp_value >= b && crisp_value <= c) {
        return 1.0;
    }
    else if (crisp_value > a && crisp_value < b) {
        return (crisp_value - a) / (b - a);
    }
    else {
        return (d - crisp_value) / (d - c);
    }
}
vector<FuzzificationResult> fuzzification(const vector<pair<string, double>>& crisp_values, const vector<fuzzy_set>& sets) {
    vector<FuzzificationResult> results;
    for (const auto& crisp_value : crisp_values) {
        for (const auto& fuzzy_set : sets) {
            if (fuzzy_set.var.name == crisp_value.first) {
                double membership;
                if (fuzzy_set.type == "TRI") {
                    membership = degree_of_membership_TRI(
                        fuzzy_set.values[0], fuzzy_set.values[1], fuzzy_set.values[2], crisp_value.second);
                }
                else if (fuzzy_set.type == "TRAP") {
                    membership = degree_of_membership_TRAP(
                        fuzzy_set.values[0], fuzzy_set.values[1], fuzzy_set.values[2], fuzzy_set.values[3],
                        crisp_value.second);
                }
                else {
                    cout << "Unsupported fuzzy set type.\n";
                    continue;
                }
                results.push_back({ crisp_value.first, fuzzy_set.name, membership });
            }
        }
    }
    return results;
}
// 2. Rule inference
vector<Rule_result> rule_inference(const vector<fuzzy_rule>& rules,
    const vector<FuzzificationResult>& fuzzification_results,
    const vector<fuzzy_set>& sets) {
    vector<Rule_result> results;
    for (const auto& rule : rules) {
        Rule_result result;
        result.output_var = rule.out_var;
        result.output_set = rule.out_set;
        if (rule.op == "and") {
            double membership_var1 = search_for_membership(fuzzification_results, rule.in_var1, rule.in_set1);
            double membership_var2 = search_for_membership(fuzzification_results, rule.in_var2, rule.in_set2);
            result.degree_of_membership = min(membership_var1, membership_var2);
        }
        else if (rule.op == "and_not") {
            double membership_var1 = search_for_membership(fuzzification_results, rule.in_var1, rule.in_set1);
            double membership_var2 = search_for_membership(fuzzification_results, rule.in_var2, rule.in_set2);
            result.degree_of_membership = min(membership_var1, (1 - membership_var2));
        }
        else if (rule.op == "or") {
            double membership_var1 = search_for_membership(fuzzification_results, rule.in_var1, rule.in_set1);
            double membership_var2 = search_for_membership(fuzzification_results, rule.in_var2, rule.in_set2);
            result.degree_of_membership = max(membership_var1, membership_var2);
        }
        else if (rule.op == "or_not") {
            double membership_var1 = search_for_membership(fuzzification_results, rule.in_var1, rule.in_set1);
            double membership_var2 = search_for_membership(fuzzification_results, rule.in_var2, rule.in_set2);
            result.degree_of_membership = max(membership_var1, (1 - membership_var2));
        }
        results.push_back(result);
       
    }
    return results;
}
// 3. Defuzzification (Weighted Average Method)
double defuzzification_weighted_average(vector<Rule_result> rule_inference_results) {
    double weighted_sum = 0.0;
    double total_weight = 0.0;
    for (auto res : rule_inference_results) {
        //skip the 0 memberships
        if (res.degree_of_membership == 0)
            continue;
        double centroid = get_centriod(res.output_set);
        weighted_sum += (centroid * res.degree_of_membership);
        total_weight += res.degree_of_membership;
    }
    return (total_weight == 0) ? 0 : weighted_sum / total_weight;
}
string defuzzification_set(double crisp_output, vector<Rule_result> rule_inference_results) {
    for (auto set : rule_inference_results) {
        int size = set.output_set.values.size();
        if (crisp_output >= set.output_set.values[0] && crisp_output <= set.output_set.values[size - 1])
            return set.output_set.name;
    }
    return "value out of range\n";
}
int main() {
    cout << "Fuzzy Logic Toolbox\n===================" << endl;
    while (true) {
        cout << "1- Create a new fuzzy system\n";
        cout << "0- Quit\n";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 0) {
            cout << "Quitting..." << endl;
            break;
        }
        else if (choice == 1) {
            system_info info = set_info();
            vector<variable> variables;
            vector<fuzzy_set> sets;
            vector<fuzzy_rule> rules;
            bool vars_done = 0, sets_done = 0, rules_done = 0;
            while (true) {
                show_menu();
                int menu_choice;
                cin >> menu_choice;
                if (menu_choice == 1) {
                    variables = set_vars();
                    get_vars(variables);
                    vars_done = true;
                }
                else if (menu_choice == 2) {
                    sets = set_fuzzy_set(variables);
                    get_fuzzy_set(sets);
                    sets_done = true;
                }
                else if (menu_choice == 3) {
                    rules = set_fuzzy_rule(variables, sets);
                    get_fuzzy_rule(rules);
                    rules_done = true;
                }
                else if (menu_choice == 4) {
                    if (!rules_done || !vars_done || !sets_done) {
                        cout << "Can't run Simulation" << endl;
                    }
                    if (!vars_done) {
                        cout << "\nAdd some variables first!\n";
                    }
                    if (!sets_done) {
                        cout << "\nAdd some sets first!\n";
                    }
                    if (!rules_done) {
                        cout << "\nAdd some rules first!\n";
                    }
                    if (rules_done && vars_done && sets_done) {
                        cout << "\nEnter the crisp values:\n";
                        vector<pair<string, double>> crisp_values;
                        for (const auto& variable : variables) {
                            if (variable.type == "IN") {
                                cout << variable.name << ": ";
                                double val;
                                cin >> val;
                                crisp_values.push_back(make_pair(variable.name, val));
                            }
                        }
                        cout << "Running the simulation" << endl;
                        vector<FuzzificationResult> fuzzification_results = fuzzification(crisp_values, sets);
                        line;
                        cout << "var name \t degree_of_membership \t fuzzy_set_name\t\n";
                        for (auto it : fuzzification_results) {
                            cout << it.variable_name << "\t";
                            cout << it.degree_of_membership << "\t";
                            cout << it.fuzzy_set_name << "\n";
                        }
                        line;
                        vector<Rule_result> rule_inference_results = rule_inference(rules, fuzzification_results, sets);
                        line;
                        cout << "var output_var \t output_set \t degree_of_membership\t\n";
                        for (auto it : rule_inference_results) {
                            cout << it.output_var.name << "\t";
                            cout << it.output_set.name << "\t";
                            cout << it.degree_of_membership << "\n";
                        }
                        line;
                        double crisp_output_value = defuzzification_weighted_average(rule_inference_results);
                        string crisp_output_set = defuzzification_set(crisp_output_value, rule_inference_results);
                        cout << "Fuzzification => done " << endl;
                        cout << "Inference => done " << endl;
                        cout << "Defuzzification => done" << endl;
                        cout << "The predicted value is " << crisp_output_set << " (" << crisp_output_value << ")\n";
                    }
                }
                else if (menu_choice == 0) {
                    cout << "Quitting...." << endl;
                    break;
                }

            }
        }
        else {
            cout << "Invalid choice. Please enter a valid option.\n";
        }
    }
    cout << "Quitting...." << endl;
    return 0;
}


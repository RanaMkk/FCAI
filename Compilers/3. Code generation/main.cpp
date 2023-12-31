#include <iostream>
#include "utility.cpp"
using namespace std;
//utility func to print
void PrintSyntaxTree(TreeNode* node, OutFile& outFile, int sh = 0)
{
    int i, NSH = 3;
    for (i = 0; i < sh; i++) fprintf(outFile.file, " ");

    fprintf(outFile.file, "[%s]", NodeKindStr[node->node_kind]);

    if (node->node_kind == OPER_NODE) fprintf(outFile.file, "[%s]", TokenTypeStr[node->oper]);
    else if (node->node_kind == NUM_NODE) fprintf(outFile.file, "[%d]", node->num);
    else if (node->node_kind == ID_NODE || node->node_kind == READ_NODE || node->node_kind == ASSIGN_NODE)
        fprintf(outFile.file, "[%s]", node->id);

    if (node->expr_data_type != VOID) fprintf(outFile.file, "[%s]", ExprDataTypeStr[node->expr_data_type]);

    fprintf(outFile.file, "\n");

    for (i = 0; i < MAX_CHILDREN; i++) if (node->child[i]) PrintSyntaxTree(node->child[i], outFile, sh + NSH);
    if (node->sibling) PrintSyntaxTree(node->sibling, outFile, sh);
}
void PrintSymbolTable(SymbolTable& symbolTable,OutFile& outFile)
{
    int i;
    for (i = 0; i < SYMBOL_HASH_SIZE; i++)
    {
        VariableInfo* curv = symbolTable.var_info[i];
        while (curv)
        {
            fprintf(outFile.file, "[Var=%s][Mem=%d]", curv->name, curv->memloc);
            LineLocation* curl = curv->head_line;
            while (curl)
            {
                fprintf(outFile.file, "[Line=%d]", curl->line_num);
                curl = curl->next;
            }
            fprintf(outFile.file, "\n");
            curv = curv->next_var;
        }
    }
}
// Step 1: Initializing a SymbolTable
void fill_symbol_table(TreeNode* root, SymbolTable& symbolTable)
{
    if (!root)
        return;
    if (root->node_kind == ID_NODE ||root->node_kind == READ_NODE || root->node_kind==ASSIGN_NODE)
    {
        symbolTable.Insert(root->id, root->line_num);
    }
    for (auto & i : root->child)
    {
        fill_symbol_table(i, symbolTable);
    }

    fill_symbol_table(root->sibling, symbolTable);
}
// Step 2: Assign Node Types
void assign_types(TreeNode* root)
{
    //if the root node is null then stop and return
    if (!root)
        return;
    //if the kind of the node is a number or an id then it's an integer
    if (root->node_kind == NUM_NODE || root->node_kind == ID_NODE)
    {
        root->expr_data_type = INTEGER;
    }
    //if the kind of the node is less than operator or an equal then it's a boolean else then it's a
    //mathematical expression and takes an integer
    else if(root->node_kind == OPER_NODE){
        if(root->oper == EQUAL || root->oper == LESS_THAN){
            root->expr_data_type = BOOLEAN;
        }else{
            root->expr_data_type = INTEGER;
        }
    }
    //All other statements are a void type
    else{
        root->expr_data_type = VOID;
    }
    //recursively do the same for the children
    for (auto & i : root->child)
    {
        assign_types(i);
    }
    //once done with the children, recursively do the same for the sibling(DFS technique)
    assign_types(root->sibling);
}
// Step 3: Type Check
void type_checking(TreeNode* root)
{
    // a boolean variable to check if there is any problem with the code
    bool check= true;
    // the message od the error that may occur
    string msg = " ";
    if (!root)
        return;
    //check that each 'if' has a 'bool' child
    if (root->node_kind == IF_NODE) {
            if(root->child[0]->expr_data_type == BOOLEAN ) {
                check = true;
        }
        check = false;
        msg += "\n error in line ";
        msg += to_string(root->line_num);
        msg +=" the (if statement should be followed by a condition)\n";
    }
    //check that each 'assign' has a 'int' child
    if(root ->node_kind == ASSIGN_NODE){
            if(root->child[0]->expr_data_type == INTEGER ) {
                check = true;
        }
        check = false;
        msg += "\n error in line ";
        msg += to_string(root->line_num);
        msg +=" the (assign statement should be followed by a value)\n";
    }
    if(!check){
        //recursively do the same for the children
        for (auto & i : root->child)
        {
            type_checking(i);
        }
        //once done with the children, recursively do the same for the sibling(DFS technique)
        type_checking(root->sibling);
    }else{
        cout<<msg<<endl;
    }

}
///////////// functions to simulate the compilation process /////////////
void run_code(TreeNode* root,SymbolTable& symbolTable,OutFile& out);
// 1. executing the expression statement ( foundation of most executions)
int exe_expression_statement (TreeNode* root, SymbolTable& symbolTable){
    // an expression statement can be an identifier or a number
    // or an operation, but it will execute it and return an integer
    // as a result in all cases
    int result = 0;
    // if it's an ID node then the result is the value of the identifier
    if (root->node_kind == ID_NODE) {
        // use the find function in the symbol table to return its value
        result =  symbolTable.Find(root->id)->value;
    }
        // if it's a number node then the result is the number itself
    else if (root->node_kind == NUM_NODE) {
        result =  root->num;
    }
        // if it's an operation of any kind
        // recursively execute its children first
    else if (root->node_kind == OPER_NODE) {
        int first_operand = exe_expression_statement(root->child[0], symbolTable);
        int second_operand = exe_expression_statement(root->child[1], symbolTable);
        if (root->oper == POWER) {
            // the power operation
            result = (int) pow(second_operand, first_operand);
        }
        else if (root->oper == PLUS){
            // the sum operation
            result = first_operand + second_operand;
        }
        else if (root->oper == MINUS) {
            // the subtraction operation
            result = first_operand - second_operand;
        }
        else if (root->oper == EQUAL){
            // the equality check (only returns 0 or 1)
            result = (first_operand == second_operand);
        }
        else if (root->oper == LESS_THAN) {
            // the less than check (only returns 0 or 1)
            result = (first_operand < second_operand);
        }
        else if (root->oper == TIMES){
            // the multiplication operation
            result = first_operand * second_operand;
        }
        else if ( root->oper == DIVIDE){
            // the division operations
            result = first_operand / second_operand;
        }
    }
    return  result;
}
// 2. executing the read statement
void exe_read_statement(const char* variable_name, SymbolTable& symbolTable) {
    int value;
    //ask the user to input the value
    cout << "Enter the value of " << variable_name << ": ";
    cin >> value;
    //update the value of this variable in the table
    symbolTable.updateVarValue(variable_name, value);
}
// 3. executing the assignment statement
void exe_assign_statement (TreeNode* root , SymbolTable& symbolTable){
    char* variable = root->id;
    // assigning the new value to the table
    int value = exe_expression_statement(root->child[0], symbolTable);
    symbolTable.updateVarValue(variable, value);
}
// 4. executing the write statement
void exe_write_statement(TreeNode* node , SymbolTable& symbolTable, OutFile& out) {
    // outputting the child of the write statement (execution result)
    cout << exe_expression_statement(node->child[0], symbolTable) << endl;
    // outputting the same inside a file
    out.Out("\n result: ");
    int result = exe_expression_statement(node->child[0], symbolTable);
    string  output_result = to_string(result);
    out.Out(output_result);
    out.Out("\n");
}
// 5. executing the repeat statement
void exe_repeat_statement(TreeNode* node, SymbolTable& symbolTable,OutFile& out) {
    // keep running the whole block of code until the child condition executed
    do {
        run_code(node->child[0], symbolTable,out);
    } while (!exe_expression_statement(node->child[1], symbolTable));
}
// 6. executing the 'if' condition statement
bool exe_if_statement(TreeNode* root, SymbolTable& symbolTable,OutFile& out) {
    // evaluate the condition to see if we're going to run the true part or the false part
    int truth = exe_expression_statement(root->child[0], symbolTable);
    // check if the condition is false
    if (!truth) {
        // If there is an else part execute it
        if (root->child[2]) {
            run_code(root->child[2],symbolTable,out);
            return true;
        }
            // If there is no else part then return false
        else {
            return false;
        }
    }
        // If the condition is evaluated to true, execute it and return true
    else {
        run_code(root->child[1],symbolTable,out);
        return true;
    }
}
// Step 4: Run the Code
void run_code(TreeNode* root, SymbolTable& symbolTable, OutFile& out) {
    // iterate over the parse tree and execute each node by the corresponding execution func
    int i = 0;
    if (root->node_kind == IF_NODE) {
        int truth = exe_if_statement(root, symbolTable,out);
        if (!truth){
            i = MAX_CHILDREN;
        }
        else return;
    }
    else if (root->node_kind == READ_NODE){
        exe_read_statement(root->id , symbolTable);
    }
    else if (root->node_kind == WRITE_NODE){
        exe_write_statement(root,symbolTable,out);
    }
    else if (root->node_kind == REPEAT_NODE) {
        exe_repeat_statement(root, symbolTable, out);
        i = MAX_CHILDREN;
    }
    else if (root->node_kind == ASSIGN_NODE) {
        exe_assign_statement(root, symbolTable);
    }
    for (i; i < MAX_CHILDREN; i++) {
        if (root->child[i]) {
            run_code(root->child[i], symbolTable, out);
        }
    }

// Recursive call for the sibling
    if (root->sibling) {
        run_code(root->sibling, symbolTable, out);
    }

}
int main()
{
    // Defining a compiler info object
    CompilerInfo compiler("input.txt", "output.txt", "debug.txt");

    // Defining the root node of the tree
    TreeNode* root = Parse(&compiler);

    // Assigning types for each tree node to turn it into annotated tree
    assign_types(root);

    // Checking each logical conditions on the tree
    type_checking(root);

    // Printing the tree after the checking is done
    OutFile out_file( "syntax_output.txt");


    // 1. in the output file for the requirement output
    out_file.Out("\nTHE SYNTAX TREE\n");
    PrintSyntaxTree(root,out_file);
    out_file.Out("\n\n");

    // 2. in the console for debugging
    cout<<"\nTHE SYNTAX TREE\n";
    PrintTree(root);
    cout<<"\n\n";


    // Defining the symbol table
    SymbolTable symbolTable;

    // Filling the symbol table
    fill_symbol_table(root, symbolTable);

    // Printing the symbol table

    // 1. in the output file for the requirement output
    out_file.Out("\nTHE SYMBOL TABLE\n");
    PrintSymbolTable(symbolTable,out_file);
    out_file.Out("\n\n");

    // 2. in the console for debugging
    cout<<"\nTHE SYMBOL TABLE\n";
    symbolTable.Print();
    cout<<"\n\n";

    // Running the code in the parse tree
    run_code(root,symbolTable,out_file);
    return 0;
}

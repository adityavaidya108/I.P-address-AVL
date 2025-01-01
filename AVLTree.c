//Aditya Samir Vaidya
//G01501989
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include <stdbool.h>

struct address
{
    int height, depth, balanceFactor;
    int octet[4];
    char alias[12];
    struct address *leftChild, *rightChild, *parent;
};

struct address *head = NULL;

//Core Functions
void AddAddress();
void LookUpAddress();
void UpdateAddress();
void DisplayList();
void DisplayAliasForLocation();
void DisplayErrorLog();
void CreateInitialBST();
void DeleteAddress();

//Helper Function
void Start();
void PrintMenu();
void Controller(int option);
void ClearBuffer();
void ValidateIntegerInput(int* option);
int ValidateMenuInputBusinessLogic(int option);
int UserOption();
void EnterInitialNodes();
int ValidateIp(int octet[], int size);
int ValidateAliasFileInput(char alias[]);
void PrintInorder(struct address *head);
int SearchInorder(struct address *head, char alias[]);
bool compareOctets(int octet1[], int octet2[], int size);
bool DuplicateOctetFinder(struct address *head, int octet[4]);
struct address* Insert(struct address *head, int octet[], char alias[], int depth);
struct address* Delete(struct address *head, char alias[]);
struct address* doubleRotateWithLeftChild(struct address* unbalancedNode);
struct address* doubleRotateWithRightChild(struct address* unbalancedNode);
struct address* rebalance(struct address* unbalancedNode, int balanceFactor);

int main()
{
    Start();
    return 0;
}

/*-----------------------------Region HelperFunctions--------------------------------*/ 

//Clears the standard input buffer
void ClearBuffer()
{
    while (getchar() != '\n');
}

//Starts the code and keeps running until user decides to quit
void Start(){
    EnterInitialNodes();
    int start = 1;
    while(start!=8){
        PrintMenu();
        start = UserOption();
        Controller(start);
    }
}

//Prints the menu for the user
void PrintMenu()
{
    printf("---------------Menu-------------\n");
    printf("1.Add address\n");
    printf("2.Look up address\n");
    printf("3.Update address\n");
    printf("4.Delete address\n");
    printf("5.Display list\n");
    printf("6.Display alias for location\n");
    printf("7.Display Error Log\n");
    printf("8.Quit\n");
    printf("--------------------------------\n");
}

//Controller Calling specific user selected function
void Controller(int option)
{
    switch (option)
    {
        case 1:
            AddAddress();
            break;
        case 2:
            LookUpAddress();
            break;
        case 3:
            UpdateAddress();
            break;
        case 4:
            DeleteAddress();
            break;
        case 5:
            DisplayList();
            break;
        case 6:
            DisplayAliasForLocation();
            break;
        case 7:
            DisplayErrorLog();
            break;
        case 8:
            printf("Goodbye!");
            break;
        default:
            break;
    }
}

//Validates if the input is an integer
void ValidateIntegerInput(int* option)
{
    while(scanf("%d",option)!=1){
        printf("Invalid input. Please enter a valid integer:");
        ClearBuffer();
    }
}

//Validates if an integer is between 1 and 8
int ValidateMenuInputBusinessLogic(int option)
{
    if(option>=1 && option<=8)
    {
        return 1;
    }
    printf("Invalid input.Please enter a value between 1 to 8:");
    return 0;
}

//Takes an integer from 1 to 8 and returns
int UserOption()
{
    int option, valid=0;
    printf("Enter an option[1-8]:");
    while (valid!=1)
    {
        ValidateIntegerInput(&option);
        valid = ValidateMenuInputBusinessLogic(option);
    }
    ClearBuffer();
    return option;
}

//Validates the IP addresses
int ValidateIp(int octet[], int size){
    for(int i=0;i<size;i++){
        if(octet[i] <0  || octet[i] > 255){
            return 0;
        }
    }
    return 1;
}

//Validates the alias while entering values in error log file 
int ValidateAliasFileInput(char alias[])
{
    if(strlen(alias)> 10)
        return 0;
    for(int i=0;alias[i]!='\0';i++){
        if(!islower(alias[i])){
            return 0;
        }
    }
    return 1;
}

//Used to get and validate alias for the I.P address 
void GetAlias(char alias[12]){
    while (1)
    {
        printf("Enter alias (upto 10 characters):");
        if(fgets(alias, 12, stdin) !=NULL){
            if(strchr(alias, '\n') == NULL){
                ClearBuffer();
                printf("Alias is too long.\nPlease enter an alias with up to 10 characters\n");
            }
            else{
                alias[strcspn(alias, "\n")] = 0;
                for (int i = 0; alias[i] != '\0'; i++) {
                    alias[i] = tolower((unsigned char)alias[i]);
                }
                break;
            }
        }
    }
}

//Validates the I.P range
int ValidateOctet(int num) {
    return num >= 0 && num <= 255;
}

//Checks if the value entered in each octet is valid
int GetSingleOctet(int index) {
    int num;
    char c;
    while (1) {
        printf("Enter octet %d: ", index + 1);
        if (scanf("%d%c", &num, &c) == 2 && c == '\n') {
            if (ValidateOctet(num)) {
                return num;
            } else {
                printf("Please enter a valid value between 0 and 255.\n");
            }
        } else {
            printf("Please enter a valid integer value.\n");
            ClearBuffer();
        }
    }
}

//Helper function to Get I.P address
void GetIp(int octet[], int size) {
    printf("Enter IP address:\n");
    for (int i = 0; i < size; i++) {
        octet[i] = GetSingleOctet(i);
    }
}


//To read every line from a file and add the valid IPs to the AVL tree and others to log file
void EnterInitialNodes()
{
    FILE *inputFile = fopen("CS531_Inet.txt", "r");
    FILE *errorFile = fopen("CS531_error-log", "w");
    if(inputFile == NULL || errorFile ==NULL){
        printf("Error opening file.\n");
        return;
    }
    int octet[4];
    char alias[12];
    char line[50];
    while(fgets(line, sizeof(line), inputFile) != NULL){
        if(sscanf(line, "%d.%d.%d.%d %11s", &octet[0], &octet[1], &octet[2], &octet[3], alias) == 5){
            int validIp = ValidateIp(octet, sizeof(octet)/sizeof(octet[0]));
            int validAlias = ValidateAliasFileInput(alias);
            if(validIp && validAlias){
                head = Insert(head, octet, alias, 0);
            }
            else{
                fprintf(errorFile, "%s", line);
            }
        }
    }
    fclose(inputFile);
    fclose(errorFile);
}

//Gives the height of each node
int Height(struct address* node)
{
    if(node == NULL)
        return -1;
    return node->height;
}

//Gives the balance factor of each node
int BalanceFactor(struct address* node)
{
    if(node==NULL)
        return 0;
    return Height(node->leftChild) - Height(node->rightChild);
}

// Finds the minimum value node in a subtree
struct address* FindMin(struct address* node) {
    while (node->leftChild != NULL) {
        node = node->leftChild;
    }
    return node;
}

// Creates and initializes a new node with the given data.
struct address* newNode(int octet[], char alias[], int depth)
{
    struct address* node = (struct address *)malloc(sizeof(struct address));
    strcpy(node->alias, alias);
    for(int i=0;i<4;i++){
        node->octet[i] = octet[i];
    }
    node->depth = depth;
    node->height = 0;
    node->balanceFactor = 0;
    node->leftChild = NULL;
    node->rightChild = NULL;
    node->parent = NULL;
    return node;
}

// Returns the maximum of two integers.
int max(int a, int b){
    return (a > b) ? a : b;
}

// Returns the maximum of two integers.
void UpdateDepth(struct address* head, int currentDepth) {
    if (head == NULL) return;
    head->depth = currentDepth;
    UpdateDepth(head->leftChild, currentDepth + 1);
    UpdateDepth(head->rightChild, currentDepth + 1);
}

// Performs a right rotation on an unbalanced node.
struct address* rotateLeft(struct address* unbalancedNode)
{
    struct address* k1 = unbalancedNode->leftChild;
    unbalancedNode->leftChild = k1->rightChild;

    if (k1->rightChild != NULL) {
        k1->rightChild->parent = unbalancedNode;
    }
    k1->rightChild = unbalancedNode;
    k1->parent = unbalancedNode->parent;
    if (k1->parent != NULL) {
        if (k1->parent->leftChild == unbalancedNode) {
            k1->parent->leftChild = k1;
        } else {
            k1->parent->rightChild = k1;
        }
    }
    unbalancedNode->parent = k1;
    UpdateDepth(k1, (k1->parent) ? k1->parent->depth + 1 : 0);
    unbalancedNode->height = 1 + max(Height(unbalancedNode->leftChild), Height(unbalancedNode->rightChild));
    k1->height = 1 + max(Height(k1->leftChild), Height(k1->rightChild));
    unbalancedNode->balanceFactor = Height(unbalancedNode->leftChild) - Height(unbalancedNode->rightChild);
    k1->balanceFactor = Height(k1->leftChild) - Height(k1->rightChild);
    return k1;
}

// Performs a right rotation on an unbalanced node.
struct address* rotateRight(struct address* unbalancedNode)
{
    struct address* newRoot = unbalancedNode->rightChild;
    unbalancedNode->rightChild = newRoot->leftChild;
    if (newRoot->leftChild != NULL) {
        newRoot->leftChild->parent = unbalancedNode;
    }
    newRoot->leftChild = unbalancedNode;
    newRoot->parent = unbalancedNode->parent;
    if (newRoot->parent != NULL) {
        if (newRoot->parent->leftChild == unbalancedNode) {
            newRoot->parent->leftChild = newRoot;
        } else {
            newRoot->parent->rightChild = newRoot;
        }
    }
    unbalancedNode->parent = newRoot;
    UpdateDepth(newRoot, (newRoot->parent) ? newRoot->parent->depth + 1 : 0);
    unbalancedNode->height = 1 + max(Height(unbalancedNode->leftChild), Height(unbalancedNode->rightChild));
    newRoot->height = 1 + max(Height(newRoot->leftChild), Height(newRoot->rightChild));
    unbalancedNode->balanceFactor = Height(unbalancedNode->leftChild) - Height(unbalancedNode->rightChild);
    newRoot->balanceFactor = Height(newRoot->leftChild) - Height(newRoot->rightChild);
    return newRoot;
}

// Performs a double rotation: right, then left, on an unbalanced node.
struct address* doubleRotateWithLeftChild(struct address* unbalancedNode)
{
    unbalancedNode->leftChild = rotateRight(unbalancedNode->leftChild);
    return rotateLeft(unbalancedNode);
}

// Performs a double rotation: left, then right, on an unbalanced node.
struct address* doubleRotateWithRightChild(struct address* unbalancedNode)
{
    unbalancedNode->rightChild = rotateLeft(unbalancedNode->rightChild);
    return rotateRight(unbalancedNode);
}

// Rebalances the tree based on the balance factor of an unbalanced node.
struct address* rebalance(struct address* unbalancedNode, int balanceFactor)
{
    if (balanceFactor == 2) {
        if (Height(unbalancedNode->leftChild->leftChild) >= Height(unbalancedNode->leftChild->rightChild)) {
            return rotateLeft(unbalancedNode);
        } else {
            return doubleRotateWithLeftChild(unbalancedNode);
        }
    }
    else if (balanceFactor == -2) {
        if (Height(unbalancedNode->rightChild->rightChild) >= Height(unbalancedNode->rightChild->leftChild)) {
            return rotateRight(unbalancedNode);
        } else {
            return doubleRotateWithRightChild(unbalancedNode);
        }
    }
    return unbalancedNode;
}

// Inserts a new node into the AVL tree, maintaining balance and updating heights.
struct address* Insert(struct address *head, int octet[], char alias[], int depth)
{
    if(head == NULL){
        return newNode(octet, alias, depth);
    }
    if(strcmp(head->alias, alias) < 0){
        head->rightChild = Insert(head->rightChild, octet, alias, depth+1);
        head->rightChild->parent = head;
    }
    else{
        head->leftChild = Insert(head->leftChild, octet, alias, depth+1);
        head->leftChild->parent = head;
    }
    head->height = 1 + max(Height(head->leftChild), Height(head->rightChild));

    head->balanceFactor = BalanceFactor(head);
    if(head->balanceFactor == 2 || head->balanceFactor == -2)
        return rebalance(head, head->balanceFactor);

    if (head->parent == NULL) {
        UpdateDepth(head, 0);
    }
    return head;
}

// Deletes a node from the AVL tree, maintaining balance and updating heights.
struct address* Delete(struct address *head, char alias[]){
    if (head == NULL) {
        return NULL;
    }
    if (strcmp(alias, head->alias) < 0)
    {
        head->leftChild = Delete(head->leftChild, alias);
        if (head->leftChild) {
            head->leftChild->parent = head;
        }
    } 
    else if (strcmp(alias, head->alias) > 0) 
    {
        head->rightChild = Delete(head->rightChild, alias);
        if (head->rightChild) {
            head->rightChild->parent = head;
        }
    }
    else 
    {
        if (head->leftChild == NULL || head->rightChild == NULL) {
            struct address* temp = head->leftChild ? head->leftChild : head->rightChild;

            if (temp == NULL) {
                temp = head;
                head = NULL;
            } else {
                *head = *temp;
            }
            free(temp);
        } else {
            struct address* temp = FindMin(head->rightChild);
            memcpy(head->octet, temp->octet, sizeof(temp->octet));
            strcpy(head->alias, temp->alias);
            head->rightChild = Delete(head->rightChild, temp->alias);
            if (head->rightChild) {
                head->rightChild->parent = head;
            }
        }
    }
    if(head == NULL)
        return NULL;
    head->height = 1 + max(Height(head->leftChild), Height(head->rightChild));
    head->balanceFactor = BalanceFactor(head);
    if (head->balanceFactor == 2 || head->balanceFactor == -2) {
        return rebalance(head, head->balanceFactor);
    }
    if (head->parent == NULL) {
        UpdateDepth(head, 0);
    }
    return head;
}

//Prints the AVl tree in Inorder fashion
void PrintInorder(struct address *head)
{
    if(head == NULL)
        return;
    PrintInorder(head->leftChild);
    printf("Alias: %s, I.P:%d.%d.%d.%d, Height:%d, Depth:%d, Balance Factor:%d, Parent: %s\n", head->alias, head->octet[0], head->octet[1], head->octet[2], head->octet[3], head->height, head->depth, head->balanceFactor, head->parent ? head->parent->alias : "None");
    PrintInorder(head->rightChild);
}

//Searches a given alias in the tree
int SearchInorder(struct address *head, char alias[])
{
    if(head == NULL)
        return 0;
    if (SearchInorder(head->leftChild, alias))
        return 1;
    if(strcmp(head->alias, alias) == 0){
        printf("The given alias was found.\n");
        printf("Alias: %s, I.P:%d.%d.%d.%d, Height:%d, Depth:%d, Balance Factor:%d, Parent: %s\n", head->alias, head->octet[0], head->octet[1], head->octet[2], head->octet[3], head->height, head->depth, head->balanceFactor, head->parent ? head->parent->alias : "None");
        return 1;
    }
    return SearchInorder(head->rightChild, alias);
}

//Compares octets against another
bool compareOctets(int octet1[], int octet2[], int size) {
    for (int i = 0; i < size; i++) {
        if (octet1[i] != octet2[i]) {
            return false;
        }
    }
    return true;
}

//Finds if a Ip address is pre-existing in the AVL tree
bool DuplicateOctetFinder(struct address *head, int octet[4])
{
    if (head == NULL) return false;
    if (DuplicateOctetFinder(head->leftChild, octet)) return true;
    if (compareOctets(head->octet, octet, 4)) return true;
    return DuplicateOctetFinder(head->rightChild, octet);
}

//Prints the I.P address if found based on alias
bool DisplayAliasHelper(struct address *head, int octet[2])
{
    if (head == NULL) return false;
    if (DisplayAliasHelper(head->leftChild, octet)) return true;
    if(compareOctets(head->octet, octet, 2)){
        printf("The given alias was found.\n");
        printf("Alias: %s, I.P:%d.%d.%d.%d, Height:%d, Depth:%d, Balance Factor:%d, Parent: %s\n", head->alias, head->octet[0], head->octet[1], head->octet[2], head->octet[3], head->height, head->depth, head->balanceFactor, head->parent ? head->parent->alias : "None");
        return true;
    } 
    return DisplayAliasHelper(head->rightChild, octet);
}

//Helper function to update I.P address
int UpdateHelper(struct address *head, char alias[])
{
    if(head == NULL) return 0;
    if(strcasecmp(head->alias, alias) == 0){
        int octet[4];
        printf("The given alias exists.\n");
        printf("I.P Address: %d.%d.%d.%d\n", head->octet[0], head->octet[1], head->octet[2], head->octet[3]);
        printf("Please enter the new I.P address to update\n");
        GetIp(octet, 4);
        int foundOctet = DuplicateOctetFinder(head, octet);
        if(!foundOctet){
            for (int i = 0; i < 4; i++) {
                head->octet[i] = octet[i];
            }
            printf("Alias updated successfully\n");
            return 1;
        }
        else{
            printf("The I.P entered already exists for another alias. Please enter unique valid I.P\n");
        }
    }
    else if(strcasecmp(alias, head->alias) < 0){
        return UpdateHelper(head->leftChild, alias);
    }
    else return UpdateHelper(head->rightChild, alias);
}

/*-----------------------------Region CoreFunctions--------------------------------*/

//Adds an I.P address node in the AVL tree
void AddAddress()
{
    int octet[4];
    char alias[12];
    GetIp(octet, 4);
    GetAlias(alias);
    if(!SearchInorder(head, alias) && !DuplicateOctetFinder(head, octet)){
        head = Insert(head, octet, alias, 0);
    }
}

//Deletes a node from the tree
void DeleteAddress()
{
    if(head == NULL){
        printf("The AVL tree is empty nothing to delete.\n");
        return;
    }
    char alias[12];
    GetAlias(alias);
    if(SearchInorder(head, alias)){
        char confirm;
        printf("Are you sure you want to delete this address? (y/n): ");
        confirm = getchar();
        ClearBuffer();
        if (confirm == 'y' || confirm == 'Y') {
            head = Delete(head, alias);
            printf("The give alias was deleted successfully.\n");
        }
    }
    else{
        printf("The given alias was not found in the tree. Please enter valid alias address.\n");
    }
}

//Displays the tree
void DisplayList()
{
    if(head == NULL){
        printf("The AVL tree is empty nothing to display.\n");
        return;
    }
    PrintInorder(head);
}

//Looks up an address based on alias
void LookUpAddress()
{
    if(head == NULL){
        printf("The AVL tree is empty nothing to lookup.\n");
        return;
    }
    char alias[12];
    GetAlias(alias);
    if(!SearchInorder(head, alias)){
        printf("The given  alias was not found. Please enter a valid alias \n");
    }
}

//Updates the I.P address based on 
void UpdateAddress()
{
    if(head == NULL){
        printf("The AVL tree is empty nothing to update.\n");
        return;
    }
    char alias[12];
    GetAlias(alias);
    if(!UpdateHelper(head, alias)){
        printf("The given alias was not found.\n");
    }
}

//Displays based on octet values given
void DisplayAliasForLocation()
{
    int octet[4];
    GetIp(octet, 2);
    bool found = false;
    if(!DisplayAliasHelper(head, octet)){
        printf("No aliases found for the address %d.%d.\n", octet[0], octet[1]);
    }
}

//Displays the error log file to the user
void DisplayErrorLog(){
    FILE *file = fopen("CS531_error-log", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }
    char line[500];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }
    fclose(file);
    printf("\n");
}
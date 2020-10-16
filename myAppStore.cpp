/*
 * Name: Joseph Feruglio
 * Class: CSE310
 * Assignment: Project 2 Milestone
 *
 */
/*
 * This project milestone creates an app store, where it will read in data with cin
 * storing the data in a hash table for quick look up and a binary search tree as the database
 * currently supports one query function: 'find app'
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <math.h>
#include <iomanip>

using namespace std;

struct app_info{
    string category;
    string app_name;
    string version;
    float size;
    string units;
    float price;
};

struct tree{
    struct app_info record;
    struct tree *left;
    struct tree *right;
};

struct categories{
    string category;
    struct tree *root;
};

struct hash_table_entry{
    string app_name;
    struct tree *app_node;
    struct hash_table_entry *next;
};

/**
 * This function will check if the number is a prime or not
 * @param val The number to be checked if is a prime
 * @return True or False value
 */
bool TestForPrime ( int val){
    int limit, factor = 2;

    limit = (long)( sqrtf((float) val) + 0.5f);
    while((factor <= limit) && (val % factor))
        factor++;


    return (factor > limit);
}

/**
 * This function recursively deletes the nodes from the bst
 * @param node The root
 */
void deleteNode(struct tree* node){
    if(node->left != nullptr){
        deleteNode(node->left);
    }
    if(node->right != nullptr){
        deleteNode(node->right);
    }
    delete node;
}

int main(){

    // declaring string variable to hold input data
    string input;

    // gets the number of categories
    getline(cin, input);

    // converts input into an int
    int howManyCategories = stoi(input);

    // dynamically allocate an array of size howManyCategories of type struct categories
    auto *app_categories = new categories[howManyCategories];

    // loops through the array of categories setting the names of each category
    // and sets the root of the bst to NULL
    for(int i = 0; i < howManyCategories; i++){
        getline(cin, input);
        app_categories[i].category = input;
        app_categories[i].root = nullptr;
    }

    // get the number of applications
    getline(cin, input);

    // converts input into an int
    int howManyApps = stoi(input);

    // initialing int and boolean to test for prime number
    int primeNumber = 2*howManyApps + 1;
    bool flag = false;

    // searching for the closest possible prime
    while(!flag){
        if(TestForPrime(primeNumber)){
            flag = true;
        }
        primeNumber++;
    }

    // initializing hash table to size of closest prime number to the right
    auto **hash_table = new hash_table_entry*[primeNumber];


    // adding apps to hash table and bst
    for(int k = 0; k < howManyApps; k++){   // outer loop for number of apps
        tree* newApp = new tree;

        getline(cin, input);
        newApp->record.category = input;
        getline(cin, input);
        newApp->record.app_name = input;
        getline(cin, input);
        newApp->record.version = input;
        getline(cin, input);
        float size = stof(input);
        newApp->record.size = size;
        getline(cin, input);
        newApp->record.units = input;
        getline(cin, input);
        float price = stof(input);
        newApp->record.price = price;
        newApp->left = nullptr;
        newApp->right = nullptr;


        // inserting newApp into bst
        for(int l = 0; l < howManyCategories; l++){     // search app_categories array
            if(app_categories[l].category == newApp->record.category){      // checking for matching category
                if(app_categories[l].root == nullptr){     // checking if first node is NULL
                    app_categories[l].root = newApp;
                    break;
                } else {                                // checking if newApp goes in left or right node until successful insert
                    bool openSpot = false;
                    tree* temp = app_categories[l].root;          // temp node to traverse bst
                    while(!openSpot){
                        if(temp->record.category.compare(newApp->record.category) <= 0){        // checking if node will go to left
                            if(temp->left == nullptr){                 // check if left node is currently NULL
                                temp->left = newApp;
                                break;       // exit while loop on successful insert
                            } else {                                // sets temp to left node and continues next traversal
                                temp = temp->left;
                                continue;
                            }
                        } else if(temp->record.category.compare(newApp->record.category) > 0){     // checking if node will go to right
                            if(temp->right == nullptr){                // check if right node is currently NULL
                                temp->right = newApp;
                                break;      // exit while loop on successful insert
                            } else {                                // sets temp to right node and continues next traversal
                                temp = temp->right;
                                continue;
                            }
                        }
                    }
                }
                break;      // exit for loop early on successful insert
            }
        }


        // inserting newApp into hash table
        int sum = 0;
        for(char m : newApp->record.app_name){  // go through string getting ascii value of each character
            sum += m;
        }
        int hash = sum % primeNumber;                               // calculating the hash
        if(&hash_table[hash] == nullptr){                               // checking if that entry is currently NULL

            hash_table[hash]->app_name = newApp->record.app_name;
            hash_table[hash]->app_node = newApp;
            hash_table[hash]->next = nullptr;
        } else {                                                    // if not NULL then make new entry
            auto* temp = new hash_table_entry;
            temp->app_node = newApp;                                // initialize entry
            temp->app_name = newApp->record.app_name;
            temp->next = hash_table[hash];                          // set next of entry to what is currently at hash
            hash_table[hash] = temp;                                // point hash to new entry

        }
    }

    // getting number of queries and turning into an int
    getline(cin, input);
    int numberOfQueries = stoi(input);

    for(int h = 0; h < numberOfQueries; h++){               // loop through the queries one at a time
        getline(cin, input);
        string whichQuery = input.substr(0,9);      // substring the command
        if(whichQuery == "find app "){                      // check if the substring is find app
            string whichApp = input.substr(9);        // take substring of rest of input (the apps name)
            int sum = 0;                                    // taking sum of ascii value for each char in app name
            for(char n : whichApp){
                sum += n;
            }
            int hash = sum % primeNumber;                   // calculating hash
            bool found = false;
            hash_table_entry* temp = hash_table[hash];      // initializing temp hash table entry to the current hash table hash
            while(temp != nullptr){                            // if temp is not null then look for matching app name
                if(temp->app_name == whichApp){
                    found = true;
                    break;
                } else {
                    temp = temp->next;
                }
            }

            if(found){                                      // if found matching app name then go through the records and print its info
                cout << "Found Application: " << whichApp << endl;
                cout << "\tCategory: " << temp->app_node->record.category << endl;
                cout << "\tApplication Name: " << temp->app_node->record.app_name << endl;
                cout << "\tVersion: " << temp->app_node->record.version << endl;
                cout << "\tSize: " << fixed << setprecision(1) << temp->app_node->record.size << endl;
                cout << "\tUnits: " << temp->app_node->record.units << endl;
                cout << "\tPrice: $" << fixed << setprecision(2) << temp->app_node->record.price << endl;
            } else {                                        // app name was not found
                cout << "Application " << whichApp.erase(whichApp.length() -1) << " not found." << endl;
            }
            if(h != numberOfQueries-1){
                cout << endl;
            }

        }
    }

    // go through each index of hash table checking if NULL or not
    for(int j = primeNumber-1; j >= 0; j--){
        if(hash_table[j] == nullptr){
            delete hash_table[j];
        } else {                   // if not null then delete the nodes
            hash_table_entry *temp = hash_table[j];
            while(temp != nullptr){
                hash_table_entry *temp2 = temp->next;
                delete temp;
                temp = temp2;
            }
        }
    }
    delete[] hash_table;        // completely delete the hash table array


    // goes through each category of bst checking if the initial root is NULL or not
    for(int i = howManyCategories-1; i >= 0; i--){
        if(app_categories[i].root != nullptr){
            deleteNode(app_categories[i].root);       // if root wasn't null send root to recursive delete func
        }
    }
    delete[] app_categories;                // completely delete the app_categories array

    return 0;
}

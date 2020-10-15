//
// Created by josep on 10/14/2020.
//

#include "myAppStore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <math.h>

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

bool TestForPrime ( int val){
    int limit, factor = 2;

    limit = (long)( sqrtf((float) val) + 0.5f);
    while((factor <= limit) && (val % factor))
        factor++;


    return (factor > limit);
}

int main(){

    // declaring string variable to hold input data
    string input;

    // gets the number of categories
    getline(cin, input);

    // converts input into an int
    int howManyCategories = stoi(input);

    // dynamically allocate an array of size howManyCategories of type struct categories
    categories *app_categories = new categories[howManyCategories];

    // loops through the array of categories setting the names of each category
    // and sets the root of the bst to NULL
    for(int i = 0; i < howManyCategories; i++){
        getline(cin, input);
        app_categories[i].category = input;
        app_categories[i].root = NULL;
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
    hash_table_entry **hash_table = new hash_table_entry*[primeNumber];

    // adding apps to hash table and bst
    for(int k = 0; k < howManyApps; k++){   // outer loop for number of apps
        tree* newApp = new struct tree;

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
        newApp->left = NULL;
        newApp->right = NULL;

        // inserting newApp into bst
        for(int l = 0; l < howManyCategories; l++){     // search app_categories array
            if(app_categories[l].category == newApp->record.category){      // checking for matching category
                if(app_categories[l].root == NULL){     // checking if first node is NULL
                    app_categories[l].root = newApp;
                    break;
                } else {                                // checking if newApp goes in left or right node until successful insert
                    bool openSpot = false;
                    while(!openSpot){
                        tree* temp = app_categories->root;          // temp node to traverse bst
                        if(temp->record.category > newApp->record.category){        // checking if node will go to left
                            if(temp->left == NULL){                 // check if left node is currently NULL
                                temp->left = newApp;
                                break;       // exit while loop on successful insert
                            } else {                                // sets temp to left node and continues next traversal
                                temp = temp->left;
                                continue;
                            }
                        } else if(temp->record.category < newApp->record.category){     // checking if node will go to right
                            if(temp->right == NULL){                // check if right node is currently NULL
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
        for(int m = 0; m < newApp->record.category.length(); m++){  // go through string getting ascii value of each character
            sum += newApp->record.category[m];
        }
        int hash = sum % primeNumber;                               // calculating the hash
        if(hash_table[hash] == NULL){                               // checking if that entry is currently NULL
            hash_table[hash]->app_name = newApp->record.app_name;
            hash_table[hash]->app_node = newApp;
            hash_table[hash]->next = NULL;
        } else {                                                    // if not NULL then make new entry
            hash_table_entry* temp = new hash_table_entry;
            temp->app_node = newApp;                                // initialize entry
            temp->app_name = newApp->record.app_name;
            temp->next = hash_table[hash];                          // set next of entry to what is currently at hash
            hash_table[hash] = temp;                                // point hash to new entry
        }

    }

    getline(cin, input);
    int numberOfQueries = stoi(input);

    for(int h = 0; h < numberOfQueries; h++){
        getline(cin, input);
        string whichQuery = input.substr(0,9);
        if(whichQuery == "find app "){
            string whichApp = input.substr(9,std::string::npos);
            int sum = 0;
            for(int n = 0; n < whichApp.length(); n++){
                sum += whichApp[n];
            }
            int hash = sum % primeNumber;
            bool found = false;
            hash_table_entry* temp = hash_table[hash];
            while(temp != NULL){
                if(temp->app_name == whichApp){
                    found = true;
                    break;
                } else {
                    temp = temp->next;
                }
            }
            if(found){
                cout << "Found Application: " << whichApp << endl;
                cout << "\tCategory: " << temp->app_node->record.category << endl;
                cout << "\tApplication Name: " << temp->app_node->record.app_name << endl;
                cout << "\tVersion: " << temp->app_node->record.version << endl;
                cout << "\tSize: " << temp->app_node->record.size << endl;
                cout << "\tUnits: " << temp->app_node->record.units << endl;
                cout << "\tPrice: $" << temp->app_node->record.price << endl;
            } else {
                cout << "Application " << whichApp << " not found." << endl;
            }
        }

        // write code for deallocating the memory (something getline with eof maybe)
    }

    return 0;
}

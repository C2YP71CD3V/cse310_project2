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

#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

struct app_info{
    char app_name[50];
    char category[50];
    char version[10];
    float size;
    char units[3];
    float price;
};

struct tree{
    struct app_info record;
    struct tree *left;
    struct tree *right;
};

struct categories{
    char category[50];
    struct tree *root;
};

struct hash_table_entry{
    char app_name[50];
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

void printInOrder(struct tree* node){
    if(node->left != nullptr){
        printInOrder(node->left);
    }
    cout << '\t' << node->record.app_name << endl;
    if(node->right != nullptr){
        printInOrder(node->right);
    }
}

void priceInOrder(struct tree* node, string arr[], int howMany){
    if(node->left != nullptr){
        priceInOrder(node->left, arr, howMany);
    }

    if(node->record.price == 0.00){
        arr[howMany] = node->record.app_name;
        howMany++;
        cout << "\t" << node->record.app_name << endl;
    }
    if(node->right != nullptr){
        priceInOrder(node->right, arr, howMany);
    }

}

void rangeAppInOrder(struct tree* node, string arr[], char a, char b, int howMany){
    if(node->left != nullptr){
        rangeAppInOrder(node->left, arr, a, b, howMany);
    }
    if(node->record.app_name[0] >= a && node->record.app_name[0] <= b){
        arr[howMany] = node->record.app_name;
        howMany++;
    }

    if(node->right != nullptr){
        rangeAppInOrder(node->right, arr, a, b, howMany);
    }

}
void rangePriceInOrder(struct tree* node, string arr[], float low, float high, int howManyTimes){
    if(node->left != nullptr){
        rangePriceInOrder(node->left, arr, low, high, howManyTimes);
    }
    if(node->record.price >= low && node->record.price <= high){
        arr[howManyTimes] = node->record.app_name;
        howManyTimes++;
    }
    if(node->right != nullptr){
        rangePriceInOrder(node->right, arr, low, high, howManyTimes);
    }

}

void deleteInOrder(struct tree* node, string appName){

    if(strcmp(node->record.app_name, appName.c_str()) > 0){
        if(node->left != nullptr){
            deleteInOrder(node->left, appName);
        }
    }

    if(strcmp(node->record.app_name, appName.c_str()) < 0){
        if(node->right != nullptr){
            deleteInOrder(node->right, appName);
        }
    }

    if(strcmp(node->record.app_name, appName.c_str()) == 0){
        deleteNode(node);
    }
}

int main(){

    // declaring string variable to hold input data
    string input;

    // gets the number of categories
    getline(cin, input);

    // converts input into an int
    int howManyCategories = stoi(input);

    // dynamically allocate an array of size howManyCategories of type struct categories
    auto app_categories = new categories[howManyCategories];

    // loops through the array of categories setting the names of each category
    // and sets the root of the bst to NULL
    for(int i = 0; i < howManyCategories; i++){
        getline(cin, input);
        string name = input;
        name += '\0';
        strcpy(app_categories[i].category, name.c_str());
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
        } else {
            primeNumber++;
        }
    }

    // initializing hash table to size of closest prime number to the right
    auto **hash_table = new hash_table_entry*[primeNumber];


    // adding apps to hash table and bst
    for(int k = 0; k < howManyApps; k++) {   // outer loop for number of apps
        tree *newApp = new tree;

        getline(cin, input);        // get category of app
        string name2 = input;
        name2[name2.length()] = '\0';
        strcpy(newApp->record.category, name2.c_str());

        getline(cin, input);        // get app name of app
        strcpy(newApp->record.app_name, input.c_str());

        getline(cin, input);        // get version of app
        strcpy(newApp->record.version, input.c_str());

        getline(cin, input);        // get size of app
        float size = stof(input);
        newApp->record.size = size;

        getline(cin, input);        // get size units (MB or GB)
        strcpy(newApp->record.units, input.c_str());

        getline(cin, input);        // get price of app
        float price = stof(input);
        newApp->record.price = price;

        newApp->left = nullptr;            // set both left and right of new app tree node to null
        newApp->right = nullptr;

        // inserting newApp into bst
        for(int l = 0; l < howManyCategories; l++){     // search app_categories array

            if(strcmp(app_categories[l].category, newApp->record.category) == 0){          // checking for matching category in bst
                if(app_categories[l].root == nullptr){          // best case if root of category is null
                    app_categories[l].root = newApp;
                    break;
                } else {
                    bool openSpot = false;
                    tree* temp = app_categories[l].root;          // temp node to traverse bst
                    while(!openSpot){
                        if(strcmp(temp->record.app_name, newApp->record.app_name) >= 0){        // checking if node will go to left
                            if(temp->left == nullptr){                 // check if left node is currently NULL
                                temp->left = newApp;
                                openSpot = true;
                            } else {                                // sets temp to left node and continues next traversal
                                temp = temp->left;
                            }
                        } else if(strcmp(temp->record.app_name, newApp->record.app_name) < 0){     // checking if node will go to right
                            if(temp->right == nullptr){                // check if right node is currently NULL
                                temp->right = newApp;
                                openSpot = true;
                            } else {                                // sets temp to right node and continues next traversal
                                temp = temp->right;
                            }
                        }
                    }
                }
                break;      // break out of for loop searching through bst categories on successful insert
            }

        }


        // inserting newApp into hash table
        int sum = 0;
        for(char m : newApp->record.app_name){  // go through string getting ascii value of each character
            sum += m;
        }
        string tempName = newApp->record.app_name;
        auto* temp = new hash_table_entry;
        int hash = sum % primeNumber;                               // calculating the hash
        if(hash_table[hash] == nullptr){                            // checking if that entry is currently NULL
            strcpy(temp->app_name, tempName.c_str());
            temp->app_node = newApp;
            temp->next = nullptr;
            hash_table[hash] = temp;
        } else {                                                    // if not NULL then make new entry
            temp->app_node = newApp;
            strcpy(temp->app_name, tempName.c_str());
            temp->next = hash_table[hash];                          // set next of entry to what is currently at hash
            hash_table[hash] = temp;                                // point hash to new entry

        }
    }

    // getting number of queries and turning into an int
    getline(cin, input);
    int numberOfQueries = stoi(input);

    for(int h = 0; h < numberOfQueries; h++){               // loop through the queries one at a time
        getline(cin, input);
        if(input == "report"){
            
        }
        string whichQuery1 = input.substr(0, input.find(" "));     // substring the command
        /*if(whichQuery1 == "range") {
            string whichCategory = input.substr(input.find('\"'));
            string whichCategory2 = whichCategory.substr(0,whichCategory.find(" "));
            for(int i = 0; i < 2; i++){
                whichCategory2.erase(whichCategory2.find('\"'), 1);
            }
            string whatToDo = input.substr(input.find(" "));
            whatToDo.erase(whatToDo.find(" "), 1);
            whatToDo = whatToDo.substr(whatToDo.find(" "));
            whatToDo.erase(whatToDo.find(" "), 1);
            whatToDo = whatToDo.substr(0, whatToDo.find(" "));
            if(whatToDo == "app"){
                string low = input.substr(input.find(" "));
                low.erase(low.find(" "), 1);
                for(int i = 0; i < 2; i++){
                    low = low.substr(low.find(" "));
                    low.erase(low.find(" "), 1);
                }
                low = low.substr(0, low.find(" "));
                string high = input.substr(input.find(" "));
                for(int j = 0; j < 4; j++){
                    high = high.substr(high.find(" "));
                    high.erase(high.find(" "), 1);
                }
                for(int i = 0; i < 2; i++){
                    low.erase(low.find('\"'), 1);
                }
                for(int i = 0; i < 2; i++){
                    high.erase(high.find('\"'), 1);
                }

                string apps3[25];
                for(int m = 0; m < howManyCategories; m++){
                    if(strcmp(app_categories[m].category, whichCategory2.c_str()) == 0){
                        cout << "here" << endl;

                        cout << "here2" << endl;
                        rangeAppInOrder(app_categories[m].root, apps3, low[0], high[0], 0);
                        if(!apps3[0].empty()){
                            cout << "Applications in Range (" << low[0] << ","
                            << high[0] << ") in Category: " << whichCategory2 << endl;
                            int howManyApps = 0;
                            while(!apps3[howManyApps].empty()){
                                cout << '\t' << apps3[howManyApps] << endl;
                                howManyApps++;
                            }
                        } else {
                            cout << "No applications found in " << whichCategory2 << " for the given range ("
                            << low[0] << "," << high[0] << ")." << endl;
                        }
                    }
                }

            } else if(whatToDo == "price"){
                string low = input.substr(input.find(" "));
                for(int i = 0; i < 2; i++){
                    low = low.substr(low.find(" "));
                }
                low = low.substr(0, low.find(" "));
                string high = input.substr(input.find(" "));
                for(int j = 0; j < 3; j++){
                    high = high.substr(high.find(" "));
                }

                float lowNum = stof(low);
                float highNum = stof(high);

                string apps2[25];
                for(int m = 0; m < howManyCategories; m++){
                    if(strcmp(app_categories[m].category, whichCategory2.c_str()) == 0){
                        if(app_categories[m].root != nullptr){
                            rangePriceInOrder(app_categories[m].root, apps2, lowNum, highNum, 0);
                            if(!apps2[0].empty()){
                                cout << "Applications in Price Range ($" << fixed << setprecision(2) << lowNum << ",$"
                                << highNum << ") in Category: " << whichCategory2 << endl;
                                int howManyApps2 = 0;
                                while(!apps2[howManyApps2].empty()){
                                    cout << '\t' << apps2[howManyApps2] << endl;
                                    howManyApps2++;
                                }
                            } else {
                                cout << "No applications found in " << whichCategory2 << " for the given price range ($"
                                << fixed << setprecision(2) << lowNum << ",$" << highNum << ")." << endl;
                            }
                        }
                    }
                }
            }
        }*/
        /*if(whichQuery1 == "delete"){
            string whichCat = input.substr(input.find('\"'));
            whichCat = whichCat.erase(whichCat.find('\"'), 1);
            whichCat = whichCat.substr(0, whichCat.find('\"'));
            whichCat = whichCat.erase(whichCat.find('\"'), 1);

            string whichApp = input.substr(input.find(" "));
            whichApp = whichApp.substr(whichApp.find(" "));

            for(int i = 0; i < 2; i++){
                whichApp.erase(whichApp.find('\"'), 1);
            }

            for(int j = 0; j < howManyCategories; j++){
                if(strcmp(app_categories[j].category, whichCat.c_str()) == 0){
                    if(app_categories[j].root != nullptr){
                        deleteInOrder(app_categories[j].root, whichApp);
                        break;
                    }

                }
            }

            int sum = 0;                                    // taking sum of ascii value for each char in app name
            for(char n : whichApp){
                sum += n;
            }

            int hash = sum % primeNumber;
            bool found = false;
            hash_table_entry* temp = hash_table[hash];      // initializing temp hash table entry to the current hash table hash
            hash_table_entry* temp2 = hash_table[hash];
            while(temp != nullptr){                            // if temp is not null then look for matching app name
                if(strcmp(temp->app_name, whichApp.c_str()) == 0){
                    while(temp2->next != temp){
                        temp2 = temp2->next;
                    }

                    temp2->next = temp->next;

                    delete temp;

                    cout << "Application " << whichApp << " from Category " << whichCat << " successfully deleted.";
                    found = true;
                    break;
                } else {
                    temp = temp->next;
                }
            }

            if(!found){
                cout << "Application " << whichApp << " could not be found in category " << whichCat << "; unable to delete."
                << endl;
            }

        }*/
        string whichQuery2 = input.substr(input.find(" "));
        whichQuery2.erase(whichQuery2.find(" "), 1);
        whichQuery2 = whichQuery2.substr(0,whichQuery2.find(" "));
        string whichQuery3 = whichQuery1 + " " + whichQuery2;
        if(whichQuery3 == "find app"){                      // check if the substring is find app
            string whichApp = input.substr(input.find('\"'));        // take substring of rest of input (the apps name)
            for(int i = 0; i < 2; i++){
                whichApp.erase(whichApp.find('\"'), 1);
            }
            int sum = 0;                                    // taking sum of ascii value for each char in app name
            for(char n : whichApp){
                sum += n;
            }
            int hash = sum % primeNumber;                   // calculating hash
            bool found = false;
            hash_table_entry* temp = hash_table[hash];      // initializing temp hash table entry to the current hash table hash
            while(temp != nullptr){                            // if temp is not null then look for matching app name
                if(strcmp(temp->app_name, whichApp.c_str()) == 0){
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
                cout << "Application " << whichApp << " not found." << endl;
            }


        } else if(whichQuery3 == "find category"){
            string whichCat = input.substr(input.find('\"'));
            whichCat.erase(whichCat.find('\"'), 1);
            whichCat.erase(whichCat.find('\"'), 2);
            for(int j = 0; j < howManyCategories; j++){
                if(strcmp(app_categories[j].category, whichCat.c_str()) == 0){
                    if(app_categories[j].root == nullptr){
                        cout << "Category " << whichCat << " no apps found." << endl;
                        break;
                    } else{
                        cout << "Category: " << whichCat << endl;
                        printInOrder(app_categories[j].root);
                    }
                } else if(j + 1 == howManyCategories){
                    cout << "Category " << whichCat << " not found." << endl;
                }
            }
        } else if(whichQuery3 == "find price") {
            bool foundOne = false;
            for(int i = 0; i < howManyCategories; i++){
                cout << "Free Applications in Category: " << app_categories[i].category << endl;
                string apps[25];
                if(app_categories[i].root != nullptr){
                    priceInOrder(app_categories[i].root, apps, 0);
                    if(!apps[0].empty()){
                    } else {
                        cout << "\tNo free applications found." << endl;
                    }
                } else {
                    cout << "\tNo free applications found." << endl;
                }
                cout << endl;
            }

        } else if(whichQuery3 == "no report"){} //do nothing

        if(h+1 < numberOfQueries){
                cout << '\n';
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

/// NAME: omar ashraf mohamed    ID:20235021///

#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

//Class Node for the linked list
class Node
{
public:
    string info;    // info for the linked list
    Node*next;      // the location of the linked list
    Node(string in):info(in),next(nullptr){}  // constructor
};

// class stack implementation for the linked list
class Stack
{
private:
    Node*topofthestack;  // element that appear in the top of the stack
public:
    Stack(){topofthestack=nullptr;}  //constructor

    // function used to push an item to the stack
    void push(string item)
    {
        Node*n=new Node(item);  // initialize an value
        if(n==nullptr)   // check if the stack is empty or not
        {
            cout << "stack push can not allocate memory" << endl;
        }
        else
        {
            n->info=item;
            n->next=topofthestack;
            topofthestack=n;
        }
    }

    // check if the stack is empty or not
    bool isEmpty() {return topofthestack==nullptr;}

    // function used to pop an item to the stack
    string pop()
    {
        if(isEmpty()) {  // check if is empty or not
                cout << "stack is empty" << endl;
                return "";
        }
        else
        {
            Node*temp=topofthestack;
            string value = temp->info;
            topofthestack=topofthestack->next;
            temp=temp->next=nullptr;
            delete temp;  // delete the current item
            return value; // return the current item after delete it
        }
    }


    // clear the stack
    void Clear()
    {
         while(!isEmpty()) {pop();}
    }
};



int main()
{
    Stack Backward , Forward , temp ;  // declare variable for backward and forward and temp navigation
    string URLs[9] ;     // declare an array to store urls
    int i =0;

    ifstream file ("URLs.txt");    // declare a file input stream to read from the file
    if(!file){
        cout<<" Error in the opening of the file" << endl;   // error message if file can not open
        return 1;   // exit the program with an error
    }

    while(i<9 && getline(file,URLs[i])){ i++; } // read up to 9 urls from the file and store them in the urls array

    for(int j=0;j<9;j++){  // push the urls array into the temp
        temp.push(URLs[j]);
    }

    while(!temp.isEmpty()) { Backward.push(temp.pop()); }  // transfer every url from temp into the backward navigation stack
                                                           // pop from temp and push to backward
    string Currentpage = Backward.pop(); // pop the first url from backward and name it as current page

    // read the 2nd line from the file which contain backward and forward
    string line;
    getline(file,line);
    file.close();

    // tokenize the command using strtok function to split by space
    char *cmd = strtok(&line[0]," ");  // get first command
    string commnd[10];  // array to store the command
    int cmdcounter=0;  // counter for the array
    while(cmd !=nullptr && cmdcounter<10){  // loop through the command line and split it into individual commands
        commnd[cmdcounter++]=cmd;   // store each command in the array
        cmd=strtok(nullptr," ");    // get the next command
    }

    // the process of each 10 commands
    for(int j=0;j<10;j++)
    {
        cout<<"\nChoice:" << (j+1) << " [" << commnd[j] << "]\n";  // display the current command choice

        if(commnd[j]=="Backward" || commnd[j]=="backward"){  // handle the backward command
            if(!Backward.isEmpty()){
                Forward.push(Currentpage);  // if the backward is not empty push current page into the forward
                Currentpage=Backward.pop();// pop the top of the url from backward and set it as current page
            }
        }else if(commnd[j]=="Forward" || commnd[j]=="forward" ){  // handle the forward command
                if(!Forward.isEmpty()){
                Backward.push(Currentpage);   // if the forward is not empty push current page into the backward
                Currentpage=Forward.pop();  // pop the top of the url from forward and set it as current page
                }
        }else {
                cout << "Invalid command: " << commnd[j] << " — Skipping.\n";  // for handling incorrect command
        }

        cout << " === The current page is : ===\n " << Currentpage << endl;  // display the current page

        string next="";
        if(!Forward.isEmpty()){
            next=Forward.pop();   // pop the next page from forward
            Forward.push(next);    // push it back
        }
        cout<< "=== The next one is : ===\n" <<(next.empty()? "NONE" : next)<<endl;  // display the next page

        string previous="";
        if(!Backward.isEmpty()){
            previous=Backward.pop(); // pop the previous page from backward
            Backward.push(previous); // push it back
        }
        cout<< "=== The previous one is : ===\n" <<(previous.empty()? "NONE" : previous)<<endl; // display the previous page

        cout << "-----------------------------------------------------------------------------------------------"<<endl;
    }


    return 0;
}


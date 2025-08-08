/// NAME: omar ashraf mohamed    ID:20235021///


#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class PrintJob
{
  public:
  string pcname;
  int arrinmins;
  string arrivaltime;
  int duration;
// Default constructor
  PrintJob()
  {
    pcname = " ";
    arrivaltime = " ";
    arrinmins = 0;
    duration = 0;
  }
// Parameterized constructor
  PrintJob(string pc, string aim, int dur)
  {
    pcname = pc;
    arrinmins = convertintomins(aim);
    duration = dur;
    arrivaltime = aim;
  }
// Convert the arrival time from string to minutes
  static int convertintomins (string time)
  {
    if(time.length()<5 || time[2] != ':') { return -1; }
    int hrs = stoi(time.substr(0,2));
    int mins = stoi(time.substr(3,2));
    return (hrs*60) + mins;
  }
  static bool createjfromstr(string data , PrintJob &job)
  {
    // Find the positions of the spaces in the line
    size_t space1 = data.find(' ');
    size_t space2 = data.find(' ', space1+1);
    if(space1 == string::npos || space2 == string::npos){return false;}
// Extract PC name, time, and duration using substr
    string pcname = data.substr(0, space1);    // PC name is before the first space
    string time = data.substr(space1 + 1, space2 - space1 - 1);    // Arrival time is between the first and second spaces
    string duration = data.substr(space2 + 1);    // Duration is after the second space
    int mins = convertintomins(time);
    if(mins==-1) return false;

    int dur = stoi(duration);    // Converted duration from string into integer

    job=PrintJob(pcname, time, dur);
    return true;
  }
};

class Node
{
public:
    PrintJob pjob;
    Node * next;
// Parameterized constructor
    Node(PrintJob job)
    {
        pjob = job;
        next=nullptr;
    }
};

class PriorityQueue
{
    public:
    Node *head;
    int counter;
    int totocctime;      // Tracks total printer occupation time
    int totwait;        // Tracks total waiting time
// Default constructor
    PriorityQueue()
    {
        head = nullptr;
        counter = 0;
        totocctime = 0;
        totwait = 0;
    }

// To add the printing job into the printing queue
      void enqueue(PrintJob job) {
// To create a new node
        Node* newn = new Node(job);
        Node* current = head;
        Node* prev = nullptr;

        while (current != nullptr &&
              (current->pjob.arrinmins < job.arrinmins ||
              (current->pjob.arrinmins == job.arrinmins &&
               current->pjob.duration <= job.duration))) {
            prev = current;
            current = current->next;
        }
// Insert at the begining
        if (prev == nullptr) {
            newn->next = head;
            head = newn;
        }
// Insert in the middle or the end
        else {
            prev->next = newn;
            newn->next = current;
        }

        counter++;
    }

    // Convert the execution time from minutes to string (hh:mm format)
    string converttimeback(int mins)
    {
        int hours = mins /60;     // Extracted the hours
        int m = mins % 60;     // Used the modulus operator to get the remaining minutes after extracting the hours
        string hh;     // a string to store the coverted hours in two digits
        string mm;     // a string to store the coverted minutes two digits
// Storing the hours in hh format
// If "hours" is less than 10 add 0 to the tens
        if(hours<10)
        {
            hh = "0" + to_string(hours);
        }
// Storing the hours in mm format
// If "hours" is larger than 10 convert it into string as it is
        else
        {
          hh = to_string(hours);
        }
        // If "m" is less than 10 add 0 to the tens
        if(m<10)
        {
            mm = "0" + to_string(m);
        }
        // If "m" is larger than 10 convert it into strung as it is
        else
        {
          mm = to_string(m);
        }
      return hh + ":" + mm;
    }
// To print the details of the printing job
    void print()
    {
        if(head==nullptr){
            cout<<"No jobs in the queue." << endl;
        }

        cout << "Jobs Executed: " << counter << endl;
// Declared "currtime" to track when the printer starts executing each job
        int currtime;
// In case the queue isn't empty therefore the "currtime" starts from when the head executes
        if(head)
        {
            currtime = head -> pjob.arrinmins;
        }
// In case the queue is empty therefore the "currtime" is set to 0
        else
        {
            currtime = 0;
        }

        Node * current = head;
        Node *prev = nullptr;
// If queue isn't empty, set "currtime" to the arrival time of the first job
        if (current)
        {
            currtime=current->pjob.arrinmins;
        }
// If queue is empty, set "currtime" to 0
        else
        {
            currtime=0;
        }
        totocctime = 0;
        totwait = 0;
// Traverses the queue
        while(current !=nullptr)
        {
// Removes the current printing job from the queue
// If the queue is empty update the head to the next node (implemented in the dequeue function above)
           if(prev == nullptr)
           {
            head = current -> next;
           }
// If the queue isn't empty; link the previous node to the next node, bypassing the current one
           else
           {
            prev -> next = current -> next;
           }

// Declared "wait" to calculate the time a job waits in the queue before it starts executing
           PrintJob job=current->pjob;
           int wait;
           if (currtime < job.arrinmins )
           {
                currtime = job.arrinmins;
           }
           wait=currtime-job.arrinmins;
           totwait += wait;
           totocctime += job.duration;
//Print job details
           cout<<"PC: " << job.pcname << "  Arrival " <<converttimeback(job.arrinmins)<<"   Duration: " << job.duration << "   executed at: " << converttimeback(currtime) << "   Waiting time: " << wait << endl;
// Update current time
           currtime +=  job.duration;
           current=current->next;
        }
        cout << endl;
// Prints the printer's total occupation time
        cout << "The printer was occupied for: "  << totocctime << " minutes" << endl;
        cout << endl;
// Prints the average waiting time
        cout << "Average waiting time for all jobs is ";
        if (counter > 0)
        {
           cout << float(totwait)/counter;
        }
        else
        {
            cout << 0;
        }
        cout << " minutes" << endl;
    }


};
int main()
{
    PriorityQueue pq;
// Create a file named "printer.txt"
    ifstream file ("printer.txt");
    string line;
// Checks if file is open
    if(!file.is_open())
    {
      cout << "Error! Can't open the file";
      return 1;
    }
    getline(file, line);
// Reads each line from the file and copies into a PriorityQueue object "job"
    while(getline(file, line))
    {
        PrintJob job;
        if(PrintJob::createjfromstr(line,job)) {
                pq.enqueue(job);
        }else
// Error handling in case there were invalid lines
        {
            cout<< "skipping the invalid lines: " << line << endl;
        }

    }
// Closes the file
    file.close();
// Prints the file's content
    pq.print();

    return 0 ;
}


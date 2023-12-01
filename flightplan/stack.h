#pragma once
#include <string>

using namespace std;
//nodes for stack only
class StackNode {
public:
    string data;
    StackNode* link;

    StackNode(string dat) {
        data = dat;
        link = NULL;
    }
};

//stack, what else can I say
class Stack {
    StackNode* top;

public:
    Stack() {
        top = NULL;
    }

    //inserts
    void push(string data) {

        StackNode* temp = new StackNode(data);

        //errcheck
        if (!temp) {
            cout << "\nStack Overflow\n";
            exit(1);
        }

        temp->data = data;
        temp->link = top;
        top = temp;
    }

    //checks
    bool isEmpty() {

        return top == NULL;
    }

    //peeks the top
    string peek() {

        if (!isEmpty()) //checking
            return top->data;
        else
            exit(1);
    }

    //pops
    void pop() {
        StackNode* temp;

        if (top == NULL) {
            cout << "\nStack Underflow\n" << endl;
            exit(1);
        }
        else {

            temp = top;
            top = top->link;
            delete(temp);
        }
    }

    //displays
    void display() {
        StackNode* temp;

        if (top == NULL) {
            cout << "\nStack Underflow";
            exit(1);
        }
        else {

            temp = top;

            while (temp != NULL) {

                cout << temp->data;
                temp = temp->link;
                if (temp != NULL)
                    cout << " -> ";
            }
            cout << "\n";
        }
    }
};


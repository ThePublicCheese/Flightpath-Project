#pragma once
#include <string>

using namespace std;
//nodes for linked lists only
class LinkedListNode {
private:
    string city;
    double cost, time;
    LinkedListNode* next;
public:
    LinkedListNode(string c, double cst, double t) {
        city = c;
        cost = cst;
        time = t;
        next = nullptr;
    }
    void setNext(LinkedListNode* node) {
        next = node;
    }
    void setCity(string c) {
        city = c;
    }
    string getCity() {
        return city;
    }
    LinkedListNode* getNext() {
        return next;
    }
    double getCost() {
        return cost;
    }
    double getTime() {
        return time;
    }
};

//linked list to represent destinations for a city
class LinkedList {

private:
    LinkedListNode* head;

public:
    LinkedList() {
        head = nullptr;
    }

    //adds node
    void addNode(string city, double cost, double time) {
        LinkedListNode* newNode = new LinkedListNode(city, cost, time);
        newNode->setNext(head);
        head = newNode;
    }

    //gets us our head
    LinkedListNode* getHead() const {
        return head;
    }

};


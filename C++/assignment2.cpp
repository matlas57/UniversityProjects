#include <iostream>
using namespace std;

/*
 * Node inner class for the datastore linked list
 */
class Node {
    public:
        int id;             // id to represent the store
        int ssize;          // size of the store 
        int *store_data;    // pointer to the data
        Node *next;         // pointer to the next node object
};

int nextID = 0;             // an integer variable that determines the ID to assign to every store, it is incremented whenever a store is created


int newstore (Node** head_ref, int ssize)
{
    // cant create a store of size 0, newstore() fails
    if (ssize <= 0)
    {
        return -1;
    }
    /*
     * Create and initialize fields of the new node 
     */
    Node* new_node = new Node();
    new_node->id = nextID; // set the id of the store
    nextID++;              // increment id field 
    new_node->ssize = ssize; // initialize the ssize field of the node
    new_node->store_data = new int[ssize](); // dynamically allocate memory for the store data
    new_node->next = NULL; // adding the node to the end of the linked list so next field is null

    Node* last = *head_ref; // to be used later when searching for the last node in the list

    // if the head node is null (meaning that the list is "empty") then set the head to the new_node
    if (*head_ref == NULL) 
    {
        *head_ref = new_node;
        return new_node->id;
    }

    // if there are already nodes in the list traverse until the last node and set its next pointer to point to the new node
    while (last->next != NULL)
    {
        last = last->next;
    }
    last->next = new_node; // change the pointer of the last node to the new node
    return new_node->id; 
}

int add_element_to_store(Node** head_ref, int id, int val, int idx = -1)
{
    // invalid ID cases, if the id is out of range of the number of stores 
    if (id <= -1 || id >= nextID)
    {
        cout << "Invalid ID passed" << endl;
        return -1;
    }
    Node* iterate = *head_ref; // a pointer to the head of the datastore linked list
    // iterate to the node with the passed ID
    while (iterate->id != id)
    {
        iterate = iterate->next;
    }
    // case 1: no index is passed add value to next open slot in the store
    int *data = iterate->store_data; // pointer to the store
    if (idx == -1)
    {
        // iterate through the store if an empty slot is found insert the value there
        for (int i = 0; i < iterate->ssize; i++)
        {
            if (*data == 0)
            {
                *data = val;
                return 0;
            }
            data++;
        }
        // if there are no available slots then adding fails and return -1
        cout << "No available slots to add value to store" << endl;
        return -1;
    }
    // case 2: index is passed and the slot is empty
    // if the index passed is greater than the size of the store it is being added to then addition fails
    if (idx >= iterate->ssize)
    {
        cout << "Index passed is greater than the size of the passed store" << endl;
        return -1;
    }
    data += idx; // increment the pointer to the index 
    if (*data == 0)
    {
        *data = val;
        return 0;
    }
    return -1;
}

void print_debug(Node** head_ref)
{
    // if the head node is null then there are no stores 
    if (*head_ref == NULL)
    {
        cout << "There are no stores initialized" << endl;
        return;
    }
    Node *iterate = *head_ref; // create a pointer to the head of the linked list
    // iterate through the stores and print their contents
    while (true)
    {
        cout << "Store " << iterate->id << " : ";
        int *ptr = iterate->store_data; // create a pointer to the store data
        int emptySlots = 0;             // variable to count the amount of empty slots in each store to print with contents
        for (int i = 0; i < iterate->ssize; i++)
        {
            int slot = *ptr;
            // if an empty slot is found increment available slots
            if (slot == 0)
            {
                emptySlots++;
            }
            cout << slot << " ";
            ptr++; // increment the pointer to go to the next value in the data store
        }
        cout << " This store has " << emptySlots << " empty slots available" << endl; // print the number of available slots
        cout << endl;
        // if the current node is the tail node break and exit the loop
        if (iterate->next == NULL)
        {
            break;
        }
        // if there are more nodes then set the iterating pointer to the next node
        iterate = iterate->next;
    } 
    cout << "#########################################" << endl;
}

void delete_element_from_store_by_value(Node** head_ref, int id, int val)
{
    Node *iterate = *head_ref; // create a pointer to the head node
    // iterate through the linked list to the passed ID
    for (int i = 0; i < id; i++)
    {
        iterate = iterate->next;
    }
    int *ptr = iterate->store_data; // create a pointer to the store data
    // iterate and search through the store, if the passed value is contained in the store delete it
    for (int i = 0; i < iterate->ssize; i++)
    {
        if (*ptr == val)
        {
            *ptr = 0;
        }
    }
}

void delete_element_from_store_by_index(Node** head_ref, int id, int idx)
{
    Node* iterate = *head_ref; // create a pointer to the head node
    // iterate through the linked list to the passed ID
    for (int i = 0; i < id; i++)
    {
        iterate = iterate->next;
    }
    // cheack if the index passed is within the range of the store
    if (!(idx >= iterate->ssize))
    {
        int *ptr = iterate->store_data; // create a pointer to the store data
        ptr += idx; // increment the pointer to the passed index
        *ptr = 0; // delete the value
    }
}

void which_stores_have_element(Node** head_ref, int val)
{
    int found = -1; // counts the number of times the value has been found, initially set to -1 to indicate that no instances have been found
    Node* iterate = *head_ref; // create a pointer to the head node
    int *IDsToPrint = new int[100]; // create a dynamically allocated array to store the IDs where the value has been found
    int *headOfIDs = IDsToPrint; // create a pointer to iterate through the IDs array
    // iterate through all the stores and save any ID where a value is found
    while (true)
    {
        int *ptr = iterate->store_data; // pointer for the data on each store
        for (int i = 0; i < iterate->ssize; i++) // iterate through the current store
        {
            if (*ptr == val)
            {
                found++;
                *IDsToPrint = iterate->id;
                IDsToPrint++;
            }
            ptr++;
        }
        if (iterate->next == NULL)
        {
            break;
        }
        iterate = iterate->next;
    }
    if (found == -1)
    {
        cout << "Element " << val << " is not available in any store" << endl;
        return;
    }
    cout << "Element " << val << " is in store IDs : ";
    for (int i = 0; i < found; i++)
    {
        cout << *headOfIDs << ", ";
        headOfIDs++;
    } 
    cout << *headOfIDs << endl;
}

void delete_store(Node** head_ref, int id)
{
    if (id < 0 || id >= nextID)
    {
        cout << "Invalid index passed" << endl;
        return;
    }
    Node* iterate = *head_ref;
    for (int i = 0; i < id - 1; i++)
    {
        iterate = iterate->next;
    }
    Node* prevNode = iterate;
    Node* nodeToDelete = iterate->next;
    delete[] nodeToDelete->store_data; // un-allocate the dynamicly allocated array to store data
    // if deleting the head store
    if (!nodeToDelete->next == NULL)
    {
        prevNode->next == nodeToDelete->next;
    }
    else 
    {
        prevNode->next = NULL;
    }
}

int resize_store(Node** head_ref, int id, int newsize)
{
    cout << "resizing store with ID " << id << endl;
    Node* iterate = *head_ref;
    for (int i = 0; i < id - 1; i++)
    {
        iterate = iterate->next;
    }
    cout << "prev node has ID " << iterate->id << endl;
    Node* prev_node = iterate;
    iterate = iterate->next;
    int *new_store = new int[newsize]();
    cout << "New store is created first element has value " << *new_store << endl;
    if (newsize > iterate->ssize)
    {
        cout << "Creating a larger store" << endl;
        int *oldptr = iterate->store_data;
        int *newptr = new_store;
        for (int i = 0; i < iterate->ssize; i++)
        {
            *oldptr = *newptr;
            oldptr++;
            newptr++;
        }
        iterate->store_data = new_store;
    }
    else if (iterate->ssize > newsize)
    {
        cout << "Creating a smaller store" << endl;
        int *oldptr = iterate->store_data;
        int *newptr = new_store;
        for (int i = 0; i < newsize; i++)
        {
            *oldptr = *newptr;
            oldptr++;
            newptr++;
        }
        iterate->store_data = new_store;
    }
    cout << "done with resize" << endl;
    return 0;
}

int main()
{
    Node* head = NULL;

    int s1 = newstore(&head, 3);
    int s2 = newstore(&head, 5);

    if (s1 != -1)
    {
        add_element_to_store(&head, 0, 13);
        add_element_to_store(&head, 0, 15);
        add_element_to_store(&head, 0, 21);
        add_element_to_store(&head, 0, 42); // should fail (right now does)
    }
    if (s2 != -1)
    {
        add_element_to_store(&head, 1, 7, 2);
        add_element_to_store(&head, 1, 15, 0);
        add_element_to_store(&head, 1, 22, 1);
    }
    print_debug(&head);

    delete_element_from_store_by_value(&head, s1, 13);
    delete_element_from_store_by_value(&head, s1, 71);
    delete_element_from_store_by_index(&head, s2, 2);
    delete_element_from_store_by_index(&head, s1, 5);
    print_debug(&head);
    
    which_stores_have_element(&head, 15);
    which_stores_have_element(&head, 29);

    delete_store(&head, s1);
    print_debug(&head);

    resize_store(&head, s2, 20);
    print_debug(&head);
}
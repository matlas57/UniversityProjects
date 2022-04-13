#include <iostream>
using namespace std;

#define ARRSIZE 50
int datastore[ARRSIZE] = {};
int newStoreReturnValue = 0; // This value increments every time a new store is created and represents the next store ID
int nextValidIndex = 0;      // This value holds where the next store to be created in datastore   
int startingIndex[ARRSIZE] = {}; // This array holds where each store starts in the datastore array
int storeSizes[1000] = {};   // This holds the size of all stores 



// Question 1
int newstore(int ssize)
{
    // cant create a store of size less than or equal to zero
    // cant create a store if there is not enough space in datastore
    if (ssize <= 0 || nextValidIndex + ssize > ARRSIZE)
    {
        return -1;
    }
    startingIndex[newStoreReturnValue] = nextValidIndex; // set where the created store begins 
    storeSizes[newStoreReturnValue] = ssize;             // set the size of the created store
    nextValidIndex += ssize;                             // set the next index for the next store to be created
    return newStoreReturnValue++;
}

// Question 2
int add_element_to_store(int id, int val, int idx=-1)
{
    // if the ID is invalid of the index is passed is greater than the size of the store
    if (id <= -1 || idx >= storeSizes[id])
    {
        return -1;
    }
    // If there is no index passed add it to the first available store
    // if there is no space then return -1
    if (idx == -1)
    {
        for (int i = startingIndex[id]; i < storeSizes[id]; i++)
        {
            if (datastore[i] == 0)
            {
                datastore[i] = val;
                return 0;
            }
        }
        return -1;
    }
    // If there is a slot passed and the slot doesnt contain an element
    int slot = startingIndex[id] + idx;
    if (datastore[slot] == 0)
    {
        datastore[slot] = val;
        return 0;
    }  
    return -1;
}

// Question 3
void print_debug()
{
    // calculate the available elements by subtracting the store sizes of the initialized stores
    int availableElements = ARRSIZE;
    for (int i = 0; i < 1000; i++)
    {
        if (storeSizes[i] != -1)
        {
            availableElements -= storeSizes[i];
        }
    }
    cout << "available elements in datastore: " << availableElements << endl;
    cout << endl;
    cout << "datastore : " << " ";
    // print out all the elements in datastore
    for (int i = 0; i < ARRSIZE; i++)
    {
        cout << datastore[i] << " ";
    }
    cout << endl;
    cout << endl;
    // iterate through all the stores and print out their respective elements
    for (int i = 0; i < ARRSIZE; i++)
    {
        if (storeSizes[i] > 0)
        {
            cout << "store " << i << ": ";
            for (int j = startingIndex[i]; j < startingIndex[i] + storeSizes[i]; j++)
            {
                cout << datastore[j] << " ";
            }
            cout << endl;  
            cout << endl;   
        }
        
    }
    cout << "#####################################" << endl;
    cout << "" << endl;
}

// Question 4
void delete_element_from_store_by_value(int id, int val)
{
    // iterate through datastore and find all instances of the passed value
    for (int i = startingIndex[id]; i < startingIndex[id] + storeSizes[id]; i++)
    {
        if (datastore[i] == val)
        {
            datastore[i] = 0;
        }
    }
}

void delete_element_from_store_by_index(int id, int idx)
{
    // first check if the index given is valid by comparing it to the size if the store 
    if (!(idx >= storeSizes[id]))
    {
        datastore[startingIndex[id] + idx] = 0;
    }
}

// Question 5
void which_stores_have_element(int val)
{
    int found = -1; // counts the number of times the value is found in datastore to determine what to print 
    int IDsToPrint[ARRSIZE] = {};
    // create an array of the ids with the given element and set all elements to -1
    // elements set to -1 to distinguish from ids to be printed
    for (int i = 0; i < ARRSIZE; i++)
    {
        IDsToPrint[i] = -1;
    }
    // iterate through the datastore and find the elements
    for (int i = 0; i < ARRSIZE; i++)
    {
        if (datastore[i] == val)
        {  
            for (int j = 0; j < ARRSIZE; j++)
            {
                // find the ID that the element belongs to by checking if is in the range of each store
                // once its found break and find the next instance of the element
                if (i >= startingIndex[j] && i < startingIndex[j] + storeSizes[j])
                {
                    found++;
                    IDsToPrint[found] = j;
                    break;
                }
            }     
        }
    }
    // If no instances of the value was found print the following line
    if (found == -1)
    {
        cout << "Element " << val << " is not available in any store" << endl;
    }
    // go through the IDs found and print them 
    else 
    {
        cout << "Element " << val << " is in store IDs: " << " ";
        for (int i = 0; i < ARRSIZE - 1; i++)
        {
            if (IDsToPrint[i + 1] == -1)
            {
                cout << IDsToPrint[i] <<  endl;
                break;
            }
            else 
            {
                cout << IDsToPrint[i] << ", ";
            }
        }
        if (IDsToPrint[ARRSIZE - 1] != -1)
        {
            cout << IDsToPrint[ARRSIZE - 1] << endl;
        }
    } 
    cout << endl; 
}

// Question 6
void delete_store(int id)
{
    // provide your implementation
    // first set all elements in store to 0 by iterating from starting index to starting index + store size
    // then go through all stores after the one deleted and shift each element by storesize of deleted id 
    // after shifting set the old position of that element to 0
    for (int i = startingIndex[id]; i < startingIndex[id] + storeSizes[id]; i ++)
    {
        datastore[i] = 0;
    }
    int startOfNextStore = startingIndex[id + 1];
    for (int i = startOfNextStore; i < ARRSIZE; i++)
    {
        datastore[i - storeSizes[id]] = datastore[i];
        datastore[i] = 0;
    }
    for (int i = id + 1; i < ARRSIZE; i++)
    {
        startingIndex[i] -= storeSizes[id];
    } 
    nextValidIndex -= storeSizes[id];
    storeSizes[id] = -1;
    //startingIndex[id] = -1;
}

// Question 7
int resize_store(int id, int newsize)
{
    // Case 1: increasing the size of a store
    if (newsize > storeSizes[id])
    {
        // 1. calculate available elements in datastore and confirm that their are enough slots to resize the store
        int availableElements = ARRSIZE;
        for (int i = 0; i < 1000; i++)
        {
            if (storeSizes[i] != -1)
            {
                availableElements -= storeSizes[i];
            }
        }
        // not enough slots to resize, return -1
        if (newsize > availableElements)
        {
            return -1;
        }
        // 2. shift all elements to the right of the store to be resized, shift from left to right to ensure no values are overwritten
        int shift = newsize - storeSizes[id];
        for (int i = ARRSIZE - shift - 1; i > id + storeSizes[id] - 1; i--)
        {
            datastore[i + shift] = datastore[i];
            datastore[i] = 0;
        }
        storeSizes[id] = newsize; // update the size of the store
        // 3. shift all starting indices of the stores after the one resized
        for (int i = id + 1; i < ARRSIZE; i++)
        {
            startingIndex[i] += shift;
        }
        nextValidIndex += shift;
        return 0;
    }
    // Case 2: decreasing the size of the store
    else if (newsize < storeSizes[id])
    {
        // 1. make sure the new size is valid
        // if the size is negative resize fails
        if (newsize < 0)
        {
            return -1;
        } 
        // if the newsize if zero it is equivilant to deleting the store so call the delete_store
        else if (newsize == 0)
        {
            delete_store(id);
            return 0;
        }
        // 2. If the store size is decreasing delete elements 
        for (int i = startingIndex[id] + newsize; i < storeSizes[id]; i++)
        {
            datastore[i] = 0;
        }
        // 3. shift all elements after the store to be resized to the left 
        int shift = storeSizes[id] - newsize;
        for (int i = startingIndex[id] + newsize; i < ARRSIZE - shift; i++)
        {
            datastore[i] = datastore[i + shift];
        }
        // 4. update store sizes and starting indices
        storeSizes[id] = newsize;
        for (int i = id + 1; i < ARRSIZE; i++)
        {
            startingIndex[i] -= shift;
        }
        nextValidIndex -= shift;
        return 0;
    }
    // Case 3: if the new size is equal to the stores current size, do nothing
    else 
    {
        return 0;
    }
}


// DO NOT ADD ANY LOGIC TO THE MAIN FUNCTION.
// YOU MAY MODIFY FOR YOUR OWN TESTS ONLY BUT THE ORIGINAL MAIN
// FUNCTION SHOULD BE SUBMITTED AS IS
int main()
{
    int s1 = newstore(3); // create new empty data store of size 3
    int s2 = newstore(5); // create new empty data store of size 5

    if (s1 != -1)
    {
        add_element_to_store(s1, 13);
        add_element_to_store(s1, 15);
        add_element_to_store(s1, 21);
        add_element_to_store(s1, 42); // this should return -1
    }
    
    if (s2 != -1)
    {
        add_element_to_store(s2, 7, 2);
        add_element_to_store(s2, 15, 0);
        add_element_to_store(s2, 22, 1);
    }
    print_debug();

    delete_element_from_store_by_value(s1, 13);
    delete_element_from_store_by_value(s1, 71);
    delete_element_from_store_by_index(s2, 2);
    delete_element_from_store_by_index(s1, 5);
    print_debug();

    which_stores_have_element(15);
    which_stores_have_element(29);

    delete_store(s1);
    print_debug();

    resize_store(s2, 20);
    int s3 = newstore(40);
    print_debug();

    s3 = newstore(30);
    add_element_to_store(s3, 7, 29);
    print_debug();
    
}
#include <iostream>
#include <vector>       // needed to use vector container for hands and decks
#include <algorithm>    // needed for shuffling a deck

using namespace std;

// enum types to represent the suit and rank of a card
enum Rank {ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ENDR};
enum Suit {CLUBS, DIAMONDS, HEARTS, SPADES, ENDS};

//storing the number of casino wins and total rounds to calculate win rate
int casinoWins = 0;
int totalRounds = 0;
bool riggingNextRound = false;

// represents a card to be used in the game
class Card
{
    public:
        // fields for a card
        Rank rank;
        Suit suit;
        // Card constructor via initialization list
        Card (Rank r, Suit s):rank(r), suit(s){} 
        
        /*
         * returns the numerical value of the card for the game based on the cards rank 
         * TEN, JACK, QUEEN, and KING all equal 10
         */
        int getValue()
        {
            if (rank == ACE)
            {
                return 1;
            }
            else if (rank == TWO)
            {
                return 2;
            }
            else if (rank == THREE)
            {
                return 3;
            }
            else if (rank == FOUR)
            {
                return 4;
            }
            else if (rank == FIVE)
            {
                return 5;
            }
            else if (rank == SIX)
            {
                return 6;
            }
            else if (rank == SEVEN)
            {
                return 7;
            }
            else if (rank == EIGHT)
            {
                return 8;
            }
            else if (rank == NINE)
            {
                return 9;
            }
            else 
            {
                return 10;
            }
        }
        // prints the representation of the card in the format [card value][first letter of suit]
        void displayCard()
        {
            // print the value concatented with the first letter of the suit
            if (suit == CLUBS)
            {
                cout << RankAsString() << "C";
            }
            else if (suit == DIAMONDS)
            {
                cout << RankAsString() << "D";
            }
            else if (suit == HEARTS)
            {
                cout << RankAsString() << "H";
            }
            else if (suit == SPADES)
            {
                cout << RankAsString() << "S";
            }
        }

        /*
         *  This is a helper method for displayCard()
         *  Based on the suit it will return a string equal to the first letter of that suit
         */
        string RankAsString()
        {
            if (rank == JACK)
            {
                return "J";
            }
            else if (rank == QUEEN)
            {
                return "Q";
            }
            else if (rank == KING)
            {
                return "K";
            }
            else 
            {
                int r = rank;
                r++;
                return to_string(r);
            }
        }
   
};

class Hand
{
    public:
        // Use a vector to represent the hand because they can be resized automatically which will be helpful when adding cards to a hand
        std::vector<Card> hand;

        // add a card to the end of the hand using the vector.push_back(Object o) method
        void add(Card *c)
        {
            hand.push_back(*c);
        }

        // clear the hand using list.clear() method
        void clear()
        {
            hand.clear();
        }
        // iterate through the hand and sum all the card values using the getValue() method
        // may need changes to decide ACE value
        int getTotal()
        {
            int sum = 0; //int to store the sum of card values
            int aces = 0;//int to store how many aces are in the hand which will be handles after all other cards are added to the sum
            
            //iterate through the hand if the card is an ace increment aces and dont add anything, otherwise add the card's value
            for (Card c : hand)
            {
                if (c.rank == ACE)
                {
                    aces++;
                }
                else
                {
                    sum += c.getValue();
                }
            }
            //below decides whether to choose 1 or 11 as an aces value
            if (aces > 0)
            {
                // iterate through each ace and decide its value
                for (int i = 0; i < aces; i++)
                {
                    // Case 1: if deciding 11 for the value would make the hand bust then choose 1
                    if (sum > 10)
                    {
                        sum++;
                    }
                    // Case 2: if choosing 11 for the current ace and choosing one for the rest would result in a non-bust then add 11 + remaining aces
                    else if (sum <= 10 && aces - i < 2)
                    {
                        sum += 11;
                    }
                    // Case 3: default: choose 1
                    else 
                    {
                        sum++;
                    }
                }
            }
            return sum;
        }
};

//Represents the game deck
class Deck
{
    public:
        //use a vector to stay consistent with data structure throughout the game
        std::vector<Card> deck;
        //add the standard 52 cards to the deck
        void populate()
        {
            //iterate through the enum types for rank and suit and create all the cards
            for (int i = Rank::ACE; i != Rank::ENDR; ++i)
            {
                Rank r = static_cast<Rank>(i); // casts the index of the enum to the actual enum value at that index
                for (int j = Suit::CLUBS; j != Suit::ENDS; ++j)
                {
                    Suit s = static_cast<Suit>(j); // casts the index of the enum to the actual enum value at that index
                    Card *c;                       // create a pointer to a new card
                    c = new Card(r,s);             // initialize a card for the pointer to point to
                    deck.push_back(*c);            // add a reference to that card to the deck 
                }
            }
        }

        // shuffle the cards in the deck
        void shuffle() 
        {
            std::random_shuffle(begin(deck), end(deck));
        };

        // take the card at the back of the deck and add it to the passed hand, then remove it from the deck
        void deal (Hand &h)
        {
            Card toDeal = deck.back();
            h.add(&toDeal);
            deck.pop_back();
        }

        void riggedDeal(Hand &h)
        {
            int handTotal = h.getTotal();
            for (int i = 0; i < deck.size(); i++)
            {
                int curVal = deck[i].getValue();
                if (handTotal + curVal == 21)
                {
                    h.add(&deck[i]);
                    deck.erase(deck.begin() + i);
                    return;
                }
            }
        }
};

//Abstract player class human and casino players will inherit this class which will give them a hand and the isDrawing() and isBusted() methods
class AbstractPlayer
{
    public:
        Hand* h;
        
        // constructor: initialized the hand pointer
        AbstractPlayer() {
            
            h = new Hand();
        }

        //pure virtual method, all derived classes must provide an implementation
        virtual bool isDrawing() const = 0;

        //a hand is busted if the hands total value is greater than 21
        bool isBusted()
        {
            if (h->getTotal() > 21)
            {
                return true;
            }
            return false;
        }
};

class HumanPlayer: public AbstractPlayer
{
    public:

        /*
         * Implementation of the isDrawing() method specific to the HumanPlayer
         * after each draw print the cards and total value and ask if the player would like to draw another card
         */
        virtual bool isDrawing() const
        {
            char response;
            cout << "Do you want to draw? (y/n)" << endl;
            cin >> response;    //retreive user input
            //keep drawing
            if (response == 'y')
            {
                return true;
            }
            //hold
            else if (response == 'n')
            {
                return false;
            }
            // if a y or n is not entered input is invalid and ask again
            else 
            {
                cout << "Invalid response" << endl;
                return isDrawing();
            }
        }

        void announce()
        {
            cout << "Human player's hand has a value of " << h->getTotal() << endl;
        }
};

class ComputerPlayer: public AbstractPlayer
{
    public:

        /*
         * Implementation of the isDrawing() method specific to the ComputerPlayer
         * Computer keeps drawing as long as its total is 16 or less
         */
        bool isDrawing() const
        {
            if (h->getTotal() <= 16)
            {
                return true;
            }
            return false;
        }
};

// Class that manages gameplay
class BlackJackGame
{
    public:
        // fields for the 
        Deck* m_deck;               //pointer to a deck to be used for cards
        ComputerPlayer* m_casino;   //Casino object
        HumanPlayer* m_human;       //HumanPlayer

        //Constructor
        BlackJackGame() 
        {
            m_deck = new Deck();
            m_casino = new ComputerPlayer();
            m_human = new HumanPlayer();
        }
        void play()
        {
            //increment total number of rounds
            totalRounds++;
            //Set up the deck
            m_deck->populate();
            m_deck->shuffle();
            //Deal first cards to human and casino (alternate between human and casino per proper dealing protocol)
            Hand cHand = *m_casino->h;
            Hand pHand = *m_human->h;
            m_deck->deal(pHand);
            m_deck->deal(cHand);
            m_deck->deal(pHand);
            m_deck->deal(cHand);
            //Print the casinos first card and its value
            cout << "Casino: ";
            cHand.hand[0].displayCard();
            cout << " [" << cHand.hand[0].getValue() << "]" << endl;
            //Print the players card and their total value
            cout << "Player: ";
            pHand.hand[0].displayCard();
            cout << " ";
            pHand.hand[1].displayCard();
            cout << " [" << pHand.getTotal() << "]" << endl;
            //Player goes first and draws cards until they bust, reach 21, or hold
            while (m_human->isDrawing())
            {
                m_deck->deal(pHand); //deal a card
                //print the hand and the hands value
                cout << "Player: ";
                for (int i = 0; i < pHand.hand.size(); i++)
                {
                    pHand.hand[i].displayCard();
                    cout << " ";
                }
                cout << " [" << pHand.getTotal() << "]" << endl;
                
                //if the player busts, casino wins
                if (pHand.getTotal() > 21)
                {
                    cout << "Player busts." << endl;
                    cout << "Casino wins." << endl;
                    casinoWins++;
                    m_deck->deck.clear(); //clear the deck to prepare for the next round
                    return;
                }
                //if they reach 21 they automatically hold since that is the best possible hand and play diverts to the casino
                else if (pHand.getTotal() == 21)
                {
                    break;
                }
            }
            //casino drawing turn
            while (m_casino->isDrawing())
            {
                /* 
                 * after 4 rounds calculate the winrate and rig the dealing if the previous round decides that it is appropriate
                 * if a round is rigged then the next round wont be rigged to avoid suspicion
                 */   
                if (totalRounds > 4  && riggingNextRound)
                {
                    //cout << "Rigged" << endl;
                    m_deck->riggedDeal(cHand);
                    riggingNextRound = false;
                }
                else 
                {
                    m_deck->deal(cHand); //deal a card to casino's hand
                }
                //print the casino's hand and its total value
                cout << "Casino: ";
                for (int i = 0; i < cHand.hand.size(); i++)
                {
                    cHand.hand[i].displayCard();
                    cout << " ";
                }
                cout << " [" << cHand.getTotal() << "]" << endl;
                //if the casino busts, player wins
                if (cHand.getTotal() > 21)
                {
                    cout << "Casino busts." << endl;
                    cout << "Player wins." << endl;
                    if (casinoWins/totalRounds < .55)
                    {
                        riggingNextRound = true;
                    }
                    m_deck->deck.clear(); //clear the deck to prepare for the next round
                    return;
                }
                //if the hand's total is greater than 16 casino must hold
                else if (cHand.getTotal() > 16)
                {
                    break;
                }               
            }
            //If code reaches this point neither the casino or player has busted, so compare the hand totals
            if (cHand.getTotal() > pHand.getTotal())
            {
                casinoWins++;
                cout << "Casino wins." << endl;
            }
            else if (cHand.getTotal() < pHand.getTotal())
            {
                if (casinoWins/totalRounds < .55)
                {
                    riggingNextRound = true;
                }
                cout << "Player wins." << endl;
            }
            // if the totals are equal we have a push situation
            else 
            {
                cout << "Push: no one wins." << endl;
            }
            m_deck->deck.clear(); //clear the deck to prepare for the next round
        }
};

int main()
{
    cout << "\tWelcome to the Comp322 Blackjack game!" << endl << endl;
    BlackJackGame game;
    // The main loop of the game
    bool playAgain = true;
    char answer = 'y';
    while (playAgain)
    {
        game.play();
        // Check whether the player would like to play another round
        cout << "Would you like another round? (y/n): ";
        cin >> answer;
        cout << endl << endl;
        playAgain = (answer == 'y' ? true : false);
    }
    cout <<"Gave over!";
    return 0;
}
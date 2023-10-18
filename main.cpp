/* Program name: M08 Final Project
 *  Author: James Ramsey
 *  Date last updated: 5/05/2023
 * Purpose: Play the game calculation
 */

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <limits.h>

using namespace std;

enum faceType
{
    ace,
    two,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    jack,
    queen,
    king,
    faceNull,
};

enum suitType
{
    diamonds,
    clubs,
    hearts,
    spades,
    suitNull,
};

struct cardType
{
    faceType face;
    suitType suit;
};

struct deck
{
    cardType myDeck[52];
};

const int FOUNDATION_COUNT = 4;
const int FOUNDATION_LENGTH = 13;

const int WASTE_COUNT = 4;
const int WASTE_LENGTH = 52;

cardType foundations[FOUNDATION_COUNT][FOUNDATION_LENGTH];
cardType waste[WASTE_COUNT][WASTE_LENGTH];

struct foundationPileStruct
{
    string displayName = "F";
    cardType foundationPile[13];
    int count = 0;
};

struct wastePileStruct
{
    string displayName = "W";
    cardType pile[52];
    int count = 0;
};

#pragma region Function_Calls
void intializeCards(deck &decks);
string toStringFaces(faceType face);
string toStringSuits(suitType suit);
void invalidInput();
void runGame(deck &deck, foundationPileStruct foundationPiles[FOUNDATION_COUNT], wastePileStruct wastePiles[WASTE_COUNT]);
void removeElementFromDeck(deck &deck, int element);
string displayFoundationPile(foundationPileStruct pile);
string displayWastePile(wastePileStruct pile);
void showGamePiles(foundationPileStruct foundationPiles[FOUNDATION_COUNT], wastePileStruct wastePiles[WASTE_COUNT]);
int showMenuForDrawOptions(foundationPileStruct foundationPiles[FOUNDATION_COUNT], bool hasWasteCards, deck &deck, wastePileStruct wastePiles[WASTE_COUNT]);
int showMenuForCardPlayOptions(bool hasWasteCards, deck &deck, int drawnCard, wastePileStruct wastePiles[WASTE_COUNT], foundationPileStruct foundationPiles[FOUNDATION_COUNT]);
void showWastePile(wastePileStruct wastePiles[WASTE_COUNT]);
int getRemainingCardCount(deck &deck);
void showRemainingCards(deck &deck);
int playOnFoundation(foundationPileStruct foundationPiles[FOUNDATION_COUNT], deck &deck, int drawnCard);
void playOnWaste(wastePileStruct wastePiles[WASTE_COUNT], deck &deck, int drawnCard);
bool pileValidation(int pile, foundationPileStruct foundationPiles[FOUNDATION_COUNT], cardType cardType);
void inializeWastePile(wastePileStruct wastePiles[WASTE_COUNT]);
bool checkWastePile(wastePileStruct wastePiles[WASTE_COUNT]);
void inializeFoundationPile(foundationPileStruct foundationPiles[FOUNDATION_COUNT]);
int playFromWaste();
void playOnFoundationFromWaste(foundationPileStruct foundationPiles[FOUNDATION_COUNT], wastePileStruct &wastePile);
void removeTopCardFromWaste(wastePileStruct &wastePile);
void calculateScore(wastePileStruct wastePiles[WASTE_COUNT]);
int showMenuForFinalOptions(int drawnCard, wastePileStruct wastePiles[WASTE_COUNT], foundationPileStruct foundationPiles[FOUNDATION_COUNT]);
bool playGameAgain();
#pragma endregion

int main()
{
    deck card;
    int select = 0;
    char choice = ' ';
    foundationPileStruct foundationPiles[FOUNDATION_COUNT];
    wastePileStruct wastePiles[WASTE_COUNT];
    bool playAgain = true;

    for (int i = 0; i < sizeof(foundationPiles) / sizeof(foundationPiles[0]); i++)
    {
        foundationPiles[i].displayName += to_string(i + 1);
    }

    for (int i = 0; i < WASTE_COUNT; i++)
    {
        wastePiles[i].displayName += to_string(i + 1);
    }

    // start game here
    while (playAgain == true)
    {

        intializeCards(card);
        inializeWastePile(wastePiles);
        inializeFoundationPile(foundationPiles);
        // main menu output
        cout << "Welcome to Calculation!" << endl;
        cout << "1. Start Game" << endl;
        cout << "2. Options" << endl;
        cout << "3. Quit" << endl;
        if (select == 0)
        {
            cin >> select;
        }
        while (select != 888)
        {
            while (!cin || select > 3 || select < 1)
            {
                invalidInput();
                cout << "You entered an invalid input. Please Try again." << endl;
                cin >> select;
            }
            if (select == 1)
            {
                cout << "Are you ready to play?" << endl;
                if (choice == ' ')
                {
                    cin >> choice;
                }

                choice = (char)tolower(choice);
                while (!cin || choice != 'n' && choice != 'y')
                {
                    invalidInput();
                    cout << "Please enter Y or N." << endl;
                    cin >> choice;
                }
                if (choice == 'y')
                {
                    runGame(card, foundationPiles, wastePiles);
                    break;
                }
                if (choice == 'n')
                {
                    break;
                }
            }
            if (select == 2)
            {
                cout << "options" << endl;
                break;
            }
            if (select == 3)
            {
                playAgain = false;
                select = 888;
            }
        }
        if (select != 888)
        {
            playAgain = playGameAgain();
            if (playAgain == true)
            {
                select = 1;
                choice = 'y';
            }
        }
    }

    return 0;
}

void runGame(deck &deck, foundationPileStruct foundationPiles[FOUNDATION_COUNT], wastePileStruct wastePiles[WASTE_COUNT])
{
    int A;
    int two;
    int three;
    int four;
    int drawnCard = 888;
    bool cardInWastePile;

    // build base for foundations
    cout << "Enter the card number for the starting A: " << endl;
    cin >> A;
    while (!cin || A != 0 && A != 13 && A != 26 && A != 39)
    {
        invalidInput();
        cout << "Invalid input, please choose an ace so numbers 0, 13, 26, or 39" << endl;
        cin >> A;
    }
    cout << "Enter the card number for the starting 2: " << endl;
    cin >> two;
    while (!cin || two != 1 && two != 14 && two != 27 && two != 40)
    {
        invalidInput();
        cout << "Invalid input, please choose a two so numbers 1, 14, 27, or 40" << endl;
        cin >> two;
    }
    cout << "Enter the card number for the starting 3: " << endl;
    cin >> three;
    while (!cin || three != 2 && three != 15 && three != 28 && three != 41)
    {
        invalidInput();
        cout << "Invalid input, please choose a three so numbers 2, 15, 28, or 41" << endl;
        cin >> three;
    }
    cout << "Enter the card number for the starting 4: " << endl;
    cin >> four;
    while (!cin || four != 3 && four != 16 && four != 29 && four != 42)
    {
        invalidInput();
        cout << "Invalid input, please choose a four so numbers 3, 16, 29, or 42" << endl;
        cin >> four;
    }

    // set the the chosen cards to the base of the foundation piles
    int index = 0;
    foundationPiles[index].foundationPile[foundationPiles[index].count] = deck.myDeck[A];
    foundationPiles[index].count++;

    index = 1;
    foundationPiles[index].foundationPile[foundationPiles[index].count] = deck.myDeck[two];
    foundationPiles[index].count++;

    index = 2;
    foundationPiles[index].foundationPile[foundationPiles[index].count] = deck.myDeck[three];
    foundationPiles[index].count++;

    index = 3;
    foundationPiles[index].foundationPile[foundationPiles[index].count] = deck.myDeck[four];
    foundationPiles[index].count++;

    removeElementFromDeck(deck, A);
    removeElementFromDeck(deck, two);
    removeElementFromDeck(deck, three);
    removeElementFromDeck(deck, four);

    // while Loop right here
    // psudeoCode while all deck.myDeck index != " "  or  drawn card = -1 exit function (ignore old version)
    do
    {
        if (drawnCard == 888)
        {
            cardInWastePile = checkWastePile(wastePiles);
            showGamePiles(foundationPiles, wastePiles);
            showRemainingCards(deck);
            drawnCard = showMenuForDrawOptions(foundationPiles, cardInWastePile, deck, wastePiles);
        }

        if (drawnCard != 888 && drawnCard != -1)
        {
            cardInWastePile = checkWastePile(wastePiles);
            showGamePiles(foundationPiles, wastePiles);
            cout << "Drawn Card: " << toStringFaces(deck.myDeck[drawnCard].face) << toStringSuits(deck.myDeck[drawnCard].suit) << endl;
            drawnCard = showMenuForCardPlayOptions(cardInWastePile, deck, drawnCard, wastePiles, foundationPiles);
        }
    } while (getRemainingCardCount(deck) > 0 && drawnCard != -1);

    while (getRemainingCardCount(deck) == 0 && drawnCard != -1)
    {
        cardInWastePile = checkWastePile(wastePiles);
        showGamePiles(foundationPiles, wastePiles);
        showRemainingCards(deck);
        drawnCard = showMenuForFinalOptions(drawnCard, wastePiles, foundationPiles);
    }
}

#pragma region constantly_updates_and_shows_without_choice
string displayFoundationPile(foundationPileStruct pile)
{
    int lastIndex = max(pile.count - 1, 0);
    return pile.displayName + ": " + toStringFaces(pile.foundationPile[lastIndex].face) + toStringSuits(pile.foundationPile[lastIndex].suit) + " ";
}

string displayWastePile(wastePileStruct pile)
{
    int lastIndex = max(pile.count - 1, 0);
    return pile.displayName + ": " + toStringFaces(pile.pile[lastIndex].face) + toStringSuits(pile.pile[lastIndex].suit) + " ";
}

int getRemainingCardCount(deck &deck)
{
    int j = 0;
    for (int i = 0; i < 52; i++)
    {
        if (deck.myDeck[i].face == faceNull)
        {
            continue;
        }
        j++;
    }
    return j;
}

void showRemainingCards(deck &deck)
{
    int remaining = getRemainingCardCount(deck);
    cout << "Remaining Cards (" << remaining << ")" << endl;
}

void showGamePiles(foundationPileStruct foundationPiles[FOUNDATION_COUNT], wastePileStruct wastePiles[WASTE_COUNT])
{

    string foundationDeckDisplay = "";
    for (int i = 0; i < FOUNDATION_COUNT; i++)
    {
        foundationDeckDisplay += displayFoundationPile(foundationPiles[i]);
    }
    cout << foundationDeckDisplay << endl;

    string wasteDeckDisplay = "";
    for (int i = 0; i < WASTE_COUNT; i++)
    {
        wasteDeckDisplay += displayWastePile(wastePiles[i]);
    }
    cout << wasteDeckDisplay << endl;
}
#pragma endregion

#pragma region only_shows_if_the_option_is_chosen
int showMenuForDrawOptions(foundationPileStruct foundationPiles[FOUNDATION_COUNT], bool hasWasteCards, deck &deck, wastePileStruct wastePiles[WASTE_COUNT])
{
    int chooseOption;
    int drawnCard;
    cout << "What would you like to do: (Enter -1 to quit)" << endl;
    cout << "1. Draw Card" << endl;
    // if they have waste cards, show option for play on waste
    if (hasWasteCards == true)
    {
        cout << "2. Play Card from Waste Pile" << endl;
        cout << "3. Display an entire waste pile" << endl;
    }

    cin >> chooseOption;
    while (!cin || (chooseOption > 3 || chooseOption < 1) && chooseOption != -1)
    {
        invalidInput();
        cout << "choose a number between 1-3" << endl;
        cin >> chooseOption;
    }

    while (!cin || (hasWasteCards == false && chooseOption >= 2 || hasWasteCards == false && chooseOption < 1) && chooseOption != -1)
    {
        invalidInput();
        cout << "Invalid choice" << endl;
        cin >> chooseOption;
    }

    switch (chooseOption)
    {
    case 1:
        cout << "Enter the card number for the drawn card: " << endl;
        cin >> drawnCard;
        while (!cin || deck.myDeck[drawnCard].face == faceNull)
        {
            invalidInput();
            cout << "Please choose a card not already chosen" << endl;
            cin >> drawnCard;
        }
        return drawnCard;
    case 2:
    {
        // get the waste pile index from which to pull
        int wastePileIndex = playFromWaste();

        playOnFoundationFromWaste(foundationPiles, wastePiles[wastePileIndex]);
        return 888;
    }
    case 3:
        showWastePile(wastePiles);
        return 888;
    case -1:
        return -1;
    default:
        return 888;
    }
}

int showMenuForCardPlayOptions(bool hasWasteCards, deck &deck, int drawnCard, wastePileStruct wastePiles[WASTE_COUNT], foundationPileStruct foundationPiles[FOUNDATION_COUNT])
{
    int PoF;
    int chooseOption;
    char choosePile;
    cout << "What would you like to do: (Enter -1 to quit)" << endl;
    cout << "1. Play Drawn Card" << endl;
    // if they have waste cards, show option for play on waste
    if (hasWasteCards == true)
    {
        cout << "2. Play Card from Waste Pile" << endl;
        cout << "3. Display an entire waste pile" << endl;
    }

    cin >> chooseOption;
    while (!cin || (chooseOption > 3 || chooseOption < 1) && chooseOption != -1)
    {
        invalidInput();
        cout << "choose a number between 1-3" << endl;
        cin >> chooseOption;
    }

    while (!cin || (hasWasteCards == false && chooseOption >= 2 || hasWasteCards == false && chooseOption < 1) && chooseOption != -1)
    {
        invalidInput();
        cout << "Invalid choice" << endl;
        cin >> chooseOption;
    }

    switch (chooseOption)
    {
    case 1:
        cout << "Do you want to play on a: " << endl;
        cout << "F. Foundation Pile" << endl;
        cout << "W. Waste Pile" << endl;
        cin >> choosePile;
        choosePile = (char)tolower(choosePile);
        while (!cin || choosePile != 'f' && choosePile != 'w')
        {
            invalidInput();
            cout << "Please choose W or F";
            cin >> choosePile;
        }
        if (choosePile == 'f')
        {
            PoF = playOnFoundation(foundationPiles, deck, drawnCard);
            if (PoF == 888)
            {
                removeElementFromDeck(deck, drawnCard);
            }
        }
        if (choosePile == 'w')
        {
            playOnWaste(wastePiles, deck, drawnCard);
            removeElementFromDeck(deck, drawnCard);
            PoF = 888;
        }
        return PoF;
    case 2:
    {
        // get the waste pile index from which to pull
        int wastePileIndex = playFromWaste();

        playOnFoundationFromWaste(foundationPiles, wastePiles[wastePileIndex]);
        return drawnCard;
    }
    case 3:
        showWastePile(wastePiles);
        return drawnCard;
    case -1:
        return -1;
    default:
        return 888;
    }
}

int showMenuForFinalOptions(int drawnCard, wastePileStruct wastePiles[WASTE_COUNT], foundationPileStruct foundationPiles[FOUNDATION_COUNT])
{
    int chooseOption;
    char choosePile;
    cout << "What would you like to do: (Enter -1 to quit)" << endl;
    cout << "1. Finish Game and Calculate Score" << endl;
    cout << "2. Play Card from Waste Pile" << endl;
    cout << "3. Display an entire waste pile" << endl;

    cin >> chooseOption;
    while (!cin || (chooseOption > 3 || chooseOption < 1) && chooseOption != -1)
    {
        invalidInput();
        cout << "choose a number between 1-3" << endl;
        cin >> chooseOption;
    }

    switch (chooseOption)
    {
    case 1:
        calculateScore(wastePiles);
        return -1;
    case 2:
    {
        // get the waste pile index from which to pull
        int wastePileIndex = playFromWaste();

        playOnFoundationFromWaste(foundationPiles, wastePiles[wastePileIndex]);
        return 888;
    }
    case 3:
        showWastePile(wastePiles);
        return drawnCard;
    case -1:
        return -1;
    default:
        return 888;
    }
}

void showWastePile(wastePileStruct wastePiles[WASTE_COUNT])
{
    int chosenWastePile;
    cout << "Which waste pile (1-4) do you want to see?" << endl;
    cin >> chosenWastePile;
    while (!cin || chosenWastePile > 4 || chosenWastePile < 1)
    {
        invalidInput();
        cout << "Please choose a number between 1-4" << endl;
        cin >> chosenWastePile;
    }

    chosenWastePile--;
    int index = wastePiles[chosenWastePile].count;
    while (index >= 0)
    {
        cout << toStringFaces(wastePiles[chosenWastePile].pile[index].face) << toStringSuits(wastePiles[chosenWastePile].pile[index].suit) << endl;
        index--;
    }
}

int playOnFoundation(foundationPileStruct foundationPiles[FOUNDATION_COUNT], deck &deck, int drawnCard)
{
    int end;
    bool isValid;
    int pileChoice;
    cout << "Which foundation pile (1-4) do you want to play on?" << endl;
    cin >> pileChoice;
    while (!cin || pileChoice > 4 || pileChoice < 1)
    {
        invalidInput();
        cout << "choose a number between 1-4" << endl;
        cin >> pileChoice;
    }
    pileChoice = pileChoice - 1;
    // validation for if it can go on pile
    cardType card = deck.myDeck[drawnCard];
    isValid = pileValidation(pileChoice, foundationPiles, card);
    // valid = pileValidation(pileChoice, drawnCard, foundationPiles, deck);

    if (isValid == true)
    {
        foundationPiles[pileChoice].foundationPile[foundationPiles[pileChoice].count] = deck.myDeck[drawnCard];
        foundationPiles[pileChoice].count++;
        end = 888;
    }
    else if (isValid == false)
    {
        cout << toStringFaces(deck.myDeck[drawnCard].face) << toStringSuits(deck.myDeck[drawnCard].suit) << " Cant be played on that pile :(" << endl;
        end = drawnCard;
    }
    return end;
}

void playOnFoundationFromWaste(foundationPileStruct foundationPiles[FOUNDATION_COUNT], wastePileStruct &wastePile)
{
    bool valid;
    int pileChoice;
    cout << "Which foundation pile (1-4) do you want to play on?" << endl;
    cin >> pileChoice;
    while (!cin || pileChoice > 4 || pileChoice < 1)
    {
        invalidInput();
        cout << "choose a number between 1-4" << endl;
        cin >> pileChoice;
    }
    pileChoice = pileChoice - 1;
    // validation for if it can go on pile
    cardType card = wastePile.pile[wastePile.count - 1];
    valid = pileValidation(pileChoice, foundationPiles, card);
    // valid = pileValidation(pileChoice, drawnCard, foundationPiles, wastePiles);

    if (valid == true)
    {
        foundationPiles[pileChoice].foundationPile[foundationPiles[pileChoice].count] = wastePile.pile[wastePile.count - 1];
        foundationPiles[pileChoice].count++;
        // remove from waste and decrement count
        removeTopCardFromWaste(wastePile);
    }
    else if (valid == false)
    {
        cout << toStringFaces(wastePile.pile[wastePile.count - 1].face) << toStringSuits(wastePile.pile[wastePile.count - 1].suit) << " Cant be played on that pile :(" << endl;
        // i can't play it so it has to go back on the waste pile
        wastePile.pile[wastePile.count] = card;
    }
}

void playOnWaste(wastePileStruct wastePiles[WASTE_COUNT], deck &deck, int drawnCard)
{
    int pileChoice;
    cout << "Which waste pile (1-4) do you want to play on?" << endl;
    cin >> pileChoice;
    while (!cin || pileChoice > 4 || pileChoice < 1)
    {
        invalidInput();
        cout << "choose a number between 1-4" << endl;
        cin >> pileChoice;
    }
    pileChoice = pileChoice - 1;
    wastePiles[pileChoice].pile[wastePiles[pileChoice].count] = deck.myDeck[drawnCard];
    wastePiles[pileChoice].count++;
}

int playFromWaste()
{
    int pileChoice;
    cout << "Which waste pile (1-4) do you want to play from?" << endl;
    cin >> pileChoice;
    while (!cin || pileChoice > 4 || pileChoice < 1)
    {
        invalidInput();
        cout << "choose a number between 1-4" << endl;
        cin >> pileChoice;
    }
    pileChoice = pileChoice - 1;
    return pileChoice;
}

void calculateScore(wastePileStruct wastePiles[WASTE_COUNT])
{
    int score = 0;
    // for (int j = 0; j < WASTE_COUNT; j++)
    // {
    //     for (int i = 0; i < 52; i++)
    //     {
    //         switch (wastePiles[j].pile[i].suit)
    //         {
    //         case spades:
    //             score++;
    //             continue;
    //         case hearts:
    //             score++;
    //             continue;
    //         case diamonds:
    //             score++;
    //             continue;
    //         case clubs:
    //             score++;
    //             continue;
    //         case suitNull:
    //             continue;
    //         default:
    //             continue;
    //         }
    //     }
    // }
    for (int i = 0; i < WASTE_COUNT; i++)
    {
        score += wastePiles[i].count;
    }
    cout << "Game Over! You scored " << score << " points." << endl;
}

bool playGameAgain()
{
    bool decision;
    char choice;
    cout << "Would you like to play again?" << endl;
    cin >> choice;
    choice = (char)tolower(choice);
    while (!cin || choice != 'n' && choice != 'y')
    {
        invalidInput();
        cout << "I don't understand." << endl;
        cout << "I was expecting Y or N." << endl;
        cout << "Would you like to play again?" << endl;
        cin >> choice;
    }
    if (choice == 'y')
    {
        decision = true;
    }
    else if (choice == 'n')
    {
        decision = false;
    }
    return decision;
}

#pragma endregion

#pragma region backend_stuff

bool pileValidation(int pile, foundationPileStruct foundationPiles[FOUNDATION_COUNT], cardType cardType)
{
    bool check;

    if (foundationPiles[pile].foundationPile[foundationPiles[pile].count].face == (cardType.face))
    {
        check = true;
    }

    else
    {
        check = false;
    }

    return check;
}

bool checkWastePile(wastePileStruct wastePiles[WASTE_COUNT])
{
    bool check;
    for (int j = 0; j < WASTE_COUNT; j++)
    {
        for (int i = 0; i < 52; i++)
        {
            switch (wastePiles[j].pile[i].suit)
            {
            case spades:
                check = true;
                break;
            case hearts:
                check = true;
                break;
            case diamonds:
                check = true;
                break;
            case clubs:
                check = true;
                break;
            case suitNull:
                continue;
            default:
                check = false;
                break;
            }
        }
    }
    return check;
}

void removeElementFromDeck(deck &deck, int element)
{
    deck.myDeck[element].face = faceNull;
    deck.myDeck[element].suit = suitNull;
}

void removeTopCardFromWaste(wastePileStruct &wastePile)
{
    if (wastePile.count == 0)
    {
        wastePile.pile[0].face = faceNull;
        wastePile.pile[0].suit = suitNull;
    }
    else
    {
        wastePile.pile[wastePile.count - 1].face = faceNull;
        wastePile.pile[wastePile.count - 1].suit = suitNull;
    }

    wastePile.count--;
}

void invalidInput()
{
    cin.clear();
    cin.ignore(INT_MAX, '\n');
}
#pragma endregion

#pragma region creates_the_deck_and_piles
void inializeFoundationPile(foundationPileStruct foundationPiles[FOUNDATION_COUNT])
{
    int count = 1;
    for (int i = 0; i < FOUNDATION_COUNT; i++)
    {

        for (int j = 1; j <= FOUNDATION_LENGTH; j++)
        {
            int a = ((j)*count) % 13;
            switch (a)
            {
            case 1:
                foundationPiles[i].foundationPile[j - 1].face = ace;
                foundationPiles[i].foundationPile[j - 1].suit = suitNull;
                continue;
            case 2:
                foundationPiles[i].foundationPile[j - 1].face = two;
                foundationPiles[i].foundationPile[j - 1].suit = suitNull;
                continue;
            case 3:
                foundationPiles[i].foundationPile[j - 1].face = three;
                foundationPiles[i].foundationPile[j - 1].suit = suitNull;
                continue;
            case 4:
                foundationPiles[i].foundationPile[j - 1].face = four;
                foundationPiles[i].foundationPile[j - 1].suit = suitNull;
                continue;
            case 5:
                foundationPiles[i].foundationPile[j - 1].face = five;
                foundationPiles[i].foundationPile[j - 1].suit = suitNull;
                continue;
            case 6:
                foundationPiles[i].foundationPile[j - 1].face = six;
                foundationPiles[i].foundationPile[j - 1].suit = suitNull;
                continue;
            case 7:
                foundationPiles[i].foundationPile[j - 1].face = seven;
                foundationPiles[i].foundationPile[j - 1].suit = suitNull;
                continue;
            case 8:
                foundationPiles[i].foundationPile[j - 1].face = eight;
                foundationPiles[i].foundationPile[j - 1].suit = suitNull;
                continue;
            case 9:
                foundationPiles[i].foundationPile[j - 1].face = nine;
                foundationPiles[i].foundationPile[j - 1].suit = suitNull;
                continue;
            case 10:
                foundationPiles[i].foundationPile[j - 1].face = ten;
                foundationPiles[i].foundationPile[j - 1].suit = suitNull;
                continue;
            case 11:
                foundationPiles[i].foundationPile[j - 1].face = jack;
                foundationPiles[i].foundationPile[j - 1].suit = suitNull;
                continue;
            case 12:
                foundationPiles[i].foundationPile[j - 1].face = queen;
                foundationPiles[i].foundationPile[j - 1].suit = suitNull;
                continue;
            case 0:
                foundationPiles[i].foundationPile[j - 1].face = king;
                foundationPiles[i].foundationPile[j - 1].suit = suitNull;
                continue;
            default:
                break;
            }
        }
        count++;
    }
}

void inializeWastePile(wastePileStruct wastePiles[WASTE_COUNT])
{
    for (int i = 0; i < WASTE_COUNT; i++)
    {
        for (int j = 0; j < 52; j++)
        {
            wastePiles[i].pile[j].face = faceNull;
            wastePiles[i].pile[j].suit = suitNull;
        }
    }
}

void intializeCards(deck &decks)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            decks.myDeck[j + i * 13].suit = static_cast<suitType>(i);
            decks.myDeck[j + i * 13].face = static_cast<faceType>(j);
        }
    }
}
#pragma endregion

#pragma region Converts_the_enums_to_strings
string toStringFaces(faceType face)
{
    switch (face)
    {
    case ace:
        return "A";
    case two:
        return "2";
    case three:
        return "3";
    case four:
        return "4";
    case five:
        return "5";
    case six:
        return "6";
    case seven:
        return "7";
    case eight:
        return "8";
    case nine:
        return "9";
    case ten:
        return "10";
    case jack:
        return "J";
    case queen:
        return "Q";
    case king:
        return "K";
    case faceNull:
        return " ";
    default:
        return "Uknown";
    }
}

string toStringSuits(suitType suit)
{
    switch (suit)
    {
    case diamonds:
        return "♦";
    case hearts:
        return "♥";
    case spades:
        return "♠";
    case clubs:
        return "♣";
    case suitNull:
        return " ";
    default:
        return "Uknown";
    }
}
#pragma endregion
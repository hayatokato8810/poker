//
//  main.cpp
//  poker
//
//  Created by Vincent Lau and Hayato Kato on 11/12/17.
//  Copyright © 2017 vincenthayato. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

void enterPlayers();
void initializeChips();
void printArray();
void playGame();
int determineAction(int bet, string playerName, int playerNumber);
void endRound();
    
const int MAXPLAYERS = 15;
int nGames = 0;
int nPlayers = 0;
int pot = 0;

string names[MAXPLAYERS];
int stats[4][MAXPLAYERS];
// row 0: active status (0-inactive, 1-active)
// row 1: # chips amount before round
// row 2: # chips betted during round
// row 3: # wins


int main() {
    enterPlayers();
    initializeChips();
    for(;;) {
        cout << endl << "START GAME #" << nGames+1 << endl;
        playGame();
        cout << "Would you like to play another game? (y/n) ";
        string s = "";
        getline(cin, s);
        if( (s == "y") || (s == "Y") ) {
            nGames++;
        }
        else if( (s == "n") || (s == "N") ) {
            cout << "Thank you for using the Poker Chip Tracker." << endl;
            return -1;
        }
        else {
            cout <<"Not a valid input! Must be \"y\" or \"n\"!" << endl;
        }
        
    }
}

bool ifTurnIsOver() {
    for(int i=0 ; i<nPlayers ; i++) {
        
    }
    return true;
}

void printArray() {
    for(int j=0 ; j<nPlayers ; j++) {
        for(int i=0 ; i<4; i++) {
            cout << stats[i][j] << "\t\t";
        }
        cout << names[j] << endl;
    }
}

void enterPlayers() {
    for(;;) {
        cout << "Enter the number of players (max " << MAXPLAYERS << "): ";
        cin >> nPlayers;
        cin.ignore(10000,'\n');
        if(nPlayers <= MAXPLAYERS) {
            break;
        }
        else {
            cout << "Not a valid input! Must enter a number <= " << MAXPLAYERS << "!" << endl;
        }
    }
    cout << endl << "Starting a " << nPlayers << " player game" << endl;
    for(int i=0 ; i<nPlayers ; i++) {
        cout << "Name of player " << i+1 << ": ";
        string s = "";
        getline(cin, s);
        names[i] = s;       // sets each element of array to player name
        stats[0][i] = 1;    // sets each element of array to active status 1
    }
    for(int i=nPlayers ; i<MAXPLAYERS ; i++) {
        stats[0][i] = 0;    // sets empty player elements of array to active status 0
    }
    cout << endl;
}

void initializeChips() {
    for(;;) {
        cout << "Does each player start with same number of chips (y/n)? ";
        string s = "";
        getline(cin, s);
        int val = 0;
        if( (s == "y") || (s == "Y") ) {
            cout << "How many chips does each player start with? ";
            cin >> val;         // WARNING: ONLY TAKES INTEGERS (run-time error)
            cin.ignore(10000, '\n');
            for(int i=0 ; i<nPlayers ; i++) {
                stats[1][i] = val;
            }
            break;
        }
        else if( (s == "n") || (s == "N") ) {
            for(int i=0 ; i<nPlayers ; i++) {
                cout << "How many chips does " << names[i] << " start with? ";
                cin >> val;     // WARNING: ONLY TAKES INTEGERS (run-time error)
                cin.ignore(10000, '\n');
                stats[1][i] = val;
            }
            break;
        }
        else {
            cout <<"Not a valid input! Must be \"y\" or \"n\"!" << endl;
        }
    }
    cout << endl;
    printArray();
}

void playGame() {
    // RESETS ROUND
    int bet = -1;
    for(int k=0 ; k<nPlayers ; k++) {
        stats[0][k] = 1;
    }
    // ROUND BEGINS
    for(int i=0 ; i<3 ; i++) {
        bet = -1;
        int subBettingRound = 0;
        cout << endl << "START BETTING ROUND " << i+1 << endl;
        int playerIndex = nGames;
        for(;;) {
            int playerNumber = playerIndex % nPlayers;
            string playerName = names[playerNumber];
            if( (stats[2][playerNumber] == bet) && (stats[0][playerNumber] == 1) ){
                break;
            }
            else if( (subBettingRound == nPlayers) && (bet == -1) ) {
                break;
            }
            else if( (stats[2][playerNumber] != bet) && (stats[0][playerNumber] == 1) ){
                bet = determineAction(bet, playerName, playerNumber);
                subBettingRound++;
            }
            playerIndex++;
        }
        for(int k=0 ; k<nPlayers ; k++) {
            pot += stats[2][k];
            stats[1][k] -= stats[2][k];
            stats[2][k] = 0;
        }
        cout << "The pot value is " << pot << endl;
    }
    printArray();
    
    endRound();
    
    printArray();
}

int determineAction(int bet, string playerName, int playerNumber) {
    for(;;) {
        cout << "(" << bet << ") " << "What would you like to do, " << playerName << "?" << endl;
        cout << "(\"x\" to check, \"b\" to bet, \"c\" to call, \"r\" to raise, \"f\" to fold) ";
        string s = "";
        getline(cin, s);
        if(s == "x") {
            if(bet == -1){
                break;
            }
            else{
                cout << "Someone has already bet " << bet << "!" << endl;
            }
        }
        // BET
        else if(s == "b") {
            if(bet == -1) {
                for(;;) {
                    cout << "What is your bet, " << playerName << "? ";
                    int val = 0;
                    cin >> val;
                    cin.ignore(10000, '\n');
                    if( (bet < val) && (val <= (stats[1][playerNumber] - stats[2][playerNumber])) ) {
                        bet = val;
                        stats[2][playerNumber] = val;
                        break;
                    }
                    else if (! (val <= (stats[1][playerNumber] - stats[2][playerNumber]))) {
                        cout << "You do not have that many chips! You must check or fold." << endl;
                    }
                    else {
                        cout << "You must bet more than " << bet << "!" << endl;
                    }
                }
                break;
            }
            else {
                cout << "Someone has already bet " << bet << "! You must call, raise, or fold." <<endl;
            }
        }
        // CALL
        else if(s == "c") {
            if(bet == -1) {
                bet = 0;
            }
            else if(bet < (stats[1][playerNumber] - stats[2][playerNumber]) ) {
                    stats[2][playerNumber] = bet;
                    break;
            }
            else {
                cout << "You do not have that many chips! You must fold." << endl;
            }
        }
        // RAISE
        else if(s == "r") {
            if(bet != -1) {
                if(bet < (stats[1][playerNumber] - stats[2][playerNumber])) {
                    for(;;) {
                        cout << "How much would you like to raise the bet to, " << playerName << "? ";
                        int val = 0;
                        cin >> val;
                        cin.ignore(10000, '\n');
                        if( (bet < val) && (val <= stats[1][playerNumber]-stats[2][playerNumber]) ) {
                            bet = val;
                            stats[2][playerNumber] = val;
                            break;
                        }
                        else if (val >= stats[1][playerNumber]-stats[2][playerNumber]) {
                            cout << "You do not have that many chips! You must raise less, call, or fold." << endl;
                        }
                        else {
                            cout << "You must bet more than " << bet << "!" << endl;
                        }
                    }
                    break;
                }
                else {
                    cout << "You cannot raise beyond " << bet << "! You must call or fold." << endl;
                }
            }
            else {
                cout << "Nobody has made a bet yet! You must check, bet, or fold." <<endl;
            }
        }
        // FOLD
        else if(s == "f") {
            stats[0][playerNumber] = 0;
            break;
        }
        else {
            cout << "Not a valid input! Must enter \"x\", \"b\", \"c\", \"r\", or \"f\"" << endl;
        }
    }
    cout << endl;
    printArray();
    cout << endl;
    return bet;
}

void endRound() {
    int nWinners;
    string winners[nPlayers];
    for(;;) {
        nWinners = 0;
        cout << endl << "How many winners in this round? ";
        cin >> nWinners;
        cin.ignore(10000, '\n');
        if( (nWinners <= nPlayers) && (nWinners > 0) ) {        // checks if num of winners is valid
            for(int w=0 ; w<nWinners ; w++) {
                cout << "What is the name of Winner " << w+1 << "? ";
                string s = "";
                getline(cin, s);
                winners[w] = s;
            }
            int validWinners = 0;
            for(int j = 0; j < nPlayers; j++){
                for(int k=0 ; k< nWinners ; k++) {
                    if(winners[k] == names[j]) {
                        validWinners++;
                    }
                }
            }
            if(validWinners != nWinners) {                      // checks if names of winners are valid
                cout << "You entered one or more invalid names for the winners! Try again." << endl;
            }
            else {
                break;
            }
        }
        else {
            cout << "You must enter a positive integer between 1 and " << nPlayers << ", inclusive!" << endl;
        }
    }
    for(int w=0 ; w<nWinners ; w++) {
        for(int v=0 ; v<nPlayers ; v++) {
            if(winners[w] == names[v]) {
                stats[1][v] += pot/nWinners;
                stats[3][v] ++;
            }
        }
    }
}

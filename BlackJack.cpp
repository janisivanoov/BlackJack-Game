#include <iostream>
#include <array>
#include <cstdlib> // for rand() and srand() functions
#include <ctime> // for time() function
using namespace std; 

// Card suit
enum CardSuit
{
	SUIT_CLUBS,
	SUIT_DIAMONDS,
	SUIT_HEARTS,
	SUIT_SPADES,
	MAX_SUITS
};

// Card rank
enum CardRank
{
	RANK_2,
	RANK_3,
	RANK_4,
	RANK_5,
	RANK_6,
	RANK_7,
	RANK_8,
	RANK_9,
	RANK_10,
	RANK_JACK,
	RANK_QUEEN,
	RANK_KING,
	RANK_ACE,
	MAX_RANKS
};


// Card "structure"
struct Card {
	CardRank rank; // Card rank
	CardSuit suit; // Card suit
};

//Printing Card(Rank & Suit)
void printCard(const Card& card) {
	//Printing card rank
	switch (card.rank) {
	case RANK_2:
		cout << "2";
		break;
	case RANK_3:
		cout << "3";
		break;
	case RANK_4:
		cout << "4";
		break;
	case RANK_5:
		cout << "5";
		break;
	case RANK_6:
		cout << "6";
		break;
	case RANK_7:
		cout << "7";
		break;
	case RANK_8:
		cout << "8";
		break;
	case RANK_9:
		cout << "9";
		break;
	case RANK_10:
		cout << "10";
		break;
	case RANK_JACK:
		cout << "J";
		break;
	case RANK_QUEEN:
		cout << "Q";
		break;
	case RANK_KING:
		cout << "K";
		break;
	case RANK_ACE:
		cout << "A";
		break;
	}

	// Printing card suit
	switch (card.suit) {
	case SUIT_DIAMONDS:
		cout << "D";
		break;
	case SUIT_HEARTS:
		cout << "H";
		break;
	case SUIT_SPADES:
		cout << "S";
		break;
	case SUIT_CLUBS:
		cout << "C";
		break;
	}
}

// Printing whole deck
void printDeck(const std::array<Card, 52>& deck) {
	for (const auto& card : deck) {
		printCard(card);
		cout << " ";
	}
	cout << endl;
}

// Change 2 card places 
void swapCard(Card& card1, Card& card2) {
	Card temp = card1;
	card1 = card2;
	card2 = temp;
}

// Generating a random number between values min & max
int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	// Distribute the random number evenly in our range
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

// Shuffling the deck
void shuffleDeck(std::array<Card, 52>& deck) {
	for (int card = 0; card < 52; ++card)
		swapCard(deck[card], deck[getRandomNumber(0, 51)]);
}

// We get the value of the card (2-10 - dignity, jack-king - 10, ace - 11)
int getCardValue(Card& card) {
	if (card.rank <= RANK_10)
		return card.rank + 2;
	else if (card.rank <= RANK_KING)
		return 10;
	return 11;
}

// We ask the player whether he takes the card or not. Here we cut off the incorrect input
int getAnswer() {
	int a;
	do // The cycle continues until the user enters the correct value.
	{
		cout << "\nYou can take the card (1), or hold (0): ";
		cin >> a;

		// Checking for a previous checkout
		if (cin.fail()) {// If the previous checkout was unsuccessful,
			cin.clear(); // then we return cin to the 'normal' mode of operation
			cin.ignore(32767, '\n'); // and remove the values ​​of the previous input from the input buffer
			a = 5;
		}
		else {
			cin.ignore(32767, '\n'); // remove unnecessary values
		}
	} while ((a < 0) || (a > 1));
	return a;
}

// Return true if won and false if not
bool playBlackjack(array<Card, 52>& deck) {

	// Link to the current card in the deck
	Card* cardPtr = &deck[0];

	// Player's and Dealer's points totals
	int sumPlayer = 0, sumDealer = 0;

	// Initial distribution: the dealer receives 1 card, the player - 2 cards
	sumDealer += getCardValue(*cardPtr++);
	cout << "The dealer received 1 card...\n";
	sumPlayer += getCardValue(*cardPtr++);
	sumPlayer += getCardValue(*cardPtr++);
	cout << "The player received 2 cards...\n";

	cout << "\nDealer's card amount - " << sumDealer << endl;
	cout << "Your card amount - " << sumPlayer << endl;

	int answer = 1; // We assign 1 to ask the player for the first time anyway

	while (true) {

		if (answer == 1) // If the player took a card for the first time or in the last round, then we ask
			answer = getAnswer();

		bool move = false; // Was there a distribution in this circle?

		// If the player answered 1, then we deal him a card
		if (answer == 1) {
			sumPlayer += getCardValue(*cardPtr++);
			cout << "\nThe player received 1 card...\n";
			move = true; // Раздача была
		}

		// If the dealer's sum is less than 17 and the player hasn't gone over yet, then we hand over the card to the dealer
		if ((sumDealer < 17) && (sumPlayer <= 21)) {
			sumDealer += getCardValue(*cardPtr++);
			if (!move) // If the player did not take a card, then an indent is needed
				cout << endl;
			cout << "The dealer received 1 card...\n";
			move = true; // The distribution was
		}

		// If something is handed out, we issue information
		if (move) {
			cout << "\nDealer's card amount - " << sumDealer << endl;
			cout << "Your card amount - " << sumPlayer << endl;
		}

		// If the player has bust, then the loss
		if (sumPlayer > 21)
			return false;

		// If the dealer has bust, then the winning
		if (sumDealer > 21)
			return true;

		// If there was no distribution in this circle, the game is over and we return the result.
		if (!move)
			return (sumDealer <= sumPlayer);
	}
}

int main() {
	// Forming an ordered deck
	array<Card, 52> deck;
	int card = 0;
	for (int suit = 0; suit < MAX_SUITS; ++suit)
		for (int rank = 0; rank < MAX_RANKS; ++rank) {
			deck[card] = { static_cast<CardRank>(rank) , static_cast<CardSuit>(suit) };
			++card;
		}

	// Shuffle the deck
	srand(static_cast<unsigned int>(time(0)));
	rand();
	shuffleDeck(deck);

	// We play and give the result
	if (playBlackjack(deck))
		cout << "\nCongatulations! You won!\n";
	else
		cout << "\nThats sad but you lose(\n";

	return 0;
}

#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdlib.h> 

struct Card
{
	char st_Card_Suit[10];
	char st_Card_Name[10];
	int st_Card_Value;
};

struct Player
{
	double st_Wallet;
	int st_Card_Index;
	Card st_Hand[11];
	int st_Hand_Total = 0;
};

void Welcome();
void SetWallet(double &_plyr_wallet);
void PlaceBet(Player &_player, double &_plr_bet);
void Play(Card* _deck, Player &_player, Player &_dealer);
void DealerPlay(Card* _deck, Player &_player, Player &_dealer, int _deck_top, bool &_hit_or_stay, bool &_dlr_bj, bool &_plr_bj, double _plr_bet);
void HitorStay(bool &_hit_or_stay);
void CheckForAce(Card _deck[], Player &_player);
void SetAceValue(int _card, Player &_player);
void DealCard(Card _deck[], int &_deck_top, Player &_player);
void iDeck(Card _deck[]);
void GetHand(Card _deck[], Player _player, Player _dealer, double _plr_bet, bool _hit_or_stay);
void CheckBlackjack(Player &_player, Player &_dealer, double &_plr_bet, bool &_play, bool &_plr_bj, bool &_dlr_bj);
void Shuffle(Card _deck[]);
Card DealTop(Card* _deck, int& _todeal);
void UpdateHandTotal(Player *_player);
void WinLose(Card* _deck, Player &_player, Player &_dealer, double &_plr_bet, bool _plr_bj, bool _dlr_bj, bool &_hit_or_stay);
void GameSet(Card* _deck, Player &_player, Player &_dealer, int &_deck_top, bool &_hit_or_stay, bool &_dlr_bj, bool &_plr_bj, double &_plr_bet);
void BlankLine(int _lines);
void InvalidInput();
void Exitprgrm();

int main()
{
	bool main_loop = true;

	// ----------- GAME LOOP -----------
	do
	{
		// SEED RANDOM NUMBER GENERATOR
		time_t qTime;
		time(&qTime);
		srand(qTime);
		srand(time(NULL));

		// SET DECK OF 52 TO 0
		Card Deck[52] = { 0 };

		// SET PLAYER AND DEALER VALUES TO 0
		Player player = { 0,0,0,0 };
		Player dealer = { 0,0,0,0 };

		// ----------- WELCOME PLAYER -----------
		Welcome();

		// ----------- SET WALLET SIZE -----------
		SetWallet(player.st_Wallet);

		// ----------- PLAY -----------

		Play(Deck, player, dealer);

	} while (main_loop == true);

	return 0;
}

void Welcome()
{
	char plyr_input = NULL;
	bool welcome = true;

	while (welcome == true)
	{
		system("CLS");

		std::cout << "--------------------------" << std::endl;
		std::cout << "| WELCOME TO BLACK JACK! |" << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::cout << "-----" << std::endl;
		std::cout << "| i |" << "  GET 21 OR AS CLOSE TO 21 WITHOUT GOING OVER 21." << std::endl;
		std::cout << "-----" << std::endl;
		BlankLine(1);
		std::cout << "PRESS [1] TO PLAY OR [0] TO EXIT" << std::endl;
		std::cout << ">> ";
		std::cin >> plyr_input;

		switch (plyr_input)
		{
			case '0':
			{
				welcome = false;
				Exitprgrm();
				break;
			}
			case '1':
			{
				welcome = false;
				break;
			}
			default:
			{
				InvalidInput();
				break;
			}
		}
	}
}

void SetWallet(double &_plyr_Wallet)
{
	bool setWallet = true;

	system("CLS");

	std::cout << "--------------------------------" << std::endl;
	std::cout << "| DEPOSIT FUNDS TO YOUR WALLET |" << std::endl;
	std::cout << "--------------------------------" << std::endl;
	std::cout << "YOUR WALLET: $" << _plyr_Wallet << std::endl;
	BlankLine(1);
	std::cout << "HOW MUCH WOULD YOU LIKE TO DEPOSIT? ($1 - $100)" << std::endl;

	while (setWallet == true)
	{
		double temp_Wallet = 0;

		std::cin.clear();
		std::cin.ignore();

		std::cout << ">> $";
		std::cin >> temp_Wallet;
		
		if (temp_Wallet == 0 || temp_Wallet < 1 || temp_Wallet > 100)
		{
			InvalidInput();
		}
		else
		{
			_plyr_Wallet = temp_Wallet;
			setWallet = false;

			BlankLine(1);
			std::cout << ">>> NEW WALLET BALANCE: $" << _plyr_Wallet << std::endl;
			Sleep(1500);
		}
	}
}

void PlaceBet(Player &_player, double &_plr_bet)
{
	bool place_bet = true;

	std::cout << "---------------" << std::endl;
	std::cout << "| PLACE A BET |" << std::endl;
	std::cout << "---------------" << std::endl;
	std::cout << "WALLET: $" << _player.st_Wallet << std::endl;
	BlankLine(1);
	std::cout << "HOW MUCH WOULD YOU LIKE TO BET? ($1 - $" << _player.st_Wallet << ")" << std::endl;

	while (place_bet == true)
		{
		double tmp_plr_bet = 0;

		std::cin.clear();
		std::cin.ignore();

		std::cout << ">> $";
		std::cin >> tmp_plr_bet;

		if (tmp_plr_bet == 0 || tmp_plr_bet < 1 || tmp_plr_bet > _player.st_Wallet)
		{
			InvalidInput();
		}
		else
		{
			_player.st_Wallet -= tmp_plr_bet;
			_plr_bet = tmp_plr_bet;
			place_bet = false;

			BlankLine(1);
			std::cout << ">>> $" << tmp_plr_bet << " BET PLACED" << std::endl;
			BlankLine(1);
			std::cout << ">>> NEW WALLET BALANCE: $" << _player.st_Wallet << std::endl;
			Sleep(2000);
		}
	}
}

void HitorStay(bool &_hit_or_stay)
{
	bool valid = false;

	BlankLine(1);
	std::cout << "---------------" << std::endl;
	std::cout << "| HIT OR STAY |" << std::endl;
	std::cout << "---------------" << std::endl;
	std::cout << "Press [H] to Hit or [S] to Stay" << std::endl;

	while (valid == false)
	{
		char decision = NULL;
		std::cout << ">> ";

		std::cin.clear();
		std::cin.ignore();

		std::cin >> decision;

		if (decision == 'H' || decision == 'h' || decision == 'S' || decision == 's')
		{
			valid = true;

			if (decision == 'H' || decision == 'h')
			{
				_hit_or_stay = true;
			}
			else if (decision == 'S' || decision == 's')
			{
				_hit_or_stay = false;
			}
		}
		else
		{
			valid = false;

			BlankLine(1);
			std::cout << "INVALID INPUT" << std::endl;
			BlankLine(1);
		}
	}
}

void CheckForAce(Card _deck[], Player &_player)
{
	int countFrom = 0;

	// DETERMINE INDEX
	if (_player.st_Card_Index == 2)
	{
		countFrom = _player.st_Card_Index - 2;
	}
	if (_player.st_Card_Index > 2)
	{
		countFrom = _player.st_Card_Index - 1;
	}

	// COUNT THE NECESSARY CARDS
	for (int i = countFrom; i != _player.st_Card_Index; i++)
	{
		char *temp = _player.st_Hand[i].st_Card_Name;

		if (*temp == 'A')
		{
			SetAceValue(i, _player);
		}
	}
}

void SetAceValue(int _card, Player &_player)
{
	// ----------- USER DETERMINES ACE VALUE -----------

	char plyr_input = NULL;
	bool set_Ace = true;

	BlankLine(1);
	std::cout << "--------------" << std::endl;
	std::cout << "| ACE PLAYED |" << std::endl;
	std::cout << "--------------" << std::endl;
	std::cout << "[H]igh or [L]ow?" << std::endl;

	do
	{
		std::cout << ">> ";
		std::cin >> plyr_input;

		if (plyr_input == 'H' || plyr_input == 'h')
		{
			_player.st_Hand[_card].st_Card_Value = 11;
			UpdateHandTotal(&_player);

			BlankLine(1);
			std::cout << ">> SET ACE VALUE: 11" << std::endl;
			BlankLine(1);

			set_Ace = false;
		}
		else if (plyr_input == 'L' || plyr_input == 'l')
		{
			_player.st_Hand[_card].st_Card_Value = 1;
			UpdateHandTotal(&_player);

			BlankLine(1);
			std::cout << ">> SET ACE VALUE: 1" << std::endl;
			BlankLine(1);

			set_Ace = false;
		}
		else
		{
			InvalidInput();
		}
	} while (set_Ace == true);


	// ----------- SET ACE VALUE AUTOMATICALLY -----------

	/*
	if (_player.st_Hand_Total <= 10)
	{
		//std::cout << "HAND TOTAL IS LESS THAN OR EQUAL TO 11" << std::endl;
		_player.st_Hand[_card].st_Card_Value = 11;
		UpdateHandTotal(&_player); // UPDATE PLAYER HAND

		std::cout << ">> SET ACE VALUE: 11" << std::endl;
		BlankLine(1);
	}
	else if (_player.st_Hand_Total >= 11)
	{
		//std::cout << "HAND TOTAL IS MORE THAN 11" << std::endl;
		_player.st_Hand[_card].st_Card_Value = 1;
		UpdateHandTotal(&_player); // UPDATE PLAYER HAND

		std::cout << ">> SET ACE VALUE: 1" << std::endl;
		BlankLine(1);
	}
	*/
}

void Play(Card* _deck, Player &_player, Player &_dealer)
{
	int _deck_top = 0;
	bool _hit_or_stay = true;
	bool play = true;
	double plr_bet = NULL;
	bool plr_bj = false;
	bool dlr_bj = false;
	bool new_Round = true;

	GameSet(_deck, _player, _dealer, _deck_top, _hit_or_stay, dlr_bj, plr_bj, plr_bet);

	// ----------- GAME LOOP -----------

	do
	{
		// ----------- SET AND RESET DECK -----------
		if (_deck_top == 0 || _deck_top >= 40)
		{
			// ----------- INITIATE DECK -----------
			iDeck(_deck);

			// ----------- SHUFFLE DECK -----------
			Shuffle(_deck);

			_deck_top = 0;
		}

		// ----------- NEW ROUND -----------
		if (new_Round == true)
		{
			system("CLS");

			// ----------- SET VARIABLES -----------
			
			for (int i = 0; i < 11; i++) // RESET HAND TO 0
			{
				_player.st_Hand[i] = { 0 };
				_dealer.st_Hand[i] = { 0 };
			}

			_player.st_Card_Index = 0;
			_dealer.st_Card_Index = 0;

			_player.st_Hand_Total = 0;
			_dealer.st_Hand_Total = 0;

			_hit_or_stay = true;

			plr_bet = 0;

			plr_bj = false;

			dlr_bj = false;
			
			// ----------- PLACE A BET -----------
			PlaceBet(_player, plr_bet);

			new_Round = false;

			system("CLS");
		}

		// ----------- DEAL -----------

		if (_player.st_Card_Index == 0)
		{
			DealCard(_deck, _deck_top, _player);
			DealCard(_deck, _deck_top, _dealer);
			DealCard(_deck, _deck_top, _player);
			DealCard(_deck, _deck_top, _dealer);

			// ----------- CHECK DEALT CARDS FOR AN ACE -----------

			CheckForAce(_deck, _player); // PLAYER HAND

			BlankLine(1);
			std::cout << "--------" << std::endl;
			std::cout << "| DEAL |" << std::endl;
			std::cout << "--------" << std::endl;
			BlankLine(1);
		}
		else if (_player.st_Card_Index >= 2 && _hit_or_stay == true)
		{
			DealCard(_deck, _deck_top, _player);

			// ----------- CHECK DEALT CARDS FOR AN ACE -----------

			CheckForAce(_deck, _player); // PLAYER HAND

			BlankLine(1);
			std::cout << "------------" << std::endl;
			std::cout << "| NEW HAND |" << std::endl;
			std::cout << "------------" << std::endl;
			BlankLine(1);
		}

		// ----------- SHOW PLAYER AND DEALER HAND -----------

		GetHand(_deck, _player, _dealer, plr_bet, _hit_or_stay);

		// ----------- CHECK PLAYER BUST -----------

		if (_player.st_Hand_Total > 21)
		{
			BlankLine(1);
			std::cout << ">>> PLAYER: BUST" << std::endl;
			BlankLine(1);

			_hit_or_stay = false;
		}
		else if (_player.st_Hand_Total == 21 && _player.st_Card_Index <= 2)
		{
			BlankLine(1);
			std::cout << ">>> PLAYER: BLACKJACK <<<" << std::endl;
			BlankLine(1);

			plr_bj = true;
			_hit_or_stay = false;
		}
		else if (_player.st_Hand_Total <= 21)
		{
			// ----------- HIT OR STAY -----------

			HitorStay(_hit_or_stay);
			Sleep(1500);
		}

		// ----------- PLAYER STAY. INITIATE DEALERS TURN. -----------

		if (_hit_or_stay == false || _player.st_Hand_Total == 21)
		{
			if (plr_bj == false && _player.st_Hand_Total <= 21)
			{
				BlankLine(1);
				std::cout << ">>> PLAYER: STAY. DEALERS TURN." << std::endl;
				Sleep(1500);
			}

			// ----------- CHECK DEALER BUST -----------

			if (_dealer.st_Hand_Total > 21)
			{
				BlankLine(1);
				std::cout << ">>> DEALER: BUST" << std::endl;
				BlankLine(1);
				Sleep(1500);
			}
			else
			{
				// ----------- DEALERS TURN -----------

				DealerPlay(_deck, _player, _dealer, _deck_top, _hit_or_stay, dlr_bj, plr_bj, plr_bet);

				// ----------- CHECK DEALER BUST -----------

				if (_dealer.st_Hand_Total > 21)
				{
					BlankLine(1);
					std::cout << ">>> DEALER: BUST" << std::endl;
					BlankLine(1);
					Sleep(1500);
				}
			}			

			// ----------- CHECK BLACKJACK -----------

			CheckBlackjack(_player, _dealer, plr_bet, play, plr_bj, dlr_bj);

			// ----------- CHECK WIN/LOSE -----------

			if (plr_bj == false && dlr_bj == false)
			{
				WinLose(_deck, _player, _dealer, plr_bet, plr_bj, dlr_bj, _hit_or_stay);
			}

			// ----------- PLAY AGAIN OR RESTART TO MENU -----------

			char plyr_input = NULL;
			bool choice_loop = true;

			BlankLine(1);
			std::cout << "PRESS [1] TO PLAY ANOTHER ROUND OR [0] TO RESTART TO MENU." << std::endl;

			while (choice_loop == true)
			{
				std::cin.clear();
				std::cin.ignore();

				std::cout << ">> ";
				std::cin >> plyr_input;

				switch (plyr_input)
				{
				case '0':
				{
					choice_loop = false;
					play = false;
					break;
				}
				case '1':
				{
					choice_loop = false;
					play = true;
					new_Round = true;
					break;
				}
				default:
				{
					InvalidInput();
					break;
				}
				}
			}
		}
	} while (play == true);
}

void DealerPlay(Card* _deck, Player &_player, Player &_dealer, int _deck_top, bool &_hit_or_stay, bool &_dlr_bj, bool &_plr_bj, double _plr_bet)
{
	bool dealer_play = true;

	while (dealer_play == true)
	{
		if (_dealer.st_Hand_Total == 21 && _dealer.st_Card_Index == 2) // BLACKJACK
		{
			dealer_play = false;

			BlankLine(1);
			std::cout << ">>> DEALER: BLACKJACK" << std::endl;

			_dlr_bj = true;

			Sleep(1500);
		}
		else if (_dealer.st_Hand_Total >= 17) // STAY
		{
			dealer_play = false;

			BlankLine(1);
			std::cout << ">>> DEALER: STAY" << std::endl;
			BlankLine(1);

			Sleep(1500);
		}
		else if (_dealer.st_Hand_Total <= 16) // HIT
		{
			BlankLine(1);
			std::cout << ">>> DEALER: HIT" << std::endl;
			BlankLine(1);

			Sleep(1500);

			DealCard(_deck, _deck_top, _dealer);

			BlankLine(1);
			std::cout << "------------" << std::endl;
			std::cout << "| NEW HAND |" << std::endl;
			std::cout << "------------" << std::endl;
			BlankLine(1);

			GetHand(_deck, _player, _dealer, _plr_bet, _hit_or_stay);
		}
	}
}

void DealCard(Card _deck[], int &_deck_top, Player &_player)
{
	_player.st_Hand[_player.st_Card_Index++] = DealTop(_deck, _deck_top); // DEAL 1 TO PLAYER
	UpdateHandTotal(&_player); // UPDATE PLAYER HAND
}

void iDeck(Card _deck[]) // INITIATE DECK OF CARDS
{
	for (int suit = 0; suit < 52; suit += 13) // 4 SUITS
	{
		for (int card = 0; card < 13; card++) // 13 CARDS
		{
			if (card > 9) // JACK, QUEEN, KING
			{
				_deck[suit + card].st_Card_Value = 10;
			}
			else if (card == 0) // SET ACE
			{
				_deck[suit + card].st_Card_Value = 11;
			}
			else // NUMBERED CARD
			{
				_deck[suit + card].st_Card_Value = card + 1;
			}

			// SET CARD SUIT
			if (suit == 0) { strcpy_s(_deck[suit + card].st_Card_Suit, "Spades"); }
			else if (suit == 13) { strcpy_s(_deck[suit + card].st_Card_Suit, "Hearts"); }
			else if (suit == 26) { strcpy_s(_deck[suit + card].st_Card_Suit, "Diamonds"); }
			else if (suit == 39) { strcpy_s(_deck[suit + card].st_Card_Suit, "Clubs"); }

			// SET CARD NAME
			switch (card)
			{
			case 0: { strcpy_s(_deck[suit + card].st_Card_Name, "Ace"); break;  }
			case 1: { strcpy_s(_deck[suit + card].st_Card_Name, "2"); break;  }
			case 2: { strcpy_s(_deck[suit + card].st_Card_Name, "3"); break;  }
			case 3: { strcpy_s(_deck[suit + card].st_Card_Name, "4"); break;  }
			case 4: { strcpy_s(_deck[suit + card].st_Card_Name, "5"); break;  }
			case 5: { strcpy_s(_deck[suit + card].st_Card_Name, "6"); break;  }
			case 6: { strcpy_s(_deck[suit + card].st_Card_Name, "7"); break;  }
			case 7: { strcpy_s(_deck[suit + card].st_Card_Name, "8"); break;  }
			case 8: { strcpy_s(_deck[suit + card].st_Card_Name, "9"); break;  }
			case 9: { strcpy_s(_deck[suit + card].st_Card_Name, "10"); break;  }
			case 10: { strcpy_s(_deck[suit + card].st_Card_Name, "Jack"); break;  }
			case 11: { strcpy_s(_deck[suit + card].st_Card_Name, "Queen"); break;  }
			case 12: { strcpy_s(_deck[suit + card].st_Card_Name, "King"); break;  }
			default: break;
			}
		}
	}
}

void GetHand(Card _deck[], Player _player, Player _dealer, double _plr_bet, bool _hit_or_stay)
{
	// ----------- GET PLAYER HAND -----------

	std::cout << "----- PLAYER -----" << std::endl;
	BlankLine(1);

	for (int i = 0; i <= _player.st_Card_Index; i++)
	{
		Card plyrhand = _player.st_Hand[i];

		std::cout << plyrhand.st_Card_Name << " " << plyrhand.st_Card_Suit << "   ";
	}

	BlankLine(2);
	std::cout << "> HAND: " << _player.st_Hand_Total << std::endl;
	std::cout << "> BET: $" << _plr_bet << std::endl;
	BlankLine(1);

	// ----------- GET DEALER HAND -----------

	std::cout << "-----DEALER-----" << std::endl;
	BlankLine(1);

	for (int i = 0; i < 11; i++)
	{
		Card dlrhand = _dealer.st_Hand[i];

		if (i < 1 && _dealer.st_Card_Index <= 2 && _hit_or_stay == true)  // HIDE FIRST CARD
		{
			std::cout << "**" << " " << "**" << "   ";
		}
		else if (i < 11)
		{
			std::cout << dlrhand.st_Card_Name << " " << dlrhand.st_Card_Suit << "   ";
		}
	}

	if (_dealer.st_Card_Index >= 2 && (_hit_or_stay == false || _player.st_Hand_Total > 21))
	{
		BlankLine(1);
		std::cout << "> HAND: " << _dealer.st_Hand_Total << std::endl;
		BlankLine(2);
	}
}

void CheckBlackjack(Player &_player, Player &_dealer, double &_plr_bet, bool &_play, bool &_plr_bj, bool &_dlr_bj)
{
	if (_plr_bj == true || _dlr_bj == true) // CHECK FOR BLACKJACK
	{
		if (_plr_bj == true && _dlr_bj == false) // PLAYER WIN BJ
		{
			BlankLine(1);
			std::cout << ">>> PLAYER WINS BLACKJACK <<<" << std::endl;
			BlankLine(1);

			_player.st_Wallet += (_plr_bet * 2);

			std::cout << ">>> +$" << _plr_bet << " | PLAYER BET DOUBLED" << std::endl;
			BlankLine(1);

			_play = false;
		}
		else if (_plr_bj == false && _dlr_bj == true) // DEALER WIN BJ
		{
			BlankLine(1);
			std::cout << ">>> DEALER WINS BLACKJACK <<<" << std::endl;
			BlankLine(1);
			std::cout << ">>> -$" << _plr_bet << " | PLAYER BET LOST" << std::endl;
			BlankLine(1);

			_dealer.st_Wallet += _plr_bet;

			_play = false;
		}
		else if (_plr_bj == true && _dlr_bj == true) // PLAYER AND DEALER HAVE BJ
		{
			BlankLine(1);
			std::cout << ">>> BLACKJACK DRAW <<<" << std::endl;
			BlankLine(1);

			_player.st_Wallet += _plr_bet;

			std::cout << ">>> +$" << _plr_bet << " | PLAYER BET RETURNED" << std::endl;
			BlankLine(1);

			_play = false;
		}

		Sleep(1500);
		std::cout << ">>> NEW WALLET BALANCE: $" << _player.st_Wallet << std::endl;
		Sleep(1500);

	}
}

void Shuffle(Card _deck[])
{
	Card deck_temp = { 0 };

	for (int i = 0; i < 52; i++) // MAKE 52 RANDOM SWAPS
	{
		deck_temp = _deck[i];
		int rnd = rand() % 52;
		_deck[i] = _deck[rnd];
		_deck[rnd] = deck_temp;
	}
}

Card DealTop(Card* _deck, int& _deck_top) // DEAL FROM TOP OF THE DECK
{
	return _deck[_deck_top++];
}

void UpdateHandTotal(Player *_player)
{
	int total = 0;

	for (int i = 0; i < _player->st_Card_Index; i++)
	{
		total += _player->st_Hand[i].st_Card_Value;
	}

	_player->st_Hand_Total = total;
}

void WinLose(Card* _deck, Player &_player, Player &_dealer, double &_plr_bet, bool _plr_bj, bool _dlr_bj, bool &_hit_or_stay)
{
	// ----------- SHOW PLAYER AND DEALER HAND -----------
	BlankLine(1);
	std::cout << "----------" << std::endl;
	std::cout << "| RESULT |" << std::endl;
	std::cout << "----------" << std::endl;
	BlankLine(1);

	GetHand(_deck, _player, _dealer, _plr_bet, _hit_or_stay);

	if (((_dealer.st_Hand_Total > _player.st_Hand_Total && _dealer.st_Hand_Total <= 21) || (_player.st_Hand_Total > 21 && _dealer.st_Hand_Total <= 21)) ) // DEALER TOTAL IS MORE THAN PLAYER
	{
		BlankLine(1);
		std::cout << ">>> DEALER WINS <<<" << std::endl;
		BlankLine(1);
		std::cout << ">>> -$" << _plr_bet << " | PLAYER BET LOST" << std::endl;
		BlankLine(1);

		_dealer.st_Wallet += _plr_bet;
	}
	else if (_dealer.st_Hand_Total == _player.st_Hand_Total) // DRAW
	{
		BlankLine(1);
		std::cout << ">>> DRAW <<<" << std::endl;
		BlankLine(1);

		_player.st_Wallet += _plr_bet;

		std::cout << ">>> +$" << _plr_bet << " | PLAYER BET RETURNED" << std::endl;
		BlankLine(1);
	}
	else if (_player.st_Hand_Total > 21 && _dealer.st_Hand_Total > 21) // BOTH BUST
	{
		double tmp_plr_hand = (_player.st_Hand_Total - 21);
		double tmp_dlr_hand = (_dealer.st_Hand_Total - 21);

		if (tmp_plr_hand > tmp_dlr_hand) // DEALER CLOSEST TO 21
		{
			BlankLine(1);
			std::cout << ">>> DEALER WINS | CLOSEST TO 21 <<<" << std::endl;
			BlankLine(1);
			std::cout << ">>> -$" << _plr_bet << " | PLAYER BET LOST" << std::endl;
			BlankLine(1);

			_dealer.st_Wallet += _plr_bet;
		}
		else if (tmp_plr_hand < tmp_dlr_hand) // PLAYER CLOSEST TO 21
		{
			BlankLine(1);
			std::cout << ">>> PLAYER WINS | CLOSEST TO 21 <<<" << std::endl;
			BlankLine(1);

			_player.st_Wallet += (_plr_bet * 2);

			std::cout << ">>> +$" << _plr_bet << " | PLAYER BET DOUBLED" << std::endl;
			BlankLine(1);
		}
	}
	else if (((_player.st_Hand_Total > _dealer.st_Hand_Total && _player.st_Hand_Total <= 21) || (_dealer.st_Hand_Total > 21)) && _player.st_Hand_Total <= 21) // PLAYER TOTAL IS MORE THAN DEALER
	{
		BlankLine(1);
		std::cout << ">>> PLAYER WINS <<<" << std::endl;
		BlankLine(1);

		_player.st_Wallet += (_plr_bet * 2);

		std::cout << ">>> +$" << _plr_bet << " | PLAYER BET DOUBLED" << std::endl;
		BlankLine(1);
	}

	Sleep(1500);
	std::cout << ">>> NEW WALLET BALANCE: $" << _player.st_Wallet << std::endl;
	Sleep(1500);

}

void GameSet(Card* _deck, Player &_player, Player &_dealer, int &_deck_top, bool &_hit_or_stay, bool &_dlr_bj, bool &_plr_bj, double &_plr_bet)
{
	_player = { _player.st_Wallet,0,0,0 };
	_dealer = { _dealer.st_Wallet,0,0,0 };
	_deck[52] = { 0 };

	_deck_top = 0;

	_plr_bet = NULL;
	_plr_bj = false;
	_dlr_bj = false;
	_deck_top = 0;
	_hit_or_stay = true;
}

void BlankLine(int _lines)
{
	for (int i = 0; i <= _lines; i++)
	{
		std::cout << std::endl;
	}
}

void InvalidInput()
{
	BlankLine(1);
	std::cout << ">>> INVALID INPUT <<<" << std::endl;
	BlankLine(1);
	Sleep(1500);
}

void Exitprgrm()
{
	BlankLine(1);
	std::cout << ">>> EXIT GAME <<<" << std::endl;
	Sleep(1000);
	BlankLine(1);
	std::cout << "THANK YOU FOR PLAYING" << std::endl;
	Sleep(1000);
	std::cout << "EXITING IN";
	Sleep(1000);
	std::cout << "...";
	Sleep(1000);
	std::cout << "3...";
	Sleep(1000);
	std::cout << "2...";
	Sleep(1000);
	std::cout << "1...";
	Sleep(1000);
	exit(0);
}
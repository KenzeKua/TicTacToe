#include <GLFW/glfw3.h>

#include "TicTacToe.h"

TicTacToe::TicTacToe()
{
	BoardSprite = new Sprite("../media/TicTacToe.bmp");
	CrossSprite = new Sprite("../media/Cross.bmp");
	CircleSprite = new Sprite("../media/Circle.bmp");
	RestartSprite = new Sprite("../media/Restart.bmp");
	TurnSprite = new Sprite("../media/Turn.bmp");
	EmptySprite = new Sprite("../media/Empty.bmp");

	MainBoard = GameObject(BoardSprite);
	RestartButton = GameObject(RestartSprite);
	TurnSign = GameObject(TurnSprite);
	TurnSymbol = GameObject(EmptySprite);
	CrossObject = GameObject(CrossSprite);
	CircleObject = GameObject(CircleSprite);

	for (int i = 0; i < 9; i++)
	{
		BoardLocation[i] = 'a';
		BoardSpace[i] = GameObject(EmptySprite);
	}

	PlayerSymbol[0] = 'X';
	PlayerSymbol[1] = 'O';
}

TicTacToe::~TicTacToe()
{
}

void TicTacToe::MarkPlayer(int PlayerNumber)
{
	if (this->PlayerNumber == 0)
	{
		if (PlayerNumber < 3)
		{
			this->PlayerNumber = PlayerNumber;

			if (this->PlayerNumber == 1)
				PlayerTurn = true;

			std::cout << "You are Player " << this->PlayerNumber << ". Your symbol is " << PlayerSymbol[this->PlayerNumber - 1] << ".\n";
		}
		else
		{
			this->PlayerNumber = PlayerNumber;
			std::cout << "You are spectating as Player " << this->PlayerNumber << ".\n";
		}
	}
}

void TicTacToe::InputAnswer(int PlayerAnswer)
{
	int InputCheckWin = -1;

	if (PlayerNumber < 3)
	{
		if (PlayerAnswer == 10)
		{
			std::cout << "Player " << this->PlayerNumber << " forcefully resets the board.\n";
			ResetBoard();
		}
		else if (PlayerTurn)
		{
			if (BoardLocation[PlayerAnswer] == PlayerSymbol[0] || BoardLocation[PlayerAnswer] == PlayerSymbol[1])
				std::cout << "Space is taken. Please choose another one.\n";
			else if (PlayerAnswer < 0 || PlayerAnswer > 8)
				std::cout << "This is not a space.\n";
			else
			{
				BoardLocation[PlayerAnswer] = PlayerSymbol[PlayerNumber - 1];
				PlayerTurn = false;
				InputCheckWin = CheckWin();
				if (InputCheckWin == 1)
				{
					std::cout << "Local: Player 1 won" << std::endl;
					Player1Score++;
					ResetBoard();
				}
				else if (InputCheckWin == 2)
				{
					std::cout << "Local: Player 2 won" << std::endl;
					Player2Score++;
					ResetBoard();
				}
				else if (InputCheckWin == 3)
				{
					std::cout << "Local: Game draw" << std::endl;
					ResetBoard();
				}
			}
		}
		else
		{
			if (PlayerNumber == 1)
				std::cout << "It is currently Player 2's turn\n";
			else
				std::cout << "It is currently Player 1's turn\n";
		}
	}
	else
	{
		std::cout << "You are Player " << PlayerNumber << ", please don't disturb the match.";
	}
}

void TicTacToe::FetchAnswer(int PlayerNumber, int PlayerAnswer)
{
	int FetchCheckWin = -1;

	if (PlayerNumber < 3)
	{
		if (PlayerAnswer == 10)
		{
			std::cout << "Player " << PlayerNumber << " forcefully resets the board.\n";
			ResetBoard();
		}
		else if (!PlayerTurn)
		{
			if (BoardLocation[PlayerAnswer] == PlayerSymbol[0] || BoardLocation[PlayerAnswer] == PlayerSymbol[1])
				std::cout << "Player " << PlayerNumber << " is blind and wants to draw on a taken space.\n";
			else if (PlayerAnswer < 0 || PlayerAnswer > 8)
				std::cout << "Player " << PlayerNumber << " wanted to cheat by drawing outside the box.\n";
			else
			{
				BoardLocation[PlayerAnswer] = PlayerSymbol[PlayerNumber - 1];
				PlayerTurn = true;
				FetchCheckWin = CheckWin();
				if (FetchCheckWin == 1)
				{
					std::cout << "Received: Player 1 won" << std::endl;
					Player1Score++;
					ResetBoard();
				}
				else if (FetchCheckWin == 2)
				{
					std::cout << "Received: Player 2 won" << std::endl;
					Player2Score++;
					ResetBoard();
				}
				else if (FetchCheckWin == 3)
				{
					std::cout << "Received: Game draw" << std::endl;
					ResetBoard();
				}
			}
		}
		else
		{
			std::cout << "Player " << PlayerNumber << " tried to draw while Player " << this->PlayerNumber << " was obviously looking.\n";
		}
	}
	else
		std::cout << "Spectator " << PlayerNumber << " tried to interrupt a game.\n";
}

int TicTacToe::CheckWin()
{
	// Horizontal check
	for (int i = 0; i < 3; i++)
	{
		if (BoardLocation[i * 3] == BoardLocation[1 + (i * 3)] && 
			BoardLocation[1 + (i * 3)] == BoardLocation[2 + (i * 3)])
		{
			if (BoardLocation[i * 3] == 'X')
				return 1;
			else if (BoardLocation[i * 3] == 'O')
				return 2;
			else
				continue;
		}
	}

	// Vertical check
	for (int i = 0; i < 3; i++)
	{
		if (BoardLocation[i] == BoardLocation[i + 3] && 
			BoardLocation[i + 3] == BoardLocation[i + 6])
		{
			if (BoardLocation[i] == 'X')
				return 1;
			else if (BoardLocation[i] == 'O')
				return 2;
			else
				continue;
		}
	}

	// Diagonal check
	if (BoardLocation[0] == BoardLocation[4] && 
		BoardLocation[4] == BoardLocation[8])
	{
		if (BoardLocation[0] == 'X')
			return 1;
		else if (BoardLocation[0] == 'O')
			return 2;
		else { }
	}
	else if (BoardLocation[2] == BoardLocation[4] && 
			 BoardLocation[4] == BoardLocation[6])
	{
		if (BoardLocation[2] == 'X')
			return 1;
		else if (BoardLocation[2] == 'O')
			return 2;
		else { }
	}

	// Check if board is full
	for (int i = 0; i < 9; i++)
	{
		if (BoardLocation[i] == 'a')
		{
			return 0;
		}
	}

	return 3;
}

void TicTacToe::ResetBoard()
{
	std::cout << "Resetted board\n";
	std::cout << "Current score: Player 1 = " << Player1Score << " , Player 2 = " << Player2Score << "\n";

	for (int i = 0; i < 9; i++)
	{
		BoardLocation[i] = 'a';
	}

	if (PlayerNumber == 1)
		PlayerTurn = true;
	else if (PlayerNumber == 2)
		PlayerTurn = false;
}

void TicTacToe::DrawBoard(double DeltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	MainBoard.setTexture(BoardSprite->m_defaultTexture);
	MainBoard.setPosition(640.0, 360.0);
	MainBoard.setScale(true, 1.0, 1.0);
	MainBoard.setColor(Color::WHITE);
	MainBoard.draw(ADDITIVE);

	RestartButton.setTexture(RestartSprite->m_defaultTexture);
	RestartButton.setPosition(55.0, 670.0);
	RestartButton.setScale(false, 0.7, 0.7);
	RestartButton.draw(ADDITIVE);

	TurnSign.setTexture(TurnSprite->m_defaultTexture);
	TurnSign.setPosition(114.5, 450.0);
	TurnSign.setScale(true, 1.0, 1.0);
	TurnSign.draw(ADDITIVE);

	if (PlayerNumber == 1)
	{
		if (PlayerTurn) 
			TurnSymbol.setTexture(CrossSprite->m_defaultTexture);
		else 
			TurnSymbol.setTexture(CircleSprite->m_defaultTexture);
	}
	else if (PlayerNumber == 2)
	{
		if (PlayerTurn) 
			TurnSymbol.setTexture(CircleSprite->m_defaultTexture);
		else 
			TurnSymbol.setTexture(CrossSprite->m_defaultTexture);
	}
	TurnSymbol.setPosition(114.5, 402.5);
	TurnSymbol.setScale(true, 1.0, 1.0);
	TurnSymbol.draw(ADDITIVE);

	// 210 pixels apart
	BoardSpace[0].setPosition(430.0, 570.0);
	BoardSpace[1].setPosition(640.0, 570.0);
	BoardSpace[2].setPosition(850.0, 570.0);

	BoardSpace[3].setPosition(430.0, 360.0);
	BoardSpace[4].setPosition(640.0, 360.0);
	BoardSpace[5].setPosition(850.0, 360.0);

	BoardSpace[6].setPosition(430.0, 150.0);
	BoardSpace[7].setPosition(640.0, 150.0);
	BoardSpace[8].setPosition(850.0, 150.0);

	for (int i = 0; i < 9; i++)
	{
		if (BoardLocation[i] == 'X') 
			BoardSpace[i].setTexture(CrossSprite->m_defaultTexture);
		else if (BoardLocation[i] == 'O') 
			BoardSpace[i].setTexture(CircleSprite->m_defaultTexture);
		else 
			BoardSpace[i].setTexture(EmptySprite->m_defaultTexture);

		BoardSpace[i].setScale(false, 1.7, 1.7);
		BoardSpace[i].draw(ADDITIVE);
	}
}
#ifndef TIC_TAC_TOE_H
#define TIC_TAC_TOE_H

#include <iostream>

#include "Sprite.h"
#include "GameObject.h"

class TicTacToe
{
private:
	Sprite* BoardSprite;
	Sprite* CrossSprite;
	Sprite* CircleSprite;
	Sprite* RestartSprite;
	Sprite* TurnSprite;
	Sprite* EmptySprite;

	GameObject MainBoard;
	GameObject RestartButton;
	GameObject TurnSign;
	GameObject TurnSymbol;
	GameObject BoardSpace[9];
	GameObject CrossObject;
	GameObject CircleObject;

	char BoardLocation[9];
	//char BoardLocation[9] = { 'X', 'X', 'O', 'O', 'O', 'X', 'X', 'O', 'X' };
	int Player1Score = 0;
	int Player2Score = 0;

	char PlayerSymbol[2] = "a";
	bool PlayerTurn = false;

public:
	TicTacToe();
	~TicTacToe();

	int PlayerNumber = 0;

	void MarkPlayer(int PlayerNumber);
	void InputAnswer(int PlayerAnswer);
	void FetchAnswer(int PlayerNumber, int PlayerAnswer);
	int CheckWin();

	void ResetBoard();
	void DrawBoard(double DeltaTime);
};

#endif
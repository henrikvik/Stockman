#ifndef ENUMS_H
#define ENUMS_H

#pragma region Comment

/*
	Author:

	This header contains different Enums

*/

#pragma endregion Description


namespace Logic
{
	enum GameState
	{
		gameStateGame,
		gameStateLoading,
		gameStateMenuMain,
		gameStateMenuSettings,
		gameStateGameOver,
		gameStateDefault,
		gameStateGameUpgrade,
		gameStateMenuSettingsVideo,
		gameStateMenuSettingsSound,
		gameStateMenuSettingsControls,
		gameStateMenuSettingsOther,
	};

	enum CardChoices
	{
		choice1,
		choice2,
		choice3
	};
}

#endif // !ENUMS_H
void writeToScreen() {
	//write to screen
}

int getButtons() {
	//get buttonstatus
	return -1;
}

void update_boxPlacement(int * boxPlacement, int boxPlacement_size) {
	float maybeAddNewBox;
	
	//update screen
	for (int i = 0; i < boxPlacement_size-1; i++) {
		boxPlacement[i] = boxPlacement[i + 1];
	}

	//Maybe add a new box if it fits
	if (boxPlacement[boxPlacement_size - 2] == 0 && boxPlacement[boxPlacement_size - 3] == 0 && boxPlacement[boxPlacement_size - 4] == 0) {
		maybeAddNewBox = rand();
		if (maybeAddNewBox > 0.5) {
			boxPlacement[boxPlacement_size - 1] = 1;
		}
		else {
			boxPlacement[boxPlacement_size - 1] = 0;
		}
	}
}

void update_ballPlacement(int * ballPlacement, int ballPlacement_size, int button, int * ballDirection) {

	switch (*ballDirection) {
	case 0:
		if (button = 1) {
			//Start a jump
			ballPlacement[0] = 0;
			ballPlacement[1] = 1;
			*ballDirection = 1;
		}
		break;
	case 1:
		//Change direction if ball is at the top point
		if (ballPlacement[ballPlacement_size - 1] == 1) {
			*ballDirection = -1;
		}
		else {
			//increase the balls higth
			for (int i = 0; i < ballPlacement_size; i++) {
				ballPlacement[i + 1] = ballPlacement[i];
			}
		}
		break;
	case -1:
		//stop the jump if the ball have reached the button
		if (ballPlacement[0] == 1) {
			*ballDirection = 0;
		}
		else {
			//decreace the balls higth
			for (int j = ballPlacement_size; j > 0; j--) {
				ballPlacement[j - 1] = ballPlacement[j];
			}
		}
		break;
	}
}

void initialize_boxPlacement(int * boxPlacement, int boxPlacement_size) {
	for (int i = 0; i < boxPlacement_size; i++) {
		boxPlacement[i] = 0;
	}
}

void initialize_ballPlacement(int * ballPlacement, int ballPlacement_size, int * ballDirection) {
	ballPlacement[0] = 1;
	*ballDirection = 0;

	for (int i = 1; i < ballPlacement_size; i++) {
		ballPlacement[i] = 0;
	}
}

void update_screen(int * boxPlacement, int * ballPlacement) {
	//Antar at screen er array med pixler
	
	int const screenLength = 6;
	int	const screenWidth = 5;

	int screen[screenLength*screenWidth];

	//Initialize screen array
	for (int i = 0; i < screenLength*screenWidth; i++) {
		screen[i] = 0;
	}

	//Add box placement
	for (int j = 0; j < screenLength; j++) {
		screen[(screenWidth*screenLength - screenLength) + j] = boxPlacement[j];
	}

	//Add ball placement
	for (int k = 0; k < screenWidth; k++) {
		screen[screenLength*k] = boxPlacement[screenWidth-1-k];

	}

	//Write to screen
	writeToScreen();
}

void run_game(bool * screen) {
	int const screenLength = 6;
	int	const screenWidth = 5;

	int boxPlacement[screenLength];
	int ballPlacement[screenWidth];

	int ballDirection = 0;
	int * p_ballDirection = &ballDirection;

	int button;
	int gameover = 0;

	initialize_boxPlacement(boxPlacement, screenLength);
	initialize_ballPlacement(ballPlacement, screenWidth, p_ballDirection);


	while (gameover == 0) {
		button = getButtons();

		update_boxPlacement(boxPlacement, screenLength);
		update_ballPlacement(ballPlacement, screenWidth, button, p_ballDirection);

		if (boxPlacement[0] == true && ballPlacement[0] == true) {
			gameover = 1;
		}
		else {
			update_screen(boxPlacement, ballPlacement);
		}
	}
}

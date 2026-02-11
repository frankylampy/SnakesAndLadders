#include<iostream>
#include<map>
#include<vector>
#include<memory>

// Configuration for ladders and snakes on the game board (for example)
std::map<int, int> GameLadderAndSnakeConfigs = {
    {2, 38},
    {7, 14},
    {8, 31},
    {15, 26},
    {21, 42},
    {28, 84},
    {36, 44},
    {51, 67},
    {71, 91},
    {78, 98},

    // Snakes (high -> low)
    {16, 6},
    {46, 25},
    {49, 11},
    {62, 19},
    {64, 60},
    {74, 53},
    {89, 68},
    {92, 88},
    {95, 75},
    {99, 80}
};

// Enum to represent the type of step (None or Ladder or Snake for each position on the game board)
enum class StepType {
    None,
    Ladder,
    Snake
};

// Abstract base class to represent a game element (Ladder or Snake)
class GameElement {
    private:
    int lowerBound = 0;
    int upperBound = 0;
    public:
    const int GetLowerBound() const {
        return lowerBound;
    }
    const int GetUpperBound() const {
        return upperBound;
    }
    GameElement(int lowerBound, int upperBound) : lowerBound(lowerBound), upperBound(upperBound) {}
    virtual int ComputeNewPosition() = 0;
    virtual ~GameElement() = default;
};

class Ladder : public GameElement {
    public:
    Ladder(int lowerBound, int upperBound) : GameElement(lowerBound, upperBound) {}
    int ComputeNewPosition() override {
            // Logic to compute new position when climbing a ladder
            return GetUpperBound(); 
    }
};

class Snake : public GameElement {
    public:
    Snake(int lowerBound, int upperBound) : GameElement(lowerBound, upperBound) {}
    int ComputeNewPosition() override {
        // Logic to compute new position when sliding down a snake
        return GetLowerBound(); 
    }
};

// Configuration for each position on the game board
struct CurrentPositionConfig {
    std::unique_ptr<GameElement> ladder;
    std::unique_ptr<GameElement> snake;
    void DefineLadderAndSnakeAtCurrentPosition(StepType stepType = StepType::None, int lBound = 0, int uBound = 0) {
        if (stepType == StepType::Ladder) {
            ladder = std::make_unique<Ladder>(lBound, uBound);
        } else if (stepType == StepType::Snake) {
            snake = std::make_unique<Snake>(lBound, uBound);
        }
    }
    ~CurrentPositionConfig() = default;
};

// Function to create the game board configuration based on the defined ladders and snakes
std::vector<CurrentPositionConfig> CreateGameBoard(int boardSize) {
    std::vector<CurrentPositionConfig> gameConfig = std::vector<CurrentPositionConfig>(boardSize);
    for (int ii = 0; ii < boardSize; ++ii) {
        auto it = GameLadderAndSnakeConfigs.find(ii); // Find if there's a ladder or snake at this position
        if (it != GameLadderAndSnakeConfigs.end()) {
            if(ii < it->second) {
                //Ladder Configuration
                gameConfig[ii].DefineLadderAndSnakeAtCurrentPosition(StepType::Ladder, ii, it->second);
            } else {
                // Snake Configuration
                gameConfig[ii].DefineLadderAndSnakeAtCurrentPosition(StepType::Snake, it->second, ii);
            }
            
        } else {
            // No ladder or snake at this position
            gameConfig[ii].DefineLadderAndSnakeAtCurrentPosition();
        }
    }
    return gameConfig;
}

int RollDice() {
    // Return a random number between 1 and 6)
    return rand() % 6 + 1;
}

// Program entry point
void main() {
    int boardSize = 100;
    std::vector<CurrentPositionConfig> gameBoard = CreateGameBoard(boardSize);
    int playerPosition = 0; // Starting position of the player
    while(true) {
        int currRoll = RollDice();
        playerPosition += currRoll;
        std::cout << "\nPlayer rolled: " << currRoll << ", new position: " << playerPosition << std::endl;
        if (playerPosition >= boardSize)
        {
            break;
        }
        
        // Check for ladder or snake at the new position and update player's position accordingly
        if (gameBoard[playerPosition].ladder != nullptr) {
            playerPosition = gameBoard[playerPosition].ladder->ComputeNewPosition();
            std::cout << "New position after Ladder: " << playerPosition << std::endl;
        } else if (gameBoard[playerPosition].snake != nullptr) {
            playerPosition = gameBoard[playerPosition].snake->ComputeNewPosition();
            std::cout << "New position after Snake: " << playerPosition << std::endl;
        }
        
    }
    std::cout << "\nPlayer Wins" << std::endl;    
}
